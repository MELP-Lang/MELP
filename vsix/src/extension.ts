import * as vscode from 'vscode';
import * as os from 'os';
import * as path from 'path';
import * as fs from 'fs';
import {
  LanguageClient,
  LanguageClientOptions,
  ServerOptions,
  TransportKind,
} from 'vscode-languageclient/node';
import { MlpDiagnosticProvider } from './mlpDiagnosticProvider';
import { HELP_TOPICS, MlpCodeActionProvider } from './mlpCodeActionProvider';
import { MlpFunctionsViewProvider } from './mlpFunctionsViewProvider';
import { MelpDesignerProvider } from './melpDesignerProvider';
import { CompilerDiagnostic, MlpCompilationError, compileMlpProgram, getBundledRuntimeBitcodePath } from './mlpCompiler';
import { MlpguiCodeEditorBridge } from './mlpguiCodeEditorBridge';

// ── Yardımcılar ──────────────────────────────────────────────────────────────

function getCompilerPath(extensionPath?: string): string {
  const configuredPath = vscode.workspace.getConfiguration('mlp').get<string>('compilerPath', '').trim();
  if (configuredPath) {
    return configuredPath;
  }

  if (extensionPath) {
    const platformSuffix = process.platform === 'win32'
      ? '_windows.exe'
      : process.platform === 'darwin'
        ? '_macos'
        : '_linux';
    const bundledCompiler = path.join(extensionPath, 'bin', `melp_compiler${platformSuffix}`);
    if (fs.existsSync(bundledCompiler)) {
      return bundledCompiler;
    }
  }

  return 'melp_compiler';
}

function getRuntimeBitcodePath(extensionPath?: string): string | undefined {
  const configuredPath = vscode.workspace.getConfiguration('mlp').get<string>('runtimeBitcodePath', '').trim();
  if (configuredPath && fs.existsSync(configuredPath)) {
    return configuredPath;
  }
  return extensionPath ? getBundledRuntimeBitcodePath(extensionPath) : undefined;
}

function getNormalizerPath(extensionPath?: string): string {
  const configuredPath = vscode.workspace.getConfiguration('mlp').get<string>('normalizerPath', '').trim();
  if (configuredPath) {
    return configuredPath;
  }

  if (extensionPath) {
    const bundledNormalizer = path.join(extensionPath, 'bin', 'melp_normalizer_bin');
    if (fs.existsSync(bundledNormalizer)) {
      return bundledNormalizer;
    }
  }

  return 'melp_normalizer_bin';
}

function findNodeExecutable(): string {
  // VS Code / VSCodium'un gömülü Node.js'ini bul
  // process.execPath bazen electron/vscodium binary'sini gösterir — gerçek node'u ara
  const execPath = process.execPath;
  const execName = path.basename(execPath).toLowerCase();

  // Zaten node ise direkt kullan
  if (execName === 'node' || execName === 'node.exe') {
    return execPath;
  }

  // VS Code / VSCodium gömülü node: binary'nin yanında veya üst dizinde
  const candidates = [
    path.join(path.dirname(execPath), 'node'),
    path.join(path.dirname(execPath), '..', 'node'),
    path.join(path.dirname(execPath), 'resources', 'app', 'node_modules', '.bin', 'node'),
  ];
  for (const c of candidates) {
    if (fs.existsSync(c)) { return c; }
  }

  // PATH'teki node
  return 'node';
}

function getLspPath(): { command: string; args: string[] } | null {
  // 1. VSIX içinde paketlenmiş bundle (bin/melp_lsp.js) — her kullanıcıda çalışır
  const bundled = path.join(__dirname, '..', 'bin', 'melp_lsp.js');
  if (fs.existsSync(bundled)) {
    return { command: findNodeExecutable(), args: [bundled] };
  }

  // 2. Geliştirme ortamı: shared/lsp/melp_lsp (native binary)
  const sharedBin = path.join(__dirname, '..', '..', 'shared', 'lsp', 'melp_lsp');
  if (fs.existsSync(sharedBin)) {
    return { command: sharedBin, args: [] };
  }

  // 3. Geliştirme: ORTAK/LSP/LSP_EN dist/server.js
  const lspDist = path.resolve(__dirname, '../../../ORTAK/LSP/LSP_EN/dist/server.js');
  if (fs.existsSync(lspDist)) {
    return { command: findNodeExecutable(), args: [lspDist] };
  }

  return null;
}

function getDapPath(): string {
  const shared = path.join(__dirname, '..', '..', 'shared', 'dap', 'melp_debugger');
  if (fs.existsSync(shared)) {
    return shared;
  }
  const dev = path.resolve(__dirname, '../../../../shared/dap/melp_debugger');
  if (fs.existsSync(dev)) {
    return dev;
  }
  return '/home/pardus/projeler/MLP/editors/shared/dap/melp_debugger';
}

function getTmpPath(ext: string): string {
  return path.join(os.tmpdir(), `melp_run_${Date.now()}${ext}`);
}

function getCompilerArgs(): string[] {
  return vscode.workspace.getConfiguration('mlp').get<string[]>('compilerArgs', [])
    .filter((arg): arg is string => typeof arg === 'string' && arg.trim().length > 0);
}

function quoteShellArg(value: string): string {
  return `"${value.replace(/"/g, '\\"')}"`;
}

function createDiagnosticRange(document: vscode.TextDocument | undefined, entry: CompilerDiagnostic): vscode.Range {
  const lineIndex = Math.max((entry.line ?? 1) - 1, 0);
  const columnIndex = Math.max((entry.column ?? 1) - 1, 0);

  if (!document || document.lineCount === 0) {
    return new vscode.Range(lineIndex, columnIndex, lineIndex, columnIndex);
  }

  const safeLine = Math.min(lineIndex, document.lineCount - 1);
  const line = document.lineAt(safeLine);
  const safeColumn = Math.min(columnIndex, line.text.length);
  const endColumn = entry.column
    ? Math.min(safeColumn + 1, Math.max(line.text.length, safeColumn + 1))
    : Math.max(line.text.length, safeColumn + 1);

  return new vscode.Range(safeLine, safeColumn, safeLine, endColumn);
}

function toCompilerDiagnostic(document: vscode.TextDocument | undefined, entry: CompilerDiagnostic): vscode.Diagnostic {
  const diagnostic = new vscode.Diagnostic(
    createDiagnosticRange(document, entry),
    entry.message,
    entry.severity === 'warning' ? vscode.DiagnosticSeverity.Warning : vscode.DiagnosticSeverity.Error
  );
  diagnostic.source = 'MLP Compiler';
  return diagnostic;
}

// ── Aktivasyon ────────────────────────────────────────────────────────────────

let lspClient: LanguageClient | undefined;

export function activate(context: vscode.ExtensionContext): void {
  const codeEditorBridge = new MlpguiCodeEditorBridge();
  const functionsProvider = new MlpFunctionsViewProvider(context);
  const diagnosticProvider = new MlpDiagnosticProvider(context.extensionPath);
  const compilerDiagnosticCollection = vscode.languages.createDiagnosticCollection('mlp-compiler');
  const designerProvider = new MelpDesignerProvider(context, codeEditorBridge);
  const designerViewTypes = ['mlp.melpDesigner', 'mlp-ide.melpDesigner'];
  const outputChannel = vscode.window.createOutputChannel('MLP');

  context.subscriptions.push(outputChannel);
  context.subscriptions.push(compilerDiagnosticCollection);

  // ── LSP Client ────────────────────────────────────────────────────────────
  const lspExec = getLspPath();
  if (lspExec) {
    const serverOptions: ServerOptions = {
      command: lspExec.command,
      args: lspExec.args,
      transport: TransportKind.stdio,
    };
    const clientOptions: LanguageClientOptions = {
      documentSelector: [{ scheme: 'file', language: 'mlp' }],
      synchronize: {
        fileEvents: vscode.workspace.createFileSystemWatcher('**/*.mlp'),
      },
      outputChannel,
    };
    lspClient = new LanguageClient('melp-lsp', 'MeLP Language Server', serverOptions, clientOptions);
    context.subscriptions.push(lspClient);
    void lspClient.start();
    outputChannel.appendLine(`[LSP] Başlatıldı: ${lspExec.command} ${lspExec.args.join(' ')}`);
  } else {
    outputChannel.appendLine('[LSP] melp_lsp bulunamadı — yerleşik provider\'lar kullanılıyor.');
  }

  const clearCompilerDiagnostics = (filePath?: string): void => {
    if (!filePath) {
      return;
    }

    compilerDiagnosticCollection.delete(vscode.Uri.file(filePath));
  };

  const publishCompilerDiagnostics = (sourcePath: string, error: unknown): void => {
    if (!(error instanceof MlpCompilationError)) {
      compilerDiagnosticCollection.set(vscode.Uri.file(sourcePath), [
        new vscode.Diagnostic(
          new vscode.Range(0, 0, 0, 0),
          error instanceof Error ? error.message : String(error),
          vscode.DiagnosticSeverity.Error
        ),
      ]);
      return;
    }

    const diagnosticEntries = error.diagnostics.length > 0
      ? error.diagnostics
      : [{ filePath: sourcePath, severity: 'error' as const, message: error.message, raw: error.output }];

    const diagnosticsByFile = new Map<string, vscode.Diagnostic[]>();

    for (const entry of diagnosticEntries) {
      const targetPath = path.resolve(entry.filePath ?? sourcePath);
      const targetUri = vscode.Uri.file(targetPath);
      const targetDocument = vscode.workspace.textDocuments.find(document => document.uri.fsPath === targetUri.fsPath);
      const existingDiagnostics = diagnosticsByFile.get(targetUri.fsPath) ?? [];
      existingDiagnostics.push(toCompilerDiagnostic(targetDocument, entry));
      diagnosticsByFile.set(targetUri.fsPath, existingDiagnostics);
    }

    compilerDiagnosticCollection.clear();
    compilerDiagnosticCollection.set(
      [...diagnosticsByFile.entries()].map(([targetPath, diagnostics]) => [vscode.Uri.file(targetPath), diagnostics])
    );
  };

  const showCompilationError = (title: string, error: unknown): void => {
    const detail = error instanceof Error ? error.message : String(error);
    outputChannel.appendLine(`[${new Date().toISOString()}] ${title}`);
    outputChannel.appendLine(detail);
    outputChannel.show(true);
    void vscode.window.showErrorMessage(title);
  };

  // LSP yoksa VSIX completion provider'ı devreye girer
  // LSP varken tamamen devre dışı — çakışma ve bozuk tab-stop önlemek için
  if (!lspClient) {
    const { MlpCompletionProvider } = require('./mlpCompletionProvider');
    context.subscriptions.push(
      vscode.languages.registerCompletionItemProvider(
        { language: 'mlp', scheme: 'file' },
        new MlpCompletionProvider(context, false),
      )
    );
  }

  // LSP yoksa hover + diagnostics provider'ları da kullan
  if (!lspClient) {
    const { MlpHoverProvider } = require('./mlpHoverProvider');
    context.subscriptions.push(
      vscode.languages.registerHoverProvider(
        { language: 'mlp', scheme: 'file' },
        new MlpHoverProvider(context)
      )
    );
    diagnosticProvider.analyzeAll();
    context.subscriptions.push(
      diagnosticProvider.collection,
      vscode.workspace.onDidOpenTextDocument(document => diagnosticProvider.analyze(document)),
      vscode.workspace.onDidChangeTextDocument(event => {
        diagnosticProvider.analyze(event.document);
        clearCompilerDiagnostics(event.document.uri.fsPath);
      }),
      vscode.workspace.onDidCloseTextDocument(document => {
        diagnosticProvider.collection.delete(document.uri);
        clearCompilerDiagnostics(document.uri.fsPath);
      })
    );
  } else {
    // LSP varken sadece derleme hatası temizliği için change dinle
    context.subscriptions.push(
      vscode.workspace.onDidChangeTextDocument(event => {
        clearCompilerDiagnostics(event.document.uri.fsPath);
      }),
      vscode.workspace.onDidCloseTextDocument(document => {
        clearCompilerDiagnostics(document.uri.fsPath);
      })
    );
  }

  context.subscriptions.push(
    vscode.languages.registerCodeActionsProvider(
      { language: 'mlp', scheme: 'file' },
      new MlpCodeActionProvider(),
      { providedCodeActionKinds: MlpCodeActionProvider.providedCodeActionKinds }
    ),
    vscode.window.registerTreeDataProvider('mlp.functionsView', functionsProvider),
    vscode.workspace.onDidSaveTextDocument(document => {
      if (!codeEditorBridge.isCodeEditorDocument(document)) {
        return;
      }
      void codeEditorBridge.syncGuiFromCodeDocument(document);
    })
  );

  context.subscriptions.push(
    ...designerViewTypes.map(viewType => vscode.window.registerCustomEditorProvider(viewType, designerProvider, {
      webviewOptions: { retainContextWhenHidden: true },
      supportsMultipleEditorsPerDocument: false,
    }))
  );

  // ── Çalıştır (F5) ─────────────────────────────────────────────────────────
  context.subscriptions.push(
    vscode.commands.registerCommand('mlp.runFile', async (uri?: vscode.Uri) => {
      const filePath = uri?.fsPath ?? vscode.window.activeTextEditor?.document.fileName;
      if (!filePath || !filePath.endsWith('.mlp')) {
        vscode.window.showErrorMessage('Çalıştırılacak .mlp dosyası bulunamadı.');
        return;
      }

      const compiler = getCompilerPath(context.extensionPath);
      const normalizer = getNormalizerPath(context.extensionPath);
      const runtimeBitcodePath = getRuntimeBitcodePath(context.extensionPath);
      const compilerArgs = getCompilerArgs();
      const langExt = filePath.endsWith('.tr.mlp') ? '.tr.mlp'
        : filePath.endsWith('.ru.mlp') ? '.ru.mlp'
        : filePath.endsWith('.ar.mlp') ? '.ar.mlp'
        : filePath.endsWith('.zh.mlp') ? '.zh.mlp'
        : '.mlp';
      const tmpSrc = getTmpPath(langExt);
      const tmpBin = getTmpPath('');

      fs.copyFileSync(filePath, tmpSrc);
      clearCompilerDiagnostics(filePath);

      try {
        await compileMlpProgram({
          compilerPath: compiler,
          normalizerPath: normalizer,
          sourcePath: tmpSrc,
          outputPath: tmpBin,
          runtimeBitcodePath,
          compilerArgs,
          extensionPath: context.extensionPath,
        });
      } catch (error) {
        publishCompilerDiagnostics(filePath, error);
        showCompilationError('MLP dosyasi derlenemedi.', error);
        fs.rmSync(tmpSrc, { force: true });
        fs.rmSync(tmpBin, { force: true });
        return;
      }

      let terminal = vscode.window.terminals.find(t => t.name === 'MLP Çalıştır');
      if (!terminal) {
        terminal = vscode.window.createTerminal('MLP Çalıştır');
      }
      terminal.show(true);
      terminal.sendText(
        `${quoteShellArg(tmpBin)} ; rm -f ${quoteShellArg(tmpSrc)} ${quoteShellArg(tmpBin)}`
      );
    })
  );

  // ── Derle ─────────────────────────────────────────────────────────────────
  context.subscriptions.push(
    vscode.commands.registerCommand('mlp.buildFile', async (uri?: vscode.Uri) => {
      const filePath = uri?.fsPath ?? vscode.window.activeTextEditor?.document.fileName;
      if (!filePath || !filePath.endsWith('.mlp')) {
        vscode.window.showErrorMessage('Derlenecek .mlp dosyası bulunamadı.');
        return;
      }

      const compiler = getCompilerPath(context.extensionPath);
      const normalizer = getNormalizerPath(context.extensionPath);
      const runtimeBitcodePath = getRuntimeBitcodePath(context.extensionPath);
      const compilerArgs = getCompilerArgs();
      const outBin = filePath.replace(/\.mlp$/, '');
      clearCompilerDiagnostics(filePath);

      try {
        await compileMlpProgram({
          compilerPath: compiler,
          normalizerPath: normalizer,
          sourcePath: filePath,
          outputPath: outBin,
          runtimeBitcodePath,
          compilerArgs,
          extensionPath: context.extensionPath,
        });
        clearCompilerDiagnostics(filePath);
        void vscode.window.showInformationMessage(`Derleme tamamlandi: ${outBin}`);
      } catch (error) {
        publishCompilerDiagnostics(filePath, error);
        showCompilationError('MLP dosyasi derlenemedi.', error);
      }
    })
  );

  // ── Debug Et ──────────────────────────────────────────────────────────────
  context.subscriptions.push(
    vscode.debug.registerDebugConfigurationProvider('mlp', {
      async resolveDebugConfigurationWithSubstitutedVariables(_folder, config) {
        if (config.request !== 'launch') {
          return config;
        }

        const filePath = typeof config.program === 'string' && config.program.trim().length > 0
          ? config.program
          : vscode.window.activeTextEditor?.document.fileName;

        if (!filePath || !filePath.endsWith('.mlp')) {
          void vscode.window.showErrorMessage('Debug edilecek .mlp dosyası bulunamadı.');
          return undefined;
        }

        const compilerPath = typeof config.compilerPath === 'string' && config.compilerPath.trim().length > 0
          ? config.compilerPath
          : getCompilerPath(context.extensionPath);
        const normalizerPath = typeof config.normalizerPath === 'string' && config.normalizerPath.trim().length > 0
          ? config.normalizerPath
          : getNormalizerPath(context.extensionPath);
        const runtimeBitcodePath = typeof config.runtimeBitcodePath === 'string' && config.runtimeBitcodePath.trim().length > 0
          ? config.runtimeBitcodePath
          : getRuntimeBitcodePath(context.extensionPath);
        const compilerArgs = Array.isArray(config.compilerArgs)
          ? config.compilerArgs.filter((arg: unknown): arg is string => typeof arg === 'string' && arg.trim().length > 0)
          : getCompilerArgs();
        const binaryPath = getTmpPath('');

        clearCompilerDiagnostics(filePath);

        try {
          await compileMlpProgram({
            compilerPath,
            normalizerPath,
            sourcePath: filePath,
            outputPath: binaryPath,
            runtimeBitcodePath,
            compilerArgs,
            debug: true,
            extensionPath: context.extensionPath,
          });
          clearCompilerDiagnostics(filePath);

          return {
            ...config,
            program: filePath,
            compilerPath,
            normalizerPath,
            compilerArgs,
            runtimeBitcodePath,
            binaryPath,
          };
        } catch (error) {
          fs.rmSync(binaryPath, { force: true });
          publishCompilerDiagnostics(filePath, error);
          showCompilationError('MLP dosyasi debug icin derlenemedi.', error);
          return undefined;
        }
      },
    }),
    vscode.commands.registerCommand('mlp.debugFile', async (uri?: vscode.Uri) => {
      const filePath = uri?.fsPath ?? vscode.window.activeTextEditor?.document.fileName;
      if (!filePath || !filePath.endsWith('.mlp')) {
        vscode.window.showErrorMessage('Debug edilecek .mlp dosyası bulunamadı.');
        return;
      }

      const folder = vscode.workspace.getWorkspaceFolder(vscode.Uri.file(filePath));
      const config: vscode.DebugConfiguration = {
        type: 'mlp',
        request: 'launch',
        name: 'MLP Debug',
        program: filePath,
        compilerPath: getCompilerPath(context.extensionPath),
        normalizerPath: getNormalizerPath(context.extensionPath),
        compilerArgs: getCompilerArgs(),
        runtimeBitcodePath: getRuntimeBitcodePath(context.extensionPath),
        stopOnEntry: false,
      };
      await vscode.debug.startDebugging(folder, config);
    })
  );

  context.subscriptions.push(
    vscode.commands.registerCommand('mlp.openDesigner', async (uri?: vscode.Uri) => {
      const target = uri ?? vscode.window.activeTextEditor?.document.uri;
      if (!target || !target.fsPath.endsWith('.mlpgui')) {
        vscode.window.showErrorMessage('Açılacak .mlpgui dosyası bulunamadı.');
        return;
      }
      await vscode.commands.executeCommand('vscode.openWith', target, 'mlp.melpDesigner');
    }),
    vscode.commands.registerCommand('mlp.openGuiCodeEditor', async (uri?: vscode.Uri) => {
      const target = uri ?? vscode.window.activeTextEditor?.document.uri;
      if (!target || !target.fsPath.endsWith('.mlpgui')) {
        vscode.window.showErrorMessage('Kod editorunde acilacak .mlpgui dosyasi bulunamadi.');
        return;
      }
      await codeEditorBridge.openCodeEditor(target);
    }),
    vscode.commands.registerCommand('mlp-ide.openDesigner', async (uri?: vscode.Uri) => {
      await vscode.commands.executeCommand('mlp.openDesigner', uri);
    }),
    vscode.commands.registerCommand('mlp.newGuiProject', async () => {
      const name = await vscode.window.showInputBox({
        prompt: 'Proje adını girin',
        placeHolder: 'benim_projem',
        validateInput: value => /^[a-zA-Z_][a-zA-Z0-9_]*$/.test(value) ? null : 'Geçerli bir isim girin',
      });
      if (!name) {
        return;
      }

      const folders = await vscode.window.showOpenDialog({
        canSelectFiles: false,
        canSelectFolders: true,
        canSelectMany: false,
        openLabel: 'Klasör Seç',
      });
      if (!folders || folders.length === 0) {
        return;
      }

      const projectDir = path.join(folders[0].fsPath, name);
      if (!fs.existsSync(projectDir)) {
        fs.mkdirSync(projectDir, { recursive: true });
      }

      const guiFile = path.join(projectDir, `${name}.mlpgui`);
      const template = JSON.stringify({
        version: '1.0',
        canvasSettings: {
          title: name,
          width: 800,
          height: 600,
          backgroundColor: '#1e1e1e',
          gridSize: 20,
          snapToGrid: true,
        },
        widgets: [],
        code: `-- ${name} - Olay işleyicileri\n\n`,
      }, null, 2);
      fs.writeFileSync(guiFile, template, 'utf-8');
      await vscode.commands.executeCommand('vscode.openWith', vscode.Uri.file(guiFile), 'mlp.melpDesigner');
    }),
    vscode.commands.registerCommand('mlp-ide.newGuiProject', async () => {
      await vscode.commands.executeCommand('mlp.newGuiProject');
    }),
    vscode.commands.registerCommand('mlp.goToLine', (lineNumber: number) => {
      const editor = vscode.window.activeTextEditor;
      if (!editor) {
        return;
      }
      const position = new vscode.Position(lineNumber, 0);
      editor.selection = new vscode.Selection(position, position);
      editor.revealRange(new vscode.Range(position, position), vscode.TextEditorRevealType.InCenter);
    }),
    vscode.commands.registerCommand('mlp.openFunctionsView', async () => {
      await vscode.commands.executeCommand('workbench.view.extension.mlp-explorer');
    }),
    vscode.commands.registerCommand('mlp.showExample', async (topicId: string) => {
      const topic = HELP_TOPICS[topicId];
      if (!topic) {
        vscode.window.showWarningMessage(`Bilinmeyen konu: ${topicId}`);
        return;
      }

      const uri = vscode.Uri.parse(`mlp-help:${topicId}.md`);
      const provider: vscode.TextDocumentContentProvider = {
        provideTextDocumentContent: () => topic.body,
      };
      const registration = vscode.workspace.registerTextDocumentContentProvider('mlp-help', provider);
      const document = await vscode.workspace.openTextDocument(uri);
      await vscode.window.showTextDocument(document, {
        preview: true,
        viewColumn: vscode.ViewColumn.Beside,
      });
      registration.dispose();
    })
  );

  // ── Debug adapter factory ─────────────────────────────────────────────────
  context.subscriptions.push(
    vscode.debug.registerDebugAdapterDescriptorFactory('mlp', {
      createDebugAdapterDescriptor(_session) {
        const dapBinary = getDapPath();
        if (fs.existsSync(dapBinary)) {
          // melp_debugger binary — DAP over stdio
          return new vscode.DebugAdapterExecutable(dapBinary, []);
        }
        // Fallback: Node.js tabanlı GDB/MI köprüsü
        const adapterPath = path.join(__dirname, 'debugAdapter.js');
        return new vscode.DebugAdapterExecutable('node', [adapterPath]);
      }
    })
  );
}

export function deactivate(): Thenable<void> | undefined {
  return lspClient?.stop();
}

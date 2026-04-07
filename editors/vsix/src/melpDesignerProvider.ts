import * as vscode from 'vscode';
import * as fs from 'fs';
import * as os from 'os';
import * as path from 'path';
import { compileMlpProgram, getBundledRuntimeBitcodePath } from './mlpCompiler';
import { MlpguiCodeEditorBridge } from './mlpguiCodeEditorBridge';

interface MlpGuiDocumentData {
  version: string;
  canvasSettings: Record<string, unknown>;
  widgets: unknown[];
  code: string;
}

function resolveCompilerPath(extensionPath: string): string {
  const configuredPath = vscode.workspace.getConfiguration('mlp').get<string>('compilerPath', '').trim();
  if (configuredPath) {
    return configuredPath;
  }

  const platformSuffix = process.platform === 'win32'
    ? '_windows.exe'
    : process.platform === 'darwin'
      ? '_macos'
      : '_linux';
  const bundledCompiler = path.join(extensionPath, 'bin', `melp_compiler${platformSuffix}`);
  return fs.existsSync(bundledCompiler) ? bundledCompiler : 'melp_compiler';
}

function resolveRuntimeBitcodePath(extensionPath: string): string | undefined {
  const configuredPath = vscode.workspace.getConfiguration('mlp').get<string>('runtimeBitcodePath', '').trim();
  if (configuredPath && fs.existsSync(configuredPath)) {
    return configuredPath;
  }
  return getBundledRuntimeBitcodePath(extensionPath);
}

function resolveNormalizerPath(extensionPath: string): string {
  const configuredPath = vscode.workspace.getConfiguration('mlp').get<string>('normalizerPath', '').trim();
  if (configuredPath) {
    return configuredPath;
  }

  const bundledNormalizer = path.join(extensionPath, 'bin', 'melp_normalizer_bin');
  return fs.existsSync(bundledNormalizer) ? bundledNormalizer : 'melp_normalizer_bin';
}

function getCompilerArgs(): string[] {
  return vscode.workspace.getConfiguration('mlp').get<string[]>('compilerArgs', [])
    .filter((arg): arg is string => typeof arg === 'string' && arg.trim().length > 0);
}

function quoteShellArg(value: string): string {
  return `"${value.replace(/"/g, '\\"')}"`;
}

function getNonce(): string {
  const chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  let value = '';
  for (let index = 0; index < 32; index++) {
    value += chars.charAt(Math.floor(Math.random() * chars.length));
  }
  return value;
}

function normalizeGuiEventSyntax(code: string): string {
  return code.replace(/^([ \t]*event[ \t]+)([A-Za-z_][A-Za-z0-9_]*)([ \t]*)(?:--.*)?$/gm, (match, prefix, handlerName, spacing) => {
    if (match.includes('(')) {
      return match;
    }
    return `${prefix}${handlerName}()${spacing}`.trimEnd();
  });
}

class MelpDocument implements vscode.CustomDocument {
  static async create(uri: vscode.Uri): Promise<MelpDocument> {
    let data: MlpGuiDocumentData;
    try {
      const bytes = await vscode.workspace.fs.readFile(uri);
      data = JSON.parse(Buffer.from(bytes).toString('utf-8')) as MlpGuiDocumentData;
    } catch {
      data = {
        version: '1.0',
        canvasSettings: {
          title: 'Pencere',
          width: 800,
          height: 600,
          backgroundColor: '#1e1e1e',
          gridSize: 20,
          snapToGrid: true,
        },
        widgets: [],
        code: '',
      };
    }
    return new MelpDocument(uri, data);
  }

  data: MlpGuiDocumentData;
  private originalSnapshot: string;

  private constructor(public readonly uri: vscode.Uri, data: MlpGuiDocumentData) {
    this.data = data;
    this.originalSnapshot = JSON.stringify(data);
  }

  markSaved(data: MlpGuiDocumentData): void {
    this.data = data;
    this.originalSnapshot = JSON.stringify(data);
  }

  revert(): void {
    this.data = JSON.parse(this.originalSnapshot) as MlpGuiDocumentData;
  }

  dispose(): void {}
}

export class MelpDesignerProvider implements vscode.CustomEditorProvider<MelpDocument> {
  private readonly onDidChangeCustomDocumentEmitter = new vscode.EventEmitter<vscode.CustomDocumentEditEvent<MelpDocument>>();
  readonly onDidChangeCustomDocument = this.onDidChangeCustomDocumentEmitter.event;

  constructor(
    private readonly context: vscode.ExtensionContext,
    private readonly codeEditorBridge: MlpguiCodeEditorBridge
  ) {}

  async openCustomDocument(
    uri: vscode.Uri,
    _openContext: vscode.CustomDocumentOpenContext,
    _token: vscode.CancellationToken
  ): Promise<MelpDocument> {
    return MelpDocument.create(uri);
  }

  async resolveCustomEditor(
    document: MelpDocument,
    webviewPanel: vscode.WebviewPanel,
    _token: vscode.CancellationToken
  ): Promise<void> {
    webviewPanel.webview.options = {
      enableScripts: true,
      localResourceRoots: [
        vscode.Uri.joinPath(this.context.extensionUri, 'media'),
        vscode.Uri.joinPath(this.context.extensionUri, 'dist'),
      ],
    };

    webviewPanel.webview.html = this.getWebviewContent(webviewPanel.webview, document);

    webviewPanel.webview.onDidReceiveMessage(async message => {
      switch (message.type) {
        case 'save':
          await this.saveDocument(document, message.content as MlpGuiDocumentData);
          this.onDidChangeCustomDocumentEmitter.fire({
            document,
            undo: async () => {},
            redo: async () => {},
            label: 'MELP Tasarımcısı düzenlemesi',
          });
          break;
        case 'requestData':
          await webviewPanel.webview.postMessage({ type: 'load', content: document.data });
          break;
        case 'showInfo':
          vscode.window.showInformationMessage(String(message.text ?? ''));
          break;
        case 'showError':
          console.error('[MELP Designer]', String(message.text ?? 'Unknown webview error'));
          vscode.window.showErrorMessage(String(message.text ?? ''));
          break;
        case 'run':
          await this.runGuiDocument(document);
          break;
        case 'openCodeEditor':
          await this.codeEditorBridge.openCodeEditor(document.uri, message.handlerName as string | undefined);
          break;
      }
    });

    void webviewPanel.webview.postMessage({ type: 'load', content: document.data });
  }

  async saveCustomDocument(document: MelpDocument, _cancellation: vscode.CancellationToken): Promise<void> {
    await this.saveDocument(document, document.data);
  }

  async saveCustomDocumentAs(
    document: MelpDocument,
    destination: vscode.Uri,
    _cancellation: vscode.CancellationToken
  ): Promise<void> {
    const content = Buffer.from(JSON.stringify(document.data, null, 2), 'utf-8');
    await vscode.workspace.fs.writeFile(destination, content);
  }

  async revertCustomDocument(document: MelpDocument, _cancellation: vscode.CancellationToken): Promise<void> {
    document.revert();
  }

  async backupCustomDocument(
    document: MelpDocument,
    context: vscode.CustomDocumentBackupContext,
    _cancellation: vscode.CancellationToken
  ): Promise<vscode.CustomDocumentBackup> {
    const content = Buffer.from(JSON.stringify(document.data, null, 2), 'utf-8');
    await vscode.workspace.fs.writeFile(context.destination, content);
    return {
      id: context.destination.toString(),
      delete: () => vscode.workspace.fs.delete(context.destination),
    };
  }

  private async saveDocument(document: MelpDocument, data: MlpGuiDocumentData): Promise<void> {
    const content = Buffer.from(JSON.stringify(data, null, 2), 'utf-8');
    await vscode.workspace.fs.writeFile(document.uri, content);
    document.markSaved(data);
    await this.codeEditorBridge.syncCodeEditorFromGui(document.uri, data.code ?? '');
  }

  private async runGuiDocument(document: MelpDocument): Promise<void> {
    const compilerPath = resolveCompilerPath(this.context.extensionPath);
    const normalizerPath = resolveNormalizerPath(this.context.extensionPath);
    const runtimeBitcodePath = resolveRuntimeBitcodePath(this.context.extensionPath);
    const compilerArgs = getCompilerArgs();
    const tmpId = Date.now();
    const tmpMlp = path.join(os.tmpdir(), `melp_run_${tmpId}.mlp`);
    const tmpBin = path.join(os.tmpdir(), `melp_run_${tmpId}`);
    const normalizedCode = normalizeGuiEventSyntax(document.data.code ?? '');
    const wrappedCode = [
      `-- ${path.basename(document.uri.fsPath)} — GUI çalıştırma modu (otomatik üretildi)`,
      normalizedCode.trimEnd(),
      '',
      'function main()',
      'end function',
    ].join('\n');

    try {
      fs.writeFileSync(tmpMlp, wrappedCode, 'utf-8');
    } catch (error) {
      vscode.window.showErrorMessage(`Geçici GUI derleme dosyası oluşturulamadı: ${error}`);
      return;
    }

    try {
      await compileMlpProgram({
        compilerPath,
        normalizerPath,
        sourcePath: tmpMlp,
        outputPath: tmpBin,
        runtimeBitcodePath,
        compilerArgs,
      });
    } catch (error) {
      vscode.window.showErrorMessage(`GUI derlenemedi: ${error instanceof Error ? error.message : String(error)}`);
      fs.rmSync(tmpMlp, { force: true });
      return;
    }

    fs.rmSync(tmpMlp, { force: true });

    let terminal = vscode.window.terminals.find(t => t.name === 'MLP GUI');
    if (!terminal || terminal.exitStatus !== undefined) {
      terminal = vscode.window.createTerminal('MLP GUI');
    }
    terminal.show(true);
    terminal.sendText(`${quoteShellArg(tmpBin)} ; rm -f ${quoteShellArg(tmpBin)}`);
  }

  private getWebviewContent(webview: vscode.Webview, document: MelpDocument): string {
    const mediaBase = vscode.Uri.joinPath(this.context.extensionUri, 'media');
    const cssUri = webview.asWebviewUri(vscode.Uri.joinPath(mediaBase, 'designer.css'));
    const jsUri = webview.asWebviewUri(vscode.Uri.joinPath(mediaBase, 'designer.js'));
    const nonce = getNonce();
    const title = String(document.data?.canvasSettings?.title ?? 'MELP Tasarımcısı');

    return `<!DOCTYPE html>
<html lang="tr">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="Content-Security-Policy" content="default-src 'none'; img-src ${webview.cspSource} data:; style-src ${webview.cspSource} 'unsafe-inline'; script-src ${webview.cspSource} 'nonce-${nonce}';">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" href="${cssUri}">
  <title>${title}</title>
</head>
<body>
  <div id="designer-root">
    <div id="toolbar" class="toolbar">
      <span class="toolbar-title">MELP Tasarımcısı</span>
      <div id="toolbar-widgets" class="toolbar-widgets">
        <button class="tool-btn" data-type="button">⬜ Düğme</button>
        <button class="tool-btn" data-type="label">🔤 Etiket</button>
        <button class="tool-btn" data-type="textbox">✏️ Metin</button>
        <button class="tool-btn" data-type="checkbox">☑️ Onay</button>
        <button class="tool-btn" data-type="radiobutton">🔘 Radyo</button>
        <button class="tool-btn" data-type="combobox">🔽 Kombo</button>
        <button class="tool-btn" data-type="listbox">📋 Liste</button>
        <button class="tool-btn" data-type="slider">🎚️ Sürgü</button>
        <button class="tool-btn" data-type="progressbar">▰▰▱ İlerleme</button>
        <button class="tool-btn" data-type="image">🖼️ Resim</button>
        <button class="tool-btn" data-type="panel">🗂️ Panel</button>
        <button class="tool-btn" data-type="groupbox">📦 Grup</button>
      </div>
      <div class="toolbar-actions">
        <button id="btn-undo">↩</button>
        <button id="btn-redo">↪</button>
        <button id="btn-delete">🗑</button>
        <button id="btn-preview">👁</button>
        <button id="btn-run" style="background:var(--vscode-button-background);color:var(--vscode-button-foreground);border:none;border-radius:3px;padding:2px 8px;cursor:pointer;">▶ Çalıştır</button>
        <button id="btn-save">💾 Kaydet</button>
      </div>
    </div>
    <div id="main-area" class="main-area">
      <div id="properties-panel" class="properties-panel">
        <div class="panel-header">Özellikler</div>
        <div id="properties-content" class="properties-content"><p class="hint">Bir widget seçin</p></div>
      </div>
      <div id="canvas-wrapper" class="canvas-wrapper">
        <div id="canvas" class="design-canvas"></div>
      </div>
      <div id="widgets-panel" class="widgets-panel">
        <div class="panel-header">Katmanlar</div>
        <div id="layers-list" class="layers-list"></div>
      </div>
    </div>
    <div id="code-panel" class="code-panel">
      <div class="panel-header">
        <span>MLP Kodu</span>
        <div id="code-tabs" class="code-tabs"></div>
        <button id="btn-open-code-editor" class="code-open-btn" type="button">⟨/⟩ Editörde Aç</button>
      </div>
      <div class="code-notice">
        <div class="code-notice-icon">{ }</div>
        <div class="code-notice-text">
          MLP kodu gerçek VS Code editöründe düzenlenir.<br>
          <strong>Editörde Aç</strong> düğmesine basın veya soldan bir sekmeye tıklayın — kod editörü sağ panelde açılır.
        </div>
      </div>
    </div>
  </div>
  <script nonce="${nonce}">
    window.__melpQueuedErrors = [];
    window.addEventListener('error', event => {
      window.__melpQueuedErrors.push({
        type: 'showError',
        text: 'Tasarimci webview hatasi: ' + event.message + ' @ ' + (event.filename || 'inline') + ':' + (event.lineno || 0),
      });
    });
    window.addEventListener('unhandledrejection', event => {
      const reason = event.reason instanceof Error ? event.reason.message : String(event.reason);
      window.__melpQueuedErrors.push({
        type: 'showError',
        text: 'Tasarimci promise hatasi: ' + reason,
      });
    });
  </script>
  <script nonce="${nonce}" src="${jsUri}"></script>
</body>
</html>`;
  }
}
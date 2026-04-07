import * as vscode from 'vscode';
import * as path from 'path';
import * as os from 'os';
import { createHash } from 'crypto';

interface MlpGuiDocumentData {
  version: string;
  canvasSettings: Record<string, unknown>;
  widgets: unknown[];
  code: string;
}

export class MlpguiCodeEditorBridge {
  private readonly guiToCodePath = new Map<string, string>();
  private readonly codeToGuiUri = new Map<string, string>();
  private readonly syncingCodePaths = new Set<string>();
  private readonly syncingGuiUris = new Set<string>();

  async openCodeEditor(guiUri: vscode.Uri, handlerName?: string): Promise<void> {
    const data = await this.readGuiDocument(guiUri);
    const codeFileUri = await this.getOrCreateCodeFileUri(guiUri, data.code ?? '');
    const document = await vscode.workspace.openTextDocument(codeFileUri);
    await vscode.languages.setTextDocumentLanguage(document, 'mlp');

    let selection: vscode.Range | undefined;
    if (handlerName) {
      const text = document.getText();
      const lines = text.split('\n');
      for (let i = 0; i < lines.length; i++) {
        if (lines[i].includes(`event ${handlerName}`)) {
          const targetLine = Math.min(i + 1, lines.length - 1);
          selection = new vscode.Range(targetLine, 0, targetLine, 0);
          break;
        }
      }
    }

    const editor = await vscode.window.showTextDocument(document, {
      preview: false,
      preserveFocus: false,
      viewColumn: vscode.ViewColumn.Beside,
      selection,
    });

    if (selection) {
      editor.revealRange(selection, vscode.TextEditorRevealType.InCenter);
    }
  }

  isCodeEditorDocument(document: vscode.TextDocument): boolean {
    return this.codeToGuiUri.has(document.uri.fsPath);
  }

  async syncGuiFromCodeDocument(document: vscode.TextDocument): Promise<void> {
    const guiUriString = this.codeToGuiUri.get(document.uri.fsPath);
    if (!guiUriString || this.syncingCodePaths.has(document.uri.fsPath)) {
      return;
    }

    const guiUri = vscode.Uri.parse(guiUriString);
    const guiKey = guiUri.toString();
    this.syncingGuiUris.add(guiKey);

    try {
      const data = await this.readGuiDocument(guiUri);
      if ((data.code ?? '') === document.getText()) {
        return;
      }

      data.code = document.getText();
      await this.writeGuiDocument(guiUri, data);
    } finally {
      this.syncingGuiUris.delete(guiKey);
    }
  }

  async syncCodeEditorFromGui(guiUri: vscode.Uri, code: string): Promise<void> {
    const codePath = this.guiToCodePath.get(guiUri.toString());
    if (!codePath || this.syncingGuiUris.has(guiUri.toString())) {
      return;
    }

    const codeUri = vscode.Uri.file(codePath);
    const openDocument = vscode.workspace.textDocuments.find(document => document.uri.fsPath === codePath);
    if (openDocument?.isDirty) {
      return;
    }

    this.syncingCodePaths.add(codePath);
    try {
      await vscode.workspace.fs.writeFile(codeUri, Buffer.from(code ?? '', 'utf-8'));
    } finally {
      this.syncingCodePaths.delete(codePath);
    }
  }

  private async getOrCreateCodeFileUri(guiUri: vscode.Uri, code: string): Promise<vscode.Uri> {
    const existingPath = this.guiToCodePath.get(guiUri.toString());
    if (existingPath) {
      await this.syncCodeEditorFromGui(guiUri, code);
      return vscode.Uri.file(existingPath);
    }

    const storageDir = await this.ensureStorageDir(guiUri);
    const baseName = path.basename(guiUri.fsPath, path.extname(guiUri.fsPath));
    const shortHash = createHash('sha1').update(guiUri.fsPath).digest('hex').slice(0, 8);
    const filePath = path.join(storageDir, `${baseName}.${shortHash}.mlp`);
    const fileUri = vscode.Uri.file(filePath);

    await vscode.workspace.fs.writeFile(fileUri, Buffer.from(code ?? '', 'utf-8'));
    this.guiToCodePath.set(guiUri.toString(), filePath);
    this.codeToGuiUri.set(filePath, guiUri.toString());
    return fileUri;
  }

  private async ensureStorageDir(guiUri: vscode.Uri): Promise<string> {
    const workspaceFolder = vscode.workspace.getWorkspaceFolder(guiUri);
    const dirPath = workspaceFolder
      ? path.join(workspaceFolder.uri.fsPath, '.vscode', 'mlpgui-code')
      : path.join(os.tmpdir(), 'mlpgui-code');

    await vscode.workspace.fs.createDirectory(vscode.Uri.file(dirPath));
    return dirPath;
  }

  private async readGuiDocument(guiUri: vscode.Uri): Promise<MlpGuiDocumentData> {
    const bytes = await vscode.workspace.fs.readFile(guiUri);
    return JSON.parse(Buffer.from(bytes).toString('utf-8')) as MlpGuiDocumentData;
  }

  private async writeGuiDocument(guiUri: vscode.Uri, data: MlpGuiDocumentData): Promise<void> {
    await vscode.workspace.fs.writeFile(guiUri, Buffer.from(JSON.stringify(data, null, 2), 'utf-8'));
  }
}
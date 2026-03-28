import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';

interface MlpFunction {
  name: string;
  signature: string;
  description: string;
}

interface MlpFunctionsData {
  categories: Array<{
    id: string;
    name: string;
    functions: MlpFunction[];
  }>;
}

class CategoryItem extends vscode.TreeItem {
  constructor(label: string, readonly functions: FunctionItem[], collapsibleState = vscode.TreeItemCollapsibleState.Collapsed) {
    super(label, collapsibleState);
    this.iconPath = new vscode.ThemeIcon('symbol-namespace');
  }
}

class FunctionItem extends vscode.TreeItem {
  constructor(readonly fn: MlpFunction, readonly lineNumber?: number) {
    super(fn.name, vscode.TreeItemCollapsibleState.None);
    this.description = fn.signature.replace(fn.name, '').trim();
    this.tooltip = new vscode.MarkdownString(`**${fn.name}**\n\n${fn.description}`);
    this.iconPath = new vscode.ThemeIcon(lineNumber === undefined ? 'symbol-function' : 'symbol-method');
    this.command = lineNumber === undefined
      ? {
          command: 'editor.action.insertSnippet',
          title: 'Ekle',
          arguments: [{ snippet: `${fn.name}($0)` }],
        }
      : {
          command: 'mlp.goToLine',
          title: 'Git',
          arguments: [lineNumber],
        };
  }
}

type TreeNode = CategoryItem | FunctionItem;

export class MlpFunctionsViewProvider implements vscode.TreeDataProvider<TreeNode> {
  private readonly onDidChangeTreeDataEmitter = new vscode.EventEmitter<TreeNode | undefined>();
  readonly onDidChangeTreeData = this.onDidChangeTreeDataEmitter.event;
  private stdlibCategories: CategoryItem[] = [];
  private fileCategory: CategoryItem | null = null;

  constructor(context: vscode.ExtensionContext) {
    this.loadStdlib(context.extensionPath);
    context.subscriptions.push(
      vscode.window.onDidChangeActiveTextEditor(editor => {
        this.updateFileCategory(editor?.document);
        this.onDidChangeTreeDataEmitter.fire(undefined);
      }),
      vscode.workspace.onDidSaveTextDocument(document => {
        if (document.languageId === 'mlp') {
          this.updateFileCategory(document);
          this.onDidChangeTreeDataEmitter.fire(undefined);
        }
      })
    );
    this.updateFileCategory(vscode.window.activeTextEditor?.document);
  }

  getTreeItem(element: TreeNode): vscode.TreeItem {
    return element;
  }

  getChildren(element?: TreeNode): TreeNode[] {
    if (!element) {
      return this.fileCategory ? [this.fileCategory, ...this.stdlibCategories] : this.stdlibCategories;
    }
    return element instanceof CategoryItem ? element.functions : [];
  }

  refresh(): void {
    this.updateFileCategory(vscode.window.activeTextEditor?.document);
    this.onDidChangeTreeDataEmitter.fire(undefined);
  }

  private loadStdlib(extensionPath: string): void {
    const catalogPath = path.join(extensionPath, 'data', 'mlp_functions.json');
    if (!fs.existsSync(catalogPath)) {
      this.stdlibCategories = [];
      return;
    }

    try {
      const raw = JSON.parse(fs.readFileSync(catalogPath, 'utf-8')) as MlpFunctionsData;
      this.stdlibCategories = raw.categories.map(category => new CategoryItem(
        category.name,
        category.functions.map(fn => new FunctionItem(fn))
      ));
    } catch {
      this.stdlibCategories = [];
    }
  }

  private updateFileCategory(document?: vscode.TextDocument): void {
    if (!document || document.languageId !== 'mlp') {
      this.fileCategory = null;
      return;
    }

    const functions: FunctionItem[] = [];
    const regex = /^\s*function\s+([a-zA-ZçğıöşüÇĞİÖŞÜ_][a-zA-Z0-9çğıöşüÇĞİÖŞÜ_.]*)\s*\(([^)]*)\)\s*(?:as\s+(\S+))?/i;
    const lines = document.getText().split('\n');
    for (let index = 0; index < lines.length; index++) {
      const match = lines[index].match(regex);
      if (!match) {
        continue;
      }
      functions.push(new FunctionItem({
        name: match[1],
        signature: `${match[1]}(${match[2].trim()}) as ${match[3] ?? 'numeric'}`,
        description: lines[index + 1]?.trim().startsWith('--')
          ? lines[index + 1].trim().replace(/^--\s*/, '')
          : `Satır ${index + 1}`,
      }, index));
    }

    this.fileCategory = functions.length > 0
      ? new CategoryItem(`Bu Dosyada (${path.basename(document.fileName)})`, functions, vscode.TreeItemCollapsibleState.Expanded)
      : null;
    if (this.fileCategory) {
      this.fileCategory.iconPath = new vscode.ThemeIcon('file-code');
    }
  }
}
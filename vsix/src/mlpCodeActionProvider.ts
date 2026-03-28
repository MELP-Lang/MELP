import * as vscode from 'vscode';

export const HELP_TOPICS: Record<string, { title: string; body: string }> = {
  E1_IF_THEN: {
    title: 'if-then sözdizimi',
    body: [
      '# if-then sözdizimi',
      '',
      '```mlp',
      'if x > 5 then',
      '    print(x)',
      'end if',
      '```',
      '',
      '`then` her zaman zorunludur.',
    ].join('\n'),
  },
  E5_UNCLOSED_BLOCK: {
    title: 'Kapanmayan blok',
    body: [
      '# Kapanmayan blok',
      '',
      'Açılan her blok uygun `end *` kapanışıyla bitmelidir.',
      '',
      '```mlp',
      'function topla(numeric a; numeric b) as numeric',
      '    return a + b',
      'end function',
      '```',
    ].join('\n'),
  },
};

export class MlpCodeActionProvider implements vscode.CodeActionProvider {
  static readonly providedCodeActionKinds = [vscode.CodeActionKind.QuickFix, vscode.CodeActionKind.Empty];

  provideCodeActions(
    document: vscode.TextDocument,
    _range: vscode.Range | vscode.Selection,
    context: vscode.CodeActionContext
  ): vscode.CodeAction[] {
    const actions: vscode.CodeAction[] = [];

    for (const diagnostic of context.diagnostics) {
      const code = typeof diagnostic.code === 'string' ? diagnostic.code : '';
      switch (code) {
        case 'E1': {
          const line = document.lineAt(diagnostic.range.start.line);
          const trimmed = line.text.trimEnd();
          const action = new vscode.CodeAction(`'then' ekle`, vscode.CodeActionKind.QuickFix);
          action.diagnostics = [diagnostic];
          action.edit = new vscode.WorkspaceEdit();
          action.edit.insert(document.uri, new vscode.Position(line.lineNumber, trimmed.length), ' then');
          action.isPreferred = true;
          actions.push(action);
          actions.push(this.createHelpAction('if-then örneğini göster', 'E1_IF_THEN'));
          break;
        }
        case 'W5': {
          if (diagnostic.message.includes('else_if')) {
            const action = new vscode.CodeAction(`'else_if' -> 'else if'`, vscode.CodeActionKind.QuickFix);
            action.diagnostics = [diagnostic];
            action.edit = new vscode.WorkspaceEdit();
            action.edit.replace(document.uri, diagnostic.range, 'else if');
            action.isPreferred = true;
            actions.push(action);
          }
          break;
        }
        case 'E5': {
          const blockMatch = diagnostic.message.match(/'([^']+)' bloğu/);
          if (blockMatch) {
            const closer = `end ${blockMatch[1]}`;
            const line = document.lineAt(document.lineCount - 1);
            const action = new vscode.CodeAction(`Dosya sonuna '${closer}' ekle`, vscode.CodeActionKind.QuickFix);
            action.diagnostics = [diagnostic];
            action.edit = new vscode.WorkspaceEdit();
            action.edit.insert(document.uri, new vscode.Position(document.lineCount - 1, line.text.length), `\n${closer}\n`);
            actions.push(action);
          }
          actions.push(this.createHelpAction('Kapanmayan blok yardımını göster', 'E5_UNCLOSED_BLOCK'));
          break;
        }
        case 'W1': {
          const action = new vscode.CodeAction(`'break' -> 'exit'`, vscode.CodeActionKind.QuickFix);
          action.diagnostics = [diagnostic];
          action.edit = new vscode.WorkspaceEdit();
          action.edit.replace(document.uri, diagnostic.range, 'exit');
          action.isPreferred = true;
          actions.push(action);
          break;
        }
      }
    }

    return actions;
  }

  private createHelpAction(title: string, topicId: string): vscode.CodeAction {
    const action = new vscode.CodeAction(title, vscode.CodeActionKind.Empty);
    action.command = {
      command: 'mlp.showExample',
      title,
      arguments: [topicId],
    };
    return action;
  }
}
// VSCode Extension for MLP Language
// Provides LSP client integration

const vscode = require('vscode');
const path = require('path');
const { LanguageClient, TransportKind } = require('vscode-languageclient/node');

let client;

function activate(context) {
    console.log('MLP Language Extension is now active!');

    // Get LSP server path from configuration
    const config = vscode.workspace.getConfiguration('mlp');
    const lspPath = config.get('lspPath') || 'mlp-lsp';

    // Server options
    const serverOptions = {
        command: lspPath,
        args: ['--stdio'],
        options: {
            shell: true
        }
    };

    // Client options
    const clientOptions = {
        documentSelector: [{ scheme: 'file', language: 'mlp' }],
        synchronize: {
            fileEvents: vscode.workspace.createFileSystemWatcher('**/*.mlp')
        }
    };

    // Create and start the language client
    client = new LanguageClient(
        'mlpLanguageServer',
        'MLP Language Server',
        serverOptions,
        clientOptions
    );

    // Start the client (and server)
    client.start();

    console.log('MLP LSP client started');
}

function deactivate() {
    if (client) {
        return client.stop();
    }
}

module.exports = {
    activate,
    deactivate
};

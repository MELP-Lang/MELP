# MLP Language Support for VSCode

Language support extension for MLP (Modern Language Project) in Visual Studio Code.

## Features

- **Syntax Highlighting** - Full syntax highlighting for MLP files
- **Autocomplete** - Intelligent code completion for functions, variables, and keywords
- **Go to Definition** - Jump to symbol definitions (F12)
- **Hover Information** - Type and signature information on hover
- **Language Server Protocol** - Powered by `mlp-lsp` server

## Installation

### 1. Build the LSP Server

```bash
cd tools/mlp-lsp
make
sudo cp mlp-lsp /usr/local/bin/  # Or add to PATH
```

### 2. Install Extension

Option A: Development mode
```bash
cd editors/vscode-mlp
npm install
# Copy to VSCode extensions folder
cp -r . ~/.vscode/extensions/mlp-language-0.1.0/
```

Option B: Package and install
```bash
cd editors/vscode-mlp
npm install
npx vsce package
code --install-extension mlp-language-0.1.0.vsix
```

### 3. Configure LSP Path (Optional)

In VSCode settings (`settings.json`):
```json
{
  "mlp.lspPath": "/path/to/mlp-lsp"
}
```

## Usage

1. Open a `.mlp` file in VSCode
2. Extension activates automatically
3. Enjoy:
   - Autocomplete: Start typing and press `Ctrl+Space`
   - Go to Definition: `F12` or right-click → "Go to Definition"
   - Hover: Move mouse over symbols

## Example

```mlp
# MLP example
fn calculate(x: Integer, y: Integer) -> Integer:
    let result = x + y
    return result

let value = calculate(10, 20)
print(value)
```

## Requirements

- VSCode 1.60.0 or higher
- Node.js (for extension development)
- `mlp-lsp` server binary in PATH

## Troubleshooting

### LSP not working?

1. Check if `mlp-lsp` is in PATH:
   ```bash
   which mlp-lsp
   ```

2. Check VSCode output panel:
   - View → Output → Select "MLP Language Server"

3. Enable verbose logging in settings:
   ```json
   {
     "mlp.trace.server": "verbose"
   }
   ```

## Development

```bash
cd editors/vscode-mlp
npm install
code .
# Press F5 to launch Extension Development Host
```

## License

MIT License

## Credits

Built by YZ_03 for the MLP project.

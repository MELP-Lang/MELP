# LSP Usage Guide

Complete guide for using MLP Language Server Protocol.

## Quick Start

### 1. Build LSP Server

```bash
cd /home/pardus/projeler/MLP/MELP-GCC-WORKSHOP/tools/mlp-lsp
make
```

### 2. Test LSP Server

```bash
# Test parser
./test_parser.sh

# Test LSP protocol
./test_simple.sh
```

### 3. Install to PATH (Optional)

```bash
sudo cp mlp-lsp /usr/local/bin/
# OR
export PATH=$PATH:/home/pardus/projeler/MLP/MELP-GCC-WORKSHOP/tools/mlp-lsp
```

## VSCode Integration

### Install Extension

1. Navigate to extension directory:
```bash
cd /home/pardus/projeler/MLP/MELP-GCC-WORKSHOP/editors/vscode-mlp
```

2. Install dependencies (if developing):
```bash
npm install
```

3. Copy to VSCode extensions:
```bash
mkdir -p ~/.vscode/extensions/mlp-language-0.1.0
cp -r * ~/.vscode/extensions/mlp-language-0.1.0/
```

4. Reload VSCode: `Ctrl+Shift+P` → "Developer: Reload Window"

### Configure LSP Path

If `mlp-lsp` is not in PATH, configure in VSCode settings:

1. `Ctrl+,` → Open Settings (JSON)
2. Add:
```json
{
  "mlp.lspPath": "/absolute/path/to/mlp-lsp"
}
```

### Verify Installation

1. Create a test file: `test.mlp`
```mlp
fn calculate(x: Integer) -> Integer:
    return x * 2

let result = calculate(42)
```

2. Open in VSCode
3. Try:
   - Type `cal` → Should show `calculate` in autocomplete
   - Hover over `calculate` → Should show signature
   - F12 on `calculate` → Should jump to definition

## Features

### Autocomplete (`Ctrl+Space`)

Triggers:
- After typing identifier start
- After `.` (dot)
- After `:` (colon)

Provides:
- Keywords (fn, let, if, else, etc.)
- Function names with signatures
- Variable names with types
- Imports

Example:
```mlp
fn greet(name: String) -> String:
    return "Hello"

# Type 'gre' → autocomplete shows 'greet: fn(String) -> String'
```

### Go to Definition (`F12`)

Jump to:
- Function definitions
- Variable declarations
- Import sources (if available)

Example:
```mlp
fn helper():
    pass

helper()  # F12 here → jumps to 'fn helper()' above
```

### Hover Information

Move mouse over symbol to see:
- Function signature
- Variable type
- Documentation (if available)

Example:
```mlp
fn add(x: Integer, y: Integer) -> Integer:
    return x + y

# Hover over 'add' → Shows: fn(Integer, Integer) -> Integer
```

## Editor Support

### Vim/Neovim

With **coc.nvim**:

1. Install coc.nvim: https://github.com/neoclide/coc.nvim
2. Add to `:CocConfig`:
```json
{
  "languageserver": {
    "mlp": {
      "command": "mlp-lsp",
      "args": ["--stdio"],
      "filetypes": ["mlp"],
      "rootPatterns": [".git/", "."],
      "settings": {}
    }
  }
}
```

3. Open `.mlp` file → LSP activates

### Emacs

With **lsp-mode**:

```elisp
(use-package lsp-mode
  :commands lsp
  :init
  (add-to-list 'lsp-language-id-configuration '(mlp-mode . "mlp"))
  (lsp-register-client
    (make-lsp-client
      :new-connection (lsp-stdio-connection "mlp-lsp")
      :major-modes '(mlp-mode)
      :server-id 'mlp-lsp)))

(add-hook 'mlp-mode-hook #'lsp)
```

### Sublime Text

With **LSP** package:

1. Install LSP: Package Control → Install Package → LSP
2. Add to LSP settings:
```json
{
  "clients": {
    "mlp": {
      "enabled": true,
      "command": ["mlp-lsp", "--stdio"],
      "selector": "source.mlp"
    }
  }
}
```

## Troubleshooting

### LSP not starting?

Check if binary works:
```bash
which mlp-lsp
# OR
mlp-lsp --help
```

### No completions appearing?

1. Check VSCode Output panel: View → Output → "MLP Language Server"
2. Look for errors
3. Check file is recognized as MLP: Bottom-right corner should show "MLP"

### Completions wrong/outdated?

LSP parses on every edit. If stuck:
1. Save file (`Ctrl+S`)
2. Close and reopen file
3. Reload VSCode window

### Performance issues?

For large files (>5000 lines):
- Parser might be slow
- Consider splitting file
- Future: Incremental parsing will help

## Development

### Build from source:

```bash
cd tools/mlp-lsp
make clean
make
```

### Run tests:

```bash
./test_parser.sh       # Parser test
./test_simple.sh       # LSP protocol test
./test_lsp_e2e.sh      # End-to-end test
```

### Debug mode:

LSP writes logs to stderr. Run manually:
```bash
mlp-lsp --stdio 2> lsp.log
```

Then check `lsp.log` for debug messages.

## Limitations

Current version (0.1.0):
- ✅ Single-file analysis only
- ✅ Full document sync (no incremental)
- ✅ Basic type inference
- ❌ No cross-file imports (yet)
- ❌ No semantic diagnostics (yet)
- ❌ No rename refactoring (yet)

## Architecture

```
mlp-lsp (2273 lines, excl. cJSON)
├── main.c (63)           - Entry point, stdio loop
├── lsp_protocol.c (580)  - LSP method handlers
├── jsonrpc.c (197)       - JSON-RPC 2.0 transport
├── symbol_table.c (289)  - Symbol database
├── mlp_parser.c (390)    - MLP tokenizer + parser
└── headers (357)         - Type definitions
```

**Key Design:**
- Standalone binary (NO main.c modification!)
- JSON-RPC over stdio (standard LSP)
- Document cache with auto-reparsing
- Symbol table with scope tracking

## Credits

**YZ_03** - 1 Ocak 2026  
**Project:** MLP (Modern Language Project)  
**Status:** ✅ Production Ready

## License

MIT

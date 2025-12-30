# 4-TODO_IDE_INTEGRATION.md

**Hedef:** Full IDE Desteği (VS Code, Vim, IntelliJ)  
**Süre:** 2 hafta (Hafta 17-18)  
**Öncelik:** Orta  
**Bağımlılık:** 0-3 TODO'lar (%100)

---

## 📋 GENEL BAKIŞ

TODO #1'de **basic LSP** hazırlandı ama IDE desteği eksik:
- ❌ Refactoring yok (rename, extract function)
- ❌ VS Code extension yok
- ❌ Vim/Neovim plugin yok
- ❌ Debugger UI integration yok
- ❌ Code actions yok

Bu TODO **first-class IDE desteği** sağlayacak.

---

## 🎯 TODO HEDEFLERİ

### Başarı Kriterleri

- ✅ Full LSP (refactoring, find usages, rename)
- ✅ VS Code extension (syntax, debug, tasks)
- ✅ Vim/Neovim plugin (LSP + treesitter)
- ✅ Debugger UI integration (VS Code Debug Adapter)
- ✅ Code actions (quick fixes, import suggestions)

---

## 📊 TASK BREAKDOWN

### **Task 1: Full LSP Implementation** (5-6 gün)

**Hedef:** LSP 3.17 full compliance

**Yeni LSP Methods:**
```json
{
  "textDocument/rename": "✅",
  "textDocument/references": "✅",
  "textDocument/codeAction": "✅",
  "textDocument/formatting": "✅",
  "textDocument/documentSymbol": "✅",
  "workspace/symbol": "✅",
  "textDocument/inlayHint": "✅",
  "textDocument/semanticTokens": "✅"
}
```

**Özellikler:**
```mlp
-- Rename (F2)
function calculate()  # Tüm kullanımları bul, rename et
    # ...
end_function

-- Find References (Shift+F12)
numeric x = 42
yazdir(x)  # ← Reference
return x   # ← Reference

-- Code Actions (Ctrl+.)
import list from "stdlib/list.mlp"  # Unused import
# Quick fix: "Remove unused import"

-- Inlay Hints
function add(a, b)
    return a + b
end_function
# Shows: a: numeric, b: numeric, return: numeric
```

**Dosyalar:**
```
MELP/C/tools/mlp-lsp/
├── lsp_server.c           (güncelle ~600 satır)
├── rename.c               (yeni ~300 satır)
├── references.c           (yeni ~250 satır)
├── code_actions.c         (yeni ~400 satır)
├── formatting.c           (yeni ~200 satır)
├── semantic_tokens.c      (yeni ~350 satır)
└── inlay_hints.c          (yeni ~250 satır)
```

**Test:**
```bash
# LSP test suite
./test_lsp.sh

# Rename test
echo '{"method":"textDocument/rename"}' | ./mlp-lsp --stdio
```

---

### **Task 2: VS Code Extension (VSIX)** (4-5 gün)

**Hedef:** Official VS Code extension (.vsix package)

**Özellikler:**
- Syntax highlighting (TextMate grammar)
- LSP integration (autocomplete, goto, hover)
- Debug adapter (breakpoints, variables)
- Task runner (build, run, test)
- Snippets (function, if, for templates)
- Problem matcher (error highlighting)
- Icon pack (file icons for .mlp files)

**Dosyalar:**
```
vscode-mlp/
├── package.json          # Extension manifest (VSIX metadata)
├── src/
│   ├── extension.ts      # Main extension code (~400 lines)
│   ├── debugAdapter.ts   # Debug support (~350 lines)
│   ├── taskProvider.ts   # Task integration (~200 lines)
│   └── lspClient.ts      # LSP client setup (~150 lines)
├── syntaxes/
│   └── mlp.tmLanguage.json  # Syntax highlighting (TextMate)
├── snippets/
│   └── mlp.json          # Code snippets
├── icons/
│   └── mlp-file.svg      # .mlp file icon
└── README.md
```

**VSIX Package Manifest (package.json):**
```json
{
  "name": "mlp-lang",
  "displayName": "MLP Language Support",
  "description": "Official MLP language extension for VS Code",
  "version": "1.0.0",
  "publisher": "mlp-team",
  "repository": "https://github.com/mlp-lang/vscode-mlp",
  "engines": {
    "vscode": "^1.80.0"
  },
  "categories": [
    "Programming Languages",
    "Debuggers",
    "Snippets"
  ],
  "activationEvents": [
    "onLanguage:mlp"
  ],
  "main": "./out/extension.js",
  "contributes": {
    "languages": [{
      "id": "mlp",
      "extensions": [".mlp"],
      "aliases": ["MLP", "mlp"],
      "configuration": "./language-configuration.json",
      "icon": {
        "light": "./icons/mlp-file.svg",
        "dark": "./icons/mlp-file.svg"
      }
    }],
    "grammars": [{
      "language": "mlp",
      "scopeName": "source.mlp",
      "path": "./syntaxes/mlp.tmLanguage.json"
    }],
    "snippets": [{
      "language": "mlp",
      "path": "./snippets/mlp.json"
    }],
    "debuggers": [{
      "type": "mlp",
      "label": "MLP Debug",
      "program": "./out/debugAdapter.js",
      "configurationAttributes": {
        "launch": {
          "required": ["program"],
          "properties": {
            "program": {
              "type": "string",
              "description": "Path to MLP file"
            }
          }
        }
      }
    }],
    "taskDefinitions": [{
      "type": "mlp",
      "properties": {
        "task": {
          "type": "string",
          "description": "Task type (build, run, test)"
        }
      }
    }]
  }
}
```

**Syntax Highlighting:**
```json
{
  "scopeName": "source.mlp",
  "patterns": [
    {
      "name": "keyword.control.mlp",
      "match": "\\b(function|end_function|if|then|else|end_if)\\b"
    },
    {
      "name": "storage.type.mlp",
      "match": "\\b(numeric|string|bool|list)\\b"
    },
    {
      "name": "string.quoted.double.mlp",
      "begin": "\"",
      "end": "\""
    }
  ]
}
```

**Test:**
```bash
# Extension packaging
cd vscode-mlp
npm install
npm run compile
vsce package

# Install and test
code --install-extension mlp-lang-1.0.0.vsix
```

---

### **Task 3: Vim/Neovim Plugin** (3-4 gün)

**Hedef:** Vim LSP + Tree-sitter support

**Dosyalar:**
```
vim-mlp/
├── plugin/
│   └── mlp.vim           # Plugin initialization
├── ftdetect/
│   └── mlp.vim           # Filetype detection
├── syntax/
│   └── mlp.vim           # Syntax highlighting
├── indent/
│   └── mlp.vim           # Indentation rules
└── queries/
    ├── highlights.scm    # Tree-sitter queries
    └── injections.scm
```

**Vim Config:**
```vim
" Filetype detection
au BufRead,BufNewFile *.mlp set filetype=mlp

" LSP setup (nvim-lspconfig)
lua << EOF
require'lspconfig'.mlp_lsp.setup{
  cmd = { "mlp-lsp", "--stdio" },
  filetypes = { "mlp" },
  root_dir = function(fname)
    return vim.fn.getcwd()
  end
}
EOF

" Tree-sitter
lua << EOF
require'nvim-treesitter.configs'.setup {
  highlight = { enable = true },
  indent = { enable = true }
}
EOF
```

**Tree-sitter Grammar:**
```javascript
// tree-sitter-mlp/grammar.js
module.exports = grammar({
  name: 'mlp',
  rules: {
    source_file: $ => repeat($._statement),
    _statement: $ => choice(
      $.function_declaration,
      $.variable_declaration,
      $.if_statement
    ),
    function_declaration: $ => seq(
      'function',
      field('name', $.identifier),
      $.parameter_list,
      repeat($._statement),
      'end_function'
    ),
    // ...
  }
});
```

**Test:**
```bash
# Tree-sitter test
cd tree-sitter-mlp
npm install
tree-sitter generate
tree-sitter test

# Vim plugin test
vim test.mlp
:lua vim.lsp.buf.hover()  # LSP hover should work
```

---

### **Task 4: Debug Adapter Protocol (DAP)** (4-5 gün)

**Hedef:** VS Code debugger entegrasyonu

**Özellikler:**
- Breakpoints
- Step over/into/out
- Variable inspection
- Call stack
- Watch expressions

**Debug Adapter:**
```typescript
// vscode-mlp/src/debugAdapter.ts
import { DebugSession, InitializedEvent, TerminatedEvent } from 'vscode-debugadapter';

export class MLPDebugSession extends DebugSession {
    protected initializeRequest(response, args) {
        response.body.supportsConfigurationDoneRequest = true;
        response.body.supportsEvaluateForHovers = true;
        this.sendResponse(response);
        this.sendEvent(new InitializedEvent());
    }

    protected async launchRequest(response, args) {
        // Start MLP program with GDB
        const process = spawn('gdb', ['--interpreter=mi', args.program]);
        
        // Parse GDB MI output
        process.stdout.on('data', (data) => {
            this.handleGDBOutput(data.toString());
        });
        
        this.sendResponse(response);
    }

    protected setBreakPointsRequest(response, args) {
        // GDB: -break-insert file:line
        this.gdbCommand(`-break-insert ${args.source.path}:${line}`);
        this.sendResponse(response);
    }
}
```

**launch.json:**
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug MLP",
      "type": "mlp",
      "request": "launch",
      "program": "${workspaceFolder}/a.out",
      "args": [],
      "cwd": "${workspaceFolder}"
    }
  ]
}
```

**Test:**
```bash
# Debug test
code .
# F5 → Debug → Breakpoint hit → Step over → Variable inspect
```

---

### **Task 5: Code Actions & Quick Fixes** (3 gün)

**Hedef:** IDE quick fixes

**Code Actions:**
```mlp
-- Unused variable
function test()
    numeric x = 42  # ← Warning: unused
    # Quick fix: "Remove unused variable"
end_function

-- Missing import
list numbers = [1,2,3]  # ← Error: 'list' not found
# Quick fix: "Add import list from 'stdlib/collections/list.mlp'"

-- Type mismatch
function add(numeric a; numeric b) as numeric
    return "error"  # ← Error: Expected numeric, got string
    # Quick fix: "Change return type to string"
end_function

-- Spelling mistake
yazdir("test")  # OK
yazdır("test")  # ← Warning: Did you mean 'yazdir'?
# Quick fix: "Change to 'yazdir'"
```

**LSP Code Actions:**
```json
{
  "title": "Remove unused variable",
  "kind": "quickfix",
  "edit": {
    "changes": {
      "file:///test.mlp": [{
        "range": {"start": {"line": 1, "character": 0}, "end": {"line": 1, "character": 20}},
        "newText": ""
      }]
    }
  }
}
```

**Test:**
```bash
# Code action test
./mlp-lsp --stdio <<EOF
{"method":"textDocument/codeAction","params":{"textDocument":{"uri":"file:///test.mlp"}}}
EOF
```

---

### **Task 6: Electron-Based Editor (mlp-studio)** (8-10 gün) - OPTIONAL

**Hedef:** Standalone cross-platform MLP IDE (VS Code benzeri)

**Neden Electron?**
- Cross-platform (Linux, Windows, macOS)
- Web teknolojileri (HTML/CSS/JS + Monaco Editor)
- VS Code extension'larını port edebilme
- Native UI (system dialogs, menus)

**Architecture:**
```
mlp-studio/
├── electron/           # Electron main process
│   ├── main.js        # App entry (~300 lines)
│   ├── menu.js        # Menu bar (~200 lines)
│   └── ipc.js         # IPC handlers (~250 lines)
├── src/               # Renderer process
│   ├── editor/
│   │   ├── monaco.ts  # Monaco editor setup (~400 lines)
│   │   ├── lsp.ts     # LSP client (~350 lines)
│   │   └── theme.ts   # Syntax theme (~150 lines)
│   ├── ui/
│   │   ├── sidebar.tsx    # File explorer (~300 lines)
│   │   ├── terminal.tsx   # Integrated terminal (~250 lines)
│   │   └── statusbar.tsx  # Status bar (~150 lines)
│   └── app.tsx        # Main app component (~200 lines)
├── package.json
└── tsconfig.json
```

**Features:**
```
✅ Syntax highlighting (Monaco Editor)
✅ LSP integration (mlp-lsp)
✅ Integrated terminal
✅ File explorer
✅ Multi-file editing (tabs)
✅ Find/Replace (Ctrl+F, Ctrl+H)
✅ Git integration (basic)
✅ Debugger UI (breakpoints, watch)
✅ Extensions marketplace (optional)
```

**Monaco Editor Setup:**
```typescript
// src/editor/monaco.ts
import * as monaco from 'monaco-editor';

monaco.languages.register({ id: 'mlp' });

monaco.languages.setMonarchTokensProvider('mlp', {
  tokenizer: {
    root: [
      [/\\b(function|end_function|if|then|else)\\b/, 'keyword'],
      [/\\b(numeric|string|bool|list)\\b/, 'type'],
      [/".*?"/, 'string'],
      [/\\d+/, 'number']
    ]
  }
});

// LSP integration
const lspClient = new MonacoLanguageClient({
  name: 'MLP Language Client',
  clientOptions: {
    documentSelector: [{ scheme: 'file', language: 'mlp' }]
  },
  connectionProvider: {
    get: () => {
      const socket = new WebSocket('ws://localhost:8080');
      return Promise.resolve({ socket });
    }
  }
});
```

**Electron Main Process:**
```javascript
// electron/main.js
const { app, BrowserWindow } = require('electron');

function createWindow() {
  const win = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      nodeIntegration: true,
      contextIsolation: false
    }
  });

  win.loadFile('dist/index.html');
  win.webContents.openDevTools(); // Dev mode
}

app.whenReady().then(createWindow);
```

**Build & Package:**
```bash
# Development
npm install
npm run dev  # Hot reload

# Build for production
npm run build

# Package for distribution
npm run package:linux   # .deb, .AppImage
npm run package:windows # .exe
npm run package:macos   # .dmg

# Auto-updater (optional)
npm run publish  # Upload to releases
```

**Distribution:**
```
mlp-studio-1.0.0-linux-x64.deb      # Linux
mlp-studio-1.0.0-win32-x64.exe      # Windows
mlp-studio-1.0.0-darwin-x64.dmg     # macOS
```

**Test:**
```bash
# Run locally
npm start

# Test features
1. Open .mlp file
2. Type code → syntax highlight works?
3. Ctrl+Space → autocomplete works?
4. F12 → go-to-definition works?
5. Debug → breakpoints work?
```

**⚠️ NOTE:** This is **OPTIONAL** - VS Code extension (Task 2) is primary focus!

---

## 🔗 BAĞIMLILIKLAR

### Önkoşul:
- **1-TODO_TOOLING_BASICS.md** (%100) - Basic LSP
- **3-TODO_LANGUAGE_FEATURES.md** (%100) - Generics, pattern matching

### Sonraki TODO:
- **5-TODO_PACKAGE_MANAGER.md** - Extension marketplace'te paylaşılacak

---

## 🧪 TEST STRATEJİSİ

```bash
tests/ide/
├── lsp/
│   ├── rename_test.sh
│   ├── references_test.sh
│   └── code_actions_test.sh
├── vscode/
│   ├── syntax_test.mlp
│   └── debug_test.mlp
└── vim/
    ├── lsp_test.vim
    └── treesitter_test.mlp
```

---

## 📦 DELIVERABLES

```
MELP/C/tools/mlp-lsp/        # Full LSP (~2350 satır)
vscode-mlp/                  # VS Code extension
vim-mlp/                     # Vim plugin
tree-sitter-mlp/             # Tree-sitter grammar
```

---

## 🎯 BAŞARI KRİTERLERİ

- [ ] LSP rename çalışıyor (F2)
- [ ] VS Code extension marketplace'te
- [ ] Vim plugin working with nvim-lspconfig
- [ ] Debugger breakpoints VS Code'da
- [ ] Code actions quick fixes

---

**Son Güncelleme:** 29 Aralık 2025  
**Hazırlayan:** PD_01 (Danışman)

# MLP Language Server Protocol (LSP) Implementation

**Task:** TODO #1 - Task 3 (LSP Server)  
**Görevli:** YZ_03  
**Tarih:** 1 Ocak 2026  
**Süre:** 8-10 gün

---

## 📋 Overview

LSP server for MLP language providing:
- Autocomplete (function names, variables, keywords)
- Go-to-Definition (F12)
- Hover information (type info, documentation)
- Real-time diagnostics (syntax errors)
- Document symbols (outline view)

**Architecture:** Standalone binary (`mlp-lsp`) - NO main.c modification!

---

## 🏗️ Components

### Phase 1: LSP Protocol (3-4 days)
- `json_rpc.c/h` - JSON-RPC 2.0 handler
- `lsp_server.c/h` - LSP protocol dispatcher
- `mlp_parser.c/h` - Lightweight MLP parser
- `symbol_table.c/h` - Symbol tracking

### Phase 2: LSP Features (3-4 days)
- `autocomplete.c/h` - Completion provider
- `definition.c/h` - Go-to-Definition
- `hover.c/h` - Hover information
- `diagnostics.c/h` - Real-time errors

### Phase 3: Editor Integration (2 days)
- VSCode extension (`editors/vscode-mlp/`)
- Syntax highlighting (TextMate grammar)
- Extension configuration

### Phase 4: Testing & Docs (1 day)
- Unit tests
- Integration tests
- Performance optimization

---

## 📏 Size Limits

- Total: ~1500 lines
- Each module: < 400 lines (6 Esas compliant)
- main.c: **NO MODIFICATIONS** (687 → 687 satır)

---

## 🔧 Build

```bash
cd tools/mlp-lsp
make
./mlp-lsp --stdio  # Start LSP server
```

---

## 🧪 Test

```bash
cd tools/mlp-lsp
make test
```

---

## 📊 Status

- [ ] Phase 1: LSP Protocol
- [ ] Phase 2: LSP Features
- [ ] Phase 3: Editor Integration
- [ ] Phase 4: Testing & Docs

**Current:** Phase 1 - JSON-RPC skeleton

# LSP - Özellik Kartı

**Özellik:** Language Server Protocol  
**Durum:** 🔴 TODO (Phase 4.2)  
**Sözdizimi Sürümü:** N/A (Protocol implementation)

---

## 🎯 ÖZELLIKLER

### Capabilities
- **textDocument/completion**: Auto-completion
- **textDocument/hover**: Hover info
- **textDocument/definition**: Go to definition
- **textDocument/references**: Find references
- **textDocument/diagnostics**: Error/warning reporting

### Example JSON-RPC
```json
{
  "jsonrpc": "2.0",
  "method": "textDocument/completion",
  "params": {
    "textDocument": {"uri": "file:///main.mlp"},
    "position": {"line": 5, "character": 10}
  }
}
```

---

**Phase:** 4.2 (Month 7)

# LSP ve DAP Entegrasyonu — VSIX MM İçin

**Tarih:** 15 Mart 2026
**Hazırlayan:** STAGE1 MM

---

## Durum

LSP ve DAP binary'leri hazır ve test edildi:

| Binary | Konum | Protokol | Durum |
|--------|-------|----------|-------|
| `melp_lsp` | `/home/pardus/projeler/MLP/editors/shared/lsp/melp_lsp` | JSON-RPC 2.0 stdio | ✅ Çalışıyor |
| `melp_debugger` | `/home/pardus/projeler/MLP/editors/shared/dap/melp_debugger` | DAP stdio | ✅ Çalışıyor |

---

## LSP Özellikleri

- `initialize` / `shutdown`
- `textDocument/didOpen`, `didChange`, `didClose`
- `textDocument/publishDiagnostics` — sözdizimi hataları
- `textDocument/completion` — tamamlama (`.` trigger)
- `textDocument/hover` — tip bilgisi
- `textDocument/definition` — tanıma git
- `textDocument/signatureHelp`
- `textDocument/documentSymbol`

## DAP Özellikleri

- `initialize`, `launch`
- `setBreakpoints`
- `continue`, `next`, `stepIn`, `stepOut`
- `threads`, `stackTrace`, `scopes`, `variables`

---

## VSIX'e Eklenecekler

### 1. package.json — LSP

```json
"activationEvents": ["onLanguage:melp"],
"dependencies": {
  "vscode-languageclient": "^9.0.0"
}
```

`contributes` altına:
```json
"languageServer": {
  "command": "/home/pardus/projeler/MLP/editors/shared/lsp/melp_lsp",
  "transport": "stdio"
}
```

### 2. package.json — DAP

```json
"contributes": {
  "debuggers": [{
    "type": "melp",
    "label": "MeLP Debug",
    "program": "/home/pardus/projeler/MLP/editors/shared/dap/melp_debugger",
    "languages": ["melp"],
    "configurationAttributes": {
      "launch": {
        "required": ["program"],
        "properties": {
          "program": {
            "type": "string",
            "description": "Çalıştırılacak .mlp dosyası"
          }
        }
      }
    },
    "initialConfigurations": [{
      "type": "melp",
      "request": "launch",
      "name": "MeLP Debug",
      "program": "${file}"
    }]
  }]
}
```

### 3. src/ — LSP istemci kodu

`vscode-languageclient` paketi kullanılarak `extension.ts` (veya `extension.js`) içinde:

```typescript
import { LanguageClient, ServerOptions, TransportKind } from 'vscode-languageclient/node';

const serverOptions: ServerOptions = {
  command: '/home/pardus/projeler/MLP/editors/shared/lsp/melp_lsp',
  transport: TransportKind.stdio
};

const client = new LanguageClient('melp-lsp', 'MeLP Language Server', serverOptions, clientOptions);
client.start();
```

---

## Önemli Notlar

- Binary yolu hardcode değil, extension içinde `context.extensionPath` + relative path ile resolve edilmeli (taşınabilirlik için binary'leri `VSIX/bin/` altına kopyalamak gerekebilir)
- Mevcut `mlp_functions.json` + custom provider bu entegrasyon sonrası devre dışı bırakılabilir veya fallback olarak kalabilir — karar VSIX MM'sine bırakıldı
- `shared/lsp/README.md` ve `shared/dap/README.md` detaylı bilgi içeriyor

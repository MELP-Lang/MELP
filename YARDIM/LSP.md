# MELP Language Server Protocol (LSP) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 4.2

---

## Genel Bilgi

MELP Language Server, editörlere standart LSP arayüzüyle akıllı özellikler sağlar.  
VS Code, Neovim, Emacs gibi LSP destekleyen tüm editörlerle çalışır.

> **⚠️ UYARI:** LSP sunucusu Phase 4.2'de implement edilecektir.

---

## Planlanan Yetenekler

| Yetenek | Açıklama |
|---------|----------|
| `textDocument/completion` | Otomatik tamamlama |
| `textDocument/hover` | Hover bilgisi |
| `textDocument/definition` | Tanıma git |
| `textDocument/references` | Referans bul |
| `textDocument/diagnostics` | Hata/uyarı raporlama |
| `textDocument/documentSymbol` | Belge sembolleri |
| `textDocument/formatting` | Kod biçimlendirme |

---

## JSON-RPC Örneği

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

## Şu Anki Editör Entegrasyonu

VS Code için syntax highlighting uzantısı mevcut:
```bash
# Uzantı yükle:
code --install-extension melp-language-support.vsix
```

**Sağlanan:**
- `.mlp` dosyaları için syntax highlighting
- Temel hata vurgulama
- Code snippets

---

## İlgili Özellikler

- **CODE_INTELLIGENCE** — Rename, refactoring özellikleri
- **ELECTRON_IDE** — Masaüstü IDE
- **TREE_SITTER** — Syntax highlighting altyapısı

**Ayrıca bakınız:**
- [CODE_INTELLIGENCE.md](CODE_INTELLIGENCE.md)
- [belgeler/EDITOR_INTEGRATION_GUIDE.md](../belgeler/EDITOR_INTEGRATION_GUIDE.md)

---

## Durum Tablosu

| Özellik | Phase 4.2 | Şu An |
|---------|-----------|-------|
| LSP sunucusu | ✅ Planlı | ❌ |
| Otomatik tamamlama | ✅ | ❌ |
| Hover bilgisi | ✅ | ❌ |
| Tanıma git | ✅ | ❌ |
| Syntax highlighting | N/A | ✅ (VSIX) |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| LSP ile Phase 4.2 öncesi kod tamamlama beklentisi | Şu an sadece syntax highlighting mevcut |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/LSP.md](../belgeler/language/feature_cards/LSP.md)
- **Editör Entegrasyon:** [belgeler/EDITOR_INTEGRATION_GUIDE.md](../belgeler/EDITOR_INTEGRATION_GUIDE.md)

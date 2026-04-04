# MELP Electron IDE Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 4.5

---

## Genel Bilgi

MELP için Electron tabanlı masaüstü IDE. Monaco editörü üzerine inşa edilmiştir.  
Phase 4.5'te implement edilecektir.

> **⚠️ UYARI:** Bu özellik Phase 4.5'e kadar mevcut değildir.  
> Şimdilik VS Code uzantısını kullanın: [belgeler/EDITOR_INTEGRATION_GUIDE.md](../belgeler/EDITOR_INTEGRATION_GUIDE.md)

---

## Planlanan Özellikler

### Ana Pencere Bileşenleri
```
ElectronWindow {
  Editor: Monaco tabanlı kod editörü
  Sidebar: Dosya gezgini (proje ağaç görünümü)
  Panel: {
    Terminal: Entegre terminal
    Debug: Debug paneli
    Output: Çıktı paneli
  }
  StatusBar: Dil durum çubuğu
}
```

### Temel Klavye Kısayolları
| Kısayol | Eylem |
|---------|-------|
| `Ctrl+B` | Kenar çubuğu aç/kapat |
| `Ctrl+J` | Panel aç/kapat |
| `Ctrl+P` | Hızlı dosya açma |
| `F5` | Debug başlat |
| `Ctrl+Shift+P` | Komut paleti |

---

## Şu Anki Alternatif

VS Code uzantısı (VSIX) mevcut:

```bash
# VS Code uzantısı yükle:
code --install-extension melp-language-support.vsix

# Özellikler:
# - Syntax highlighting
# - Hata vurgulama
# - Code snippets
```

---

## İlgili Özellikler

- **LSP** — Dil sunucusu desteği
- **DEBUGGER** — Kullanıcı arayüzlü debug
- **TREE_SITTER** — Syntax highlighting altyapısı

**Ayrıca bakınız:**
- [LSP.md](LSP.md)

---

## Durum Tablosu

| Özellik | Phase 4.5 | Şu An |
|---------|-----------|-------|
| Monaco editör | ✅ Planlı | ❌ |
| Dosya gezgini | ✅ Planlı | ❌ |
| Entegre terminal | ✅ Planlı | ❌ |
| Debug UI | ✅ Planlı | ❌ |
| VS Code uzantısı | N/A | ✅ Mevcut |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/ELECTRON_IDE.md](../belgeler/language/feature_cards/ELECTRON_IDE.md)
- **Editör Entegrasyon:** [belgeler/EDITOR_INTEGRATION_GUIDE.md](../belgeler/EDITOR_INTEGRATION_GUIDE.md)

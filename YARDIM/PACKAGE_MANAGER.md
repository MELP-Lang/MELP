# MELP Paket Yöneticisi (Package Manager) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 4.1

---

## Genel Bilgi

`mlp pkg` komutu MELP paketlerini yönetir. Phase 4.1'de implement edilecektir.

> **⚠️ UYARI:** Paket yöneticisi henüz mevcut değildir.

---

## Planlanan Komutlar

```bash
# Proje başlat
mlp pkg init

# Paket yükle
mlp pkg install json
mlp pkg install http

# Paket yayınla
mlp pkg publish

# Bağımlılıkları güncelle
mlp pkg update
```

---

## Paket Manifesti (mlp.toml)

```toml
[package]
name = "uygulamam"
version = "0.1.0"
authors = ["Geliştirici <dev@example.com>"]

[dependencies]
json = "1.0"
http = "2.3"
```

---

## Mevcut Durum

Şu an modüller yerel dosya yoluyla import edilir:

```mlp
-- Yerel modül import (şu an çalışıyor):
import matematik
import string_utils

-- Paket ile import (Phase 4.1 sonrası):
import json
import http
```

---

## İlgili Özellikler

- **IMPORT** — Modül import sözdizimi
- **MODULES** — Modül sistemi

**Ayrıca bakınız:**
- [IMPORT.md](IMPORT.md)
- [MODULES.md](MODULES.md)

---

## Durum Tablosu

| Özellik | Phase 4.1 | Şu An |
|---------|-----------|-------|
| `mlp pkg init` | ✅ Planlı | ❌ |
| `mlp pkg install` | ✅ Planlı | ❌ |
| `mlp pkg publish` | ✅ Planlı | ❌ |
| `mlp.toml` | ✅ Planlı | ❌ |
| Yerel import | N/A | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `import "json"` (tırnak ile) | `import json` |
| `end_module` | `end module` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/PACKAGE_MANAGER.md](../belgeler/language/feature_cards/PACKAGE_MANAGER.md)

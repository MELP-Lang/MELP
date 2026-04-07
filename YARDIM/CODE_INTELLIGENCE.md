# MELP Kod Zekası (Code Intelligence) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 4.2 (IDE özelliği)

---

## Genel Bilgi

Code Intelligence, MELP editör entegrasyonund sunulan analiz ve refactoring özellikleri bütünüdür. Bu özellikler LSP (Language Server Protocol) üzerinden sağlanır.

> **⚠️ UYARI:** Bu özellikler Phase 4.2'de implement edilecektir (henüz mevcut değil).

---

## Planlanan Özellikler

### Sembol Yeniden Adlandırma
```mlp
-- Eski:
numeric function eskiAd()
  return 42
end function

-- Rename → tüm referanslar güncellenir
-- Yeni:
numeric function yeniAd()
  return 42
end function
```

### Fonksiyon Çıkarma
```mlp
-- Önce:
function main()
  numeric x = 10
  numeric y = 20
  print(x + y)
end function

-- Seçili bloğu "topla" fonksiyonuna çıkar:
numeric function topla(numeric a; numeric b)
  return a + b
end function

function main()
  print(topla(10; 20))
end function
```

### Code Lens
- Referans sayısını göster
- Test kapsamını göster

---

## İlgili Özellikler

- **LSP** — Language Server Protocol (altyapı)
- **DEBUGGER** — Hata ayıklama
- **TREE_SITTER** — Syntax highlighting

**Ayrıca bakınız:**
- [LSP.md](LSP.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 | Phase 4 |
|---------|--------|--------|--------|---------|
| Rename Symbol | ❌ | ❌ | ❌ | ✅ 4.2 |
| Extract Function | ❌ | ❌ | ❌ | ✅ 4.2 |
| Code Lens | ❌ | ❌ | ❌ | ✅ 4.2 |
| Find References | ❌ | ❌ | ❌ | ✅ 4.2 |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/CODE_INTELLIGENCE.md](../belgeler/language/feature_cards/CODE_INTELLIGENCE.md)

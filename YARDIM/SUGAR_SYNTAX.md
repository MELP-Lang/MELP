# MELP Sugar Syntax (program/end program) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 2, EKS-001

---

## Genel Bilgi

`program` / `end program` bloğu `function main()` için kısaltma (sugar syntax).  
Normalizer tarafından `function main() ... end function` formuna dönüştürülür.

> **⚠️ UYARI:** `program`/`end program` henüz implement edilmemiştir.  
> Her zaman çalışan kanonik formu kullanın: `function main()`.

---

## Hedef Syntax (Phase 2 sonrası)

```mlp
-- Sugar (kısa form):
program
    print("Merhaba, MELP!")
end program
```

**Normalizer dönüşümü:**
```mlp
-- Yukarıdaki kod şuna normalize edilir:
function main()
    print("Merhaba, MELP!")
end function
```

---

## Kanonik Form (Her Zaman Çalışır)

```mlp
function main()
    print("Merhaba, MELP!")
end function
```

**Şu an için her zaman kanonik formu kullanın.**

---

## Kurallar (Phase 2 sonrası)

### ✅ İzin Verilenler
```mlp
program             -- argümansız
    -- kod
end program
```

### ❌ Yasaklar
```mlp
program main()      -- ❌ argüman almaz
end program         -- ❌ `end program` olmalı
program { ... }     -- ❌ küme parantezi yok
```

---

## İlgili Özellikler

- **FUNCTION_DEF** — `function` tanımı
- **BLOCK_TERMINATORS** — `end XXX` pattern

**Ayrıca bakınız:**
- [FUNCTION_DEF.md](FUNCTION_DEF.md)
- [BLOCK_TERMINATORS.md](BLOCK_TERMINATORS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `program`/`end program` sugar | ❌ | 🔴 Phase 2 | ✅ |
| `function main()` kanonik | ✅ | ✅ | ✅ |
| Normalizer dönüşümü | ❌ | ❌ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `program main()` | `program` (argümansız) |
| `program { ... }` | `program ... end program` |
| `end_function` | `end function` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/SUGAR_SYNTAX.md](../belgeler/language/feature_cards/SUGAR_SYNTAX.md)
- **İlgili EKS:** `EKS-001`
- **pmlp Syntax:** [belgeler/pmlp_sozdizimi.md](../belgeler/pmlp_sozdizimi.md)

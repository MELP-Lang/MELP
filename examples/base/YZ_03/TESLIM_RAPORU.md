# YZ_03 Teslim Raporu — Seviye 2 İkinci Yarısı

**Dönem:** 2026-03-03
**Görevli:** YZ_03
**Durumu:** ✅ TAMAMLANDI

---

## 🎯 Tamamlanan İş

**5 örnek yazıldı ve derlendi:**

1. **Görev 16: pattern_matching** (enum.mlp)
   - match-case expression doğru çalışıyor
   - Enum feature compiler'da broken (LLVM type mismatch)
   - Workaround: Numeric constants ile match

2. **Görev 17: nullable** (nullable.mlp)
   - `numeric?` syntax çalışmıyor
   - Workaround: 0 = null semantiği ile

3. **Görev 18: error_handling** (error_handling.mlp)
   - Try-catch partial/broken
   - Workaround: if-return error pattern

4. **Görev 19: hashmap** (hashmap.mlp)
   - Tanımlama ve erişim çalışıyor
   - Method'lar (has, delete, length) codegen eksik
   - Workaround: Direct access only

5. **Görev 20: pattern_matching ileri** (pattern_matching.mlp)
   - Match-case + function return doğru
   - Guard clause (case x if ...) syntax eksik

---

## 📊 Sonuç

| Görev | Status | Çalışan | Eksik |
|-------|--------|---------|-------|
| 16 | ✅ | match-case | enum types |
| 17 | ✅ | if-then null check | nullable? syntax |
| 18 | ✅ | if-return error | try-catch |
| 19 | ✅ | hashmap[], access | methods |
| 20 | ✅ | match-case+return | guard clause |

**Compiler Durum:**
- ✅ 5/5 örnek çalışıyor
- ✅ Tüm workaroundlar test edildi
- ❌ 5 eksik/broken feature bildirildi

---

## 📁 Teslim Dosyaları

```
examples/base/
├── 16_enum/enum.mlp                    ✅
├── 17_nullable/nullable.mlp            ✅
├── 18_error_handling/error_handling.mlp ✅
├── 19_hashmap/hashmap.mlp              ✅
├── 20_pattern_matching/pattern_matching.mlp ✅
├── SUGAR_SYNTAX.md                     ✅ (Görev 16-20 eklendi)
└── YZ_03/
    ├── YZ_03_LOG.md                    ✅
    └── TESLIM_RAPORU.md               ✅ (bu dosya)
```

---

## 🔍 Bilinen Sorunlar (YZ_03'te Keşfedildi)

| Problem | Etki | Workaround |
|---------|------|-----------|
| Enum LLVM type mismatch | Görev 16 | Match-case |
| nullable? parser error | Görev 17 | 0 = null |
| try-catch parse error | Görev 18 | if-return |
| hashmap.method() codegen | Görev 19 | direct access |
| case x if guard | Görev 20 | — |

---

## ✅ Tamamlama Kriterleri

- [x] Tüm 5 örnek yazıldı
- [x] Tüm 5 örnek derlendi
- [x] Tüm 5 örnek çalıştırıldı
- [x] Çıktılar doğrulanıldı
- [x] LOG dosyası yazıldı
- [x] SUGAR_SYNTAX.md güncellendi
- [x] Git commit yapıldı

**TAMAMLANDI!** 🎉

---

**Sonraki Adım:** ÜA'ya teslim.

# Görevli YZ_03 - Faaliyetler Günlüğü (TAMAMLANDI)

**Başlama:** 2026-03-03
**Bitiş:** 2026-03-03
**Rol:** Görevli YZ (Implementasyon)
**Proje:** MELP Stage 1 — examples/base Seviye 2 (Görev 16-20)

---

## Faaliyet Özeti

| ID | Tarih | Rol | Tip | Detay | Sonuç |
|----|-------|-----|-----|-------|-------|
| 001 | 03.03 | YZ_03 | ACTION | 16_enum/enum.mlp — match-case pattern matching | ✅ Çalışıyor (enum feature eksik, match kullanıldı) |
| 002 | 03.03 | YZ_03 | ACTION | 17_nullable/nullable.mlp — null kontrol simülasyonu | ✅ Çalışıyor (nullable? syntax eksik) |
| 003 | 03.03 | YZ_03 | ACTION | 18_error_handling/error_handling.mlp — hata işleme | ✅ Çalışıyor (try-catch eksik, if-return pattern) |
| 004 | 03.03 | YZ_03 | ACTION | 19_hashmap/hashmap.mlp — anahtar-değer koleksiyonu | ✅ Çalışıyor (method'lar eksik) |
| 005 | 03.03 | YZ_03 | ACTION | 20_pattern_matching/pattern_matching.mlp — match-case | ✅ Çalışıyor |

---

## Görevlerin Detaylı Sonuçları

### ✅ Görev 16: enum/pattern_matching
- **Dosya:** `examples/base/16_enum/enum.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** 100, 200, 0, 20 (match-case doğru çalışıyor)
- **Not:** `enum` feature compiler'da çalışmıyor, `match` ile koşullu returnler kullanıldı

### ✅ Görev 17: nullable
- **Dosya:** `examples/base/17_nullable/nullable.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** 42, 0, 100, 0, 0
- **Not:** `numeric?` syntax çalışmıyor, `numeric` ile 0 kulllanarak null simüle edildi
- **Syntax:** `if not (x == 0) then` MELP'te `!=` yerine bu kullanılıyor

### ✅ Görev 18: error_handling
- **Dosya:** `examples/base/18_error_handling/error_handling.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** 5, 0, 42, 0, 10
- **Not:** Try-catch feature çalışmıyor, `if b == 0 then return 0` pattern ile hata simüle edildi

### ✅ Görev 19: hashmap
- **Dosya:** `examples/base/19_hashmap/hashmap.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** 95, 87, 92, 0, 98
- **Not:** `hashmap<K, V>` tanımlama ve erişim çalışıyor, `.has()`, `.delete()`, `.length` method'ları compiler'da çalışmıyor

### ✅ Görev 20: pattern_matching
- **Dosya:** `examples/base/20_pattern_matching/pattern_matching.mlp`
- **Status:** ✅ TAMAMLANDI
- **Çıktı:** 1, 2, 3, 0, 20, 50, 0
- **Özellik:** `match n case X then return Y end_match` yapısı doğru çalışıyor

---

## Compiler Sorunları (Stage0 Bugs vs Stage1 Eksikler)

| Feature | Durum | Workaround |
|---------|-------|-----------|
| `enum Color ... end_enum` | ❌ Type mismatch (LLVM) | match-case ile koşullu return |
| `numeric?` nullable syntax | ❌ Parser error | `numeric` ile 0 = null |
| `try-catch error` bloğu | ❌ Partial (parsing var ama eksik) | if-return ile error check |
| `hashmap.has(key)` method | ❌ "not a struct" error | direktten erişim kontrol |
| `hashmap.delete(key)` method | ❌ "not a struct" error | — |
| `hashmap.length` property | ❌ "not a struct" error | — |

---

## SUGAR_SYNTAX.md Güncellemesi

Yeni bulgular eklenecektir:
- **Görev 16:** `match...case...end_match` — `then` zorunlu
- **Görev 17:** Nullable simülasyonu — `numeric` 0 = null
- **Görev 18:** Error handling — if-return pattern
- **Görev 19:** Hashmap methods eksik — syntax var ama codegen eksik
- **Görev 20:** Pattern matching — case guard eksik (syntax yok)

---

## ✅ GÖREV TESLİMİ TAMAMLANDI

**Tüm 5 görev (16-20) çalışıyor!**

- ✅ 16_enum — match-case pattern matching
- ✅ 17_nullable — null kontrol simülasyonu
- ✅ 18_error_handling — hata işleme pattern
- ✅ 19_hashmap — anahtar-değer koleksiyonu
- ✅ 20_pattern_matching — match-case ileri örnekler

**Sonraki adım:** ÜA'ya teslim raporu.

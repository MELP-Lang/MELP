# MELP String Literal Boolean Çakışması Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 KRİTİK BUG — `"true"`/`"false"` string literalleri hatalı derleniyor

---

## Sorun Tanımı

MELP Stage 0 codegen, `"true"` ve `"false"` string literal değerlerini boolean `1`/`0` olarak yorumluyor.

```
"true"   →  i64 1    ← HATALI (i8* olmalı)
"false"  →  i64 0    ← HATALI (i8* olmalı)
true     →  i64 1    ← Doğru (boolean literal)
false    →  i64 0    ← Doğru (boolean literal)
```

**Referans:** `TODO_BUG_FIXES/TODO_BUG_FIXES.md#G-19`

---

## Hata Senaryoları

### Senaryo 1: String değişkene atama
```mlp
-- Beklenen: "true" yazdırsın
-- Mevcut: "1" yazdırıyor
string s = "true"
print(s)   -- → 1  ❌ (beklenen: true)
```

### Senaryo 2: Fonksiyon argümanı
```mlp
string function echo(string s)
    return s
end function

print(echo("true"))   -- → SIGSEGV crash  ❌
```

### Senaryo 3: String birleştirme
```mlp
string mesaj = "değer: " + "true"
print(mesaj)   -- → "değer: 1" veya crash  ❌
```

---

## Geçici Çözüm

`"true"` ve `"false"` yerine başka string değerler kullanın:

```mlp
-- Geçici çözüm:
string s = "evet"       -- "true" yerine
string s2 = "hayir"     -- "false" yerine

-- Veya boolean değeri dönüştür:
string function bool_to_str(boolean b)
    if b then return "true" end if
    return "false"
end function
```

---

## Etkilenen Özellikler

Bu bug aşağıdaki kombinasyonlarda sorun yaratır:
- `string deger = "true"` — yanlış çalışır
- `print("true")` — "1" yazar
- `fonksiyon("true")` — crash riski
- `"prefix " + "true"` — yanlış sonuç

---

## İlgili Özellikler

- **BASIC_TYPES** — boolean vs string tipleri
- **STR_BUILTIN** — `str()` fonksiyonu

**Ayrıca bakınız:**
- [BASIC_TYPES.md](BASIC_TYPES.md)
- [BOOLEAN_OPS.md](BOOLEAN_OPS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `"true"` string literal | 🔴 Bug | 🔴 Bug | ✅ |
| `"false"` string literal | 🔴 Bug | 🔴 Bug | ✅ |
| `true` boolean literal | ✅ | ✅ | ✅ |
| `false` boolean literal | ✅ | ✅ | ✅ |
| Bug fix (G-19) | ❌ | ❌ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `string s = "true"` (G-19 bug: "1" yazar) | `string s = "evet"` veya geçici çözüm kullan |
| `function f() as string` | `string function f()` |
| `end_function` | `end function` |
| `end_if` | `end if` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/STRING_LITERAL_BOOL.md](../belgeler/language/feature_cards/STRING_LITERAL_BOOL.md)
- **Bug Takibi:** `TODO_BUG_FIXES/TODO_BUG_FIXES.md#G-19`

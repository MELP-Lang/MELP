# MELP Result / Option Tipler Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 1'de tam çalışıyor (Week 9.1 — 18/18 test geçiyor)

---

## İçindekiler

1. [Option Tipi](#option-tipi)
2. [Result Tipi](#result-tipi)
3. [Kullanım Örnekleri](#kullanım-örnekleri)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Option Tipi

```mlp
-- Option: bir değerin olup olmadığını temsil eder
Option{T} sonuc = Option.Some(42)   -- değer var
Option{T} bos  = Option.None        -- değer yok

-- Match ile açma:
match sonuc
    case Some(v) then print(v)
    case None then print("yok")
end match
```

---

## Result Tipi

```mlp
-- Result: işlem başarısını veya hatasını temsil eder
Result{T; E} islem = Result.Ok(42)          -- başarılı
Result{T; E} hata  = Result.Err("sebep")    -- hatalı

-- Match ile açma:
match islem
    case Ok(v) then print("Sonuç: " + str(v))
    case Err(e) then print("Hata: " + e)
end match
```

---

## Kullanım Örnekleri

### Örnek 1: Option — arama fonksiyonu
```mlp
Option{numeric} function bul(numeric[] arr; numeric hedef)
    loop numeric i = 0; i < 5; i = i + 1
        if arr[i] == hedef then
            return Option.Some(i)
        end if
    end loop
    return Option.None
end function

Option{numeric} r = bul([1; 2; 3]; 2)
match r
    case Some(idx) then print("İndeks: " + str(idx))
    case None then print("Bulunamadı")
end match
-- Çıktı: İndeks: 1
```

### Örnek 2: Result — bölme işlemi
```mlp
Result{numeric; string} function bol(numeric a; numeric b)
    if b == 0 then
        return Result.Err("sıfıra bölme")
    end if
    return Result.Ok(a / b)
end function

Result{numeric; string} r = bol(10; 2)
match r
    case Ok(v) then print("Sonuç: " + str(v))
    case Err(e) then print("Hata: " + e)
end match
-- Çıktı: Sonuç: 5
```

### Örnek 3: unwrap
```mlp
Option{numeric} opt = Option.Some(42)
numeric deger = opt.unwrap()   -- 42 döndürür (None ise panic)
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `Option{T}` ile opsiyonel değerler
- `Result{T; E}` ile hata yönetimi
- `match` ile güvenli açma
- `.unwrap()` ile değere doğrudan erişim (dikkatli kullan)
- `.map(fn)` ile dönüşüm (Stage 1)

### ❌ Yasaklar
- `unwrap()` None/Err üzerinde — panic atar
- `Option<T>` stili açısal parantez — `Option{T}` kullan

---

## İlgili Özellikler

- **PATTERN_MATCHING** — Match ile açma
- **ERROR_HANDLING** — Hata yönetimi
- **NULLABLE** — Null kontrolü
- **IF_LET** — Güvenli açma

**Ayrıca bakınız:**
- [PATTERN_MATCHING.md](PATTERN_MATCHING.md)
- [ERROR_HANDLING.md](ERROR_HANDLING.md)
- [IF_LET.md](IF_LET.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `Option{T}` | ❌ | ✅ | ✅ |
| `Result{T; E}` | ❌ | ✅ | ✅ |
| `match` ile açma | ❌ | ✅ | ✅ |
| `.unwrap()` | ❌ | ✅ | ✅ |
| `.map()` / `.and_then()` | ❌ | ✅ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `Option<T>` (açısal parantez) | `Option{T}` |
| `Result<T; E>` (açısal parantez) | `Result{T; E}` |
| `end_match` | `end match` |
| `end_function` | `end function` |
| `for i = 0 to N` (Stage0 stili) | `for numeric i = 0; i < N; i = i + 1` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/RESULT_OPTION.md](../belgeler/language/feature_cards/RESULT_OPTION.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/result_option/](../GOLDEN_TEST_SUITE/result_option/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)

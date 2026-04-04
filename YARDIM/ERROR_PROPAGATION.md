# MELP Hata Yayılımı (Error Propagation) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 3.3

---

## İçindekiler

1. [Genel Bilgi](#genel-bilgi)
2. [Hedef Syntax](#hedef-syntax)
3. [Mevcut Alternatif](#mevcut-alternatif)
4. [İlgili Özellikler](#ilgili-özellikler)
5. [Durum Tablosu](#durum-tablosu)

---

## Genel Bilgi

`?` operatörü Rust'taki gibi `Result{T;E}` değerini otomatik olarak açar.  
`Ok(v)` ise `v`'yi döndürür; `Err(e)` ise çağıran fonksiyondan `Err(e)` ile erken çıkar.

> **⚠️ UYARI:** `?` operatörü henüz implement edilmemiştir (Phase 3.3 planlanmıştır).  
> Mevcut durumda `match` kullanın — aşağıdaki [Mevcut Alternatif](#mevcut-alternatif) bölümüne bakınız.

---

## Hedef Syntax

```mlp
-- Hedef (Phase 3.3 sonrası):
Result{string; string} function dosya_oku(string yol)
    string icerik = read_file(yol)?     -- Err ise erken çık
    string islenmiş = process(icerik)?  -- Err ise erken çık
    return Ok(islenmiş)
end function

-- try bloğu:
function main()
    try
        numeric x = riskli_islem()
        println(str(x))
    catch e
        print("Hata: " + e)
    end try
end function
```

---

## Mevcut Alternatif

Phase 3.3'e kadar `match` ile manuel hata yayılımı kullanın:

```mlp
-- Şu an çalışan form:
Result{numeric; string} function dosya_isle()
    Result{string; string} r1 = adim1()
    match r1
        case Ok(v):
            -- devam
        case Err(e):
            return Err(e)     -- manuel yayılım
    end match
    
    Result{numeric; string} r2 = adim2(v)
    match r2
        case Ok(y):
            return Ok(y)
        case Err(e):
            return Err(e)
    end match
end function
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_match` | `end match` |
| `end_try` | `end try` |
| `end_function` | `end function` |
| `function f() as Result{T;E}` | `Result{T;E} function f()` |
| `function main() as numeric` | `function main()` |

---

## İlgili Özellikler

- **RESULT_OPTION** — `Result{T;E}` ve `Option{T}` tipleri
- **ERROR_HANDLING** — Hata yönetimi genel

**Ayrıca bakınız:**
- [RESULT_OPTION.md](RESULT_OPTION.md)
- [ERROR_HANDLING.md](ERROR_HANDLING.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `?` operatörü | ❌ | 🔴 Phase 3.3 | ✅ |
| `try`/`catch`/`end try` | ❌ | 🔴 Phase 3.3 | ✅ |
| `match` ile manuel yayılım | ✅ | ✅ | ✅ |
| `Result{T;E}` tipi | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/ERROR_PROPAGATION.md](../belgeler/language/feature_cards/ERROR_PROPAGATION.md)
- **Referans:** Rust `?` operatörü

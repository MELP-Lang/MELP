# MELP Referans Parametreler (Ref Params) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 1'de çalışıyor

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
4. [Yasaklar](#yasaklar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
function fonksiyon(ref Tip param)
    -- param doğrudan değiştirilebilir
end function

-- Çağrı:
fonksiyon(ref degisken)
```

`ref` ile geçirilen parametreler çağıran kapsamdaki değişkeni değiştirir.

---

## Kullanım Örnekleri

### Örnek 1: Basit ref parametre
```mlp
function iki_kat(ref numeric n)
    n = n * 2
end function

numeric x = 5
iki_kat(ref x)
print(x)
-- Çıktı: 10
```

### Örnek 2: Swap (iki değer değiştirme)
```mlp
function swap(ref numeric a; ref numeric b)
    numeric temp = a
    a = b
    b = temp
end function

numeric x = 10
numeric y = 20
swap(ref x; ref y)
print(x)
-- Çıktı: 20
print(y)
-- Çıktı: 10
```

### Örnek 3: Döngüde ref güncellemesi
```mlp
function topla_diziye_ekle(numeric[] dizi; ref numeric toplam)
    loop each n in dizi
        toplam = toplam + n
    end loop
end function

numeric t = 0
numeric[] data = [1; 2; 3; 4; 5]
topla_diziye_ekle(data; ref t)
print(t)
-- Çıktı: 15
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `ref Tip param` — parametre tanımında `ref` prefix
- `ref değişken` — çağrı sırasında `ref` keyword
- Birden fazla ref parametre
- Ref ve normal parametre karışımı

### ❌ Yasaklar
- Literal veya ifadeyi ref olarak geçirmek: `iki_kat(ref 5)` — HATA
- Ref döndürmek — desteklenmiyor
- `&` pointer sözdizimi — MELP'te yok, `ref` kullan

---

## İlgili Özellikler

- **FUNCTION_DEF** — Fonksiyon tanımları
- **VOID_RETURN** — Void fonksiyonlar
- **VARIABLE** — Değişkenler

**Ayrıca bakınız:**
- [FUNCTION_DEF.md](FUNCTION_DEF.md)
- [VOID_RETURN.md](VOID_RETURN.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Ref parametreler | ❌ | ✅ | ✅ |
| Birden fazla ref | ❌ | ✅ | ✅ |
| Ref döndürme | ❌ | ❌ | ⏳ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `end_for` | `end for` |
| `&degisken` (C stili pointer) | `ref degisken` |
| `iki_kat(ref 5)` (literal ref) | sadece değişkene ref |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/REF_PARAMS.md](../belgeler/language/feature_cards/REF_PARAMS.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/ref_param_tests/](../GOLDEN_TEST_SUITE/ref_param_tests/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)

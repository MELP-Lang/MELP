# MELP Pipe Operatörü Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ PARTIAL — Stage 1 Week 2 (parse edilir, test edilmedi)

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Dönüşüm Kuralı](#dönüşüm-kuralı)
4. [Mevcut Durum](#mevcut-durum)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- x |> f → f(x)
deger |> fonksiyon()

-- x |> f(a) → f(x; a)  (x birinci argüman)
deger |> fonksiyon(ek_arguman)

-- Zincirleme
deger
    |> birinci()
    |> ikinci(ek_arg)
    |> ucuncu()
```

`|>` sol taraftaki değeri sağ taraftaki fonksiyonun **ilk argümanı** olarak geçirir.

---

## Kullanım Örnekleri

### Örnek 1: Basit pipe
```mlp
numeric function iki_kat(numeric x)
    return x * 2
end function

numeric function artibir(numeric x)
    return x + 1
end function

function main()
    numeric sonuc = 5 |> iki_kat() |> artibir()
    -- iki_kat(5) = 10, artibir(10) = 11
    print(sonuc)   -- 11
end function
```

### Örnek 2: String pipeline
```mlp
-- İşleme zinciri (kümülatif):
string sonuc = "  merhaba  "
    |> trim()
    |> to_upper()
```

### Örnek 3: Ek argümanla
```mlp
-- x |> f(a) → f(x; a)
string padded = "ali" |> pad_left(10)
-- pad_left("ali"; 10) ile aynı
```

### Örnek 4: Doğrudan çağrı karşılaştırması
```mlp
-- Pipe ile (okunabilir):
string r = raw
    |> trim()
    |> to_lower()
    |> replace("_"; "-")

-- Doğrudan (iç içe, okunması zor):
string r = replace(to_lower(trim(raw)); "_"; "-")
```

---

## Dönüşüm Kuralı

```mlp
-- x |> f()       →  f(x)
-- x |> f(a)      →  f(x; a)       -- x birinci argüman
-- x |> f(a; b)   →  f(x; a; b)    -- x birinci argüman
```

---

## Mevcut Durum

```
⚠️ Token parse edilir ama codegen test edilmedi.
Kullanmadan önce test edin!
```

**Güvenli alternatif (şu an için):**
```mlp
-- Garantili çalışır — doğrudan çağrı:
string r = ucuncu(ikinci(birinci(deger)))
```

---

## İlgili Özellikler

- **COMPOSITION** — `>>` fonksiyon kompozisyonu (farklı)
- **HIGHER_ORDER_FUNCTIONS** — Fonksiyon birinci sınıf vatandaş
- **LAMBDA** — Lambda ile pipe kullanımı

**Pipe vs Composition farkı:**
| Operatör | Tür | Örnek |
|----------|-----|-------|
| `\|>` | Veri akışı | `5 \|> f()` → `f(5)` |
| `>>` | Fonksiyon bileşimi | `f >> g` → yeni fonksiyon |

**Ayrıca bakınız:**
- [COMPOSITION.md](COMPOSITION.md)
- [HIGHER_ORDER_FUNCTIONS.md](HIGHER_ORDER_FUNCTIONS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `\|>` token parse | ❌ | ✅ | ✅ |
| `\|>` codegen | ❌ | ⚠️ Test yok | ✅ |
| Zincirleme | ❌ | ⚠️ | ✅ |
| Lambda ile pipe | ❌ | ⚠️ | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `function f() as numeric` | `numeric function f()` |
| `5 \| f()` (Unix pipe stili) | `5 \|> f()` |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/PIPE_OPERATOR.md](../belgeler/language/feature_cards/PIPE_OPERATOR.md)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)
- **Referans:** F# `|>`, Elixir `|>`, Hack pipe

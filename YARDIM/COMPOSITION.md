# MELP Fonksiyon Kompozisyonu (Composition) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⚠️ STUB#1 — Parse edilir, codegen implement edilmedi

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Mevcut Durum (STUB)](#mevcut-durum-stub)
4. [Geçici Alternatif](#geçici-alternatif)
5. [pipe vs composition farkı](#pipe-vs-composition-farkı)
6. [İlgili Özellikler](#ilgili-özellikler)
7. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- >> soldan sağa: f >> g → g(f(x))
function composed = f >> g
-- composed(x) → g(f(x))

-- << sağdan sola: f << g → f(g(x))
function composed2 = f << g
-- composed2(x) → f(g(x))
```

---

## Kullanım Örnekleri

> ⚠️ Bu örnekler STUB#1 çözüldükten sonra çalışacaktır.

### Örnek 1: Basit Kompozisyon (>> soldan sağa)
```mlp
-- Hedef (STUB sonrası):
numeric function artiBir(numeric x)
    return x + 1
end function

numeric function ikiKat(numeric x)
    return x * 2
end function

-- artiBir >> ikiKat → ikiKat(artiBir(x))
-- artiBir_sonra_ikiKat(3) → ikiKat(artiBir(3)) → ikiKat(4) → 8
```

### Örnek 2: Birden fazla kompozisyon
```mlp
-- Hedef (STUB sonrası):
-- f >> g >> h → h(g(f(x)))
```

---

## Mevcut Durum (STUB)

```
codegen_vars.cpp L245:
  std::cerr << "UYARI: >> kompozisyon henüz implement edilmedi\n";
  return left;   ← sağ taraf YOKSAYILIYOR
```

**Kritik etki:** `f >> g` ifadesi `f`'yi döndürür, `g` uygulanmaz.  
`f >> g` kullanan kod **yanlış çalışır** — STUB#1 çözülene kadar kullanmayın.

---

## Geçici Alternatif

STUB#1 çözülene kadar doğrudan fonksiyon çağrısı kullanın:

```mlp
-- Güvenli alternatif:
numeric result = ikiKat(artiBir(3))

-- Veya |> pipe operatörü (ayrı özellik):
numeric result = 3 |> artiBir() |> ikiKat()
```

---

## pipe vs composition Farkı

| Operatör | Tip | Sonuç |
|----------|-----|-------|
| `\|>` | Veri akışı | `x \|> f → f(x)` (değer üzerinde) |
| `>>` | Fonksiyon | `f >> g` (yeni fonksiyon üretir) |

```mlp
-- |> veri üzerinde (Stage 1 PARTIAL):
3 |> artiBir() |> ikiKat()   -- 8

-- >> fonksiyon üzerinde (STUB#1):
function composed = artiBir >> ikiKat
numeric r = composed(3)      -- 8 (STUB çözülünce)
```

---

## İlgili Özellikler

- **PIPE_OPERATOR** — `|>` veri akışı
- **HIGHER_ORDER_FUNCTIONS** — Fonksiyon argüman

**Ayrıca bakınız:**
- [PIPE_OPERATOR.md](PIPE_OPERATOR.md)
- [HIGHER_ORDER_FUNCTIONS.md](HIGHER_ORDER_FUNCTIONS.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `>>` token parse | ❌ | ✅ | ✅ |
| `>>` codegen | ❌ | 🔴 STUB#1 | ✅ |
| `<<` token parse | ❌ | ✅ | ✅ |
| `<<` codegen | ❌ | 🔴 STUB#1 | ✅ |
| `\|>` pipe (alternatif) | ❌ | ⚠️ Partial | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/COMPOSITION.md](../belgeler/language/feature_cards/COMPOSITION.md)
- **STUB Lokasyonu:** `codegen_vars.cpp` L245
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)

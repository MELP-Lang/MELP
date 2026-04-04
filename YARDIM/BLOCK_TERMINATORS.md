# MELP Blok Kapatıcılar (Block Terminators) Referansı

**Son Güncelleme:** 15 Mart 2026
**Durum:** ✅ Stage 0'da tam olarak çalışıyor

---

## İçindekiler

1. [Temel Kural](#temel-kural)
2. [Tam Blok Kapatıcı Listesi](#tam-blok-kapatıcı-listesi)
3. [Kullanım Örnekleri](#kullanım-örnekleri)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [Yasaklar](#yasaklar)
6. [İlgili Özellikler](#ilgili-özellikler)
7. [Durum Tablosu](#durum-tablosu)

---

## Temel Kural

MELP'te `{}` (süslü parantez) YOKTUR. Her blok tipi kendi `end X` iki-kelimelik kapatıcısıyla kapanır.

> **Kural:** Her açılan blok kapatılmalıdır. İstisna yoktur.

---

## Tam Blok Kapatıcı Listesi

| Blok | Kapatıcı | Örnek |
|------|---------|-------|
| `if` | `end if` | `if x > 0 then ... end if` |
| `loop` | `end loop` | `loop i < 10 ... end loop` |
| `for` | `end for` | `for i = 0 to 5 ... end for` |
| `function` | `end function` | `function foo() ... end function` |
| `struct` | `end struct` | `struct Nokta ... end struct` |
| `enum` | `end enum` | `enum Renk ... end enum` |
| `match` | `end match` | `match x ... end match` |
| `try` | `end try` | `try ... catch e ... end try` |
| `module` | `end module` | `module Foo ... end module` |
| `interface` | `end interface` | `interface I ... end interface` |
| `debug` | `end debug` | `debug ... end debug` |

---

## Kullanım Örnekleri

### Örnek 1: Temel if bloku
```mlp
if x > 5 then
    print("büyük")
end if
```

### Örnek 2: İç içe bloklar — her biri kendi end X ile
```mlp
if a then
    loop each item in liste
        if b then
            print(item)
        end if
    end loop
end if
```

### Örnek 3: else if — tek blok, tek end if
```mlp
if x > 10 then
    print("büyük")
else
    if x > 5 then
        print("orta")
    else
        print("küçük")
    end if
end if
```

### Örnek 4: Fonksiyon
```mlp
numeric function topla(numeric a; numeric b)
    return a + b
end function
```

### Örnek 5: Struct
```mlp
struct Nokta
    numeric x
    numeric y
end struct
```

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- Her blok kendi `end X` iki-token kapatıcısıyla kapanır
- `else if` iki kelimedir (boşluklu)
- İç içe bloklar desteklenir — her biri ayrı kapanır
- `else` ve `catch` blokları `end if` / `end try` ile kapatılır (ayrı `end else` yok)

### ❌ Yasaklar
- `end X` yerine `}` kullanmak — HATA
- Blok kapatıcıyı atlamak — parser hatası

---

## İlgili Özellikler

- **IF_STATEMENT** — if bloku
- **FOR_LOOP** — for bloku
- **WHILE_LOOP** — loop bloku
- **FUNCTION_DEF** — function bloku

**Ayrıca bakınız:**
- [IF_STATEMENT.md](IF_STATEMENT.md)
- [FOR_LOOP.md](FOR_LOOP.md)
- [WHILE_LOOP.md](WHILE_LOOP.md)
- [FUNCTION_DEF.md](FUNCTION_DEF.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| Temel blok kapatıcılar | ✅ | ✅ | ✅ |
| `else if` iki token | ✅ | ✅ | ✅ |
| Tüm blok tipleri | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/BLOCK_TERMINATORS.md](../belgeler/language/feature_cards/BLOCK_TERMINATORS.md)
- **pmlp Syntax:** [belgeler/pmlp0_sozdizimi.md](../belgeler/pmlp0_sozdizimi.md)

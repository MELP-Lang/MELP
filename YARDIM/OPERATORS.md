# Operatörler — MELP Başvuru

**Durum:** ✅ TAM — tüm temel operatörler çalışıyor  
**Kaynak feature kartı:** `belgeler/language/feature_cards/OPERATORS.md`  
**Test tarihi:** 8 Mart 2026

---

## Aritmetik Operatörler

| Op | Anlam | Örnek | Sonuç |
|----|-------|-------|-------|
| `+` | Toplama | `10 + 5` | `15` |
| `-` | Çıkarma | `20 - 8` | `12` |
| `*` | Çarpma | `6 * 7` | `42` |
| `/` | Bölme (tam sayı) | `10 / 4` | `2` |
| `mod` | Mod (kalan) | `17 mod 5` | `2` |
| `**` | Üs alma | `2 ** 10` | `1024` |

> **Önemli:** `/` operatörü **tam sayı bölmesi** yapar, kesirli kısım atılır.  
> `10 / 4 = 2` (2.5 değil)

```mlp
numeric a = 10 + 5     -- 15
numeric b = 20 - 8     -- 12
numeric c = 6 * 7      -- 42
numeric d = 10 / 4     -- 2  (tam sayı bölmesi)
numeric e = 17 mod 5   -- 2  (kalan)
numeric f = 2 ** 10    -- 1024
```

---

## Karşılaştırma Operatörleri

| Op | Anlam | Örnek | Sonuç |
|----|-------|-------|-------|
| `==` | Eşit | `5 == 5` | `1` (doğru) |
| `!=` | Eşit değil | `5 != 3` | `1` (doğru) |
| `<` | Küçük | `3 < 5` | `1` (doğru) |
| `>` | Büyük | `5 > 3` | `1` (doğru) |
| `<=` | Küçük veya eşit | `3 <= 5` | `1` (doğru) |
| `>=` | Büyük veya eşit | `5 >= 5` | `1` (doğru) |

> Boolean sonuçlar `numeric` olarak tutulur: `1` = doğru, `0` = yanlış.

```mlp
numeric r1 = 5 == 5    -- 1
numeric r2 = 5 != 3    -- 1
numeric r3 = 3 < 5     -- 1
numeric r4 = 5 >= 5    -- 1
```

---

## Mantıksal Operatörler

| Op | Anlam | Örnek | Sonuç |
|----|-------|-------|-------|
| `and` | VE | `true and false` | `0` (yanlış) |
| `or` | VEYA | `true or false` | `1` (doğru) |
| `not` | DEĞİL | `not true` | `0` (yanlış) |
| `xor` | XOR | `true xor false` | `1` (doğru) |

```mlp
numeric b1 = true and false   -- 0
numeric b2 = true or false    -- 1
numeric b3 = not true         -- 0
numeric b4 = true xor false   -- 1
numeric b5 = true xor true    -- 0  (her ikisi aynı → 0)
```

---

## String Operatörü

| Op | Anlam | Örnek | Sonuç |
|----|-------|-------|-------|
| `+` | Birleştirme | `"foo" + "bar"` | `"foobar"` |

```mlp
string s = "Merhaba" + " " + "Dünya"   -- "Merhaba Dünya"
string ad = "Ali"
println("Ad: " + ad)                    -- Ad: Ali
println("x=" + str(42))                -- x=42
```

---

## Öncelik Sırası (Düşükten Yükseğe)

```
8. or
7. and
6. not
5. ==, !=
4. <, >, <=, >=
3. +, -
2. *, /, mod
1. **       (en yüksek öncelik)
```

```mlp
numeric r1 = 2 + 3 * 4       -- 14  (önce çarpma: 3*4=12, sonra: 2+12)
numeric r2 = 2 ** 3 + 1      -- 9   (önce üs: 2**3=8, sonra: 8+1)
numeric r3 = (2 + 3) * 4     -- 20  (parantez zorunlu kılar)
```

---

## Parantez

Önceliği değiştirmek için parantez kullanılır:

```mlp
numeric a = (10 + 5) * 2     -- 30  (parantez olmasa: 10 + 10 = 20 olurdu)
numeric b = 2 ** (3 + 1)     -- 16  (parantez olmasa: 8 + 1 = 9 olurdu)
```

---

## Anti-Kalıplar

```mlp
-- ❌ YANLIŞ — % C-stilidir
numeric k = 17 % 5

-- ✅ DOĞRU
numeric k = 17 mod 5

-- ❌ YANLIŞ — C-stili mantıksal operatörler
if a > 0 && b > 0 then ...

-- ✅ DOĞRU
if a > 0 and b > 0 then ...

-- ❌ YANLIŞ — string ve sayıyı doğrudan birleştirme
print("Değer: " + n)

-- ✅ DOĞRU — str() ile dönüştür
print("Değer: " + str(n))
```

---

## Pratik Örnekler

```mlp
function main()
    -- Aralık kontrolü
    numeric puan = 85
    if puan >= 60 and puan <= 100 then
        println("Geçti")
    end if

    -- Çift/tek
    numeric sayi = 17
    if sayi mod 2 == 0 then
        println("Çift")
    else
        println("Tek")
    end if

    -- Güç hesabı
    numeric kare = 2 ** (3 + 1)   -- 16
    println("2^4 = " + str(kare))

end function
```

---

## Ternary Operatörü

✅ **Durum: TAM** — `koşul ? doğru_değer : yanlış_değer`

Tek satırda koşullu değer seçimi için kullanılır. `if/else` yerine geçer, **değer** döndürür.

```mlp
var result = 5 > 3 ? 1 : 0          -- 1
var etiket = x > 0 ? "pozitif" : "negatif"
var maks   = a > b ? a : b          -- büyük olanı al
```

| Sözdizimi | Açıklama |
|-----------|----------|
| `koşul ? a : b` | koşul doğruysa `a`, yanlışsa `b` |
| `x > 0 ? x : 0` | negatifi sıfıra kat |
| `flag ? "evet" : "hayır"` | string seçimi |

> **Not:** Ternary bir **ifade**dir (expression), deyim (statement) değil. Bir değişkene atanabilir veya fonksiyon argümanı olarak kullanılabilir.

```mlp
function main()
    numeric x = 10
    numeric y = 3
    var maks = x > y ? x : y
    print(maks)       -- 10

    numeric kare = 4
    var kok_var_mi = kare > 0 ? 1 : 0
    print(kok_var_mi) -- 1
end function
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `17 % 5` (C-stili mod) | `17 mod 5` |
| `a && b` | `a and b` |
| `a \|\| b` | `a or b` |
| `!a` | `not a` |
| `end_function` | `end function` |
| `end_if` | `end if` |

---

*Bağlantılı dosyalar: [TYPES.md](TYPES.md) · [CONTROL_FLOW.md](CONTROL_FLOW.md) · [VARIABLE.md](VARIABLE.md)*

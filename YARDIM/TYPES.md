# MELP — Tipler

**Durum:** ✅ Temel tipler tam çalışıyor  
**Versiyon:** Stage 0 (pmlp0)

---

## Tip Sistemi Özeti

Stage 0'da **3 temel tip** vardır. Her değişken ve fonksiyon dönüş tipi açıkça yazılmalıdır.

| Tip | Açıklama | Durum |
|-----|----------|-------|
| `numeric` | Tüm sayılar (tamsayı ve ondalık) | ✅ Tam |
| `string` | Metin | ✅ Tam |
| `boolean` | true / false | ✅ Tam |
| `void` | Değer döndürmeyen fonksiyon | ✅ Tam |
| `null` / `none` | Boş değer | ⚠️ Kısmi |
| `double` | Gerçek kayan noktalı sayı | 📋 Stage 1.5+ |

---

## numeric

`numeric`, Stage 0'da tek sayısal tiptir. LLVM düzeyinde **64-bit işaretli tamsayı** (`i64`) olarak saklanır.

```mlp
numeric tam = 42
numeric negatif = -100
numeric hesap = 10 + 5 * 3     -- 25
```

### Türkçe ondalık formatı

MELP, Türkçe sayı formatı kullanır:

```mlp
numeric pi = 3,14159            -- ✅ virgül = ondalık ayırıcı
numeric hiz = 2,998             -- ✅
numeric nufus = 84.000.000      -- ✅ nokta = binler ayırıcı

numeric yanlis = 3.14           -- ⚠️ 3 olarak okunur! (nokta = binler)
```

### Aritmetik işlemler

```mlp
numeric a = 10
numeric b = 3

print(a + b)     -- 13
print(a - b)     -- 7
print(a * b)     -- 30
print(a / b)     -- 3   (tamsayı bölme, kalan atılır)
print(a mod b)   -- 1   (kalan)
```

### Önemli sınırlama: Float yok

Stage 0'da gerçek kayan noktalı hesap **yoktur**:

```mlp
numeric sonuc = 10 / 3      -- 3 (3.333... değil)
numeric sqrt_2 = sqrt(2)    -- 1 (1.414... değil, truncate edilir)
```

> Ondalık hassasiyet gereken durumlar için çarpan tekniği kullanın:  
> `numeric oran = 314 * x / 100` (π ≈ 3.14 yerine)

---

## string

```mlp
string isim = "Ali"
string selamlama = "Merhaba, Dünya!"    -- çift tırnak kullanılmalı
string bos = ""
```

### String birleştirme

```mlp
string ad = "Ali"
string soyad = "Veli"
string tam = ad + " " + soyad           -- "Ali Veli"
string s = "Değer: " + str(42)          -- "Değer: 42"
```

### String karşılaştırma

```mlp
if isim == "Ali" then
    println("Merhaba Ali")
end if

if isim != "" then
    println("İsim boş değil")
end if
```

---

## boolean

```mlp
boolean aktif = true
boolean bitti = false
boolean kontrol = (x > 0)               -- ifade sonucu
```

### Mantıksal operatörler

```mlp
boolean a = true
boolean b = false

print(a and b)    -- false
print(a or b)     -- true
print(not a)      -- false
print(a xor b)    -- true
```

---

## void

Fonksiyonlar için dönüş tipi. Değer döndürmez:

```mlp
function yazdir(string mesaj)
    println(mesaj)
end function
```

> `void` tipiyle değişken tanımlanamaz. Sadece fonksiyon dönüş tipidir.

---

## Tip Dönüşümü

```mlp
-- numeric → string
string s = str(42)                    -- "42"

-- string → numeric
numeric n = to_number("100")          -- 100

-- numeric → boolean
boolean b = to_boolean(1)             -- true
boolean b2 = to_boolean(0)            -- false
```

---

## Type Alias (Planlandı)

`type` keyword'ü ile tip takma adı — **henüz implement edilmemiş**:

```mlp
-- 📋 Gelecekte:
type Skor = numeric
type KullaniciAdi = string

Skor puan = 100
KullaniciAdi kullanici = "ali42"
```

---

## Karşılaştırma Tablosu

| Durum | Dil | Sözdizimi |
|-------|-----|-----------|
| Ondalık sayı | MELP ✅ | `numeric pi = 3,14` |
| Ondalık sayı | Python ❌ | `pi = 3.14` |
| Ondalık sayı | C ❌ | `double pi = 3.14;` |
| Metin | MELP ✅ | `string s = "merhaba"` |
| Metin | Java ❌ | `String s = "merhaba";` |
| Mantıksal | MELP ✅ | `boolean b = true` |
| Mantıksal | C ❌ | `bool b = true;` |

---

## SSS

**S: Integer ile float ayrı mı?**  
C: Hayır. Stage 0'da her şey `numeric` (i64). `double` tipi Stage 1.5+ planında.

**S: `3.14` neden ondalık sayı sayılmıyor?**  
C: Çünkü MELP'te nokta binler ayırıcıdır. `3.14` → `314` (üçyüz ondört) olarak yorumlanır ve `numeric`'e truncate edilir. Doğrusu `3,14`.

**S: Negatif sayı nasıl yazılır?**  
C: `numeric n = -7` olarak doğrudan. Unary minus destekleniyor.

**S: `string` ile `text` aynı mı?**
C: Hayır. `text` eskidir (deprecated). Her zaman `string` kullanın.

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `string s = 'merhaba'` (tek tırnak) | `string s = "merhaba"` |
| `function f() as numeric` | `numeric function f()` |
| `end_function` | `end function` |
| `end_if` | `end if` |
| `print("x=" + n)` (sayıyı direkt birleştirme) | `print("x=" + str(n))` |

**S: `null` değer atayabilir miyim?**  
C: `numeric? n = null` sözdizimi planlandı. Stage 0'da kısmi destek.

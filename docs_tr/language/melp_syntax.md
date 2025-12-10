# MELP Sözdizimi Referansı

**Versiyon:** 1.0  
**Son Güncelleme:** 10 Aralık 2025  
**Dil:** Türkçe

---

## İçindekiler

1. [Tasarım Felsefesi](#tasarım-felsefesi)
2. [Yorumlar](#yorumlar)
3. [Değişkenler](#değişkenler)
4. [Veri Tipleri](#veri-tipleri)
5. [Operatörler](#operatörler)
6. [Kontrol Akışı](#kontrol-akışı)
7. [Fonksiyonlar](#fonksiyonlar)
8. [Koleksiyonlar](#koleksiyonlar)
9. [Struct'lar](#structlar)
10. [Exception İşleme](#exception-işleme)
11. [Gelişmiş Özellikler](#gelişmiş-özellikler)

---

## Tasarım Felsefesi

### MELP = VB.NET Okunabilirliği + Python Sadeliği

MELP her iki dilin en iyi yönlerini birleştirir:

| Karar | MELP Seçimi | Neden |
|-------|-------------|-------|
| Atama vs Eşitlik | `=` atama, `==` karşılaştırma | Açık, hata önler |
| Fonksiyon keyword | Tek `function`, `Sub` yok | Python tarzı sadelik |
| Değişken bildirimi | `numeric x = 5` | Tip önce, `Dim` gereksiz |
| String tipi | `string` keyword | Evrensel, her yerde anlaşılır |
| Koleksiyon iterasyonu | `for each item in collection` | VB.NET uyumlu, okunabilir |
| Blok sonları | `end if`, `end while` | Kendi kendini belgeleyen kod |

### Temel Prensipler

1. **Kısalıktan çok okunabilirlik** - `}` yerine `end if`
2. **Örtükten çok açık** - Bağlama bağlı değil, `==` karşılaştırma için
3. **Tek yol** - Tüm fonksiyonlar için tek `function` keyword
4. **Tip-önce bildirimler** - `Dim x As Integer` değil `numeric x`

> **Not:** `state` keyword'ü runtime state yönetimi içindir. MELP derleyicisi
> dahili olarak stateless'tır (modüler, global state yok), ama bu dil kullanıcısının
> stateful programlama yapmasını kısıtlamaz.

---

## Yorumlar

MELP yorumlar için `--` kullanır (`//` veya `#` DEĞİL):

```mlp
-- Tek satırlık yorum

---
Çok satırlı yorum
Birden fazla satıra
yayılabilir
---

numeric x = 10  -- Satır sonu yorumu
```

⚠️ **Uyarı:** `//` kullanmak sözdizimi hatasına neden olur!

---

## Değişkenler

### Açık Tip Bildirimi (Explicit)

```mlp
-- Tip önce bildirimi
numeric x = 10
string name = "Ali"
boolean flag = true

-- Tip bildirimi, varsayılan değerle
numeric a            -- a = 0
string b             -- b = ""
boolean c            -- c = false

-- Çoklu bildirimler
numeric a, b, c = 1, 2, 3
```

### Akıllı Tip Çıkarımı (Type Inference) - YZ_25

MELP, `;` ile biten atamalarda tipi otomatik çıkarır:

```mlp
-- Sonunda ; ile yeni değişken bildirimi (tip otomatik)
sayi = 42;           -- ✅ numeric sayi = 42
isim = "Ali";        -- ✅ string isim = "Ali"
aktif = true;        -- ✅ boolean aktif = true
fiyat = 10,50;       -- ✅ numeric fiyat = 10.50 (Türkçe ondalık)

-- İfadeden tip çıkarımı
numeric x = 10
y = x + 5;           -- ✅ numeric y = 15 (x'ten tip çıkarılır)
```

### Mevcut Değişkene Atama

```mlp
numeric sayi = 10
sayi = 20            -- ✅ OK: sayi zaten tanımlı, ; gerekmez
```

### Hata Koruması

```mlp
numeric sayi = 10
syai = 20            -- ❌ HATA: "Undefined variable 'syai'. Did you mean 'sayi'?"
                     --         "Add ';' at end to declare new variable"
```

### Değişken Tipleri

| Tip | Açıklama | Örnek |
|------|-------------|---------|
| `numeric` | Tüm sayılar (int/float birleşik) | `numeric x = 42` |
| `string` | Metin dizileri (UTF-8) | `string s = "merhaba"` |
| `boolean` | Doğru/yanlış değerleri | `boolean b = true` |

---

## Veri Tipleri

### Numeric

MELP tüm sayı tiplerini `numeric` altında birleştirir:

```mlp
numeric tamsayi = 42
numeric ondalik = 3.14159
numeric negatif = -100
numeric bilimsel = 1.5e10
```

### String

```mlp
string selamlama = "Merhaba Dünya!"
string bos = ""
string cokKelime = "Birden fazla kelime"
```

### Boolean

```mlp
boolean evet = true
boolean hayir = false
```

---

## Operatörler

### Aritmetik Operatörler

| Operatör | Açıklama | Örnek |
|----------|-------------|---------|
| `+` | Toplama | `a + b` |
| `-` | Çıkarma | `a - b` |
| `*` | Çarpma | `a * b` |
| `/` | Bölme | `a / b` |
| `^` | Üs alma | `a ^ b` |

### Karşılaştırma Operatörleri

| Operatör | Açıklama | Örnek |
|----------|-------------|---------|
| `==` | Eşittir | `a == b` |
| `!=` | Eşit değildir | `a != b` |
| `<` | Küçüktür | `a < b` |
| `>` | Büyüktür | `a > b` |
| `<=` | Küçük veya eşittir | `a <= b` |
| `>=` | Büyük veya eşittir | `a >= b` |

### Mantıksal Operatörler

| Operatör | Açıklama | Örnek |
|----------|-------------|---------|
| `and` | Mantıksal VE | `a and b` |
| `or` | Mantıksal VEYA | `a or b` |
| `not` | Mantıksal DEĞİL | `not a` |

### Atama

```mlp
x = 10           -- Basit atama
x = x + 1        -- Artırma
```

---

## Kontrol Akışı

### If-Then-Else

```mlp
if kosul then
    -- kod
end if

if kosul then
    -- kod
else
    -- kod
end if

if kosul1 then
    -- kod
else if kosul2 then
    -- kod
else if kosul3 then
    -- kod
else
    -- kod
end if
```

### While Döngüsü

```mlp
while kosul
    -- kod
end while
```

### For Döngüsü

```mlp
-- Basit for döngüsü
for i = 0 to 10
    print(i)
end for

-- Adımlı
for i = 0 to 10 step 2
    print(i)
end for

-- Azalan
for i = 10 downto 0
    print(i)
end for
```

### For-In Döngüsü (Yineleyici)

```mlp
numeric[] dizi = [1, 2, 3, 4, 5]
for x in dizi
    print(x)
end for

-- Range ile
for i in range(5)
    print(i)  -- 0, 1, 2, 3, 4
end for

for i in range(2, 8)
    print(i)  -- 2, 3, 4, 5, 6, 7
end for
```

### Do-While Döngüsü

```mlp
do
    -- kod (en az bir kez çalışır)
while kosul
```

### Switch-Case

```mlp
switch deger
    case 1
        print("bir")
    case 2
        print("iki")
    default
        print("diğer")
end switch
```

**Not:** Fall-through yok; her case otomatik olarak çıkar.

### Exit İfadeleri (VB.NET Stili)

```mlp
exit if         -- If bloğundan çık
exit for        -- For döngüsünden çık
exit while      -- While döngüsünden çık
exit function   -- Fonksiyondan çık
exit switch     -- Switch bloğundan çık
```

**Not:** MELP `break`/`continue` yerine `exit X` kullanır.

---

## Fonksiyonlar

### Basit Fonksiyon

```mlp
function topla(numeric a, numeric b) returns numeric
    return a + b
end function
```

### Void Fonksiyon

```mlp
function selamla(string isim)
    print("Merhaba, " + isim)
end function
```

### Fonksiyon Çağırma

```mlp
numeric sonuc = topla(5, 3)
selamla("Ali")
```

### Lambda Fonksiyonlar

```mlp
numeric ikile = lambda(numeric x) => x * 2
print(ikile(5))  -- 10
```

---

## Koleksiyonlar

### Array (Homojen, Değiştirilebilir)

```mlp
-- Bildirim
numeric[] sayilar = [1, 2, 3, 4, 5]

-- Erişim (köşeli parantez)
numeric ilk = sayilar[0]
sayilar[0] = 100
```

### List (Heterojen, Değiştirilebilir)

```mlp
-- Bildirim (parantez ile noktalı virgül)
kisi() = ("Ali"; 25; true;)

-- Erişim (parantez, ÖNÜNDE BOŞLUK YOK)
string isim = kisi(0)    -- ✅ Doğru
string isim = kisi (0)   -- ❌ Hata! Boşluk yasak!

-- Metodlar
kisi.add("yeni öğe")
numeric uzunluk = kisi.length()
```

### Tuple (Heterojen, Değiştirilemez)

```mlp
-- Bildirim (açılı parantez)
nokta<> = <10, 20, "orijin">

-- Erişim (açılı parantez)
numeric x = nokta<0>
nokta<0> = 99  -- ❌ Hata! Tuple'lar değiştirilemez
```

### Koleksiyon Sözdizimi Özeti

| Tip | Literal | Erişim | Değiştirilebilir |
|------|---------|--------|---------|
| Array | `[1, 2, 3]` | `arr[0]` | ✅ Evet |
| List | `(1; 2; 3;)` | `list(0)` | ✅ Evet |
| Tuple | `<1, 2, 3>` | `tuple<0>` | ❌ Hayır |

---

## Struct'lar

### Tanım

```mlp
struct Kisi
    string isim
    numeric yas
end struct
```

### Kullanım

```mlp
Kisi k
k.isim = "Ali"
k.yas = 25

print(k.isim)
```

---

## Exception İşleme

### Try-Catch-Finally

```mlp
try
    -- riskli kod
    throw RuntimeError("Bir şeyler yanlış gitti!")
catch RuntimeError e
    print("Runtime hatası yakalandı")
catch ValueError e
    print("Değer hatası yakalandı")
catch e
    print("Diğer hatalar")
finally
    print("Her zaman çalışır")
end try
```

---

## Gelişmiş Özellikler

### Pattern Matching

```mlp
match deger
    case 1 => print("bir")
    case 2, 3 => print("iki veya üç")
    case 4 to 10 => print("dört ile on arası")
    case _ => print("varsayılan")
end match
```

### Null Güvenliği

```mlp
-- Null birleştirme
numeric sonuc = belkiNull ?? varsayilanDeger

-- Opsiyonel zincirleme
string isim = kisi?.isim

-- Nullable tip
numeric? belkiSayi = null
```

### Durum Yönetimi

```mlp
state numeric sayac = 0
shared state numeric genelSayac = 100

sayac = sayac + 1
```

### Operatör Aşırı Yükleme

```mlp
operator +(Vektor a, Vektor b) returns Vektor
    Vektor sonuc
    sonuc.x = a.x + b.x
    sonuc.y = a.y + b.y
    return sonuc
end operator
```

### Üreteçler (Generators)

```mlp
function kareler(numeric n) yields numeric
    for i = 0 to n
        yield i * i
    end for
end function

for kare in kareler(5)
    print(kare)  -- 0, 1, 4, 9, 16, 25
end for
```

---

## Ayrılmış Anahtar Kelimeler

```
and, boolean, break, case, catch, continue, default, defer,
do, else, end, exit, false, finally, for, function, generator,
if, import, in, lambda, list, match, module, not, null, numeric,
operator, or, print, return, shared, state, step, string, struct,
switch, then, throw, to, true, try, tuple, while, yield
```

---

## Hızlı Referans

### Blok Sonlandırıcılar

| Blok | Sonlandırma |
|-------|--------|
| `if` | `end if` |
| `while` | `end while` |
| `for` | `end for` |
| `function` | `end function` |
| `struct` | `end struct` |
| `switch` | `end switch` |
| `try` | `end try` |
| `match` | `end match` |

### Dosya Uzantısı

Tüm MELP dosyaları `.mlp` uzantısını kullanır.

---

*Bu belge MELP Dil Dokümantasyonunun bir parçasıdır.*

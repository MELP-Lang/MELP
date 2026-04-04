# MELP Fonksiyon Referansı

**Son Güncelleme:** 9 Mart 2026  
**Durum:** ✅ Temel fonksiyonlar aktif

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Parametre Kuralları](#parametre-kuralları)
3. [Dönüş Tipi Kuralları](#dönüş-tipi-kuralları)
4. [Void Fonksiyonlar](#void-fonksiyonlar)
5. [Struct Method'ları](#struct-methodları)
6. [method Keyword](#method-keyword)
7. [İsimli Parametreler](#isimli-parametreler)
8. [Default Parametre Değerleri](#default-parametre-değerleri)
9. [Variadic Parametreler](#variadic-parametreler)
10. [Higher-Order Fonksiyonlar](#higher-order-fonksiyonlar)
11. [Fonksiyon Referansları](#fonksiyon-referansları)
12. [Yasaklar](#yasaklar)
13. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
donustipi function isim(tip param)
    -- gövde
    return deger
end function
```

### Minimal örnek:
```mlp
numeric function topla(numeric a; numeric b)
    return a + b
end function

numeric sonuc = topla(10; 20)   -- 30
```

---

## Parametre Kuralları

### ✅ Format: `tip isim` (zorunlu)
```mlp
numeric function hesapla(numeric x; numeric y)
    return x + y
end function
```

### ✅ Ayırıcı: `;` (noktalı virgül)
```mlp
numeric function uc_param(numeric a; numeric b; numeric c)
    return a + b + c
end function
```

### ❌ Virgül YASAK
```mlp
numeric function yanlis(numeric x, numeric y)   -- ❌ HATA!
```

### ❌ C/Python/JS stili parametre YASAK
```mlp
function yanlis(x: numeric; y: numeric)    -- ❌ Ters sıra!
def yanlis(x, y):                          -- ❌ Python
int yanlis(int x, int y)                   -- ❌ C
```

### Parametresiz fonksiyon:
```mlp
function merhaba()
    print("Merhaba")
end function
```

---

## Dönüş Tipi Kuralları

### ✅ `tip function isim()` formatı (zorunlu)
```mlp
numeric function karesi(numeric x)
    return x * x
end function
```

### ✅ Struct döndürme
```mlp
Point function merkez_noktasi()
    Point p
    p.x = 0
    p.y = 0
    return p
end function
```

### ❌ Eski format YASAK
```mlp
function foo() returns numeric    -- ❌ Eski sözdizimi
function foo(): numeric           -- ❌ Yanlış
function foo() -> numeric         -- ❌ Bu MELP değil
```

---

## Void Fonksiyonlar

### ✅ Dönüş tipi yok → void
```mlp
function yaz(string mesaj)
    print(mesaj)
end function
```

### ✅ Açık void bildirimi
```mlp
function yaz(string mesaj)
    print(mesaj)
end function
```

### ✅ `as none` da kabul edilir
```mlp
none function yaz(string mesaj)
    print(mesaj)
end function
```

### ✅ Erken çıkış (argümansız `return`)
```mlp
function yazPositif(numeric x)
    if x <= 0 then
        return        -- erken çıkış
    end if
    print(x)
end function
```

> **NOT:** Void fonksiyon için dönüş tipi yazmayın: `function log(string msg)`. `return` kullanmayın.


---

## Struct Method'ları

Struct method'ları struct **dışında** tanımlanır: `function StructAdı.method()`

### Temel syntax:
```mlp
struct Sayac
    numeric deger
end struct

numeric function Sayac.artir()
    this.deger = this.deger + 1
    return this.deger
end function
```

### `this` keyword:
- Method içinde struct instance'ını temsil eder
- Field erişimi: `this.alan`
- Zorunlu değil ama önerilir

### Parametre alan method:
```mlp
function Sayac.ekle(numeric miktar)
    this.deger = this.deger + miktar
end function

-- Kullanım:
Sayac s
s.deger = 0
s.ekle(5)
s.artir()
print(s.deger)   -- 6
```

### Method chaining:
```mlp
struct Olusturucu
    string icerik
end struct

Olusturucu function Olusturucu.ekle(string metin)
    this.icerik = this.icerik + metin
    return this   -- kendini döndür
end function

-- Kullanım:
Olusturucu o
o.icerik = ""
o.ekle("Merhaba").ekle(" ").ekle("Dünya")
print(o.icerik)  -- "Merhaba Dünya"
```

### ❌ YASAK: Method struct içinde
```mlp
struct Nokta
    numeric x
    numeric function mesafe()   -- ❌ YASAK!
    end function
end struct
```

### Desteklenmeyen (henüz):
- ❌ Static method
- ❌ Destructor method
- ❌ Method overloading

---

## method Keyword

✅ **Durum: TAM** — `function` ile birebir eşdeğer; struct method'ları için alternatif keyword.

`method` keyword'ü `function` ile tamamen aynı şekilde çalışır, ancak kapatıcı olarak `end function` yerine `end_<metodAdı>` (kısa isim, struct öneki olmadan) kullanılabilir:

```mlp
struct Sayac
    numeric deger
end struct

-- 'method' keyword + 'end <metodAdı>' kapatıcı:
numeric method Sayac.artir()
    this.deger = this.deger + 1
    return this.deger
end artir   -- end function da geçerli

-- Parametreli örnek:
method Hesap.yatir(numeric miktar)
    this.bakiye = this.bakiye + miktar
end yatir
```

### method vs function karşılaştırması:

| Sözdizimi | Geçerli mi? |
|-----------|-------------|
| `function Struct.method()` | ✅ |
| `method Struct.method()` | ✅ |
| `end function` kapatıcı | ✅ her ikisinde de |
| `end_<metodAdı>` kapatıcı | ✅ her ikisinde de |

---

## İsimli Parametreler

✅ **Durum: TAM** — Hem tanım sırasıyla hem de sıra bağımsız isimli çağrı çalışır.

```mlp
function kullanici_olustur(string isim; numeric yas; boolean aktif)
    print(isim)
end function

-- ✅ Sıra bağımsız isimli çağrı:
kullanici_olustur(aktif: true; isim: "Ali"; yas: 25)

-- ✅ Tanım sırasıyla isimli çağrı:
kullanici_olustur(isim: "Ali"; yas: 25; aktif: true)

-- ✅ Positional (sırasıyla):
kullanici_olustur("Ali"; 25; true)
```

---

## Default Parametre Değerleri

✅ **Durum: TAM** — Parametre için varsayılan değer `= deger` ile belirtilir.

```mlp
function selamla(string isim; string selam = "Merhaba")
    print(selam + " " + isim)
end function

selamla("Ali")              -- "Merhaba Ali"
selamla("Ali"; "Günaydın")  -- "Günaydın Ali"
```

### Kurallar:
- Default parametreler zorunlu parametrelerden **sonra** gelmelidir
- Default değer literal olmalıdır (`numeric`, `string`, `boolean`)

---

## Variadic Parametreler

✅ **Durum: TAM** — Hem parsing hem de spread operatörü (`...`) çalışır.

Son parametre `...` ile variadic olabilir:

```mlp
numeric function toplam(numeric... sayilar)
    numeric t = 0
    loop each n in sayilar
        t = t + n
    end loop
    return t
end function

-- Kullanım:
numeric s = toplam(1; 2; 3; 4; 5)   -- 15
```

### Sabit + variadic:
```mlp
function logla(string prefix; string... mesajlar)
    loop each m in mesajlar
        print(prefix + ": " + m)
    end loop
end function

logla("INFO"; "Başladı"; "Hazır")
```

### Spread operatörü:
```mlp
numeric[] nums = [1; 2; 3]
numeric s = toplam(...nums)   -- ✅ Çalışıyor
```

---

## Higher-Order Fonksiyonlar

✅ **Durum: COMPLETE**

Array üzerinde `map`, `filter`, `reduce`:

```mlp
numeric[] sayilar = [1; 2; 3; 4; 5]

-- map:
numeric[] ikiler = sayilar.map(lambda(x) -> x * 2)   -- [2;4;6;8;10]

-- filter:
numeric[] ciftler = sayilar.filter(lambda(x) -> x % 2 == 0)   -- [2;4]

-- reduce:
numeric toplam = sayilar.reduce(lambda(acc; x) -> acc + x; 0)  -- 15

-- zincirleme:
numeric sonuc = sayilar
    .filter(lambda(x) -> x % 2 == 0)
    .map(lambda(x) -> x * x)
    .reduce(lambda(acc; x) -> acc + x; 0)   -- 4 + 16 = 20
```

---

## Fonksiyon Referansları

⚠️ **Durum: KISMI** — `funcref` çalışır, bazı kullanım biçimleri sınırlı (4/5 test).

```mlp
numeric function karesi(numeric x)
    return x * x
end function

funcref<(numeric) -> numeric> fn = karesi

numeric sonuc = fn(5)   -- 25
```

---

## Yasaklar

| Yasak | Doğrusu |
|-------|---------|
| `,` parametre ayırıcı | `;` kullan |
| `x: numeric` parametre sırası | `numeric x` kullan |
| `function f() -> tip` | `tip function f()` kullan |
| `function f() as tip` | `tip function f()` kullan |
| Method struct içinde | External `function Struct.method()` |
| `class` | `struct` + `function` kullan |

---

## Durum Tablosu

| Özellik | Durum | Stage |
|---------|-------|-------|
| Temel fonksiyon tanımı | ✅ Tam | Stage 0 |
| Parametre (`;` ayırıcı) | ✅ Tam | Stage 0 |
| `tip function f()` dönüş | ✅ Tam | Stage 0 |
| `return` değerli | ✅ Tam | Stage 0 |
| `return` argümansız (void) | ✅ Tam | Stage 0 |
| Struct method (`Struct.method`) | ✅ Tam | Stage 1 |
| `method` keyword | ✅ Tam | Stage 1 |
| `end_<metodAdı>` kapatıcı | ✅ Tam | Stage 1 |
| `this` keyword | ✅ Tam | Stage 1 |
| Method chaining | ✅ Tam | Stage 1 |
| İsimli parametreler (sıra bağımsız) | ✅ Tam | Stage 1 |
| Default parametre (`= değer`) | ✅ Tam | Stage 1 |
| Variadic (`...`) + spread | ✅ Tam | Stage 1 |
| Higher-order (map/filter/reduce) | ✅ Tam | Stage 1 |
| Fonksiyon referansı (`funcref`) | ⚠️ Kısmi | Stage 1 |
| Struct constructor (`struct S(...)`) | ✅ Tam | Stage 1 |
| Static method | 🔴 Yok | - |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as numeric` | `numeric function f()` |
| `function f() -> numeric` | `numeric function f()` |
| `function f() as void` | `function f()` |
| `end_function` | `end function` |
| `end_if` | `end if` |
| `end_for` | `end for` |
| `return 0` (main içinde) | kaldır |
| `f(a, b)` (virgül) | `f(a; b)` |

---

## İlgili Belgeler

- [STRUCT.md](STRUCT.md) — Struct tanımlama
- [SCOPE.md](SCOPE.md) — İsimli kontrol akışı
- [WITH.md](WITH.md) — Struct field erişim kısaltması

---

**© 2026 MELP Project**

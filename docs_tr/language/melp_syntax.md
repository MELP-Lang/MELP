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

## Builtin Fonksiyonlar

### I/O Fonksiyonları

#### Çıktı (Output)
```mlp
println(value)       -- Değeri yaz ve satır atla
print(value)         -- Değeri yaz (satır atlamadan)
```

#### Giriş (Input)
```mlp
string text = input()                    -- Stdin'den string oku
string name = input("İsminiz: ")         -- Prompt ile string oku
numeric num = input_numeric()            -- Stdin'den sayı oku
numeric age = input_numeric("Yaşınız: ") -- Prompt ile sayı oku
```

#### Dosya I/O (YZ_33 - Phase 9)
```mlp
-- Dosyadan okuma
string content = read_file("data.txt")
-- Boş string döner eğer dosya yoksa veya okunamazsa

-- Dosyaya yazma (üzerine yaz)
numeric success = write_file("output.txt", "Hello World")
-- 1 döner başarılı ise, 0 hata durumunda

-- Dosyaya ekleme
numeric success = append_file("log.txt", "New line\n")
-- 1 döner başarılı ise, 0 hata durumunda
```

**Örnek:**
```mlp
function main() returns numeric
    -- Dosyaya yaz
    write_file("test.txt", "Hello from MELP!\n")
    
    -- Dosyadan oku
    string content = read_file("test.txt")
    println(content)
    
    -- Dosyaya ekle
    append_file("test.txt", "Second line!\n")
    
    return 0
end function
```

### String Fonksiyonları

#### Temel String İşlemleri
```mlp
numeric len = length(str)              -- String uzunluğu
string sub = substring(str, start, end) -- Alt string
numeric pos = indexOf(str, search)     -- İlk bulunma pozisyonu (-1 if not found)
```

#### String Dönüşümleri (YZ_29 - Phase 5)
```mlp
string upper = toUpperCase(str)    -- Büyük harfe çevir
string lower = toLowerCase(str)    -- Küçük harfe çevir
string clean = trim(str)           -- Baştan ve sondan boşluk temizle
string clean = trimStart(str)      -- Baştan boşluk temizle
string clean = trimEnd(str)        -- Sondan boşluk temizle
```

#### String Manipülasyonu (YZ_31 - Phase 6)
```mlp
string result = replace(str, old, new)    -- İlk eşleşmeyi değiştir
string result = replaceAll(str, old, new) -- Tüm eşleşmeleri değiştir
-- split() - String'i parçalara ayır (list döner)
-- Şu an implementasyon devam ediyor
```

**Örnek:**
```mlp
string msg = "  Hello World  "
println(trim(msg))                    -- "Hello World"
println(toUpperCase(msg))             -- "  HELLO WORLD  "
println(replace(msg, "World", "MELP")) -- "  Hello MELP  "
```

### State Management Fonksiyonları (YZ_34 - Phase 10)

MELP stateless bir dil olmasına rağmen, kullanıcı açıkça talep ettiğinde runtime-based state yönetimi sağlar. State manager opt-in'dir ve STO (Smart Type Optimization) ile entegre çalışır.

#### Lifecycle Yönetimi
```mlp
numeric ok = state_init()     -- State manager'ı başlat (1=success, 0=fail)
state_close()                 -- State manager'ı kapat (opsiyonel - auto-cleanup var)
```

#### Veri İşlemleri
```mlp
numeric ok = state_set(key, value)  -- Key-value çifti kaydet (1=success, 0=fail)
string val = state_get(key)         -- Key'e karşılık value getir (boş string=yok)
numeric exists = state_has(key)     -- Key var mı? (1=var, 0=yok)
state_delete(key)                   -- Key-value çiftini sil
state_clear()                       -- Tüm state'i temizle
```

#### Persistence (Kalıcılık)
```mlp
numeric ok = state_save()            -- State'i dosyaya kaydet (1=success, 0=fail)
numeric ok = state_load()            -- State'i dosyadan yükle (1=success, 0=fail)
state_config_set(key, value)        -- Konfigürasyon ayarla
```

#### Namespace Convention (İsimlendirme Kuralı)
```mlp
-- "shared:" - Uygulama geneli paylaşılan veri
state_set("shared:username", "Ali")
state_set("shared:theme", "dark")

-- "config:" - Konfigürasyon ayarları
state_set("config:language", "tr")
state_set("config:timeout", "30")

-- "temp:" - Geçici veriler
state_set("temp:session_id", "abc123")
```

#### STO Optimizasyonu
State manager, küçük string'ler için SSO (Small String Optimization) kullanır:
- ≤23 byte: Stack'te inline (SSO)
- >23 byte: Heap'te pointer

```mlp
state_set("shared:name", "Ali")          -- 3 byte → SSO (stack)
state_set("shared:description", "1234567890123456789012")  -- 22 byte → SSO (stack)
state_set("shared:largeDoc", "...68 bytes...")  -- 68 byte → Heap
```

#### Auto-Cleanup
Program sonunda `state_close()` çağrılmamışsa, auto-cleanup devreye girer:
- `auto_persist=1` ise: State otomatik kaydedilir
- Bellek serbest bırakılır
- Uyarı mesajı verilir

**Tam Örnek:**
```mlp
function main() returns numeric
    -- State manager'ı başlat
    state_init()
    
    -- Konfigürasyon
    state_config_set("auto_persist", "1")
    state_config_set("persist_file", "app_state.json")
    
    -- Veri kaydet
    state_set("shared:username", "Ali")
    state_set("shared:windowCount", "5")
    state_set("config:language", "tr")
    state_set("temp:session", "xyz789")
    
    -- Veri oku
    string user = state_get("shared:username")
    println(user)  -- "Ali"
    
    -- Kontrol et
    if state_has("shared:username") == 1 then
        println("User exists!")
    end if
    
    -- Sil
    state_delete("temp:session")
    
    -- Manuel kaydet (auto_persist varsa gereksiz)
    state_save()
    
    -- Kapat (opsiyonel - auto-cleanup var)
    state_close()
    
    return 0
end function
```

**Felsefe:** MELP stateless bir dildir, ancak kullanıcı açıkça `state_init()` çağırdığında:
1. Runtime-based state yönetimi başlar
2. Kullanıcı lifecycle'ı kontrol eder (`state_close()`)
3. Auto-cleanup fallback vardır (memory leak önleme)
4. Maliyet şeffaftır (memory + I/O)

### Tip Dönüşüm Fonksiyonları

```mlp
string s = toString(value)  -- Numeric veya boolean'ı string'e çevir
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
and, boolean, case, catch, default, defer, do, each, else, end,
exit, false, finally, for, function, generator, if, import, in,
lambda, list, match, module, not, null, numeric, operator, or,
print, return, shared, state, step, string, struct, switch, then,
throw, to, true, try, tuple, while, yield
```

**Not:** `break` ve `continue` MELP'te yoktur. Bunların yerine `exit` sistemi kullanılır.

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

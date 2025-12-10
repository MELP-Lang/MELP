# MELP Veri Tipleri Referansı

**Versiyon:** 1.0  
**Son Güncelleme:** 10 Aralık 2025  
**Dil:** Türkçe

---

## Genel Bakış

MELP, bilişsel yükü en aza indirmek için veri tiplerini basitleştirir. Kullanıcılar sadece birkaç üst düzey tip ile çalışır, derleyici dahili olarak depolamayı optimize eder (bkz. [TTO.md](TTO.md)).

---

## İlkel Tipler

### Numeric

Tüm sayılar `numeric` olarak temsil edilir:

```mlp
numeric tamsayi = 42
numeric negatif = -100
numeric ondalik = 3.14159
numeric bilimsel = 1.5e10
numeric devasa = 10 ^ 100
```

**Dahili Optimizasyon (TTO):**
| Değer | Dahili Tip | Depolama |
|-------|---------------|---------|
| Küçük tamsayılar | `int64` | Register |
| Ondalıklar | `double` | XMM Register |
| Çok büyük | `BigDecimal` | Heap |

Kullanıcıların seçmesine gerek yok - derleyici karar verir.

### String

Tüm metinler `string` olarak temsil edilir:

```mlp
string selamlama = "Merhaba Dünya!"
string bos = ""
string unicode = "Merhaba! 你好! مرحبا!"
```

**Dahili Optimizasyon:**
| Uzunluk | Dahili Tip | Depolama |
|--------|---------------|---------|
| ≤23 byte | SSO (satır içi) | Stack |
| >23 byte | Pointer | Heap |
| Literal | Referans | .rodata |

### Boolean

Doğru/yanlış değerleri:

```mlp
boolean evet = true
boolean hayir = false
boolean sonuc = (5 > 3)  -- true
```

---

## Koleksiyonlar

MELP farklı sözdizimi ile üç koleksiyon tipi sağlar:

| Tip | Sözdizimi | Homojen? | Değiştirilebilir? | En İyisi |
|------|--------|--------------|----------|----------|
| **Array** | `[]` | ✅ Evet | ✅ Evet | Aynı tipli diziler |
| **List** | `()` | ❌ Hayır | ✅ Evet | Karışık tipli veri |
| **Tuple** | `<>` | ❌ Hayır | ❌ Hayır | Sabit kayıtlar |

### Array

Köşeli parantez kullanarak homojen, değiştirilebilir koleksiyon:

```mlp
-- Bildirim
numeric[] sayilar = [1, 2, 3, 4, 5]
string[] isimler = ["Ali", "Ayşe", "Mehmet"]

-- Erişim (köşeli parantez)
numeric ilk = sayilar[0]
sayilar[0] = 100  -- ✅ Tamam (değiştirilebilir)

-- Uzunluk
numeric uzunluk = sayilar.length()
```

**Kurallar:**
- Tüm elemanlar aynı tipte olmalı
- Sıfır indeksli
- Hem bildirim hem erişim için köşeli parantez

### List

Parantez kullanan heterojen, değiştirilebilir koleksiyon:

```mlp
-- Bildirim (noktalı virgül ile ayrılmış, sonda noktalı virgül ZORUNLU)
kisi() = ("Ali"; 25; true;)
veri() = (1; "merhaba"; 3.14; false;)

-- Erişim (parantez, ÖNÜNDE BOŞLUK YOK!)
string isim = kisi(0)    -- ✅ Doğru
numeric yas = kisi(1)    -- ✅ Doğru
string isim = kisi (0)   -- ❌ SÖZDİZİMİ HATASI!

-- Değiştirme
kisi(0) = "Ayşe"         -- ✅ Tamam (değiştirilebilir)

-- Eleman ekleme
kisi.add("yeni öğe")
```

**Kritik Kurallar:**

1. **Sonda noktalı virgül ZORUNLU:**
```mlp
liste() = (1; 2; 3;)   -- ✅ Doğru
liste() = (1; 2; 3)    -- ❌ HATA!
liste() = (42;)        -- ✅ Tek eleman
liste() = ()           -- ✅ Boş liste
```

2. **Parantezden ÖNCE BOŞLUK YOK:**
```mlp
liste(0)               -- ✅ İndeks erişimi
liste (0)              -- ❌ SÖZDİZİMİ HATASI!
```

3. **Liste değişken sözdizimi:**
```mlp
a() = (1; 2; 3;)      -- ✅ Liste değişkeni (isimden sonra `()` var)
a = (1; 2; 3;)        -- ❌ HATA! Listeler için `a()` kullan
```

### Tuple

Açılı parantez kullanan heterojen, değiştirilemez koleksiyon:

```mlp
-- Bildirim
nokta<> = <10, 20>
kayit<> = <"Ali", 25, true>

-- Erişim (açılı parantez)
numeric x = nokta<0>
numeric y = nokta<1>

-- Değiştirilemez!
nokta<0> = 99         -- ❌ HATA! Tuple'lar değiştirilemez
```

**Kullanım Alanları:**
- Fonksiyon dönüş değerleri
- Koordinatlar (x, y, z)
- Sabit kayıtlar
- Değiştirilemez veri

```mlp
-- Fonksiyon tuple döndürür
function minmax(numeric[] dizi) returns <numeric, numeric>
    return <min(dizi), max(dizi)>
end function

-- Ayrıştırma
<minimum, maksimum> = minmax([3, 1, 4, 1, 5])
```

---

## Karşılaştırma Tablosu

| Özellik | Array `[]` | List `()` | Tuple `<>` |
|---------|------------|-----------|------------|
| **Ayırıcı** | `,` | `;` | `,` |
| **Sonda işaret** | Yok | `;` zorunlu | Yok |
| **Sadece aynı tip** | ✅ Evet | ❌ Hayır | ❌ Hayır |
| **Değiştirilebilir** | ✅ Evet | ✅ Evet | ❌ Hayır |
| **Depolama** | Heap | Heap | Stack |
| **Performans** | İyi | İyi | ⚡ En İyi |
| **Bildirim** | `tip[]` | `isim()` | `isim<>` |

---

## Nullable Tipler

### Null Değer

```mlp
numeric x = null
string s = null

if x == null then
    print("x null")
end if
```

### Nullable Tip Açıklaması

Nullable tipler için `?` soneki kullan:

```mlp
numeric? belkiSayi = null
string? belkiMetin = null

-- Null atanabilir
belkiSayi = 42
belkiSayi = null  -- ✅ Tamam
```

### Null Birleştirme Operatörü

`??` ilk null olmayan değeri döndürür:

```mlp
numeric sonuc = belkiNull ?? varsayilanDeger
numeric zincirleme = a ?? b ?? c ?? 999
```

### Opsiyonel Zincirleme

`?.` üyelere güvenli erişim sağlar, nesne null ise null döndürür:

```mlp
string? isim = kisi?.isim    -- kisi null ise null
numeric? yas = kullanici?.profil?.yas  -- zincirleme
```

---

## Tip Çıkarımı

MELP bazı bağlamlarda tipleri çıkarabilir:

```mlp
-- Açık tip
numeric x = 10

-- Çıkarım ile çoklu atama
a, b, c = 10, "merhaba", true
-- a numeric, b string, c boolean
```

---

## Tip Kontrolü

### Karşılaştırma

Karşılaştırma için tipler uyumlu olmalı:

```mlp
if x == 5 then        -- ✅ numeric == numeric
if s == "merhaba" then  -- ✅ string == string
if x == "merhaba" then  -- ❌ Tip uyuşmazlığı hatası
```

### Aritmetik

Sayısal işlemler numeric tipler gerektirir:

```mlp
numeric toplam = a + b         -- ✅ Tamam
string birlestir = s1 + s2     -- ✅ String birleştirme
numeric yanlis = x + "merhaba"   -- ❌ Tip hatası
```

---

## Struct'lar

Kullanıcı tanımlı bileşik tipler:

```mlp
struct Kisi
    string isim
    numeric yas
    boolean aktif
end struct

-- Kullanım
Kisi k
k.isim = "Ali"
k.yas = 25
k.aktif = true

-- Erişim
print(k.isim)
```

### İç İçe Struct'lar

```mlp
struct Adres
    string sokak
    string sehir
end struct

struct Kisi
    string isim
    Adres adres
end struct

Kisi k
k.adres.sehir = "İstanbul"
```

---

## Hızlı Referans

### Tip Özeti

| Tip | Örnek | Değiştirilebilir | Nullable |
|------|---------|---------|----------|
| `numeric` | `42`, `3.14` | - | `?` ile |
| `string` | `"merhaba"` | - | `?` ile |
| `boolean` | `true`, `false` | - | `?` ile |
| `numeric[]` | `[1, 2, 3]` | ✅ Evet | - |
| `list()` | `(1; 2; 3;)` | ✅ Evet | - |
| `tuple<>` | `<1, 2, 3>` | ❌ Hayır | - |
| `struct` | Kullanıcı tanımlı | ✅ Evet | - |

### Koleksiyon Sözdizimi

| İşlem | Array | List | Tuple |
|-----------|-------|------|-------|
| Bildir | `[1, 2, 3]` | `(1; 2; 3;)` | `<1, 2, 3>` |
| Erişim | `arr[0]` | `list(0)` | `tuple<0>` |
| Değiştir | ✅ `arr[0] = x` | ✅ `list(0) = x` | ❌ Değiştirilemez |

---

*Bu belge MELP Dil Dokümantasyonunun bir parçasıdır.*

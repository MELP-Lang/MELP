# Eğitim 02: Değişkenler ve Tipler

Bu eğitimde, MELP'te değişkenleri nasıl bildireceğinizi ve kullanacağınızı, tip sistemini anlayacak ve temel işlemleri gerçekleştireceksiniz.

## İçindekiler
- [Neler Öğreneceksiniz](#neler-öğreneceksiniz)
- [Değişken Bildirimi](#değişken-bildirimi)
- [MELP Tip Sistemi](#melp-tip-sistemi)
- [Numeric (Sayısal) Tip](#numeric-sayısal-tip)
- [String (Metin) Tip](#string-metin-tip)
- [Boolean (Mantıksal) Tip](#boolean-mantıksal-tip)
- [Değişken Kapsamı](#değişken-kapsamı)
- [Tip Güvenliği](#tip-güvenliği)
- [En İyi Uygulamalar](#en-i̇yi-uygulamalar)
- [Alıştırmalar](#alıştırmalar)
- [Sonraki Adımlar](#sonraki-adımlar)

---

## Neler Öğreneceksiniz

Bu eğitimin sonunda:
- Farklı tiplerle değişken bildirebileceksiniz
- MELP'in tip sistemini anlayacaksınız (numeric, string, boolean)
- Aritmetik ve string işlemleri yapabileceksiniz
- Değişken kapsamını ve ömrünü anlayacaksınız
- Tip güvenli kod yazabileceksiniz

---

## Değişken Bildirimi

MELP'te değişkenler açık bir tip, ardından bir isim ve başlangıç değeri ile bildirilir.

### Temel Sözdizimi

```mlp
tip değişkenİsmi = değer
```

### Örnek: Tam Program

```mlp
function main() returns numeric
    numeric yas = 25
    string isim = "Ayşe"
    boolean aktif = true
    
    print("Değişken bildirimleri tamamlandı!")
    return 0
end
```

**Derle ve çalıştır:**
```bash
./compiler/stage0/melp degiskenler.mlp degiskenler.ll
clang degiskenler.ll -o degiskenler
./degiskenler
```

**Çıktı:**
```
Değişken bildirimleri tamamlandı!
```

---

## MELP Tip Sistemi

MELP'in üç temel tipi vardır:

| Tip | Açıklama | Örnek Değerler | Boyut |
|-----|----------|----------------|-------|
| **numeric** | Tamsayılar | `42`, `-10`, `0` | 64-bit işaretli tamsayı |
| **string** | Metin verisi | `"Merhaba"`, `"MELP"` | UTF-8 string pointer'ı |
| **boolean** | Mantıksal değerler | `true`, `false` | 1-bit (LLVM'de i1 olarak saklanır) |

> **Not:** MELP **statik tiplidir** - değişken tipleri derleme zamanında kontrol edilir!

---

## Numeric (Sayısal) Tip

`numeric` tipi 64-bit işaretli tamsayıları temsil eder.

### Bildirimi ve Atama

```mlp
function main() returns numeric
    numeric x = 10
    numeric y = 20
    numeric toplam = x + y
    
    print("Hesaplama tamamlandı!")
    return toplam
end
```

### Aritmetik İşlemler

MELP standart aritmetik operatörleri destekler:

```mlp
function main() returns numeric
    numeric a = 100
    numeric b = 25
    
    numeric toplama = a + b       // 125
    numeric cikarma = a - b       // 75
    numeric carpma = a * b        // 2500
    numeric bolme = a / b         // 4
    numeric modulo = a % b        // 0
    
    return 0
end
```

**Desteklenen Operatörler:**
- `+` Toplama
- `-` Çıkarma
- `*` Çarpma
- `/` Tamsayı bölme
- `%` Modulo (kalan)

### Negatif Sayılar

```mlp
function main() returns numeric
    numeric sicaklik = -15
    numeric borc = -1000
    numeric sonuc = sicaklik + 20  // 5
    
    return sonuc
end
```

### Aralık

- **Minimum:** `-9,223,372,036,854,775,808` (−2^63)
- **Maksimum:** `9,223,372,036,854,775,807` (2^63 − 1)

---

## String (Metin) Tip

`string` tipi UTF-8 kodlu metni temsil eder.

### Bildirimi ve Atama

```mlp
function main() returns numeric
    string selamlama = "Merhaba, Dünya!"
    string dil = "MELP"
    string emoji = "🚀"
    
    print(selamlama)
    print(dil)
    print(emoji)
    
    return 0
end
```

**Çıktı:**
```
Merhaba, Dünya!
MELP
🚀
```

### String Birleştirme

`+` operatörünü kullanarak string'leri birleştirebilirsiniz:

```mlp
function main() returns numeric
    string ad = "Ahmet"
    string soyad = "Yılmaz"
    string tamAd = ad + " " + soyad
    
    print(tamAd)  // "Ahmet Yılmaz"
    
    return 0
end
```

### String Karşılaştırma

Karşılaştırma operatörlerini kullanarak string'leri karşılaştırın:

```mlp
function main() returns numeric
    string sifre = "admin"
    
    if sifre == "admin" then
        print("Erişim izni verildi!")
    end if
    
    if sifre != "misafir" then
        print("Misafir kullanıcı değil")
    end if
    
    return 0
end
```

**Desteklenen Karşılaştırma Operatörleri:**
- `==` Eşittir
- `!=` Eşit değildir
- `<` Küçüktür (sözlüksel)
- `>` Büyüktür (sözlüksel)
- `<=` Küçük eşittir
- `>=` Büyük eşittir

### UTF-8 Desteği

MELP string'leri tam UTF-8 kodlamasını destekler:

```mlp
function main() returns numeric
    string turkce = "Merhaba Dünya"
    string cince = "你好世界"
    string rusca = "Привет мир"
    string arapca = "مرحبا بالعالم"
    
    print(turkce)
    print(cince)
    print(rusca)
    print(arapca)
    
    return 0
end
```

---

## Boolean (Mantıksal) Tip

`boolean` tipi mantıksal değerleri temsil eder: `true` (doğru) veya `false` (yanlış).

### Bildirimi ve Atama

```mlp
function main() returns numeric
    boolean gecerli = true
    boolean hataVar = false
    boolean hazir = true
    
    return 0
end
```

### Boolean İşlemleri

```mlp
function main() returns numeric
    boolean a = true
    boolean b = false
    
    // Mantıksal VE
    boolean ikisi = a && b  // false
    
    // Mantıksal VEYA
    boolean biri = a || b  // true
    
    // Mantıksal DEĞİL
    boolean aDegilse = !a  // false
    
    return 0
end
```

### Karşılaştırmalardan Boolean

```mlp
function main() returns numeric
    numeric yas = 25
    boolean yetiskin = yas >= 18  // true
    boolean cocuk = yas < 13      // false
    
    if yetiskin then
        print("Yetişkin kullanıcı")
    end if
    
    return 0
end
```

---

## Değişken Kapsamı

MELP'teki değişkenler **fonksiyon kapsamına** sahiptir - bildirimden fonksiyonun sonuna kadar görünürdürler.

### Yerel Değişkenler

```mlp
function hesapla() returns numeric
    numeric x = 10  // hesapla()'ya yerel
    numeric y = 20  // hesapla()'ya yerel
    return x + y
end

function main() returns numeric
    numeric sonuc = hesapla()
    // x ve y burada ERİŞİLEBİLİR DEĞİL!
    
    numeric z = 100  // main()'e yerel
    return sonuc + z
end
```

### Değişken Gölgeleme (İzin Verilmez)

MELP değişken gölgelemeye **izin vermez**:

```mlp
function main() returns numeric
    numeric x = 10
    
    if true then
        numeric x = 20  // ❌ HATA! 'x' değişkeni zaten bildirilmiş!
    end if
    
    return 0
end
```

### Kontrol Yapılarında Blok Kapsamı

`if`, `while` veya diğer blokların içinde bildirilen değişkenler hala fonksiyon kapsamındadır:

```mlp
function main() returns numeric
    if true then
        numeric gecici = 100
    end if
    
    // gecici hala burada erişilebilir (fonksiyon kapsamı!)
    numeric sonuc = gecici  // ✅ Bu MELP'te çalışır
    
    return sonuc
end
```

> **Not:** Bu davranış gelecekteki MELP sürümlerinde daha iyi kapsam için değişebilir.

---

## Tip Güvenliği

MELP **statik tiplidir** ve derleme zamanında tip güvenliğini zorlar.

### Tip Uyumsuzluğu Hataları

```mlp
function main() returns numeric
    numeric yas = "25"      // ❌ HATA! String'i numeric'e atayamazsınız
    string isim = 100       // ❌ HATA! Numeric'i string'e atayamazsınız
    boolean bayrak = 1      // ❌ HATA! Numeric'i boolean'a atayamazsınız
    
    return 0
end
```

### Doğru Tip Kullanımı

```mlp
function main() returns numeric
    numeric yas = 25           // ✅ Doğru
    string isim = "Ayşe"       // ✅ Doğru
    boolean aktif = true       // ✅ Doğru
    
    return 0
end
```

### İşlem Tip Uyumluluğu

```mlp
function main() returns numeric
    numeric x = 10
    string y = "20"
    
    numeric toplam = x + y  // ❌ HATA! Numeric ve string toplanamaz
    
    return 0
end
```

**Doğru versiyon:**
```mlp
function main() returns numeric
    numeric x = 10
    numeric y = 20
    
    numeric toplam = x + y  // ✅ Doğru! İkisi de numeric
    
    return toplam
end
```

---

## En İyi Uygulamalar

### 1. Anlamlı Değişken İsimleri Kullanın

```mlp
// ❌ Kötü
function main() returns numeric
    numeric x = 25
    numeric y = 30
    return x + y
end

// ✅ İyi
function main() returns numeric
    numeric kullaniciYasi = 25
    numeric emeklilikYasi = 30
    numeric emekliligeKalanYil = emeklilikYasi - kullaniciYasi
    return emekliligeKalanYil
end
```

### 2. Değişkenleri Hemen Başlatın

```mlp
// ✅ Her zaman bildirirken başlatın
function main() returns numeric
    numeric sayac = 0
    string mesaj = "Başlıyor..."
    boolean hazir = false
    
    return sayac
end
```

### 3. Değişken Kapsamını Küçük Tutun

```mlp
// ❌ Kaçının: Tüm değişkenleri en üstte bildirmek
function main() returns numeric
    numeric a = 0
    numeric b = 0
    numeric c = 0
    numeric d = 0
    
    // ... 50 satır kod ...
    
    a = 10  // 'a' ne için kullanılıyordu?
    return a
end

// ✅ Daha iyi: Kullanıma yakın bildirin
function main() returns numeric
    // ... kod ...
    
    numeric hesaplamaSonucu = 10
    return hesaplamaSonucu
end
```

### 4. Sihirli Sayılar İçin Sabitler Kullanın

```mlp
// ❌ Sihirli sayılardan kaçının
function main() returns numeric
    numeric toplam = fiyat * 100
    return toplam
end

// ✅ Daha iyi: İsimlendirilmiş sabitler
function main() returns numeric
    numeric DOLAR_BASINA_KURUS = 100
    numeric toplam = fiyat * DOLAR_BASINA_KURUS
    return toplam
end
```

---

## Alıştırmalar

Öğrendiklerinizi pratik yapmak için bu alıştırmaları deneyin!

### Alıştırma 1: Yaş Hesaplayıcı

Emekliliğe kaç yıl kaldığını hesaplayan bir program oluşturun:

```mlp
function main() returns numeric
    numeric suankiYas = 30
    numeric emeklilikYasi = 65
    numeric kalanYil = emeklilikYasi - suankiYas
    
    print("Emekliliğe kalan yıl hesaplandı!")
    return kalanYil
end
```

**Beklenen dönüş değeri:** `35`

### Alıştırma 2: Sıcaklık Dönüştürücü

Celsius'u Fahrenheit'a dönüştüren bir program oluşturun:

```mlp
// Formül: F = (C * 9 / 5) + 32
function main() returns numeric
    numeric celsius = 25
    numeric fahrenheit = (celsius * 9 / 5) + 32
    
    print("Sıcaklık dönüştürüldü!")
    return fahrenheit
end
```

**Beklenen dönüş değeri:** `77`

### Alıştırma 3: String Oluşturucu

Parçalardan bir selamlama mesajı oluşturun:

```mlp
function main() returns numeric
    string selamlama = "Merhaba"
    string isim = "Ayşe"
    string noktalama = "!"
    string mesaj = selamlama + ", " + isim + noktalama
    
    print(mesaj)
    return 0
end
```

**Beklenen çıktı:** `Merhaba, Ayşe!`

### Alıştırma 4: Doğrulama Kontrolü

Basit bir doğrulama programı oluşturun:

```mlp
function main() returns numeric
    string kullaniciAdi = "admin"
    string sifre = "gizli123"
    
    boolean gecerliKullanici = kullaniciAdi == "admin"
    boolean gecerliSifre = sifre == "gizli123"
    boolean girisYapabilir = gecerliKullanici && gecerliSifre
    
    if girisYapabilir then
        print("Giriş başarılı!")
    end if
    
    return 0
end
```

**Beklenen çıktı:** `Giriş başarılı!`

### Alıştırma 5: Dikdörtgen Alanı

Bir dikdörtgenin alanını hesaplayın:

```mlp
function main() returns numeric
    numeric uzunluk = 15
    numeric genislik = 10
    numeric alan = uzunluk * genislik
    numeric cevre = 2 * (uzunluk + genislik)
    
    print("Dikdörtgen hesaplamaları tamamlandı!")
    return alan
end
```

**Beklenen dönüş değeri:** `150`

---

## Ne Öğrendiniz

✅ Değişken bildirimi sözdizimi: `tip isim = değer`  
✅ Üç temel tip: numeric, string, boolean  
✅ Aritmetik işlemler: `+`, `-`, `*`, `/`, `%`  
✅ String birleştirme ve karşılaştırma  
✅ Boolean işlemleri: `&&`, `||`, `!`  
✅ Değişken kapsamı (fonksiyon seviyesi)  
✅ Tip güvenliği ve derleme zamanı tip kontrolü  
✅ Değişken isimlendirme ve düzenleme için en iyi uygulamalar  

---

## Öğrenmeye Devam Edin

**Sonraki Eğitim:** [03 - Fonksiyonlar](03_functions.md)

Şunları öğrenin:
- Kendi fonksiyonlarınızı tanımlama
- Parametreleri ve dönüş değerlerini kullanma
- Fonksiyonları diğer fonksiyonlardan çağırma
- Özyinelemeli (recursive) fonksiyonları anlama

---

## Ek Kaynaklar

- **Örnekler:** [examples/basics/](../../examples/basics/)
- **Dil Spesifikasyonu:** [docs/language/types.md](../../docs/language/types.md)
- **String Destek Kılavuzu:** [docs/STRING_SUPPORT.md](../../docs/STRING_SUPPORT.md)

---

**Kodlamaya devam edin! 🎯**

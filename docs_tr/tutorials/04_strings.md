# Eğitim 04: MELP'te String'ler

Bu eğitimde, MELP'te string'lerle çalışma hakkında her şeyi öğreneceksiniz - temel literal'lerden birleştirme ve karşılaştırma gibi gelişmiş işlemlere kadar.

## İçindekiler
- [Neler Öğreneceksiniz](#neler-öğreneceksiniz)
- [String Temelleri](#string-temelleri)
- [String Literal'leri](#string-literalleri)
- [String Değişkenleri](#string-değişkenleri)
- [String Birleştirme](#string-birleştirme)
- [String Karşılaştırma](#string-karşılaştırma)
- [Fonksiyonlarda String'ler](#fonksiyonlarda-stringler)
- [UTF-8 Desteği](#utf-8-desteği)
- [STO String Tipi](#sto-string-tipi)
- [Perde Arkası](#perde-arkası)
- [En İyi Uygulamalar](#en-i̇yi-uygulamalar)
- [Alıştırmalar](#alıştırmalar)
- [Sonraki Adımlar](#sonraki-adımlar)

---

## Neler Öğreneceksiniz

Bu eğitimin sonunda:
- MELP'in string tip sistemini anlayacaksınız
- String literal'lerini ve değişkenlerini kullanacaksınız
- `+` operatörü ile string'leri birleştireceksiniz
- `==`, `!=`, `<`, `>`, `<=`, `>=` ile string'leri karşılaştıracaksınız
- String'leri parametre olarak fonksiyonlara geçireceksiniz
- UTF-8 kodlu metin ile çalışacaksınız
- MELP'te string'lerin nasıl uygulandığını anlayacaksınız (STO sistemi)

---

## String Temelleri

MELP'te `string` tipi metin verisini temsil eder. String'ler:
- **Değişmezdir** - Oluşturulduktan sonra değiştirilemez
- **UTF-8 kodludur** - Tam Unicode desteği
- **Referans tipleridir** - Karakter verisine pointer olarak saklanır
- **Null-sonlandırılmıştır** - C-stil string'ler ile uyumludur

### Hızlı Örnek

```mlp
function main() returns numeric
    string mesaj = "Merhaba, MELP!"
    print(mesaj)
    return 0
end
```

**Çıktı:**
```
Merhaba, MELP!
```

---

## String Literal'leri

**String literal**, çift tırnak içine alınmış metindir.

### Temel String Literal'leri

```mlp
function main() returns numeric
    print("Merhaba")
    print("Dünya")
    print("MELP harika!")
    return 0
end
```

**Çıktı:**
```
Merhaba
Dünya
MELP harika!
```

### Boş String'ler

```mlp
function main() returns numeric
    string bos = ""
    print(bos)  // Hiçbir şey yazdırmaz (boş satır)
    return 0
end
```

### Çok Kelimeli String'ler

```mlp
function main() returns numeric
    string cumle = "Hızlı kahverengi tilki tembel köpeğin üzerinden atlar"
    print(cumle)
    return 0
end
```

### Özel Karakterli String'ler

```mlp
function main() returns numeric
    string bosluklarla = "Merhaba    Dünya"    // Çoklu boşluklar
    string noktalamali = "Merhaba, Dünya!"     // Noktalama işaretleri
    string sayilarla = "Yıl 2024"              // Sayılar
    
    print(bosluklarla)
    print(noktalamali)
    print(sayilarla)
    
    return 0
end
```

**Çıktı:**
```
Merhaba    Dünya
Merhaba, Dünya!
Yıl 2024
```

---

## String Değişkenleri

String'leri yeniden kullanım için değişkenlerde saklayabilirsiniz.

### Bildirimi ve Atama

```mlp
function main() returns numeric
    string isim = "Ayşe"
    string dil = "MELP"
    string selamlama = "Hoş geldiniz!"
    
    print(isim)
    print(dil)
    print(selamlama)
    
    return 0
end
```

**Çıktı:**
```
Ayşe
MELP
Hoş geldiniz!
```

### String Değişkenlerini Kullanma

```mlp
function main() returns numeric
    string kullaniciAdi = "Mehmet"
    
    print("Kullanıcı giriş yaptı:")
    print(kullaniciAdi)
    
    return 0
end
```

**Çıktı:**
```
Kullanıcı giriş yaptı:
Mehmet
```

### Yeniden Atama (İzin Verilmez)

Mevcut MELP'te değişkenler yeniden atanamaz:

```mlp
function main() returns numeric
    string mesaj = "Merhaba"
    mesaj = "Hoşçakal"  // ❌ HATA! Yeniden atama henüz desteklenmiyor
    return 0
end
```

> **Not:** Değişken yeniden ataması gelecek MELP sürümleri için planlanmıştır.

---

## String Birleştirme

**String birleştirme**, `+` operatörünü kullanarak string'leri birleştirmek demektir.

### Temel Birleştirme

```mlp
function main() returns numeric
    string selamlama = "Merhaba"
    string isim = "Dünya"
    string mesaj = selamlama + " " + isim
    
    print(mesaj)  // "Merhaba Dünya"
    
    return 0
end
```

**Çıktı:**
```
Merhaba Dünya
```

### Birden Fazla String Birleştirme

```mlp
function main() returns numeric
    string birinci = "MELP"
    string ikinci = "çok"
    string ucuncu = "harika"
    
    string cumle = birinci + " " + ikinci + " " + ucuncu
    
    print(cumle)  // "MELP çok harika"
    
    return 0
end
```

**Çıktı:**
```
MELP çok harika
```

### Literal ve Değişkenleri Birleştirme

```mlp
function main() returns numeric
    string kullaniciAdi = "Ayşe"
    string mesaj = "Hoş geldiniz, " + kullaniciAdi + "!"
    
    print(mesaj)  // "Hoş geldiniz, Ayşe!"
    
    return 0
end
```

**Çıktı:**
```
Hoş geldiniz, Ayşe!
```

### Zincirleme Birleştirme

```mlp
function main() returns numeric
    string a = "Bir"
    string b = "İki"
    string c = "Üç"
    string d = "Dört"
    
    string sonuc = a + " " + b + " " + c + " " + d
    
    print(sonuc)  // "Bir İki Üç Dört"
    
    return 0
end
```

---

## String Karşılaştırma

MELP, karşılaştırma operatörleri kullanarak string'leri karşılaştırmayı destekler.

### Eşitlik Karşılaştırması

```mlp
function main() returns numeric
    string sifre = "admin123"
    
    if sifre == "admin123" then
        print("Şifre doğru!")
    end if
    
    if sifre != "yanlis" then
        print("Şifre 'yanlis' değil")
    end if
    
    return 0
end
```

**Çıktı:**
```
Şifre doğru!
Şifre 'yanlis' değil
```

### Sözlüksel Karşılaştırma

String'ler alfabetik olarak karşılaştırılır (sözlük sırası):

```mlp
function main() returns numeric
    string kelime1 = "elma"
    string kelime2 = "muz"
    
    if kelime1 < kelime2 then
        print("elma, muz'dan önce gelir")
    end if
    
    if kelime2 > kelime1 then
        print("muz, elma'dan sonra gelir")
    end if
    
    return 0
end
```

**Çıktı:**
```
elma, muz'dan önce gelir
muz, elma'dan sonra gelir
```

### Tüm Karşılaştırma Operatörleri

```mlp
function main() returns numeric
    string a = "elma"
    string b = "muz"
    
    // Eşitlik
    boolean esit = a == a        // true
    boolean esitDegil = a != b   // true
    
    // Sıralama
    boolean kucuk = a < b        // true  (elma < muz)
    boolean buyuk = b > a        // true  (muz > elma)
    boolean kucukEsit = a <= a   // true  (elma <= elma)
    boolean buyukEsit = b >= a   // true  (muz >= elma)
    
    if kucuk then
        print("elma < muz: true")
    end if
    
    return 0
end
```

**Çıktı:**
```
elma < muz: true
```

### Büyük/Küçük Harf Duyarlılığı

String karşılaştırmaları **büyük/küçük harf duyarlıdır**:

```mlp
function main() returns numeric
    string kucuk = "merhaba"
    string buyuk = "MERHABA"
    
    if kucuk == buyuk then
        print("Aynı")
    end if
    
    if kucuk != buyuk then
        print("Farklı!")  // Bu yazdırılacak
    end if
    
    return 0
end
```

**Çıktı:**
```
Farklı!
```

---

## Fonksiyonlarda String'ler

String'ler parametre olarak geçirilebilir ve fonksiyonlardan döndürülebilir.

### String Parametreleri

```mlp
function selamla(string isim) returns numeric
    print("Merhaba, " + isim + "!")
    return 0
end

function main() returns numeric
    selamla("Ayşe")
    selamla("Mehmet")
    
    return 0
end
```

**Çıktı:**
```
Merhaba, Ayşe!
Merhaba, Mehmet!
```

### String Döndürme

```mlp
function selamlamaOlustur(string isim) returns string
    string selamlama = "Hoş geldiniz, " + isim
    return selamlama
end

function main() returns numeric
    string mesaj1 = selamlamaOlustur("Ayşe")
    string mesaj2 = selamlamaOlustur("Mehmet")
    
    print(mesaj1)
    print(mesaj2)
    
    return 0
end
```

**Çıktı:**
```
Hoş geldiniz, Ayşe
Hoş geldiniz, Mehmet
```

### Çoklu String Parametreleri

```mlp
function isimleriBlrlestir(string ad, string soyad) returns string
    string tamIsim = ad + " " + soyad
    return tamIsim
end

function main() returns numeric
    string isim = isimleriBlrlestir("Ahmet", "Yılmaz")
    print(isim)  // "Ahmet Yılmaz"
    
    return 0
end
```

**Çıktı:**
```
Ahmet Yılmaz
```

### String ve Numeric Parametreleri

```mlp
function mesajFormatla(string metin, numeric sayi) returns string
    string sonuc = metin + " (sayı: " + sayi + ")"
    return sonuc
end

function main() returns numeric
    string mesaj = mesajFormatla("Ürünler", 5)
    print(mesaj)  // "Ürünler (sayı: 5)"
    
    return 0
end
```

**Çıktı:**
```
Ürünler (sayı: 5)
```

---

## UTF-8 Desteği

MELP, uluslararası karakterlere izin veren UTF-8 kodlamasını tam olarak destekler.

### Uluslararası Metin

```mlp
function main() returns numeric
    string turkce = "Merhaba Dünya"
    string cince = "你好世界"
    string rusca = "Привет мир"
    string arapca = "مرحبا بالعالم"
    string japonca = "こんにちは世界"
    
    print(turkce)
    print(cince)
    print(rusca)
    print(arapca)
    print(japonca)
    
    return 0
end
```

**Çıktı:**
```
Merhaba Dünya
你好世界
Привет мир
مرحبا بالعالم
こんにちは世界
```

### Emoji Desteği

```mlp
function main() returns numeric
    string roket = "🚀"
    string kalp = "❤️"
    string yildiz = "⭐"
    
    string mesaj = "MELP " + roket + " harika " + kalp + yildiz
    
    print(mesaj)  // "MELP 🚀 harika ❤️⭐"
    
    return 0
end
```

**Çıktı:**
```
MELP 🚀 harika ❤️⭐
```

### Karışık Diller

```mlp
function main() returns numeric
    string karisik = "Hello नमस्ते Bonjour 你好 Merhaba"
    print(karisik)
    return 0
end
```

**Çıktı:**
```
Hello नमस्ते Bonjour 你好 Merhaba
```

---

## STO String Tipi

MELP, string yönetimi için **STO (String Type Object)** sistemini kullanır.

### STO Nedir?

STO, MELP'in şunları sağlayan çalışma zamanı tip sistemidir:
- **Otomatik bellek yönetimi** - Manuel tahsis/serbest bırakma yok
- **String birleştirme** - Çalışma zamanı `mlp_string_concat()` fonksiyonu
- **String karşılaştırma** - Çalışma zamanı `mlp_string_compare()` fonksiyonu
- **Tip güvenliği** - Derleme zamanı tip kontrolü

### STO Çalışma Zamanı Fonksiyonları

String işlemlerini kullandığınızda, MELP bu C fonksiyonlarını çağırır:

```c
// Çalışma zamanı fonksiyonları (runtime/sto/ içinde uygulanmıştır)
char* mlp_string_concat(const char* s1, const char* s2);
int mlp_string_compare(const char* s1, const char* s2);
int mlp_string_equals(const char* s1, const char* s2);
```

### Örnek: Perde Arkasında Neler Olur

**MELP Kodu:**
```mlp
function main() returns numeric
    string selamlama = "Merhaba"
    string isim = "Dünya"
    string mesaj = selamlama + " " + isim
    
    if mesaj == "Merhaba Dünya" then
        print("Başarılı!")
    end if
    
    return 0
end
```

**Üretilen x86-64 Assembly (basitleştirilmiş):**
```asm
; String birleştirme
leaq .LC0(%rip), %rdi        ; "Merhaba"
leaq .LC1(%rip), %rsi        ; " "
call mlp_string_concat@PLT   ; "Merhaba " döndürür

movq %rax, %rdi              ; "Merhaba "
leaq .LC2(%rip), %rsi        ; "Dünya"
call mlp_string_concat@PLT   ; "Merhaba Dünya" döndürür

; String karşılaştırma
movq %rax, %rdi              ; "Merhaba Dünya"
leaq .LC3(%rip), %rsi        ; "Merhaba Dünya"
call mlp_string_compare@PLT  ; 0 döndürür (eşit)
```

---

## Perde Arkası

### String Depolama

String'ler **salt okunur veri bölümünde (.rodata)** saklanır:

**LLVM IR:**
```llvm
@.str.0 = private unnamed_addr constant [8 x i8] c"Merhaba\00", align 1
@.str.1 = private unnamed_addr constant [6 x i8] c"Dünya\00", align 1
```

**x86-64 Assembly:**
```asm
.section .rodata
.LC0:
    .string "Merhaba"
.LC1:
    .string "Dünya"
```

### String Değişkenleri

String değişkenleri yığında (stack) **pointer** olarak saklanır:

**LLVM IR:**
```llvm
%mesaj_ptr = alloca i8*, align 8
%tmp1 = getelementptr inbounds [8 x i8], [8 x i8]* @.str.0, i64 0, i64 0
store i8* %tmp1, i8** %mesaj_ptr, align 8
```

**x86-64 Assembly:**
```asm
subq    $16, %rsp           ; Yığın alanı tahsis et
leaq    .LC0(%rip), %rax    ; String adresini yükle
movq    %rax, -8(%rbp)      ; Pointer'ı yığında sakla
```

---

## En İyi Uygulamalar

### 1. Mesaj Oluşturmak İçin String Birleştirme Kullanın

```mlp
// ✅ İyi - açık ve okunabilir
function kullaniciFormatla(string isim, numeric id) returns string
    string sonuc = "Kullanıcı: " + isim + " (ID: " + id + ")"
    return sonuc
end
```

### 2. Eşitlik İçin == ile String Karşılaştırın

```mlp
// ✅ İyi - açık niyet
function sifreKontrol(string sifre) returns boolean
    if sifre == "admin123" then
        return true
    end if
    return false
end
```

### 3. Açıklayıcı String Değişken İsimleri Kullanın

```mlp
// ❌ Kötü
function main() returns numeric
    string s = "Merhaba"
    string s2 = "Dünya"
    return 0
end

// ✅ İyi
function main() returns numeric
    string selamlama = "Merhaba"
    string hedef = "Dünya"
    return 0
end
```

### 4. Boş String Karşılaştırmalarından Kaçının

```mlp
// ✅ Açık boşluk kontrolü
function bosMu(string metin) returns boolean
    if metin == "" then
        return true
    end if
    return false
end
```

---

## Alıştırmalar

### Alıştırma 1: Tam İsim Oluşturucu

Parçalardan tam isim oluşturan bir fonksiyon yaratın:

```mlp
function tamIsimOlustur(string ad, string ortaIsim, string soyad) returns string
    string sonuc = ad + " " + ortaIsim + " " + soyad
    return sonuc
end

function main() returns numeric
    string isim = tamIsimOlustur("Ahmet", "Ali", "Yılmaz")
    print(isim)  // "Ahmet Ali Yılmaz"
    return 0
end
```

### Alıştırma 2: E-posta Doğrulayıcı

Basit bir e-posta doğrulayıcı oluşturun:

```mlp
function gecerliEpostaMi(string eposta) returns boolean
    // Basit kontrol: "@" içermeli
    if eposta == "" then
        return false
    end if
    
    // Gerçek uygulamada "@" karakterini kontrol edersiniz
    // Şimdilik sadece bir desene uyup uymadığını kontrol edeceğiz
    if eposta == "kullanici@ornek.com" then
        return true
    end if
    
    return false
end

function main() returns numeric
    boolean gecerli = gecerliEpostaMi("kullanici@ornek.com")
    
    if gecerli then
        print("Geçerli e-posta!")
    end if
    
    return 0
end
```

### Alıştırma 3: Selamlama Üretici

Günün saatine göre farklı selamlamalar oluşturun:

```mlp
function selamlamaAl(string gunZamani) returns string
    if gunZamani == "sabah" then
        return "Günaydın!"
    end if
    
    if gunZamani == "ogleden-sonra" then
        return "İyi öğleden sonralar!"
    end if
    
    if gunZamani == "aksam" then
        return "İyi akşamlar!"
    end if
    
    return "Merhaba!"
end

function main() returns numeric
    string mesaj1 = selamlamaAl("sabah")
    string mesaj2 = selamlamaAl("aksam")
    
    print(mesaj1)  // "Günaydın!"
    print(mesaj2)  // "İyi akşamlar!"
    
    return 0
end
```

### Alıştırma 4: String Karşılaştırma

Üç string'i alfabetik olarak karşılaştırın:

```mlp
function ilkiBul(string a, string b, string c) returns string
    if a < b && a < c then
        return a
    end if
    
    if b < a && b < c then
        return b
    end if
    
    return c
end

function main() returns numeric
    string ilk = ilkiBul("zebra", "elma", "mango")
    print(ilk)  // "elma"
    return 0
end
```

### Alıştırma 5: URL Oluşturucu

Parçalardan bir URL oluşturun:

```mlp
function urlOlustur(string protokol, string domain, string yol) returns string
    string sonuc = protokol + "://" + domain + "/" + yol
    return sonuc
end

function main() returns numeric
    string url = urlOlustur("https", "ornek.com", "api/kullanicilar")
    print(url)  // "https://ornek.com/api/kullanicilar"
    return 0
end
```

---

## Ne Öğrendiniz

✅ Çift tırnakla string literal'leri `"metin"`  
✅ String değişkenleri: `string isim = "değer"`  
✅ `+` operatörü ile string birleştirme  
✅ String karşılaştırma: `==`, `!=`, `<`, `>`, `<=`, `>=`  
✅ String'leri parametre olarak fonksiyonlara geçirme  
✅ Fonksiyonlardan string döndürme  
✅ Uluslararası karakterler ve emoji için UTF-8 desteği  
✅ String işlemleri için STO çalışma zamanı sistemi  
✅ String'lerin LLVM IR ve assembly'ye nasıl derlendiği  

---

## Öğrenmeye Devam Edin

**Önerilen Sonraki Adımlar:**

1. **Gelişmiş Örnekleri Keşfedin:**
   - `examples/basics/test_string_*.mlp` dosyalarını inceleyin
   - Üretilen LLVM IR ve assembly'yi çalışın

2. **Teknik Dokümantasyonu Okuyun:**
   - [docs/STRING_SUPPORT.md](../../docs/STRING_SUPPORT.md) - String uygulamasına derin dalış
   - [docs/LLVM_IR_GUIDE.md](../../docs/LLVM_IR_GUIDE.md) - LLVM IR üretimini anlama

3. **Denemeler Yapın:**
   - Karmaşık string birleştirmeleri deneyin
   - UTF-8 uç durumlarını test edin
   - LLVM vs x86-64 backend'lerini karşılaştırın

---

## Ek Kaynaklar

- **String Örnekleri:** [examples/basics/](../../examples/basics/)
- **String Çalışma Zamanı:** [runtime/sto/](../../runtime/sto/)
- **String Uygulaması:** [docs/STRING_SUPPORT.md](../../docs/STRING_SUPPORT.md)
- **Mimari:** [ARCHITECTURE.md](../../ARCHITECTURE.md)

---

**MELP'te string'lerde ustalaştınız! 🎉**

String desteği, MELP'in en güçlü özelliklerinden biridir ve metin işleme, kullanıcı arayüzleri ve daha fazlası ile gerçek dünya uygulamaları oluşturmanıza olanak tanır.

**Mutlu kodlamalar! 🚀**

# Eğitim 03: Fonksiyonlar

Bu eğitimde, MELP'te fonksiyonları nasıl tanımlayıp kullanacağınızı, parametreler ve dönüş değerleri ile nasıl çalışacağınızı ve gelişmiş fonksiyon kavramlarını öğreneceksiniz.

## İçindekiler
- [Neler Öğreneceksiniz](#neler-öğreneceksiniz)
- [Neden Fonksiyonlar?](#neden-fonksiyonlar)
- [Fonksiyon Temelleri](#fonksiyon-temelleri)
- [Parametreler](#parametreler)
- [Dönüş Değerleri](#dönüş-değerleri)
- [Fonksiyon Çağırma](#fonksiyon-çağırma)
- [Çoklu Fonksiyonlar](#çoklu-fonksiyonlar)
- [Özyinelemeli Fonksiyonlar](#özyinelemeli-fonksiyonlar)
- [Fonksiyon En İyi Uygulamaları](#fonksiyon-en-i̇yi-uygulamaları)
- [Yaygın Desenler](#yaygın-desenler)
- [Alıştırmalar](#alıştırmalar)
- [Sonraki Adımlar](#sonraki-adımlar)

---

## Neler Öğreneceksiniz

Bu eğitimin sonunda:
- Fonksiyonların amacını ve faydalarını anlayacaksınız
- Parametreli ve dönüş değerli fonksiyonlar tanımlayacaksınız
- Fonksiyonları diğer fonksiyonlardan çağıracaksınız
- Özyinelemeli fonksiyonlar yazacaksınız
- Fonksiyon tasarımı için en iyi uygulamaları takip edeceksiniz

---

## Neden Fonksiyonlar?

Fonksiyonlar, şu özelliklere sahip yeniden kullanılabilir kod bloklarıdır:
- **Kodu organize eder** - mantıksal birimlere ayırır
- **Tekrarı azaltır** (DRY - Don't Repeat Yourself / Kendini Tekrarlama)
- **Test etmeyi ve hata ayıklamayı kolaylaştırır**
- **Soyutlama sağlar** - karmaşıklığı basit isimler arkasına gizler

### Fonksiyonsuz (Tekrarlayıcı)

```mlp
function main() returns numeric
    // Alan 1'i hesapla
    numeric uzunluk1 = 10
    numeric genislik1 = 5
    numeric alan1 = uzunluk1 * genislik1
    
    // Alan 2'yi hesapla
    numeric uzunluk2 = 20
    numeric genislik2 = 8
    numeric alan2 = uzunluk2 * genislik2
    
    // Alan 3'ü hesapla
    numeric uzunluk3 = 15
    numeric genislik3 = 12
    numeric alan3 = uzunluk3 * genislik3
    
    return 0
end
```

### Fonksiyonlarla (Temiz)

```mlp
function alanHesapla(numeric uzunluk, numeric genislik) returns numeric
    numeric alan = uzunluk * genislik
    return alan
end

function main() returns numeric
    numeric alan1 = alanHesapla(10, 5)
    numeric alan2 = alanHesapla(20, 8)
    numeric alan3 = alanHesapla(15, 12)
    
    return 0
end
```

**Faydaları:** Daha az kod, bakımı daha kolay, yeniden kullanılabilir!

---

## Fonksiyon Temelleri

### Bir Fonksiyonun Anatomisi

```mlp
function fonksiyonIsmi(parametreler) returns donusTipi
    // Fonksiyon gövdesi
    // Yerel değişkenler
    // Hesaplamalar
    return deger
end
```

**Bileşenler:**
- **`function`** - Fonksiyon bildirmek için anahtar kelime
- **`fonksiyonIsmi`** - Açıklayıcı isim (camelCase kullanın)
- **`(parametreler)`** - Girdi değerleri (boş olabilir)
- **`returns donusTipi`** - Çıktı tipi (numeric, string, boolean)
- **Fonksiyon gövdesi** - Çağrıldığında çalışan kod
- **`return deger`** - Çağırana geri gönderilen değer
- **`end`** - Fonksiyonun sonunu işaretler

### Basit Fonksiyon Örneği

```mlp
function selamla() returns numeric
    print("Bir fonksiyondan merhaba!")
    return 0
end

function main() returns numeric
    selamla()
    return 0
end
```

**Derle ve çalıştır:**
```bash
./compiler/stage0/melp selamla.mlp selamla.ll
clang selamla.ll -o selamla
./selamla
```

**Çıktı:**
```
Bir fonksiyondan merhaba!
```

---

## Parametreler

Parametreler fonksiyonların girdi değerleri kabul etmesini sağlar.

### Tek Parametre

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

### Çoklu Parametreler

```mlp
function topla(numeric a, numeric b) returns numeric
    numeric toplam = a + b
    return toplam
end

function main() returns numeric
    numeric sonuc1 = topla(10, 20)     // 30
    numeric sonuc2 = topla(100, 50)    // 150
    
    print("Hesaplamalar tamamlandı!")
    return sonuc1 + sonuc2  // 180
end
```

### Parametre Tipleri

Parametrelerin açık tipleri olmalıdır:

```mlp
function goster(string mesaj, numeric sayi, boolean onemli) returns numeric
    if onemli then
        print("ÖNEMLİ: " + mesaj)
    end if
    
    return sayi
end

function main() returns numeric
    goster("Sistem başlıyor", 1, true)
    goster("Arkaplan görevi", 5, false)
    return 0
end
```

**Çıktı:**
```
ÖNEMLİ: Sistem başlıyor
```

---

## Dönüş Değerleri

Fonksiyonlar çağıranlara değer döndürebilir.

### Numeric Değer Döndürme

```mlp
function karesi(numeric x) returns numeric
    numeric sonuc = x * x
    return sonuc
end

function main() returns numeric
    numeric sayi = 5
    numeric kareAlindi = karesi(sayi)  // 25
    
    print("Hesaplama yapıldı!")
    return kareAlindi
end
```

### String Değer Döndürme

```mlp
function selamlamaOlustur(string isim) returns string
    string selamlama = "Hoş geldiniz, " + isim + "!"
    return selamlama
end

function main() returns numeric
    string mesaj = selamlamaOlustur("Ayşe")
    print(mesaj)  // "Hoş geldiniz, Ayşe!"
    
    return 0
end
```

### Boolean Değer Döndürme

```mlp
function ciftMi(numeric n) returns boolean
    numeric kalan = n % 2
    boolean sonuc = kalan == 0
    return sonuc
end

function main() returns numeric
    boolean kontrol1 = ciftMi(4)   // true
    boolean kontrol2 = ciftMi(7)   // false
    
    if kontrol1 then
        print("4 çifttir")
    end if
    
    return 0
end
```

**Çıktı:**
```
4 çifttir
```

### Erken Dönüş

Fonksiyonun sonuna ulaşmadan önce fonksiyondan dönebilirsiniz:

```mlp
function sifreKontrol(string sifre) returns boolean
    if sifre == "" then
        return false  // Erken dönüş - boş şifre
    end if
    
    if sifre == "admin" then
        return true   // Erken dönüş - doğru şifre
    end if
    
    return false  // Varsayılan dönüş - yanlış şifre
end

function main() returns numeric
    boolean gecerli = sifreKontrol("admin")
    
    if gecerli then
        print("Erişim izni verildi")
    end if
    
    return 0
end
```

---

## Fonksiyon Çağırma

### Temel Fonksiyon Çağrısı

```mlp
function merhabaDe() returns numeric
    print("Merhaba!")
    return 0
end

function main() returns numeric
    merhabaDe()  // Fonksiyonu çağır
    return 0
end
```

### Dönüş Değerlerini Kullanma

```mlp
function carp(numeric a, numeric b) returns numeric
    return a * b
end

function main() returns numeric
    // Dönüş değerini sakla
    numeric sonuc = carp(5, 6)
    
    // Dönüş değerini doğrudan kullan
    numeric toplam = carp(3, 4) + carp(2, 8)
    
    return sonuc  // 30
end
```

### Değişkenleri Argüman Olarak Geçirme

```mlp
function toplamHesapla(numeric fiyat, numeric miktar) returns numeric
    numeric toplam = fiyat * miktar
    return toplam
end

function main() returns numeric
    numeric urunFiyati = 50
    numeric urunSayisi = 3
    
    numeric tutar = toplamHesapla(urunFiyati, urunSayisi)  // 150
    
    return tutar
end
```

### Literal'leri Argüman Olarak Geçirme

```mlp
function topla(numeric x, numeric y) returns numeric
    return x + y
end

function main() returns numeric
    // Literal değerleri doğrudan geçir
    numeric toplam = topla(100, 200)  // 300
    
    return toplam
end
```

---

## Çoklu Fonksiyonlar

Aynı programda birden fazla fonksiyon tanımlayabilirsiniz.

### Örnek: Hesap Makinesi Programı

```mlp
function topla(numeric a, numeric b) returns numeric
    return a + b
end

function cikar(numeric a, numeric b) returns numeric
    return a - b
end

function carp(numeric a, numeric b) returns numeric
    return a * b
end

function bol(numeric a, numeric b) returns numeric
    return a / b
end

function main() returns numeric
    numeric x = 20
    numeric y = 5
    
    numeric toplam = topla(x, y)      // 25
    numeric fark = cikar(x, y)        // 15
    numeric carpim = carp(x, y)       // 100
    numeric bolum = bol(x, y)         // 4
    
    print("Tüm hesaplamalar tamamlandı!")
    return toplam + fark + carpim + bolum  // 144
end
```

### Fonksiyonların Diğer Fonksiyonları Çağırması

```mlp
function karesi(numeric x) returns numeric
    return x * x
end

function karelerToplami(numeric a, numeric b) returns numeric
    numeric kareA = karesi(a)
    numeric kareB = karesi(b)
    return kareA + kareB
end

function main() returns numeric
    numeric sonuc = karelerToplami(3, 4)  // 3² + 4² = 9 + 16 = 25
    return sonuc
end
```

---

## Özyinelemeli Fonksiyonlar

**Özyinelemeli fonksiyon**, kendisini çağıran bir fonksiyondur.

### Örnek: Faktöriyel

```mlp
function faktoriyel(numeric n) returns numeric
    if n <= 1 then
        return 1
    end if
    
    numeric kucuk = faktoriyel(n - 1)
    return n * kucuk
end

function main() returns numeric
    numeric sonuc = faktoriyel(5)  // 5! = 120
    return sonuc
end
```

**Nasıl çalışır:**
```
faktoriyel(5)
= 5 * faktoriyel(4)
= 5 * (4 * faktoriyel(3))
= 5 * (4 * (3 * faktoriyel(2)))
= 5 * (4 * (3 * (2 * faktoriyel(1))))
= 5 * (4 * (3 * (2 * 1)))
= 5 * (4 * (3 * 2))
= 5 * (4 * 6)
= 5 * 24
= 120
```

### Örnek: Fibonacci

```mlp
function fibonacci(numeric n) returns numeric
    if n <= 1 then
        return n
    end if
    
    numeric onceki1 = fibonacci(n - 1)
    numeric onceki2 = fibonacci(n - 2)
    return onceki1 + onceki2
end

function main() returns numeric
    numeric fib5 = fibonacci(5)   // 5
    numeric fib10 = fibonacci(10) // 55
    
    return fib10
end
```

**Fibonacci dizisi:** 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55...

### Özyineleme Gereksinimleri

Özyinelemenin doğru çalışması için:
1. **Temel durum** - Özyinelemeyi durduracak koşul
2. **Özyinelemeli durum** - Daha basit girdiyle kendini çağırma
3. **İlerleme** - Her çağrı temel duruma doğru ilerlemeli

---

## Fonksiyon En İyi Uygulamaları

### 1. Açıklayıcı İsimler Kullanın

```mlp
// ❌ Kötü - belirsiz isimler
function hesap(numeric x, numeric y) returns numeric
    return x * y
end

// ✅ İyi - açık amaç
function dikdortgenAlanHesapla(numeric uzunluk, numeric genislik) returns numeric
    return uzunluk * genislik
end
```

### 2. Fonksiyonları Küçük Tutun

```mlp
// ❌ Çok uzun - çok fazla iş yapıyor
function siparisiIsle() returns numeric
    // 100 satır kod...
    return 0
end

// ✅ Daha iyi - küçük, odaklı fonksiyonlar
function siparisDogrula() returns boolean
    // ...
    return true
end

function toplamHesapla() returns numeric
    // ...
    return 0
end

function onayGonder() returns numeric
    // ...
    return 0
end
```

**Kural:** Bir fonksiyon ekrana sığmıyorsa, bölün!

### 3. Tek Sorumluluk

Her fonksiyon **bir şey** yapmalı ve bunu iyi yapmalıdır.

```mlp
// ❌ Kötü - çoklu sorumluluklar
function kullaniciVerisiIsle(string isim, numeric yas) returns numeric
    // Girdiyi doğrula
    // Veritabanına kaydet
    // E-posta gönder
    // Arayüzü güncelle
    return 0
end

// ✅ İyi - fonksiyon başına tek sorumluluk
function kullaniciVerisiDogrula(string isim, numeric yas) returns boolean
    // Sadece doğrulama
    return true
end

function kullaniciVeritabaninaKaydet(string isim, numeric yas) returns numeric
    // Sadece veritabanı işlemleri
    return 0
end

function hosgeldinEpostasiGonder(string isim) returns numeric
    // Sadece e-posta gönderme
    return 0
end
```

### 4. Yan Etkilerden Kaçının

Saf fonksiyonlar dış durumu değiştirmez:

```mlp
// ✅ Saf fonksiyon - yan etki yok
function topla(numeric a, numeric b) returns numeric
    return a + b
end

// ⚠️ Yan etkiler - konsola yazdırıyor
function toplaVeYazdir(numeric a, numeric b) returns numeric
    numeric toplam = a + b
    print("Toplam: " + toplam)  // Yan etki!
    return toplam
end
```

**Not:** Yan etkiler her zaman kötü değildir, ama kasıtlı olun.

### 5. Parametre Sırası

En önemli parametreleri önce koyun:

```mlp
// ✅ İyi - önemli parametreler önce
function epostaGonder(string alici, string konu, string icerik) returns numeric
    // ...
    return 0
end

function main() returns numeric
    epostaGonder("kullanici@ornek.com", "Hoş Geldiniz", "Kaydolduğunuz için teşekkürler!")
    return 0
end
```

---

## Yaygın Desenler

### Desen 1: Doğrulayıcı Fonksiyonlar

```mlp
function gecerliYasMi(numeric yas) returns boolean
    if yas < 0 then
        return false
    end if
    
    if yas > 150 then
        return false
    end if
    
    return true
end

function main() returns numeric
    boolean gecerli = gecerliYasMi(25)  // true
    
    if gecerli then
        print("Yaş geçerli")
    end if
    
    return 0
end
```

### Desen 2: Yardımcı Fonksiyonlar

```mlp
function artikYilMi(numeric yil) returns boolean
    numeric mod4 = yil % 4
    numeric mod100 = yil % 100
    numeric mod400 = yil % 400
    
    if mod400 == 0 then
        return true
    end if
    
    if mod100 == 0 then
        return false
    end if
    
    if mod4 == 0 then
        return true
    end if
    
    return false
end

function subatGunSayisi(numeric yil) returns numeric
    boolean artik = artikYilMi(yil)
    
    if artik then
        return 29
    end if
    
    return 28
end

function main() returns numeric
    numeric gunler = subatGunSayisi(2024)  // 29
    return gunler
end
```

### Desen 3: Oluşturucu Fonksiyonlar

```mlp
function tamIsimOlustur(string ad, string soyad) returns string
    return ad + " " + soyad
end

function adresOlustur(string sokak, string sehir, string postaKodu) returns string
    return sokak + ", " + sehir + " " + postaKodu
end

function main() returns numeric
    string isim = tamIsimOlustur("Ahmet", "Yılmaz")
    string adres = adresOlustur("Atatürk Cad. 123", "İstanbul", "34000")
    
    print(isim)
    print(adres)
    
    return 0
end
```

---

## Alıştırmalar

### Alıştırma 1: Maksimum Fonksiyonu

İki sayıdan büyük olanı döndüren bir fonksiyon yazın:

```mlp
function maks(numeric a, numeric b) returns numeric
    if a > b then
        return a
    end if
    return b
end

function main() returns numeric
    numeric sonuc = maks(15, 23)  // 23 döndürmeli
    return sonuc
end
```

### Alıştırma 2: Üs Fonksiyonu

x^n hesaplayan bir fonksiyon yazın (özyineleme kullanarak):

```mlp
function us(numeric taban, numeric ustel) returns numeric
    if ustel == 0 then
        return 1
    end if
    
    numeric kucuk = us(taban, ustel - 1)
    return taban * kucuk
end

function main() returns numeric
    numeric sonuc = us(2, 5)  // 2^5 = 32
    return sonuc
end
```

### Alıştırma 3: Sıcaklık Dönüştürücü

Celsius ve Fahrenheit arasında dönüştürme yapan fonksiyonlar oluşturun:

```mlp
function celsiusToFahrenheit(numeric celsius) returns numeric
    numeric fahrenheit = (celsius * 9 / 5) + 32
    return fahrenheit
end

function fahrenheitToCelsius(numeric fahrenheit) returns numeric
    numeric celsius = (fahrenheit - 32) * 5 / 9
    return celsius
end

function main() returns numeric
    numeric f = celsiusToFahrenheit(25)   // 77
    numeric c = fahrenheitToCelsius(100)  // 37
    
    return f + c  // 114
end
```

### Alıştırma 4: String Formatlayıcı

Bir kişinin bilgilerini formatlayan bir fonksiyon oluşturun:

```mlp
function kisiFormatla(string isim, numeric yas, string sehir) returns string
    string yasStr = "Yaş: " + yas
    string sonuc = isim + " (" + yasStr + ") - " + sehir
    return sonuc
end

function main() returns numeric
    string bilgi = kisiFormatla("Ayşe", 30, "İstanbul")
    print(bilgi)  // "Ayşe (Yaş: 30) - İstanbul"
    
    return 0
end
```

### Alıştırma 5: Aralık Toplamı

1'den n'e kadar sayıların toplamını hesaplayan özyinelemeli fonksiyon yazın:

```mlp
function aralikToplami(numeric n) returns numeric
    if n <= 0 then
        return 0
    end if
    
    numeric kucuk = aralikToplami(n - 1)
    return n + kucuk
end

function main() returns numeric
    numeric sonuc = aralikToplami(10)  // 1+2+3+...+10 = 55
    return sonuc
end
```

---

## Ne Öğrendiniz

✅ Fonksiyon bildirimi sözdizimi: `function isim(params) returns tip`  
✅ Açık tiplerle parametreleri tanımlama  
✅ Fonksiyonlardan değer döndürme  
✅ Fonksiyonları çağırma ve dönüş değerlerini kullanma  
✅ Bir programda birden fazla fonksiyon yazma  
✅ Özyinelemeli fonksiyonlar oluşturma (faktöriyel, fibonacci)  
✅ Fonksiyon en iyi uygulamaları (isimlendirme, boyut, sorumluluk)  
✅ Yaygın fonksiyon desenleri (doğrulayıcılar, yardımcılar, oluşturucular)  

---

## Öğrenmeye Devam Edin

**Sonraki Eğitim:** [04 - String'ler](04_strings.md)

Şunları öğrenin:
- String literal'leri ve değişkenleri ile çalışma
- String birleştirme yapma
- String'leri karşılaştırma
- STO string tip sistemini anlama

---

## Ek Kaynaklar

- **Örnekler:** [examples/basics/](../../examples/basics/)
- **Gelişmiş Örnekler:** [examples/advanced/](../../examples/advanced/)
- **Fonksiyon Spesifikasyonları:** [docs/language/functions.md](../../docs/language/functions.md)

---

**Fonksiyonlarda ustalaşın ve MELP becerilerinizi geliştirin! 🚀**

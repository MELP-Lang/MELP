# MELP Dil Referansı — Türkçe MLP

**Sürüm:** 1.0  
**Tarih:** 2026-03-30  
**Aşama kapsamı:** Stage 0 ✅ + Stage 1 ✅ + Stage 2 (planlanan)  
**Kanonik kaynak:** `ORTAK/dil/MLP_SYNTAX_TR.md` + `tr/keywords.json`  
**İngilizce referans:** [referans_en.md](referans_en.md)  
**Anahtar kelime tablosu:** [anahtar_kelimeler.md](anahtar_kelimeler.md)

> Bu belge topluluk için hazırlanmıştır. Diğer MM'ler de Türkçe MLP sözdizimini  
> buradan öğrenebilir. Derleyicinin iç formatı (pmlp) farklıdır; normalizer dönüşümü  
> otomatik yapılır.

---

## § 1 — Temel Kurallar (önce oku)

| Kural | Doğru ✅ | Yanlış ❌ |
|---|---|---|
| Parametre ayırıcı | `fonksiyon(a; b; c)` | `fonksiyon(a, b, c)` |
| Dizi literali | `[1; 2; 3]` | `[1, 2, 3]` |
| Satır sonu `;` yok | `x = 42` | `x = 42;` |
| Blok kapanışları (iki kelime) | `fonksiyon sonu`, `koşul sonu` | `fonksiyon_sonu`, `koşul_sonu` |
| `koşul` her zaman `ise` ile biter | `koşul x > 0 ise` | `koşul x > 0` |
| `değilse koşul` (else if) | `değilse koşul x > 0 ise` | `elif x > 0` |
| Döngüden çıkış | `çık` | `break` |
| Yorumlar | `-- yorum` | `// yorum` |
| Mantıksal operatörler | `ve`, `veya`, `değil` | `&&`, `\|\|`, `!` |
| Boolean değerler | `doğru`, `yanlış` | `True`, `False`, `1`, `0` |
| Ondalık ayırıcı | `3,14` | `3.14` |

---

## § 2 — Tipler

```mlp
#lang turkish
#syntax mlp

sayısal x = 42
sayısal pi = 3,14
metin isim = "Ali"
mantıksal bayrak = doğru
mantıksal bitti = yanlış
```

| Türkçe | İngilizce | Açıklama |
|---|---|---|
| `sayısal` | `numeric` | Tam sayı veya ondalık |
| `metin` | `string` | Metin değeri |
| `mantıksal` | `boolean` | `doğru` / `yanlış` |
| `hiçbir` | `none` | Dönüş değeri yok |
| `boş` | `null` | Boş değer |

---

## § 3 — Değişkenler ve Sabitler

```mlp
sayısal x = 42
metin s = "merhaba"
mantıksal b = doğru

-- Yeniden atama (tip anahtar kelimesi gerekmez)
x = 100
s = "dünya"

-- Sabit
sabit sayısal MAX = 1000
sabit metin UYGULAMA = "MELP"
```

---

## § 4 — Operatörler

### Aritmetik

```mlp
x + y
x - y
x * y
x / y
x mod y     -- mod işlemi (% değil)
```

### Karşılaştırma

```mlp
x == y
x != y
x < y
x > y
x <= y
x >= y
```

### Mantıksal

```mlp
x ve y
x veya y
değil x

-- postfix "değil" (Türkçe'ye özgü)
x > 5 değil        -- not (x > 5)
```

### Metin birleştirme

```mlp
metin tam = "Merhaba, " + isim + "!"
```

---

## § 5 — Kontrol Akışı

### koşul / değilse koşul / yoksa

```mlp
koşul x > 0 ise
    yazdır("pozitif")
değilse koşul x < 0 ise
    yazdır("negatif")
yoksa
    yazdır("sıfır")
koşul sonu
```

---

## § 6 — Döngüler

### Koşullu döngü (while-stili)

```mlp
sayısal i = 0
döngü i < 10
    yazdır(i)
    i = i + 1
döngü sonu
```

### Sayaçlı döngü (artan)

```mlp
döngü i = 0 kadar 10
    yazdır(i)   -- 0..9
döngü sonu
```

### Sayaçlı döngü (azalan)

```mlp
döngü i = 10 geriye 1
    yazdır(i)   -- 10..1
döngü sonu
```

### Adımlı döngü

```mlp
döngü i = 0 kadar 20 adım 2
    yazdır(i)   -- 0, 2, 4, ...18
döngü sonu
```

### Her biri için (Stage 1)

```mlp
sayısal[] sayılar = [1; 2; 3; 4; 5]
her sayı içinde sayılar
    yazdır(sayı)
her sonu
```

### Çık ve Devam

```mlp
çık        -- döngüden çık
devam      -- sonraki iterasyon
```

---

## § 7 — Fonksiyonlar

### Temel tanım

```mlp
sayısal fonksiyon topla(sayısal a; sayısal b)
    döndür a + b
fonksiyon sonu
```

### Dönüş değeri yok

```mlp
hiçbir fonksiyon selamla(metin isim)
    yazdır("Merhaba, " + isim)
fonksiyon sonu
```

### Giriş noktası

```mlp
hiçbir fonksiyon giriş()
    yazdır("Merhaba Dünya!")
fonksiyon sonu
```

### Çok parametreli

```mlp
metin fonksiyon biçimle(metin şablon; sayısal değer; mantıksal bayrak)
    döndür şablon + str(değer)
fonksiyon sonu
```

---

## § 8 — Yapılar (Struct)

```mlp
yapı Nokta
    sayısal x
    sayısal y
yapı sonu

-- Örnek oluşturma
Nokta n
n.x = 10
n.y = 20

-- Metot (yapı dışında tanımlanır)
sayısal fonksiyon Nokta.uzaklık()
    döndür bu.x + bu.y
fonksiyon sonu

yazdır(n.uzaklık())   -- 30
```

---

## § 9 — Diziler (Array)

```mlp
-- Tanımlama
sayısal[] sayılar = [1; 2; 3; 4; 5]

-- Erişim (0-tabanlı)
yazdır(sayılar[0])     -- 1
sayılar[0] = 100

-- Uzunluk
yazdır(sayılar.uzunluk)  -- 5
```

---

## § 10 — Sıralamalar / Enum (Stage 1)

```mlp
sıralama Renk
    KIRMIZI
    YEŞİL
    MAVİ
sıralama sonu

Renk r = Renk.KIRMIZI
```

---

## § 11 — Arayüzler (Stage 1)

```mlp
arayüz Şekil
    sayısal fonksiyon alan()
arayüz sonu

yapı Daire uygular Şekil
    sayısal yarıçap
yapı sonu

sayısal fonksiyon Daire.alan()
    döndür 3 * bu.yarıçap * bu.yarıçap
fonksiyon sonu
```

---

## § 12 — Lambda (Stage 1)

```mlp
-- Tam lambda
sayısal fonksiyon iki_kat = lambda(sayısal x)
    döndür x * 2
lambda sonu

yazdır(iki_kat(5))   -- 10

-- Kısa lambda (tek parametre)
sayısal üç_kat = x -> x * 3
yazdır(üç_kat(4))    -- 12
```

---

## § 13 — Hata Yönetimi (Stage 1)

```mlp
dene
    sayısal sonuç = riskli_işlem()
    yazdır(sonuç)
hata yakala
    yazdır("Hata: " + hata.mesaj)
sonunda
    temizle()
dene sonu
```

---

## § 14 — Seçim / Match (Stage 1)

```mlp
metin fonksiyon sınıflandır(sayısal x)
    döndür seç x
        durum 1 o zaman "bir"
        durum 2 o zaman "iki"
        durum 3 o zaman "üç"
        yoksa "çok"
    seç sonu
fonksiyon sonu
```

---

## § 15 — Standart Kütüphane (Stage 0+)

### Giriş / Çıktı

```mlp
yazdır("Merhaba")                 -- stdout + yeni satır
metin satır = satır_oku()          -- stdin'den satır oku
metin içerik = dosya_oku("veri.txt")
sayısal tamam = dosya_yaz("/tmp/çıktı.txt"; "içerik\n")
```

### Tip Dönüşümü

```mlp
metin s = str(42)          -- "42"
metin b = str(doğru)       -- "true"
sayısal n = sayıya("42")   -- 42
```

### Matematik

```mlp
mutlak(-7)       -- 7
en_küçük(10; 3)   -- 3
en_büyük(10; 3)   -- 10
karekök(25)      -- 5
```

### Metin İşlemleri

```mlp
uzunluk("merhaba")                        -- 7
büyük("merhaba")                          -- "MERHABA"
küçük("MERHABA")                          -- "merhaba"
kırp("  merhaba  ")                       -- "merhaba"
içerir("merhaba dünya"; "dünya")          -- doğru
böl("a,b,c"; ",")                         -- ["a"; "b"; "c"]
değiştir("aaa"; "a"; "b")                -- "bbb"
alt_metin("merhaba"; 1; 3)                -- "erh"
```

---

## § 16 — Modüller (Stage 2 — planlandı)

```mlp
-- hesap.tr.mlp
modül hesap
    sayısal dışa_aktar fonksiyon kare(sayısal x)
        döndür x * x
    fonksiyon sonu
modül sonu

-- ana.tr.mlp
içe_aktar hesap

hiçbir fonksiyon giriş()
    sayısal sonuç = hesap.kare(10)
    yazdır(sonuç)   -- 100
fonksiyon sonu
```

---

## Ek A — Blok Kapanış Tablosu

| Blok açılışı | Kapanışı |
|---|---|
| `fonksiyon` | `fonksiyon sonu` |
| `koşul` | `koşul sonu` |
| `döngü` | `döngü sonu` |
| `her ... içinde` | `her sonu` |
| `yapı` | `yapı sonu` |
| `sıralama` | `sıralama sonu` |
| `arayüz` | `arayüz sonu` |
| `uygula` | `uygula sonu` |
| `seç` | `seç sonu` |
| `dene` | `dene sonu` |
| `lambda` | `lambda sonu` |
| `modül` | `modül sonu` |

---

## Ek B — Aşama Özet Tablosu

| Özellik | Stage 0 | Stage 1 | Stage 2 |
|---|---|---|---|
| Değişkenler, tipler | ✅ | ✅ | ✅ |
| koşul / değilse koşul | ✅ | ✅ | ✅ |
| döngü (sayaçlı/koşullu/foreach) | ✅ | ✅ | ✅ |
| Fonksiyonlar | ✅ | ✅ | ✅ |
| Yapı + metot | ✅ | ✅ | ✅ |
| Diziler, hashmap | ✅ | ✅ | ✅ |
| Stdlib (io/math/string) | ✅ | ✅ | ✅ |
| sıralama (enum) | ❌ | ✅ | ✅ |
| Arayüz | ❌ | ✅ | ✅ |
| Generics | ❌ | ✅ | ✅ |
| Lambda / closure | ❌ | ✅ | ✅ |
| dene / hata yakala | ❌ | ✅ | ✅ |
| seç / durum (match) | ❌ | ✅ | ✅ |
| modül / içe_aktar | ❌ | ❌ | ✅ |
| eşzamansız / bekle | ❌ | ❌ | ✅ |


# MELP Türkçe Sözdizimi Referansı

Kanonik anahtar kelime tablosu: [../../belgeler_v2/dil/anahtar_kelimeler.md](../../belgeler_v2/dil/anahtar_kelimeler.md)

> **MELP Türkçe sözdizimi (`.tr.mlp`)** — blok kapanışları boşlukla ayrılır,
> alt çizgi `_` kullanılmaz. Normalizer bu dosyaları kanonik `.mlp`'ye çevirir.

---

## Temel Kurallar

1. Blok kapanışları boşluklu: `koşul sonu`, `fonksiyon sonu`, `döngü sonu`
2. `;` yalnızca ayraç (parametre listesi, dizi elemanları) — satır sonunda noktalı virgül yok
3. `ise` keyword'ü `koşul` sonrasında zorunlu
4. Dönüş tipi fonksiyon adından **önce** yazılır: `sayısal fonksiyon topla(...)`
5. `döndür` isteğe bağlı — son ifade otomatik döner; erken çıkış için kullanılır
6. Yorumlar: `--` tek satır, `---` blok yorum, `---/ /---` belge yorumu

---

## Blok Anahtar Kelimeleri

| İngilizce          | Türkçe               | Notlar                              |
|--------------------|----------------------|-------------------------------------|
| `function`         | `fonksiyon`          | Başlangıç                           |
| `end function`     | `fonksiyon sonu`     |                                     |
| `if`               | `koşul`              | Kanonik Türkçe koşul anahtar kelimesi |
| `then`             | `ise`                | `koşul X ise` zorunlu               |
| `else if`          | `değilse koşul X ise`| Kanonik çok dallı koşul formu       |
| `else`             | `yoksa`              | Kanonik else formu                  |
| `end if`           | `koşul sonu`         | Kanonik kapanış                     |
| `loop`             | `döngü`              | Tüm döngü formları için             |
| `end loop`         | `döngü sonu`         |                                     |
| `end for` (foreach)| `için sonu`          | Yalnızca `döngü her...` kapatır     |
| `match`            | `seç`                | Kanonik eşleştirme anahtar kelimesi |
| `case X`           | `durum X ise`        |                                     |
| `end match`        | `seç sonu`           |                                     |
| `struct`           | `yapı`               |                                     |
| `end struct`       | `yapı sonu`          |                                     |
| `enum`             | `sıralama`           |                                     |
| `end enum`         | `sıralama sonu`      |                                     |
| `interface`        | `arayüz`             |                                     |
| `end interface`    | `arayüz sonu`        |                                     |
| `lambda`           | `lambda`             | Anonim fonksiyon                    |
| `end lambda`       | `lambda sonu`        |                                     |
| `try`              | `dene`               |                                     |
| `catch`            | `hata yakala`        |                                     |
| `finally`          | `sonunda`            |                                     |
| `end try`          | `dene sonu`          |                                     |
| `module`           | `modül`              |                                     |
| `end module`       | `modül sonu`         |                                     |
| `scope`            | `kapsam`             |                                     |
| `end scope`        | `kapsam sonu`        |                                     |

---

## Kontrol Akışı

| İngilizce    | Türkçe          | Notlar                                          |
|--------------|-----------------|-------------------------------------------------|
| `return`     | `döndür`        | Prefix; `1 döndür` postfix de geçerli           |
| `break`      | `çık`           | Döngüden çık                                    |
| `continue`   | `devam`         | Sonraki iterasyon                               |
| `exit_scope` | `çık kapsam`    |                                                 |
| `where`      | `sadece`        | Foreach filtresi                                |
| `as`         | `olarak`        | Tip dönüştürme/belirteci                        |
| `import`     | `içe_aktar`     |                                                 |
| `export`     | `dışa_aktar`    |                                                 |
| `package`    | `paket`         |                                                 |
| `main`       | `giriş`         | Program giriş noktası                          |
| `not`        | `değil`         | Prefix (`değil x`) **veya** postfix (`x değil`) |
| `and`        | `ve`            |                                                 |
| `or`         | `veya`          |                                                 |

### `değil` Operatörü — Postfix Kuralı

Postfix `değil` yalnızca solundaki tek ifadeye bağlanır (normalizer çevirir, parser değişmez):

| Yazım | Anlam |
|-------|-------|
| `x > 5 değil` | `not (x > 5)` |
| `x > 5 değil ve y < 3` | `(not (x > 5)) ve (y < 3)` |
| `x > 5 ve y < 3 değil` | `(x > 5) ve (not (y < 3))` |
| `(x > 5 ve y < 3) değil` | `not (x > 5 ve y < 3)` |

```mlp
-- prefix form
koşul değil bayrak ise
    yaz("bayrak yanlış")
koşul sonu

-- postfix form (Türkçe'ye özgü — aynı anlam)
koşul bayrak değil ise
    yaz("bayrak yanlış")
koşul sonu

-- until semantiği (döngüde postfix)
döngü i >= 10 değil
    yaz(i)
    i = i + 1
döngü sonu
-- Eşdeğeri: döngü i < 10
```

---

## Veri Tipleri

| İngilizce  | Türkçe       |
|------------|--------------|
| `numeric`  | `sayısal`    |
| `string`   | `metin`      |
| `boolean`  | `mantıksal`  |
| `array`    | `dizi`       |
| `list`     | `liste`      |
| `struct`   | `yapı`       |
| `enum`     | `sıralama`   |
| `true`     | `doğru`      |
| `false`    | `yanlış`     |
| `null`     | `boş`        |

---

## Fonksiyon Niteleyicileri

Dönüş tipinden önce yazılır:

| İngilizce    | Türkçe            | Örnek                                           |
|--------------|-------------------|-------------------------------------------------|
| `async`      | `eşzamansız`      | `metin eşzamansız fonksiyon veriGetir(...)`     |
| `await`      | `bekle`           | `metin yanıt = bekle http_al(adres)`            |
| `pure`       | `saf`             | `sayısal saf fonksiyon topla(...)`              |
| `inline`     | `satıriçi`        | `sayısal satıriçi fonksiyon hızlı(...)`         |
| `comptime`   | `derleme_zamanı`  | `sayısal derleme_zamanı fonksiyon fakt(...)`    |
| `own`        | `sahip`           | `fonksiyon sahiplen(sahip metin s)`             |
| `&mut`       | `& değiş`         | `fonksiyon düzenle(metin& değiş s)`             |
| `const`      | `sabit`           | `sabit sayısal PI = 3.14159`                    |
| `export fn`  | `dışa_aktar fonksiyon` | `sayısal dışa_aktar fonksiyon kare(...)`   |

---

## Döngü Formu Özeti

| Anlam                | Türkçe sözdizimi                          | Kapanış       |
|----------------------|-------------------------------------------|---------------|
| while (ön koşul)     | `döngü koşul`                             | `döngü sonu`  |
| sonsuz               | `döngü`                                   | `döngü sonu`  |
| do-while (son koşul) | `döngü` ... `döngü sonu koşul`            | `döngü sonu`  |
| until                | `döngü değil koşul`                       | `döngü sonu`  |
| aralık (artan)       | `döngü i = 0 dan 10 a kadar`              | `döngü sonu`  |
| aralık (kısa)        | `döngü i = 0 dan 10`                      | `döngü sonu`  |
| aralık (azalan)      | `döngü i = 10 dan 0 geriye`               | `döngü sonu`  |
| foreach              | `döngü her öğe içinde koleksiyon`         | `için sonu`   |
| foreach (filtreli)   | `döngü her öğe içinde kol sadece koşul`  | `için sonu`   |

---

## Örnek Programlar

### Temel

```mlp
sayısal fonksiyon faktoriyel(sayısal n)
    koşul n <= 1 ise
        döndür 1
    koşul sonu
    döndür n * faktoriyel(n - 1)
fonksiyon sonu

fonksiyon giriş()
    sayısal sonuç = faktoriyel(10)
    yaz(sonuç)
fonksiyon sonu
```

### Koşul

```mlp
koşul x > 0 ise
    yaz("pozitif")
değilse x < 0 ise
    yaz("negatif")
yoksa
    yaz("sıfır")
koşul sonu
```

### Döngü

```mlp
-- Aralık döngüsü
döngü i = 0 dan 10 a kadar
    yaz(i)
döngü sonu

-- Foreach (filtreli)
sayısal[] sayılar = [1; 2; 3; 4; 5; 6]
döngü her n içinde sayılar sadece n mod 2 == 0
    yaz(n)
için sonu
```

### Yapı

```mlp
yapı Nokta
    sayısal x
    sayısal y
yapı sonu

sayısal fonksiyon Nokta.uzaklık()
    döndür bu.x + bu.y
fonksiyon sonu

Nokta n = Nokta { x: 3; y: 4; }
yaz(n.uzaklık())
```

### Örüntü Eşleme

```mlp
metin fonksiyon sınıflandır(sayısal x)
    döndür seç x
        durum 1 ise "bir"
        durum 2 ise "iki"
        durum _ => döndür "diğer"
    seç sonu
fonksiyon sonu
```

### Lambda

```mlp
sayısal fonksiyon ikile = lambda(sayısal x)
    döndür x * 2
lambda sonu

yaz(ikile(5))  -- 10
```

### Hata Yönetimi

```mlp
dene
    sayısal sonuç = riskli_işlem()
    yaz(sonuç)
hata yakala
    yaz("Hata: " + hata.mesaj)
sonunda
    temizle()
dene sonu
```

### Eşzamansız

```mlp
metin eşzamansız fonksiyon veriGetir(metin adres)
    metin yanıt = bekle http_al(adres)
    döndür yanıt
fonksiyon sonu

fonksiyon giriş()
    metin veri = bekle veriGetir("https://api.ornek.com")
    yaz(veri)
fonksiyon sonu
```

### Sıralama (Enum)

```mlp
sıralama Renk
    KIRMIZI
    YEŞİL
    MAVİ
sıralama sonu

Renk r = Renk.KIRMIZI
```

### Modül

```mlp
-- matematik.mlp
modül matematik
    sayısal dışa_aktar fonksiyon kare(sayısal x)
        döndür x * x
    fonksiyon sonu
modül sonu

-- giriş.mlp
içe_aktar matematik
fonksiyon giriş()
    yaz(matematik.kare(10))
fonksiyon sonu
```

---

## Belge Yorumu

Fonksiyon/yapı tanımı üstüne `---/ /---` ile belge yorumu eklenir — LSP hover tooltip'te görünür:

```mlp
---/
İki sayıyı toplar.
@param a İlk sayı
@param b İkinci sayı
/---
sayısal fonksiyon topla(sayısal a; sayısal b)
    döndür a + b
fonksiyon sonu
```

> **ESK-001:** `---/ ... /---` lexer token'ı henüz Stage1'de yok; LSP hover Stage0
> binary üzerinden `getDocCommentAbove()` ile çalışıyor (22 Mart 2026 düzeltmesi).

---

## Dosya Uzantısı

| Uzantı     | Anlam                                       |
|------------|---------------------------------------------|
| `.mlp`     | Standart MELP (İngilizce sözdizimi)         |
| `.tr.mlp`  | Türkçe MELP — normalizer `.mlp`'ye çevirir  |

```bash
# Normalizer + derleme
melp_normalizer dosyam.tr.mlp -o cikti.mlp
mlp derle cikti.mlp
```

---

## Kanonik Yazım Notu

Kullanıcıya açık Türkçe belgelerde aşağıdaki kanonik biçimler kullanılmalıdır:

| English MLP | Türkçe MLP |
|---|---|
| `if` | `koşul` |
| `else if` | `değilse koşul` |
| `else` | `yoksa` |
| `end if` | `koşul sonu` |
| `function` | `fonksiyon` |
| `end function` | `fonksiyon sonu` |
| `print` | `yaz` |
| `main` | `giriş` |

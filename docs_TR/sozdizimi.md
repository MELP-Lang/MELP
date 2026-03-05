# MELP Türkçe Sözdizimi Referansı

Bu dosya **kanonik** Türkçe keyword eşlemelerini tanımlar.  
Normalizer (`src/normalizer/lang_turkish.mlp`) bu listeyi uygular.

---

## Blok Anahtar Kelimeleri

| İngilizce           | Türkçe (kanonik)       | Açıklama                        |
|---------------------|------------------------|---------------------------------|
| `function`          | `fonksiyon`            | Fonksiyon tanımı başlangıcı     |
| `end_function`      | `fonksiyon_sonu`       | Fonksiyon tanımı sonu           |
| `if`                | `eğer`                 | Koşul başlangıcı                |
| `then`              | `sonra`                | Koşul gövdesi başlangıcı        |
| `else_if`           | `değilse_eğer`         | Alternatif koşul                |
| `else`              | `değilse`              | Koşul karşılanmazsa             |
| `end_if`            | `eğer_sonu`            | Koşul sonu                      |
| `while`             | `döngü`                | Döngü başlangıcı                |
| `end_while`         | `döngü_sonu`           | Döngü sonu                      |
| `for`               | `için`                 | For döngüsü                     |
| `end_for`           | `için_sonu`            | For döngüsü sonu                |
| `match`             | `eşleş`                | Pattern matching                |
| `case`              | `durum`                | Eşleşme durumu                  |
| `end_match`         | `eşleş_sonu`           | Pattern matching sonu           |
| `struct`            | `yapı`                 | Yapı tanımı                     |
| `end_struct`        | `yapı_sonu`            | Yapı tanımı sonu                |
| `class`             | `sınıf`                | Sınıf tanımı                    |
| `end_class`         | `sınıf_sonu`           | Sınıf tanımı sonu               |

## Kontrol Akışı

| İngilizce  | Türkçe (kanonik) | Açıklama             |
|------------|------------------|----------------------|
| `return`   | `döndür`         | Değer döndür         |
| `as`       | `olarak`         | Tip belirteci        |
| `each`     | `her`            | For-each             |
| `import`   | `içe_aktar`      | Modül içe aktar      |
| `export`   | `dışa_aktar`     | Modül dışa aktar     |
| `module`   | `modül`          | Modül bildirimi      |
| `package`  | `paket`          | Paket bildirimi      |

## Veri Tipleri

| İngilizce  | Türkçe (kanonik) | Açıklama               |
|------------|------------------|------------------------|
| `numeric`  | `sayısal`        | Sayısal tip            |
| `string`   | `metin`          | Metin (string) tip     |
| `boolean`  | `mantıksal`      | Mantıksal (bool) tip   |
| `array`    | `dizi`           | Dizi tip               |
| `list`     | `liste`          | Liste tip              |
| `struct`   | `yapı`           | Yapı tip               |
| `class`    | `sınıf`          | Sınıf tip              |
| `variable` | `değişken`       | Değişken bildirimi     |

## Değer Sabitleri

| İngilizce | Türkçe (kanonik) | Açıklama       |
|-----------|------------------|----------------|
| `true`    | `doğru`          | Mantıksal doğru|
| `false`   | `yanlış`         | Mantıksal yanlış|
| `null`    | `boş`            | Boş değer      |

## Mantıksal Operatörler

| İngilizce | Türkçe (kanonik) | Açıklama |
|-----------|------------------|----------|
| `and`     | `ve`             | VE       |
| `or`      | `veya`           | VEYA     |
| `not`     | `değil`          | DEĞİL    |

---

## Örnek Program

```melp
fonksiyon faktoriyel(sayısal n) olarak sayısal
    eğer n <= 1 sonra
        döndür 1
    eğer_sonu
    döndür n * faktoriyel(n - 1)
fonksiyon_sonu

fonksiyon ana()
    sayısal i = 1
    döngü i <= 5
        print(str(i))
        i = i + 1
    döngü_sonu
    metin mesaj = "merhaba"
    mantıksal aktif = doğru
    print(mesaj)
fonksiyon_sonu
```

---

## Kabul Edilen Alternatifler (sadece okunabilirlik için)

Normalizer aşağıdaki alternatifleri de kanonik forma çevirir:

| Alternatif      | Kanonik             |
|-----------------|---------------------|
| `son_fonksiyon` | `fonksiyon_sonu`    |
| `işlev`         | `fonksiyon`         |
| `yoksa_eğer`    | `değilse_eğer`      |
| `değilse`       | `else`              |
| `yoksa`         | `else`              |
| `sayı`          | `sayısal`           |
| `yazı`          | `metin`             |
| `hiç`           | `boş`               |
| `dön`           | `döndür`            |

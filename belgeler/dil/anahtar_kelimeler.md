# MELP Anahtar Kelimeler — Türkçe ↔ İngilizce Tam Tablo

**Sürüm:** 1.0  
**Tarih:** 2026-03-30  
**Kanonik kaynak:** `ORTAK/dil/tr/keywords.json`

---

## Kontrol Akışı

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `koşul ... ise` | `if ... then` | Koşul başlangıcı |
| `değilse koşul ... ise` | `else if ... then` | Ek koşul dalı |
| `yoksa` | `else` | Varsayılan dal |
| `koşul sonu` | `end if` | Koşul bloğu sonu |
| `döngü` | `loop` | Döngü başlangıcı |
| `döngü sonu` | `end loop` | Döngü sonu |
| `her ... içinde ...` | `for each ... in ...` | Foreach başlangıcı |
| `her sonu` | `end for` | Foreach sonu |
| `kadar` | `to` | Sayan döngü artış yönü |
| `geriye` | `downto` | Sayan döngü azalış yönü |
| `adım` | `step` | Döngü adımı |
| `sadece` | `where` | Foreach filtresi |
| `çık` | `exit` | Döngüden çıkış |
| `devam` | `continue` | Sonraki iterasyon |

## Fonksiyon ve Değer

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `fonksiyon` | `function` | Fonksiyon tanımı |
| `fonksiyon sonu` | `end function` | Fonksiyon sonu |
| `döndür` | `return` | Değer döndür |
| `lambda` | `lambda` | Lambda fonksiyon |
| `lambda sonu` | `end lambda` | Lambda sonu |
| `sabit` | `const` | Sabit tanımı |
| `saf` | `pure` | Yan etkisiz fonksiyon |
| `satıriçi` | `inline` | Inline öneri |
| `giriş` | `main` | Program giriş noktası |

## Tipler

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `sayısal` | `numeric` | Sayısal tip |
| `metin` | `string` | Metin tipi |
| `mantıksal` | `boolean` | Boolean tip |
| `hiçbir` | `none` | Dönüş değeri yok |
| `boş` | `null` | Null değer |
| `doğru` | `true` | Boolean doğru |
| `yanlış` | `false` | Boolean yanlış |
| `liste` | `list` | Liste tipi |

## Yapılar

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `yapı` | `struct` | Yapı tanımı |
| `yapı sonu` | `end struct` | Yapı sonu |
| `bu` | `this` | Geçerli örnek |
| `sıralama` | `enum` | Enum tanımı |
| `sıralama sonu` | `end enum` | Enum sonu |
| `arayüz` | `interface` | Arayüz tanımı |
| `arayüz sonu` | `end interface` | Arayüz sonu |
| `uygular` | `implements` | Arayüz uygulama |
| `uygula` | `implement` | Uygulama bloğu |
| `uygula sonu` | `end implement` | Uygulama sonu |

## Hata Yönetimi

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `dene` | `try` | Hata yakalama bloğu |
| `dene sonu` | `end try` | Try sonu |
| `hata yakala` | `catch` | Hata yakalama dalı |
| `sonunda` | `finally` | Daima çalışan blok |
| `fırlat` | `throw` | Hata fırlat |

## Seçim / Match

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `seç` | `match` | Match başlangıcı |
| `seç sonu` | `end match` | Match sonu |
| `durum` | `case` | Eşleme durumu |

## Modüler Yapı (Stage 2)

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `modül` | `module` | Modül tanımı |
| `modül sonu` | `end module` | Modül sonu |
| `içe_aktar` | `import` | Modül içe aktarma |
| `dışa_aktar` | `export` | Modül dışa aktarma |

## Eşzamanlılık (Stage 2)

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `eşzamansız` | `async` | Async fonksiyon |
| `bekle` | `await` | Async bekleme |
| `başlat` | `spawn` | Thread/görev başlatma |
| `başlat sonu` | `end spawn` | Spawn sonu |
| `olay` | `event` | Olay tanımı |
| `olay sonu` | `end event` | Olay sonu |

## Mant ıksal Operatörler

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `ve` | `and` | Mantıksal ve |
| `veya` | `or` | Mantıksal veya |
| `değil` | `not` | Mantıksal değil (prefix veya postfix) |

## Diğer

| Türkçe MLP | İngilizce MLP | Açıklama |
|---|---|---|
| `yaz` / `yazdır` | `print` | Stdout çıktı |
| `satıryaz` | `println` | Yeni satır ekli çıktı |
| `türü` | `typeof` | Tip sorgulama |
| `aralık` | `range` | Aralık operatörü |
| `mod` | `mod` | Modulo operatörü |
| `sahip` | `own` | Sahiplik belirteci |
| `değiş` | `mut` | Mutable belirteci |
| `dışsal` | `external` | FFI harici tanım |

## Alias'lar (Kabul Edilen Alternatif Yazımlar)

| Alias | Canonical Türkçe | İngilizce |
|---|---|---|
| `eğer` | `koşul` | `if` |
| `değilse` | `yoksa` | `else` |
| `yoksa koşul` | `değilse koşul` | `else if` |
| `dön` | `döndür` | `return` |
| `işlev` | `fonksiyon` | `function` |
| `sayı` | `sayısal` | `numeric` |
| `yazı` | `metin` | `string` |
| `eşleş` | `seç` | `match` |
| `ana` | `giriş` | `main` |

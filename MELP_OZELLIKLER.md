# MELP — Öne Çıkan Dil Özellikleri

**Versiyon:** Stage 1  
**Tarih:** 22 Mart 2026  
**Dil:** Türkçe (İngilizce: `MELP_FEATURES.md`)

---

## 1. Çok Dilli, Çok Sözdizimli Yapı

MELP'te her program birden fazla doğal dilde yazılabilir. Türkçe, İngilizce ve Rusça birinci sınıf sözdizimi olarak desteklenir.

```
kodun.tr.mlp   → Türkçe sözdizimi
kodun.ru.mlp   → Rusça sözdizimi
kodun.mlp      → İngilizce (canonical) sözdizimi
```

Her dil versiyonu, normalizer katmanından geçerek kanonik MELP formuna (`.mlp`) dönüştürülür; ardından aynı derleyici tüm dilleri derler. Kullanıcı hangi dili seçerse seçsin üretilen binary özdeştir.

Syntax esnekliği yalnızca keyword çevirisiyle sınırlı değildir. Her diyalekt kendi kapanış biçimine sahiptir:

```melp
-- Türkçe
sayısal fonksiyon topla(sayısal a; sayısal b)
    döndür a + b
fonksiyon sonu

-- İngilizce
numeric function add(numeric a; numeric b)
    return a + b
end function
```

Aynı program. Aynı binary. Farklı yazım biçimi.

---

## 2. Tasarım Zamanı Debug Keyword'ü

MELP, programcının kaynak koda "denetim noktası" yerleştirmesine olanak tanıyan yerleşik bir `debug` keyword'üne sahiptir. Bu keyword özel bir derleyici bayrağı (`--debug-mode`) ile aktif olur; üretim derlemelerinde tamamen etkisizleşir — sıfır maliyet.

```melp
numeric function factorial(numeric n)
    debug("faktöriyel çağrıldı", n)   -- yalnızca --debug-mode'da çalışır
    if n <= 1 then
        return 1
    end if
    return n * factorial(n - 1)
end function
```

Derleyici `--debug-mode` bayrağı olmadan derlendiğinde `debug(...)` satırları hiç üretilmez. Bu sayede geliştirici assert/log kalabalığına gerek duymadan üretim kodunu temiz tutar.

---

## 3. Sınırsız Adlandırılmış Kapsam (scope)

MELP'te `scope` bloğu, fonksiyon sınırları dışında herhangi bir yerde oluşturulabilir. Her `scope`'a isim verilebilir. Bu sayede:

- Kaynaklar (bellek, dosya, kilit) kapsam sonunda otomatik serbest bırakılır
- Kodun okunabilirliği artar — blok ne iş yaptığı ismiyle belli olur
- İç içe scope'lar mantıksal bölümler oluşturur

```melp
scope "veritabanı bağlantısı"
    Connection db = db_connect("sunucu")
    -- kapsam biterken db otomatik kapatılır
    numeric sonuc = db.query("SELECT count(*) FROM users")
end scope

scope "dosya yazma"
    File f = file_open("rapor.txt", "w")
    file_write(f, sonuc)
end scope
```

Türkçe eşdeğeri:

```melp
kapsam "veritabanı bağlantısı"
    ...
kapsam sonu
```

Adlandırılmış kapsam ile anonim kapsam aynı mekanizmayı paylaşır; isim yalnızca okunabilirlik ve `exit` hedefi için kullanılır.

---

## 4. Herhangi Bir Kapsamdan Koşullu Çıkış (`exit if`)

MELP, iç içe geçmiş blokların herhangi bir noktasından belirli bir kapsama adı vererek çıkmayı mümkün kılar. Bu özellik, derin iç içe döngülerde veya karmaşık koşul ağaçlarında `break`/`goto` karmaşası olmadan temiz akış kontrolü sağlar.

```melp
scope "arama"
    loop item in liste
        loop alt in item.altkayitlar
            if alt.deger > esik then
                exit scope "arama"   -- doğrudan dış scope'tan çık
            end if
        end loop
    end loop
end scope
-- buraya gelindi: ya arama bitti ya da eşik aşıldı
```

`exit if` biçimi koşulu da satır içi taşır:

```melp
exit scope "arama" if hata_var
```

Bu sayede `flag` değişkeni tutmak ya da çok katlı `break` kullanmak gerekmez. Akar ve okunur.

---

## 5. OK (Optimizasyon Kalite Katmanı)

MELP derleyicisi, üretilen kodu çalıştırmadan önce üç katmanlı bir optimizasyon ve analiz hattından geçirir:

```
Kaynak Kod
    │
    ▼
 AST (Soyut Sözdizimi Ağacı)
    │
    ▼
┌───────────────────────────────┐
│  STO — Smart Type Optimization      │  ← numeric → i64/f64/BigDecimal seçer
│  SAO — Semantic-Aware Optimization  │  ← Semantic metadata'yı backend'e taşır
│  FBO — Frontend Backend Optimization │  ← Birleştirme, ölü kod eleme, güvenlik
└───────────────────────────────┘
    │
    ▼
 LLVM IR → Binary
```

**STO (Smart Type Optimization):** Kullanıcı yalnızca `numeric`/`string`/`boolean` yazar; STO derleme zamanında değerin büyüklüğüne ve kullanım bağlamına göre `i64`, `f64` veya `BigDecimal` seçer. Kullanıcı dahili tipi hiç görmez.

**SAO (Semantic-Aware Optimization):** Frontend'in semantik analiz sırasında öğrendiklerini (tip kanıtları, purity, no-alias) backend'e LLVM metadata olarak taşır. "İki Aşamalı Amnezi" problemini çözer: frontend kanıtları backend’in agresif optimizasyonu için kullanılır.

**FBO (Frontend Backend Optimization):** Her AST düğümüne 4 byte'lık metadata paketi atar (Byte 0: STO, Byte 1: SAO, Byte 2: Safety, Byte 3: Effect). Ölü semantik yolları elimine eder; güvenlik katmanı (`SafetyLevel`) burada çalışır.

OK katmanı, programcının ek bayrak vermesine gerek kalmadan her derleme işleminde çalışır. Sonuç: fazladan yazım olmadan daha güvenli ve daha hızlı kod.

## 6. Üç Tür Sistem — Boyut Kaygısı Yok

MELP'te kullanıcının bilmesi gereken yalnızca üç değişken türü vardır:

| Tür | Karşılık | Örnek |
|---|---|---|
| `numeric` / `sayısal` | Tüm sayılar (tam, ondalık, büyük) | `1`, `3.14`, `10**23` |
| `string` / `metin` | Metin | `"merhaba"` |
| `boolean` / `mantıksal` | Doğru/yanlış | `doğru`, `yanlış` |

`int`, `long`, `float`, `double`, `int32`, `uint64` gibi ayrımlar yoktur. Kullanıcı `a = 1` veya `b = 10**23` yazdığında hangi CPU alanının kullanılacağını düşünmek zorunda kalmaz.

Bu ayrımı derleyicinin OK katmanındaki **STO (Smart Type Optimization)** yapar. STO, değerin büyüklüğünü ve kullanım bağlamını statik olarak analiz ederek CPU'da ihtiyaca göre alan tahsis eder.

```melp
-- Türkçe: kullanıcı boyut belirtmez
sayısal küçük = 5
sayısal büyük = 10 ** 23
sayısal ondalık = 3.14

-- Üçü de "sayısal" — STO doğru alanı seçer
yaz(küçük + büyük + ondalık)
```

Bu tasarım iki şeyi birden sağlar: **okunabilirlik** (tip ayrıntısı yok) ve **doğruluk** (taşma veya kesilme riski STO tarafından derleme zamanında yakalanır).

---

## 7. Enum — Tip Güvenli Sabit Grupları

MELP'te `enum` (Türkçe: `sıralama`) ile birbirine ilişkili sabitler tek yapı altında gruplandırılır. Değerler sıfırdan başlayarak otomatik numaralandırılır; `EnumAdı.DEĞER` notasyonuyla erişilir. Derleyici `match` ifadesiyle birlikte kullanıldığında tüm durumların kapsandığını (exhaustive) derleme zamanında doğrular.

```melp
-- Türkçe
sıralama Renk
    KIRMIZI
    YEŞİL
    MAVİ
sıralama sonu
-- KIRMIZI=0, YEŞİL=1, MAVİ=2

sayısal renk = Renk.YEŞİL

seç renk
    case Renk.KIRMIZI ise yaz("kırmızı")
    case Renk.YEŞİL   ise yaz("yeşil")
    case Renk.MAVİ    ise yaz("mavi")
seç sonu
```

```melp
-- İngilizce
enum Color
    RED
    GREEN
    BLUE
end enum

numeric c = Color.GREEN

match c
    case Color.RED   then print("red")
    case Color.GREEN then print("green")
    case Color.BLUE  then print("blue")
end match
```

Hem `int`/`uint` ayrımı yoktur hem de değer aralığı kontrolü otomatiktir. Derleyici `match` içinde listelenmeyen bir enum değeri tespit ederse derleme hatası verir. Bu, runtime'da unutulan dal hatasını derleme zamanına taşır.

**Kaynak:** [`belgeler/language/feature_cards/ENUM.md`](belgeler/language/feature_cards/ENUM.md)

---

## 8. Desen Eşleştirme (Pattern Matching)

MELP'in `match / case / end match` yapısı, değeri olası kalıplarla karşılaştırır ve her durumda ne yapılacağını belirler. `if/else if` zincirinin okunaksız hale geldiği durumlarda temiz ve güvenli bir alternatif sunar.

Üç temel özelliği birlikte çalışır:

- **Guard clause:** `case x if x > 10 then` — Kalıbın yanına koşul ekler.
- **Enum destructuring:** `case Some(değer) then` — Enum içindeki değeri otomatik çıkarır.
- **İfade olarak match:** `match` bir değer döndürebilir; atama ifadesine dönüşebilir.

```melp
-- Türkçe: sayısal aralık analizi
sayısal n = 15

seç n
    case x eğer x < 10  ise yaz("küçük")
    case x eğer x < 100 ise yaz("orta")
    değilse                  yaz("büyük")
seç sonu
```

```melp
-- İngilizce: match as expression
string label = match n
    case x if x < 10  then "small"
    case x if x < 100 then "medium"
    else                   "large"
end match
```

```melp
-- İngilizce: enum destructuring ile Option<T>
enum Option<T>
    Some(T)
    None
end enum

Option<numeric> result = find_user(42)

match result
    case Some(user_id) then print("Bulundu: " + user_id)
    case None          then print("Yok")
end match
```

Bir `match` içinde tüm enum değerleri kapsanmazsa derleyici uyarı verir. Bu, yeni enum değeri eklendiğinde eski `match` bloklarının güncellenmesini zorunlu kılar.

**Kaynak:** [`belgeler/language/feature_cards/PATTERN_MATCHING.md`](belgeler/language/feature_cards/PATTERN_MATCHING.md)

---

## 9. Tür Parametreli Yapılar (Generics)

MELP, `<T>` sözdizimi ile tür parametreli fonksiyonlar ve yapılar tanımlamayı destekler. Aynı mantığı farklı veri tipleriyle kullanmak için kod kopyalamak gerekmez; derleyici her somut kullanım için parametreyi örnek üretir (monomorphization).

```melp
-- Türkçe: genel kutu yapısı
yapı Kutu<T>
    T değer
    mantıksal dolu
yapı sonu

Kutu<sayısal> sayıKutusu
sayıKutusu.değer = 100
sayıKutusu.dolu  = doğru

Kutu<metin> metinKutusu
metinKutusu.değer = "merhaba"
metinKutusu.dolu  = doğru
```

```melp
-- İngilizce: generic function
T function identity<T>(T value)
    return value
end function

numeric x = identity<numeric>(42)
string  s = identity<string>("hello")
```

Generics'in en yaygın kullanımı `Result<T; E>` ve `Option<T>` hata-yönetim tipleridir. Bunlar kullanıcı tanımlı enum olarak tanımlanır; işlem başarılıysa `Ok(değer)`, hatalıysa `Err(hata)` döner. Böylece `null` kontrolü veya exception fırlatma yerine tip güvenceli hata akışı yazılır.

```melp
-- İngilizce: Result<T; E> ile hata yönetimi
enum Result<T; E>
    Ok(T)
    Err(E)
end enum

Result<numeric; string> parse_age(string input)
    if input == "" then
        return Result.Err("boş girdi")
    end if
    return Result.Ok(42)
end function

match parse_age("...")
    case Ok(age)  then print("Yaş: " + age)
    case Err(msg) then print("Hata: " + msg)
end match
```

**Kaynak:** [`belgeler/language/feature_cards/GENERICS.md`](belgeler/language/feature_cards/GENERICS.md) · [`belgeler/RESULT_TYPE_USAGE_GUIDE.md`](belgeler/RESULT_TYPE_USAGE_GUIDE.md)

---

## Daha Fazla Bilgi

| Konu | Belge |
|---|---|
| Tüm öne çıkan özellikler (İngilizce) | [`MELP_FEATURES.md`](MELP_FEATURES.md) |
| Türkçe/İngilizce sözdizimi referansı | [`belgeler/mlp1_sozdizimi.md`](belgeler/mlp1_sozdizimi.md) |
| Tüm anahtar kelimeler (TR↔EN tablosu) | [`belgeler/language/mlp_keywords_turkish.md`](belgeler/language/mlp_keywords_turkish.md) |
| OK katmanı teknik detay | [`belgeler/OK_LAYER_ARCHITECTURE.md`](belgeler/OK_LAYER_ARCHITECTURE.md) |
| Debug module | [`belgeler/DEBUG_MODULE_BRIEFING.md`](belgeler/DEBUG_MODULE_BRIEFING.md) |
| scope, exit yapısı | [`belgeler/language/mlp1_syntax.md`](belgeler/language/mlp1_syntax.md) |
| Enum sözdizimi | [`belgeler/language/feature_cards/ENUM.md`](belgeler/language/feature_cards/ENUM.md) |
| Pattern matching | [`belgeler/language/feature_cards/PATTERN_MATCHING.md`](belgeler/language/feature_cards/PATTERN_MATCHING.md) |
| Generics | [`belgeler/language/feature_cards/GENERICS.md`](belgeler/language/feature_cards/GENERICS.md) |
| Result/Option kullanımı | [`belgeler/RESULT_TYPE_USAGE_GUIDE.md`](belgeler/RESULT_TYPE_USAGE_GUIDE.md) |
| FFI — C kütüphanesi çağırma | [`belgeler/FFI_USER_GUIDE.md`](belgeler/FFI_USER_GUIDE.md) |
| Tüm belge indeksi | [`belgeler/README.md`](belgeler/README.md) |

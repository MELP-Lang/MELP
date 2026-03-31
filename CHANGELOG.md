# Değişiklik Günlüğü

Bu proje [Keep a Changelog](https://keepachangelog.com/tr/1.0.0/) formatını kullanır.  
Sürümleme [Semantik Sürümleme](https://semver.org/) ilkelerine göre yapılır.

---

## [0.1.1-alfa] - 2026-03-31

### Düzeltmeler ve İyileştirmeler

- **BigDecimal FBO dispatch:** 17+ anlamlı basamaklı ondalık sayılar artık IEEE 754'e düşmüyor — `mlp_bigdecimal_from_str()` ile tam hassasiyetli hesaplama
- **BigInteger:** 19+ hane sayılar `mlp_biginteger_t` ile doğru işleniyor
- **Lambda:** Virgülle ayrılmış parametre listesi ve `-> Tip` dönüş tipi anotasyonu desteği eklendi
- **Codegen:** Explicit `x86_64-pc-linux-gnu` target triple ile daha güvenilir derlemeler
- **STAGE0:** 775/775 golden test tümüyle geçiyor (Product Ready)
- **FBO Tablo:** Statik 52+ katalog + 4.29 milyar dinamik kapasite belgelendi
- `--ok-stats` çıktısına `BIGDECIMAL: N` satırı eklendi

### Bilinen Kısıtlamalar (güncellendi)
- String interpolasyon `{isim}` çalışıyor; `${expr}` formu henüz desteklenmiyor
- `switch/case` derleniyor fakat çıktı üretmiyor (bilinen hata)

---

## [0.1.0-alfa] - 2026-03-11

### İlk Alfa Sürümü

Bu, MELP programlama dilinin halka açık ilk alfa sürümüdür.

### Eklenenler
- `melp_compiler` — LLVM tabanlı tam derleyici (Linux x86_64)
  - Değişkenler: `numeric`, `string`, `boolean`, otomatik tip çıkarımı (`var`, `auto`)
  - Kontrol akışı: `if/else if/else/end if`, `while/end while`, `for/end for`
  - Fonksiyonlar: parametreli, dönüş tipli, özyinelemeli
  - Veri yapıları: `struct`, `enum` (temel)
  - Koleksiyonlar: `numeric[]`, `string[]` (temel), `hashmap`
  - Standart kütüphane: `print()`, `str()`, `len()`, `read_line()`, matematik fonksiyonları
  - Modül sistemi: `import "dosya.mlp"`
  - Çok dilli sözdizimi: Türkçe (`.tr.mlp`), Rusça (`.ru.mlp`) anahtar kelimeler
- `mlp_runtime.bc` — LLVM bitcode runtime kütüphanesi
- 40+ örnek program (`examples/` dizini)
- Türkçe ve İngilizce belgeler (`belgeler/`, `docs_EN/`, `docs_TR/`)

### Bilinen Kısıtlamalar
- Yalnızca Linux x86_64 desteklenmektedir (macOS/Windows: Beta'da)
- Closure dış kapsam yakalama henüz tamamlanmamıştır
- Generic türler (`list<T>`) kısmen uygulanmıştır
- String interpolasyon (`${x}`) henüz yoktur — `str(x)` kullanın
- `switch/case` derleniyor fakat çıktı üretmiyor (bilinen hata)
- `Result<T>` generic dönüş tipi henüz desteklenmiyor

### Gereksinimler
- LLVM 14 (`clang-14`, `lli-14`)
- Linux x86_64

---

## Gelecek Sürümler

### [0.2.0-beta] - Planlanan
- macOS ve Windows desteği
- String interpolasyon (`${expr}`)
- `switch/case` düzeltmesi
- `Result<T>` ve `?` operatörü
- Closure dış kapsam yakalama
- CI/CD otomatik test raporu
- **[Normalizer]** Türkçe postfix `değil` operatörü: `x > 5 değil` → `not (x > 5)`
  - Solundaki tek ifadeye bağlanır; parantez ile daha geniş kapsam: `(x ve y) değil`
  - Döngülerde: `döngü i >= 10 değil` → until semantiği
- **[Stage0 Parser]** Tek satır `if-then` formu: `if koşul then komut`
  - `end if` gerektirmez; `then` sonrası newline yoksa tek statement alır
  - Örnek: `if not running then exit`
- **[Stage0 Parser]** Adlandırılmış döngü etiketi: `loop isim koşul ... end loop isim`
  - `exit isim` ve `continue isim` ile iç içe döngüden hedefli çıkış
  - Örnek: `loop outer i = 0 to 10 ... exit outer ... end loop outer`
- **[Stage0 Codegen]** Implicit return: fonksiyonun son ifadesi `return` yazmadan döner
  - Erken çıkış için `return` hala geçerli; sadece zorunluluk kalktı

### [1.0.0] - Planlanan (v2 sonrası)
- Derleyicinin MELP ile yazılmış sürümü (öz-derleme)
- Tam platform desteği
- Package manager

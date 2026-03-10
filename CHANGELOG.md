# Değişiklik Günlüğü

Bu proje [Keep a Changelog](https://keepachangelog.com/tr/1.0.0/) formatını kullanır.  
Sürümleme [Semantik Sürümleme](https://semver.org/) ilkelerine göre yapılır.

---

## [0.1.0-alfa] - 2026-03-11

### İlk Alfa Sürümü

Bu, MELP programlama dilinin halka açık ilk alfa sürümüdür.

### Eklenenler
- `melp_compiler` — LLVM tabanlı tam derleyici (Linux x86_64)
  - Değişkenler: `numeric`, `string`, `boolean`, otomatik tip çıkarımı (`var`, `auto`)
  - Kontrol akışı: `if/else_if/else/end_if`, `while/end_while`, `for/end_for`
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

### [1.0.0] - Planlanan (v2 sonrası)
- Derleyicinin MELP ile yazılmış sürümü (öz-derleme)
- Tam platform desteği
- Package manager

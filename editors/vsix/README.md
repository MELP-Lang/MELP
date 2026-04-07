# MELP IDE — VS Code Uzantısı

> **Alpha Sürümü** — Aktif geliştirme devam ediyor. Temel özellikler çalışıyor; bazı sınırlamalar mevcuttur (bkz. [Bilinen Sorunlar](#bilinen-sorunlar)).

MELP (Multi-Language Programming), kullanıcının kendi doğal dilinde kod yazabildiği bir programlama dilidir. Türkçe, Rusça veya İngilizce olarak yazılan program aynı derleyiciye gönderilir — sonuç aynı binary'dir.

```
fonksiyon selamla(metin isim)          function greet(string name)
    yaz("Merhaba, " + isim + "!")   ←→     print("Hello, " + name + "!")
fonksiyon sonu                         end_function
```

---

## Kurulum

### Gereksinimler

| Bileşen | Sürüm | Notlar |
|---|---|---|
| VS Code | ≥ 1.85 | VSCodium da destekleniyor |
| Linux x86-64 | — | macOS / Windows yakında |
| `lli` | ≥ 14 | `sudo apt install llvm` |

### .vsix ile Kurulum

1. En son `.vsix` dosyasını [Releases](../../releases) sayfasından indirin.
2. VS Code'da: `Ctrl+Shift+P` → **"Extensions: Install from VSIX..."**
3. İndirilen dosyayı seçin.

### Market'ten Kurulum (yakında)

```
ext install pardus.mlp-ide
```

---

## Hızlı Başlangıç

### "Merhaba Dünya" — Türkçe

1. `merhaba.tr.mlp` adında yeni bir dosya oluşturun.
2. Aşağıdaki kodu yapıştırın:

```mlp
fonksiyon giriş()
    yaz("Merhaba, Dünya!")
fonksiyon sonu
```

3. `F5` tuşuna basın — program derlenir ve çalışır.

### "Hello World" — English

1. Create a new file named `hello.mlp`.
2. Paste the following:

```mlp
function main()
    print("Hello, World!")
end_function
```

3. Press `F5` — the program compiles and runs.

> **İpucu:** Dil ayarı için `Ctrl+Shift+P` → **"MLP: Dil Seç"** komutunu kullanın ya da `settings.json`'a `"mlp.language": "tr"` ekleyin.

---

## Desteklenen Özellikler (Alpha)

| Özellik | Durum | Not |
|---|---|---|
| Türkçe sözdizimi | ✅ | Normalizer üzerinden |
| İngilizce sözdizimi | ✅ | Doğrudan derleyici |
| Rusça sözdizimi | ✅ | Normalizer üzerinden |
| IntelliSense (completion) | ✅ | 121 keyword, snippet |
| Syntax highlighting | ✅ | `.mlp`, `.tr.mlp`, `.ru.mlp` |
| Hata işaretleme (diagnostic) | ✅ | W1-W8 / E1-E9 kuralları |
| Hover dokümantasyonu | ✅ | İngilizce — TR yakında |
| Go-to-definition | 🔶 Kısmi | Fonksiyon tanımları |
| F5 ile çalıştır | ✅ | stdout → Output Panel |
| GUI tasarımcı | ✅ | `.mlpgui` dosyaları |
| WASM (tarayıcı derleme) | ⏳ | Beta'da gelecek |

---

## Dil Özellikleri

MELP Alpha şu özellikleri destekler:

- `numeric`, `string`, `boolean` veri tipleri
- `if / else if / else / end_if`
- `while / end_while`, `for / end_for`, `for each / end_for`
- Fonksiyon tanımı ve çağrısı
- `numeric[]`, `string[]`, `boolean[]` diziler
- `struct / end_struct` yapıları ve metotları
- `print()`, `str()`, `len()`, `input()` yerleşik fonksiyonlar
- Modül importları

Türkçe karşılıklar için [YARDIM/](../../../LLVM/STAGE0/YARDIM/) belgelere bakın.

---

## Bilinen Sorunlar

Bu bir **Alpha** sürümüdür. Aşağıdaki sınırlamalar bilinmektedir:

| # | Sorun | Durum |
|---|---|---|
| 1 | WASM yok — derleme için `lli` (LLVM) kurulu olmalı | Beta'da çözülecek |
| 2 | macOS / Windows binary yok | Beta'da gelecek |
| 3 | `---` blok yorum sözdizimi STAGE0 lexer'da yok (ESK-001) | Geçici çözüm: `--` satır yorumu kullanın |
| 4 | `for j = 0 dan 10 a kadar` yapısı bazı kenar vakalarda yanlış çevrilebilir (NRM-004) | Geçici çözüm: `döngü j <= 10` kullanın |
| 5 | Hover Türkçe keyword açıklamalarını göstermiyor | Session 2'de düzelecek |

---

## Örnek Dosyalar

`examples/` klasöründe hazır örnekler bulunmaktadır:

- [`merhaba_dünya.tr.mlp`](examples/merhaba_dünya.tr.mlp) — Türkçe tam örnek (koşul, döngü, fonksiyon)

Daha fazla örnek için: [`LLVM/STAGE1/examples/`](../../LLVM/STAGE1/examples/)

---

## Mimari Özeti

```
.tr.mlp  →  Normalizer  →  STAGE0 Derleyici  →  LLVM IR  →  binary
.ru.mlp  →  Normalizer  ↗
.mlp     ──────────────────────────────────────→  LLVM IR  →  binary
```

MELP üç aşamada selfhosting'e ulaşacaktır:

| Aşama | Tanım | Durum |
|---|---|---|
| Stage 0 | C++ derleyici, PMLP derler | ✅ Tamamlandı |
| Stage 1 | C++ derleyici, MLP derler | 🔄 Devam ediyor (Alpha → Beta) |
| Stage 2 | MLP derleyici, MLP derler | 📋 Planlandı (Beta → Stable) |

---

## Katkı

Proje aktif geliştirme aşamasındadır. Hata bildirimi, öneri ve katkı için [Issues](../../issues) sayfasını kullanın.

Katkı rehberi: yakında.

---

## Lisans

[MIT](LICENSE)

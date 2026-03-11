# MELP Programlama Dili · Alfa

> Temiz sözdizimine sahip, statik tipli, derlenebilir bir programlama dili.  
> LLVM altyapısı sayesinde gerçek binary dosyaları üretir.

---

## Merhaba Dünya

```melp
function main()
    print("Merhaba Dünya!")
end_function
```

Türkçe anahtar kelimelerle de yazılabilir (`.tr.mlp` dosyaları):

```melp
fonksiyon ana()
    yaz("Merhaba Dünya!")
son_fonksiyon
```

---

## ▶ Hemen Dene (Kurulum Yok)

Tarayıcı tabanlı IDE ile MeLP'i anında kullanabilirsiniz:

**[melp-lang.github.io/melp-ide-web/](https://melp-lang.github.io/melp-ide-web/)**

> Yerel kurulum için aşağıya bakın.

---

## 🛠️ Editör Desteği

### 1. VS Code Extension (VSIX)

**Özellikler:**
- ✅ Syntax highlighting (Turkish/Russian/English)
- ✅ Code snippets (40+ şablon)
- ✅ IntelliSense
- ✅ Error highlighting
- ✅ Build & run (F5)

**Kurulum:**
1. [Releases](https://github.com/MELP-Lang/MELP/releases) sayfasından `melp-ide-*.vsix` indirin
2. VS Code: `Ctrl+Shift+P` → **Install from VSIX**
3. `.mlp` dosyası açın → `Ctrl+F5` ile çalıştırın

Derleyici extension içine gömülü — ayrı kurulum gerektirmez.

---

### 2. Web IDE (Ön İzleme)

**Tarayıcıda çalışır, kurulum gerektirmez:**

**[melp-lang.github.io/melp-ide-web/](https://melp-lang.github.io/melp-ide-web/)**

**Özellikler:**
- ✅ Syntax highlighting
- ✅ Canlı derleme
- ✅ Örnek kod kütüphanesi
- ✅ Paylaşım linki

**İdeal için:**
- Öğrenme/eğitim
- Hızlı deneme
- Kod paylaşma

**Sınırlamalar:**
- Dosya I/O yok
- FFI/native kütüphane yok

---

### 3. MeLP GUI IDE (Alfa)

**Electron tabanlı standalone IDE — VS Code'a gerek yok:**

**İndirme:** [Releases](https://github.com/MELP-Lang/MELP/releases) sayfasından `melp-ide-linux-x64.AppImage` indirin

**Kurulum (Linux):**
```bash
chmod +x melp-ide-linux-x64.AppImage
./melp-ide-linux-x64.AppImage
```

> ⚠️ **macOS ve Windows sürümleri Beta'da gelecek.** Şu an yalnızca Linux desteklenmektedir.

**Özellikler:**
- ✅ Görsel widget tasarımı (sürükle-bırak)
- ✅ Entegre terminal
- ✅ Proje yönetimi (.mlpgui formatı)
- ✅ Otomatik kod üretimi
- ⚠️ Alfa aşamasında

**Desteklenen Platform:**
- ✅ Linux: `.AppImage` (tek dosya, kurulum gerektirmez)
- 🔄 macOS: Beta'da gelecek  
- 🔄 Windows: Beta'da gelecek

---

### Hangi Editörü Seçmeliyim?

| Kullanım | Öneri |
|----------|-------|
| 🎓 Yeni öğreniyorum | Web IDE |
| 💻 Ciddi geliştirme | VS Code Extension |
| 🚀 Production | VS Code + native compiler |
| 📚 Eğitim veriyorum | Web IDE |

**Alfa sürümünde yalnızca Linux desteklenmektedir.** macOS ve Windows desteği Beta'da gelecek.

---

## Dil Özellikleri

```melp
-- Değişkenler
numeric x = 42
numeric pi = 3,14        -- Virgül ondalık ayırıcıdır
string isim = "MELP"
boolean aktif = true

-- Fonksiyonlar
function topla(a as numeric; b as numeric) as numeric
    return a + b
end_function

-- Koşul ifadeleri
if x > 10 then
    print("büyük")
else_if x > 5 then
    print("orta")
else
    print("küçük")
end_if

-- Döngüler
for i = 1 to 10
    print(i)
end_for

while x > 0
    x = x - 1
end_while

-- Diziler
numeric[] sayilar = [1; 2; 3; 4; 5]
print(sayilar[0])

-- Yapılar (struct)
struct Nokta
    numeric x
    numeric y
end_struct

-- Sabit listeler (enum) + Örüntü eşleme
enum Renk
    Kirmizi
    Yesil
    Mavi
end_enum

match renk
    case Kirmizi
        print("kırmızı")
    end_case
end_match

-- Modül içe aktarma
import "modullerim/yardimci.mlp"
```

---

## Örnekler

[`examples/`](examples/) klasöründe seviyeye göre düzenlenmiş 40+ program bulunmaktadır:

| Klasör | Konular |
|---|---|
| `base/` | merhaba dünya, değişkenler, aritmetik, string işlemleri, kontrol akışı, fonksiyonlar, struct, enum |
| `cli/` | argüman ayrıştırma, dosya G/Ç, metin işleme |
| `stdlib/` | matematik, string işlemleri, sıralama algoritmaları |
| `games/` | sayı tahmin oyunu, metin macerası |

---

## Çok Dilli Destek

MELP programları birden fazla doğal dilde yazılabilir.  
Normalizer, derleme öncesinde otomatik olarak çeviri yapar:

```
program.mlp      ← İngilizce (temel sözdizimi)
program.tr.mlp   ← Türkçe anahtar kelimeler → otomatik normalleştirilir
program.ru.mlp   ← Rusça anahtar kelimeler → otomatik normalleştirilir
```

---

## Proje Yapısı

```
MELP/
├── examples/          ← örnek programlar (40+)
├── src/
│   ├── compiler/      ← MELP ile yazılmış MELP derleyicisi (devam ediyor)
│   │   └── lexer.mlp  ← 676 satırlık tokenizer
│   ├── normalizer/    ← çok dilli anahtar kelime normalizer'ı
│   └── stdlib/        ← standart kütüphane modülleri
├── editors/
│   └── VSIX/          ← VS Code uzantısı kaynak kodu
└── belgeler/          ← dil belgeleri
```

---

## Yol Haritası

| Kilometre Taşı | Durum |
|---|---|
| Alfa: derleyici + VS Code uzantısı + 40 örnek | ✅ Şimdi |
| Beta: `parser.mlp` + macOS/Windows desteği | 🔧 Devam ediyor |
| v1.0: derleyici tamamen MELP ile yazılmış (öz-derleme) | 🔴 Planlandı |

---

## Platform Desteği

| Platform | Alfa | Beta |
|---|---|---|
| Linux x86_64 | ✅ | ✅ |
| macOS | — | 🔄 |
| Windows | — | 🔄 |

---

## Lisans

[MIT](LICENSE)

# MELP Programlama Dili · Alfa

> Temiz sözdizimine sahip, statik tipli, derlenebilir bir programlama dili.  
> LLVM altyapısı sayesinde gerçek binary dosyaları üretir.

---

## Merhaba Dünya

```melp
function main() as numeric
    print("Merhaba Dünya!")
    return 0
end_function
```

Türkçe anahtar kelimelerle de yazılabilir (`.tr.mlp` dosyaları):

```melp
fonksiyon ana() olarak sayısal
    yaz("Merhaba Dünya!")
    döndür 0
son_fonksiyon
```

---

## Hızlı Başlangıç (VS Code)

1. [Sürümler](../../releases) sayfasından `melp-ide-*.vsix` dosyasını indirin
2. VS Code: `Ctrl+Shift+P` → **Install from VSIX** → dosyayı seçin
3. Herhangi bir `.mlp` dosyası açın → `Ctrl+F5` ile çalıştırın

Derleyici uzantının içine gömülüdür — ayrıca kurulum gerekmez.

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

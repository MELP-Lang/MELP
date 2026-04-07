# MELP — Modül Sistemi

**Durum:**  
- `import modül_adi` ✅ Çalışıyor  
- `export function/struct` ✅ Çalışıyor  
- `module/end module` ⚠️ Parser giriyor ama nitelikli erişim (`Mod.fonksiyon`) çalışmıyor  
- `from modül import sembol` 🔴 Çalışmıyor  
**Versiyon:** Stage 0/1  
**Test Tarihi:** 8 Mart 2026

---

## import / export ✅

MELP'te çoklu dosya projeleri `import` ve `export` ile yönetilir.

### Dosya yapısı

```
proje/
├── utils.mlp    ← kütüphane
└── main.mlp     ← giriş noktası
```

### utils.mlp

```mlp
export numeric function topla(numeric a; numeric b)
    return a + b
end function

export numeric function carp(numeric a; numeric b)
    return a * b
end function
```

### main.mlp

```mlp
import utils

function main()
    numeric r1 = topla(5; 3)
    numeric r2 = carp(4; 7)
    println("Toplam: " + str(r1))   -- 8
    println("Çarpım: " + str(r2))   -- 28
end function
```

### Derleme

```bash
melp_compiler main.mlp -o main.ll
clang -O0 main.ll -lm -o program
./program
```

> Derleyici `import utils` gördüğünde aynı dizinde `utils.mlp` dosyasını arar ve otomatik dahil eder.

---

## Kritik Kurallar

| Kural | Doğru | Yanlış |
|-------|-------|--------|
| Parametre ayracı | `function f(numeric a; numeric b)` | `function f(numeric a, numeric b)` |
| Dosya uzantısı import'ta yazılmaz | `import utils` | `import utils.mlp` |
| `export` zorunlu | Dışarıdan görünmesi istenen her şeye | Sadece struct'larda değil, fonksiyonlarda da |
| Import yolları | `import utils` (aynı dizin) | `import /mutlak/yol` ❌ |

> ⚠️ **Önemli:** Fonksiyon parametreleri `,` değil `;` ile ayrılır. Bu genel MELP kuralıdır — import/export bağımsız.

---

## Birden Fazla Import

```mlp
import string_utils
import math_utils
import veri_yapilari

function main()
    numeric toplam = math_utils_topla(10; 20)
    -- ...
end function
```

> Her modül yalnızca bir kez yüklenir (deduplication otomatik).

---

## Struct Export ✅

```mlp
-- geometry.mlp
export struct Nokta
    numeric x
    numeric y
end struct

export numeric function uzaklik(Nokta a; Nokta b)
    numeric dx = a.x - b.x
    numeric dy = a.y - b.y
    return dx * dx + dy * dy
end function
```

```mlp
-- main.mlp
import geometry

function main()
    Nokta p1
    p1.x = 0
    p1.y = 0

    Nokta p2
    p2.x = 3
    p2.y = 4

    numeric d = uzaklik(p1; p2)
    println("Uzaklık karesi: " + str(d))   -- 25
end function
```

---

## module / end module ⚠️

`module` keyword'ü ile aynı dosyada namespace tanımlanabilir. Parser girer ama **nitelikli erişim (`Mod.fonksiyon`) şu an çalışmıyor** — fonksiyonlara niteliksiz (doğrudan isimle) erişin.

```mlp
module MatematikYardimci
    numeric function topla(numeric a; numeric b)
        return a + b
    end function
end module

function main()
    numeric r = topla(3; 4)       -- ✅ niteliksiz erişim çalışıyor
    -- numeric r = MatematikYardimci.topla(3; 4)   -- ❌ çalışmıyor
    println("Sonuç: " + str(r))
end function
```

---

## from ... import 🔴

> **Çalışmıyor.** `from utils import topla` sözdizimi parser'da henüz tanımlı değil.

```mlp
-- ❌ Bu henüz çalışmıyor (EKS-012):
from utils import topla
from math import carp, bol

-- ✅ Bunun yerine kullanın:
import utils
```

---

## Durum Tablosu

| Özellik | Sözdizimi | Durum |
|---------|-----------|-------|
| Temel import | `import modül` | ✅ |
| Fonksiyon export | `export function f()` | ✅ |
| Struct export | `export struct S` | ✅ |
| Seçici import | `from m import f` | 🔴 |
| Nitelikli erişim | `Mod.fonksiyon()` | 🔴 (`module` bloğu için) |
| Göreli path import | `import ./alt/yol` | ⚠️ Kısmi |
| Module alias | `import mat as m` | 🔴 |
| Döngüsel bağımlılık | — | ✅ Otomatik algılama |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `export function f() as numeric` | `export numeric function f()` |
| `end_function` | `end function` |
| `import utils.mlp` | `import utils` |
| `f(a, b)` (virgül) | `f(a; b)` |

---

## SSS

**S: Import'ta dosya uzantısı yazılır mı?**  
C: Hayır. `import utils` yazın, `import utils.mlp` değil.

**S: Parametre ayracı neden virgül değil noktalı virgül?**  
C: MELP'te virgül (`,`) ondalık sayı ayracıdır. Fonksiyon parametreleri `;` ile ayrılır — bu genel bir kural, import'a özgü değil.

**S: `export` olmayan fonksiyonlara dışarıdan erişilebilir mi?**  
C: Hayır. `export` keyword'ü olmadan tanımlanan fonksiyonlar dışarıdan görülmez.

**S: Bir modülü birden fazla dosyaya import edebilir miyim?**  
C: Evet. Derleyici her modülü yalnızca bir kez işler.

**S: `module/end module` ne zaman düzelecek?**  
C: Nitelikli erişim (`Mod.f()`) özelliği roadmap'te. Şu an `module` bloğu namespace oluşturmuyor — fonksiyonlar global düzeyde tanımlanıyor.

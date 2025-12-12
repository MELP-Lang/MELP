# MELP: Programlamanın Eritme Potası

> *"Programlama dili kullanıcıya engel değil, yardımcı olmalı."*

---

## 🎯 MELP Nedir?

**MELP** (Multi-Language Programming) — birden fazla dilin ve sözdiziminin tek bir potada eritildiği modern bir programlama dilidir.

```
İsmin Kökeni:
MLP = Multi Language Programming
MELP = Melting Pot (Eritme Potası) 🫕
```

---

## 💡 Temel Felsefe

### 1. Basitlik Öncelikli

```mlp
-- Kullanıcı sadece 3 temel tip bilir:
numeric x = 42        -- Tüm sayılar
string s = "hello"    -- Tüm metinler
boolean b = true      -- Doğru/Yanlış

-- Gerisini compiler halleder (STO)
```

**Neden?** `int8`, `int16`, `int32`, `float`, `double`, `BigDecimal` arasında seçim yapmak **kullanıcının işi değil**.

### 2. Okunabilirlik

```mlp
if age > 18 then
    println("Yetişkin")
end if

for each item in myList
    println(item)
end for
```

Süslü parantezler `{}` yerine **İngilizce kelimeler**. Kod = Düzyazı.

### 3. Stateless by Default

```mlp
-- Fonksiyonlar pure (saf)
function add(numeric a, numeric b) returns numeric
    return a + b  -- Hiçbir yan etki yok
end function

-- State gerekirse: opt-in
import "state"
state_set("counter", value)
```

**Neden?** Predictability, testability, parallelism.

---

## 🔮 Vizyon: Çok Dilli Programlama

MELP'in uzun vadeli hedefi:

```
Kullanıcı Kodu (Türkçe + Python-style)     Kullanıcı Kodu (English + C-style)
         ↓                                           ↓
    [diller.json]                               [syntax.json]
         ↓                                           ↓
         └──────────────→ PMPL (Pragmatik MLP) ←────────────┘
                                ↓
                           Compiler
                                ↓
                         Native Binary
```

**Hayal edin:**
```
-- Türkçe yazan geliştirici:
eğer yaş > 18 ise
    yazdır("Yetişkin")
son eğer

-- Aynı projedeki İngilizce yazan geliştirici:
if age > 18 then
    print("Adult")
end if

-- İkisi de aynı binary'ye derlenir!
```

---

## ⚡ Modern Tarafları

| Özellik | MELP | Diğer Diller |
|---------|------|--------------|
| **Tip sistemi** | STO (otomatik optimizasyon) | Manuel seçim |
| **Syntax** | Okunabilir (end if, end for) | Sembolik ({, }) |
| **State** | Default stateless | Default stateful |
| **Modülerlik** | 26 bağımsız modül | Monolitik compiler |
| **Self-hosting** | MLP'de MLP yazılıyor | Genelde C/C++ |
| **Error messages** | Renkli, önerili, typo detection | Kriptik mesajlar |

---

## 🛠️ Teknik Yenilikler

### STO (Smart Type Optimization)

```
Kullanıcı yazar    →    Compiler seçer
─────────────────────────────────────
numeric x = 42     →    int64 (register)
numeric y = 3.14   →    double (XMM)
numeric z = 10^100 →    BigDecimal (heap)
string s = "Ali"   →    SSO (stack, 23 byte)
string t = "uzun"  →    heap pointer
```

Kullanıcı **düşünmez**, performans **düşmez**.

### Modüler Compiler Mimarisi

```
compiler/stage0/modules/
├── arithmetic/     ← Bağımsız
├── comparison/     ← Bağımsız
├── functions/      ← Bağımsız
├── for_loop/       ← Bağımsız
└── ...             ← Her biri standalone test edilebilir
```

**Avantaj:** AI agent'lar küçük parçaları yönetebilir.

---

## 🚀 Gelecek Yol Haritası

```
2025 Q4: Stage 0 Complete ✅
         └── C'de yazılmış compiler çalışıyor

2026 Q1: Self-hosting
         └── MELP'te MELP compiler

2026 Q2: Çok Dilli Destek
         └── diller.json + syntax.json aktif

2026 Q3: LLVM Backend (opsiyonel)
         └── WebAssembly, ARM desteği

2026+:   MELP Ecosystem
         └── Package manager, IDE plugins, community
```

---

## 🎨 MELP'i Farklı Kılan Ne?

1. **Eritme Potası** — Tek dil, çok sözdizimi, çok kültür
2. **STO** — Basitlik + Performans birlikte
3. **Stateless** — Modern, functional yaklaşım
4. **Self-hosting** — Dilin kendi kendini derlemesi
5. **AI-Friendly** — Modüler mimari, AI ile geliştirme

---

## 📝 Sonuç

MELP, programlama dünyasında bir **köprü** olmayı hedefliyor:

- Başlangıç seviyesi ↔ İleri seviye
- Türkçe ↔ İngilizce ↔ Diğer diller
- Basitlik ↔ Performans
- Okunabilirlik ↔ Güç

> *"Herkes kendi dilinde, kendi tarzında kod yazabilmeli."*

---

**Proje:** [github.com/MELP-Lang/MELP](https://github.com/MELP-Lang/MELP)  
**Durum:** Stage 0 Complete, Self-hosting in Progress  
**Son Güncelleme:** 12 Aralık 2025

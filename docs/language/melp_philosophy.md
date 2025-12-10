# MELP Dil Felsefesi

**Versiyon:** 1.0  
**Son Güncelleme:** 10 Aralık 2025  
**Dil:** Türkçe

---

## İçindekiler

1. [MELP Nedir?](#melp-nedir)
2. [Tasarım İlkeleri](#tasarım-ilkeleri)
3. [Sözdizimi Felsefesi](#sözdizimi-felsefesi)
4. [Tip Sistemi Felsefesi](#tip-sistemi-felsefesi)
5. [Çok Dillilik](#çok-dillilik)
6. [Mimari Kararlar](#mimari-kararlar)
7. [Diğer Dillerle Karşılaştırma](#diğer-dillerle-karşılaştırma)

---

## MELP Nedir?

### İsmin Anlamı

**MLP** = **M**ulti **L**anguage **P**rogramming

**MELP** = **M**ulti **L**anguage **P**rogramming'in kullanışlı kısaltması

"Eritme Potası" (Melting Pot) anlamına da gelen MELP, birçok programlama dilinin ve sözdiziminin tek bir potada eritilmesini temsil eder.

### Temel Vizyon

MELP, programcıların:
- **İstedikleri dilde** (Türkçe, İngilizce, Rusça, Arapça...)
- **İstedikleri sözdiziminde** (C-style, Python-style, Pascal-style...)
- Kod yazabilmelerini sağlar.

Derleyici her zaman aynı ara dili (Pragmatik MLP) görür.

---

## Tasarım İlkeleri

### 1. Basitlik Öncelikli

```
"Kullanıcı sadece 2 temel tip bilmeli: numeric ve string"
```

Kullanıcı `int`, `float`, `double`, `BigDecimal` arasında seçim yapmak zorunda kalmamalı. MELP bunu arka planda otomatik optimize eder (TTO - Transparent Type Optimization).

### 2. Okunabilirlik

MELP, İngilizce (veya seçilen dil) gibi okunabilir olmalı:

```mlp
if age > 18 then
    print("Yetişkin")
end if
```

Süslü parantezler `{}` yerine kelime tabanlı blok sonlandırıcılar (`end if`, `end while`, `end function`).

### 3. Güvenlik

- Otomatik overflow handling
- Null safety operatörleri (`??`, `?.`)
- Exception handling (`try`/`catch`/`finally`)

### 4. Çok Dillilik

```
Kullanıcı Kodu (Herhangi Dil + Herhangi Sözdizimi)
       ↓
[diller.json + syntax.json]
       ↓
Pragmatik MLP (İngilizce Base)
       ↓
Derleyici
```

Kullanıcı Türkçe `yazdır("Merhaba")` yazabilir, derleyici `print("Hello")` olarak işler.

---

## Sözdizimi Felsefesi

### VB.NET + Pascal Hybrid

MELP'in sözdizimi iki ana kaynaktan ilham alır:

| Özellik | Kaynak | MELP Örneği |
|---------|--------|-------------|
| Blok sonlandırıcılar | VB.NET/Pascal | `end if`, `end while` |
| Döngü çıkışı | VB.NET | `exit for`, `exit while` |
| Fonksiyon tanımı | Pascal | `function name()` |
| Koşul ifadesi | VB.NET | `if ... then ... end if` |

### Neden Bu Seçimler?

#### 1. `end X` vs `}`

**C-style:**
```c
if (x > 5) {
    // code
}
```

**MELP:**
```mlp
if x > 5 then
    -- code
end if
```

**Gerekçe:**
- Daha okunabilir
- Hangi bloğun kapandığı açık
- Yeni başlayanlar için daha anlaşılır

#### 2. `exit X` vs `break`/`continue`

**C-style:**
```c
for (i = 0; i < 10; i++) {
    if (condition) break;
}
```

**MELP:**
```mlp
for i = 0 to 10
    if condition then
        exit for
    end if
end for
```

**Gerekçe:**
- Nereden çıkıldığı açık (`exit for` vs sadece `break`)
- VB.NET tutarlılığı
- İç içe döngülerde hangi döngüden çıkıldığı belli

#### 3. `==` vs `=` (Karşılaştırma)

**Tartışma:** VB.NET context-aware `=` kullanır, C-style `==` kullanır.

**MELP Kararı:** `==` kullanır (C-style)

**Gerekçe:**
- Atama (`=`) ve karşılaştırma (`==`) karışmaz
- Daha az hata yapma potansiyeli
- Çoğu modern dilin standardı

> **Not:** Bu, "saf VB.NET hybrid" iddiasından bir sapma, ama pratik gerekçeler bunu destekler.

---

## Tip Sistemi Felsefesi

### Kullanıcı Perspektifi

Kullanıcı sadece şu tipleri bilir:
- `numeric` - Tüm sayılar
- `string` - Tüm metinler
- `boolean` - true/false

### Derleyici Perspektifi (TTO)

Derleyici arka planda optimize eder:

| Kullanıcı Yazar | Derleyici Kullanır |
|-----------------|-------------------|
| `numeric x = 42` | `int64` (register) |
| `numeric y = 3.14` | `double` (XMM) |
| `numeric z = 10^100` | `BigDecimal` (heap) |
| `string s = "Ali"` | SSO (stack inline) |
| `string t = "Uzun metin..."` | Heap pointer |

### Neden Tek Tip?

1. **Basitlik:** `int` vs `long` vs `float` kararı yok
2. **Güvenlik:** Overflow otomatik yönetilir
3. **Tutarlılık:** `1 / 2` her zaman beklenen sonucu verir

---

## Çok Dillilik

### Mimari

```
┌─────────────────────────────────────────────┐
│  Kullanıcı Kodu                             │
│  (Türkçe + Python-style)                    │
│                                             │
│  eğer yaş > 18:                             │
│      yazdır("Yetişkin")                     │
└─────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────┐
│  syntax.json (Python → MLP)                 │
│  diller.json (Türkçe → İngilizce)           │
└─────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────┐
│  Pragmatik MLP (PMLP)                       │
│                                             │
│  if age > 18 then                           │
│      print("Yetişkin")                      │
│  end_if                                     │
└─────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────┐
│  Derleyici (Lexer → Parser → Codegen)       │
│  Sadece PMLP görür                          │
└─────────────────────────────────────────────┘
                    ↓
┌─────────────────────────────────────────────┐
│  x86-64 Assembly                            │
└─────────────────────────────────────────────┘
```

### PMLP (Pragmatik MLP)

PMLP, derleyicinin gördüğü ara dildir:
- İngilizce keyword'ler
- Birleştirilmiş tokenler (`end if` → `end_if`)
- Standart sözdizimi

**Not:** Kullanıcı PMLP'yi görmez.

---

## Mimari Kararlar

### Modüler Yapı

MELP monolitik bir `compiler.c` yerine modüler yapı kullanır:

```
modules/
├── lexer/        -- Token üretimi
├── parser/       -- AST oluşturma
├── codegen/      -- Assembly üretimi
├── variable/     -- Değişken yönetimi
├── control_flow/ -- if/while/for
├── functions/    -- Fonksiyon tanımları
├── array/        -- Array/List/Tuple
└── ...
```

**Gerekçe:**
- Her modül bağımsız test edilebilir
- Bakım kolaylığı
- AI agent'lar için anlaşılabilirlik

### Self-Hosting Hedefi

MELP'in nihai hedefi:
- Derleyici tamamen MELP'te yazılacak
- Runtime C'de kalacak (performans için)

Bu "bootstrap" sürecidir.

---

## Diğer Dillerle Karşılaştırma

### MELP vs Python

| Özellik | Python | MELP |
|---------|--------|------|
| Tip bildirimi | Dinamik | Statik (`numeric x`) |
| Blok yapısı | Girinti | `end X` |
| Performans | Yorumlanan | Derlenen (native) |
| Çoklu sözdizimi | Hayır | Evet |

### MELP vs C

| Özellik | C | MELP |
|---------|---|------|
| Tip sayısı | Çok (int, float, char...) | Az (numeric, string) |
| Bellek yönetimi | Manuel | Otomatik (TTO) |
| Okunabilirlik | Düşük (`{` `}`) | Yüksek (`end if`) |
| Güvenlik | Düşük | Yüksek (overflow handling) |

### MELP vs VB.NET

| Özellik | VB.NET | MELP |
|---------|--------|------|
| Platform | .NET | Native x86-64 |
| `=` operatörü | Context-aware | Explicit (`==`) |
| Blok yapısı | `End If` | `end if` |
| Çoklu dil | Hayır | Evet |

---

## Sonuç

MELP şu felsefeyi benimser:

> "Programlama dili kullanıcıya engel değil, yardımcı olmalı."

Bu nedenle:
- Tip karmaşıklığı gizlenir (TTO)
- Sözdizimi okunabilirdir (`end if`)
- Çoklu dil/sözdizimi desteklenir
- Güvenlik önceliklidir (null safety, exception handling)

---

## Sözlük

| Terim | Açıklama |
|-------|----------|
| **MLP** | Multi Language Programming |
| **MELP** | MLP'nin kullanışlı kısaltması |
| **PMLP** | Pragmatik MLP - Derleyicinin gördüğü ara dil |
| **TTO** | Transparent Type Optimization |
| **SSO** | Small String Optimization |
| **Bootstrap** | Derleyiciyi kendi dilinde yazma |

---

*Bu belge MELP'in tasarım felsefesini açıklar. Teknik detaylar için diğer dökümanlara bakın.*

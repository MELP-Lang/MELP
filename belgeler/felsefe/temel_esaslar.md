# MELP 9 Temel Esas — Hızlı Referans

**Tam kaynak:** [`belgeler/MELP_PHILOSOPHY.md`](../../belgeler/MELP_PHILOSOPHY.md)

Bu dosya bir özet ve hızlı erişim belgesidir. Her esas için tam açıklama ve örnekler birincil kaynaktadır.

---

## 9 Temel Esas (ASLA İHLAL ETME!)

### 1️⃣ MODÜLER — Merkezi Orchestrator YASAK

- Her modül tek bir sorumluluğa sahiptir
- Modüller peer-to-peer import eder (ihtiyaç duyduğunu alır)
- Merkezi `main.mlp` veya `main.cpp` tüm modülleri yönetemez
- Derleme sırası otomatik dependency graph'tan çözülür

```
✅ codegen.mlp → import parser, token_types
❌ main.mlp    → import EVERYTHING  ← YASAK
```

### 2️⃣ GCC/LLVM — Standart Backend

```
Stage 1: .mlp → Normalizer → PMLP → Lexer → Parser → OK → LLVM IR → Binary
Stage 2: .mlp → LLVM IR → LLVM → Binary  (self-hosting başlangıcı)
Stage 3: Self-hosting (tam)
```

Özel backend çözümleri veya handwritten assembler yasaklanmıştır.

### 3️⃣ STO — Kullanıcı Üç Tip Görür

Kullanıcı sadece üç tip görür: `numeric`, `string`, `boolean`. Derleyici arka planda optimize eder.

```melp
numeric küçük   = 42        -- STO: i64
numeric büyük   = 10**100   -- STO: BigDecimal
numeric ondalık = 3.14      -- STO: f64
string kısa     = "Ali"     -- STO: SSO (stack)
string uzun     = oku("kitap.txt")  -- STO: heap ptr
```

`int`, `float`, `double`, `int64` kullanıcı kodunda YASAK.

### 4️⃣ STATELESS — Durumsuz Fonksiyonlar Önce

- Fonksiyonlar mümkün olduğunca saf (pure) yazılır
- Global state minimuma indirilir
- State tutmak YASAK değil, ama önce stateless alternatif düşünülür
- Ownership sorunu statelessness'le kendiliğinden çözülür

### 5️⃣ STRUCT + FONKSİYON — Class/OOP Yok

MELP'te `class` yoktur. Yapı + fonksiyon kombinasyonu kullanılır:

```melp
struct Nokta
    numeric x
    numeric y
end struct

numeric function uzaklik(Nokta a; Nokta b)
    return sqrt((a.x-b.x)**2 + (a.y-b.y)**2)
end function
```

Yöntemler (method) struct dışında tanımlanan fonksiyonlardır. OOP hiyerarşisi yoktur.

### 6️⃣ MODÜL = ŞABLON — Parametre Alabilen Modüller

- Modüller tip parametresi alabilir (`module Stack<T>`)
- Generics modül sistemine entegre
- Monomorphization: her somut tip için ayrı kod üretilir
- Her modül hem AST kalıbı hem derleyici davranışı tanımlar

### 7️⃣ SAO — Semantik Farkındalıklı Optimizasyon

- SAO fonksiyon saflığını ve sahiplik geçişlerini takip eder
- "İki Fazlı Hafıza Yitimi" problemini çözer (SAO bridge)
- LLVM metadata'ya gömülür: 4-byte encoding
- Detay: [`belgeler/mimari/ok_katmani.md`](../mimari/ok_katmani.md)

### 8️⃣ ÖLÜ MODÜL FELSEFESİ — Her Modül Varsayılan Olarak Ölüdür

- Her modül varsayılan olarak **ölüdür** — kaynak tüketmez, çalışmaz
- Kullanan modül onu **diriltir** (instantiate) ve işi bitince **öldürür** (dispose)
- Modüller API gibi (sürekli ayakta) değil, **şablon** gibi davranır
- Çağıran modül tarafından oluşturulur ve yok edilir — GC'ye gerek kalmaz
- Sonuç: hiçbir modül atıl kaynak tutmaz; yaşam döngüsü çağıran tarafından kontrol edilir

### 9️⃣ FBO — Frontend Backend Optimization

4 byte × her AST düğümü = 4,3 milyar olası durum:

```
Byte 0: STO metadata  (256 tip kararı)
Byte 1: SAO metadata  (256 saflık/sahiplik durumu)
Byte 2: Safety level  (SafetyLevel enum'u)
Byte 3: Effect flags  (IO, mut, async, exception)
```

- 16 düğüm/cache satırı (Rust'ın 2-4'üne karşı)
- %5-15 derleme hızı artışı
- Fase 5.1: Fonksiyon füzyonu (döngü birleştirme)
- Faz 5.2: Ölü-yol eleme
- Faz 6: SafetyLevel tabanlı optimizasyon

Detay: [`belgeler/mimari/ok_katmani.md`](../mimari/ok_katmani.md)

---

## Tasarım Kararları Özeti

| Karar | Gerekçe |
|---|---|
| `numeric` tek sayı tipi | Kullanıcı optimizasyon kararı vermez, STO verir |
| `;` parametre ayırıcı | Virgül Türkçe'de ondalık `3,14` — çakışma önlenir |
| `end function` (boşluklu) | `end_function` PMLP'dir; kullanıcı görüntüsü daha okunabilir |
| Merkezi orchestrator yasak | Tek-sorumluluk ilkesi + ölçeklenebilirlik |
| Class yok, struct + func | OOP karmaşıklığı olmadan aynı güç |
| Ownership yok (Stage 2'ye kadar) | Statelessness büyük çoğunluğu çözer |

---

## Kontrol Soruları

Herhangi bir tasarım kararında şu soruları sor:

1. ❓ Kullanıcı dahili tipi görüyor mu? → **HAYIR** olmalı (STO ilkesi)
2. ❓ Merkezi orchestrator oluşuyor mu? → **HAYIR** olmalı (Modüler ilkesi)
3. ❓ Class/OOP hiyerarşisi var mı? → **HAYIR** olmalı (Struct+Func ilkesi)
4. ❓ Fonksiyon saf yazılabilir mi? → **EVET** önce dene (Stateless ilkesi)
5. ❓ Yeni bir backend mi? → **HAYIR**, GCC/LLVM kullan

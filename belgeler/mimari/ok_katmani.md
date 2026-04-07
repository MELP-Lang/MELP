# OK Katmanı — Optimizasyon Katmanı

**Tek kaynak belgesi.**
Aşağıdaki 11 orijinal belgeden konsolide edilmiştir:
`OK_LAYER_ARCHITECTURE.md` · `STO_PRINCIPLES.md` · `SAO_PRINCIPLES.md` ·
`STO_PERFORMANCE.md` · `FBO_COMPACT_ENCODING.md` · `STO_SAO_IMPLEMENTATION_ROADMAP.md` ·
`language/STO.md` · `melp_felsefesi/FBO_FRONTEND_BACKEND_OPTIMIZATION.md` ·
`melp_felsefesi/FBO_FRONTEND_BACKEND_OPTIMIZATION.md` ·
`melp_felsefesi/OK_AILESI_COK_POK_ROK.md` · `melp_felsefesi/OK_MIDDLEWARE_PATTERN.md`

---

## İçindekiler

1. [Genel Bakış](#1-genel-bakış)
2. [Pipeline: STO → SAO → FBO](#2-pipeline-sto--sao--fbo)
3. [STO — Smart Type Optimization](#3-sto--smart-type-optimization)
4. [SAO — Semantic-Aware Optimization](#4-sao--semantic-aware-optimization)
5. [FBO — Frontend Backend Optimization](#5-fbo--frontend-backend-optimization)
6. [OK Ailesi: COK / POK / ROK / EOK](#6-ok-ailesi-cok--pok--rok--eok)
7. [Middleware Deseni](#7-middleware-deseni)
8. [Performans Verileri](#8-performans-verileri)
9. [Uygulama Durumu](#9-uygulama-durumu)

---

## 1. Genel Bakış

OK (*Optimizasyon Katmanı*), MELP derleyicisinin AST dönüşüm ve güvenlik
analiz altyapısıdır. Üç alt sistemden oluşur:

| Katman | Açık adı | Sorumluluk |
|---|---|---|
| **STO** | Smart Type Optimization | `numeric` → i64/f64/BigDecimal; type inference |
| **SAO** | Semantic-Aware Optimization | Frontend semantic kanıtlarını backend'e taşır |
| **FBO** | Frontend Backend Optimization | 4-byte metadata; dead-path eleme; SafetyLevel |

Üç katman birlikte çalışır: tüm faz çıktıları tek bir **FBO Tablosu** üzerinden
paylaşılır; ara veri yapısı kopyalanmaz.

```
AST Girişi
    │
    ▼
┌──────────┐  güven kümesi
│   STO    │──────────────────────────────────┐
└──────────┘                                  │
    │                                         │
    ▼                                         │
┌──────────┐  tip/scope doğrulama             │
│   SAO    │──────────────────────────────────┤
└──────────┘                                  │
    │                                         ▼
    ▼                               ┌─────────────────┐
┌──────────┐                        │   FBO Tablosu   │
│   FBO    │◄───────────────────────│  (global index) │
│ package()│                        └─────────────────┘
│ validate()│
│ analyzeFusion()
│ eliminateDeadPaths()
│ applySafety()
└──────────┘
    │
    ▼
  LLVM IR Üretici
```

`OptimizationLayer::process()` çağrı sırası (`ok_layer.cpp`):

```cpp
FBO::package(ast);              // İndeks protokolü: metadata.combined yaz
FBO::validate(ast);             // STO güven + SAO tip doğrulama
FBO::analyzeFusion(ast);        // Pipe chain füzyon
FBO::eliminateDeadPaths(ast);   // Ölü semantik yol silme
FBO::applySafety(ast, config.safetyLevel); // Güvenlik katmanı
```

---

## 2. Pipeline: STO → SAO → FBO

Programcının gördüğü:

```melp
numeric x = 42
numeric y = x + 1
```

Derleyicinin OK katmanında gördüğü (kavramsal):

```
STO:  x → {VALUE, trust=HIGH, internal=i64}
      y → {VALUE, trust=HIGH, internal=i64}
SAO:  x uyumlu, kapsam geçerli
      y = x + 1 → tip uyumu onaylı, taşma risk=NONE
FBO:  füzyon mümkün değil (tek işlem)
      ölü yol yok
      SafetyLevel=BASIC → bounds check skip (statik değer)
IR:   %y = add i64 %x, 1  ; metadata attached
```

---

## 3. STO — Smart Type Optimization

### Kullanıcı Perspektifi

Kullanıcı yalnızca üç tip bilir:

| Kullanıcı tipi | Örnek değerler | STO dahili temsil |
|---|---|---|
| `numeric` / `sayısal` | `42`, `3.14`, `10**23` | `i64`, `f64`, `BigDecimal` |
| `string` / `metin` | `"merhaba"`, uzun metin | SSO (≤23 byte inline), heap ptr |
| `boolean` / `mantıksal` | `doğru`, `yanlış` | `i1` |

Kullanıcı hiçbir zaman `int64`, `double`, `BigDecimal` yazmaz. STO bu kararı
statik olarak derleme zamanında verir.

### Çalışma Mekanizması

STO her AST düğümüne bir "güven kümesi" (trust cluster) atar. Güven kümesi şu
bilgileri içerir:

- **Değer kategorisi:** VALUE / STRUCTURE / EFFECT
- **Dahili tip:** `i64`, `f64`, `BigDecimal`, `SSO`, `heap_ptr`
- **Güven seviyesi:** HIGH (statik kanıtlandı) / MED / LOW
- **Taşma riski:** NONE / POSSIBLE / CERTAIN → FBO'ya iletilir

```
numeric a = 5        → trust=HIGH, internal=i64,  overflow_risk=NONE
numeric b = input()  → trust=LOW,  internal=?,    overflow_risk=POSSIBLE
numeric c = a + b    → trust=MED,  FBO→ROK embed
```

### Taşma Otomatik Promosyonu

`i64` taşarsa STO otomatik olarak `BigDecimal`'a yükseltir; kullanıcı müdahale
etmez, program doğru sonuç verir:

```melp
numeric n = 10 ** 23   -- STO: i64 yetmez → BigDecimal seçilir
numeric m = 42         -- STO: i64 yeterli
```

---

## 4. SAO — Semantic-Aware Optimization

### Felsefe: İki Aşamalı Amnezi Problemi

Geleneksel derleyiciler kodunuzu **iki kez** analiz eder ama iki aşama birbirine
konuşmaz:

```
1. Frontend: "Bu kod güvenli mi?" → Type checking, ownership analizi → ✅
2. Backend:  "Bu kodu nasıl hızlandırayım?" → Kör optimizasyon → muhafazakâr

PROBLEM: Frontend'in öğrendikleri backend'e ulaşmıyor.
```

SAO bu köprüyü kurar:

```
Frontend kanıtları → SAO metadata → LLVM optimization hints
     ↓                                        ↑
     └──────────── SAO Bridge ────────────────┘
Sonuç: Güvenli + agresif optimizasyon = C performansı + Rust güvenliği
```

### SAO'nun Doğruladıkları

- Tip uyumu: `numeric + numeric` geçerli, `numeric + string` hata
- Kapsam sınırları: değişken kapsamı dışında erişim → derleme hatası  
- Purity: yan etkisiz fonksiyonlar → LLVM `readonly` / `readnone` ekler
- No-alias kanıtı: iki pointer aynı belleğe işaret etmiyor → vectorization açılır

```llvm
; SAO kanıtları LLVM metadata olarak:
!sao.sto   = !{i1 true, !"type_proven", !"int64"}
!sao.borrow = !{i1 true, !"noalias_proven"}
; Backend bu metadata'ya güvenerek vektorizasyon yapar
```

---

## 5. FBO — Frontend Backend Optimization

### 4-Byte Metadata Paketi

Her AST düğümü, 4 byte'lık bir FBO metadata paketiyle işaretlenir:

```
Byte 0: STO  — Tip kanıtı (i64, f64, BigDecimal, SSO, heap)
Byte 1: SAO  — Ownership/purity kanıtı (noalias, borrowed, pure)
Byte 2: Safety — Güvenlik kontrol durumu (overflow, bounds, null)
Byte 3: Effect — Yan etki analizi (pure, reads_only, writes, spawns)

4 byte × 256 kombinasyon = 4.3 milyar olası durum
```

**Neden 4 byte?**

| | Rust | Java/JVM | MELP/FBO |
|---|---|---|---|
| Node başına metadata | ~32 byte | ~64 byte | **4 byte** |
| Cache line'a sığan node | 2 | 1 | **16** |
| L1 cache etkinliği | orta | düşük | **çok yüksek** |

### FBO'nun Yaptıkları

**Phase 5.1 — Pipe Chain Füzyon:**
```melp
-- Kullanıcı kodu (3 ayrı map operasyonu):
result = liste.map(f1).map(f2).map(f3)

-- FBO füzyonu sonrası (1 geçiş):
result = liste.map(x -> f3(f2(f1(x))))   -- Geçici kopyalar yok
```

**Phase 5.2 — Ölü Semantik Yol Eliminasyonu:**
```melp
numeric mode = 1  -- sabit değer

if mode == 1 then
    -- Bu dal her zaman çalışır; FBO tespit eder
end if
if mode == 2 then
    -- Bu dal hiç çalışmaz; FBO kaldırır
end if
```

**Phase 6 — Güvenlik Katmanı (SafetyLevel):**

```
NONE   → Hiçbir güvenlik kontrolü eklenmez (C hızı)
BASIC  → Yalnızca untrusted düğümler kontrol edilir (varsayılan)
STRICT → Tüm düğümler kontrol edilir, trusted dahil (Rust seviyesi)
```

### FBO Tablosu Yapısı

```cpp
struct FBOTableEntry {
    // STO alanları (Değer Tipi)
    uint32_t  value_type_id;   // Katalog ID (101=INT8 vb.)
    std::string llvm_type;     // LLVM IR tip string ("i64", "f64"…)
    int       alignment;       // Byte hizalaması
    bool      is_signed;

    // SAO alanları (Yapı + Semantik)
    uint32_t  structure_id;    // Yapı türü (1010=ARRAY vb.)
    uint32_t  ownership_id;    // Sahiplik (2001=OWNED vb.)
    uint32_t  effect_id;       // Yan etki (3001=PURE vb.)
    uint32_t  lifetime_id;     // Yaşam süresi (4002=LOCAL vb.)
    uint32_t  trust_id;        // Güven seviyesi (5001=TRUSTED vb.)

    // FBO alanları (LLVM Attribute + Kontrol)
    bool  attr_readonly;
    bool  attr_noalias;
    bool  attr_willreturn;
    bool  attr_alwaysinline;
    bool  needs_overflow_check;
    bool  needs_bounds_check;
    bool  needs_null_check;

    // Optimizasyon ipuçları
    bool  fusion_candidate;    // Pipeline Fusion Phase 5
    bool  dead_path;           // Dead Path Elimination Phase 5
    std::string alloc_strategy; // "register" / "stack" / "heap" / "rodata"
};

// İki bölgeli tablo:
// Statik bölge: 52+ önceden hesaplanmış giriş (fbo_catalog.h)
// Dinamik bölge: çalışma zamanında oluşturulan giriş
```

---

## 6. OK Ailesi: COK / POK / ROK / EOK

Aynı STO→SAO→FBO soyutlaması dört farklı etki alanında uygulanır:

```
COK → Compiler OK      (derleme zamanı, AST üzerinde)       ✅ Mevcut
POK → Parser-Codegen OK (derleme zamanı, AST bypass, flat)  📋 Stage 2
ROK → Runtime OK        (çalışma zamanı, heap gömülü tablo) ✅ POC kanıtlandı
EOK → Encryptic OK      (binary koruma)                     📋 Gelecek
```

### COK (Compiler OK)

Bugünkü uygulama. `ok_layer.cpp` (1166 satır) ile `fbo_catalog.h` (52+ statik giriş).

**Bilinen eksik:** Codegen FBO tablosunu henüz okumuyor. FBO hesaplanıyor ama
IR üreticiye iletilmiyor. → `TODO_BEFORE_SELFHOSTING.md` B-1 maddesi.

### POK (Parser-Codegen OK)

AST ağaç yapısını atlayan **flat tablo** yaklaşımı (Zig ZIR'a benzer):

```
COK pipeline: Token → AST → STO→SAO→FBO → IR    (zengin analiz, yavaş)
POK pipeline: Token → Flat Tablo → STO→SAO→FBO → IR  (hızlı, az analiz)
```

Neden gerekli:
- AST her node için heap allocation → flat tablo tek allocation
- Ardışık bellek → L1 cache hit oranı çok yüksek
- Zig kanıtı: C++'dan 3–5× hızlı derleme

### ROK (Runtime OK)

FBO tablosu derlenmiş binary içine gömülür; program normal memory read ile okur.
CPU cache hiyerarşisine göre katmanlanır:

| ROK Katmanı | Hedef cache | Max boyut | İçerik |
|---|---|---|---|
| ROK-HOT | L1 (32 KB) | ≤8 KB | En sık 256 strateji kararı |
| ROK-WARM | L2 (256 KB) | ≤64 KB | Tüm STO tag tablosu |
| ROK-COLD | L3 (12 MB) | ≤2 MB | SAO enrichment, nadir kararlar |

**POC Sonuçları:**

*POC 1 — Tip-Strateji Dispatch:*
```
i64 + i64   → NATIVE_ADD    → doğru sonuç
f64 + f64   → FLOAT_ADD     → doğru sonuç
i64 + f64   → MIXED_ADD     → otomatik dönüşüm + doğru sonuç
ROK overhead: ~0 cycle (L1 cache hit — ölçülemez seviyede)
ROK-HOT tablo: 25 byte (L1'in %0.07'si)
```

*POC 2 — Fibonacci Adaptif:*
```
fib(222) = 11111460156937785151929026842503960837766832936 (46 hane)
i64 naive: YANLIŞ (taşma)
ROK:       DOĞRU (fib(93)'te i64→BigDecimal geçişi)

Memoize etkisi:
  İlk hesaplama:  35582 cycle
  Tekrar erişim:    230 cycle (ROK-HOT L1 hit)
  Hızlanma:          154×
```

### EOK (Encryptic OK)

Binary koruma — düşük öncelik, COK/POK/ROK sonrası:
- EOK1: Control flow obfuscation
- EOK2: String/sabit şifreleme (kuantum dirençli)
- EOK3: VM-tabanlı kod sanallaştırma

---

## 7. Middleware Deseni

OK katmanı, modüllerin doğrudan optimizasyon yapmaması için bir middleware
katmanı oluşturur:

```
YANLIŞ:  parser.mlp  →  "optimizasyon yap"  ← parser bunu bilmemeli
DOĞRU:   parser.mlp  →  OK katmanı  →  "optimizasyonu BEN yaparım"
                 ↑
         Modüller optimizasyonu bilmez; OK araya girer, organize eder.
```

Bu desen:
- Single Responsibility: her modül kendi işini yapar
- Separation of Concerns: optimizasyon bilgisi tek yerde
- Peer-to-peer uyumlu: merkezi orchestrator gerektirmez

---

## 8. Performans Verileri

| Dil/Sistem | Compile-time opt | AST bypass | Runtime adapt | Binary koruma | Birleşik framework |
|---|---|---|---|---|---|
| C/C++ | LLVM | ❌ | ❌ | ❌ | ❌ |
| Go | gc | ❌ | GC+sched | ❌ | ❌ |
| Rust | LLVM | ❌ | ❌ | ❌ | ❌ |
| Zig | Self-hosted | ZIR ✅ | ❌ | ❌ | ❌ |
| **MELP** | **COK** | **POK** | **ROK** | **EOK** | **✅ Hepsi STO→SAO→FBO** |

**Ölçülen kazanımlar:**

- Cache line etkinliği: 16 node/cache line (Rust: 2–4)
- Derleme hızı: %5–15 artış (FBO kompakt encoding)
- ROK memoize: 154× (fib benchmark)
- STO cast overhead: ~1 instruction (Rust ile eşdeğer)

---

## 9. Uygulama Durumu

```
Stage 1 (Mevcut):
  ✅ COK implementasyonu — ok_layer.cpp (1166 satır)
  ✅ FBO tablo altyapısı — fbo_catalog.h (52+ giriş), fbo_table.h/cpp
  ✅ ROK POC kanıtlandı — source/okr_poc.c, source/okr_fib.c
  ❌ COK→Backend entegrasyonu — FBO kararları codegen'e henüz iletilmiyor
  ❌ Parser kritik düzeltmeler — else if, for, string concat

Stage 2:
  ☐ POK tasarımı ve implementasyonu (flat tablo, AST bypass)
  ☐ COK + POK mode seçimi --mode=cok/pok/auto
  ☐ ROK'u MELP codegen'e entegre et

Stage 3:
  ☐ ROK-HOT profiling ve otomatik boyutlandırma
  ☐ ROK prefetch hint'leri (LLVM intrinsic)
  ☐ EOK1 temel koruma
```

**İlgili kaynak dosyalar:**
```
compiler/stage0/ok_layer/
├── ok_layer.h          (368 satır)
├── ok_layer.cpp        (1166 satır) — pipeline
├── fbo_catalog.h       (340 satır) — 52+ statik FBO giriş
├── fbo_table.h/cpp     (418 satır) — iki bölgeli tablo
├── value_type.h        (151 satır) — değer tipleri
└── structure_kind.h    (152 satır) — yapı türleri

source/
├── okr_poc.c           — ROK POC: tip-strateji dispatch
└── okr_fib.c           — ROK POC: fibonacci adaptif
```

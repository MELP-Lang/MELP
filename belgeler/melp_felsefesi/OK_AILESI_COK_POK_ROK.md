# MELP OK Ailesi: COK, POK, ROK (ve EOK)

**Tarih:** 23 Şubat 2026  
**Durum:** Mimari Vizyon + ROK POC Kanıtlandı  
**Yazar:** PD (Product Director) + Kurucu beyin fırtınası

---

## 🎯 Temel Fikir

OK (Optimization Kernel) tek bir soyutlamadır: **STO → SAO → FBO pipeline'ı**.  
Bu soyutlama **dört farklı etki alanında** uygulanır:

```
COK  →  Compiler OK      (derleme zamanı, AST üzerinde)
POK  →  Parser-Codegen OK (derleme zamanı, AST'yi bypass, flat tablo)
ROK  →  Runtime OK        (çalışma zamanı, canlı veri üzerinde)
EOK  →  Encryptic OK      (binary koruma, nice-to-have)
```

Hiçbir programlama dilinde aynı soyutlama hem derleme hem çalışma zamanında
birleşik olarak çalışmaz. Her dilde optimize eden sistem farklı API, farklı
felsefe, farklı veri yapısı kullanır. MELP'te **STO→SAO→FBO her yerde aynıdır.**

---

## 📐 Pipeline Hatırlatma: STO → SAO → FBO

```
STO (Static/Semantic Tag Objects)
  "NE var?" — Veriyi etiketle
  CT: AST node'larına tag ata (VALUE, STRUCTURE, EFFECT)
  RT: Canlı nesneye tag ata (I64, BIGNUM, STRING, HOT_DATA)

SAO (Semantic Analysis Objects)  
  "NE yapılacak?" — İşlemi zenginleştir
  CT: Ownership, lifetime, effect analizi
  RT: Erişim paterni, paylaşım durumu, I/O frekansı

FBO (Fast Binary Objects)
  "NASIL yapılsın?" — Strateji seç
  CT: inline, dead path elimination, stack/heap kararı
  RT: algoritma seçimi (native vs software), cache hint, prefetch
```

---

## 🔷 COK — Compiler OK

**Etki alanı:** Derleme zamanı  
**Girdi:** AST (Abstract Syntax Tree)  
**Çıktı:** Optimize edilmiş LLVM IR  
**Durum:** ✅ Implementasyon var (ok_layer.cpp, 1166 satır)

### Bugün Ne Yapıyor

```
Kaynak (.mlp) → Lexer → Parser → AST
                                   ↓
                              ┌────────┐
                              │  COK   │
                              │ STO    │ → AST node'larını etiketle
                              │ SAO    │ → ownership, lifetime analiz
                              │ FBO    │ → optimizasyon kararları
                              └───┬────┘
                                  ↓
                              LLVM IR → Binary
```

### Dosyalar

```
compiler/stage0/ok_layer/
├── ok_layer.h          (368 satır) — STO, SAO, FBO sınıfları
├── ok_layer.cpp        (1166 satır) — Pipeline implementasyonu
├── fbo_catalog.h       (340 satır) — 52+ statik FBO entry
├── fbo_table.h/cpp     (418 satır) — İki bölgeli tablo (statik + dinamik)
├── value_type.h        (151 satır) — Değer tipleri
└── structure_kind.h    (152 satır) — Yapı türleri
```

### Bilinen Eksik: Backend FBO'yu Okumuyor

COK pipeline'ı çalışıyor, FBO tablosunu dolduruyor — ama codegen bu tabloyu
hiç sorgulamıyor. `TODO_BEFORE_SELFHOSTING.md` → B-1 maddesi.

Bu demek ki COK şu an STO→SAO→FBO hesaplıyor ama FBO kararları boşa gidiyor.
Backend'in FBO'yu okuması COK'un tamamlanması için **zorunlu**.

---

## 🔶 POK — Parser-Codegen OK

**Etki alanı:** Derleme zamanı (AST bypass)  
**Girdi:** Token stream  
**Çıktı:** LLVM IR (doğrudan, AST ağaç traversal olmadan)  
**Durum:** 📋 Konsept (Stage 2 hedefi)

### Fikir

COK'ta pipeline: Token → AST → STO→SAO→FBO → IR  
POK'ta pipeline: Token → Flat Tablo → STO→SAO→FBO → IR  

POK, AST ağaç yapısını atlayarak **flat bir tablo** (Zig'in ZIR'ına benzer)
üzerinden çalışır. Her token doğrudan bir tablo satırına dönüşür:

```
Token Stream:    FUNCTION  ADD  LPAREN  A  AS  NUMERIC  ...
                    ↓       ↓     ↓     ↓  ↓     ↓
POK Flat Table:  ┌──────┬──────┬──────┬──────┬──────┐
                 │OP:FN │OP:ID │OP:GRP│OP:ID │OP:TYP│
                 │idx:0 │idx:1 │idx:2 │idx:3 │idx:4 │
                 └──────┴──────┴──────┴──────┴──────┘
```

### Neden Gerekli

1. **Derleme hızı:** AST ağaç oluşturmak + traverse etmek pahalı. POK bu maliyeti sıfırlar.
2. **Bellek:** AST her node için heap allocation yapar. Flat tablo tek allocaiton.
3. **Cache dostu:** Flat tablo ardışık bellekte yaşar → L1 cache hit oranı çok yüksek.
4. **Zig kanıtı:** Zig derleyicisi bu yaklaşımla C++'tan 3-5x hızlı derliyor.

### COK ile İlişki

POK, COK'un **yerine geçmez** — alternatif bir pipeline'dır:

```
--mode=cok  → AST tabanlı derleme (daha zengin analiz, yavaş)
--mode=pok  → Flat tablo derleme (daha az analiz, hızlı)
--mode=auto → Küçük dosyalarda POK, büyük/karmaşık dosyalarda COK
```

---

## 🔴 ROK — Runtime OK

**Etki alanı:** Çalışma zamanı (derlenmiş binary içinde)  
**Girdi:** Canlı veri (heap'teki nesneler)  
**Çıktı:** Runtime strateji kararları  
**Durum:** ✅ POC kanıtlandı (2 ayrı benchmark)

### Temel Prensip

ROK, derlenmiş programın **içine gömülen** bir karar motorudur.  
CPU FBO tablosunu doğrudan okuyamaz — CPU'nun instruction set'inde böyle bir
opcode yok. Çözüm: FBO tablosu **heap'e gömülür**, program normal memory read
ile okur.

```
Binary başlatıldı:
  ROK tabloları heap'e yüklenir
  ↓
Her karar noktasında:
  1. STO tag'ini oku (heap'ten, ~4 cycle L1 hit)
  2. FBO strateji tablosuna bak (heap'ten, ~4 cycle L1 hit)  
  3. Branch: strateji A mı B mi?
  4. İlgili kodu çalıştır
```

### Cache Hiyerarşisi Stratejisi

ROK tabloları CPU cache hiyerarşisine bilinçli olarak yerleştirilir:

```
┌──────────┬──────────┬───────────┬───────────────────────────────┐
│ ROK      │ Hedef    │ Max Boyut │ İçerik                        │
│ Katmanı  │ Cache    │           │                               │
├──────────┼──────────┼───────────┼───────────────────────────────┤
│ ROK-HOT  │ L1 32KB  │ ≤8 KB    │ En sık 256 strateji kararı    │
│ ROK-WARM │ L2 256KB │ ≤64 KB   │ Tüm STO tag tablosu           │
│ ROK-COLD │ L3 12MB  │ ≤2 MB    │ SAO enrichment, nadir kararlar│
│ ROK-FULL │ RAM      │ Sınırsız │ Tam state + profiling verisi   │
└──────────┴──────────┴───────────┴───────────────────────────────┘
```

CPU cache'i **doğrudan kontrol edilemez** ama **davranışı garanti edilebilir:**
- Küçük tut → L1'e sığar → otomatik olarak orada kalır
- Sık eriş → LRU politikası onu cache'te tutar
- Ardışık diz → cache line (64 byte) verimliliği maksimum
- Prefetch hint → CPU bir sonraki veriyi önceden cache'e çeker

### COK ↔ ROK İlişkisi

COK (derleme zamanı) ROK'un **ne kadar agresif** olacağına karar verir:

```melp
numeric a = 5            -- COK: tip belli, boyut belli → ROK BYPASS, native ADD
numeric b = input()      -- COK: boyut runtime'da belli olacak → ROK EMBED
```

COK basit işlemlerde ROK'u devre dışı bırakır (sıfır overhead).  
COK sadece dinamik, karmaşık işlemlerde ROK lookup kodu yayar.

### POC Kanıtları

#### POC 1: Tip-Strateji Dispatch (`source/okr_poc.c`)

```
i64 + i64:   NATIVE_ADD stratejisi → doğru sonuç
f64 + f64:   FLOAT_ADD stratejisi → doğru sonuç  
i64 + f64:   MIXED_ADD stratejisi → tip dönüşümü + doğru sonuç
str + str:   STRING_CONCAT stratejisi → doğru birleştirme

ROK overhead: ~0 cycle (L1 cache hit — ölçülemez)
ROK-HOT tablo: 25 byte (L1'in %0.07'si)
```

#### POC 2: Fibonacci Adaptif (`source/okr_fib.c`)

```
fib(222) = 11111460156937785151929026842503960837766832936 (46 hane)

Naive i64:  YANLIŞ sonuç (taşma, sarmalama)
ROK:        DOĞRU sonuç (STO tag → FBO strateji geçişi)

Karar dağılımı:
  fib(2)..fib(92):   NATIVE_ADD  (91 işlem, sıfır overhead)
  fib(93)..fib(222): BIGNUM_ADD  (130 işlem, software bignum)
  Geçiş noktası:     fib(93) — STO taşma tespit → FBO strateji değiştir

Memo cache etkisi:
  İlk hesaplama:     35582 cycle
  Tekrar erişim:     230 cycle (ROK-HOT, L1 hit)
  Hızlanma:          154x

Memo tablosu: 18.3 KB → L1'in %57'si ✅ SIĞIYOR
```

---

## 🟣 EOK — Encryptic OK (Nice-to-Have)

**Etki alanı:** Binary koruma (ters mühendislik dayanıklılığı)  
**Durum:** 📋 Konsept, gelecek aşama  
**Öncelik:** Düşük — COK/POK/ROK'tan sonra

Detaylar `beyin_fırtınası/` altında tartışıldı. Özet:
- EOK1: Control flow obfuscation
- EOK2: String/constant encryption (kuantum dirençli algoritmalarla)
- EOK3: VM-based code virtualization
- EFBO: Şifreli veriyi CPU'ya gönderip hedef makinede çözen dağıtık model

EOK için tehdit modeli ayrı belgelenmelidir. Algoritmik güç (SHA-3, PQC) ile
mimari güvenlik (anti-debug, anti-dump) ayrı kavramlardır.

---

## 🗺️ Yol Haritası

```
Stage 1 (ŞİMDİ):
  ☑ COK implementasyonu var (ok_layer.cpp)
  ☐ COK→Backend entegrasyonu (FBO'yu codegen okusun)
  ☑ ROK POC kanıtlandı (2 benchmark)
  ☐ Parser kritik düzeltmeler (else_if, for, string concat)
  ☐ 291/291 golden test PASS ✅

Stage 2 (Self-hosting sonrası):
  ☐ POK tasarımı ve implementasyonu (flat tablo, AST bypass)
  ☐ COK + POK mode seçimi (--mode=cok/pok/auto)
  ☐ ROK'u MELP codegen'e entegre et (binary'ye ROK tablosu göm)

Stage 3 (Olgunlaşma):
  ☐ ROK-HOT cache profiling ve otomatik tablo boyutlandırma
  ☐ ROK prefetch hint'leri (LLVM intrinsic)
  ☐ EOK1 temel koruma

Stage 4 (İleri):
  ☐ POK + ROK birleşik optimizasyon
  ☐ EOK2/EOK3 gelişmiş koruma
  ☐ EFBO dağıtık şifreleme modeli
```

---

## 📊 Karşılaştırma: MELP vs Diğer Diller

| Özellik | C/C++ | Go | Rust | Java/JVM | Zig | **MELP** |
|---------|-------|-----|------|----------|-----|---------|
| Compile-time opt | LLVM | gc | LLVM | - | Self-hosted | **COK** (STO→SAO→FBO) |
| AST bypass | Yok | Yok | Yok | Yok | ZIR ✅ | **POK** (benzer ama OK ile) |
| Runtime opt | Yok | GC+sched | Yok | JIT | Yok | **ROK** (L1 cache hit) |
| Binary koruma | Yok | Yok | Yok | Obfuscator | Yok | **EOK** (kuantum dir.) |
| Birleşik framework | ❌ | ❌ | ❌ | ❌ | ❌ | **✅ Hepsi STO→SAO→FBO** |

---

## 📁 İlgili Dosyalar

```
Implementasyon:
  compiler/stage0/ok_layer/          — COK implementasyonu (2594 satır)
  source/okr_poc.c                   — ROK POC: tip-strateji dispatch
  source/okr_fib.c                   — ROK POC: fibonacci adaptif

Belgeler:
  belgeler/melp_felsefesi/README.md  — Felsefe dizini ana sayfa
  belgeler/melp_felsefesi/OK_MIDDLEWARE_PATTERN.md  — OK middleware konsepti
  belgeler/melp_felsefesi/FBO_FAST_BINARY_OBJECTS.md — FBO detay
  belgeler/melp_felsefesi/FBO_FRONTEND_BACKEND_OPTIMIZATION.md — FBO pipeline
  belgeler/melp_felsefesi/MELP_OWNERSHIP_MODEL.md — Ownership felsefesi

Beyin fırtınası kaynakları:
  beyin_fırtınası/                   — EOK, EFBO, Dual-OK tartışmaları
```

---

## ⚠️ Sonraki PD İçin Notlar

1. **COK çalışıyor ama backend onu okumuyor.** `codegen_dispatcher.cpp`'de
   FBO tablosunu sorgulayan hiçbir kod yok. Bu TODO B-1.

2. **ROK henüz MELP derleyicisine entegre değil.** Sadece C ile yazılmış bağımsız
   POC'lar var (`source/okr_poc.c`, `source/okr_fib.c`). MELP codegen'in binary'ye
   ROK tablosu gömmesi Stage 2 işi.

3. **POK tamamen konsept.** Kod yok, sadece vizyon. Zig'in ZIR'ını incele.

4. **EOK deprioritize edildi.** COK/POK/ROK tamamlanmadan EOK'a başlama.

5. **OK adlandırma değişti.** Eski belgelerde "OK Layer", "OptimizationLayer",
   "STO/SAO/FBO" olarak geçer. Yeni terminoloji: COK/POK/ROK/EOK.
   Eski isimleri gördüğünde bunlar aynı şeydir.

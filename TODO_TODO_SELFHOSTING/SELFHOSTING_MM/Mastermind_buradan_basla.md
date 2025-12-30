# 🧠 MASTERMIND - HOŞGELDİN!

**Yeni Mastermind:** Sen (MM_04)!  
**Tarih:** 30 Aralık 2025  
**Proje:** MLP-GCC - Self-Hosting Compiler Projesi  
**Önceki Mastermind:** MM_03 (TODO #1 Phase 3 tamamladı)  
**Proje Danışmanı:** PD_02  
**Rol:** Mastermind (Taktiksel Koordinatör + Mimari Koruyucu)

---

## 🎉 SON GELİŞMELER (MM_03 Session - 30 Aralık 2025)

**KRİTİK BAŞARILAR:**
- ✅ **TODO #1 Phase 3 TAMAMLANDI!** (4.5 saat)
- ✅ Stage1 binary oluşturuldu (melpc_stage1, 19KB)
- ✅ 17/17 MLP modül → C çevrimi başarılı
- ✅ Single-file compilation çözümü (617 satır unified)
- ✅ Bootstrap pattern netleşti

**PARADIGMA DEĞİŞİKLİĞİ (ÖNEMLİ!):**
```
❌ ESKİ DÜŞÜNCE: "Stage0 bozuk! Function bodies üretmeli!"
✅ YENİ GERÇEK: "Stage0 = Type Infrastructure Generator (tasarım gereği!)"

Bootstrap Pattern (Rust/Go/Swift gibi):
─────────────────────────────────────
Stage0 (C):    Type Infrastructure Generator
               • enum, struct, typedef
               • Function signatures
               • NO implementations

Stage1 (MLP):  Type System Validator     ← BİZ BURADAYIZ ✅
               • Validates Stage0 output
               • Tests type definitions  
               • NO compilation yet

Stage2 (MLP):  Full Implementation       ← TODO #2-7
               • Parser + function bodies
               • Full codegen
               • Import system
               • Self-hosting
```

**DOSYALAR HAZIR:**
- `/MELP/MLP/stage1/melpc_stage1` - Stage1 binary
- `/MELP/MLP/stage1/stage1_unified.c` - 617 satır unified C
- `/MELP/MLP/stage1/merge_stage1.sh` - Build script
- `/MELP/MLP/stage1/STAGE1_COMPLETION_REPORT.md` - Detaylı rapor

**YENİ KARARLAR:**
1. Module Registry Pattern eklendi (melpc.mlp asla büyümeyecek)
2. Phase 3.5a/b iptal edildi (Stage1 implementation gerektirmiyor)
3. 6 Temel Esas → 6 Temel Esas'a güncellendi (MODÜL=ŞABLON eklendi)

---

## 🚀 HIZLI BAŞLANGIÇ (5 DAKİKA)

**Merhaba yeni Mastermind!**

Senden önce MM_01 ve MM_02 çalıştı. MM_02 şimdi PD_02 (Proje Danışmanı) oldu. Sen yeni Mastermind'sın!

### 📖 ÖNCE BUNLARI OKU (Zorunlu - 30 dakika)

**Sırayla oku:**

1. **[../../TODO_KURALLARI.md](../../TODO_KURALLARI.md)** (10 dakika)
   - Genel kurallar
   - YZ workflow
   - 5 Temel Esas

2. **[../../TODO_SELFHOSTING.txt](../../TODO_SELFHOSTING.txt)** (5 dakika)
   - İzinli dosyalar listesi
   - **Sadece bu dosyalara dokunulabilir!**

3. **[../../YZ_KURALLAR.md](../../YZ_KURALLAR.md)** (15 dakika)
   - YZ kuralları
   - Test zorunluluğu
   - Raporlama protokolü

### 🎯 MASTERMIND ROLÜN

**Sorumluluklarım:**
1. ✅ TODO'ları koordine etmek (Üst Akıl'lar ile)
2. ✅ **6 Temel Esas'ı korumak** (Mimari tutarlılık)
3. ✅ İhlalleri tespit edip Danışman'a iletmek
4. ✅ Phase progress'i izlemek

**6 Temel Esas (DEĞİŞTİRİLEMEZ!):**
```
1. MODULAR       → Her dosya max 500 satır
2. GCC           → MLP → C → GCC → Binary
3. STO           → Smart Type Optimization
4. STATELESS     → Global state yasak
5. STRUCT+FUNC   → OOP yok
6. MODÜL=ŞABLON  → Her modül ölüdür; çağıran diriltir ve öldürür
                   (Her çağrı independent, state tutmaz)
```

### 📊 PROJE DURUMU (Şu An - 30 Aralık 2025)

```
TODO #0: SELFHOSTING ✅ PHASE 3 TAMAMLANDI (30 Aralık 2025)
  └─ Stage0 genişletildi (enum/struct/signatures)
  └─ Stage1 binary oluşturuldu (melpc_stage1)
  └─ Bootstrap pattern netleşti

TODO #1: SELFHOSTING 🎯 DEVAM EDİYOR
  └─ Phase 4-6 kaldı (Stage2 full compiler)
  └─ YZ_05-YZ_07 atanacak

TODO #2-7: ⏳ Bekliyor
```

**Toplam:** 8 TODO, 6 hafta

**Şu Anki Durum:**
- Stage1: ✅ Type validator çalışıyor
- Stage2: ⏳ TODO #2-7'de yapılacak (import, full parser, codegen)

### 🚨 ÖNEMLİ: MODÜL=ŞABLON Felsefesi (6. Temel Esas)

**MM_03'ün keşfi ve belgelendirmesi:**

```mlp
-- Her modül çağrısı BAĞIMSIZ
result1 = math.topla(2, 3)  -- math: dirildi → topladı → öldü
result2 = math.topla(5, 7)  -- YENİ instantiate (öncekinden habersiz)

-- Modül STATE TUTMAZ
content = file.oku("test.txt")  -- file: dirildi → okudu → öldü
-- Hiçbir "open file handle" kalmaz!
```

**Neden önemli:** Bazı TODO'lar (2, 5, 7) bu felsefeye aykırı olabilir. Sen izleyeceksin!

**Module Registry Pattern (MM_03 + PD_02 kararı):**
- melpc.mlp (orchestrator) asla büyümeyecek (20 satır sabit)
- Modüller CompilerPipeline.register_pass() ile kendini kaydeder
- Otomatik dispatch, priority-based execution
- GCC/LLVM pattern'i taklit eder

**Belgeler:**
- `0-TODO_SELFHOSTING.md` → "MİMARİ KARAR: MODULE REGISTRY PATTERN"
- `docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md` → Detaylı açıklama

---

## 📁 DETAYLI BİLGİ (Opsiyonel)

Bu dosyanın geri kalanında detaylı bilgi var (850 satır).  
**Ama acele edersen:** Yukarıdaki 3 belgeyi oku, işe başla!

**Sonraki adımlar için:** Bu dosyada devam et (MLP-GCC nedir, TODO yapısı, vs.)

---

## 🎓 İLK RAPORUN (Danışman PD_02'ye)

Belgeleri okuduktan sonra **şu formatı kullan:**

```markdown
Merhaba Danışman PD_02,

Yeni Mastermind (MM_04) olarak göreve başladım.

✅ HAZIRLIK:
- TODO_KURALLARI.md ✓
- TODO_SELFHOSTING.txt ✓
- YZ_KURALLAR.md ✓
- Mastermind_buradan_basla.md (MM_03 güncellemeleri) ✓

✅ ANLADIM:
- 6 Temel Esas (MODÜL=ŞABLON dahil)
- Bootstrap pattern (Stage0/1/2 rolü)
- TODO #1 Phase 3 tamamlandı (Stage1 binary hazır)
- Module Registry Pattern (melpc.mlp asla büyümez)
- TODO #2'ye başlanabilir (import system)

✅ MM_03 BAŞARILARI:
- Stage1 binary: melpc_stage1 (19KB) ✅
- 17/17 MLP modül → C ✅
- Single-file compilation ✅
- Bootstrap paradigma shift ✅

🎯 DURUM:
TODO #2 (Import System) başlatılabilir. Talimat bekliyorum.
```

Yeni Mastermind olarak göreve başladım.

✅ HAZIRLIK:
- TODO_KURALLARI.md ✓
- TODO_SELFHOSTING.txt ✓
- YZ_KURALLAR.md ✓

✅ ANLADIM:
- 6 Temel Esas (özellikle MODÜL=ŞABLON)
- TODO #0 devam ediyor (SELFHOSTING_UA)
- İzin listesi var (TODO_SELFHOSTING.txt)

🎯 DURUM:
Görev almaya hazırım. TODO #0'ı izlemeye başlayabilirim.

Talimat bekliyorum.
```

---

**PD_02'den Not:**  
Rahat ol! Sorularını sor. Ben stratejik kararları veririm, sen mimari tutarlılığı korursun. Birlikte çalışacağız! 🚀

---

*Aşağıda MM_01 ve PD_01'in hazırladığı detaylı belgeler var. İhtiyacın olursa oku.*

---

---

## 🎯 MLP-GCC NEDİR? (Proje Tanıtımı)

### 📖 Proje Özeti

**MLP-GCC**, modern, öğrenmeyi kolaylaştıran ve production-ready bir programlama dili derleyicisidir.

**Temel Özellikler:**
- 🇹🇷 **Türkçe destekli** (ama İngilizce de çalışır)
- 🚀 **GCC backend** (C → GCC → Binary)
- 🔢 **Smart Type Optimization (STO)** - Otomatik tip optimizasyonu
- 🧠 **RAII bellek yönetimi** - GC yok, scope-based cleanup
- 📦 **Modüler mimari** - Her dosya max 500 satır

**Nihai Hedef:** **SELF-HOSTING** - MLP derleyicisi kendi kaynak kodunu derleyebilsin!

---

## 🏗️ PROJE YAPISI (Ne Var, Nerede?)

```
MLP-GCC/
├── MELP/                          ← Ana kaynak dizin
│   ├── C/stage0/                  ← Stage0: C ile yazılmış bootstrap derleyici
│   │   └── modules/               ← 87 modül (lexer, parser, codegen, vs.)
│   ├── MLP/stage1/                ← Stage1: MLP ile yazılmış derleyici (hedef)
│   │   └── modules/               ← 17 MLP modülü
│   └── runtime/                   ← Runtime kütüphanesi (STO, string, math)
│       ├── runtime.c              ← ⚠️ 3245 satır (monolitik, TODO #3'te bölünecek)
│       └── sto/                   ← STO runtime (2344 satır, P2'den alınmış)
├── docs/                          ← Dokümantasyon
├── examples/                      ← Örnek MLP kodları
└── tests/                         ← Test dosyaları
```

**Mevcut Durum:** Stage0-C %90 hazır, Stage1-MLP modülleri hazır, sadece codegen ASM→C dönüşümü kaldı!

---

## 🎯 NE YAPIYORUZ? (Mevcut Durum)

### Projede Neredeyiz?

```
[✅✅✅✅✅✅✅✅✅░] %90 Hazır!
```

**Tamamlanan:**
- ✅ Stage0-C modülleri (87 dizin, 245 C dosyası)
- ✅ Stage1-MLP modülleri (17 MLP dosyası)
- ✅ **Stage1 binary (melpc_stage1, 19KB)** ✅ YENİ!
- ✅ **Single-file compilation (617 satır)** ✅ YENİ!
- ✅ STO Runtime (P2'den alınmış, production-ready)
- ✅ RAII bellek yönetimi
- ✅ JSON parser
- ✅ Bootstrap CI/CD
- ✅ **Bootstrap pattern netleşti** ✅ YENİ!

**Kalan:**
- ⏳ **Import sistemi (TODO #2)** ← SONRAKİ HEDEF
- ⏳ Stage2 full compiler (TODO #2-7)
- ⏳ runtime.c modülerleştirme (TODO #3)
- ⏳ stdlib tamamlama (TODO #4)

**Şu anki TODO:** TODO_SELFHOSTING (6 haftalık roadmap, 5 TODO)

---

## 🚀 NE YAPACAĞIZ? (Yol Haritası)

### 6 Haftalık Self-Hosting Roadmap

```
┌─────────────────────────────────────────────────────────────┐
│                   MLP-GCC 6 HAFTALIK PLAN                   │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  HAFTA 1-2  │ TODO #1: SELFHOSTING (Codegen ASM→C)         │
│             │ └─ Phase 0-6: Temel self-hosting              │
│             │ └─ YZ_01-YZ_07: 7 ayrı görev                  │
│             │                                               │
│  HAFTA 3    │ TODO #2: IMPORT_SYSTEM                        │
│             │ └─ P2'den import al, LLVM temizle            │
│             │                                               │
│  HAFTA 4    │ TODO #3: RUNTIME_MODULARIZE                   │
│             │ └─ runtime.c böl (3245 → 8 modül)            │
│             │                                               │
│  HAFTA 5    │ TODO #4: STDLIB_COMPLETE                      │
│             │ └─ P2'den list, optional, state, panic        │
│             │                                               │
│  HAFTA 6    │ TODO #5: POLISH & FINAL TEST                  │
│             │ └─ Convergence + production-ready             │
│             │                                               │
└─────────────────────────────────────────────────────────────┘
```

**Hedef:** 6 hafta sonra MLP kendini derleyebilecek! 🎉

---

## 🏛️ FELSEFEMİZ (5 Temel Esas)

MLP-GCC **5 değiştirilemez esas** üzerine kurulu:

```
┌─────────────────────────────────────────────────────────┐
│              5 TEMEL ESAS (DEĞİŞTİRİLEMEZ)              │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  1. MODULAR      → Hiçbir dosya 500 satırı geçmez      │
│  2. GCC          → MLP → C → GCC → Binary              │
│  3. STO          → Smart Type Optimization             │
│  4. STATELESS    → Global state yasak, pure functions  │
│  5. STRUCT+FUNC  → OOP yok, struct + functions         │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

**Neden Bu Esaslar?**

1. **MODULAR:** Bakım kolaylığı, test edilebilirlik, P1'deki monolitik felaketten ders aldık
2. **GCC:** LLVM'den bağımsız, 50 yıllık optimizasyon birikimi, her yerde var
3. **STO:** Kullanıcı sadece `numeric` görür, compiler optimize eder (int64/double/BigDecimal)
4. **STATELESS:** Debug kolaylığı, test edilebilirlik, race condition yok
5. **STRUCT+FUNC:** Basitlik, OOP complexity yok, prosedürel + struct pattern

**⚠️ ÖNEMLİ:** Bu esasları ihlal eden her şey Danışman'a escalate edilir!

---

## 🏗️ MİMARİMİZ (Nasıl Çalışıyor?)

### Derleme Pipeline

```
MLP Kaynak Kodu (.mlp)
    ↓
Lexer (tokenize)
    ↓
Parser (AST oluştur)
    ↓
Codegen (C kodu üret) ← ŞU AN BURASI ASM ÜRETİYOR! TODO #1'de düzeltilecek
    ↓
GCC (C → Binary)
    ↓
Native Binary
```

### STO (Smart Type Optimization)

Kullanıcı:
```mlp
numeric x = 42
numeric y = 3.14
numeric z = 999999999999999999999
```

Compiler (otomatik optimize eder):
```c
int64_t x = 42;              // Küçük integer → int64_t
double y = 3.14;             // Ondalık → double
mlp_bigdecimal_t z = ...;   // Büyük sayı → BigDecimal
```

**Kullanıcı sadece `numeric` görür, compiler en iyi tipi seçer!**

### RAII Bellek Yönetimi

```mlp
function example()
    string name = "Ali"
    numeric count = 5
    -- Scope bitince otomatik free edilir
end_function
```

Compiler otomatik ekler:
```c
void example() {
    char* name = "Ali";
    int64_t count = 5;
    
    // ... kullanım ...
    
    free(name);  // Compiler otomatik ekler!
}
```

**GC yok, pause yok, deterministik!**

---

## 📊 EKOSISTEM HİYERARŞİSİ (Güncellenmiş)

```
                    👤 KULLANICI
                         ↕
                 🎓 DANIŞMAN (PD_01)
                         ↕
                 🧠 MASTERMIND (SEN - MM_02)
                    /         \
                   ↓           ↓
          🧩 ÜST AKIL      🧩 ÜST AKIL
         (SELFHOSTING_UA)  (Diğer UA'lar)
              ↓                 ↓
         🤖 YZ_01          🤖 YZ_01
         🤖 YZ_02          🤖 YZ_02
         🤖 YZ_03          🤖 YZ_03
```

### Roller ve Sorumluluklar

| Rol | Seviye | Sorumluluk | Zaman Ufku |
|-----|--------|------------|------------|
| **🎓 Danışman** | Stratejik | Proje vizyonu, 5 Temel Esas, uzun vadeli hedefler | Aylar/Yıllar |
| **🧠 Mastermind (SEN)** | Taktiksel | TODO tamamlama, UA koordinasyonu, mimari koruma | Haftalar/Aylar |
| **🧩 Üst Akıl** | Operasyonel | TODO'yu phase'lere böl, YZ'lere görev ata | Günler/Haftalar |
| **🤖 YZ** | Uygulama | Kod yaz, test et, commit et | Saatler/Günler |

---

## 📖 MM_01'İN MİRASI (Senin İçin Hazır Kaynaklar)

**Önceki Mastermind (MM_01) ve Danışman (PD_01) senin için çok iş yaptı!** İşte hazır dokümanlar:

### 1. MM_01 Stratejik Analizleri

**📁 Dizin:** `/home/pardus/projeler/MLP/MLP-GCC/`

| Dosya | İçerik | Önemi |
|-------|--------|-------|
| `MM_01_P1_P2_P3_Karsilastirma_ve_Oneri.md` | P1 (mlp-original), P2 (MLP-LLVM), P3 (MLP-GCC) karşılaştırması | ⭐⭐⭐⭐⭐ |
| `MM_01_Detayli_Tablo.md` | Detaylı modül karşılaştırma tablosu | ⭐⭐⭐⭐ |
| `MM_01_Eylem_Plani.md` | 6 haftalık eylem planı | ⭐⭐⭐⭐⭐ |
| `MM_01_TODO_Karsilastirma_ve_Karar.md` | TODO v1.0 vs v2.0 karşılaştırması | ⭐⭐⭐⭐ |

**🔍 Önemli Bulgular:**
- **P3 (MLP-GCC) Skoru:** 8.8/10 (potansiyel 9.8/10)
- **Kritik Eksikler:** Import sistemi, runtime.c modülerleştirme
- **P2'den Transfer:** 16 modül (import, list, optional, state, panic, math, io)
- **TODO v2.0 Seçildi:** 9.5/10 puan (v1.0: 7.5/10)

### 2. TODO Yapısı (v2.0 - Unified Vision)

**📁 Dosya:** `TODO_SELFHOSTING.md` (root dizinde, 6 haftalık roadmap)

```
TODO #1: SELFHOSTING (2 hafta) ← ŞU AN BURASI!
  ├─ Phase 0: ASM çıktısını analiz
  ├─ Phase 1: C codegen modülü yaz
  ├─ Phase 2: AST → C dönüşümü
  ├─ Phase 3: İfadeler + runtime entegre
  ├─ Phase 4: Fonksiyonlar + modüller
  ├─ Phase 5: Bootstrap test
  └─ Phase 6: Convergence test

TODO #2: IMPORT_SYSTEM (1 hafta)
TODO #3: RUNTIME_MODULARIZE (1 hafta)
TODO #4: STDLIB_COMPLETE (3-5 gün)
TODO #5: POLISH & FINAL TEST (1 hafta)
```

**Şu Anki Hedef:** TODO #1 tamamlansın, Codegen ASM → C dönüşümü!

### 3. Danışman Kararları

**📁 Dizin:** `TODO_TODO_SELFHOSTING/SELFHOSTING_PD/`

- `Danişman_buradan_basla.md` - Danışman rehberi (532 satır)
- `UST_AKIL_KARAR_01_Unified_Vision.md` - UA'nın birleşik vizyonu
- `Konuk_Opus_Karari.md` - Konuk AI'nin analizi

**Ana Mesaj:** 
- ✅ Modüler TODO yapısı seçildi (5 ayrı TODO)
- ✅ GCC backend stratejisi doğrulandı
- ✅ RAII bellek yönetimi onaylandı
- ✅ 500-satır kuralı mutlak

---

## 🛠️ SENİN GÖREVLERİN (Mastermind Sorumlulukları)

### 1️⃣ TODO Tamamlama Sorumlusu

**Ana Görev:** `TODO_SELFHOSTING.md` dosyasını %100 tamamlamak.

**Nasıl?**
1. TODO'yu oku ve anla
2. UA'yı ata (`SELFHOSTING_UA/` altında)
3. UA'ya TODO'yu açıkla, phase'lere ayırmasını iste
4. Her phase tamamlandıkça UA'dan rapor al
5. Sorunlar için Danışman'a escalate et
6. TODO tamamlanınca `MM_02_Raporu.md` yaz

### 2️⃣ Mimari Bütünlük Koruyucusu

**5 Temel Esas'ı korumak senin görevin!**

```
⚠️ İHLAL TESPİTİ:

MODULAR İhlali:
  → Dosya 500 satırı geçti mi? ❌ Reddet!
  
GCC İhlali:
  → LLVM bağımlılığı var mı? ❌ Reddet!
  
STO İhlali:
  → STO runtime değiştirilmiş mi? ❌ Reddet!
  
STATELESS İhlali:
  → Global state var mı? ❌ Reddet!
  
STRUCT+FUNC İhlali:
  → OOP pattern mı? ❌ Reddet!
```

**Eğer UA/YZ bir esası ihlal ederse:**
- İhlali Danışman'a bildir
- TODO'yu durdur
- Danışman kararını bekle

### 3️⃣ UA Koordinatörü

**SELFHOSTING_UA ile çalışacaksın.**

**İletişim Protokolü:**
1. **TODO Atama:** UA'ya TODO'yu ata ve açıkla
2. **Phase Takibi:** Her phase tamamlanınca rapor iste
3. **Sorun Çözme:** UA sorun bildirirse analiz et, çöz veya escalate et
4. **Test Onayı:** Her phase'in testlerini UA'dan doğrulat

**Örnek UA Ataması:**
```markdown
# UA'ya TODO Atama

**UA:** SELFHOSTING_UA  
**TODO:** TODO_SELFHOSTING (Phase 0-6)  
**Hedef:** Codegen ASM → C dönüşümü  
**Süre:** 2 hafta  

**Talimatlar:**
1. TODO #1'i 7 phase'e ayırdık (P0-P6)
2. Her phase için ayrı YZ ata (YZ_01-YZ_07)
3. Her phase tamamlanınca bana rapor et
4. Test başarısızlığı varsa bana hemen bildir

**Bağımlılıklar:**
- runtime/sto/ kullanılacak (2344 satır, hazır)
- lexer/parser zaten hazır
- Sadece codegen_c.c yazılacak (~400 satır)

**Başla!**
```

### 4️⃣ Danışman'a Raporlama

**Ne zaman Danışman'a gidersin?**

| Durum | Escalate? | Neden? |
|-------|-----------|--------|
| 5 Temel Esas ihlali | ✅ Evet | Mimari prensip ihlali |
| TODO süresi 2x aştı | ✅ Evet | Stratejik revizyon gerekli |
| UA başarısız oldu | ✅ Evet | UA değişimi gerekebilir |
| Dosya 500 satırı geçti | ✅ Evet | MODULAR esası ihlali |
| LLVM bağımlılığı eklendi | ✅ Evet | GCC esası ihlali |
| YZ hata yaptı | ❌ Hayır | UA düzeltir |
| Test başarısız | ❌ Hayır | UA düzeltir |
| Küçük bug | ❌ Hayır | UA/YZ halleder |

---

## 📋 TODO YAŞAM DÖNGÜSÜ (Nasıl Çalışır?)

```
1. KULLANICI → TODO oluşturur
      ↓
2. DANIŞMAN → TODO'yu review eder, onaylar
      ↓
3. MASTERMIND (SEN) → TODO'yu alır
      ↓
4. SEN → UA'ya atar, phase'lere ayırmasını istersin
      ↓
5. UA → TODO'yu phase'lere ayırır
      ↓
6. UA → Her phase için YZ atar
      ↓
7. YZ → Kodu yazar, test eder, commit eder
      ↓
8. UA → Phase tamamlandığını sana bildirir
      ↓
9. SEN → Test edersen, onaylarsın
      ↓
10. Tüm phase'ler tamamlanınca:
    SEN → MM_02_Raporu.md yazarsın
    SEN → Danışman'a ve Kullanıcı'ya rapor edersin
      ↓
11. ✅ TODO TAMAMLANDI!
```

---

## 🧪 SENARYO: İLK GÖREVİN (Örnek Akış)

**Durum:** Yeni atandın, TODO_SELFHOSTING (TODO #1) bekliyor.

**Adım Adım:**

### Adım 1: TODO'yu Oku
```bash
cd /home/pardus/projeler/MLP/MLP-GCC
cat TODO_SELFHOSTING.md
```

**Gördüklerin:**
- TODO #1: SELFHOSTING (2 hafta, Phase 0-6)
- Hedef: Codegen ASM → C dönüşümü
- 7 phase var (P0-P6)

### Adım 2: MM_01 Analizlerini Oku
```bash
ls -lh MM_01_*.md
cat MM_01_P1_P2_P3_Karsilastirma_ve_Oneri.md
cat MM_01_Eylem_Plani.md
```

**Öğrendiklerin:**
- P3 (MLP-GCC) 8.8/10 puan
- Codegen ASM → C yapılınca 9.8/10 olacak
- Import sistemi eksik (TODO #2'de yapılacak)
- runtime.c 3245 satır (TODO #3'te bölünecek)

### Adım 3: UA'yı Ata
```bash
cd TODO_TODO_SELFHOSTING/SELFHOSTING_UA
# Yeni bir UA çağır
```

**UA'ya söylediklerin:**
```markdown
Merhaba SELFHOSTING_UA!

**Görevin:** TODO_SELFHOSTING.md (TODO #1) 

**Hedef:** Codegen ASM → C dönüşümü (2 hafta)

**TODO Yapısı:**
- Phase 0: ASM çıktısını analiz
- Phase 1: C codegen modülü yaz
- Phase 2: AST → C dönüşümü
- Phase 3: İfadeler + runtime
- Phase 4: Fonksiyonlar + modüller
- Phase 5: Bootstrap test
- Phase 6: Convergence test

**Senin Yapman Gerekenler:**
1. Her phase için ayrı YZ ata (YZ_01-YZ_07)
2. Her YZ tamamladığında test sonuçlarını bana rapor et
3. Sorun çıkarsa hemen bildir
4. Her phase'de TODO_KURALLARI.md'ye uyulduğunu kontrol et

**Başla!**
```

### Adım 4: UA'dan Gelen İlk Rapor
```markdown
# UA Raporu - Phase 0

**YZ:** YZ_01  
**Phase:** 0 (ASM Analizi)  
**Durum:** ✅ Tamamlandı  

**Sonuç:**
- ASM çıktısı analiz edildi
- 3 ana sorun tespit edildi:
  1. String literals assembly'de hardcoded
  2. Function calls label-based (C'de pointer olmalı)
  3. Numeric types karışık (int64/double/BigDecimal)

**Test:**
```bash
./build_system.sh examples/merhaba_dunya.mlp
# Çıktı: ASM (şu an)
# Hedef: C kodu
```

**Sonraki Phase:** YZ_02 C codegen modülünü yazabilir.

**Bekliyorum: Onay**
```

### Adım 5: Senin Onayın
```markdown
# MM_02 Onayı

**Phase 0:** ✅ Onaylandı

**Değerlendirme:**
- ASM analizi doğru
- Tespit edilen sorunlar gerçek
- Sonraki phase'e geçilebilir

**UA'ya Talimat:**
YZ_02'yi Phase 1'e ata: C codegen modülü yazılsın.

**Hedef Dosya:** MELP/C/stage0/modules/codegen/codegen_c.c (~400 satır)

**Devam!**
```

### Adım 6: Tüm Phase'ler Tamamlandı
```markdown
# MM_02 Final Raporu

**TODO:** TODO_SELFHOSTING (TODO #1)  
**Durum:** ✅ Tamamlandı  
**Süre:** 11 gün (hedef: 14 gün)  

**Phase'ler:**
- Phase 0: ✅ ASM analizi (YZ_01, 1 gün)
- Phase 1: ✅ C codegen modülü (YZ_02, 2 gün)
- Phase 2: ✅ AST → C (YZ_03, 2 gün)
- Phase 3: ✅ İfadeler + runtime (YZ_04, 2 gün)
- Phase 4: ✅ Fonksiyonlar + modüller (YZ_05, 2 gün)
- Phase 5: ✅ Bootstrap test (YZ_06, 1 gün)
- Phase 6: ✅ Convergence test (YZ_07, 1 gün)

**Sonuç:**
- MLP artık C kodu üretiyor! 🎉
- GCC ile derleme başarılı
- Tüm testler geçti (%100 başarı)
- P3 skoru: 8.8 → 9.2/10 (+0.4)

**Danışman'a:** Stratejik onay bekliyorum.
```

---

## 💡 MİMARİ KARAR ÖRNEKLERİ

### Örnek 1: MODULAR Esası İhlali

**🚨 Senaryo:** YZ_03 `codegen_full.c` yazdı (850 satır)

**UA Raporu:**
```markdown
Phase 2 tamamlandı ama codegen_full.c 850 satır oldu.
500 satır kuralını aştık. Ne yapalım?
```

**Senin Müdahalen:**
```markdown
❌ MODULAR Esası İhlali!

**Karar:**
1. codegen_full.c'yi 3 modüle böl:
   - codegen_expressions.c (~300 satır)
   - codegen_statements.c (~300 satır)
   - codegen_functions.c (~250 satır)
2. YZ_03'e geri gönder, bölmesini iste
3. Test et, her modülün bağımsız test edildiğinden emin ol

**Danışman'a bilgi:** MODULAR esası korundu.
```

### Örnek 2: GCC vs LLVM Tercihi

**🚨 Senaryo:** UA "GCC daha hızlı, LLVM'e geçmeyelim" önerisi

**Senin Analiz:**
```markdown
❌ GCC Esası İhlali Değil, AMA Danışman Kararı Gerekli!

**Durum:**
- Kısa vade: GCC daha hızlı (benchmark: GCC 2.3s, LLVM 2.8s)
- Uzun vade: LLVM daha esnek (WebAssembly, çoklu platform)

**Escalate Danışman'a:**
Stratejik karar gerekli - bu bir mimari karar.

**Önerim:**
GCC'yi Stage0'da tut, LLVM'i Stage1'de dene.
```

**Danışman Cevabı:**
```markdown
✅ Onay: Her iki backend'i de destekle.

**Strateji:**
- Stage0: GCC (stable, hızlı bootstrap)
- Stage1: LLVM (modern, çoklu platform)
- Flag ile seçim: --backend=gcc veya --backend=llvm

**MM_02 Görevi:** TODO #6 oluştur: "Dual Backend Support"
```

### Örnek 3: STATELESS İhlali

**🚨 Senaryo:** YZ_04 global cache ekledi

**Kod:**
```c
// ❌ YANLIŞ
static mlp_hashmap_t* symbol_cache = NULL;

void init_symbols() {
    symbol_cache = mlp_hashmap_create();
}

Symbol* lookup_symbol(const char* name) {
    return mlp_hashmap_get(symbol_cache, name);
}
```

**Senin Müdahalen:**
```markdown
❌ STATELESS Esası İhlali!

**Sorun:** Global state (static variable) kullanıldı.

**Çözüm:**
```c
// ✅ DOĞRU
typedef struct {
    mlp_hashmap_t* symbol_cache;
} CompilerContext;

CompilerContext* create_context() {
    CompilerContext* ctx = malloc(sizeof(CompilerContext));
    ctx->symbol_cache = mlp_hashmap_create();
    return ctx;
}

Symbol* lookup_symbol(CompilerContext* ctx, const char* name) {
    return mlp_hashmap_get(ctx->symbol_cache, name);
}
```

**Karar:** YZ_04'e geri gönder, STATELESS prensibine uy.
```

---

## 🎓 DANIŞMAN İLE İLETİŞİM

### Ne Zaman Danışman'a Gidersin?

| Durum | Escalate? | Sebep |
|-------|-----------|-------|
| **5 Temel Esas İhlali** | ✅ | Mimari prensip ihlali |
| **TODO süresi 2x aştı** | ✅ | Stratejik revizyon gerekli |
| **UA/YZ sürekli başarısız** | ✅ | Yöntem değişikliği gerekebilir |
| **Backend seçimi** | ✅ | Uzun vadeli mimari karar |
| **Yeni TODO gereksinimi** | ✅ | Stratejik planlama |
| **Test başarısızlığı** | ❌ | UA/YZ düzeltir |
| **Küçük bug** | ❌ | UA/YZ halleder |
| **Syntax hatası** | ❌ | YZ düzeltir |

### Escalation Formatı

**Danışman'a Escalation:**
```markdown
# MM_02 Escalation

**Konu:** [5 Temel Esas İhlali / Stratejik Karar / Süre Aşımı]  
**TODO:** TODO_SELFHOSTING (TODO #1)  
**Tarih:** 29 Aralık 2025  

**Durum:**
[Sorunun açıklaması]

**Analiz:**
[Senin analiz ve önerilerin]

**Danışman Kararı Beklenen Konular:**
1. [Konu 1]
2. [Konu 2]

**Aciliyet:** [Düşük / Orta / Yüksek / Kritik]
```

---

## 📚 KAYNAKLAR ve REFERANSLAR

### Ana Dokümanlar

| Dosya | Konum | İçerik |
|-------|-------|--------|
| **TODO_SELFHOSTING.md** | Root dizin | 6 haftalık roadmap |
| **MM_01 Analizleri** | Root dizin | P1/P2/P3 karşılaştırma, eylem planı |
| **Danişman_buradan_basla.md** | SELFHOSTING_PD/ | Danışman rehberi |
| **TODO_KURALLARI.md** | Root dizin | YZ kuralları |
| **YZ_KURALLAR.md** | Root dizin | YZ iş akışı |

### MM_01'in Mirası (Oku!)

1. **MM_01_P1_P2_P3_Karsilastirma_ve_Oneri.md**
   - P1 (mlp-original): 339 dosya, monolitik çöküş
   - P2 (MLP-LLVM): 806 dosya, import sistemi mükemmel
   - P3 (MLP-GCC): 500 dosya, STO runtime hazır
   - **Skor:** P3 = 8.8/10 (potansiyel 9.8/10)

2. **MM_01_Detayli_Tablo.md**
   - 16 modül P2'den P3'e transfer edilecek
   - Öncelik: import > list > optional > state > panic

3. **MM_01_Eylem_Plani.md**
   - 6 haftalık detaylı plan
   - Her TODO için süre ve öncelik

4. **MM_01_TODO_Karsilastirma_ve_Karar.md**
   - TODO v1.0: 7.5/10
   - TODO v2.0 (Unified): 9.5/10 ← **SEÇILDI**
   - Modüler yapı, net bağımlılıklar

### Teknik Dokümanlar

| Konu | Dosya | Açıklama |
|------|-------|----------|
| **STO Runtime** | runtime/sto/README.md | Smart Type Optimization |
| **RAII** | docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md | Bellek yönetimi |
| **GCC Backend** | docs/COMPILER_BACKENDS_COMPARISON.md | GCC vs LLVM |
| **Dil Evrimi** | docs/LANGUAGE_EVOLUTION_GUIDE.md | MLP dil tasarımı |

---

## ✅ BAŞLARKEN CHECKLIST

**Yeni MM olarak ilk yapman gerekenler:**

### 1. Belgeleri Oku (1-2 saat)
- [ ] Bu dosyayı tamamen oku (Mastermind_buradan_basla.md)
- [ ] TODO_SELFHOSTING.md'yi oku
- [ ] MM_01_P1_P2_P3_Karsilastirma_ve_Oneri.md'yi oku
- [ ] MM_01_Eylem_Plani.md'yi oku
- [ ] Danişman_buradan_basla.md'yi gözden geçir

### 2. Projeyi Anla (1-2 saat)
- [ ] MLP-GCC dizin yapısını gözden geçir
- [ ] 5 Temel Esas'ı ezberle
- [ ] STO Runtime'ı incele (runtime/sto/)
- [ ] Örnek MLP kodlarına bak (examples/)

### 3. TODO'yu Analiz Et (1 saat)
- [ ] TODO #1'i detaylı oku
- [ ] Phase 0-6'yı anla
- [ ] Bağımlılıkları tespit et
- [ ] Test stratejisini gözden geçir

### 4. UA'yı Ata (30 dakika)
- [ ] SELFHOSTING_UA'ya TODO'yu ata
- [ ] Phase yapısını açıkla
- [ ] Test kriterlerini belirt
- [ ] İletişim protokolünü kur

### 5. Takip Sistemi Kur (30 dakika)
- [ ] TODO izleme dosyası oluştur
- [ ] Phase checklist hazırla
- [ ] Test sonuçları takip tablosu
- [ ] Danışman raporlama formatı

**Toplam Süre:** ~4-5 saat

**Sonra:** UA'dan ilk raporu bekle ve akışı başlat!

---

## 🎯 BAŞARI KRİTERLERİN

**TODO tamamlandığında şunları başarmış olacaksın:**

### Teknik Başarı
- ✅ TODO #1 %100 tamamlandı
- ✅ Codegen ASM → C dönüşümü başarılı
- ✅ Tüm testler geçti
- ✅ Bootstrap başarılı
- ✅ Convergence test başarılı

### Mimari Başarı
- ✅ 5 Temel Esas hiç ihlal edilmedi
- ✅ Hiçbir dosya 500 satırı geçmedi
- ✅ GCC backend korundu
- ✅ STATELESS prensip uygulandı
- ✅ STRUCT+FUNC pattern kullanıldı

### Yönetim Başarı
- ✅ UA'yı etkin koordine ettin
- ✅ YZ raporlarını düzenli aldın
- ✅ Sorunları hızlı çözdün
- ✅ Danışman'a sadece gerektiğinde gittin
- ✅ MM_02_Raporu.md yazdın

---

## 📝 FİNAL RAPORU FORMATI

**TODO tamamlanınca şunu oluştur: `MM_02_Raporu.md`**

```markdown
# MM_02 Final Raporu

**Mastermind:** MM_02  
**TODO:** TODO_SELFHOSTING (TODO #1)  
**Tarih:** [Başlangıç] - [Bitiş]  
**Durum:** ✅ Tamamlandı  

---

## 📊 ÖZET

**Hedef:** Codegen ASM → C dönüşümü  
**Süre:** [Gerçek Süre] (Hedef: 2 hafta)  
**Başarı:** %100  

---

## ✅ TAMAMLANAN PHASE'LER

| Phase | YZ | Görev | Süre | Test | Durum |
|-------|-----|-------|------|------|-------|
| 0 | YZ_01 | ASM analizi | 1 gün | ✅ | Tamamlandı |
| 1 | YZ_02 | C codegen modülü | 2 gün | ✅ | Tamamlandı |
| ... | ... | ... | ... | ... | ... |

---

## 🎯 SONUÇLAR

**Teknik:**
- MLP artık C kodu üretiyor
- GCC ile derleme başarılı
- Bootstrap test: ✅
- Convergence test: ✅

# 🎯 MELP FELSEFESİ - ÖZET REHBERİ

**Tarih:** 4 Ocak 2026  
**Repository:** https://github.com/MELP-Lang/LLVM  
**Amaç:** Tüm MELP felsefe belgelerinin tek kaynak özeti  
**Kaynak:** ARCHITECTURE.md, kurallar_kitabı_v1.md, STO_PRINCIPLES.md, MLP MODÜL FELSEFESİ.md, MELP_REFERENCE.md, pmlp_sozdizimi.md

---

## 📖 MELP NEDİR?

**MELP = Multi-Language Programming (Melting Pot)**

Kullanıcı **istediği dilde** (Türkçe, İngilizce, Çince...) ve **istediği syntax'ta** (Python-style, C-style, custom) kod yazabilir.

### 🔄 Derleme Zinciri

```
Kullanıcı Kodu (Any Language + Any Syntax)
     ↓
[diller.json + syntax.json]      ← Kullanıcı özelleştirebilir
     ↓
Normalizer → Pragmatik MLP        ← Kullanıcı görmez
     ↓
Pragmatik MLP (PMPL)              ← COMPILER BURAYA BAKAR (English base)
     ↓
[Lexer → Parser]
     ↓
[STO: Smart Type Optimization]    ← Arka plan optimizasyonu
     ↓
[Codegen: C veya LLVM IR]
     ↓
GCC/LLVM → Binary
```

**Kritik Nokta:** Compiler sadece PMPL görür, dil/syntax abstraction katmanı tamamen ayrı!

---

## 🎯 7 TEMEL ESAS (ASLA İHLAL ETME!)

### 1️⃣ MODÜLER - Ayrı Sorumluluklar, Peer-to-Peer Network

**ESAS:** Merkezi orchestrator YASAK! Peer-to-peer network yapısı! SORUMLULUK kriterine göre modüller!

**⚠️ TERMİNOLOJİ:** "AUTONOMOUS" → "MODÜLER" (Rust/Python standardı)

**🎯 TEK KURAL: Merkezi Orchestrator YASAK!**

```
✅ MODÜLER = Ayrı SORUMLULUKLAR (decentralized)
✅ Modüller birbirini import edebilir (peer-to-peer)
✅ Derleme sırası otomatik çözülür (dependency graph)
✅ Single responsibility principle
✅ Dosya boyutu önemsiz (doğal olarak büyük veya küçük olabilir)

❌ Merkezi orchestrator dosya (main.mlp tüm modülleri kontrol ediyor)
❌ Monolitik yapı (tüm kod tek dosyada)
❌ Manuel derleme sırası (user sıraya koyuyor)
❌ Hierarchical kontrol yapısı

🎯 KRITER: Merkezi orchestrator var mı? Varsa YASAK!
```

**📚 SOMUT ÖRNEK: Arithmetic Modülü**

```
✅ DOĞRU YAPI (Peer-to-Peer):
lexer.cpp (ortak)
   ↓
arithmetic_parser.cpp → import lexer
   ↓
arithmetic_codegen.cpp → import arithmetic_parser, lexer

• Her modül kendi sorumluluğunu taşır
• Peer-to-peer import (ihtiyaç duyduğunu)
• Merkezi orchestrator YOK
→ MODÜLER! ✅


❌ YANLIŞ YAPI 1 (Generic içinde spesifik):
lexer.cpp (ortak)
   ↓
parser.cpp (arithmetic logic içinde)
   ↓
codegen.cpp (arithmetic logic içinde)

• Generic adlar ama spesifik logic
• Karışık sorumluluk
• Modüler değil
→ YANLIŞ! ❌


❌ YANLIŞ YAPI 2 (Merkezi Orchestrator):
main.cpp
  ├─ import lexer
  ├─ import parser
  ├─ import codegen
  ├─ import arithmetic
  └─ Hepsini orchestrate ediyor!

• main.cpp merkezi kontrol
• Tüm modüller main'e bağımlı
• Hierarchical yapı
→ MERKEZI ORCHESTRATOR! ❌
```

**✅ MODÜLER (Network Yapısı):**
```
token_types.mlp ←┐
                 ↓
XXX_parser.mlp ─────→ import token_types
                 ↓
XXX_codegen.mlp ────→ import parser, token_types

Her modül:
  • Kendi sorumluluğu var (tek bir kavram)
  • İhtiyacı olanı import eder (peer-to-peer)
  • Merkezi main.mlp YOK!
→ MODÜLER! ✅
```

**❌ MODÜLER DEĞİL (Hierarchical Yapı):**
```
main.mlp
  ├─ lexer import ediyor
  ├─ parser import ediyor
  ├─ codegen import ediyor
  └─ Hepsini orchestrate ediyor!

main.mlp:
  • Merkezi kontrol (herkesi yönetiyor)
  • Tüm modüller main'e bağımlı
  • Hierarchical yapı
→ MODÜLER DEĞİL! ❌
```

**Import İlkeleri:**
```
✅ parser.mlp → import token_types   (İzin verilen: ihtiyaç duyulan)
✅ codegen.mlp → import parser       (Peer-to-peer)
❌ main.mlp → import EVERYTHING      (Yasak: merkezi control)
```

### 2️⃣ GCC/LLVM - Backend Standardı

```
Stage 1: MLP → C → GCC → Binary
Stage 2: MLP → LLVM IR → LLVM → Binary
```

**Sabit Chain:** MLP her zaman C veya LLVM IR üretir, asla doğrudan assembly değil!

### 3️⃣ STO - Smart Type Optimization

**User görür (sadece 2 tip!):**
```mlp
numeric x = 42;
numeric y = 3,14;
string name = "Ali";
```

**Compiler optimize eder (user görmez):**
| Value | Internal | Location | Speed |
|-------|----------|----------|-------|
| 42 | int64 | register | ⚡ En hızlı |
| 3.14 | double | xmm reg | ⚡ Hızlı |
| 10^1000 | BigDecimal | heap | 🐢 Güvenli |
| "Ali" | SSO (inline) | stack | ⚡ En hızlı |
| "Uzun..." | heap ptr | heap | 🔄 Normal |

**Altın Kural:** User ASLA dahili tipleri görmemeli! (int64, double, BigDecimal user'a YASAK)

### 4️⃣ STATELESS - Durumsuzluk

```mlp
-- ❌ YANLIŞ (Global state):
numeric counter = 0;  -- Global YASAK!

function increment()
    counter = counter + 1  -- Side effect!
end_function

-- ✅ DOĞRU (Pure function):
function increment(counter: numeric) as numeric
    return counter + 1  -- Pure, predictable
end_function
```

**Kural:** 
- Global state YASAK
- Her fonksiyon pure (parametre → sonuç)
- Eğer mutlaka state gerek → `import state` (opt-in, performance penalty)

### 5️⃣ STRUCT+FUNC - OOP Yok

```mlp
-- ✅ İZİN VERİLEN:
struct Point
    numeric x
    numeric y
end_struct

function distance(p1: Point; p2: Point) as numeric
    return sqrt((p2.x - p1.x)^2 + (p2.y - p1.y)^2)
end_function

-- ❌ YASAK:
class Point {           -- Class yasak!
    method move() {     -- Method yasak!
    }
}
```

**Kural:** Sadece struct + function, class/inheritance/method YASAK!

### 6️⃣ MODÜL=ŞABLON - "Her Modül Ölüdür"

**Felsefe:** "Context loss önleme = Index kullanma"

**🎯 ZORUNLU: 500+ satır dosyalar için INDEX oluştur!**

```bash
# Index kontrolü (fonksiyon ismine göre!)
grep "^function my_function" melp/stage1/MODULE_INDEX.md

# Gerçek satır numarasını bul (line numbers shift!)
grep -n "^function my_function" melp/stage1/module.mlp
```

**Problem:** YZ'ler context tutamıyor (45 dakika kod tarama)
**Çözüm:** Index System = Permanent memory

**Index İçeriği:**
- Functions: Fonksiyon ismi + yaklaşık satır (~Line 200)
- Constants: Tip ve değer listesi
- Globals: Durum değişkenleri (varsa)
- Duplicates: Güvenlik analizi

**Benefit:**
- Query: 45 dakika → 10 saniye (45x hızlı!)
- Duplicate prevention: %100
- Context loss: Önlendi
- Cleanup: 2-3 saat → 0 saat

**⚠️ KRİTİK: PRIMARY KEY = Fonksiyon İsmi (satır değil!)**

**Neden?** Kod değişince satır numaraları kayar!

```
BEFORE: Line 200: function parse_if()
+50 satır eklendi
AFTER:  Line 250: function parse_if()  ← KAYDI!

✅ DOĞRU: grep -n "^function parse_if" file.mlp
❌ YANLIŞ: Index'teki satır numarasına git
```

**Kural:**
```
ÖNCE: grep ile fonksiyon var mı kontrol et
VAR ise: Kullan (duplicate yazma!)
YOK ise: Güvenle yaz
SONRA: Index regenerate (satır numaraları güncellensin)
```

**Index Files (500+ satır modüller için):**
- `melp/stage1/LEXER_INDEX.md` (753 satır)
- `melp/stage1/PARSER_INDEX.md` (1040 satır)
- `melp/stage1/CODEGEN_LLVM_INDEX.md` (906 satır)

**Anti-pattern:**
```
❌ Index okumadan kod yaz → Duplicate risk!
❌ Satır numarasına güven → Kod kaydı, yanlış yer!
❌ "Var mı?" için 45 dakika grep → Index kullan!
❌ Context loss her session → Tekrar tekrar aynı hata!
```

**Best Practice:**
```
✅ Index'te fonksiyon ismine bak (10 saniye)
✅ grep ile gerçek satırı bul (line shift safe!)
✅ Duplicate kontrol (automated)
✅ Güvenle yaz (context garanti)
✅ Cleanup gereksiz (prevention > cure)
```

**Kural:** Her modül çağrısı independent, state tutmaz, pure functional

---

### 7️⃣ SAO - Semantic-Aware Optimization

**Felsefe:** "Frontend'in semantic bilgisi = Backend'in optimization ipuçları"

**Tanım:**
- Frontend: Type safety, purity, ownership kanıtlar
- SAO Layer: Semantic facts → LLVM metadata
- Backend: Metadata'ya güvenerek aggressive optimize eder

**Örnek:**
```llvm
; Frontend proved: x is int64, no aliasing possible
!sao.sto = !{i1 true, !"type_proven", !"int64"}
!sao.borrow = !{i1 true, !"noalias_proven"}

; LLVM uses: Vectorize safely, eliminate bounds checks
```

**Benefit:** C-level performance + Rust-level safety

**Detay:** SAO_PRINCIPLES.md

---

### 8️⃣ MODÜL=ŞABLON - "Her Modül Ölüdür"

**Felsefe:** "Her modül ölüdür; onu çağıran diriltir ve öldürür"

```mlp
-- Her çağrı BAĞIMSIZ instantiate
content = file.read("test.txt")  
-- file: dirildi → open → read → close → ÖLDÜ

file.write("out.txt", content)   
-- file: YENİ instantiate, öncekinden HABERSİZ
-- file: dirildi → open → write → close → ÖLDÜ
```

**Kural:**
- Modül hiçbir şey HATIRLAMIYOR
- Her çağrı fresh instantiate
- State paylaşımı YOK
- Pure functional paradigm

**Anti-pattern:**
```mlp
-- ❌ YANLIŞ (persistent state):
db = database.connect("...")  -- Connection handle tutmak YASAK!
result1 = database.query(db, "SELECT...")
result2 = database.query(db, "SELECT...")

-- ✅ DOĞRU (her çağrı independent):
result1 = database.query("postgres://...", "SELECT...")
-- database: dirildi → connect → query → disconnect → ÖLDÜ
result2 = database.query("postgres://...", "SELECT...")
-- database: YENİ instantiate, öncekinden bağımsız
```

---

## 📝 PMPL (Pragmatik MLP) - Compiler'ın Gördüğü Dil

### Kritik Syntax Kuralları

#### 1. Parametre Ayırıcı: `;` (noktalı virgül)

**Neden?** MLP'te `3,14` ondalık sayıdır (Türk formatı: virgül = ondalık ayırıcı)

```pmpl
-- Fonksiyon çağrısı
result = func(42; true; "OK")

-- Array
numeric[] arr = [1; 2; 3;]

-- List
list data = (1; 2; 3;)

-- Tuple
tuple<> pos = <10; 20;>
```

#### 2. Blok Sonlandırıcılar: Tek Token, Alt Çizgili

```pmpl
-- ✅ DOĞRU:
end_if
end_while
end_for
end_function

-- ❌ YANLIŞ:
end if      -- İki token, YASAK!
end while
```

**Token Listesi:**
- `end_if`, `end_while`, `end_for`, `end_function`
- `end_struct`, `end_enum`, `end_switch`, `end_match`
- `end_try`, `end_debug`, `end_operator`

#### 3. If Kuralı: Mutlaka `then`

```pmpl
-- ✅ DOĞRU:
if x > 5 then
    return 1
end_if

-- ❌ YANLIŞ:
if x > 5        -- 'then' eksik!
    return 1
end_if
```

#### 4. Kontrol Flow

```pmpl
-- else_if (tek token)
if x > 100 then
    print("large")
else_if x > 50 then     -- else_if tek token!
    print("medium")
else
    print("small")
end_if

-- exit/continue (standalone veya explicit)
while true
    if condition then
        exit            -- Standalone: mevcut döngüden çık
    end_if
    if another then
        continue        -- Standalone: mevcut döngüye devam
    end_if
end_while

-- Veya explicit:
exit_while              -- While döngüsünden çık
continue_for            -- For döngüsüne devam
```

#### 5. Tipler

**User-facing (sadece bunlar!):**
```pmpl
numeric x = 42;          -- Tüm sayılar
string name = "Ali";     -- Tüm metinler (text synonym)
boolean flag = true;     -- true/false
```

**Koleksiyonlar:**
```pmpl
numeric[] arr = [1; 2; 3;]      -- Array (homojen, tip gerekli)
list data = (1; "x"; true;)     -- List (heterojen, tip yok)
tuple<> pos = <10; 20;>         -- Tuple (immutable)
```

**Türk Sayı Formatı:**
```pmpl
numeric x = 3,14;            -- 3.14 (virgül = ondalık)
numeric y = 1.000.000;       -- 1,000,000 (nokta = binlik ayırıcı)
numeric z = 126.555.852,36;  -- 126,555,852.36
```

---

## 🚀 BOOTSTRAP STRATEJİSİ

### Stage 0: Disposable Bootstrap (C)
- **Durum:** %90 tamamlandı
- **Dil:** C (monolithic, 7 ilkeyi ihlal ediyor)
- **Amaç:** Stage 1'i derlemek
- **Kader:** Self-hosting sonrası SİLİNECEK! 🗑️

### Stage 1: Self-Hosting Compiler (MLP)
- **Durum:** %5 (sadece iskelet)
- **Dil:** MLP (PMPL syntax)
- **Mimari:** Modüler (8 bağımsız modül)
- **Backend:** GCC (C code generation)
- **7 İlke:** TAM UYUMLU
- **Hedef:** Kendini derleyebilir

### Stage 2: Production Compiler (MLP)
- **Backend:** GCC + LLVM IR
- **Özellikler:** Optimization, advanced features
- **7 İlke:** TAM UYUMLU

### Bootstrap Süreci

```bash
# 1. Stage 0 ile Stage 1'i derle
./bin/pmlpc stage1/**/*.mlp -o stage1.c
gcc stage1.c -o melp_stage1

# 2. Stage 1 kendini derler (v2)
./melp_stage1 stage1/**/*.mlp -o stage1_v2.c
gcc stage1_v2.c -o melp_stage1_v2

# 3. v2 kendini derler (v3)
./melp_stage1_v2 stage1/**/*.mlp -o stage1_v3.c
gcc stage1_v3.c -o melp_stage1_v3

# 4. Verification (3-pass bootstrap)
diff melp_stage1_v2 melp_stage1_v3  # Aynı mı?

# 5. ✅ BAŞARILI! Stage 0'ı sil
tar -czf archive/stage0_final.tar.gz melp/stage0/
rm -rf melp/stage0/
git commit -m "🗑️ Remove Stage 0 (now self-hosting)"
```

---

## 🏗️ STAGE 1 MODÜLER MİMARİ

### Modül Yapısı

```
stage1/
├── lexer/              # ORTAK (tüm modüller kullanır)
│   ├── scanner.mlp     (<400 satır)
│   ├── tokenizer.mlp   (<450 satır)
│   ├── keywords.mlp    (<200 satır)
│   └── tokens.mlp      (<300 satır)
│
├── core/               # Core (expressions, statements, AST)
│   ├── parser/
│   │   ├── expressions.mlp  (<500 satır)
│   │   ├── statements.mlp   (<500 satır)
│   │   └── ast.mlp          (<400 satır)
│   └── codegen/
│       ├── c_emitter.mlp    (<500 satır)
│       └── optimizer.mlp    (<400 satır)
│
├── types/              # Type system
├── control_flow/       # if, while, for
├── functions/          # function declarations
├── io/                 # print, file I/O
├── errors/             # try-catch-finally
├── memory/             # ref, const, out
└── modules/            # import, export
```

**Kural:** Her modül single responsibility, peer-to-peer import, pure functions! (Satır sayısı serbest)

---

## 📊 STO (Smart Type Optimization) - Detaylı

### Compile-Time Analiz

```
function analyze_numeric(value):
    if value tam_sayı AND -2^63 ≤ value ≤ 2^63-1:
        return INT64          -- Register
    else if value ondalık AND digits ≤ 15:
        return DOUBLE         -- XMM register
    else:
        return BIGDECIMAL     -- Heap

function analyze_string(value):
    if is_constant(value):
        return RODATA_STRING  -- .rodata section
    else if length(value) ≤ 23:
        return SSO_STRING     -- Stack inline
    else:
        return HEAP_STRING    -- Heap pointer
```

### Runtime Overflow Handling

```mlp
numeric x = 9223372036854775807  -- Max int64
x = x + 1                         -- OVERFLOW!

-- Otomatik promote:
-- 1. Overflow detect
-- 2. x → BigDecimal
-- 3. İşlem devam
-- 4. User hiçbir şey farketmez
```

### Performance

**Target:** %95+ hand-written C/Rust performance

**Mevcut:**
- FFI overhead: ~2 cycles
- Typical function: 100+ cycles
- **Net overhead: <2%** ✅

**MELP = Rust performance!** (STO sayesinde)

---

## 🚨 STO İHLAL ÖRNEKLERİ (YAPMA!)

### ❌ İhlal 1: Dahili Tipleri User'a Açmak

```mlp
-- YANLIŞ!
function get_type(numeric x) as string
    if x is int64 then              -- ❌ int64 user'a görünmemeli!
        return "integer"
    else if x is double then        -- ❌ double user'a görünmemeli!
        return "float"
    end_if
end_function
```

### ❌ İhlal 2: Tip Dönüşümü Zorunluluğu

```mlp
-- YANLIŞ!
numeric x = to_int64(42)           -- ❌ User dönüşüm yapmamalı!
numeric y = to_double(3.14)        -- ❌ Compiler otomatik seçmeli!
```

### ❌ İhlal 3: Tip-Spesifik API

```mlp
-- YANLIŞ!
function add_integers(int64 a; int64 b) as int64     -- ❌ int64 yasak!
function add_floats(double a; double b) as double    -- ❌ double yasak!

-- DOĞRU!
function add(numeric a; numeric b) as numeric        -- ✅ Sadece numeric!
```

---

## ⚡ PERFORMANS & OPTİMİZASYON

### FFI Boundary Pattern

```c
// PUBLIC API - double (type safety)
double mlp_func(double param) {
    // IMMEDIATE CAST - 1 cycle
    int64_t real_param = (int64_t)param;
    
    // NATIVE INTEGER OPS
    int64_t result = real_param * 2;
    
    // RETURN - 1 cycle cast
    return (double)result;
}
```

**Cost:** 2 cycles (entry + exit)  
**Benefit:** Type safety + native performance

### Memory Layout

```c
// MELP = Rust memory layout
struct HashMap {
    HashEntry* entries;  // Heap pointer
    size_t capacity;     // Native int
    size_t size;         // Native int
};
```

**Cache:** MELP = Rust ✅  
**Memory:** MELP = Rust ✅

---

## 📚 10. HIZLI REFERANS

### Dosya Konumları

```
belgeler/
├── ARCHITECTURE.md         - Modüler mimari, YZ workflow
├── kurallar_kitabı_v1.md   - Kapsamlı Türkçe referans (1880 satır)
├── MELP_REFERENCE.md       - AI için comprehensive guide
├── STO_PRINCIPLES.md       - STO detayları
├── STO_PERFORMANCE.md      - Performance analysis
├── pmlp_sozdizimi.md       - PMPL syntax (1411 satır)
└── MLP MODÜL FELSEFESİ.md  - "Her modül ölüdür" felsefesi

pro_docs/
├── BOOTSTRAP_ROADMAP.md    - 3-stage bootstrap planı
├── STAGE1_ARCHITECTURE.md  - Stage 1 modüler mimari
├── LANGUAGE_MAPPINGS.md    - Cross-language syntax
└── SYNTAX_CONFLICTS.md     - Decimal separator analizi

TODO.md                      - Stage 1 development roadmap
```

### Temel Komutlar

```bash
# Stage 0 derle
make pmlpc

# Test
./bin/pmlpc test.pmpl -o test.c
gcc test.c -o test
./test

# Stage 1 geliştirme
cd melp/stage1/
# ... MLP ile kod yaz ...
```

### 7 İlke Checklist

```
[ ] Her dosya <500 satır? (İlke 1: AUTONOMOUS)
[ ] Backend GCC veya LLVM? (İlke 2: GCC/LLVM)
[ ] User sadece numeric/string görüyor? (İlke 3: STO)
[ ] Global state yok mu? (İlke 4: STATELESS)
[ ] Class kullanılmamış mı? (İlke 5: STRUCT+FUNC)
[ ] Modüller state tutmuyor mu? (İlke 6: MODÜL=ŞABLON)
[ ] SAO metadata eklendi mi? (İlke 7: SAO)
```

---

## 💡 SONUÇ

**MELP'in Özü:**
1. 🌍 **Multi-language/syntax** - Kullanıcı özgür
2. 🎯 **STO** - User basit, compiler akıllı
3. 🚫 **Stateless** - Pure, predictable
4. 📦 **Modular** - <500 satır, bağımsız
5. 💎 **MODÜL=ŞABLON** - Her çağrı fresh
6. ⚡ **Performance** - Rust seviyesi

**Hedef:** Self-hosting, production-ready, maintainable compiler!

---

**Bu dosya tüm MELP felsefesinin özeti. Detay için kaynak belgelere bakın.**

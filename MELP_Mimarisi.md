# MELP MİMARİSİ - AI Asistan Kuralları

**Son Güncelleme:** 20 Aralık 2025 (YZ_ÜA_02: Modül felsefesi eklendi)  
**Amaç:** Her AI asistanı bu dosyayı okuyup onaylamalıdır

---

## 🎯 TEMEL PRENSİPLER

MELP şu 5 temel prensip üzerine kuruludur:

1. **%100 Modüler** - Her feature ayrı modül (kendi parser + codegen)
2. **LLVM Backend** - Platform-independent, değiştirilmez
3. **Stateless** - Global state YASAK, parametre geçişi
4. **STO (Smart Type Optimization)** - Runtime'da otomatik optimizasyon
5. **Struct + Functions** - OOP YOK (Class/Inheritance yasak)

---

## 🧬 MODÜL FELSEFESİ (KRİTİK!)

> **"Her modül ölüdür; onu, çağıran modül diriltir ve öldürür."**

### Rust-Style Import Modeli

MELP, **Monomorphization + Tree Shaking** yaklaşımını kullanır:

```
┌─────────────────────────────────────────────────────────────┐
│   math.mlp (ÖLÜ ŞABLON)        main.mlp (CANLI)            │
│   ┌──────────────────┐         ┌─────────────────────┐     │
│   │ function add     │         │ import math         │     │
│   │ function sub     │    →    │                     │     │
│   │ function mul     │ SADECE  │ x = math.add(1; 2)  │     │
│   │ function div     │  ADD    │                     │     │
│   │ function squa    │ KOPYALA │ -- sub, mul, div,   │     │
│   │ (100 fonksiyon)  │         │ -- squa KOPYALANMAZ │     │
│   └──────────────────┘         └─────────────────────┘     │
│                                         ↓                   │
│                                 ┌─────────────────────┐     │
│                                 │ Final Binary:       │     │
│                                 │ - main()            │     │
│                                 │ - add() [inline]    │     │
│                                 │ (99 fonksiyon YOK!) │     │
│                                 └─────────────────────┘     │
└─────────────────────────────────────────────────────────────┘
```

### Temel Kurallar

| Özellik | Değer |
|---------|-------|
| **Modül** | Ölü şablon (disk'te tanımlar) |
| **Import** | Seçici kopyalama (sadece kullanılanlar) |
| **State** | YOK (stateless) |
| **Bellek sızıntısı** | İMKANSIZ |
| **GC gerekli** | HAYIR |

### Avantajlar (Zero-Cost Abstraction)

1. **Tree Shaking:** Kullanılmayan kod binary'ye dahil edilmez
2. **Inlining:** Fonksiyon çağrısı yerine kod gömülür
3. **Constant Folding:** LLVM sabit değerleri hesaplar
4. **No Call Overhead:** push/pop/call komutları elimine edilir
5. **No Shared State:** Her modül bağımsız, sızıntı imkansız

### ❌ YANLIŞ: Canlı API Modeli

```
-- YANLIŞ DÜŞÜNCE:
import math        -- math.dll belleğe yüklenir
math.add(1; 2)     -- Shared state'e erişilir
-- Risk: Bellek sızıntısı, race condition, GC gerekli
```

### ✅ DOĞRU: Ölü Şablon Modeli

```
-- DOĞRU DÜŞÜNCE:
import math        -- math.mlp parse edilir
math.add(1; 2)     -- add() kodu BURAYA kopyalanır
-- Sonuç: Zero overhead, no state, no leak
```

---

## 📁 MODÜLER YAPI (ÇOK ÖNEMLİ!)

### ✅ DOĞRU: Her Modül Bağımsız

```
modules/
├── functions/
│   ├── functions_parser.mlp   # Function'ları parse eder
│   └── functions_codegen.mlp  # Function'ları LLVM IR'a çevirir
├── variables/
│   ├── variables_parser.mlp
│   └── variables_codegen.mlp
├── control_flow/
│   ├── control_flow_parser.mlp
│   └── control_flow_codegen.mlp
└── ...
```

**Kural:** Her feature modülü kendi parser'ını + codegen'ini içerir!

**Referans:** Stage 0 C compiler'ı incele → `compiler/stage0/modules/`

### ❌ YANLIŞ: Merkezi Parser/CodeGen

```
parser.mlp        # ❌ Tüm parse işlemleri burada
codegen.mlp       # ❌ Tüm codegen burada
```

**Bu yapı YASAK!** Merkezi/monolitik yapı mimariye aykırıdır.

---

## 🚫 STATELESS KURALI

### Global State YASAK

```mlp
-- ❌ YANLIŞ:
list g_tokens
numeric g_current_pos

function parse() returns string
    token = g_tokens[g_current_pos]  -- Global kullanımı
end_function
```

```mlp
-- ✅ DOĞRU:
function parse(list tokens, numeric position) returns list
    token = tokens[position]  -- Parametre geçişi
    return [result, new_position]  -- Yeni state döndür
end_function
```

**Kural:** Mutable global değişkenler asla kullanılmaz!

---

## 📌 CONST DEĞİŞKENLER (Rust Modeli)

### Compile-Time Constants İzin Verilir

```mlp
-- ✅ İZİN VERİLİR: Immutable, compile-time
const numeric PI = 3.14159
const numeric T_FUNCTION = 1

function calculate_area(numeric radius) returns numeric
    return PI * radius * radius  -- Const kullanımı OK
end_function
```

**Kural:** 
- `const` = compile-time sabit, inline edilir
- Stateless'i ihlal etmez (immutable)
- Rust'taki `const` ile aynı mantık

**Fark:**
- ❌ `numeric x = 1` → Global variable (YASAK!)
- ✅ `const numeric X = 1` → Constant (İZİN VERİLİR)

---

## 🔧 STO (Smart Type Optimization)

Kullanıcı sadece `numeric` ve `string` kullanır, compiler runtime'da optimize eder:

```mlp
numeric x = 42                    -- Runtime → int64
numeric y = 999999999999999       -- Runtime → BigInt
numeric z = 3.14                  -- Runtime → BigDecimal (IEEE 754 YASAK!)
string name = "Hi"                -- Runtime → SSO (Small String Optimization)
string text = "Long text..."      -- Runtime → Heap allocated
```

**Kural:** Kullanıcı tip seçmez, STO otomatik karar verir!

**Detay:** `docs_tr/language/STO.md`

---

## 📝 PMPL SÖZDİZİMİ (Kısaca)

MELP kaynak kodu → PMPL (normalize) → Lexer → Parser → CodeGen

```mlp
-- PMPL örneği:
function add(numeric a, numeric b) returns numeric
    return a + b
end_function
```

**Detay:** 
- `docs_tr/PMPL_SOZDIZIMI.md`
- `docs_tr/language/data_types.md`

---

## 🚨 YASAKLAR

### ❌ Kesinlikle Yapılmaması Gerekenler

1. **Merkezi/Monolitik Dosya Oluşturma**
   - `parser.mlp` (tüm parsing burada) → YASAK
   - Test/concat için geçici oluşturabilirsin
   - İşin bitince SİL!

2. **Orchestrator Yaratma**
   - Modüller arası koordinasyon katmanı ekleme
   - Her modül bağımsız çalışmalı

3. **Global State Kullanımı**
   - Mutable global değişkenler
   - Shared memory yapıları

4. **OOP Yapıları**
   - Class/Inheritance (şimdilik yok)
   - Sadece Struct + Functions

5. **IEEE 754 Floating Point**
   - `float`, `double` YASAK
   - BigDecimal kullan

6. **Otomatik Merge/PR**
   - Asla `main`'e merge etme
   - Asla Pull Request açma

7. **Ana Dizine Test Dosyası** ⭐ YENİ!
   - Ana dizine (root) ASLA test dosyası oluşturma
   - `test_*.mlp`, `test_*.s`, `test_*.o` → Ana dizine YASAK!
   - Test dosyaları sadece `tests/` veya `modules/*/` içinde
   - **Kural:** Her modül kendi testini kendi dizininde tutar

---

## ✅ YZ BAŞLANGIÇ CHECKLİST

Her session başında ONAYLAMAN gereken kurallar:

```
[ ] NEXT_AI_START_HERE.md okudum
[ ] TODO.md okudum
[ ] Modüler yapıyı anladım (her modül = parser + codegen)
[ ] Stateless prensibi anladım (global state yasak)
[ ] Const = Rust modeli (compile-time, immutable)
[ ] Monolitik dosya oluşturmayacağım (geçici hariç)
[ ] Orchestrator yaratmayacağım
[ ] STO prensibini anladım (runtime optimization)
[ ] Struct + Functions kullanacağım (OOP yok)
[ ] Kendi dalımda çalışacağım: [görev]_YZ_[XX]
[ ] Merge/PR yapmayacağım
[ ] Session sonu NEXT_AI_START_HERE.md güncelleyeceğim
[ ] ⭐ Ana dizine test dosyası YAZMAYACAĞIM (modules/*/ içinde)

ONAYLIYORUM - YZ_[numaran]
```

---

## 🧪 TEST STRATEJİSİ (YZ_06 Eklentisi)

### Test Dosyası Konumu
- ✅ `compiler/stage1/modules/operators/test_operators.mlp`
- ✅ `compiler/stage1/modules/functions/test_functions.mlp`
- ❌ `/test_operators.mlp` (Ana dizin - YASAK!)
- ❌ `/compiler/test_operators.mlp` (Yanlış konum)

### Test Çalıştırma
**Mevcut Durum (Stage 1 geliştirme aşamasında):**
1. Test dosyaları YAZILIYOR ✅
2. Test dosyaları syntax olarak doğru ✅
3. Testler ÇALIŞTIRILAMIYOR ❌ (Stage 1 compiler henüz yok)

**Gelecek (Stage 1 tamamlanınca):**
1. Test dosyası yaz
2. Stage 0 ile Stage 1'i compile et (bootstrap)
3. Stage 1 ile testleri çalıştır
4. Sonuçları doğrula

**Standart Test Pattern:**
```
modules/my_module/
├── my_module_parser.mlp
├── my_module_codegen.mlp
├── test_my_module.mlp      ← Burası!
└── README.md
```

---

## 🔄 YZ İŞLEM PROTOKOLÜ

### Session Başı
1. `NEXT_AI_START_HERE.md` oku
2. `TODO.md` oku
3. Bu dosyayı oku ve ONAYLA
4. `git status` kontrol et
5. Kendi dalını oluştur: `git checkout -b [görev]_YZ_[XX]`

### Session Sırasında
- Mimari ihlal görürsen → DURDUR, kullanıcıya sor
- Test başarısız → DURDUR, kullanıcıya sor
- Otomatik düzeltme yapma → Kullanıcı onayı al
- Her değişiklik öncesi backup düşün

### Session Sonu
1. `git add .` ve `git commit -m "YZ_XX: [özet]"`
2. `git push origin [görev]_YZ_[XX]`
3. `NEXT_AI_START_HERE.md` güncelle:
   - Son durum
   - İlerleme yüzdesi
   - Sonraki görev
   - Kritik notlar
4. Session raporu oluştur: `YZ_XX_report.md`

---

## 📚 REFERANS DOSYALAR

**Mimari Dokümantasyon:**
- `docs_tr/PMPL_SOZDIZIMI.md` - PMPL syntax
- `docs_tr/language/STO.md` - Smart Type Optimization
- `docs_tr/language/data_types.md` - Data types

**Stage 0 Referans:**
- `compiler/stage0/ARCHITECTURE.md` - C compiler mimarisi
- `compiler/stage0/RULES.md` - C compiler kuralları
- `compiler/stage0/modules/` - Modüler yapı örneği

**YZ Sistemi:**
- `NEXT_AI_START_HERE.md` - Mevcut durum ve sonraki görev
- `TODO.md` - Task listesi
- `CONTRIBUTING.md` - Genel katkı rehberi

---

## 🎯 ÖZET (TL;DR)

**YAPMALISIN:**
✅ Modüler yapı (her feature = ayrı modül)
✅ Stateless (parametre geçişi)
✅ Const = Rust modeli
✅ Struct + Functions
✅ Kendi dalında çalış
✅ Her session NEXT_AI.md güncelle

**YAPMAMALISIN:**
❌ Merkezi parser/codegen
❌ Global state
❌ Monolitik dosyalar (geçici hariç)
❌ Orchestrator
❌ OOP (Class/Inheritance)
❌ Merge/PR

---

**Pipeline:**
```
User code → Normalize → PMPL → Lexer → Parser → CodeGen → LLVM IR → Binary
```

**Her modül bu pipeline'ı kendi feature'ı için çalıştırır!**

---

**MELP şampiyonlar ligine hazırlanıyor!** 🚀
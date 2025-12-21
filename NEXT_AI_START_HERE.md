# 🚀 MELP PROJECT - START HERE!

**Last Session:** YZ_205 (Package Structure System) - ✅ **TAMAMLANDI**  
**Date:** 22 Ocak 2025  
**Agent:** GitHub Copilot (Claude Sonnet 4.5)  
**Status:** 🎉 **YZ_205 COMPLETE!** - Package System Fully Functional

**⚠️ Project Status:**
- **Stage 0:** ✅ TAMAMLANDI (C Compiler) - **List (), Map {}, Optional ?, Generic <T> working! ✅**
- **Stage 1:** ✅ **BLOKER KALDIRILDI** - Modüller derlenebilir durumda!
- **LLVM Faz 1:** ✅ TAMAMLANDI (YZ_00-04, Production Ready!)
- **YZ_200:** ✅ **TAMAMLANDI** - List operations
- **YZ_201:** ✅ **TAMAMLANDI** - Map/Dictionary Type
- **YZ_202:** ✅ **TAMAMLANDI** - Optional Type (?, ??, null)
- **YZ_203:** ✅ **TAMAMLANDI** - Generic Types (Explicit `<T>`)
- **YZ_203.5:** ✅ **TAMAMLANDI** - Generic Type Inference
- **YZ_204:** ✅ **TAMAMLANDI** - Module Import/Export System 🚀
- **YZ_205:** ✅ **TAMAMLANDI** - Package Structure System 📦

---

## 🎉 YZ_205 TAMAMEN TAMAMLANDI! (1 Session - 22 Ocak 2025)

**Package System Fully Functional:**
- ✅ Package manifest parser: `package.mlp` format
- ✅ Dependency resolution: Circular detection, topological sort
- ✅ Build system: Directory creation, compilation orchestration
- ✅ Validation: Required fields, identifier format
- ✅ 17/17 tests passing (~2900 LOC)
- ✅ 3 phases complete: Parser, Resolver, Builder

**Working Example:**
```pmpl
package {
    name: "my_package"
    version: "1.0.0"
    entry_point: "main.mlp"
    dependencies: [
        { name: "base_lib", version: ">=1.0.0" }
    ]
}
```

**Commits:** 
- Phase 1: 0053c8ca (Parser - 5/5 tests)
- Phase 2: e495bebe (Resolver - 4/4 tests)
- Phase 3: 878677f3 (Builder - 8/8 tests)

**Next Task:** YZ_206 - Result Type Pattern (Result<T, E>, ? operator)

---

## 🎉 YZ_204 TAMAMEN TAMAMLANDI! (2 Sessions - 21 Aralık 2025)

**Module System Fully Functional:**
- ✅ Module declaration: `module math`
- ✅ Export tracking: `export function add()`
- ✅ Import system: `import math`
- ✅ Qualified calls: `math.add(10)`
- ✅ Name mangling: `math.add → math_add`
- ✅ Export accessibility check
- ✅ End-to-end test successful!

**Working Example:**
```pmpl
-- math_simple.mlp
module math_simple
export function double(numeric x) returns numeric
    return x + x
end_function

-- test_qualified.mlp
import math_simple
function main() returns numeric
    return math_simple.double(21)  -- Returns 42! ✅
end_function
```

**LLVM IR Output:**
```llvm
define i64 @main() {
entry:
    %tmp3 = call i64 @math_simple_double(i64 21)
    ret i64 %tmp3
}
```

**Details:** See `LLVM_YZ/YZ_204_TAMAMLANDI.md` for full report!

**Files:**
- `compiler/stage0/modules/import/module_declaration.{h,c}` ✅
- `compiler/stage0/modules/import/export_tracker.{h,c}` ✅
- `compiler/stage0/modules/import/namespace_resolver.{h,c}` ✅
- `compiler/stage0/modules/import/import.c` (enhanced) ✅
- `compiler/stage0/modules/arithmetic/arithmetic_parser.c` (enhanced) ✅
- `tests/modules/*.mlp` (13 test files) ✅

**Commits:**
- `be6763be`: Phase 1 - Module/Export
- `d12b5f33`: Phase 2.1 - Import connection
- `594ddf65`: Phase 2.2 - Qualified resolution ✅
- `139226fb`: Documentation complete

**Branch:** `module-system_YZ_204` - ✅ READY TO MERGE

---

## 🎯 SONRAKİ GÖREV: YZ_205

**Adın:** YZ_205  
**Görevin:** Package Structure  
**Branch:** Yeni branch oluştur: `package-structure_YZ_205`  
**Öncelik:** 🟡 Orta (proje organizasyonu)  
**Süre Tahmini:** 5 gün

### 📋 YZ_205 GÖREVLERİ:

**Hedef:** `package.mlp` manifest sistemi

**1️⃣ Package Definition (P1)**
- [ ] Package manifest parser: `package.mlp`
- [ ] Metadata fields: name, version, author
- [ ] Dependency list: `dependencies = ["stdlib:1.0"]`
- [ ] Entry point: `entry = "src/main.mlp"`
- [ ] Build configuration

**Test Case:**
```pmpl
package {
    name = "my_project",
    version = "1.0.0",
    author = "Developer",
    dependencies = ["stdlib:1.0"],
    entry = "src/main.mlp"
}
```

**2️⃣ Dependency Resolution (P1)**
- [ ] Parse dependency list
- [ ] Version checking
- [ ] Dependency graph
- [ ] Circular dependency detection

**3️⃣ Build System Integration (P2)**
- [ ] Output directory structure: `build/`, `dist/`
- [ ] Package verification
- [ ] Manifest validation

**Dosyalar (başlangıç):**
- `compiler/stage0/modules/package/package_parser.c`
- `compiler/stage0/modules/package/dependency_resolver.c`
- `tests/package/test_manifest.mlp`

**Referans:** `TODO_MODERN_LANGUAGE.md` Line 337-356

---

## 🎉 SON TAMAMLANAN: YZ_204 - Module Import/Export System

**✅ DURUM: TAMAMLANDI (21 Aralık 2025)**

**Module System Features:**
- ✅ Module declaration: `module math`
- ✅ Export tracking: `export function add()`
- ✅ Import loading: `import math`
- ✅ Qualified calls: `math.add(10)`
- ✅ Name mangling: `math.add → math_add`
- ✅ Export accessibility check
- ✅ Circular dependency detection
- ✅ Module caching

**Working Example:**
```pmpl
-- math_simple.mlp
module math_simple
export function double(numeric x) returns numeric
    return x + x
end_function

-- test_qualified.mlp
import math_simple
function main() returns numeric
    return math_simple.double(21)  -- Returns 42! ✅
end_function
```

**Stats:**
- 2 sessions (~2.5 hours)
- 19 files changed
- +787 lines of code
- 4 commits total

**Report:** `LLVM_YZ/YZ_204_TAMAMLANDI.md`

---

## 📚 REFERANSLAR

function main() returns numeric
    return math.add(10, 20)  -- Should return 30
end_function
```

**⚠️ ÖNCE KONTROL ET:**
```bash
ls -la compiler/stage0/modules/import/
# Import modülü var mı? Varsa baştan yazma, entegre et!
```

**Dosyalar:**
- `compiler/stage0/modules/import/` (zaten var mı kontrol et!)
- `compiler/stage0/modules/module/` (yeni modül gerekebilir - SOR!)
- `tests/modules/test_import.mlp`

**Süre Tahmini:** 1 hafta

**⚠️ MELP Prensipleri:**
- ✅ Mevcut `import/` modülünü kontrol et (baştan yazma!)
- 🚨 Yeni modül gerekirse: **KULLANICIYA SOR!**
- ✅ Keyword syntax: `import`, `export`, `module` (MELP-style)
- ✅ Namespace: `math.add()` (nokta notation)

**🆕 YENİ ÇALIŞMA ŞEKLİ (YZ_204'ten itibaren):**
- ✅ Kendi hızında çalış (günlük hedef yok)
- ✅ TODO'daki tüm maddeleri tamamla
- ⚠️ Yorulunca söyle ("Ara verelim")
- ✅ Kritik kararlarda sor (yukarıdaki liste)
- ✅ Bitince rapor yaz: `LLVM_YZ/YZ_204_TAMAMLANDI.md`

function test_inference() returns numeric
    return max(10, 20)  -- Should infer T = numeric, return 20
end_function
```

**Algoritma (genel):**
1. **Detect generic call without type args:** `identity(42)` (no `<...>`)
2. **Lookup template:** Find `identity<T>` in registry
3. **Analyze arguments:** `42` → type is `numeric`
4. **Infer type params:** `T = numeric`
5. **Call monomorphization:** `generic_instantiate("identity", "numeric")`
6. **Emit LLVM IR:** Reuse existing codegen

**Dosyalar:**
- `compiler/stage0/modules/type_system/type_inference.h` (yeni modül)
- `compiler/stage0/modules/type_system/type_inference.c` (inference engine)
- `compiler/stage0/modules/functions/functions_codegen_llvm.c` (extend)
- `tests/llvm/11_generics/test_inference.mlp`

**Süre Tahmini:** 5 gün (type inference algorithm)

**⚠️ MELP Prensipleri:**
- ✅ Yeni modül: `type_inference` → type_system içinde OK
- ✅ Mevcut monomorphization'ı kullan (baştan yazma!)
- ✅ STO uyumlu (numeric remains generic)
- ✅ Error messages okunabilir

---

## 🎉 SON TAMAMLANAN: YZ_202 - Optional Type (Null Safety)

**✅ DURUM: TAMAMLANDI**
- ✅ Lexer tokens (?, ??, null) - 3 tokens
- ✅ Variable nullable support (is_nullable, is_null flags)
- ✅ Runtime library (mlp_optional.h/c) - 193 satır
- ✅ Null literal parsing (TOKEN_NULL → INTERNAL_TYPE_POINTER)
- ✅ ?? operator (parse_coalesce_stateless) - right-associative
- ✅ LLVM IR codegen (llvm_emit_select + llvm_emit_icmp)
- ✅ Test suite (8 test files) - LLVM IR compiles successfully
- ✅ LocalVariable tracking (is_nullable flag)

**📊 Test Sonuçları:**
```bash
# Compilation tests:
./functions_compiler test_coalesce_simple.mlp test.s  # ✅ SUCCESS
./functions_compiler test_coalesce_var.mlp test.s     # ✅ SUCCESS
./functions_compiler test_minimal.mlp test.s          # ✅ SUCCESS

# Generated IR (example):
%is_null = icmp eq i64 %x, 0
%result = select i1 %is_null, i64 100, i64 %x

# Constant folding works:
0 ?? 100  →  optimized to: 100 ✅
```

**📋 Commit:**
- Branch: `optional-type_YZ_202`
- Commits: 7 commits (16329a2c → 2883bebd)
- Files changed: 15 files, ~500 satır yeni kod
- Key commit: `2883bebd` "YZ_202: ?? Operator - Full Implementation ✅"

**⚠️ Stage 1'e Ertelenen:**
- Optional chaining (`?.field`) - null_safety modülünde mevcut, entegrasyon gerekli
- Null assertion (`value!`) - null_safety modülünde mevcut
- Safe unwrapping (`if x is not none`) - parser extension gerekli
- Nullable collections (`list?`, `map?`) - type system gerekli

---

## 📜 YZ ÇALIŞMA PROTOKOLÜ (ZORUNLU!)

**Her YZ Agent'ı (YZ_XX) bu kuralları takip eder:**

### ⚠️ BAŞLAMADAN ÖNCE OKU (ZORUNLU!)
1. **`MELP_VISION.md`** ← MELP felsefesi (Kod = Düzyazı, STO, Modüler)
2. **`ARCHITECTURE.md`** ← 26 modül sistemi
3. **`melp_modulleri.md`** ← Mevcut modüller (baştan yazma!)
4. **`YZ_PROTOKOL.md`** ← Detaylı çalışma kuralları
5. **`TODO_MODERN_LANGUAGE.md`** ← Görev detayları

### 🛡️ MELP MİMARİ KURALLARI (İHLAL ETME!)

**Kritik Prensipler:**
- ✅ **Modüler Yapı:** 26 bağımsız modül, merkezi dosya YOK
- ✅ **MELP Felsefesi:** Kod = Düzyazı (`end_if` not `}`)
- ✅ **Keyword > Symbol:** `spread items` not `...`
- ✅ **STO Korunmalı:** `numeric` = compiler seçer (int64/double/BigDecimal)
- ✅ **Mevcut Kodu Kontrol Et:** `melp_modulleri.md` bak, baştan yazma!

**KULLANICI ONAYI GEREKTİREN DURUMLAR:**
- 🔴 Yeni modül oluşturma
- 🔴 Mevcut modülü yeniden yazma
- 🔴 Lexer'a yeni token (syntax değişikliği)
- 🔴 Type system değişiklikleri
- 🔴 Symbol vs keyword kararları
- 🔴 Merkezi resolver/manager dosyası

**Şüphen varsa sor:** "Bu karar MELP mimarisini etkiler mi?"

### 1️⃣ Görev Tespit
- ✅ **Önce yukarıdaki ZORUNLU OKUMA listesini oku!**
- ✅ `NEXT_AI_START_HERE.md` oku → Sıradaki görev senin görevin!
- ✅ Adın: **YZ_XX** (örnek: YZ_201, YZ_202...)
- ✅ Görev: `TODO_MODERN_LANGUAGE.md` içindeki sıradaki işaretlenmemiş görev
- ✅ `melp_modulleri.md` kontrol et → Modül var mı?
- ✅ Kullanıcıyı bilgilendir: "Ben YZ_XX'im, görevim [görev açıklaması]"

### 2️⃣ Çalışma Şekli (YZ_204'ten itibaren YENİ!)
- ✅ **Branş oluştur:** `TODO_gorevi_YZ_XX`
- ✅ **Kendi hızında çalış** (günlük hedef yok)
- ✅ **TODO'daki tüm maddeleri tamamla**
- ✅ Her anlamlı ilerlemeyi commit et
- ⚠️ **Yorulursan veya takılırsan: SÖYLEMELİSİN!** ("Ara verelim" de)
- ✅ Kritik kararlarda kullanıcıya sor (yukarıdaki liste)
- ✅ Bitince: `LLVM_YZ/YZ_XX_TAMAMLANDI.md` yaz

### 3️⃣ Görev Tamamlama
- ✅ **TODO güncelle:** `TODO_MODERN_LANGUAGE.md` içinde görevini `- [x]` olarak işaretle
- ✅ **LLVM_YZ rapor:** `LLVM_YZ/YZ_XX_TAMAMLANDI.md` dosyası oluştur:
  ```markdown
  # YZ_XX: [Görev Adı] ✅ TAMAMLANDI
  
  **Tamamlanma Tarihi:** [tarih]
  **Branch:** TODO_gorevi_YZ_XX
  **Commit:** [hash] "[mesaj]"
  
  ## 📊 Yapılanlar
  - [x] [İş 1]
  - [x] [İş 2]
  
  ## ✅ Test Sonuçları
  [test çıktıları]
  
  ## 🔧 Değiştirilen Dosyalar
  - path/to/file1.c
  - path/to/file2.h
  
  ## 🎯 Sonuç
  [Özet açıklama]
  ```

### 4️⃣ Sonraki YZ İçin Hazırlık
- ✅ **NEXT_AI güncelle:** `NEXT_AI_START_HERE.md` içinde:
  - Tamamlanan görevi ✅ olarak işaretle
  - Yeni görev (YZ_XX+1) için "SIRASKI GÖREV" bölümünü güncelle
  - Commit: `TODO_gorevi_YZ_XX` branch'ine
- ✅ **Kullanıcıyı bilgilendir:** Görevin tamamlandı, sonraki YZ_XX+1 hazır

### 5️⃣ Teslim
- ✅ Branch: `TODO_gorevi_YZ_XX` (merge etme, sadece commit at!)
- ✅ Dosyalar: Kod + TODO + NEXT_AI + LLVM_YZ raporu
- ✅ Kullanıcı onayı: "YZ_XX tamamlandı, sonraki YZ_XX+1 başlayabilir"

---

## 🚨 MEVCUT GÖREV: SEN YZ_201'SİN!

**Adın:** YZ_201  
**Görevin:** Map/Dictionary Type Implementation  
**Branch:** `TODO_gorevi_YZ_201` (henüz oluşturulmadı)  
**Kaynak:** `TODO_MODERN_LANGUAGE.md` Line 97-118

**⚠️ YZ Directory Structure:**
- `stage_0_YZ/` - Stage 0 sessions (YZ_01 - YZ_97) ✅ ARCHIVED
- `stage_1_YZ/` - Stage 1 sessions (YZ_01 - YZ_24) ✅ ARCHIVED
- `LLVM_YZ/` - LLVM backend sessions (YZ_00 - YZ_07) ✅ + YZ_200 ✅ TAMAMLANDI
- `ust_akil_YZ/` - Üst Akıl devir belgeleri (YZ_ÜA_01 - YZ_ÜA_07) ✅

---

## 🎉 SON TAMAMLANAN: YZ_200 - List Type Full Implementation

**✅ DURUM: TAMAMLANDI**
- ✅ Runtime library (mlp_list.h/c) - 7/7 tests passing
- ✅ List syntax: `list x = (1; 2; 3;)` working
- ✅ append(list; value) working
- ✅ length(list) working  
- ✅ List indexing: `numbers(i)` working (bitcast + load)
- ✅ Variable type tracking implemented
- ✅ Full integration test: 212 exit code (3+9+200) ✅

**📊 Test Sonuçları:**
```bash
# test_list_simple: exit 0 ✅
# test_list_append_simple: exit 1 ✅
# test_list_operations: exit 212 (3+9+200) ✅
```

**📋 Commit:**
- Branch: `stage0-list-syntax_YZ_06`
- Commit: `8349f47e` "YZ_200 TAMAMLANDI: List Operations Full Implementation ✅"
- Files changed: 10 files, 43 insertions

---

---

## 🎯 YZ_201 GÖREVE BAŞLAMA REHBERİ

**⚠️ ÖNEMLİ:** YZ Çalışma Protokolünü okumayı unutma!  
📜 **`YZ_PROTOKOL.md`** - 5 adımlık zorunlu protokol (350+ satır rehber)

### Adım 0: Protokolü Oku (3 dakika) ⭐
```bash
cat YZ_PROTOKOL.md | less
```
**İçerik:**
- 5 adımlık zorunlu protokol
- Template'ler (rapor, commit, bildirim)
- Test stratejileri
- Başarı kriterleri
- YZ_200 örnek akışı

### Adım 1: Branş Oluştur (1 dakika)
```bash
cd /home/pardus/projeler/MLP/MLP
git checkout -b TODO_gorevi_YZ_201
```

### Adım 2: Kullanıcıyı Bilgilendir (30 saniye)
```
Merhaba! Ben YZ_201. Görevim: Map/Dictionary Type Implementation.
TODO_MODERN_LANGUAGE.md Line 97-118'deki görevi tamamlayacağım.
Başlıyorum!
```

### Adım 3: Görevi Tamamla (4-5 gün)
- [ ] Runtime: `runtime/stdlib/mlp_map.{h,c}`
- [ ] Lexer: `{}` token support
- [ ] Parser: Map literal parsing
- [ ] Codegen: LLVM IR generation
- [ ] Tests: Full integration test

### Adım 4: Bitince TODO İşaretle
`TODO_MODERN_LANGUAGE.md` içinde:
```diff
- - [ ] **YZ_201** - Map/Dictionary Type (hash table with chaining)
+ - [x] **YZ_201** - Map/Dictionary Type (hash table with chaining) ✅
```

### Adım 5: Rapor Yaz
`LLVM_YZ/YZ_201_TAMAMLANDI.md` oluştur  
📄 Template: `YZ_PROTOKOL.md` veya `LLVM_YZ/YZ_200_TAMAMLANDI.md` (örnek)

### Adım 6: NEXT_AI Güncelle
Bu dosyayı (NEXT_AI_START_HERE.md) güncelle → YZ_202'ye hazırla  
📜 Nasıl yapılır: `YZ_PROTOKOL.md` Adım 5 (Sonraki YZ İçin Hazırlık)

**⚠️ Unutma:** Her adımın detayı `YZ_PROTOKOL.md` içinde!

---

## 📋 YZ_201 DETAYLI GÖREV TANIMI

**📍 Konum:** `TODO_MODERN_LANGUAGE.md` - Line 97-118

**Öncelik:** 🟡 Yüksek (stdlib için gerekli)  
**Süre Tahmini:** 5 gün  
**Zorluk:** Orta-Yüksek

**Yapılacaklar:**
- [ ] Map type definition (hash table)
- [ ] Map operations: insert, get, remove, has_key
- [ ] Map literals: `{"key": value}` - **DİKKAT:** List () ile çakışmaması için {} syntax
- [ ] Map iteration: `for key, value in map`
- [ ] Type safety: key/value type constraints
- [ ] LLVM IR codegen
- [ ] Collision handling: chaining

**Test Case:**
```pmpl
function test_map() returns string
    map person = {"name": "Alice"; "age": "30"; "city": "NYC"}
    return person["name"]  -- Should return "Alice"
end_function
```

**⚠️ Önemli Notlar:**
- Map literal syntax: `{}` (curly braces) - List'ten farklı!
- Key-value ayırıcı: `:` (colon)
- Element ayırıcı: `;` (semicolon, list ile tutarlı)
- Hash table implementation gerekli (runtime/stdlib/mlp_map.c)
- Collision handling: chaining method
- YZ_200 List'e benzer pattern: runtime → parser → codegen

**🔧 Implementation Plan:**
1. Runtime library: `runtime/stdlib/mlp_map.{h,c}`
   - Hash function (FNV-1a veya djb2)
   - Bucket array with chaining
   - Insert, get, remove, has_key operations
   
2. Lexer: `compiler/stage0/modules/lexer/lexer.c`
   - `{` `}` tokenization for map literals
   - `:` token for key:value pairs
   
3. Parser: `compiler/stage0/modules/arithmetic/arithmetic_parser.c`
   - Map literal parsing: `{"key": value; "key2": value2}`
   - Map access: `map["key"]`
   
4. Codegen: `compiler/stage0/modules/functions/functions_codegen_llvm.c`
   - Map allocation: `melp_map_create()`
   - Map operations: `melp_map_insert()`, `melp_map_get()`
   - Variable type tracking (like list)

**🎯 Başarı Kriteri:**
```bash
# Map creation and access
map data = {"x": 10; "y": 20}
numeric result = data["x"]  # Should return 10

# Map operations
insert(data; "z"; 30)
numeric len = length(data)  # Should return 3
boolean has = has_key(data; "y")  # Should return true
```

---

## 📚 KAYNAKLAR

### 🔴 ZORUNLU (İlk oku!)
- **`YZ_PROTOKOL.md`** ⭐ - YZ Çalışma Protokolü (350+ satır kapsamlı rehber)
- **`NEXT_AI_START_HERE.md`** - Bu dosya (Görev tespiti)
- **`TODO_MODERN_LANGUAGE.md`** - Görev detayları (YZ_200-220)

### Proje Dokümantasyonu
- **Architecture:** `ARCHITECTURE.md` (1400+ satır)
- **MELP Reference:** `MELP_REFERENCE.md` (eksiksiz syntax guide)
- **PMPL Syntax:** `pmlp_kesin_sozdizimi.md` (Türkçe, detaylı)
- **Module List:** `melp_modulleri.md` (66 modül, checkbox'lı)

### YZ İlerleme Takibi
- **`LLVM_YZ/YZ_200_TAMAMLANDI.md`** ⭐ - Rapor örneği (template)
- **LLVM_YZ/YZ_200_GOREV.md** - YZ_200 detaylı plan
- **LLVM_YZ/YZ_200_ILERLEME.md** - Progress raporu
- **LLVM_YZ/FAZ_1_TAMAMLANDI.md** - Faz 1 özeti

### Kritik Dosyalar (YZ_200 ile ilgili)
- `runtime/stdlib/mlp_list.h` - List API (165 lines)
- `runtime/stdlib/mlp_list.c` - List implementation (329 lines)
- `compiler/stage0/modules/functions/functions_codegen_llvm.c` - List codegen
- `compiler/stage0/modules/llvm_backend/llvm_backend.c` - Runtime declarations
- `tests/llvm/08_list/test_list_operations.mlp` - Full test

---

## 🎯 GENEL DURUM

### ✅ Tamamlanan Major Özellikler
1. **Core Language** (Variables, Functions, Control Flow)
2. **Data Types** (numeric, string, boolean, struct, enum, **list ✅**)
3. **LLVM Backend** (Full IR generation)
4. **Runtime Library** (STO, stdlib with lists)
5. **Import System** (Basic module support)

### 🚧 Aktif Geliştirme
- **YZ_201**: Map/Dictionary (planlama aşaması)
- **Integration**: async, lambda, memory, null_safety modülleri (yazılmış, entegre değil)

### 📝 Önümüzdeki Major Özellikler
- Map/Dictionary (YZ_201)
- Optional/Null Safety (YZ_202)
- Generics (YZ_203)
- Error Handling (YZ_206-207)
- Closures (YZ_208)
- GC (YZ_210)
- Threading (YZ_218)

---

## 🛠️ YARDIMCI KOMUTLAR
   - Trailing `;` desteği

2. **Parser:** `compiler/stage0/modules/functions/functions_parser.c`
   - List literal parsing: `(elem1; elem2; elem3;)`
   - Boş list: `()`
   - Return statement'da list literal
   - Function parameters'da `;` ayırıcı

3. **Codegen:** `compiler/stage0/modules/array/` veya yeni `modules/list/`
   - List allocation (LLVM IR)
   - Element initialization
## 🤖 YZ_07 HIZLI BAŞLANGIÇ (İLK 10 DAKİKA!)

**🎯 SEN YZ_07'SİN! Görevin: YZ_06'yı tamamlamak (Stage 0 List Syntax)**

### Adım 1: Durumu Anla (2 dakika)

```bash
# Mevcut durum ne?
./compiler/stage0/modules/functions/functions_compiler \
  modules/lexer_mlp/token.mlp temp/test.ll --backend=llvm
# Sonuç: 0 functions ❌ (parser error)
```

### Adım 2: Brief'i Oku (5 dakika)

1. **`LLVM_YZ/YZ_06_GOREV.md`** ← Ana görev brifingi (166 satır)
2. **`temp/YZ_06_GOREV_OZET.md`** ← Detaylı analiz (158 satır)
3. **`TODO_MODERN_LANGUAGE.md`** Faz 0 ← Bu görev neden kritik?

### Adım 3: Syntax Öğren (3 dakika)

- **`pmlp_kesin_sozdizimi.md`** line 640-642 → List syntax kuralları
- **`kurallar_kitabı.md`** line 857-898 → Collection types
---

## 🎯 SONRAKI GÖREV: YZ_200 - List Type Full Implementation

**Hedef:** Proper list allocation ve operations
**Dosya:** `TODO_MODERN_LANGUAGE.md` - Faz 0, Görev #1
**Tahmin:** 3-5 gün

**🚀 YZ_08 (veya YZ_200) Hızlı Başlangıç:**

### Adım 1: Durumu Anla (2 dakika)
```bash
# YZ_07 başarılı mı?
./functions_compiler modules/lexer_mlp/token.mlp temp/test.ll
# Beklenen: 8 functions ✅ (BAŞARILI!)
```

### Adım 2: Raporları Oku (5 dakika)
1. **`LLVM_YZ/YZ_07_TAMAMLANDI.md`** → Son görev raporu
2. **`TODO_MODERN_LANGUAGE.md`** → YZ_200 detayları
3. **`ust_akil_YZ/YZ_ÜA_07.md`** → Koordinatör notları
## 📋 Session Tamamlama Checklist (YZ_07 İçin!)

**YZ_07 bittiğinde yapılacaklar (15 dakika):**

1. **Başarı kriterini test et:**
   ```bash
   ./functions_compiler modules/lexer_mlp/token.mlp temp/test.ll
   # Beklenen: 6 functions ✅
   
   # Tüm Stage 1 modülleri test et
   for file in modules/**/*.mlp; do
     ./functions_compiler "$file" temp/test.ll || echo "FAIL: $file"
   done
4. **Bu dosyayı güncelle (NEXT_AI_START_HERE.md):**
   - Last Session → YZ_07
   - Status → ✅ BLOKER ÇÖZÜLDÜ! YZ_200 HAZIR
   - Project Status → Stage 1 derleniyor ✅

5. **Git commit & push:**
   ```bash
   git add compiler/stage0/ \
           LLVM_YZ/YZ_07_TAMAMLANDI.md \
           TODO_MODERN_LANGUAGE.md \
           NEXT_AI_START_HERE.md
   
   git commit -m "YZ_07: Stage 0 List Syntax Support - BLOKER ÇÖZÜLDÜ!

   ✅ Completed:
   - Lexer: List literal tokenization
   - Parser: List syntax parsing
   - Codegen: List allocation & init
   
   📊 Metrics:
   - 66 Stage 1 modules compile successfully
   - 580+ lines fixed
   - token.mlp: 6 functions ✅
   
   Status: CRITICAL BLOCKER RESOLVED ✅
   Next: YZ_200 (List Type Full Implementation)"
   
   git push origin stage0-list-syntax_YZ_06
   ```

6. **KULLANICIYA SOR:** "YZ_07 tamamlandı! Stage 1 bloker çözüldü 🎉 Sırada YZ_200 (List Operations). Devam edelim mi?"
   ```
5. **Stage 0 kodunu incele:**
   - `compiler/stage0/modules/lexer/lexer.c` → Tokenization
   - `compiler/stage0/modules/functions/functions_parser.c` → Parsing

### Normal YZ için Başlangıç (YZ_06 sonrası):

1. **Bu dosyayı oku** → Mevcut durum nedir?
2. **TODO dosyalarını oku:**
   - `TODO_MODERN_LANGUAGE.md` → Modern dil özellikleri
   - `TODO_LLVM_SUMMARY.md` → LLVM backend özet
   - `LLVM_YZ/TODO.md` → LLVM detaylı görevler
3. **Son raporları oku:**
   - `ust_akil_YZ/YZ_ÜA_05.md` → Son üst akıl raporu
   - `LLVM_YZ/FAZ_1_TAMAMLANDI.md` → Faz 1 özet

---

## 📋 Session Tamamlama Checklist

**Session bittiğinde yapılacaklar (15 dakika):**

1. **YZ raporu yaz:**
   - YZ_06 için: `LLVM_YZ/YZ_06_GOREV.md` + `LLVM_YZ/YZ_06_TAMAMLANDI.md`
   - Diğer görevler: `LLVM_YZ/YZ_XX_TAMAMLANDI.md`

2. **TODO dosyalarını güncelle:**
   - `TODO_MODERN_LANGUAGE.md` → YZ_06'u [x] işaretle
   - `TODO_LLVM_SUMMARY.md` → Özet güncelle (gerekirse)
   - `LLVM_YZ/TODO.md` → İlerleme %'sini güncelle (gerekirse)

3. **Bu dosyayı güncelle:**
   - Last Session → YZ_XX
   - Date → Bugünün tarihi
   - Status → Yeni durum
   - Project Status → Güncel durumları

4. **Ana dizini temizle:**
   ```bash
   rm -f test_*.mlp *.s *.o temp/*.s*
   ```

5. **Git commit & push:**
   ```bash
   git add .
   git commit -m "YZ_XX: Task Description"
   git push origin branch-name_YZ_XX
   ```

6. **KULLANICIYA SOR:** "Görev tamamlandı. Sırada ne yapmak istersin?"

---

## 🔧 Git Workflow

**📌 Dal İsimlendirme Kuralı:**
- Format: `task-description_YZ_XX`
- Görev adı kısa ve açıklayıcı (kebab-case)
- Sonunda mutlaka `_YZ_XX` eki (XX = session numarası)

**Örnekler (YZ_06 ve sonrası):**
- `stage0-list-syntax_YZ_06` (ACİL!)
- `llvm-array-support_YZ_05`
- `llvm-foreach-loop_YZ_06`
- `module-system_YZ_204`

**Git Commands:**
```bash
# 1. Yeni dal oluştur
git checkout -b stage0-list-syntax_YZ_06

# 2. Değişiklikleri ekle
git add compiler/stage0/modules/lexer/lexer.c \
        compiler/stage0/modules/functions/functions_parser.c \
        LLVM_YZ/YZ_06_*.md \
        TODO_MODERN_LANGUAGE.md \
        NEXT_AI_START_HERE.md

# 3. Commit et
git commit -m "YZ_06: Stage 0 List Syntax Support

✅ Completed Tasks:
- List literal tokenization
- List syntax parsing
- List codegen support

📊 Metrics:
- 66 Stage 1 modules now compile
- 580+ lines fixed
- All tests passing

Status: BLOCKER RESOLVED ✅"

# 4. Push et
git push origin stage0-list-syntax_YZ_06
```

**⚠️ ÖNEMLİ:** Session bittiğinde mutlaka TODO + NEXT_AI_START_HERE + YZ raporu + GIT COMMIT yapılmalı!

---

## 🚨 SYNTAX UYARISI - İLK OKUYACAĞIN ŞEY!

**Kullanıcı kodu (.mlp test/örnek):** Boşluklu → `end if`, `else if`, `end while`  
**Compiler kodu (parser.mlp, lexer.mlp):** Alt çizgili → `end_if`, `else_if`, `end_while`

**Hızlı Kural:** Örnekler için `end if`, modüller için `end_if`. Kararsızsan → `kurallar_kitabı.md` oku!

**⚠️ YZ_06 İçin Önemli:**
- List syntax: `(elem1; elem2; elem3;)` - Parantez + noktalı virgül
- Array syntax: `[elem1; elem2; elem3]` - Köşeli parantez
- Parametre ayırıcı: `;` (noktalı virgül, virgül DEĞİL!)
- Trailing semicolon: Gerekli! `(1; 2; 3;)`

---

## 📊 Project Status Details

### ✅ Completed Phases

**Stage 0 (C Compiler):**
- ✅ Basic syntax (functions, variables, if/while)
- ✅ LLVM backend integration
- ✅ Struct support
- ⚠️ **EKSIK:** List literal syntax `()` - YZ_06 ile eklenecek

**Stage 1 (Self-hosting):**
- ⚠️ **ENGELLENMIŞ:** 66 modül var ama derlenemiyor
- Neden: Stage 0'da list syntax yok
- Çözüm: YZ_06 tamamlanınca bootstrap devam edecek

**LLVM Backend (Faz 1):**
- ✅ YZ_00: LLVM IR alt yapı kurulumu
- ✅ YZ_01: Temel fonksiyon desteği
- ✅ YZ_02: If/While control flow
- ✅ YZ_03: Struct tanımlama ve kullanımı
- ✅ YZ_04: Stage 1 full compilation

### 🚨 Critical Blocker

**YZ_06: Stage 0 List Syntax Support (1 hafta)**
- **Öncelik:** 🔴🔴🔴 EN YÜKSEK
- **Süre:** 21-28 Aralık 2025
- **Etki:** 66 Stage 1 modül, 580+ satır
- **Detay:** `temp/YZ_06_GOREV_OZET.md`

### ⏳ Pending Phases (YZ_06 sonrası)

**Modern Language Features (19 hafta):**
- Faz 1: List operations (YZ_200-203) - 4 hafta
- Faz 2: Module system (YZ_204-208) - 5 hafta
- Faz 3: Error handling (YZ_209-215) - 7 hafta
- Faz 4: Advanced features (YZ_216-224) - 9 hafta

**LLVM Optional Features (ihtiyaç oldukça):**
- YZ_05: Array support
- YZ_06: Foreach loop
- YZ_07-13: Diğer features

---

## 📚 Key Documents

### YZ_06 İçin Gerekli Belgeler:

**1. Görev Belgeleri:**
- `temp/YZ_06_GOREV_OZET.md` - Detaylı görev brifingi
- `TODO_MODERN_LANGUAGE.md` - Faz 0 açıklaması

**2. Syntax Referansları:**
- `pmlp_kesin_sozdizimi.md` - Line 170, 640-642 (list syntax)
- `kurallar_kitabı.md` - Line 857-898 (collection types)

**3. Analiz Belgeleri:**
- `temp/MODÜL_ANALIZ_KARŞILAŞTIRMA.md` - 66 modül analizi
- `modules/lexer_mlp/lexer.mlp` - List kullanım örneği
- `modules/lexer_mlp/token.mlp` - Test modülü

**4. Stage 0 Kaynak Kodları:**
- `compiler/stage0/modules/lexer/lexer.c` - Tokenization
- `compiler/stage0/modules/functions/functions_parser.c` - Parsing
- `compiler/stage0/modules/codegen/` - Code generation

### Genel Belgeler:

**Architecture:**
- `ARCHITECTURE.md` - Sistem mimarisi
- `MELP_VISION.md` - Proje vizyonu
- `MELP_REFERENCE.md` - Dil referansı

**TODO Files:**
- `TODO_MODERN_LANGUAGE.md` - Modern dil özellikleri TODO
- `TODO_LLVM_SUMMARY.md` - LLVM backend özet
- `LLVM_YZ/TODO.md` - LLVM detaylı TODO

**Stage 0:**
- `STAGE0_LIMITATIONS_DETAILED.md` - Stage 0 kısıtları
- `STAGE0_SIGNOFF.md` - Stage 0 onay belgesi

**LLVM:**
- `docs/LLVM_IR_GUIDE.md` - LLVM IR kılavuzu
- `LLVM_YZ/FAZ_1_TAMAMLANDI.md` - Faz 1 özet

**YZ Reports:**
- `ust_akil_YZ/YZ_ÜA_05.md` → Son üst akıl raporu (şu an yazılacak)
- `LLVM_YZ/YZ_04_TAMAMLANDI.md` - Son görev raporu

---

## 🎯 Success Criteria (YZ_06)

**Test 1: Token Module (minimal test)**
```bash
./compiler/stage0/modules/functions/functions_compiler \
  modules/lexer_mlp/token.mlp temp/test.ll --backend=llvm
# Beklenen: 6 functions compiled ✅
# Şu an: 0 functions compiled ❌
```

**Test 2: Lexer Module (full test)**
```bash
./compiler/stage0/modules/functions/functions_compiler \
  modules/lexer_mlp/lexer.mlp temp/test.ll --backend=llvm
# Beklenen: 19 functions compiled ✅
# Şu an: 0 functions compiled ❌
```

**Test 3: All Stage 1 Modules**
```bash
for file in modules/**/*.mlp; do
  echo "Testing: $file"
  ./functions_compiler "$file" temp/test.ll --backend=llvm
done
# Beklenen: Tüm modüller başarıyla derlenmeli
```

**Test 4: List Syntax Test Cases**
```bash
# Test empty list
echo 'function test() returns list
  return (;)
end_function' > temp/test_list.mlp

# Test single element
echo 'function test() returns list
  return ("hello";)
end_function' > temp/test_list.mlp

# Test multiple elements
echo 'function test() returns list
  return (1; "two"; 3.0;)
end_function' > temp/test_list.mlp
```

---

## 💡 YZ_06 Implementation Tips

**1. Lexer Changes (lexer.c):**
```c
// TOKEN_LPAREN ve TOKEN_RPAREN tanımla
// List literal detection: 
//   - '(' sonrası identifier veya literal geliyorsa list
//   - ';' ayırıcı olarak kullan
**⚠️ SON UYARI:** YZ_07 en yüksek öncelikli görev! Stage 1 bootstrap tamamen buna bağlı!

**🎯 HEDEF:** 66 Stage 1 modül derlenebilir hale gelsin → Bootstrap devam etsin → TODO_MODERN_LANGUAGE.md başlasın!

**🚀 BAŞARININ ANAHTARI:** 
- List syntax `()` (parantez, köşeli parantez DEĞİL!)
- Parametre ayırıcı `;` (noktalı virgül, virgül DEĞİL!)
- Trailing semicolon zorunlu: `(1; 2; 3;)`

**📊 İlerleme Takibi:**
- Lexer → Parser → Codegen → Test
- Her aşamada `token.mlp` test et (0 → 2 → 4 → 6 functions)

---

**Last Updated:** 21 Aralık 2025, 15:45 (YZ_ÜA_07)  
**Current Session:** YZ_07 (Stage 0 List Syntax Support - DEVAM)  
**Next Session:** YZ_08 veya YZ_200 (YZ_07 tamamlandıktan sonra)  
**Priority:** 🔴🔴🔴 CRITICAL BLOCKER

**3. Codegen Changes:**
```c
// List allocation ve initialization
// - malloc ile memory ayır
// - Element'leri kopyala
// - List metadata oluştur (size, capacity)
```

**4. Test Strategy:**
- Önce token.mlp ile minimal test
- Sonra lexer.mlp ile tam test
- Son olarak tüm 66 modül

---

## 🚀 Next Steps After YZ_06

1. **Stage 1 Bootstrap:**
   - 66 modül derlenecek
   - Stage 1 self-hosting tamamlanacak

2. **Modern Language Features:**
   - YZ_200: List operations (append, prepend, length)
   - YZ_201: Array type full implementation
   - YZ_204: Module system (import/export)

3. **LLVM Optional Features:**
   - YZ_05: Array support
   - YZ_06: Foreach loop
   - YZ_07-13: Diğer features

---

## 📞 Help & Support

**Problem yaşarsan:**
1. `kurallar_kitabı.md` kontrol et
2. `pmlp_kesin_sozdizimi.md` oku
3. `compiler/stage0/` kaynak kodlarını incele
4. Test durumunda: `temp/` klasöründeki test dosyalarına bak
5. Kullanıcıya danış

**Kritik Dosyalar:**
- Syntax: `pmlp_kesin_sozdizimi.md`, `kurallar_kitabı.md`
- TODO: `TODO_MODERN_LANGUAGE.md`
- Test: `temp/YZ_06_GOREV_OZET.md`
- Stage 0: `compiler/stage0/modules/`

---

**⚠️ SON UYARI:** YZ_06 en yüksek öncelikli görev! Stage 1 bootstrap tamamen buna bağlı. 1 hafta içinde tamamlanmalı!

**🎯 HEDEF:** 66 Stage 1 modül derlenebilir hale gelsin → Bootstrap devam etsin → Modern dil features eklensin!

**🚀 BAŞARININ ANAHTARI:** List syntax `()` + noktalı virgül `;` + trailing semicolon!

---

**Last Updated:** 21 Aralık 2025, 14:30 (YZ_ÜA_05)  
**Next Session:** YZ_06 (Stage 0 List Syntax Support)  
**Priority:** 🔴🔴🔴 CRITICAL BLOCKER

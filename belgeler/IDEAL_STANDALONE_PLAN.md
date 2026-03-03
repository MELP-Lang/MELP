# IDEAL Standalone Plan - Copy & Extend Strategy

**Tarih:** 24 Ocak 2026  
**Durum:** 🚀 Planlanıyor  
**Amaç:** IDEAL modülünü tamamen standalone yapmak

---

## 🎯 Hedef: Zero Dependency IDEAL

**Mevcut Durum (YANLIŞ):**
```
compiler/ideal/modules/
├─ arrays/parser_array.cpp    → #include "../../../TC/..."  ❌
├─ parser/parser_elseif.cpp   → #include "../../../TC/..."  ❌
├─ structs/
└─ enums/

→ TC'ye bağımlı! Başka proje alamaz!
```

**Hedef Durum (DOĞRU):**
```
compiler/ideal/
├─ modules/
│   ├─ lexer/           (BASE'den kopyalandı)
│   ├─ parser/          (BASE'den kopyalandı)
│   ├─ control_flow/    (BASE'den kopyalandı + else_if eklendi)
│   ├─ arrays/          (YENİ - IDEAL feature)
│   ├─ structs/         (YENİ - IDEAL feature)
│   ├─ enums/           (YENİ - IDEAL feature)
│   ├─ string/          (BASE'den kopyalandı + concat eklendi)
│   ├─ io/              (BASE'den kopyalandı)
│   ├─ codegen/         (BASE'den kopyalandı + extends)
│   └─ runtime/         (BASE'den kopyalandı)
├─ CMakeLists.txt       (Standalone build)
└─ main.cpp             (IDEAL compiler entry point)

→ Zero dependency! Copy-paste edilebilir!
```

---

## 📋 İşlem Adımları

### Phase 1: BASE'den Copy (2-3 saat)

```bash
# 1. BASE modüllerini IDEAL'e kopyala
cp -r compiler/base/modules/lexer compiler/ideal/modules/
cp -r compiler/base/modules/io compiler/ideal/modules/
cp -r compiler/base/modules/string compiler/ideal/modules/

# 2. TC'den gerekli core modülleri kopyala
cp -r compiler/TC/modules/parser compiler/ideal/modules/
cp -r compiler/TC/modules/codegen compiler/ideal/modules/
cp -r compiler/TC/modules/runtime compiler/ideal/modules/

# 3. Namespace'leri güncelle
# base:: → ideal::
# tc:: → ideal::
```

### Phase 2: Control Flow Refactor (4-6 saat)

```
1. TC/parser.cpp'deki if/else/while/for'u ayır
2. ideal/modules/control_flow/ oluştur
3. Files:
   - if_stmt.cpp       (if/else parsing)
   - elseif_stmt.cpp   (else_if parsing)
   - while_stmt.cpp    (while parsing)
   - for_stmt.cpp      (for parsing)
4. Full else_if implementasyonu (3+ levels)
```

### Phase 3: IDEAL Features (12-16 saat)

```
1. Arrays (arrays/)
   - parser_array.cpp
   - codegen_array.cpp
   - array_runtime.cpp

2. Structs (structs/)
   - parser_struct.cpp
   - codegen_struct.cpp
   - struct_runtime.cpp

3. Enums (enums/)
   - parser_enum.cpp
   - codegen_enum.cpp
   - enum_runtime.cpp

4. String Concat (string/)
   - Extend BASE string module
   - operator+ for strings
```

### Phase 4: Build System (2-3 saat)

```
1. compiler/ideal/CMakeLists.txt
   - Standalone build
   - Zero external dependencies
   - Output: ideal_compiler binary

2. Update #include paths
   - Remove ../../../../TC
   - Use local paths: modules/lexer/...

3. Update namespaces
   - namespace melp::ideal
   - All modules under ideal::
```

### Phase 5: Testing (4-6 saat)

```
1. Copy BASE tests → ideal/tests/
2. Add IDEAL-specific tests:
   - test_elseif.cpp
   - test_arrays.cpp
   - test_structs.cpp
   - test_enums.cpp
3. Run full test suite
4. Valgrind memory check
```

---

## ⏱️ Timeline Estimation

| Phase | Duration | Output |
|-------|----------|--------|
| Copy BASE modules | 2-3h | Core modules copied |
| Control flow refactor | 4-6h | Modular control flow |
| IDEAL features | 12-16h | Arrays, structs, enums |
| Build system | 2-3h | Standalone compilation |
| Testing | 4-6h | 200+ tests passing |
| **TOTAL** | **24-34h** | **3-4 days** |

---

## 🚨 Kritik Noktalar

### 1. Zero Dependency Rule
```cpp
// ❌ YANLIŞ
#include "../../../TC/modules/lexer/token.hpp"
#include "../../../base/modules/string/string_ops.hpp"

// ✅ DOĞRU
#include "modules/lexer/token.hpp"
#include "modules/string/string_ops.hpp"
```

### 2. Namespace Consistency
```cpp
// Tüm IDEAL modülleri:
namespace melp {
namespace ideal {

// ...

} // namespace ideal
} // namespace melp
```

### 3. No Cross-Module References
```
✅ ideal/modules/arrays → ideal/modules/lexer   (aynı klasör)
❌ ideal/modules/arrays → base/modules/lexer    (dış bağımlılık)
❌ ideal/modules/arrays → TC/modules/lexer      (dış bağımlılık)
```

---

## 📦 Standalone Test

**Kullanıcı test senaryosu:**
```bash
# Bir kullanıcı sadece IDEAL'i alıyor
cd /tmp
cp -r /path/to/compiler/ideal ./my_ideal_compiler

# Build (hiç bağımlılık yok!)
cd my_ideal_compiler
mkdir build && cd build
cmake ..
make

# Kullanım
./ideal_compiler test.mlp -o test

# Başarı kriteri: Zero error, no missing dependencies
```

---

## ✅ Başarı Kriterleri

- [ ] IDEAL klasörü TC'den bağımsız
- [ ] IDEAL klasörü BASE'den bağımsız
- [ ] `#include "../../../TC` hiçbir yerde yok
- [ ] `#include "../../../base` hiçbir yerde yok
- [ ] Standalone build (cmake && make) çalışıyor
- [ ] 200+ test passing
- [ ] Valgrind clean (0 memory leaks)
- [ ] Copy-paste test başarılı (/tmp'de build ediliyor)

---

## 🔗 İlgili Belgeler

- [BOOTSTRAP_STRATEGY.md](BOOTSTRAP_STRATEGY.md) - Copy & Extend stratejisi
- [3-TODO_IDEAL_FEATURES.md](../3-TODO_IDEAL_FEATURES.md) - IDEAL TODO
- [MELP_PHILOSOPHY.md](MELP_PHILOSOPHY.md) - Modüler mimari prensipleri

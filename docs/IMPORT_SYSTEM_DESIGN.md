# MELP Import System Design

**YZ_01 - Import Tasarımı ve Analiz**  
**Tarih:** 17 Aralık 2025  
**Durum:** ✅ Stage 0'da zaten var, MELP için spesifikasyon hazırlanıyor

---

## 🎯 GENEL BAKIŞ

MELP'te modüler yapı için **import sistemi zorunludur**. Stage 0 C compiler zaten import modülüne sahip. Bu döküman:
1. Mevcut Stage 0 import sistemini analiz eder
2. MELP (Stage 1) için import syntax'ını tanımlar
3. Implementation planını belirler

---

## 📊 MEVCUT DURUM - STAGE 0 ANALIZI

### Stage 0 Import Modülü

**Konum:** `compiler/stage0/modules/import/`

**Dosyalar:**
- `import.h` - Import statement yapısı
- `import.c` - Module path resolution, loading
- `import_parser.h/c` - Import statement parsing
- `import_cache.h/c` - Module caching (aynı dosya 1 kez import)
- `import_cache_persist.h/c` - Cache persistence

### Mevcut Import Syntax (C Implementation)

```c
// import.h - ImportStatement structure
typedef struct ImportStatement {
    char* module_name;              // "math", "string_utils"
    char* module_path;              // "modules/core/math.mlp"
    int is_resolved;                // Path resolved?
    FunctionDeclaration* functions; // Functions from module
    struct ImportStatement* next;   // Linked list
} ImportStatement;
```

### Module Resolution Strategy

**Search Order:**
1. `./modules/core/[module_name].mlp`
2. `./modules/advanced/[module_name].mlp`
3. `./modules/experimental/[module_name].mlp`
4. `./[module_name].mlp` (current directory)

**Örnek:**
```
import "math"
→ Arama: modules/core/math.mlp ✅
```

---

## 📝 MELP IMPORT SYNTAX TASARIMI

### Syntax Specification

```mlp
-- Basit import (module ismi)
import "module_name"

-- Relative path import
import "../core/utils"
import "./local_module"

-- Absolute path (opsiyonel, önerilmez)
import "/full/path/to/module.mlp"
```

### Örnekler

```mlp
-- Core utilities
import "char_utils"      -- modules/core/char_utils.mlp
import "token_types"     -- modules/core/token_types.mlp
import "type_mapper"     -- modules/core/type_mapper.mlp

-- Feature modules
import "functions_parser"   -- modules/functions/functions_parser.mlp
import "variables_codegen"  -- modules/variables/variables_codegen.mlp

-- Relative import (aynı feature içinde)
import "./functions_codegen"  -- Aynı dizinde
import "../core/utils"        -- Üst dizin
```

### Symbol Access

**Varsayılan:** Tüm public fonksiyonlar import edilir

```mlp
-- math.mlp içinde:
function add(numeric a, numeric b) returns numeric
    return a + b
end_function

function multiply(numeric a, numeric b) returns numeric
    return a * b
end_function

-- main.mlp içinde:
import "math"

function main() returns numeric
    numeric result = add(10, 20)        -- math.add() çağrısı
    numeric result2 = multiply(5, 6)    -- math.multiply() çağrısı
    return result
end_function
```

**Namespace (Opsiyonel, İleri Aşama):**
```mlp
-- Gelecekte desteklenebilir
import "math" as m

numeric result = m.add(10, 20)
```

---

## 🔧 IMPLEMENTATION PLANI

### Faz 1: Lexer (YZ_02)

**Görev:** `TOKEN_IMPORT` ekle

```c
// lexer.h
typedef enum {
    // ... mevcut tokenlar
    TOKEN_IMPORT,     // "import" keyword
    // ...
} TokenType;
```

**Değişecek Dosyalar:**
- `compiler/stage0/modules/lexer/lexer.h`
- `compiler/stage0/modules/lexer/lexer.c`
  - Keyword tablosuna `import` ekle

**Test:**
```mlp
import "test"
-- Lexer output: TOKEN_IMPORT, TOKEN_STR_LIT("test")
```

### Faz 2: Parser (YZ_02)

**Görev:** Import statement parsing

**AST Node (zaten var):**
```c
typedef struct ImportStatement {
    char* module_name;
    char* module_path;
    int is_resolved;
    FunctionDeclaration* functions;
    struct ImportStatement* next;
} ImportStatement;
```

**Parser Fonksiyonu (zaten var):**
```c
// import_parser.c
ImportStatement* parse_import_statement(Lexer* lexer);
```

**Değişecek Dosyalar:**
- `compiler/stage0/modules/import/import_parser.c` (zaten var, güncelle)

**Test:**
```mlp
import "math"
-- AST: ImportStatement { module_name: "math", is_resolved: 0 }
```

### Faz 3: Module Resolution (YZ_03)

**Görev:** File resolution ve symbol table

**Module Resolution (zaten var):**
```c
// import.c
char* import_resolve_module_path(const char* module_name);
```

**Search Order:**
1. `modules/core/[name].mlp`
2. `modules/[name]/[name]_parser.mlp` (feature modül)
3. `modules/advanced/[name].mlp`
4. `./[name].mlp`

**Symbol Loading (zaten var):**
```c
FunctionDeclaration* import_load_module(const char* module_path);
```

**Cache System (zaten var):**
```c
// import_cache.c
// Aynı dosya birden fazla kez import edilmez
```

**Circular Dependency Detection:**
```c
// import_cache.c içine eklenecek
int import_check_circular(const char* module_path, 
                          ImportStatement* current_imports);
```

**Değişecek Dosyalar:**
- `compiler/stage0/modules/import/import.c`
- `compiler/stage0/modules/import/import_cache.c`

**Test:**
```mlp
-- a.mlp
import "b"

-- b.mlp
import "a"  -- ❌ Circular dependency detected!
```

### Faz 4: CodeGen (YZ_04)

**Görev:** LLVM IR generation

**İki Yaklaşım:**

#### Yaklaşım A: Header-Only (C benzeri)
```llvm
; Import edilen modül sadece declare (external)
declare i64 @add(i64, i64)  ; math.mlp'den

define i64 @main() {
    %1 = call i64 @add(i64 10, i64 20)
    ret i64 %1
}
```

#### Yaklaşım B: Full Inline (tercih edilen)
```llvm
; Import edilen modülün tüm kodu inline edilir
define i64 @add(i64 %a, i64 %b) {
    %1 = add i64 %a, %b
    ret i64 %1
}

define i64 @main() {
    %1 = call i64 @add(i64 10, i64 20)
    ret i64 %1
}
```

**Tercih:** Yaklaşım B (inline) - LLVM optimizer daha iyi çalışır

**Module Assembly Generation (zaten var):**
```c
// import.c
int import_generate_module_assembly(const char* module_path, 
                                     FunctionDeclaration* functions);
```

**Değişecek Dosyalar:**
- `compiler/stage0/modules/import/import.c`
- `compiler/stage0/compiler.c` (main pipeline entegrasyonu)

**Test:**
```bash
# Compile
./stage0 main.mlp -o main
./main
# Output: 30
```

---

## 🔄 MODULE LOADING FLOW

```
User code: import "math"
    ↓
1. Lexer → TOKEN_IMPORT, TOKEN_STR_LIT("math")
    ↓
2. Parser → ImportStatement { module_name: "math" }
    ↓
3. Resolver → module_path: "modules/core/math.mlp"
    ↓
4. Cache Check → Already loaded? Yes → Reuse, No → Continue
    ↓
5. Load Module → Parse math.mlp → FunctionDeclaration list
    ↓
6. Symbol Table → Add functions to global scope
    ↓
7. CodeGen → Inline math.mlp IR into main.ll
    ↓
8. Output → main.ll (tüm kod birleştirilmiş)
```

---

## 🚨 ERROR HANDLING

### Error Types

1. **Module Not Found**
```
Error: Module 'math' not found
Searched in:
  - modules/core/math.mlp
  - modules/advanced/math.mlp
  - ./math.mlp
```

2. **Circular Dependency**
```
Error: Circular import detected
Import chain: a.mlp → b.mlp → c.mlp → a.mlp
```

3. **Parse Error in Module**
```
Error: Failed to parse imported module 'math.mlp'
  Line 5: Syntax error in function declaration
```

4. **Symbol Conflict**
```
Error: Function 'add' already defined
  First definition: math.mlp:10
  Second definition: utils.mlp:20
```

---

## 📁 MODULAR STAGE 1 STRUCTURE

Import sistemi ile hedef yapı:

```
modules/
├── core/
│   ├── char_utils.mlp
│   ├── token_types.mlp
│   └── type_mapper.mlp
│
├── functions/
│   ├── functions_parser.mlp
│   │   import "../core/token_types"
│   │   import "../core/char_utils"
│   └── functions_codegen.mlp
│       import "../core/type_mapper"
│
├── variables/
│   ├── variables_parser.mlp
│   │   import "../core/token_types"
│   └── variables_codegen.mlp
│       import "../core/type_mapper"
│
└── integration/
    └── stage1_compiler.mlp
        import "../core/token_types"
        import "../functions/functions_parser"
        import "../functions/functions_codegen"
        import "../variables/variables_parser"
        import "../variables/variables_codegen"
```

---

## ✅ SUCCESS CRITERIA

Import sistemi başarılı sayılır eğer:

1. ✅ Module path resolution çalışıyor
2. ✅ Circular dependency yakalanıyor
3. ✅ Aynı modül birden fazla import edilemiyor (cache)
4. ✅ Symbol table doğru çalışıyor
5. ✅ LLVM IR düzgün generate ediliyor
6. ✅ Multi-file MELP programları derlenebiliyor
7. ✅ Stage 0 self-compile hala çalışıyor

---

## 📊 STAGE 0 MEVCUT DURUM

**✅ Zaten Var:**
- Import modülü (`compiler/stage0/modules/import/`)
- Module resolution
- Import cache
- Import parser

**⏳ Güncellenecek (YZ_02-04):**
- Lexer (TOKEN_IMPORT)
- Parser (import statement)
- CodeGen (multi-file support)
- Main compiler pipeline (import entegrasyonu)

**✅ Test:**
```bash
cd compiler/stage0
make
./melpc test_import.mlp -o test
./test
# Output: Import sistemi çalışıyor!
```

---

## 🎯 IMPLEMENTATION TIMELINE

| YZ | Görev | Süre | Durum |
|----|-------|------|-------|
| YZ_01 | Bu tasarım dokümanı | 3h | ✅ TAMAMLANDI |
| YZ_02 | Lexer + Parser | 4h | ⏸️ BEKLİYOR |
| YZ_03 | Resolution + Symbol | 5h | ⏸️ BEKLİYOR |
| YZ_04 | CodeGen + Integration | 3h | ⏸️ BEKLİYOR |

**Toplam:** 13 saat

---

## 📚 REFERANSLAR

**Stage 0 Kodu:**
- `compiler/stage0/modules/import/import.h`
- `compiler/stage0/modules/import/import.c`
- `compiler/stage0/modules/import/import_parser.c`
- `compiler/stage0/modules/import/import_cache.c`

**C Benzerlikleri:**
- `#include "module.h"` → `import "module"`
- Header guards → Import cache
- Linker → LLVM IR inline

**Diğer Diller:**
- Python: `import math`
- JavaScript: `import { add } from 'math'`
- Rust: `use math::add`

---

**SON GÜNCELLEME:** YZ_01 - 17 Aralık 2025  
**DURUM:** ✅ Tasarım tamamlandı  
**SONRAKİ:** YZ_02 - Import Lexer/Parser implementation

---

**MELP'in modüler mimarisi hazır!** 🎯🚀

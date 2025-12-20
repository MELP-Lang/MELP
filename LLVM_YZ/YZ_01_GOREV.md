# LLVM_YZ_01: CLI Entegrasyonu ve İlk Test

**Tarih:** 20 Aralık 2025  
**Atanan:** YZ_01 (LLVM Görevlisi)  
**Üst Akıl:** YZ_ÜA_04  
**Öncelik:** 🔴 YÜKSEK  
**Önceki:** LLVM_YZ_00 (Analiz tamamlandı)  
**Süre:** 2 saat

---

## 🎯 Görev

LLVM backend'i `functions_compiler`'a entegre et ve `--backend=llvm` flag'i ile kullanılabilir hale getir.

---

## 📋 LLVM_YZ_00 Bulguları

### Mevcut Durum (HAZIR ✅):
- LLVM backend implementasyonu tamam (829 satır)
- API hazır (`function_generate_declaration_llvm()`)
- Tüm temel features destekleniyor

### Sorun:
**CLI entegrasyonu yok:**
```bash
./functions_compiler --backend=llvm test.mlp test.ll
# ❌ Usage error - flag tanınmıyor
```

---

## 🔧 Yapılacak Değişiklikler

### Dosya: `compiler/stage0/modules/functions/functions_standalone.c`

#### 1. Header Include Ekle
```c
#include "functions_codegen_llvm.h"  // LLVM backend
```

#### 2. Backend Enum Ekle
```c
typedef enum {
    BACKEND_ASSEMBLY,
    BACKEND_LLVM
} BackendType;
```

#### 3. CLI Argument Parsing
**Mevcut:**
```c
if (argc != 3) {
    fprintf(stderr, "Usage: %s <input.mlp> <output.s>\n", argv[0]);
    return 1;
}
```

**Yeni:**
```c
BackendType backend = BACKEND_ASSEMBLY;  // Default
char* input_file = NULL;
char* output_file = NULL;

// Parse arguments
for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--backend=llvm") == 0) {
        backend = BACKEND_LLVM;
    } else if (strcmp(argv[i], "--backend=assembly") == 0) {
        backend = BACKEND_ASSEMBLY;
    } else if (!input_file) {
        input_file = argv[i];
    } else if (!output_file) {
        output_file = argv[i];
    }
}

if (!input_file || !output_file) {
    fprintf(stderr, "Usage: %s [--backend=llvm|assembly] <input.mlp> <output>\n", argv[0]);
    return 1;
}
```

#### 4. Backend Selection Logic
**Eklenecek Bölüm (satır ~240, codegen'den önce):**
```c
// Generate code based on backend
if (backend == BACKEND_LLVM) {
    // LLVM IR generation
    FunctionLLVMContext* llvm_ctx = function_llvm_context_create(output);
    
    // Module header
    function_generate_module_header_llvm(output);
    
    // Generate all functions
    FunctionDeclaration* func = functions;
    while (func) {
        function_generate_declaration_llvm(llvm_ctx, func);
        func = func->next;
    }
    
    // Module footer
    function_generate_module_footer_llvm(output);
    
    // Cleanup
    function_llvm_context_free(llvm_ctx);
    
} else {
    // Assembly generation (existing code)
    function_generate_header(output);
    
    FunctionDeclaration* func = functions;
    while (func) {
        function_generate_declaration(output, func);
        func = func->next;
    }
    
    function_generate_footer(output);
}
```

#### 5. Output File Extension Helper (Optional)
```c
// Auto-detect output extension if not provided
if (!output_file) {
    if (backend == BACKEND_LLVM) {
        output_file = replace_extension(input_file, ".ll");
    } else {
        output_file = replace_extension(input_file, ".s");
    }
}
```

---

## 🧪 Test Planı

### Test 1: Basic Return
**Dosya:** `temp/llvm_test_return.mlp`
```pmpl
function main() returns numeric
    return 42
end
```

**Komutlar:**
```bash
./functions_compiler --backend=llvm temp/llvm_test_return.mlp temp/llvm_test_return.ll
lli temp/llvm_test_return.ll
echo $?  # Expected: 42
```

### Test 2: Arithmetic
**Dosya:** `temp/llvm_test_arithmetic.mlp`
```pmpl
function main() returns numeric
    return 10 + 20 + 12
end
```

**Expected:** Exit code 42

### Test 3: Variables
**Dosya:** `temp/llvm_test_variables.mlp`
```pmpl
function main() returns numeric
    var x = 30
    var y = 12
    return x + y
end
```

**Expected:** Exit code 42

### Test 4: Function Call
**Dosya:** `temp/llvm_test_call.mlp`
```pmpl
function add(a as numeric, b as numeric) returns numeric
    return a + b
end

function main() returns numeric
    return add(30, 12)
end
```

**Expected:** Exit code 42

### Test 5: If Statement
**Dosya:** `temp/llvm_test_if.mlp`
```pmpl
function main() returns numeric
    var x = 42
    if x > 0 then
        return x
    end if
    return 0
end
```

**Expected:** Exit code 42

---

## 📁 Değiştirilecek Dosyalar

**Tek dosya:**
- `compiler/stage0/modules/functions/functions_standalone.c`

**Eklenen satırlar:** ~60 satır

**Diğer dosyalar değişmeyecek** (backend kodu zaten hazır)

---

## ✅ Tamamlanma Kriterleri

- [ ] `--backend=llvm` flag eklendi
- [ ] Backend selection logic implementasyonu
- [ ] Test 1 geçiyor (return 42) ✅
- [ ] Test 2 geçiyor (arithmetic) ✅
- [ ] Test 3 geçiyor (variables) ✅
- [ ] Test 4 geçiyor (function call) ✅
- [ ] Test 5 geçiyor (if statement) ✅
- [ ] Compiler yeniden derlendi (`make`)
- [ ] LLVM IR output doğru format
- [ ] `lli` ile çalıştırılabiliyor
- [ ] Rapor yazıldı (`LLVM_YZ/YZ_01.md`)
- [ ] Git commit yapıldı (`llvm-cli-integration_YZ_01`)

---

## 🔗 Referanslar

- `LLVM_YZ/YZ_00.md` - Analiz raporu
- `compiler/stage0/modules/functions/functions_standalone.c` - Değiştirilecek dosya
- `compiler/stage0/modules/functions/functions_codegen_llvm.h` - LLVM API

---

## 🎬 Implementation Adımları

### Adım 1: Header Include (1 dakika)
```c
#include "functions_codegen_llvm.h"
```

### Adım 2: Backend Enum (2 dakika)
```c
typedef enum {
    BACKEND_ASSEMBLY,
    BACKEND_LLVM
} BackendType;
```

### Adım 3: Argument Parsing (15 dakika)
- Flag detection loop
- Input/output file assignment
- Error handling

### Adım 4: Backend Selection (30 dakika)
- LLVM path implementasyonu
- Context creation/destruction
- Module header/footer

### Adım 5: Testing (60 dakika)
- 5 test dosyası oluştur
- Compile ve execute
- Output verification

### Adım 6: Documentation (15 dakika)
- Usage update
- README update
- Report writing

**Toplam:** ~2 saat

---

## 💡 Usage Examples

### LLVM Backend
```bash
# Explicit LLVM backend
./functions_compiler --backend=llvm program.mlp program.ll
lli program.ll

# Or compile with Clang
./functions_compiler --backend=llvm program.mlp program.ll
clang program.ll -o program
./program
```

### Assembly Backend (Default)
```bash
# Default (assembly)
./functions_compiler program.mlp program.s
gcc program.s -o program
./program

# Explicit assembly backend
./functions_compiler --backend=assembly program.mlp program.s
```

---

## 🐛 Potansiyel Sorunlar

### 1. Context Initialization
**Sorun:** LLVM context düzgün initialize edilmezse segfault

**Çözüm:**
```c
FunctionLLVMContext* llvm_ctx = function_llvm_context_create(output);
if (!llvm_ctx) {
    fprintf(stderr, "Error: Failed to create LLVM context\n");
    return 1;
}
```

### 2. Memory Management
**Sorun:** Context free edilmezse memory leak

**Çözüm:** Her return path'te cleanup:
```c
if (error) {
    function_llvm_context_free(llvm_ctx);
    fclose(output);
    return 1;
}
```

### 3. String Globals
**Sorun:** String globals emit edilmeli

**Çözüm:** `function_generate_module_footer_llvm()` bunu hallediyor

---

## 📊 Expected Output

### Sample LLVM IR
**Input:**
```pmpl
function main() returns numeric
    return 42
end
```

**Output (program.ll):**
```llvm
; ModuleID = 'melp_module'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i64 @main() {
entry:
  ret i64 42
}
```

### Execution
```bash
$ lli program.ll
$ echo $?
42
```

---

## 🔄 Sonraki Adım

**LLVM_YZ_02:** Test suite ve CI entegrasyonu

---

**Atanan:** YZ_01  
**Durum:** ⏳ Hazır

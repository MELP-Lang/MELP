# YZ_200: List Type Full Implementation - GÖREV BRİFİNGİ

**Tarih:** 21 Aralık 2025  
**Görevli:** YZ_200 (GitHub Copilot - Claude Sonnet 4.5)  
**Branch:** `list-operations_YZ_200`  
**Öncelik:** 🔴 KRİTİK  
**Süre:** 3-5 gün  
**Bağlı Görev:** YZ_07 (TAMAMLANDI ✅)

---

## 🎯 GÖREV ÖZETİ

**Hedef:** List type için tam operasyon desteği eklemek

**Mevcut Durum (YZ_07):**
- ✅ List literal parsing: `(1; 2; 3;)` çalışıyor
- ✅ Empty list: `()` çalışıyor
- ✅ List type declaration: `list numbers` çalışıyor
- ❌ List operations: append, prepend, length, get, set YOK
- ❌ List allocation: Sadece NULL pointer döndürülüyor (stub)
- ❌ List iteration: `for item in list` YOK

**Sonuç Hedefi:**
- List runtime memory allocation (heap-based dynamic array)
- Temel operasyonlar: append, prepend, length, get, set
- List indexing: `list[0]`, `list[i]`
- Optional: List iteration (for-in loop)

---

## 📋 YAPILACAKLAR (Sıralı)

### Faz 1: Runtime List Structure (1 gün)

**1.1. List Runtime Structure Tasarımı**
- `runtime/stdlib/list.h` - List struct tanımı
- `runtime/stdlib/list.c` - List operations implementasyonu

**List Struct:**
```c
typedef struct {
    void** elements;      // Dynamic array of elements (generic pointers)
    size_t length;        // Current number of elements
    size_t capacity;      // Allocated capacity
    size_t element_size;  // Size of each element (bytes)
} MelpList;
```

**1.2. Core Runtime Functions**
```c
MelpList* melp_list_create(size_t element_size);
void melp_list_free(MelpList* list);
void melp_list_append(MelpList* list, void* element);
void melp_list_prepend(MelpList* list, void* element);
void* melp_list_get(MelpList* list, size_t index);
void melp_list_set(MelpList* list, size_t index, void* element);
size_t melp_list_length(MelpList* list);
```

**Dosyalar:**
- `runtime/stdlib/list.h` - Header file (yeni)
- `runtime/stdlib/list.c` - Implementation (yeni)
- `runtime/stdlib/Makefile` - Build kuralları (güncelle)

---

### Faz 2: Compiler Integration (1.5 gün)

**2.1. Lexer - List Operation Keywords**
`compiler/stage0/modules/lexer/lexer.c`

**Yeni Keyword'ler:**
```c
TOKEN_APPEND,    // "append"
TOKEN_PREPEND,   // "prepend"
TOKEN_LENGTH,    // "length"
```

**2.2. Parser - List Operations**
`compiler/stage0/modules/functions/functions_parser.c`

**Syntax:**
```pmpl
-- List creation
list numbers = (1; 2; 3;)

-- List operations
append(numbers; 4)
prepend(numbers; 0)
numeric len = length(numbers)

-- List indexing
numeric first = numbers[0]
numbers[1] = 99
```

**2.3. AST Nodes**
Yeni statement tipi: `STMT_LIST_OPERATION`

```c
typedef enum {
    LIST_OP_APPEND,
    LIST_OP_PREPEND,
    LIST_OP_GET,
    LIST_OP_SET,
    LIST_OP_LENGTH
} ListOperationType;

typedef struct {
    ListOperationType op_type;
    char* list_name;
    ArithmeticExpr* value;  // For append/prepend/set
    ArithmeticExpr* index;  // For get/set
} ListOperationNode;
```

---

### Faz 3: LLVM Codegen (1.5 gün)

**3.1. List Literal Allocation**
`compiler/stage0/modules/functions/functions_codegen_llvm.c`

**Şu anki kod (Line 104-116):**
```c
if (arith->is_collection && arith->collection) {
    // STUB: Return NULL pointer
    return llvm_const_i64(0);
}
```

**Yeni kod:**
```c
if (arith->is_collection && arith->collection) {
    Collection* coll = arith->collection;
    if (coll->type == COLL_LIST) {
        // 1. Call melp_list_create(element_size)
        // 2. For each element: call melp_list_append(list, element)
        // 3. Return list pointer
        return codegen_list_literal(coll);
    }
}
```

**3.2. List Operations Codegen**

**append(list; value):**
```llvm
; Call melp_list_append(list_ptr, &value)
call void @melp_list_append(i8* %list_ptr, i8* %value_ptr)
```

**length(list):**
```llvm
; Call melp_list_length(list_ptr)
%len = call i64 @melp_list_length(i8* %list_ptr)
```

**list[index]:**
```llvm
; Call melp_list_get(list_ptr, index)
%elem_ptr = call i8* @melp_list_get(i8* %list_ptr, i64 %index)
%elem = load i64, i8* %elem_ptr
```

**3.3. Runtime Function Declarations**
`compiler/stage0/modules/llvm_backend/llvm_backend.c`

```c
// Declare runtime functions
LLVMValueRef melp_list_create = LLVMAddFunction(
    module, "melp_list_create",
    LLVMFunctionType(LLVMPointerType(LLVMInt8Type(), 0), 
                     &LLVMInt64Type(), 1, 0));

LLVMValueRef melp_list_append = LLVMAddFunction(
    module, "melp_list_append",
    LLVMFunctionType(LLVMVoidType(), append_params, 2, 0));

// ... other functions
```

---

### Faz 4: Testing & Validation (1 gün)

**4.1. Unit Tests**
`tests/llvm/08_list/`

**Test Cases:**
```pmpl
-- test_list_basic.mlp
function test_list_create() returns numeric
    list numbers = (1; 2; 3;)
    return length(numbers)  -- Expected: 3
end_function

-- test_list_append.mlp
function test_list_append() returns numeric
    list numbers = (1; 2;)
    append(numbers; 3)
    append(numbers; 4)
    return length(numbers)  -- Expected: 4
end_function

-- test_list_indexing.mlp
function test_list_indexing() returns numeric
    list numbers = (10; 20; 30;)
    return numbers[1]  -- Expected: 20
end_function

-- test_list_set.mlp
function test_list_set() returns numeric
    list numbers = (1; 2; 3;)
    numbers[1] = 99
    return numbers[1]  -- Expected: 99
end_function

-- test_list_empty.mlp
function test_list_empty() returns numeric
    list numbers = ()
    append(numbers; 1)
    return length(numbers)  -- Expected: 1
end_function
```

**4.2. Stage 1 Module Test**
```bash
./functions_compiler modules/lexer_mlp/token.mlp temp/test.ll --backend=llvm
./functions_compiler modules/lexer_mlp/lexer.mlp temp/test.ll --backend=llvm

# All modules should compile without errors
for file in modules/**/*.mlp; do
    ./functions_compiler "$file" temp/test.ll --backend=llvm || echo "FAIL: $file"
done
```

**4.3. Runtime Execution Test**
```bash
cd tests/llvm/08_list
./run_test.sh test_list_basic.mlp
# Expected output: 3

./run_test.sh test_list_append.mlp
# Expected output: 4
```

---

## 📊 BAŞARI KRİTERLERİ

### Must Have (Zorunlu):
- ✅ List literal allocation (heap-based)
- ✅ `length(list)` operasyonu
- ✅ `append(list; value)` operasyonu
- ✅ `list[index]` indexing (get)
- ✅ Tüm test case'ler geçiyor

### Nice to Have (Opsiyonel):
- ⚪ `prepend(list; value)` operasyonu
- ⚪ `list[index] = value` indexing (set)
- ⚪ List bounds checking (runtime error)
- ⚪ List concatenation: `list1 + list2`

### Out of Scope (YZ_201+):
- ❌ Generic types: `list<numeric>`, `list<string>`
- ❌ List iteration: `for item in list`
- ❌ List comprehension: `[x * 2 for x in list]`
- ❌ List methods: `list.map()`, `list.filter()`

---

## 🔧 DOSYA DEĞİŞİKLİKLERİ

### Yeni Dosyalar:
1. `runtime/stdlib/list.h` - List runtime header
2. `runtime/stdlib/list.c` - List runtime implementation
3. `tests/llvm/08_list/test_list_basic.mlp` - Test files
4. `tests/llvm/08_list/test_list_append.mlp`
5. `tests/llvm/08_list/test_list_indexing.mlp`
6. `tests/llvm/08_list/run_test.sh` - Test runner

### Değiştirilecek Dosyalar:
1. `compiler/stage0/modules/lexer/lexer.c` - Keyword tokenization
2. `compiler/stage0/modules/functions/functions_parser.c` - List operations parsing
3. `compiler/stage0/modules/functions/functions_codegen_llvm.c` - List codegen
4. `compiler/stage0/modules/llvm_backend/llvm_backend.c` - Runtime declarations
5. `runtime/stdlib/Makefile` - Build rules update

**Tahmin:**
- +500 satır (runtime)
- +300 satır (parser)
- +400 satır (codegen)
- +200 satır (tests)
- **Toplam: ~1400 satır**

---

## 📚 REFERANS BELGELER

### İç Belgeler:
1. `LLVM_YZ/YZ_07_TAMAMLANDI.md` - Mevcut list literal parsing
2. `compiler/stage0/modules/arithmetic/arithmetic_parser.c` (Line 1450-1600) - List parsing kodu
3. `pmlp_kesin_sozdizimi.md` (Line 640-642) - List syntax
4. `TODO_MODERN_LANGUAGE.md` (Line 60-95) - YZ_200 görev tanımı
5. `runtime/stdlib/README.md` - Runtime library structure

### LLVM Referansları:
1. `docs/LLVM_IR_GUIDE.md` - LLVM IR basics
2. LLVM Function Calls: https://llvm.org/docs/LangRef.html#call-instruction
3. LLVM Pointers: https://llvm.org/docs/LangRef.html#pointer-type

### C Runtime Örnekleri:
- Python list implementation (CPython)
- Rust Vec implementation (std::vec)
- C++ std::vector

---

## 🚀 UYGULAMA PLANI

### Gün 1: Runtime Foundation
- [ ] `runtime/stdlib/list.h` struct tanımı
- [ ] `runtime/stdlib/list.c` create, free, append
- [ ] Makefile update + build test
- [ ] Simple C test program

### Gün 2: Parser Integration
- [ ] Lexer keyword additions (append, length)
- [ ] Parser: List operation statement parsing
- [ ] AST node definitions
- [ ] Parser test (syntax check only)

### Gün 3: Codegen Part 1
- [ ] LLVM runtime function declarations
- [ ] List literal allocation codegen
- [ ] `length(list)` codegen
- [ ] Basic test compilation

### Gün 4: Codegen Part 2
- [ ] `append(list; value)` codegen
- [ ] List indexing `list[index]` codegen
- [ ] Integration test
- [ ] Stage 1 module compilation test

### Gün 5: Testing & Polish
- [ ] Write all test cases
- [ ] Run tests and fix bugs
- [ ] Documentation update
- [ ] YZ_200 completion report

---

## ⚠️ POTANSİYEL PROBLEMLER

### 1. Type Safety
**Problem:** List elements are `void*` pointers - no type checking
**Çözüm:** Phase 1'de sadece numeric değerlere izin ver. Generic types YZ_203'te eklenecek.

### 2. Memory Management
**Problem:** List allocation heap'te, kim free edecek?
**Çözüm:** STO (Safe Token Ownership) runtime ile entegre et. YZ_07'de STO zaten var.

### 3. Array vs List Confusion
**Problem:** `[1, 2, 3]` array mi, `(1; 2; 3;)` list mi?
**Çözüm:** Şimdilik sadece list `()` destekle. Array `[]` YZ_201'de eklenecek.

### 4. Performance
**Problem:** Dynamic reallocation her append'de yavaş
**Çözüm:** Capacity doubling strategy (Python/Rust gibi). Initial capacity = 4.

---

## 🎯 SONRAKI GÖREVLER (YZ_200 Sonrası)

**YZ_201: Map/Dictionary Type (5 gün)**
- Hash table implementation
- `map person = {"name": "Alice"}`

**YZ_202: Optional Type (3 gün)**
- Null safety: `optional numeric`
- Safe unwrapping: `value ?? default`

**YZ_203: Generic Types (2 hafta)**
- `function max<T>(T a, T b)`
- Type parameter constraints

---

## 📝 NOTLAR

1. **List literal parsing zaten çalışıyor** (YZ_07'de fix edildi)
2. **Syntax `()` kullanıyor** (köşeli parantez değil!)
3. **Parametre ayırıcı `;` kullanıyor** (virgül değil!)
4. **Minimal implementation yeterli** - Generic types sonra gelecek

**ÖNEMLİ:** Bu görev Stage 1 bootstrap için kritik değil (YZ_07 bloker'i çözdü). Ancak proper list operations modern dil standardı için zorunlu.

---

**Hazırlayan:** YZ_200 Pre-Brief  
**Tarih:** 21 Aralık 2025  
**Sonraki Adım:** Runtime list structure implementasyonu ile başla!

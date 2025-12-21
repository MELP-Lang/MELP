# YZ_213: Collections Parser Integration
**Başlangıç:** 22 Aralık 2025  
**Öncelik:** 🔴 Yüksek (YZ_212 runtime tamamlandı, parser entegrasyonu gerekli)  
**Tahmini Süre:** 3-4 gün  
**Durum:** 📋 Hazır (Görev başlatılabilir)

---

## 📋 Görev Özeti

YZ_212'de Collections Library runtime implementasyonu tamamlandı (22/22 test passing). Şimdi bu collection'ları MELP dilinde kullanılabilir hale getirmek için **parser entegrasyonu** yapılacak.

**Hedef:** MELP kodunda `create_set()`, `enqueue()`, `push()` gibi keyword-based collection fonksiyonlarını kullanabilmek.

---

## 🎯 Hedefler

### Phase 1: Lexer Keywords (1 gün)
Collection fonksiyonları için keyword tokenları ekle:

**List Extensions:**
- `insert_at` - Liste ortasına ekleme
- `find_in_list` - Eleman arama
- `sort_list` - Sıralama

**Map Extensions:**
- `map_keys` - Key listesi al
- `map_values` - Value listesi al
- `clear_map` - Map'i temizle

**Set Operations:**
- `create_set` - Yeni set oluştur
- `add_to_set` - Element ekle
- `set_contains` - Element kontrolü
- `remove_from_set` - Element sil
- `set_union` - Birleşim
- `set_intersection` - Kesişim
- `set_difference` - Fark
- `set_size` - Eleman sayısı

**Queue Operations:**
- `create_queue` - Yeni queue oluştur
- `enqueue` - FIFO ekle
- `dequeue` - FIFO çıkar
- `queue_peek` - İlk elemanı gör
- `queue_size` - Eleman sayısı
- `queue_is_empty` - Boş mu?

**Stack Operations:**
- `create_stack` - Yeni stack oluştur
- `push` - LIFO ekle
- `pop` - LIFO çıkar
- `stack_peek` - Üst elemanı gör
- `stack_size` - Eleman sayısı
- `stack_is_empty` - Boş mu?

**Dosya:** `compiler/stage0/lexer.c`

```c
// Örnek token eklemeleri:
{ "create_set", TOKEN_CREATE_SET },
{ "add_to_set", TOKEN_ADD_TO_SET },
{ "enqueue", TOKEN_ENQUEUE },
{ "push", TOKEN_PUSH },
// ... (toplam ~25 keyword)
```

---

### Phase 2: Parser Integration (1.5 gün)

**2.1. Function Call Parser Extension**
Mevcut function call parser'ı genişlet:

```c
// compiler/stage0/modules/functions/functions_parser.c

ASTNode* parse_collection_call(Token* token) {
    // Set operations
    if (token->type == TOKEN_CREATE_SET) {
        return parse_create_set();
    }
    if (token->type == TOKEN_ADD_TO_SET) {
        return parse_add_to_set();  // add_to_set(set, item)
    }
    
    // Queue operations
    if (token->type == TOKEN_ENQUEUE) {
        return parse_enqueue();  // enqueue(queue, item)
    }
    
    // Stack operations
    if (token->type == TOKEN_PUSH) {
        return parse_push();  // push(stack, item)
    }
    
    // ... diğer operations
}
```

**2.2. AST Node Types**
Collection fonksiyonları için AST node tipleri:

```c
// compiler/stage0/ast.h

typedef enum {
    // ... mevcut node types
    
    // Collection operations
    AST_CREATE_SET,
    AST_ADD_TO_SET,
    AST_SET_CONTAINS,
    AST_ENQUEUE,
    AST_DEQUEUE,
    AST_PUSH,
    AST_POP,
    // ... (toplam ~25 node type)
} ASTNodeType;
```

**2.3. Type Checking**
Collection return type'ları kontrol et:

```c
// create_set() returns Set*
// enqueue(Queue*, item) returns void
// pop(Stack*) returns void* (STO generic)
```

---

### Phase 3: LLVM Codegen (1 gün)

Runtime fonksiyonlarına LLVM call generation:

```c
// compiler/stage0/modules/functions/functions_codegen_llvm.c

LLVMValueRef codegen_collection_call(ASTNode* node) {
    switch (node->type) {
        case AST_CREATE_SET: {
            // Call: melp_set_create()
            LLVMValueRef func = LLVMGetNamedFunction(module, "melp_set_create");
            return LLVMBuildCall(builder, func, NULL, 0, "set");
        }
        
        case AST_ADD_TO_SET: {
            // Call: melp_set_add(set, item)
            LLVMValueRef set = codegen(node->children[0]);
            LLVMValueRef item = codegen(node->children[1]);
            
            LLVMValueRef func = LLVMGetNamedFunction(module, "melp_set_add");
            LLVMValueRef args[] = {set, item};
            return LLVMBuildCall(builder, func, args, 2, "");
        }
        
        case AST_ENQUEUE: {
            // Call: melp_queue_enqueue(queue, item)
            LLVMValueRef queue = codegen(node->children[0]);
            LLVMValueRef item = codegen(node->children[1]);
            
            LLVMValueRef func = LLVMGetNamedFunction(module, "melp_queue_enqueue");
            LLVMValueRef args[] = {queue, item};
            return LLVMBuildCall(builder, func, args, 2, "");
        }
        
        // ... diğer operations
    }
}
```

**Runtime Function Declarations:**
```c
// LLVM'de declare et:
declare %Set* @melp_set_create()
declare void @melp_set_add(%Set*, i8*)
declare i32 @melp_set_contains(%Set*, i8*)
declare %Queue* @melp_queue_create()
declare void @melp_queue_enqueue(%Queue*, i8*)
// ... (tüm runtime fonksiyonları)
```

---

### Phase 4: End-to-End Testing (0.5 gün)

**4.1. MELP Syntax Tests**
```pmpl
-- tests/llvm/13_collections/test_set.mlp
function test_set() returns numeric
    -- Create set
    Set numbers = create_set()
    
    -- Add elements
    add_to_set(numbers, 10)
    add_to_set(numbers, 20)
    add_to_set(numbers, 10)  -- Duplicate, ignored
    
    -- Check contains
    numeric has_10 = set_contains(numbers, 10)  -- Should be 1
    numeric has_99 = set_contains(numbers, 99)  -- Should be 0
    
    -- Size check
    numeric size = set_size(numbers)  -- Should be 2
    
    return size  -- Returns 2
end_function

-- tests/llvm/13_collections/test_queue.mlp
function test_queue() returns numeric
    Queue tasks = create_queue()
    
    enqueue(tasks, 100)
    enqueue(tasks, 200)
    enqueue(tasks, 300)
    
    numeric first = dequeue(tasks)  -- Should be 100 (FIFO)
    numeric second = dequeue(tasks) -- Should be 200
    
    return first + second  -- Returns 300
end_function

-- tests/llvm/13_collections/test_stack.mlp
function test_stack() returns numeric
    Stack history = create_stack()
    
    push(history, 10)
    push(history, 20)
    push(history, 30)
    
    numeric top = pop(history)     -- Should be 30 (LIFO)
    numeric next = pop(history)    -- Should be 20
    
    return top + next  -- Returns 50
end_function

-- tests/llvm/13_collections/test_list_ext.mlp
function test_list_insert() returns numeric
    list numbers = (10; 30; 50;)
    
    insert_at(numbers, 1, 20)  -- Insert at index 1
    -- Result: (10; 20; 30; 50;)
    
    numeric found = find_in_list(numbers, 30)  -- Should be 2
    
    return found  -- Returns 2
end_function

-- tests/llvm/13_collections/test_map_ext.mlp
function test_map_keys() returns numeric
    map person = {"name": "Alice"; "age": 30; "city": "NYC";}
    
    list keys = map_keys(person)  -- Should return ("name"; "age"; "city";)
    numeric key_count = length(keys)  -- Should be 3
    
    clear_map(person)
    numeric empty = length(person)  -- Should be 0
    
    return key_count  -- Returns 3
end_function
```

**4.2. Test Execution**
```bash
cd /home/pardus/projeler/MLP/MLP

# Compile tests
./compiler/stage0/melp_functions_compiler tests/llvm/13_collections/test_set.mlp -o test_set.ll
./compiler/stage0/melp_functions_compiler tests/llvm/13_collections/test_queue.mlp -o test_queue.ll
./compiler/stage0/melp_functions_compiler tests/llvm/13_collections/test_stack.mlp -o test_stack.ll
./compiler/stage0/melp_functions_compiler tests/llvm/13_collections/test_list_ext.mlp -o test_list_ext.ll
./compiler/stage0/melp_functions_compiler tests/llvm/13_collections/test_map_ext.mlp -o test_map_ext.ll

# Generate executables
clang -O2 test_set.ll runtime/stdlib/mlp_*.c -o test_set
clang -O2 test_queue.ll runtime/stdlib/mlp_*.c -o test_queue
clang -O2 test_stack.ll runtime/stdlib/mlp_*.c -o test_stack
clang -O2 test_list_ext.ll runtime/stdlib/mlp_*.c -o test_list_ext
clang -O2 test_map_ext.ll runtime/stdlib/mlp_*.c -o test_map_ext

# Run tests
./test_set      # Expected: 2
./test_queue    # Expected: 300
./test_stack    # Expected: 50
./test_list_ext # Expected: 2
./test_map_ext  # Expected: 3
```

**Success Criteria:** 5/5 tests passing ✅

---

## 📊 Dosya Listesi

### Yeni Dosyalar (Test)
```
tests/llvm/13_collections/
├── test_set.mlp           (Set operations test)
├── test_queue.mlp         (Queue FIFO test)
├── test_stack.mlp         (Stack LIFO test)
├── test_list_ext.mlp      (List insert/find/sort)
└── test_map_ext.mlp       (Map keys/values/clear)
```

### Değiştirilecek Dosyalar
```
compiler/stage0/
├── lexer.c                           (+25 keywords)
├── ast.h                             (+25 AST node types)
└── modules/functions/
    ├── functions_parser.c            (+200 lines: collection call parser)
    └── functions_codegen_llvm.c      (+300 lines: LLVM codegen)
```

**Toplam:** ~525 LOC eklenecek

---

## ⚠️ Dikkat Edilecekler

### 1. STO Compliance
Collection'lar `void*` (generic pointer) kullanır:
```c
// Runtime'da zaten var:
void melp_set_add(Set* set, void* item);
void* melp_queue_dequeue(Queue* queue);
```

**Parser'da:** Type checking minimal - STO felsefesi gereği runtime'da handle edilir.

### 2. Keyword Seçimi
YZ_212'de belirlenen keyword-based syntax:
- ✅ `create_set()` (MELP keyword)
- ❌ `new Set()` (OOP syntax, MELP'de yok)
- ❌ `Set.create()` (Static method, MELP'de yok)

### 3. Existing Syntax Integration
Mevcut list/map syntax ile uyumlu olmalı:
```pmpl
list numbers = (1; 2; 3;)         -- Existing literal syntax
Set unique = create_set()          -- New keyword function
add_to_set(unique, 10)             -- New keyword function
append(numbers, 4)                 -- Existing function
```

### 4. Return Type Handling
```c
// Void return (no value)
enqueue(queue, 100)  -- Statement, no return

// Pointer return (collection)
Set s = create_set()  -- Returns Set*

// Numeric return (boolean/size)
numeric has = set_contains(s, 10)  -- Returns int (0/1)
```

### 5. Memory Management
Collection'lar YZ_210 RC GC kullanır:
```c
// Runtime'da zaten entegre:
Set* set = melp_set_create();  // ref_count = 1
// ... use set
// Scope sonunda rc_release() auto call
```

**Parser'da:** Memory management için ekstra kod gerekmez (runtime handle eder).

---

## 🧪 Test Coverage

| Collection | Operation | Test File | Expected Output |
|-----------|-----------|-----------|-----------------|
| Set | create, add, contains | test_set.mlp | 2 (size) |
| Set | union, intersection | test_set_ops.mlp | 3 (union size) |
| Queue | enqueue, dequeue | test_queue.mlp | 300 (sum) |
| Stack | push, pop | test_stack.mlp | 50 (sum) |
| List | insert_at, find | test_list_ext.mlp | 2 (index) |
| Map | keys, values, clear | test_map_ext.mlp | 3 (key count) |

**Hedef:** 6/6 tests passing ✅

---

## 📈 İlerleme Takibi

### Phase 1: Lexer Keywords ⏸️
- [ ] List extensions: insert_at, find_in_list, sort_list
- [ ] Map extensions: map_keys, map_values, clear_map
- [ ] Set operations: create_set, add_to_set, set_contains, ...
- [ ] Queue operations: create_queue, enqueue, dequeue, ...
- [ ] Stack operations: create_stack, push, pop, ...
- [ ] Lexer token enum update
- [ ] Keyword hash table update

### Phase 2: Parser Integration ⏸️
- [ ] Collection call parser (functions_parser.c)
- [ ] AST node types (ast.h)
- [ ] Type checking (minimal STO)
- [ ] Argument parsing
- [ ] Error handling

### Phase 3: LLVM Codegen ⏸️
- [ ] Runtime function declarations
- [ ] Set operations codegen
- [ ] Queue operations codegen
- [ ] Stack operations codegen
- [ ] List/Map extensions codegen
- [ ] Return value handling

### Phase 4: Testing ⏸️
- [ ] Test file creation (5 files)
- [ ] Compilation tests
- [ ] Execution tests
- [ ] Output validation
- [ ] Integration test (mixed operations)

---

## 🎯 Başarı Kriterleri

✅ **Phase 1:** 25 keyword token eklendi, lexer test passed  
✅ **Phase 2:** Collection call parser implemented, AST nodes created  
✅ **Phase 3:** LLVM codegen working, runtime functions declared  
✅ **Phase 4:** 5/5 end-to-end tests passing  

**Final Check:**
```pmpl
-- Real-world usage example
function process_data() returns numeric
    -- Create collections
    Set unique_ids = create_set()
    Queue pending = create_queue()
    Stack history = create_stack()
    
    -- Add data
    add_to_set(unique_ids, 100)
    add_to_set(unique_ids, 200)
    enqueue(pending, 100)
    push(history, 100)
    
    -- Process
    numeric task = dequeue(pending)
    numeric last = pop(history)
    
    return task + last  -- Should work!
end_function
```

---

## 📚 Referanslar

**Zorunlu Okuma:**
1. `LLVM_YZ/YZ_212_GOREV.md` - Collections Library specification
2. `runtime/stdlib/mlp_set.h` - Set API reference
3. `runtime/stdlib/mlp_queue.h` - Queue API reference
4. `runtime/stdlib/mlp_stack.h` - Stack API reference
5. `compiler/stage0/modules/functions/functions_parser.c` - Function call parser
6. `compiler/stage0/lexer.c` - Keyword token system

**Faydalı Dosyalar:**
- `tests/llvm/08_list/` - List syntax examples
- `tests/llvm/09_map/` - Map syntax examples
- `ARCHITECTURE.md` - MELP keyword-based syntax philosophy

---

## 🚀 Sonraki Adımlar (YZ_213 tamamlandıktan sonra)

1. **YZ_214: String Library** (5 gün) - split, join, replace, trim
2. **YZ_211: Move Semantics** (devam) - Parser issue fix (%70 WIP)
3. **YZ_215: Math Library** (3 gün) - sqrt, pow, sin, cos
4. **YZ_216: C FFI** (1 hafta) - extern "C" function calls

---

## ✅ Hazırlık Checklist

Görev başlamadan önce kontrol et:
- [x] YZ_212 runtime tamamlandı (22/22 tests passing) ✅
- [x] Runtime header files mevcut (mlp_set.h, mlp_queue.h, mlp_stack.h) ✅
- [x] Test infrastructure hazır (tests/llvm/ directory) ✅
- [x] Compiler build system çalışıyor (melp_functions_compiler) ✅
- [x] LLVM backend hazır (functions_codegen_llvm.c) ✅

**Görev YZ_213 başlatılabilir! 🚀**

---

**Üst Akıl Notu:**  
YZ_212 runtime'ı production-ready durumda. YZ_213 ile parser entegrasyonu yapılırsa, Collections Library tam MELP dili özelliği haline gelir. Bu görev **3-4 gün** sürer ve **5 test file** ile doğrulanır. YZ_211 (Move Semantics) düşük öncelikli, YZ_213 önerilir.

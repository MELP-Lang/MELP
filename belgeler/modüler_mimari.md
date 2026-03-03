# MELP Modüler Mimari

## Özet
MELP'in modüler sistemi **plug-and-play** bileşenler ve **bağımlılık yönetimi** sağlar. Stage 2 (PMLP0) için `module`/`external` pattern kullanılır.

## ⚠️ CRITICAL: Modüler ≠ Dosya Bölme

### ❌ YANLIŞ YAKLAŞIM
```
dosya.mlp (500 satır)
→ "Çok büyük, split edelim!"
→ dosya_part1.mlp (250 satır)
→ dosya_part2.mlp (250 satır)
```

### ✅ DOĞRU YAKLAŞIM
```
dosya.mlp (500 satır)
→ "Tek sorumluluk var mı? EVET"
→ Dosya KALIR (500 satır OK)

hashmap.mlp (800 satır)
→ "Hash table implementation" (tek sorumluluk)
→ Dosya KALIR (800 satır OK)
```

### Modülerlik Kriteri: SORUMLULUK, SATIR DEĞİL!

**Single Responsibility Principle:**
- 1 modül = 1 sorumluluk (size irrelevant)
- `function_ptr.mlp`: Function pointer operations (78 satır)
- `closure.mlp`: Closure management (163 satır)
- `mlp_function_ptr.c`: C runtime implementation (221 satır)

**❌ Bölmeyin:**
```mlp
-- BAD: Arbitrary split
-- hashmap_part1.mlp (create, set, get)
-- hashmap_part2.mlp (remove, clear, size)
```

**✅ Bölün:**
```mlp
-- GOOD: Different responsibilities
-- hashmap.mlp (hash table operations)
-- list.mlp (list operations)
-- function_ptr.mlp (function pointer operations)
```

## Terminoloji

**AUTONOMOUS (MELP)** = **MODÜLER (Rust/Python)**

Aynı konsept, farklı isim. Rust/Python'da "modüler" standart terim:
- Rust: `mod hashmap { ... }`, `use crate::hashmap`
- Python: `import hashmap`
- **MELP:** `module hashmap`, `external function ...`

**"AUTONOMOUS" terimi yanıltıcı** - standart "modüler" terimi kullan.

## Temel Pattern

### 1. Module Declaration
```mlp
module function_ptr
    -- Module exports
    export function fn_ptr_create(...) as numeric
        return mlp_fn_ptr_create_internal(...)
    end_function
end_module
```

### 2. External Dependencies
```mlp
module closure
    -- Import from other modules
    external function fn_ptr_create(...) as numeric
    external function fn_ptr_call_0(...) as numeric
    
    -- Use them
    function closure_create(numeric fn_ptr) as numeric
        return fn_ptr
    end_function
end_module
```

### 3. C Runtime Integration
```mlp
module function_ptr
    -- Delegate to C runtime
    external function mlp_fn_ptr_create_internal(...) as numeric
    
    export function fn_ptr_create(...) as numeric
        return mlp_fn_ptr_create_internal(...)
    end_function
end_module
```

## Avantajlar

### ✅ Plug-and-Play
```
HashMap → mlp_hashmap.c  (değiştirilebilir)
List → mlp_list.c        (değiştirilebilir)
FnPtr → mlp_function_ptr.c (değiştirilebilir)
```

### ✅ Bağımlılık Grafiği

**Stdlib Örneği (Runtime Modülleri):**
```
functional.mlp
    └── closure.mlp
            └── function_ptr.mlp
                    └── mlp_function_ptr.c
```

**Compiler Pattern (Her Modül İçin):**
```
lexer.mlp (genel - tüm modüller kullanır)
    ↓
modülx_parser.mlp (modül-spesifik syntax)
    ↓
modülx_codegen.mlp (modül-spesifik LLVM IR)

Örnek:
lexer.mlp → hashmap_parser.mlp → hashmap_codegen.mlp
lexer.mlp → list_parser.mlp → list_codegen.mlp
lexer.mlp → lambda_parser.mlp → lambda_codegen.mlp
```

### ✅ Context Separation
Her modül kendi sorumluluğunu bilir:
- `function_ptr`: Pointer encoding/decoding
- `closure`: Capture management + delegation
- `functional`: Composition + higher-order ops

### ✅ Test Isolation
```bash
# Sadece bir modül test et
./compiler/stage2/stage2_bootstrap stdlib/function_ptr.mlp

# Regression: 18/18 tests pass (hiçbir modül diğerini bozmadı)
```

## Stage 2 Constraints (PMLP0)

❌ **NO import keyword** → `external` declaration kullan  
❌ **NO global constants** → Hardcode literals  
❌ **NO struct** → Opaque numeric handles  
✅ **module/external pattern** → Dependency resolution  
✅ **C runtime delegation** → Complex operations  

## Best Practices

### 1. Opaque Handles
```mlp
-- NOT: struct FunctionPtr { address, arity }
-- YES: numeric fn_ptr (encoded: arity << 56 | address)
```

### 2. External-First
```mlp
-- Declare dependencies at top
external function dep1(...) as numeric
external function dep2(...) as numeric

-- Then implement
function my_func(...) as numeric
    return dep1(dep2(...))
end_function
```

### 3. Runtime Delegation
```mlp
-- Complex logic → C runtime
external function mlp_complex_operation_internal(...) as numeric

export function complex_operation(...) as numeric
    return mlp_complex_operation_internal(...)
end_function
```

## Real Examples

### HashMap Pattern
```mlp
module hashmap
    external function mlp_hash_create_internal() as numeric
    external function mlp_hash_set_internal(...) as numeric
    
    export function hash_create() as numeric
        return mlp_hash_create_internal()
    end_function
end_module
```

### Closure Pattern (Composition)
```mlp
module closure
    external function fn_ptr_call_1(...) as numeric
    
    export function closure_call_1(...) as numeric
        return fn_ptr_call_1(...)  -- Delegate to dependency
    end_function
end_module
```

## Migration Path

**Phase 6 (NOW):** module/external + C runtime  
**Phase 7 (FUTURE):** import/struct + generics  

```mlp
-- Phase 6 (PMLP0):
module closure
    external function fn_ptr_create(...) as numeric
end_module

-- Phase 7 (PMLP1):
import function_ptr

struct Closure {
    fn_ptr as FunctionPtr
    captures as list<numeric>
}
```

## Success Metrics

✅ **585 satır** modüler kod (function_ptr + closure + functional)  
✅ **0 regression** (18/18 tests pass)  
✅ **0 TODO violations** (clean delegation pattern)  
✅ **Plug-and-play** C runtime (mlp_function_ptr.c swappable)  

## YZ'ler İçin Kurallar

### 1. Dosya Boyutuna Bakma
```
500+ satır = PROBLEM DEĞİL
Tek sorumluluk = 1000 satır OK
Çoklu sorumluluk = 100 satır PROBLEM
```

### 2. Split Kararı
```python
if "farklı sorumluluklar var":
    split()  # hashmap + list ayrı modüller
elif "tek sorumluluk":
    keep()   # 800 satır hashmap.c OK
```

### 3. Modüler Checklist
- [ ] Her modül tek bir sorumluluk mu? (✅)
- [ ] Bağımlılıklar `external` ile mi? (✅)
- [ ] Plug-and-play değiştirilebilir mi? (✅)
- [ ] 500 satırdan az mı? (❌ **İLGİSİZ!**)

---

**TL;DR:** 
- Module = namespace + exports
- External = dependencies  
- C runtime = complex ops
- **Modüler ≠ küçük dosyalar**
- **Modüler = ayrı sorumluluklar**
- Result = maintainable + testable + swappable components

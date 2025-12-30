# 3-TODO_LANGUAGE_FEATURES.md

**Hedef:** Modern Dil Özellikleri Ekleme  
**Süre:** 2-3 hafta (Hafta 14-16)  
**Öncelik:** Yüksek  
**Bağımlılık:** 0, 1, 2 TODO'lar (%100)

---

## 📋 GENEL BAKIŞ

TODO #2 sonrası stdlib zengin ama **dil sözdizimi** sınırlı:
- ❌ Generics yok (HashMap<K,V> yazılamaz)
- ❌ Pattern matching yok
- ❌ Closures/lambdas yok
- ❌ Operator overloading yok
- ❌ Compile-time metaprogramming yok
- ❌ Type inference zayıf

Bu TODO **modern dil özelliklerini** ekleyecek.

---

## 🎯 TODO HEDEFLERİ

### Başarı Kriterleri

**TODO tamamlandığında:**
- ✅ Generics (List<T>, HashMap<K,V>)
- ✅ Pattern matching (match/case)
- ✅ Closures & lambdas
- ✅ Operator overloading (custom types için +, -, *, /)
- ✅ Compile-time macros
- ✅ Advanced type inference

---

## 📊 TASK BREAKDOWN

### **Task 1: Generics** (7-8 gün)

**Hedef:** Parametric polymorphism

**Syntax:**
```mlp
-- Generic function
function max<T>(T a; T b) as T where T: Comparable
    if a > b then
        return a
    end_if
    return b
end_function

-- Generic struct
struct List<T>
    T[] items
    numeric size
end_struct

function list_add<T>(ref List<T> list; T item)
    list.items[list.size] = item
    list.size = list.size + 1
end_function

-- Usage
List<numeric> numbers
list_add(numbers, 42)

List<string> names
list_add(names, "Ali")

-- Generic HashMap
struct HashMap<K, V>
    Entry<K,V>[] buckets
    numeric size
end_struct

function hashmap_insert<K,V>(ref HashMap<K,V> map; K key; V value)
    numeric hash = hash_key(key)
    # ... insert logic
end_function

HashMap<string, numeric> ages
hashmap_insert(ages, "Ali", 25)
```

**Compiler Changes:**
```
MELP/C/stage0/modules/
├── parser/
│   └── generics.c      (~400 satır) # Generic syntax parsing
├── typechecker/
│   └── generic_types.c (~500 satır) # Type parameter checking
└── codegen/
    └── monomorphization.c (~600 satır) # C++ template style
```

**Monomorphization Strategy:**
```
List<numeric> → list_numeric (C struct)
List<string> → list_string (C struct)

# Compile-time code generation (Rust style)
```

**Test:**
```bash
# Generic function test
./mlp-gcc tests/generics/max_test.mlp && ./a.out

# Generic struct test
./mlp-gcc tests/generics/list_test.mlp && ./a.out

# Multiple type parameters
./mlp-gcc tests/generics/hashmap_test.mlp && ./a.out
```

---

### **Task 2: Pattern Matching** (5-6 gün)

**Hedef:** Algebraic data types + pattern matching

**Syntax:**
```mlp
-- Enum (sum type)
enum Result<T, E>
    Ok(T)
    Err(E)
end_enum

-- Pattern matching
function process_result(Result<numeric, string> res)
    match res
        case Ok(value):
            yazdir("Success: " + string(value))
        case Err(error):
            yazdir("Error: " + error)
    end_match
end_function

-- Option pattern matching
optional value = some(42)

match value
    case Some(x):
        yazdir("Value: " + string(x))
    case None:
        yazdir("No value")
end_match

-- List pattern matching
list numbers = [1, 2, 3, 4, 5]

match numbers
    case []:
        yazdir("Empty")
    case [head, ...tail]:
        yazdir("Head: " + string(head))
end_match

-- Complex patterns
struct Point
    numeric x
    numeric y
end_struct

match point
    case Point(0, 0):
        yazdir("Origin")
    case Point(x, 0):
        yazdir("On X-axis")
    case Point(0, y):
        yazdir("On Y-axis")
    case Point(x, y):
        yazdir("General point")
end_match
```

**Compiler Changes:**
```
MELP/C/stage0/modules/
├── parser/
│   └── pattern_match.c  (~500 satır)
├── typechecker/
│   └── exhaustiveness.c (~400 satır) # Ensure all cases covered
└── codegen/
    └── match_codegen.c  (~550 satır) # Compile to switch/if-else
```

**Test:**
```bash
# Basic pattern matching
./mlp-gcc tests/pattern/result_test.mlp && ./a.out

# Exhaustiveness check (compile error if missing case)
./mlp-gcc tests/pattern/exhaustive.mlp 2>&1 | grep "non-exhaustive"
```

---

### **Task 3: Closures & Lambdas** (6-7 gün)

**Hedef:** First-class functions

**Syntax:**
```mlp
-- Lambda syntax
function apply(function(numeric) as numeric fn; numeric x) as numeric
    return fn(x)
end_function

numeric result = apply(lambda(x) => x * 2, 5)
# result = 10

-- Closure (captures environment)
function make_counter() as function() as numeric
    numeric count = 0
    return lambda() =>
        count = count + 1
        return count
    end_lambda
end_function

function counter = make_counter()
yazdir(counter())  # 1
yazdir(counter())  # 2
yazdir(counter())  # 3

-- Higher-order functions
list numbers = [1, 2, 3, 4, 5]

list doubled = list.map(numbers, lambda(x) => x * 2)
# doubled = [2, 4, 6, 8, 10]

list evens = list.filter(numbers, lambda(x) => x % 2 == 0)
# evens = [2, 4]

numeric sum = list.reduce(numbers, 0, lambda(acc, x) => acc + x)
# sum = 15
```

**Memory Model:**
```c
// Closure struct
typedef struct {
    void* fn_ptr;          // Function pointer
    void* captured_env;    // Captured variables (heap)
    size_t env_size;       // Environment size
} mlp_closure_t;

// Cleanup via RAII (scope exit → free env)
```

**Compiler Changes:**
```
MELP/C/stage0/modules/
├── parser/
│   └── lambda.c        (~400 satır)
├── typechecker/
│   └── closure_check.c (~450 satır)
└── codegen/
    └── closure_codegen.c (~600 satır) # Generate closure structs
```

**Test:**
```bash
# Lambda test
./mlp-gcc tests/lambda/basic.mlp && ./a.out

# Closure capture test
./mlp-gcc tests/lambda/closure.mlp && ./a.out

# Higher-order functions
./mlp-gcc tests/lambda/map_filter.mlp && ./a.out
```

---

### **Task 4: Operator Overloading** (4-5 gün)

**Hedef:** Custom types için operatör tanımlama

**Syntax:**
```mlp
struct Vector
    numeric x
    numeric y
end_struct

-- Operator overloading
operator +(Vector a; Vector b) as Vector
    Vector result
    result.x = a.x + b.x
    result.y = a.y + b.y
    return result
end_operator

operator *(Vector v; numeric scalar) as Vector
    Vector result
    result.x = v.x * scalar
    result.y = v.y * scalar
    return result
end_operator

operator ==(Vector a; Vector b) as bool
    return a.x == b.x and a.y == b.y
end_operator

-- Usage
Vector v1 = Vector(1.0, 2.0)
Vector v2 = Vector(3.0, 4.0)

Vector sum = v1 + v2          # Vector(4.0, 6.0)
Vector scaled = v1 * 3        # Vector(3.0, 6.0)
bool equal = v1 == v2         # false

-- BigDecimal example (STO ile entegre)
numeric a = 999999999999999999
numeric b = 888888888888888888
numeric c = a + b  # Compiler otomatik BigDecimal + kullanır
```

**Supported Operators:**
```
+, -, *, /, %       # Arithmetic
==, !=, <, >, <=, >= # Comparison
[], []=             # Indexing
()                  # Call
```

**Compiler Changes:**
```
MELP/C/stage0/modules/
├── parser/
│   └── operator_overload.c (~350 satır)
├── typechecker/
│   └── operator_check.c    (~400 satır)
└── codegen/
    └── operator_codegen.c  (~450 satır)
```

**Test:**
```bash
# Vector math test
./mlp-gcc tests/operator/vector.mlp && ./a.out

# Matrix multiplication
./mlp-gcc tests/operator/matrix.mlp && ./a.out
```

---

### **Task 5: Compile-Time Metaprogramming** (6-7 gün)

**Hedef:** Compile-time code generation

**Syntax:**
```mlp
-- Compile-time constants
const MAX_SIZE = 1000

-- Compile-time if
comptime_if is_debug() then
    yazdir("Debug mode")
else
    # Kod tamamen silinir (zero-cost)
end_if

-- Compile-time functions
function comptime_factorial(numeric n) as numeric comptime
    if n <= 1 then
        return 1
    end_if
    return n * comptime_factorial(n - 1)
end_function

const FACT_10 = comptime_factorial(10)  # 3628800 (compile-time hesapla)

-- Macros (hygenic)
macro repeat(times, body)
    for i = 0 to ${times} do
        ${body}
    end_for
end_macro

repeat(5,
    yazdir("Hello")
)

-- Code generation
macro generate_getter(struct_name, field_name)
    function get_${field_name}(${struct_name} obj) as typeof(obj.${field_name})
        return obj.${field_name}
    end_function
end_macro

struct Person
    string name
    numeric age
end_struct

generate_getter(Person, name)  # Creates get_name()
generate_getter(Person, age)   # Creates get_age()
```

**Compiler Changes:**
```
MELP/C/stage0/modules/
├── parser/
│   └── macro.c           (~500 satır)
├── preprocessor/
│   └── comptime_eval.c   (~600 satır) # Compile-time evaluation
└── codegen/
    └── macro_expansion.c (~550 satır)
```

**Test:**
```bash
# Compile-time constant
./mlp-gcc tests/comptime/const.mlp && ./a.out

# Macro expansion
./mlp-gcc tests/comptime/macro.mlp && ./a.out
```

---

### **Task 6: Advanced Type Inference** (4-5 gün)

**Hedef:** Hindley-Milner style type inference

**Özellikler:**
```mlp
-- Type inference (explicit type yok)
let x = 42              # numeric (inferred)
let name = "Ali"        # string (inferred)
let list = [1, 2, 3]    # List<numeric> (inferred)

-- Function return type inference
function add(numeric a; numeric b)
    return a + b  # Return type: numeric (inferred)
end_function

-- Generic type inference
function identity(x)
    return x
end_function

numeric n = identity(42)      # T = numeric
string s = identity("test")   # T = string

-- Complex inference
function map(list, fn)
    # list: List<A>, fn: function(A) as B
    # return: List<B>
    # All inferred!
end_function

let doubled = map([1,2,3], lambda(x) => x * 2)
# doubled: List<numeric>
```

**Compiler Changes:**
```
MELP/C/stage0/modules/
├── typechecker/
│   ├── type_inference.c   (~700 satır) # Hindley-Milner algorithm
│   └── unification.c      (~500 satır) # Type unification
```

**Test:**
```bash
# Type inference test
./mlp-gcc tests/inference/basic.mlp && ./a.out

# Complex inference
./mlp-gcc tests/inference/higher_order.mlp && ./a.out
```

---

## 🔗 BAĞIMLILIKLAR

### Önkoşul:
- **0-TODO_SELFHOSTING.md** (%100)
- **1-TODO_TOOLING_BASICS.md** (%100)
- **2-TODO_STDLIB_EXPAND.md** (%100)
  - Generics stdlib'de kullanılacak (List<T>, HashMap<K,V>)

### Sonraki TODO:
- **4-TODO_IDE_INTEGRATION.md**
  - LSP generics support
  - Pattern matching autocomplete

---

## 🧪 TEST STRATEJİSİ

```bash
tests/language/
├── generics/
│   ├── basic_generic.mlp
│   ├── multiple_params.mlp
│   └── constraints.mlp
├── pattern/
│   ├── match_test.mlp
│   ├── exhaustive_test.mlp
│   └── nested_pattern.mlp
├── lambda/
│   ├── closure.mlp
│   ├── higher_order.mlp
│   └── capture.mlp
├── operator/
│   ├── vector_math.mlp
│   └── matrix_mul.mlp
├── comptime/
│   ├── const_eval.mlp
│   └── macro_expand.mlp
└── inference/
    ├── basic_inference.mlp
    └── complex_inference.mlp
```

---

## 📦 DELIVERABLES

```
MELP/C/stage0/modules/
├── parser/
│   ├── generics.c           (~400 satır)
│   ├── pattern_match.c      (~500 satır)
│   ├── lambda.c             (~400 satır)
│   ├── operator_overload.c  (~350 satır)
│   └── macro.c              (~500 satır)
├── typechecker/
│   ├── generic_types.c      (~500 satır)
│   ├── exhaustiveness.c     (~400 satır)
│   ├── closure_check.c      (~450 satır)
│   ├── operator_check.c     (~400 satır)
│   ├── type_inference.c     (~700 satır)
│   └── unification.c        (~500 satır)
├── preprocessor/
│   └── comptime_eval.c      (~600 satır)
└── codegen/
    ├── monomorphization.c   (~600 satır)
    ├── match_codegen.c      (~550 satır)
    ├── closure_codegen.c    (~600 satır)
    ├── operator_codegen.c   (~450 satır)
    └── macro_expansion.c    (~550 satır)

TOPLAM: ~8900 satır (modüler, <500/dosya)
```

---

## 🎯 BAŞARI KRİTERLERİ

- [ ] List<T>, HashMap<K,V> çalışıyor
- [ ] Pattern matching exhaustiveness check
- [ ] Closures captures environment
- [ ] Operator overloading Vector math
- [ ] Compile-time factorial hesaplanıyor
- [ ] Type inference higher-order functions

---

**Son Güncelleme:** 29 Aralık 2025  
**Hazırlayan:** PD_01 (Danışman)

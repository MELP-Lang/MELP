# 📘 PMLP Feature Card: Generics (Type Parameters)

**Durum:** ⚠️ PARTIAL (Stage 1 — {T} parse edilir, tip kontrolü eksik)  
**Sözdizimi Sürümü:** pmlp1 / Stage1  

**Stage:** Stage 1 Phase 1  
**Complexity:** ⭐⭐⭐ High  
**Estimated Time:** 4-5 days

---

## 🎯 Feature Summary

**What:** Generic types with type parameters for reusable code.

**Why:** Write functions and structs that work with any type.

**How:** Type parameters in angle brackets `<T>`, monomorphization at compile-time.

---

## 📖 Syntax Reference

### Basic Generic Function

```pmlp
function identity<T>(T value) as T
    return value
end_function

-- Usage
numeric x = identity<numeric>(42)
string s = identity<string>("hello")
```

**MELP Format:** `T value` (type first, then name)

### Generic Struct

```pmlp
struct Box<T>
    T value
    boolean is_filled
end_struct

-- Usage
Box<numeric> num_box
num_box.value = 100
num_box.is_filled = true

Box<string> str_box
str_box.value = "hello"
str_box.is_filled = true
```

### Multiple Type Parameters

```pmlp
function pair<T; U>(T first; U second) as tuple<T; U>
    return <first; second>
end_function

-- Usage
tuple<numeric; string> result = pair<numeric; string>(42; "answer")
```

**Note:** Semicolon `;` separates type parameters, just like function parameters.

### Generic with Struct

```pmlp
struct Pair<T; U>
    T first
    U second
end_struct

function create_pair<T; U>(T a; U b) as Pair<T; U>
    Pair<T; U> result
    result.first = a
    result.second = b
    return result
end_function
```

---

## 🔍 Detailed Specification

### 1. Type Parameter Syntax

**Declaration:**
```
<TypeParam1; TypeParam2; ...>
```

**Rules:**
- Type parameter names: Single uppercase letter (T, U, V, E) or PascalCase
- Separator: `;` (semicolon)
- Position: After function/struct name, before parameters

### 2. Function Generics

**Syntax:**
```pmlp
function name<T>(T param) as T
    -- body
end_function
```

**Example:**
```pmlp
function swap<T>(T a; T b) as tuple<T; T>
    return <b; a>
end_function

tuple<numeric; numeric> result = swap<numeric>(10; 20)
```

### 3. Struct Generics

**Syntax:**
```pmlp
struct Name<T>
    T field_name
    -- other fields
end_struct
```

**Example:**
```pmlp
struct Container<T>
    T item
    numeric size
    string label
end_struct

Container<numeric> c1
c1.item = 42
c1.size = 1
c1.label = "numbers"
```

### 4. Type Constraints (Optional - Stage 1.5+)

**Syntax:**
```pmlp
function compare<T>(T a; T b) as boolean where T: Comparable
    return a > b
end_function
```

**Built-in Traits:**
- `Comparable` - Supports `<`, `>`, `==`
- `Numeric` - Supports arithmetic operations
- `Printable` - Can be printed

---

## 🧪 Test Cases

See: `GOLDEN_TEST_SUITE/generics_tests/`

### Test 1: Basic Generic Function
```pmlp
function identity<T>(T x) as T
    return x
end_function

function main() as numeric
    print(identity<numeric>(42))
    print(identity<string>("hello"))
    return 0
end_function
```

### Test 2: Generic Struct
```pmlp
struct Box<T>
    T value
end_struct

function main() as numeric
    Box<numeric> b
    b.value = 100
    print(b.value)
    return 0
end_function
```

---

## ⚙️ Implementation Notes

**Compilation Strategy:** Monomorphization
- Each `identity<numeric>` creates separate function
- Each `Box<string>` creates separate struct
- No runtime overhead

**Type Inference:** Not in Stage 1
```pmlp
-- ❌ Not supported (yet)
numeric x = identity(42)  -- Type inference

-- ✅ Must be explicit
numeric x = identity<numeric>(42)
```

---

## 🚨 MELP Syntax Rules

**✅ Correct:**
```pmlp
function swap<T>(T a; T b) as tuple<T; T>
    return <b; a>
end_function
```

**❌ Wrong (C++ style):**
```pmlp
template<typename T>
T swap(T a, T b) {
    return {b, a};
}
```

**❌ Wrong (Rust style):**
```pmlp
fn swap<T>(a: T, b: T) -> (T, T) {
    (b, a)
}
```

**MELP way:**
- `function` keyword
- Type first: `T a` not `a: T`
- Semicolon separator: `T a; T b` not `T a, T b`
- `as` for return type: `as T` not `-> T`
- `end_function` terminator

---

## 📋 Success Criteria

- ✅ Generic functions compile
- ✅ Generic structs compile
- ✅ Multiple type parameters work
- ✅ Type instantiation is explicit
- ✅ All golden tests pass

---

**Reference:** GOLDEN_TEST_SUITE/generics_tests/

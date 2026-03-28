# 📘 MLP Feature Card: Generics (Type Parameters)

**Durum:** ⚠️ PARTIAL (Stage 1 — {T} parse edilir, tip kontrolü eksik)  
**Sözdizimi Sürümü:** mlp1 / Stage1  

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

```mlp
T function identity<T>(T value)
    return value
end function

-- Usage
numeric x = identity<numeric>(42)
string s = identity<string>("hello")
```

**MELP Format:** `T value` (type first, then name)

### Generic Struct

```mlp
struct Box<T>
    T value
    boolean is_filled
end struct

-- Usage
Box<numeric> num_box
num_box.value = 100
num_box.is_filled = true

Box<string> str_box
str_box.value = "hello"
str_box.is_filled = true
```

### Multiple Type Parameters

```mlp
tuple<T; U> function pair<T; U>(T first; U second)
    return <first; second>
end function

-- Usage
tuple<numeric; string> result = pair<numeric; string>(42; "answer")
```

**Note:** Semicolon `;` separates type parameters, just like function parameters.

### Generic with Struct

```mlp
struct Pair<T; U>
    T first
    U second
end struct

Pair<T; U> function create_pair<T; U>(T a; U b)
    Pair<T; U> result
    result.first = a
    result.second = b
    return result
end function
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
```mlp
T function name<T>(T param)
    -- body
end function
```

**Example:**
```mlp
tuple<T; T> function swap<T>(T a; T b)
    return <b; a>
end function

tuple<numeric; numeric> result = swap<numeric>(10; 20)
```

### 3. Struct Generics

**Syntax:**
```mlp
struct Name<T>
    T field_name
    -- other fields
end struct
```

**Example:**
```mlp
struct Container<T>
    T item
    numeric size
    string label
end struct

Container<numeric> c1
c1.item = 42
c1.size = 1
c1.label = "numbers"
```

### 4. Type Constraints (Optional - Stage 1.5+)

**Syntax:**
```mlp
boolean function compare<T>(T a; T b) where T: Comparable
    return a > b
end function
```

**Built-in Traits:**
- `Comparable` - Supports `<`, `>`, `==`
- `Numeric` - Supports arithmetic operations
- `Printable` - Can be printed

---

## 🧪 Test Cases

See: `GOLDEN_TEST_SUITE/generics_tests/`

### Test 1: Basic Generic Function
```mlp
T function identity<T>(T x)
    return x
end function

function main()
    print(identity<numeric>(42))
    print(identity<string>("hello"))
end function
```

### Test 2: Generic Struct
```mlp
struct Box<T>
    T value
end struct

function main()
    Box<numeric> b
    b.value = 100
    print(b.value)
end function
```

---

## ⚙️ Implementation Notes

**Compilation Strategy:** Monomorphization
- Each `identity<numeric>` creates separate function
- Each `Box<string>` creates separate struct
- No runtime overhead

**Type Inference:** Not in Stage 1
```mlp
-- ❌ Not supported (yet)
numeric x = identity(42)  -- Type inference

-- ✅ Must be explicit
numeric x = identity<numeric>(42)
```

---

## 🚨 MELP Syntax Rules

**✅ Correct:**
```mlp
tuple<T; T> function swap<T>(T a; T b)
    return <b; a>
end function
```

**❌ Wrong (C++ style):**
```mlp
template<typename T>
T swap(T a, T b) {
    return {b, a};
}
```

**❌ Wrong (Rust style):**
```mlp
fn swap<T>(a: T, b: T) -> (T, T) {
    (b, a)
}
```

**MELP way:**
- `function` keyword
- Type first: `T a` not `a: T`
- Semicolon separator: `T a; T b` not `T a, T b`
- Return type before `function`: `T function f()` not `function f() -> T`
- `end function` terminator

---

## 📋 Success Criteria

- ✅ Generic functions compile
- ✅ Generic structs compile
- ✅ Multiple type parameters work
- ✅ Type instantiation is explicit
- ✅ All golden tests pass

---

**Reference:** GOLDEN_TEST_SUITE/generics_tests/

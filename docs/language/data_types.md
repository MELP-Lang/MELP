# MELP Data Types Reference

**Version:** 1.0  
**Last Updated:** December 10, 2025  
**Language:** English

---

## Overview

MELP simplifies data types to minimize cognitive overhead. Users work with only a few high-level types, while the compiler optimizes storage internally (see [TTO.md](TTO.md)).

---

## Primitive Types

### Numeric

All numbers are represented as `numeric`:

```mlp
numeric integer = 42
numeric negative = -100
numeric decimal = 3.14159
numeric scientific = 1.5e10
numeric huge = 10 ^ 100
```

**Internal Optimization (TTO):**
| Value | Internal Type | Storage |
|-------|---------------|---------|
| Small integers | `int64` | Register |
| Decimals | `double` | XMM Register |
| Very large | `BigDecimal` | Heap |

Users don't need to choose - the compiler decides.

### String

All text is represented as `string`:

```mlp
string greeting = "Hello, World!"
string empty = ""
string unicode = "Merhaba! 你好! مرحبا!"
```

**Internal Optimization:**
| Length | Internal Type | Storage |
|--------|---------------|---------|
| ≤23 bytes | SSO (inline) | Stack |
| >23 bytes | Pointer | Heap |
| Literal | Reference | .rodata |

### Boolean

True/false values:

```mlp
boolean yes = true
boolean no = false
boolean result = (5 > 3)  -- true
```

---

## Collections

MELP provides three collection types with distinct syntax:

| Type | Syntax | Homogeneous? | Mutable? | Best For |
|------|--------|--------------|----------|----------|
| **Array** | `[]` | ✅ Yes | ✅ Yes | Same-type sequences |
| **List** | `()` | ❌ No | ✅ Yes | Mixed-type data |
| **Tuple** | `<>` | ❌ No | ❌ No | Fixed records |

### Array

Homogeneous, mutable collection using square brackets:

```mlp
-- Declaration
numeric[] numbers = [1, 2, 3, 4, 5]
string[] names = ["Alice", "Bob", "Charlie"]

-- Access (square brackets)
numeric first = numbers[0]
numbers[0] = 100  -- ✅ OK (mutable)

-- Length
numeric len = numbers.length()
```

**Rules:**
- All elements must be same type
- Zero-indexed
- Square brackets for both declaration and access

### List

Heterogeneous, mutable collection using parentheses:

```mlp
-- Declaration (semicolon separated, trailing semicolon required)
person() = ("Alice"; 25; true;)
data() = (1; "hello"; 3.14; false;)

-- Access (parentheses, NO SPACE before!)
string name = person(0)    -- ✅ Correct
numeric age = person(1)    -- ✅ Correct
string name = person (0)   -- ❌ SYNTAX ERROR!

-- Modification
person(0) = "Bob"          -- ✅ OK (mutable)

-- Add element
person.add("new item")
```

**Critical Rules:**

1. **Trailing semicolon is REQUIRED:**
```mlp
list() = (1; 2; 3;)   -- ✅ Correct
list() = (1; 2; 3)    -- ❌ ERROR!
list() = (42;)        -- ✅ Single element
list() = ()           -- ✅ Empty list
```

2. **NO SPACE before parentheses:**
```mlp
list(0)               -- ✅ Index access
list (0)              -- ❌ SYNTAX ERROR!
```

3. **List variable syntax:**
```mlp
a() = (1; 2; 3;)      -- ✅ List variable (has `()` after name)
a = (1; 2; 3;)        -- ❌ ERROR! Use `a()` for lists
```

### Tuple

Heterogeneous, immutable collection using angle brackets:

```mlp
-- Declaration
point<> = <10, 20>
record<> = <"Alice", 25, true>

-- Access (angle brackets)
numeric x = point<0>
numeric y = point<1>

-- Immutable!
point<0> = 99         -- ❌ ERROR! Tuples cannot be modified
```

**Use Cases:**
- Function return values
- Fixed records (coordinates, RGB values)
- Data that shouldn't change

```mlp
-- Function returning tuple
function minmax(numeric[] arr) returns <numeric, numeric>
    return <min(arr), max(arr)>
end function

-- Destructuring
<min_val, max_val> = minmax([3, 1, 4, 1, 5])
```

---

## Nullable Types

### Null Value

```mlp
numeric x = null
string s = null

if x == null then
    print("x is null")
end if
```

### Nullable Type Annotation

Use `?` suffix for nullable types:

```mlp
numeric? maybeNum = null
string? maybeStr = null

-- Can be assigned null
maybeNum = 42
maybeNum = null  -- ✅ OK
```

### Null Coalescing Operator

`??` returns the first non-null value:

```mlp
numeric result = maybeNull ?? defaultValue
numeric chained = a ?? b ?? c ?? 999
```

### Optional Chaining

`?.` safely accesses members, returning null if object is null:

```mlp
string? name = person?.name    -- null if person is null
numeric? age = user?.profile?.age  -- chained
```

---

## Type Inference

MELP can infer types in some contexts:

```mlp
-- Explicit type
numeric x = 10

-- Multiple assignment with inference
a, b, c = 10, "hello", true
-- a is numeric, b is string, c is boolean
```

---

## Type Checking

### Comparison

Types must be compatible for comparison:

```mlp
if x == 5 then        -- ✅ numeric == numeric
if s == "hello" then  -- ✅ string == string
if x == "hello" then  -- ❌ Type mismatch error
```

### Arithmetic

Numeric operations require numeric types:

```mlp
numeric sum = a + b         -- ✅ OK
string concat = s1 + s2     -- ✅ String concatenation
numeric bad = x + "hello"   -- ❌ Type error
```

---

## Structs

User-defined composite types:

```mlp
struct Person
    string name
    numeric age
    boolean active
end struct

-- Usage
Person p
p.name = "Alice"
p.age = 25
p.active = true

-- Access
print(p.name)
```

### Nested Structs

```mlp
struct Address
    string street
    string city
end struct

struct Person
    string name
    Address address
end struct

Person p
p.address.city = "Istanbul"
```

---

## Pointers

For low-level memory operations:

```mlp
numeric x = 42
numeric* ptr = &x       -- Get address
numeric value = *ptr    -- Dereference

*ptr = 100              -- Modify through pointer
print(x)                -- Now 100
```

**Warning:** Use pointers carefully. They bypass MELP's safety features.

---

## Type Conversion

### Implicit

Safe conversions happen automatically:

```mlp
numeric x = 42
-- Internally: int64 may become double in division
numeric y = x / 3  -- Result is double internally
```

### String to Numeric

```mlp
string s = "42"
numeric n = to_numeric(s)  -- Explicit conversion
```

### Numeric to String

```mlp
numeric n = 42
string s = to_string(n)    -- "42"
```

---

## Memory Management

### Stack vs Heap

| Type | Where | Managed |
|------|-------|---------|
| Small numeric | Stack | Automatic |
| Small string (SSO) | Stack | Automatic |
| Large string | Heap | GC |
| Array | Heap | GC |
| List | Heap | GC |
| Struct | Stack/Heap | Depends |

### Garbage Collection

MELP uses automatic garbage collection for heap-allocated data. Users don't need to free memory manually.

---

## Quick Reference

### Type Summary

| Type | Example | Mutable | Nullable |
|------|---------|---------|----------|
| `numeric` | `42`, `3.14` | - | With `?` |
| `string` | `"hello"` | - | With `?` |
| `boolean` | `true`, `false` | - | With `?` |
| `numeric[]` | `[1, 2, 3]` | ✅ Yes | - |
| `list()` | `(1; 2; 3;)` | ✅ Yes | - |
| `tuple<>` | `<1, 2, 3>` | ❌ No | - |
| `struct` | User-defined | ✅ Yes | - |

### Collection Syntax

| Operation | Array | List | Tuple |
|-----------|-------|------|-------|
| Declare | `[1, 2, 3]` | `(1; 2; 3;)` | `<1, 2, 3>` |
| Access | `arr[0]` | `list(0)` | `tuple<0>` |
| Modify | ✅ `arr[0] = x` | ✅ `list(0) = x` | ❌ Immutable |

---

*This document is part of the MELP Language Documentation.*

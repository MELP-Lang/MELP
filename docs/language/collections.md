# MELP Collections Reference

**Version:** 1.0  
**Last Updated:** December 10, 2025  
**Language:** English

---

## Overview

MELP provides three collection types with distinct syntax and semantics. Each collection type has its own bracket style for both declaration and access.

| Type | Brackets | Homogeneous | Mutable | Storage |
|------|----------|-------------|---------|---------|
| **Array** | `[ ]` | ✅ Yes | ✅ Yes | Stack/Heap |
| **List** | `( )` | ❌ No | ✅ Yes | Heap |
| **Tuple** | `< >` | ❌ No | ❌ No | Stack ⚡ |

---

## Array `[ ]`

Arrays are homogeneous (same-type) mutable collections.

### Declaration

```mlp
-- Typed array declaration
numeric[] numbers = [1, 2, 3, 4, 5]
string[] names = ["Alice", "Bob", "Charlie"]
boolean[] flags = [true, false, true]

-- Empty array
numeric[] empty = []
```

### Access

```mlp
-- Read (square brackets)
numeric first = numbers[0]
numeric last = numbers[4]

-- Write
numbers[0] = 100
```

### Methods

```mlp
numeric len = numbers.length()
numbers.add(6)           -- Append element
numbers.remove(0)        -- Remove at index
boolean has = numbers.contains(3)
```

### Iteration

```mlp
for num in numbers
    print(num)
end for

for i = 0 to numbers.length() - 1
    print(numbers[i])
end for
```

### Use Cases

- Fixed-type sequences
- Performance-critical operations
- Mathematical operations

---

## List `( )`

Lists are heterogeneous (mixed-type) mutable collections.

### Declaration

```mlp
-- List declaration (parentheses after identifier)
person() = ("Alice"; 25; true;)
data() = (1; "hello"; 3.14;)

-- Empty list
empty() = ()

-- Single element (trailing semicolon required!)
single() = (42;)
```

### ⚠️ Critical Syntax Rules

#### Rule 1: Semicolon Separator + Trailing Semicolon

```mlp
list() = (10; 20; 30;)     -- ✅ CORRECT
list() = (10; 20; 30)      -- ❌ ERROR: Missing trailing semicolon
list() = (42;)             -- ✅ Single element
list() = (42)              -- ❌ ERROR: Missing trailing semicolon
list() = ()                -- ✅ Empty list
list() = (1, 2, 3)         -- ❌ ERROR: Use semicolons, not commas
```

#### Rule 2: NO SPACE Before Access Parentheses

```mlp
list(0)                    -- ✅ CORRECT: Index access
list (0)                   -- ❌ SYNTAX ERROR: No space allowed!
```

This rule enforces bitişik yazım (adjacent writing) for function-like index access.

#### Rule 3: List Variable Identifier

```mlp
-- List variables use () after the identifier
myList() = (1; 2; 3;)      -- ✅ Declares a list variable

-- Regular variables cannot hold list literals
a = (1; 2; 3;)             -- ❌ ERROR: Use a() for lists
a = (5)                    -- ✅ OK: Math parentheses, a = 5
```

### Access

```mlp
-- Read (parentheses, NO SPACE!)
string name = person(0)    -- "Alice"
numeric age = person(1)    -- 25

-- Write
person(0) = "Bob"
person(1) = 30
```

### Methods

```mlp
person.add("new value")    -- Append
person.remove(2)           -- Remove at index
numeric len = person.length()
```

### Turkish Decimal Support

Lists support Turkish decimal notation (comma as decimal separator):

```mlp
prices() = (10,5; 20,3; 100,99;)   -- [10.5, 20.3, 100.99]
```

### Use Cases

- Mixed-type records
- Dynamic data structures
- JSON-like data

---

## Tuple `< >`

Tuples are heterogeneous immutable collections stored on the stack.

### Declaration

```mlp
-- Tuple declaration
point<> = <10, 20>
record<> = <"Alice", 25, true>
coordinates<> = <x, y, z>
```

### Access

```mlp
-- Read (angle brackets)
numeric x = point<0>       -- 10
numeric y = point<1>       -- 20

-- Write NOT ALLOWED!
point<0> = 99              -- ❌ ERROR: Tuples are immutable!
```

### Function Returns

Tuples are perfect for returning multiple values:

```mlp
function minmax(numeric[] arr) returns <numeric, numeric>
    numeric min_val = arr[0]
    numeric max_val = arr[0]
    for num in arr
        if num < min_val then min_val = num end if
        if num > max_val then max_val = num end if
    end for
    return <min_val, max_val>
end function

-- Destructuring assignment
<minimum, maximum> = minmax([3, 1, 4, 1, 5, 9])
print(minimum)  -- 1
print(maximum)  -- 9
```

### Use Cases

- Function return values
- Coordinates (x, y, z)
- Fixed records
- Immutable data

---

## Comparison Table

| Feature | Array `[]` | List `()` | Tuple `<>` |
|---------|------------|-----------|------------|
| **Separator** | `,` | `;` | `,` |
| **Trailing marker** | None | `;` required | None |
| **Same types only** | ✅ Yes | ❌ No | ❌ No |
| **Can modify** | ✅ Yes | ✅ Yes | ❌ No |
| **Storage** | Heap | Heap | Stack |
| **Performance** | Good | Good | ⚡ Best |
| **Declaration** | `type[]` | `name()` | `name<>` |

---

## Memory Layout

```
ARRAY (Heap allocated):
┌────────────────────────────────────┐
│ Header: length, capacity, type     │
├────────────────────────────────────┤
│ Element 0                          │
│ Element 1                          │
│ Element 2                          │
│ ...                                │
└────────────────────────────────────┘

LIST (Heap allocated, type-tagged):
┌────────────────────────────────────┐
│ Header: length, capacity           │
├────────────────────────────────────┤
│ Element 0: <type_tag, value>       │
│ Element 1: <type_tag, value>       │
│ Element 2: <type_tag, value>       │
│ ...                                │
└────────────────────────────────────┘

TUPLE (Stack allocated, fixed):
┌────────────────────────────────────┐
│ Element 0 (inline)                 │
│ Element 1 (inline)                 │
│ Element 2 (inline)                 │
└────────────────────────────────────┘
```

---

## Common Patterns

### Array of Structs

```mlp
struct Person
    string name
    numeric age
end struct

Person[] people = [p1, p2, p3]
for person in people
    print(person.name)
end for
```

### Nested Collections

```mlp
-- Array of arrays (2D)
numeric[][] matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
numeric value = matrix[1][2]  -- 6

-- List containing tuples
data() = (<1, "a">; <2, "b">; <3, "c">;)
```

### Returning Multiple Values

```mlp
function divmod(numeric a, numeric b) returns <numeric, numeric>
    return <a / b, a % b>
end function

<quotient, remainder> = divmod(17, 5)
-- quotient = 3, remainder = 2
```

---

## Error Examples

### Common List Mistakes

```mlp
-- ❌ Missing trailing semicolon
list() = (1; 2; 3)

-- ❌ Space before access
list (0)

-- ❌ Comma instead of semicolon
list() = (1, 2, 3)

-- ❌ Missing parentheses in identifier
list = (1; 2; 3;)
```

### Common Tuple Mistakes

```mlp
-- ❌ Trying to modify
tuple<0> = 5

-- ❌ Missing <> in identifier
tuple = <1, 2, 3>
```

---

## Quick Reference

```mlp
-- ARRAY: Square brackets, comma-separated, same types
numeric[] arr = [1, 2, 3]
arr[0] = 10

-- LIST: Parentheses, semicolon-separated, trailing semicolon
data() = (1; "two"; 3.0;)
data(0) = 10

-- TUPLE: Angle brackets, comma-separated, immutable
point<> = <10, 20>
x = point<0>
```

---

*This document is part of the MELP Language Documentation.*

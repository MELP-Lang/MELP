# MELP Language Reference — English (MLP)

**Version:** 1.0  
**Date:** 2026-03-30  
**Stage coverage:** Stage 0 ✅ + Stage 1 ✅ + Stage 2 (planned)

> This reference covers **human-readable MLP syntax** with English keywords.  
> The compiler internally transforms MLP to **pmlp** (underscore-joined tokens) before compilation.  
> For Turkish MLP syntax, see [belgeler/dil/referans.md](../belgeler/dil/referans.md).

---

## § 1 — Critical Rules

| Rule | Correct | Wrong |
|---|---|---|
| Parameter separator | `func(a; b; c)` | `func(a, b, c)` |
| Array literal | `[1; 2; 3]` | `[1, 2, 3]` |
| No statement terminator | `x = 42` | `x = 42;` |
| Block terminators (two words) | `end function`, `end if`, `end loop` | `end_function`, `endfunction` |
| `if` requires `then` | `if x > 0 then` | `if x > 0` |
| `else if` (two words) | `else if x > 0 then` | `elif x > 0` |
| Exit loop | `exit` | `break` |
| Comments | `-- comment` | `// comment` |
| Logical operators | `and`, `or`, `not` | `&&`, `\|\|`, `!` |
| Comparison (not equal) | `!=` | `<>` |
| Return type prefix | `numeric function add(...)` | `add(...) returns numeric` |
| String type | `string` | `text`, `str` (as type) |
| Boolean literals | `true`, `false` | `True`, `False`, `1`, `0` |

---

## § 2 — Types

### Primitive Types

```mlp
numeric x = 42
numeric pi = 3.14
string name = "Ali"
boolean flag = true
boolean done = false
```

### Type Inference

| Value | Internal | Notes |
|---|---|---|
| `42` | `int64` | Integers −2⁶³..2⁶³−1 |
| `3.14` | `double` | Up to 15 significant digits |
| very large | `BigDecimal` | Arbitrary precision |

### Null / None

```mlp
string name = null

none function greet(string name)
    print("Hello, " + name)
end function
```

---

## § 3 — Variables

```mlp
numeric x = 42
string s = "hello"
boolean b = true

-- Reassignment (no keyword needed)
x = 100
```

### Constants

```mlp
const numeric MAX = 1000
const string APP = "MELP"
```

---

## § 4 — Operators

### Arithmetic

```mlp
x + y    x - y    x * y    x / y    x mod y
```

### Comparison

```mlp
x == y    x != y    x < y    x > y    x <= y    x >= y
```

### Logical / String

```mlp
x and y
x or y
not x
string full = "Hello, " + name + "!"
```

### Range

```mlp
range(1; 10)    -- 1..9 (exclusive end)
```

---

## § 5 — Control Flow

```mlp
if x > 0 then
    print("positive")
else if x < 0 then
    print("negative")
else
    print("zero")
end if
```

> `else if` is always two separate words.

---

## § 6 — Loops

### While-style

```mlp
numeric i = 0
loop i < 10
    print(i)
    i = i + 1
end loop
```

### Infinite / do-while

```mlp
loop
    if done then exit
    process()
end loop

-- do-while: condition after end loop
loop
    print(i)
    i = i + 1
end loop i < 0    -- runs at least once
```

### Range loops

```mlp
loop i = 0 to 10
    print(i)      -- 0..9
end loop

loop i = 10 downto 0
    print(i)      -- 10..1
end loop

loop i = 0 to 20 step 2
    print(i)      -- 0, 2, 4, ..., 18
end loop
```

### foreach (Stage 1)

```mlp
numeric[] nums = [1; 2; 3; 4; 5]
for each num in nums
    print(num)
end for

-- with filter (Stage 2)
for each num in nums where num mod 2 == 0
    print(num)
end for
```

### Exit / Continue

```mlp
exit        -- exit loop
continue    -- next iteration
```

---

## § 7 — Functions

```mlp
-- With return value
numeric function add(numeric a; numeric b)
    return a + b
end function

-- No return value
none function greet(string name)
    print("Hello, " + name)
end function

-- Entry point
none function main()
    print("Hello, World!")
end function

-- Alternate return type form
function add(numeric a; numeric b) as numeric
    return a + b
end function
```

---

## § 8 — Structs

```mlp
struct Point
    numeric x
    numeric y
end struct

-- Instantiation
Point p = Point { x: 3; y: 4; }

-- Method (outside struct)
numeric function Point.distance()
    return this.x + this.y
end function

print(p.distance())    -- 7
```

---

## § 9 — Arrays

```mlp
numeric[] nums = [1; 2; 3; 4; 5]
print(nums[0])     -- 1 (0-based)
nums[0] = 100
print(nums.length) -- 5
nums.push(6)
numeric last = nums.pop()
```

---

## § 10 — Hashmaps

```mlp
hashmap<string; numeric> scores = {}
scores["alice"] = 95

if scores.has("alice") then
    print(scores["alice"])
end if

scores.delete("alice")

for each k in scores.keys()
    print(k + ": " + str(scores[k]))
end for
```

---

## § 11 — Enums (Stage 1)

```mlp
enum Color
    RED
    GREEN
    BLUE
end enum

Color c = Color.RED
```

---

## § 12 — Interfaces (Stage 1)

```mlp
interface Shape
    numeric function area()
end interface

struct Circle implements Shape
    numeric radius
end struct

numeric function Circle.area()
    return 3.14 * this.radius * this.radius
end function
```

---

## § 13 — Generics (Stage 1)

```mlp
T function identity<T>(T value)
    return value
end function

numeric x = identity<numeric>(42)
```

---

## § 14 — Lambdas (Stage 1)

```mlp
numeric function double = lambda(numeric x)
    return x * 2
end lambda

print(double(5))    -- 10

-- Shorthand (single param)
numeric triple = x -> x * 3
```

---

## § 15 — Error Handling (Stage 1)

```mlp
try
    numeric result = risky()
    print(result)
catch
    print("Error: " + error.message)
finally
    cleanup()
end try
```

---

## § 16 — Pattern Matching (Stage 1)

```mlp
string function classify(numeric x)
    return match x
        case 1 then "one"
        case 2 then "two"
        else "many"
    end match
end function
```

---

## § 17 — Operator Overloading (Stage 1)

```mlp
Vector function Vector.operator+(Vector other)
    Vector result
    result.x = this.x + other.x
    result.y = this.y + other.y
    return result
end function
```

---

## § 18 — Modules (Stage 2)

```mlp
module math
    numeric export function square(numeric x)
        return x * x
    end function
end module

import math

none function main()
    print(math.square(10))
end function
```

---

## § 19 — Async / Await (Stage 2)

```mlp
string async function fetchData(string url)
    string response = await httpGet(url)
    return response
end function
```

---

## § 20 — Standard Library

See [stdlib.md](stdlib.md) for the complete standard library reference.

### Quick Reference

| Category | Functions |
|---|---|
| I/O | `print`, `println`, `read_line`, `read_file`, `write_file`, `append_file`, `read_arg` |
| Conversion | `str`, `to_number`, `to_boolean` |
| Math | `abs`, `min`, `max`, `sqrt`, `floor`, `ceil` |
| String | `len`, `upper`, `lower`, `trim`, `contains`, `starts_with`, `ends_with`, `split`, `join`, `replace`, `substring`, `index_of` |
| Array | `.push`, `.pop`, `.length` |
| Hashmap | `.has`, `.delete`, `.keys`, `.length` |

---

## § 21 — Comments

```mlp
-- single-line comment

---/
Multi-line doc comment.
/---
```

---

## Appendix A — Stage Feature Table

| Feature | Stage 0 | Stage 1 | Stage 2 |
|---|---|---|---|
| Variables, types, literals | ✅ | ✅ | ✅ |
| if / else if / else | ✅ | ✅ | ✅ |
| loop (while/range/foreach) | ✅ | ✅ | ✅ |
| Functions | ✅ | ✅ | ✅ |
| Structs + methods | ✅ | ✅ | ✅ |
| Arrays, hashmaps | ✅ | ✅ | ✅ |
| Stdlib | ✅ | ✅ | ✅ |
| enum | ❌ | ✅ | ✅ |
| Interfaces | ❌ | ✅ | ✅ |
| Generics | ❌ | ✅ | ✅ |
| Lambda / closures | ❌ | ✅ | ✅ |
| try / catch / finally | ❌ | ✅ | ✅ |
| match / case | ❌ | ✅ | ✅ |
| Operator overloading | ❌ | ✅ | ✅ |
| Testing framework | ❌ | ✅ | ✅ |
| Modules / import | ❌ | ❌ | ✅ |
| async / await | ❌ | ❌ | ✅ |
| foreach with where filter | ❌ | ❌ | ✅ |

---

## Appendix B — Block Terminator Table

| Block | Terminator |
|---|---|
| `function` | `end function` |
| `if` | `end if` |
| `loop` | `end loop` |
| `for each` | `end for` |
| `struct` | `end struct` |
| `enum` | `end enum` |
| `interface` | `end interface` |
| `implement` | `end implement` |
| `match` | `end match` |
| `try` | `end try` |
| `lambda` | `end lambda` |
| `module` | `end module` |

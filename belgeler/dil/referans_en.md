# MELP Language Reference — English (MLP)

**Version:** 1.0  
**Date:** 2026-03-30  
**Stage coverage:** Stage 0 ✅ + Stage 1 ✅ + Stage 2 (planned)  
**Canonical source:** `ORTAK/dil/MLP_SYNTAX_EN.md`

> This reference covers **human-readable MLP syntax** with English keywords.  
> The compiler internally transforms MLP to **pmlp** (underscore-joined tokens) before compilation.

---

## § 1 — Critical Rules

These rules apply across all stages and are non-negotiable:

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
| very large numbers | `BigDecimal` | Arbitrary precision |

### Null

```mlp
string name = null
```

### None (void return)

```mlp
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

-- Reassignment
x = 100
s = "world"
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
x + y
x - y
x * y
x / y
x mod y     -- modulo (not %)
```

### Comparison

```mlp
x == y
x != y
x < y
x > y
x <= y
x >= y
```

### Logical

```mlp
x and y
x or y
not x
```

### String Concatenation

```mlp
string full = "Hello, " + name + "!"
```

### Range

```mlp
range(1; 10)    -- 1..9 (exclusive end)
```

---

## § 5 — Control Flow

### if / else if / else

```mlp
if x > 0 then
    print("positive")
else if x < 0 then
    print("negative")
else
    print("zero")
end if
```

> `else if` is always two words. `elif` is not valid MLP.

---

## § 6 — Loops

### While-style loop

```mlp
numeric i = 0
loop i < 10
    print(i)
    i = i + 1
end loop
```

### Infinite loop

```mlp
loop
    if done then exit
    process()
end loop
```

### Do-while (condition after `end loop`)

```mlp
numeric i = 0
loop
    print(i)
    i = i + 1
end loop i < 0    -- runs at least once; stop when false
```

### Counting up

```mlp
loop i = 0 to 10
    print(i)    -- 0..9
end loop
```

### Counting down

```mlp
loop i = 10 downto 0
    print(i)    -- 10..1
end loop
```

### Step

```mlp
loop i = 0 to 20 step 2
    print(i)    -- 0, 2, 4, ..., 18
end loop
```

### foreach (Stage 1)

```mlp
numeric[] nums = [1; 2; 3; 4; 5]
for each num in nums
    print(num)
end for
```

With filter:

```mlp
for each num in nums where num mod 2 == 0
    print(num)
end for
```

### Exit and Continue

```mlp
exit        -- exit current loop
continue    -- skip to next iteration
```

### Named loops (labels)

```mlp
loop outer i = 0 to 10
    loop inner j = 0 to 10
        if i == 5 and j == 5 then
            exit outer
        end if
    end loop
end loop outer
```

---

## § 7 — Functions

### Basic definition

```mlp
numeric function add(numeric a; numeric b)
    return a + b
end function
```

### No return value

```mlp
none function greet(string name)
    print("Hello, " + name)
end function
```

### Program entry point

```mlp
none function main()
    print("Hello, World!")
end function
```

### Multiple parameters (semicolon separator)

```mlp
string function format(string template; numeric value; boolean flag)
    return template + str(value)
end function
```

### Alternate return type form (`as`)

```mlp
function add(numeric a; numeric b) as numeric
    return a + b
end function
```

Both forms are valid. The normalizer converts `as TYPE` to the prefixed form.

---

## § 8 — Structs

### Definition

```mlp
struct Point
    numeric x
    numeric y
end struct
```

### Instantiation

```mlp
Point p
p.x = 10
p.y = 20
```

### Struct literal

```mlp
Point p = Point { x: 3; y: 4; }
```

### Methods

```mlp
numeric function Point.distance()
    return this.x + this.y
end function

print(p.distance())    -- 7
```

### Methods inside struct

```mlp
struct Rectangle
    numeric width
    numeric height
    numeric function area()
        return this.width * this.height
    end function
end struct
```

---

## § 9 — Arrays

```mlp
-- Declaration
numeric[] nums = [1; 2; 3; 4; 5]

-- Access (0-based)
print(nums[0])     -- 1
nums[0] = 100

-- Length
print(nums.length)  -- 5

-- Push / Pop
nums.push(6)
numeric last = nums.pop()

-- Empty array
numeric[] empty = []
```

---

## § 10 — Hashmaps

```mlp
hashmap<string; numeric> scores = {}
scores["alice"] = 95
scores["bob"] = 87

-- Key check
if scores.has("alice") then
    print(scores["alice"])
end if

-- Delete
scores.delete("bob")

-- Iterate over keys
for each k in scores.keys()
    print(k + ": " + str(scores[k]))
end for

-- Length
print(scores.length)
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
string s = identity<string>("hello")
```

---

## § 14 — Lambdas (Stage 1)

```mlp
numeric function double = lambda(numeric x)
    return x * 2
end lambda

print(double(5))    -- 10
```

### Shorthand lambda (single parameter)

```mlp
numeric double = x -> x * 2
print(double(7))    -- 14

-- Inline in higher-order calls
numeric[] results = map(nums; x -> x * 10)
```

---

## § 15 — Error Handling (Stage 1)

### try / catch / finally

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

### Result type

```mlp
Result<numeric; string> function divide(numeric a; numeric b)
    Result<numeric; string> r
    if b == 0 then
        r.ok = false
        r.error = "Division by zero"
    else
        r.ok = true
        r.value = a / b
    end if
    return r
end function
```

---

## § 16 — Pattern Matching (Stage 1)

```mlp
string function classify(numeric x)
    return match x
        case 1 then "one"
        case 2 then "two"
        case 3 then "three"
        else "many"
    end match
end function
```

---

## § 17 — Operator Overloading (Stage 1)

```mlp
struct Vector
    numeric x
    numeric y
end struct

Vector function Vector.operator+(Vector other)
    Vector result
    result.x = this.x + other.x
    result.y = this.y + other.y
    return result
end function

Vector v3 = v1 + v2
```

---

## § 18 — Modules (Stage 2)

```mlp
-- math.mlp
module math
    numeric export function square(numeric x)
        return x * x
    end function
end module

-- main.mlp
import math

none function main()
    numeric result = math.square(10)
    print(result)    -- 100
end function
```

---

## § 19 — Async / Await (Stage 2)

```mlp
string async function fetchData(string url)
    string response = await httpGet(url)
    return response
end function

none function main()
    string data = await fetchData("https://api.example.com")
    print(data)
end function
```

---

## § 20 — Standard Library

### I/O

```mlp
print("Hello")                   -- stdout
println("Hello")                 -- stdout + newline
string line = read_line()
string contents = read_file("data.txt")
numeric ok = write_file("/tmp/out.txt"; "content\n")
numeric ok2 = append_file("/tmp/log.txt"; "line\n")
string arg1 = read_arg(1)
```

### Type Conversion

```mlp
string s = str(42)           -- "42"
string b = str(true)         -- "true"
numeric n = to_number("42")  -- 42
boolean b2 = to_boolean(1)   -- true
```

### Math

```mlp
abs(-7)         -- 7
min(10; 3)      -- 3
max(10; 3)      -- 10
sqrt(25)        -- 5
floor(3.7)      -- 3
ceil(3.2)       -- 4
```

### String Operations

```mlp
len("hello")                          -- 5
upper("hello")                        -- "HELLO"
lower("HELLO")                        -- "hello"
trim("  hi  ")                        -- "hi"
contains("hello world"; "world")      -- true
starts_with("hello"; "he")            -- true
ends_with("hello"; "lo")              -- true
split("a,b,c"; ",")                   -- ["a"; "b"; "c"]
join(["a"; "b"; "c"]; ", ")           -- "a, b, c"
replace("aaa"; "a"; "b")              -- "bbb"
substring("hello"; 1; 3)              -- "ell"
index_of("hello"; "ll")               -- 2
```

### Array Operations

```mlp
nums.push(42)
numeric last = nums.pop()
```

### Hashmap Operations

```mlp
map.has("key")      -- boolean
map.delete("key")   -- none
map.keys()          -- string[]
```

### Testing (Stage 1)

```mlp
test("addition"; lambda()
    assert_eq(1 + 1; 2)
    assert_true(5 > 3)
    assert_false(5 > 10)
end lambda)
```

---

## § 21 — Comments

```mlp
-- single-line comment

---/
Multi-line doc comment.
Any content here.
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
| Stdlib (io/math/string/array/hashmap) | ✅ | ✅ | ✅ |
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
| `scope` | `end scope` |
| `debug` | `end debug` |
| `test` | `end test` |
| `spawn` | `end spawn` |

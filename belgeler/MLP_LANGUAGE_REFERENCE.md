# MLP Language Reference

> **Version:** Stage 0 (518/518 tests passing)
> **Status:** Production-ready syntax
> **Toolchain:** `melp_compiler` → LLVM IR → native binary

MLP (Multi-Language Programming) is a compiled, statically-typed language that compiles to LLVM IR. The syntax is clean, readable, and block-structured — no curly braces, no semicolons at line ends.

---

## Table of Contents

1. [Quick Start](#1-quick-start)
2. [Comments](#2-comments)
3. [Types](#3-types)
4. [Variables](#4-variables)
5. [Operators](#5-operators)
6. [Control Flow](#6-control-flow)
7. [Loops](#7-loops)
8. [Functions](#8-functions)
9. [Structs](#9-structs)
10. [Enums](#10-enums)
11. [Arrays](#11-arrays)
12. [Hashmaps](#12-hashmaps)
13. [Pattern Matching](#13-pattern-matching)
14. [Lambdas](#14-lambdas)
15. [Imports](#15-imports)
16. [Error Handling](#16-error-handling)
17. [Built-in Functions](#17-built-in-functions)
18. [String Operations](#18-string-operations)
19. [Number Format](#19-number-format)
20. [Compiler Usage](#20-compiler-usage)

---

## 1. Quick Start

```mlp
-- Hello World
function main() as numeric
    print("Hello, World!")
    return 0
end_function
```

```mlp
-- Fibonacci
function fib(numeric n) as numeric
    if n <= 1 then
        return n
    end_if
    return fib(n - 1) + fib(n - 2)
end_function

function main() as numeric
    numeric result = fib(10)
    print(result)
    return 0
end_function
```

**Compile and run:**
```bash
melp_compiler program.mlp
lli output.ll
```

---

## 2. Comments

```mlp
-- This is a single-line comment

-- Multi-line comments: use multiple -- lines
-- Line 1
-- Line 2
-- Line 3
```

> **Rule:** Comments start with `--` (double dash). There are no block comments.

---

## 3. Types

| Type | Description | Example |
|------|-------------|---------|
| `numeric` | All numbers (integer and float) | `42`, `3,14` |
| `string` | Text, UTF-8 | `"hello"`, `'world'` |
| `boolean` | True/false | `true`, `false` |
| `none` | No return value (function return type only) | — |

### Type Annotations

Types are declared **before** the variable name. There is no type inference — explicit declaration is always required.

```mlp
numeric count = 100
string name = "Ali"
boolean active = true
```

---

## 4. Variables

### Declaration

```mlp
type name = value
```

```mlp
numeric x = 42
string greeting = "Hello"
boolean flag = true
```

### Reassignment

After declaration, reassign without the type keyword:

```mlp
numeric x = 10
x = 20          -- reassignment: no type prefix
x = x + 5      -- expression assignment
```

### Rules

- No `var`, `let`, `const`, or `auto` — type always required on first use
- No semicolons at end of lines
- Type comes **before** name: `numeric x` not `x: numeric`

```mlp
-- ✅ Correct
numeric result = 0
string message = "ok"

-- ❌ Wrong
result = 0              -- missing type on first use
numeric result = 0;     -- no semicolons
x: numeric = 0          -- wrong order
```

---

## 5. Operators

### Arithmetic

| Operator | Meaning | Example |
|----------|---------|---------|
| `+` | Addition | `10 + 5` → `15` |
| `-` | Subtraction | `10 - 5` → `5` |
| `*` | Multiplication | `4 * 3` → `12` |
| `/` | Division | `10 / 2` → `5` |
| `mod` | Modulo (remainder) | `17 mod 5` → `2` |
| `**` | Power / Exponent | `2 ** 8` → `256` |

> **Note:** Use `mod` not `%`.

### Comparison

| Operator | Meaning |
|----------|---------|
| `==` | Equal |
| `!=` | Not equal |
| `<` | Less than |
| `>` | Greater than |
| `<=` | Less than or equal |
| `>=` | Greater than or equal |

### Logical

| Operator | Meaning | Example |
|----------|---------|---------|
| `and` | Logical AND | `a > 0 and b > 0` |
| `or` | Logical OR | `a == 0 or b == 0` |
| `not` | Logical NOT | `not flag` |

> **Note:** Use `and`/`or`/`not`, **not** `&&`/`||`/`!`.

### Operator Precedence (High → Low)

1. `**` (power)
2. `*`, `/`, `mod`
3. `+`, `-`
4. `<`, `>`, `<=`, `>=`
5. `==`, `!=`
6. `not`
7. `and`
8. `or`

```mlp
numeric r = 2 + 3 * 4       -- 14 (multiplication first)
numeric s = 2 ** 3 + 1      -- 9  (power first)
boolean b = 5 > 3 and 2 < 4 -- true
```

---

## 6. Control Flow

### If / Else If / Else

```mlp
if condition then
    -- body
end_if
```

```mlp
if condition then
    -- true branch
else
    -- false branch
end_if
```

```mlp
if condition1 then
    -- branch 1
else_if condition2 then
    -- branch 2
else_if condition3 then
    -- branch 3
else
    -- default
end_if
```

**Examples:**

```mlp
numeric score = 85

if score >= 90 then
    print("A")
else_if score >= 80 then
    print("B")
else_if score >= 70 then
    print("C")
else
    print("F")
end_if
```

**Rules:**

- `if` **must** be followed by `then`
- `else_if` is a single token (not `elif` or `else if`)
- Block ends with `end_if`
- Parentheses around condition are optional

```mlp
-- ✅ Correct
if x > 5 then
    print("big")
end_if

-- ❌ Wrong: missing "then"
if x > 5
    print("big")
end_if

-- ❌ Wrong: "elif" not valid
elif x > 2 then
```

---

## 7. Loops

### While Loop

```mlp
while condition
    -- body
end_while
```

```mlp
numeric i = 0
while i < 10
    print(i)
    i = i + 1
end_while
```

> **Note:** No `do` keyword after `while`. No parentheses required.

### For Loop (Range)

```mlp
-- Ascending
for variable = start to end
    -- body
end_for

-- Descending
for variable = start downto end
    -- body
end_for

-- With step
for variable = start to end step increment
    -- body
end_for
```

```mlp
-- Count 0 to 9
for i = 0 to 9
    print(i)
end_for

-- Count down from 10
for i = 10 downto 1
    print(i)
end_for

-- Even numbers
for i = 0 to 20 step 2
    print(i)
end_for
```

> **Note:** `for` range loops are **inclusive** on both ends.

### For Each Loop (Collections)

```mlp
for each item in collection
    -- body
end_for
```

```mlp
numeric[] nums = [1; 2; 3; 4; 5]
for each n in nums
    print(n)
end_for

string[] names = ["Ali"; "Veli"; "Ayşe"]
for each name in names
    print("Hello " + name)
end_for
```

---

## 8. Functions

### Declaration

```mlp
function name(type param1; type param2) as return_type
    -- body
    return value
end_function
```

```mlp
-- Simple function
function add(numeric a; numeric b) as numeric
    return a + b
end_function

-- No return value
function greet(string name) as none
    print("Hello, " + name)
end_function

-- No parameters
function get_pi() as numeric
    return 3
end_function
```

### Calling Functions

```mlp
numeric result = add(10; 20)
greet("Ali")
numeric pi = get_pi()
```

### Rules

- Parameters separated by **`;`** (semicolon), not `,` (comma)
- Return type declared with `as type`
- `as none` for functions with no return value
- Block ends with `end_function`
- `return` required for non-`none` functions

```mlp
-- ✅ Correct
function multiply(numeric x; numeric y) as numeric
    return x * y
end_function

-- ❌ Wrong: comma separator
function multiply(numeric x, numeric y) as numeric

-- ❌ Wrong: C-style return type
function multiply(numeric x; numeric y) -> numeric

-- ❌ Wrong: missing "as"
function multiply(numeric x; numeric y) numeric
```

### Recursive Functions

```mlp
function factorial(numeric n) as numeric
    if n <= 1 then
        return 1
    end_if
    return n * factorial(n - 1)
end_function
```

### Struct Methods

Methods are defined **outside** the struct using dot notation:

```mlp
struct Rectangle
    numeric width
    numeric height
end_struct

function Rectangle.area() as numeric
    return this.width * this.height
end_function

function Rectangle.perimeter() as numeric
    return 2 * (this.width + this.height)
end_function
```

---

## 9. Structs

### Declaration

```mlp
struct Name
    type field1
    type field2
end_struct
```

```mlp
struct Point
    numeric x
    numeric y
end_struct

struct Person
    string name
    numeric age
end_struct
```

### Creating Instances

```mlp
Point p
p.x = 10
p.y = 20
print(p.x)   -- 10
```

### Nested Structs

```mlp
struct Address
    string street
    numeric zipcode
end_struct

struct Person
    string name
    Address addr
end_struct

Person person
person.name = "Ali"
person.addr.street = "Atatürk Cad."
print(person.addr.street)
```

### Methods

```mlp
struct Circle
    numeric radius
end_struct

function Circle.area() as numeric
    return 3 * this.radius * this.radius
end_function

-- Usage
Circle c
c.radius = 5
numeric area = c.area()
print(area)   -- 75
```

> **Design principle:** MLP uses `STRUCT + FUNC` — methods live outside structs. No OOP-style method definitions inside struct bodies.

---

## 10. Enums

### Declaration

```mlp
enum Name
    VALUE1
    VALUE2
    VALUE3
end_enum
```

Values auto-number from 0: `VALUE1=0`, `VALUE2=1`, etc.

```mlp
enum Color
    RED
    GREEN
    BLUE
end_enum

-- RED=0, GREEN=1, BLUE=2
numeric c = Color.GREEN   -- 1
```

### Using Enums

```mlp
enum Status
    PENDING
    ACTIVE
    DONE
end_enum

numeric state = Status.ACTIVE

if state == Status.ACTIVE then
    print("Running")
end_if
```

### Enum with Match

```mlp
match state
    case Status.PENDING then print("Waiting")
    case Status.ACTIVE  then print("Running")
    case Status.DONE    then print("Complete")
    else print("Unknown")
end_match
```

---

## 11. Arrays

### Declaration

```mlp
-- Empty array
type[] name = []

-- Array with values (semicolon-separated)
type[] name = [val1; val2; val3]
```

```mlp
numeric[] scores = [95; 87; 72; 61]
string[] names = ["Ali"; "Veli"; "Ayşe"]
boolean[] flags = [true; false; true]
```

### Access and Mutation

```mlp
numeric[] nums = [10; 20; 30; 40; 50]

print(nums[0])      -- 10  (0-indexed)
nums[2] = 99
print(nums[2])      -- 99

numeric len = nums.length   -- 5
```

### Dynamic Operations

```mlp
numeric[] bag = []
bag.push(7)
bag.push(8)
bag.push(9)
print(bag.length)   -- 3

numeric last = bag.pop()
print(last)         -- 9
```

### Iterating

```mlp
numeric[] data = [1; 2; 3; 4; 5]

-- With index (for loop)
for i = 0 to 4
    print(data[i])
end_for

-- Without index (for each)
for each n in data
    print(n)
end_for
```

> **Note:** Array indices are **0-based**. Out-of-bounds access is a runtime error.

---

## 12. Hashmaps

### Declaration

```mlp
hashmap<KeyType; ValueType> name = {}
```

```mlp
hashmap<string; numeric> scores = {}
hashmap<string; string> config = {}
hashmap<numeric; string> lookup = {}
```

### Operations

```mlp
hashmap<string; numeric> ages = {}

-- Set
ages["Ali"] = 30
ages["Veli"] = 25

-- Get
numeric ali_age = ages["Ali"]   -- 30

-- Check existence
if ages.has("Veli") then
    print("Found")
end_if

-- Delete
ages.delete("Veli")

-- Size
numeric count = ages.length     -- 1

-- Iterate keys
for each key in ages.keys()
    print(key + ": " + str(ages[key]))
end_for
```

> **Note:** Accessing a missing key returns the zero value: `0` for numeric, `""` for string.

---

## 13. Pattern Matching

### Basic Match

```mlp
match expression
    case value1 then
        -- branch 1
    case value2 then
        -- branch 2
    else
        -- default
end_match
```

```mlp
numeric x = 5

match x
    case 1 then print("one")
    case 2 then print("two")
    case 5 then print("five")
    else print("other")
end_match
```

### Match as Expression

```mlp
string label = match x
    case 1 then "one"
    case 2 then "two"
    else "other"
end_match
```

### Guard Clauses

```mlp
match n
    case x if x < 0  then print("negative")
    case x if x == 0 then print("zero")
    case x if x < 10 then print("small")
    else print("large")
end_match
```

### Enum Match

```mlp
enum Day
    MON
    TUE
    WED
    THU
    FRI
    SAT
    SUN
end_enum

numeric today = Day.WED

match today
    case Day.MON then print("Monday")
    case Day.TUE then print("Tuesday")
    case Day.WED then print("Wednesday")
    else print("Other day")
end_match
```

---

## 14. Lambdas

Lambdas are anonymous inline functions stored in variables.

### Syntax

```mlp
type name = lambda(param1; param2) -> expression
```

```mlp
-- Basic lambda
numeric double = lambda(x) -> x * 2
print(double(7))   -- 14

-- Multi-parameter
numeric add = lambda(x; y) -> x + y
print(add(3; 4))   -- 7
```

### Closures

Lambdas capture outer scope variables by value:

```mlp
numeric multiplier = 3
numeric mult = lambda(x) -> x * multiplier
print(mult(5))    -- 15
print(mult(10))   -- 30
```

### Higher-Order Functions

```mlp
-- Lambda passed to test framework
test("addition works"; lambda()
    assert_eq(1 + 1; 2)
end_lambda)
```

> **Current limitation:** Inline expression lambdas (`lambda(x) -> expr`) fully supported. Block lambdas (`end_lambda`) are supported in test framework context.

---

## 15. Imports

### Single File Import

```mlp
import module_name
```

> Module name = filename without `.mlp` extension.

**File: math_utils.mlp**
```mlp
function add(numeric a; numeric b) as numeric
    return a + b
end_function

function square(numeric x) as numeric
    return x * x
end_function
```

**File: main.mlp**
```mlp
import math_utils

function main() as numeric
    numeric result = math_utils.add(5; 3)
    print(result)   -- 8

    numeric sq = math_utils.square(4)
    print(sq)       -- 16

    return 0
end_function
```

### Multiple Imports

```mlp
import string_utils
import math_utils
import data_helpers

function main() as numeric
    string text = string_utils.concat("Hello"; " World")
    numeric sum = math_utils.add(10; 20)
    return 0
end_function
```

### Importing Structs

**File: geometry.mlp**
```mlp
struct Point
    numeric x
    numeric y
end_struct

function Point.distance() as numeric
    return this.x * this.x + this.y * this.y
end_function
```

**File: main.mlp**
```mlp
import geometry

function main() as numeric
    geometry.Point p
    p.x = 3
    p.y = 4
    print(p.distance())   -- 25
    return 0
end_function
```

### Project Structure

```
project/
├── main.mlp          -- entry point (contains main function)
├── utils.mlp         -- utility functions
├── models.mlp        -- struct definitions
└── helpers.mlp       -- helper functions
```

**Compile multi-file:**
```bash
melp_compiler main.mlp utils.mlp models.mlp helpers.mlp -o output
```

---

## 16. Error Handling

### Try / Catch

```mlp
try
    -- code that may fail
catch error
    print("Error: " + error.message)
end_try
```

```mlp
try
    numeric result = risky_operation()
    print(result)
catch err
    print("Failed: " + err.message)
end_try
```

### With Finally

```mlp
try
    open_file("data.txt")
    process_data()
catch err
    print("File error: " + err.message)
finally
    close_file()
end_try
```

### Result Pattern

For functions that can fail without exceptions:

```mlp
struct Result
    boolean ok
    numeric value
    string error
end_struct

function divide(numeric a; numeric b) as Result
    Result r
    if b == 0 then
        r.ok = false
        r.error = "Division by zero"
    else
        r.ok = true
        r.value = a / b
    end_if
    return r
end_function

function main() as numeric
    Result r = divide(10; 0)
    if r.ok then
        print(r.value)
    else
        print("Error: " + r.error)
    end_if
    return 0
end_function
```

---

## 17. Built-in Functions

### Output

| Function | Description | Example |
|----------|-------------|---------|
| `print(x)` | Print value + newline | `print(42)` |
| `print(s)` | Print string + newline | `print("hello")` |

```mlp
print(42)               -- outputs: 42
print("Hello")          -- outputs: Hello
print("Value: " + str(x))  -- outputs: Value: 5
```

### Type Conversion

| Function | Input → Output | Example |
|----------|---------------|---------|
| `str(x)` | numeric/boolean → string | `str(42)` → `"42"` |
| `to_number(s)` | string → numeric | `to_number("3")` → `3` |
| `to_boolean(n)` | numeric → boolean | `to_boolean(1)` → `true` |

```mlp
numeric n = 42
string s = str(n)              -- "42"
print("Count: " + str(n))     -- "Count: 42"

string s = str(true)           -- "true"
string s = str(false)          -- "false"
```

### Math

| Function | Description |
|----------|-------------|
| `abs(n)` | Absolute value |
| `min(a; b)` | Minimum of two values |
| `max(a; b)` | Maximum of two values |
| `sqrt(n)` | Square root |
| `floor(n)` | Floor division |
| `ceil(n)` | Ceiling division |

### String

| Function | Description | Example |
|----------|-------------|---------|
| `len(s)` | String length | `len("hello")` → `5` |
| `upper(s)` | Uppercase | `upper("hi")` → `"HI"` |
| `lower(s)` | Lowercase | `lower("HI")` → `"hi"` |
| `trim(s)` | Remove whitespace | `trim(" hi ")` → `"hi"` |
| `contains(s; sub)` | Contains substring | `contains("hello"; "ell")` |
| `starts_with(s; pre)` | Prefix check | `starts_with("hello"; "he")` |
| `ends_with(s; suf)` | Suffix check | `ends_with("hello"; "lo")` |
| `split(s; delim)` | Split string | `split("a,b,c"; ",")` |
| `join(arr; delim)` | Join array | `join(parts; ", ")` |
| `replace(s; old; new)` | Replace substring | `replace("hello"; "l"; "r")` |
| `substring(s; start; len)` | Substring | `substring("hello"; 1; 3)` → `"ell"` |
| `index_of(s; sub)` | Find position | `index_of("hello"; "ll")` → `2` |

### I/O

| Function | Description |
|----------|-------------|
| `read_line()` | Read a line from stdin |
| `read_file(path)` | Read entire file as string |
| `write_file(path; content)` | Write string to file |

---

## 18. String Operations

### Concatenation

```mlp
string a = "Hello"
string b = " World"
string c = a + b            -- "Hello World"
string d = "Hi " + "there" -- "Hi there"
```

### String + Number

Use `str()` to convert numbers before concatenation:

```mlp
numeric n = 42
string s = "Value: " + str(n)    -- "Value: 42"
string t = str(n) + " items"     -- "42 items"
```

### Multiline Building

```mlp
string result = ""
result = result + "Line 1\n"
result = result + "Line 2\n"
result = result + "Line 3\n"
print(result)
```

### String Comparison

```mlp
string s = "hello"

if s == "hello" then
    print("match")
end_if

if s != "world" then
    print("different")
end_if
```

---

## 19. Number Format

MLP uses the **Turkish number format** by default:

| Format | Meaning | Value |
|--------|---------|-------|
| `3,14` | Decimal (comma = decimal point) | 3.14 |
| `1.000` | Thousands (dot = thousands separator) | 1000 |
| `1.000.000` | Millions | 1,000,000 |
| `126.555,36` | Combined | 126,555.36 |

```mlp
numeric pi = 3,14159
numeric rate = 0,05
numeric population = 85.000.000
numeric price = 1.250,99
```

> **Note:** In MLP, comma (`,`) is the decimal separator and dot (`.`) is the thousands separator — the opposite of English convention.

---

## 20. Compiler Usage

### Basic Compilation

```bash
# Compile to LLVM IR
melp_compiler program.mlp

# Output file: output.ll (LLVM IR)
# Run with LLVM interpreter
lli output.ll

# Or compile to native binary
llc output.ll -o output.s
clang output.s -o program
./program
```

### Multi-file Compilation

```bash
melp_compiler main.mlp utils.mlp models.mlp -o output
```

### Compiler Flags

| Flag | Description |
|------|-------------|
| `--ok-all` | Enable all OK layer optimizations (default) |
| `--ok-none` | Disable OK layer (for debugging) |
| `--emit-ir` | Output LLVM IR to stdout |
| `-o <file>` | Specify output file |

### Performance

The OK (Optimization Kit) layer provides automatic function annotation:

```mlp
-- Pure functions automatically get LLVM optimization hints:
-- ReadOnly, WillReturn, NoUnwind, AlwaysInline (if small)
-- NoAlias, NoCapture on pointer parameters
function add(numeric a; numeric b) as numeric
    return a + b
end_function
```

Benchmarks (OK layer enabled vs disabled):
- `fib(40)`: +15.7% speedup
- Chain functions: +17.8% speedup

---

## Appendix: Syntax Quick Reference

### Block Terminators

| Block | Terminator |
|-------|------------|
| `function ... end_function` | `end_function` |
| `if ... end_if` | `end_if` |
| `while ... end_while` | `end_while` |
| `for ... end_for` | `end_for` |
| `struct ... end_struct` | `end_struct` |
| `enum ... end_enum` | `end_enum` |
| `match ... end_match` | `end_match` |
| `try ... end_try` | `end_try` |
| `lambda() ... end_lambda` | `end_lambda` |

### Common Mistakes

| Wrong | Correct | Reason |
|-------|---------|--------|
| `function f(int x, int y)` | `function f(numeric x; numeric y)` | `;` not `,` |
| `if x > 0` | `if x > 0 then` | `then` required |
| `else if` | `else_if` | single token |
| `end if` | `end_if` | underscore, single token |
| `17 % 5` | `17 mod 5` | use `mod` |
| `&&`, `\|\|`, `!` | `and`, `or`, `not` | word operators |
| `numeric x = 42;` | `numeric x = 42` | no semicolons |
| `to_string(x)` | `str(x)` | current builtin name |
| `function f() -> numeric` | `function f() as numeric` | `as` syntax |

### Entry Point

Every MLP program needs a `main` function:

```mlp
function main() as numeric
    -- program starts here
    return 0
end_function
```

Return value convention: `0` = success, non-zero = error code.

---

*MLP Language Reference — Stage 0 — 2 March 2026*

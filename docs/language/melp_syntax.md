# MELP Syntax Reference

**Version:** 1.0  
**Last Updated:** December 10, 2025  
**Language:** English

---

## Table of Contents

1. [Comments](#comments)
2. [Variables](#variables)
3. [Data Types](#data-types)
4. [Operators](#operators)
5. [Control Flow](#control-flow)
6. [Functions](#functions)
7. [Collections](#collections)
8. [Structs](#structs)
9. [Exception Handling](#exception-handling)
10. [Advanced Features](#advanced-features)

---

## Comments

MELP uses `--` for comments (NOT `//` or `#`):

```mlp
-- Single line comment

---
Multi-line comment
Can span multiple lines
---

numeric x = 10  -- End of line comment
```

⚠️ **Warning:** Using `//` will cause a syntax error!

---

## Variables

### Explicit Type Declaration

```mlp
-- Type-first declaration
numeric x = 10
string name = "Alice"
boolean flag = true

-- Type declaration with default value
numeric a            -- a = 0
string b             -- b = ""
boolean c            -- c = false

-- Multiple declarations
numeric a, b, c = 1, 2, 3
```

### Type Inference (YZ_25)

MELP automatically infers types for assignments ending with `;`:

```mlp
-- New variable declaration with ; (type inferred)
count = 42;          -- ✅ numeric count = 42
name = "Alice";      -- ✅ string name = "Alice"
active = true;       -- ✅ boolean active = true
price = 10.50;       -- ✅ numeric price = 10.50

-- Type inference from expression
numeric x = 10
y = x + 5;           -- ✅ numeric y = 15 (type inferred from x)
```

### Assignment to Existing Variable

```mlp
numeric count = 10
count = 20           -- ✅ OK: count already declared, no ; needed
```

### Typo Protection

```mlp
numeric count = 10
counnt = 20          -- ❌ ERROR: "Undefined variable 'counnt'. Did you mean 'count'?"
                     --          "Add ';' at end to declare new variable"
```

### Variable Types

| Type | Description | Example |
|------|-------------|---------|
| `numeric` | All numbers (int/float unified) | `numeric x = 42` |
| `string` | Text strings (UTF-8) | `string s = "hello"` |
| `boolean` | True/false values | `boolean b = true` |

---

## Data Types

### Numeric

MELP unifies all number types under `numeric`:

```mlp
numeric integer = 42
numeric decimal = 3.14159
numeric negative = -100
numeric scientific = 1.5e10
```

### String

```mlp
string greeting = "Hello, World!"
string empty = ""
string multiword = "Multiple words here"
```

### Boolean

```mlp
boolean yes = true
boolean no = false
```

---

## Operators

### Arithmetic Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `+` | Addition | `a + b` |
| `-` | Subtraction | `a - b` |
| `*` | Multiplication | `a * b` |
| `/` | Division | `a / b` |
| `^` | Exponentiation | `a ^ b` |

### Comparison Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `==` | Equal to | `a == b` |
| `!=` | Not equal to | `a != b` |
| `<` | Less than | `a < b` |
| `>` | Greater than | `a > b` |
| `<=` | Less than or equal | `a <= b` |
| `>=` | Greater than or equal | `a >= b` |

### Logical Operators

| Operator | Description | Example |
|----------|-------------|---------|
| `and` | Logical AND | `a and b` |
| `or` | Logical OR | `a or b` |
| `not` | Logical NOT | `not a` |

### Assignment

```mlp
x = 10           -- Simple assignment
x = x + 1        -- Increment
```

---

## Control Flow

### If-Then-Else

```mlp
if condition then
    -- code
end if

if condition then
    -- code
else
    -- code
end if

if condition1 then
    -- code
else if condition2 then
    -- code
else
    -- code
end if
```

### While Loop

```mlp
while condition do
    -- code
end while
```

### For Loop

```mlp
-- Basic for loop
for i = 0 to 10
    print(i)
end for

-- With step
for i = 0 to 10 step 2
    print(i)
end for

-- Downward
for i = 10 downto 0
    print(i)
end for
```

### For-In Loop (Iterator)

```mlp
numeric[] arr = [1, 2, 3, 4, 5]
for x in arr
    print(x)
end for

-- With range
for i in range(5)
    print(i)  -- 0, 1, 2, 3, 4
end for

for i in range(2, 8)
    print(i)  -- 2, 3, 4, 5, 6, 7
end for
```

### Do-While Loop

```mlp
do
    -- code (executes at least once)
while condition
```

### Switch-Case

```mlp
switch value
    case 1
        print("one")
    case 2
        print("two")
    default
        print("other")
end switch
```

**Note:** No fall-through; each case automatically breaks.

### Exit Statements (VB.NET Style)

```mlp
exit if         -- Exit from if block
exit for        -- Exit from for loop
exit while      -- Exit from while loop
exit function   -- Exit from function
exit switch     -- Exit from switch block
```

**Note:** MELP uses `exit X` instead of `break`/`continue`.

---

## Functions

### Basic Function

```mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end function
```

### Void Function

```mlp
function greet(string name)
    print("Hello, " + name)
end function
```

### Calling Functions

```mlp
numeric result = add(5, 3)
greet("Alice")
```

### Lambda Functions

```mlp
numeric doubler = lambda(numeric x) => x * 2
print(doubler(5))  -- 10
```

---

## Collections

### Array (Homogeneous, Mutable)

```mlp
-- Declaration
numeric[] numbers = [1, 2, 3, 4, 5]

-- Access (square brackets)
numeric first = numbers[0]
numbers[0] = 100
```

### List (Heterogeneous, Mutable)

```mlp
-- Declaration (parentheses with semicolons)
person() = ("Alice"; 25; true;)

-- Access (parentheses, NO SPACE before)
string name = person(0)    -- ✅ Correct
string name = person (0)   -- ❌ Error! No space allowed!

-- Methods
person.add("new item")
numeric len = person.length()
```

### Tuple (Heterogeneous, Immutable)

```mlp
-- Declaration (angle brackets)
point<> = <10, 20, "origin">

-- Access (angle brackets)
numeric x = point<0>
point<0> = 99  -- ❌ Error! Tuples are immutable
```

### Collection Syntax Summary

| Type | Literal | Access | Mutable |
|------|---------|--------|---------|
| Array | `[1, 2, 3]` | `arr[0]` | ✅ Yes |
| List | `(1; 2; 3;)` | `list(0)` | ✅ Yes |
| Tuple | `<1, 2, 3>` | `tuple<0>` | ❌ No |

---

## Structs

### Definition

```mlp
struct Person
    string name
    numeric age
end struct
```

### Usage

```mlp
Person p
p.name = "Alice"
p.age = 25

print(p.name)
```

---

## Exception Handling

### Try-Catch-Finally

```mlp
try
    -- risky code
    throw RuntimeError("Something went wrong!")
catch RuntimeError e
    print("Runtime error caught")
catch ValueError e
    print("Value error caught")
catch e
    print("Other errors")
finally
    print("Always executes")
end try
```

---

## Advanced Features

### Pattern Matching

```mlp
match value
    case 1 => print("one")
    case 2, 3 => print("two or three")
    case 4 to 10 => print("four to ten")
    case _ => print("default")
end match
```

### Null Safety

```mlp
-- Null coalescing
numeric result = maybeNull ?? defaultValue

-- Optional chaining
string name = person?.name

-- Nullable type
numeric? maybeNum = null
```

### State Management

```mlp
state numeric counter = 0
shared state numeric globalCounter = 100

counter = counter + 1
```

### Operator Overloading

```mlp
operator +(Vector a, Vector b) returns Vector
    Vector result
    result.x = a.x + b.x
    result.y = a.y + b.y
    return result
end operator
```

### Generators

```mlp
function squares(numeric n) yields numeric
    for i = 0 to n
        yield i * i
    end for
end function

for sq in squares(5)
    print(sq)  -- 0, 1, 4, 9, 16, 25
end for
```

---

## Reserved Keywords

```
and, boolean, break, case, catch, continue, default, defer,
do, else, end, exit, false, finally, for, function, generator,
if, import, in, lambda, list, match, module, not, null, numeric,
operator, or, print, return, shared, state, step, string, struct,
switch, then, throw, to, true, try, tuple, while, yield
```

---

## Quick Reference

### Block Endings

| Block | Ending |
|-------|--------|
| `if` | `end if` |
| `while` | `end while` |
| `for` | `end for` |
| `function` | `end function` |
| `struct` | `end struct` |
| `switch` | `end switch` |
| `try` | `end try` |
| `match` | `end match` |

### File Extension

All MELP files use the `.mlp` extension.

---

*This document is part of the MELP Language Documentation.*

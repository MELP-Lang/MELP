# MELP Functions Reference

**Version:** 1.0  
**Last Updated:** December 10, 2025  
**Language:** English

---

## Overview

MELP supports multiple function types: regular functions, lambdas, closures, generators, and nested functions.

---

## Basic Functions

### Function Declaration

```mlp
function functionName(parameters) returns returnType
    -- body
    return value
end function
```

### Examples

```mlp
-- Function with return value
function add(numeric a, numeric b) returns numeric
    return a + b
end function

-- Void function (no return type)
function greet(string name)
    print("Hello, " + name)
end function

-- Multiple parameters
function calculate(numeric x, numeric y, numeric z) returns numeric
    return x * y + z
end function
```

### Function Calls

```mlp
numeric result = add(5, 3)
greet("Alice")
numeric total = calculate(2, 3, 4)  -- 2*3+4 = 10
```

---

## Parameters

### Parameter Types

```mlp
function process(numeric n, string s, boolean flag) returns numeric
    if flag then
        return n
    end if
    return 0
end function
```

### Array Parameters

```mlp
function sum(numeric[] numbers) returns numeric
    numeric total = 0
    for num in numbers
        total = total + num
    end for
    return total
end function

numeric result = sum([1, 2, 3, 4, 5])  -- 15
```

### Struct Parameters

```mlp
function displayPerson(Person p)
    print(p.name + " is " + to_string(p.age))
end function
```

---

## Return Values

### Single Return

```mlp
function square(numeric x) returns numeric
    return x * x
end function
```

### Tuple Return (Multiple Values)

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

<minimum, maximum> = minmax([3, 1, 4, 1, 5])
```

### Early Return

```mlp
function findFirst(numeric[] arr, numeric target) returns numeric
    for i = 0 to arr.length() - 1
        if arr[i] == target then
            return i  -- Early return
        end if
    end for
    return -1  -- Not found
end function
```

---

## Lambda Functions

Anonymous functions for concise code:

### Basic Syntax

```mlp
-- Lambda assigned to variable
numeric doubler = lambda(numeric x) => x * 2

-- Usage
print(doubler(5))  -- 10
```

### As Arguments

```mlp
function apply(numeric[] arr, lambda fn) returns numeric[]
    numeric[] result = []
    for num in arr
        result.add(fn(num))
    end for
    return result
end function

numeric[] doubled = apply([1, 2, 3], lambda(x) => x * 2)
-- [2, 4, 6]
```

### Multi-line Lambda

```mlp
numeric processor = lambda(numeric x) =>
    if x < 0 then
        return 0
    else
        return x * 2
    end if
end lambda
```

---

## Closures

Functions that capture their environment:

```mlp
function makeCounter() returns lambda
    numeric count = 0
    return lambda() =>
        count = count + 1
        return count
    end lambda
end function

lambda counter = makeCounter()
print(counter())  -- 1
print(counter())  -- 2
print(counter())  -- 3
```

### Practical Example

```mlp
function makeMultiplier(numeric factor) returns lambda
    return lambda(numeric x) => x * factor
end function

lambda triple = makeMultiplier(3)
lambda quintuple = makeMultiplier(5)

print(triple(10))     -- 30
print(quintuple(10))  -- 50
```

---

## Nested Functions

Functions defined inside other functions:

```mlp
function outer(numeric x) returns numeric
    function inner(numeric y) returns numeric
        return y * 2
    end function
    
    return inner(x) + 1
end function

print(outer(5))  -- 11 (5*2 + 1)
```

### Accessing Outer Scope

```mlp
function process(numeric[] data) returns numeric
    numeric total = 0
    
    function addToTotal(numeric value)
        total = total + value  -- Access outer variable
    end function
    
    for num in data
        addToTotal(num)
    end for
    
    return total
end function
```

---

## Generators

Functions that yield values one at a time:

### Basic Generator

```mlp
function countdown(numeric n) yields numeric
    while n > 0 do
        yield n
        n = n - 1
    end while
end function

for num in countdown(5)
    print(num)  -- 5, 4, 3, 2, 1
end for
```

### Infinite Generator

```mlp
function naturals() yields numeric
    numeric n = 0
    while true do
        yield n
        n = n + 1
    end while
end function

-- Use with early exit
for num in naturals()
    if num > 100 then
        exit for
    end if
    print(num)
end for
```

### Generator with Parameters

```mlp
function range(numeric start, numeric stop, numeric step) yields numeric
    numeric i = start
    while i < stop do
        yield i
        i = i + step
    end while
end function

for i in range(0, 10, 2)
    print(i)  -- 0, 2, 4, 6, 8
end for
```

---

## Defer Statement

Execute code when function exits (cleanup):

```mlp
function processFile(string filename) returns numeric
    file = open(filename)
    defer close(file)  -- Will execute when function returns
    
    -- Process file...
    if error then
        return -1  -- close(file) still executes!
    end if
    
    return 0  -- close(file) executes here too
end function
```

### Multiple Defer

Defer statements execute in LIFO order:

```mlp
function example()
    defer print("First defer")   -- Executes third
    defer print("Second defer")  -- Executes second
    defer print("Third defer")   -- Executes first
    print("Function body")
end function

-- Output:
-- Function body
-- Third defer
-- Second defer
-- First defer
```

---

## Exit Function

Early exit from function:

```mlp
function search(numeric[] arr, numeric target)
    for i = 0 to arr.length() - 1
        if arr[i] == target then
            print("Found at " + to_string(i))
            exit function  -- Exit immediately
        end if
    end for
    print("Not found")
end function
```

---

## Recursion

Functions calling themselves:

```mlp
function factorial(numeric n) returns numeric
    if n <= 1 then
        return 1
    end if
    return n * factorial(n - 1)
end function

print(factorial(5))  -- 120
```

### Tail Recursion

```mlp
function factorial_tail(numeric n, numeric acc) returns numeric
    if n <= 1 then
        return acc
    end if
    return factorial_tail(n - 1, n * acc)
end function

function factorial(numeric n) returns numeric
    return factorial_tail(n, 1)
end function
```

---

## Operator Overloading

Define operators for custom types:

```mlp
struct Vector
    numeric x
    numeric y
end struct

operator +(Vector a, Vector b) returns Vector
    Vector result
    result.x = a.x + b.x
    result.y = a.y + b.y
    return result
end operator

operator -(Vector a, Vector b) returns Vector
    Vector result
    result.x = a.x - b.x
    result.y = a.y - b.y
    return result
end operator

-- Usage
Vector v1
v1.x = 10
v1.y = 20

Vector v2
v2.x = 5
v2.y = 15

Vector sum = v1 + v2  -- Uses overloaded +
```

### Supported Operators

- Arithmetic: `+`, `-`, `*`, `/`
- Comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`

---

## Quick Reference

### Function Syntax

```mlp
-- Basic function
function name(type param) returns type
    return value
end function

-- Void function
function name(type param)
    -- no return needed
end function

-- Lambda
lambda(param) => expression

-- Generator
function name(param) yields type
    yield value
end function
```

### Keywords

| Keyword | Purpose |
|---------|---------|
| `function` | Function declaration |
| `returns` | Return type |
| `return` | Return value |
| `lambda` | Anonymous function |
| `yields` | Generator function |
| `yield` | Produce value |
| `defer` | Deferred execution |
| `exit function` | Early exit |
| `operator` | Operator overload |

---

*This document is part of the MELP Language Documentation.*

# Tutorial 02: Variables and Types

In this tutorial, you'll learn how to declare and use variables in MELP, understand the type system, and perform basic operations.

## Table of Contents
- [What You'll Learn](#what-youll-learn)
- [Variable Declaration](#variable-declaration)
- [MELP Type System](#melp-type-system)
- [Numeric Type](#numeric-type)
- [String Type](#string-type)
- [Boolean Type](#boolean-type)
- [Variable Scope](#variable-scope)
- [Type Safety](#type-safety)
- [Best Practices](#best-practices)
- [Exercises](#exercises)
- [Next Steps](#next-steps)

---

## What You'll Learn

By the end of this tutorial, you will:
- Declare variables with different types
- Understand MELP's type system (numeric, string, boolean)
- Perform arithmetic and string operations
- Understand variable scope and lifetime
- Write type-safe code

---

## Variable Declaration

In MELP, variables are declared with an explicit type followed by a name and an initial value.

### Basic Syntax

```mlp
type variableName = value
```

### Example: Complete Program

```mlp
function main() returns numeric
    numeric age = 25
    string name = "Alice"
    boolean isActive = true
    
    print("Variable declarations complete!")
    return 0
end
```

**Compile and run:**
```bash
./compiler/stage0/melp variables.mlp variables.ll
clang variables.ll -o variables
./variables
```

**Output:**
```
Variable declarations complete!
```

---

## MELP Type System

MELP has three fundamental types:

| Type | Description | Example Values | Size |
|------|-------------|----------------|------|
| **numeric** | Integer numbers | `42`, `-10`, `0` | 64-bit signed integer |
| **string** | Text data | `"Hello"`, `"MELP"` | Pointer to UTF-8 string |
| **boolean** | Truth values | `true`, `false` | 1-bit (stored as i1 in LLVM) |

> **Note:** MELP is **statically typed** - variable types are checked at compile time!

---

## Numeric Type

The `numeric` type represents 64-bit signed integers.

### Declaration and Assignment

```mlp
function main() returns numeric
    numeric x = 10
    numeric y = 20
    numeric sum = x + y
    
    print("Calculation complete!")
    return sum
end
```

### Arithmetic Operations

MELP supports standard arithmetic operators:

```mlp
function main() returns numeric
    numeric a = 100
    numeric b = 25
    
    numeric addition = a + b       // 125
    numeric subtraction = a - b    // 75
    numeric multiplication = a * b // 2500
    numeric division = a / b       // 4
    numeric modulo = a % b         // 0
    
    return 0
end
```

**Supported Operators:**
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Integer division
- `%` Modulo (remainder)

### Negative Numbers

```mlp
function main() returns numeric
    numeric temperature = -15
    numeric debt = -1000
    numeric result = temperature + 20  // 5
    
    return result
end
```

### Range

- **Minimum:** `-9,223,372,036,854,775,808` (−2^63)
- **Maximum:** `9,223,372,036,854,775,807` (2^63 − 1)

---

## String Type

The `string` type represents UTF-8 encoded text.

### Declaration and Assignment

```mlp
function main() returns numeric
    string greeting = "Hello, World!"
    string language = "MELP"
    string emoji = "🚀"
    
    print(greeting)
    print(language)
    print(emoji)
    
    return 0
end
```

**Output:**
```
Hello, World!
MELP
🚀
```

### String Concatenation

You can combine strings using the `+` operator:

```mlp
function main() returns numeric
    string firstName = "John"
    string lastName = "Doe"
    string fullName = firstName + " " + lastName
    
    print(fullName)  // "John Doe"
    
    return 0
end
```

### String Comparison

Compare strings using comparison operators:

```mlp
function main() returns numeric
    string password = "admin"
    
    if password == "admin" then
        print("Access granted!")
    end if
    
    if password != "guest" then
        print("Not a guest user")
    end if
    
    return 0
end
```

**Supported Comparison Operators:**
- `==` Equal to
- `!=` Not equal to
- `<` Less than (lexicographic)
- `>` Greater than (lexicographic)
- `<=` Less than or equal
- `>=` Greater than or equal

### UTF-8 Support

MELP strings fully support UTF-8 encoding:

```mlp
function main() returns numeric
    string turkish = "Merhaba Dünya"
    string chinese = "你好世界"
    string russian = "Привет мир"
    string arabic = "مرحبا بالعالم"
    
    print(turkish)
    print(chinese)
    print(russian)
    print(arabic)
    
    return 0
end
```

---

## Boolean Type

The `boolean` type represents truth values: `true` or `false`.

### Declaration and Assignment

```mlp
function main() returns numeric
    boolean isValid = true
    boolean hasError = false
    boolean isReady = true
    
    return 0
end
```

### Boolean Operations

```mlp
function main() returns numeric
    boolean a = true
    boolean b = false
    
    // Logical AND
    boolean both = a && b  // false
    
    // Logical OR
    boolean either = a || b  // true
    
    // Logical NOT
    boolean notA = !a  // false
    
    return 0
end
```

### Boolean from Comparisons

```mlp
function main() returns numeric
    numeric age = 25
    boolean isAdult = age >= 18  // true
    boolean isChild = age < 13   // false
    
    if isAdult then
        print("Adult user")
    end if
    
    return 0
end
```

---

## Variable Scope

Variables in MELP have **function scope** - they are visible from declaration until the end of the function.

### Local Variables

```mlp
function calculate() returns numeric
    numeric x = 10  // Local to calculate()
    numeric y = 20  // Local to calculate()
    return x + y
end

function main() returns numeric
    numeric result = calculate()
    // x and y are NOT accessible here!
    
    numeric z = 100  // Local to main()
    return result + z
end
```

### Variable Shadowing (Not Allowed)

MELP does **not** allow variable shadowing:

```mlp
function main() returns numeric
    numeric x = 10
    
    if true then
        numeric x = 20  // ❌ ERROR! Variable 'x' already declared!
    end if
    
    return 0
end
```

### Block Scope in Control Structures

Variables declared inside `if`, `while`, or other blocks are still function-scoped:

```mlp
function main() returns numeric
    if true then
        numeric temp = 100
    end if
    
    // temp is still accessible here (function scope!)
    numeric result = temp  // ✅ This works in MELP
    
    return result
end
```

> **Note:** This behavior may change in future MELP versions for better scoping.

---

## Type Safety

MELP is **statically typed** and enforces type safety at compile time.

### Type Mismatch Errors

```mlp
function main() returns numeric
    numeric age = "25"  // ❌ ERROR! Cannot assign string to numeric
    string name = 100   // ❌ ERROR! Cannot assign numeric to string
    boolean flag = 1    // ❌ ERROR! Cannot assign numeric to boolean
    
    return 0
end
```

### Correct Type Usage

```mlp
function main() returns numeric
    numeric age = 25           // ✅ Correct
    string name = "Alice"      // ✅ Correct
    boolean isActive = true    // ✅ Correct
    
    return 0
end
```

### Operation Type Compatibility

```mlp
function main() returns numeric
    numeric x = 10
    string y = "20"
    
    numeric sum = x + y  // ❌ ERROR! Cannot add numeric and string
    
    return 0
end
```

**Correct version:**
```mlp
function main() returns numeric
    numeric x = 10
    numeric y = 20
    
    numeric sum = x + y  // ✅ Correct! Both are numeric
    
    return sum
end
```

---

## Best Practices

### 1. Use Meaningful Variable Names

```mlp
// ❌ Bad
function main() returns numeric
    numeric x = 25
    numeric y = 30
    return x + y
end

// ✅ Good
function main() returns numeric
    numeric userAge = 25
    numeric retirementAge = 30
    numeric yearsUntilRetirement = retirementAge - userAge
    return yearsUntilRetirement
end
```

### 2. Initialize Variables Immediately

```mlp
// ✅ Always initialize when declaring
function main() returns numeric
    numeric count = 0
    string message = "Starting..."
    boolean isReady = false
    
    return count
end
```

### 3. Keep Variable Scope Small

```mlp
// ❌ Avoid: Declaring all variables at the top
function main() returns numeric
    numeric a = 0
    numeric b = 0
    numeric c = 0
    numeric d = 0
    
    // ... 50 lines of code ...
    
    a = 10  // What was 'a' for again?
    return a
end

// ✅ Better: Declare close to usage
function main() returns numeric
    // ... code ...
    
    numeric calculationResult = 10
    return calculationResult
end
```

### 4. Use Constants for Magic Numbers

```mlp
// ❌ Avoid magic numbers
function main() returns numeric
    numeric total = price * 100
    return total
end

// ✅ Better: Named constants
function main() returns numeric
    numeric CENTS_PER_DOLLAR = 100
    numeric total = price * CENTS_PER_DOLLAR
    return total
end
```

---

## Exercises

Try these exercises to practice what you've learned!

### Exercise 1: Age Calculator

Create a program that calculates how many years until retirement:

```mlp
function main() returns numeric
    numeric currentAge = 30
    numeric retirementAge = 65
    numeric yearsRemaining = retirementAge - currentAge
    
    print("Years until retirement calculated!")
    return yearsRemaining
end
```

**Expected return value:** `35`

### Exercise 2: Temperature Converter

Create a program that converts Celsius to Fahrenheit:

```mlp
// Formula: F = (C * 9 / 5) + 32
function main() returns numeric
    numeric celsius = 25
    numeric fahrenheit = (celsius * 9 / 5) + 32
    
    print("Temperature converted!")
    return fahrenheit
end
```

**Expected return value:** `77`

### Exercise 3: String Builder

Create a greeting message from parts:

```mlp
function main() returns numeric
    string greeting = "Hello"
    string name = "Alice"
    string punctuation = "!"
    string message = greeting + ", " + name + punctuation
    
    print(message)
    return 0
end
```

**Expected output:** `Hello, Alice!`

### Exercise 4: Validation Check

Create a simple validation program:

```mlp
function main() returns numeric
    string username = "admin"
    string password = "secret123"
    
    boolean validUser = username == "admin"
    boolean validPass = password == "secret123"
    boolean canLogin = validUser && validPass
    
    if canLogin then
        print("Login successful!")
    end if
    
    return 0
end
```

**Expected output:** `Login successful!`

### Exercise 5: Rectangle Area

Calculate the area of a rectangle:

```mlp
function main() returns numeric
    numeric length = 15
    numeric width = 10
    numeric area = length * width
    numeric perimeter = 2 * (length + width)
    
    print("Rectangle calculations complete!")
    return area
end
```

**Expected return value:** `150`

---

## What You Learned

✅ Variable declaration syntax: `type name = value`  
✅ Three fundamental types: numeric, string, boolean  
✅ Arithmetic operations: `+`, `-`, `*`, `/`, `%`  
✅ String concatenation and comparison  
✅ Boolean operations: `&&`, `||`, `!`  
✅ Variable scope (function-level)  
✅ Type safety and compile-time type checking  
✅ Best practices for naming and organizing variables  

---

## Continue Learning

**Next Tutorial:** [03 - Functions](03_functions.md)

Learn how to:
- Define your own functions
- Use parameters and return values
- Call functions from other functions
- Understand recursive functions

---

## Additional Resources

- **Examples:** [examples/basics/](../../examples/basics/)
- **Language Specification:** [docs/language/types.md](../language/types.md)
- **String Support Guide:** [docs/STRING_SUPPORT.md](../STRING_SUPPORT.md)

---

**Keep coding! 🎯**

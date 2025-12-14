# Tutorial 03: Functions

In this tutorial, you'll learn how to define and use functions in MELP, work with parameters and return values, and understand advanced function concepts.

## Table of Contents
- [What You'll Learn](#what-youll-learn)
- [Why Functions?](#why-functions)
- [Function Basics](#function-basics)
- [Parameters](#parameters)
- [Return Values](#return-values)
- [Calling Functions](#calling-functions)
- [Multiple Functions](#multiple-functions)
- [Recursive Functions](#recursive-functions)
- [Function Best Practices](#function-best-practices)
- [Common Patterns](#common-patterns)
- [Exercises](#exercises)
- [Next Steps](#next-steps)

---

## What You'll Learn

By the end of this tutorial, you will:
- Understand the purpose and benefits of functions
- Define functions with parameters and return values
- Call functions from other functions
- Write recursive functions
- Follow best practices for function design

---

## Why Functions?

Functions are reusable blocks of code that:
- **Organize code** into logical units
- **Reduce repetition** (DRY - Don't Repeat Yourself)
- **Make code easier to test** and debug
- **Enable abstraction** - hide complexity behind simple names

### Without Functions (Repetitive)

```mlp
function main() returns numeric
    // Calculate area 1
    numeric length1 = 10
    numeric width1 = 5
    numeric area1 = length1 * width1
    
    // Calculate area 2
    numeric length2 = 20
    numeric width2 = 8
    numeric area2 = length2 * width2
    
    // Calculate area 3
    numeric length3 = 15
    numeric width3 = 12
    numeric area3 = length3 * width3
    
    return 0
end
```

### With Functions (Clean)

```mlp
function calculateArea(numeric length, numeric width) returns numeric
    numeric area = length * width
    return area
end

function main() returns numeric
    numeric area1 = calculateArea(10, 5)
    numeric area2 = calculateArea(20, 8)
    numeric area3 = calculateArea(15, 12)
    
    return 0
end
```

**Benefits:** Less code, easier to maintain, reusable!

---

## Function Basics

### Anatomy of a Function

```mlp
function functionName(parameters) returns returnType
    // Function body
    // Local variables
    // Computations
    return value
end
```

**Components:**
- **`function`** - Keyword to declare a function
- **`functionName`** - Descriptive name (use camelCase)
- **`(parameters)`** - Input values (can be empty)
- **`returns returnType`** - Output type (numeric, string, boolean)
- **Function body** - Code that executes when called
- **`return value`** - Value sent back to caller
- **`end`** - Marks the end of the function

### Simple Function Example

```mlp
function greet() returns numeric
    print("Hello from a function!")
    return 0
end

function main() returns numeric
    greet()
    return 0
end
```

**Compile and run:**
```bash
./compiler/stage0/melp greet.mlp greet.ll
clang greet.ll -o greet
./greet
```

**Output:**
```
Hello from a function!
```

---

## Parameters

Parameters allow functions to accept input values.

### Single Parameter

```mlp
function greet(string name) returns numeric
    print("Hello, " + name + "!")
    return 0
end

function main() returns numeric
    greet("Alice")
    greet("Bob")
    return 0
end
```

**Output:**
```
Hello, Alice!
Hello, Bob!
```

### Multiple Parameters

```mlp
function add(numeric a, numeric b) returns numeric
    numeric sum = a + b
    return sum
end

function main() returns numeric
    numeric result1 = add(10, 20)     // 30
    numeric result2 = add(100, 50)    // 150
    
    print("Calculations complete!")
    return result1 + result2  // 180
end
```

### Parameter Types

Parameters must have explicit types:

```mlp
function display(string message, numeric count, boolean isImportant) returns numeric
    if isImportant then
        print("IMPORTANT: " + message)
    end if
    
    return count
end

function main() returns numeric
    display("System starting", 1, true)
    display("Background task", 5, false)
    return 0
end
```

**Output:**
```
IMPORTANT: System starting
```

---

## Return Values

Functions can return values to their callers.

### Returning Numeric Values

```mlp
function square(numeric x) returns numeric
    numeric result = x * x
    return result
end

function main() returns numeric
    numeric num = 5
    numeric squared = square(num)  // 25
    
    print("Calculation done!")
    return squared
end
```

### Returning String Values

```mlp
function makeGreeting(string name) returns string
    string greeting = "Welcome, " + name + "!"
    return greeting
end

function main() returns numeric
    string message = makeGreeting("Alice")
    print(message)  // "Welcome, Alice!"
    
    return 0
end
```

### Returning Boolean Values

```mlp
function isEven(numeric n) returns boolean
    numeric remainder = n % 2
    boolean result = remainder == 0
    return result
end

function main() returns numeric
    boolean check1 = isEven(4)   // true
    boolean check2 = isEven(7)   // false
    
    if check1 then
        print("4 is even")
    end if
    
    return 0
end
```

**Output:**
```
4 is even
```

### Early Return

You can return from a function before reaching the end:

```mlp
function checkPassword(string password) returns boolean
    if password == "" then
        return false  // Early return - empty password
    end if
    
    if password == "admin" then
        return true   // Early return - correct password
    end if
    
    return false  // Default return - wrong password
end

function main() returns numeric
    boolean valid = checkPassword("admin")
    
    if valid then
        print("Access granted")
    end if
    
    return 0
end
```

---

## Calling Functions

### Basic Function Call

```mlp
function sayHello() returns numeric
    print("Hello!")
    return 0
end

function main() returns numeric
    sayHello()  // Call the function
    return 0
end
```

### Using Return Values

```mlp
function multiply(numeric a, numeric b) returns numeric
    return a * b
end

function main() returns numeric
    // Store return value
    numeric result = multiply(5, 6)
    
    // Use return value directly
    numeric total = multiply(3, 4) + multiply(2, 8)
    
    return result  // 30
end
```

### Passing Variables as Arguments

```mlp
function calculateTotal(numeric price, numeric quantity) returns numeric
    numeric total = price * quantity
    return total
end

function main() returns numeric
    numeric itemPrice = 50
    numeric itemCount = 3
    
    numeric cost = calculateTotal(itemPrice, itemCount)  // 150
    
    return cost
end
```

### Passing Literals as Arguments

```mlp
function add(numeric x, numeric y) returns numeric
    return x + y
end

function main() returns numeric
    // Pass literal values directly
    numeric sum = add(100, 200)  // 300
    
    return sum
end
```

---

## Multiple Functions

You can define multiple functions in the same program.

### Example: Calculator Program

```mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end

function subtract(numeric a, numeric b) returns numeric
    return a - b
end

function multiply(numeric a, numeric b) returns numeric
    return a * b
end

function divide(numeric a, numeric b) returns numeric
    return a / b
end

function main() returns numeric
    numeric x = 20
    numeric y = 5
    
    numeric sum = add(x, y)          // 25
    numeric diff = subtract(x, y)    // 15
    numeric prod = multiply(x, y)    // 100
    numeric quot = divide(x, y)      // 4
    
    print("All calculations complete!")
    return sum + diff + prod + quot  // 144
end
```

### Functions Calling Other Functions

```mlp
function square(numeric x) returns numeric
    return x * x
end

function sumOfSquares(numeric a, numeric b) returns numeric
    numeric sqA = square(a)
    numeric sqB = square(b)
    return sqA + sqB
end

function main() returns numeric
    numeric result = sumOfSquares(3, 4)  // 3² + 4² = 9 + 16 = 25
    return result
end
```

---

## Recursive Functions

A **recursive function** is a function that calls itself.

### Example: Factorial

```mlp
function factorial(numeric n) returns numeric
    if n <= 1 then
        return 1
    end if
    
    numeric smaller = factorial(n - 1)
    return n * smaller
end

function main() returns numeric
    numeric result = factorial(5)  // 5! = 120
    return result
end
```

**How it works:**
```
factorial(5)
= 5 * factorial(4)
= 5 * (4 * factorial(3))
= 5 * (4 * (3 * factorial(2)))
= 5 * (4 * (3 * (2 * factorial(1))))
= 5 * (4 * (3 * (2 * 1)))
= 5 * (4 * (3 * 2))
= 5 * (4 * 6)
= 5 * 24
= 120
```

### Example: Fibonacci

```mlp
function fibonacci(numeric n) returns numeric
    if n <= 1 then
        return n
    end if
    
    numeric prev1 = fibonacci(n - 1)
    numeric prev2 = fibonacci(n - 2)
    return prev1 + prev2
end

function main() returns numeric
    numeric fib5 = fibonacci(5)   // 5
    numeric fib10 = fibonacci(10) // 55
    
    return fib10
end
```

**Fibonacci sequence:** 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55...

### Recursion Requirements

For recursion to work correctly:
1. **Base case** - Condition to stop recursion
2. **Recursive case** - Call to itself with simpler input
3. **Progress** - Each call moves toward the base case

---

## Function Best Practices

### 1. Use Descriptive Names

```mlp
// ❌ Bad - unclear names
function calc(numeric x, numeric y) returns numeric
    return x * y
end

// ✅ Good - clear purpose
function calculateRectangleArea(numeric length, numeric width) returns numeric
    return length * width
end
```

### 2. Keep Functions Small

```mlp
// ❌ Too long - does too much
function processOrder() returns numeric
    // 100 lines of code...
    return 0
end

// ✅ Better - small, focused functions
function validateOrder() returns boolean
    // ...
    return true
end

function calculateTotal() returns numeric
    // ...
    return 0
end

function sendConfirmation() returns numeric
    // ...
    return 0
end
```

**Rule of thumb:** If a function doesn't fit on one screen, split it!

### 3. Single Responsibility

Each function should do **one thing** and do it well.

```mlp
// ❌ Bad - multiple responsibilities
function processUserData(string name, numeric age) returns numeric
    // Validate input
    // Save to database
    // Send email
    // Update UI
    return 0
end

// ✅ Good - single responsibility per function
function validateUserData(string name, numeric age) returns boolean
    // Only validation
    return true
end

function saveUserToDatabase(string name, numeric age) returns numeric
    // Only database operations
    return 0
end

function sendWelcomeEmail(string name) returns numeric
    // Only email sending
    return 0
end
```

### 4. Avoid Side Effects

Pure functions don't modify external state:

```mlp
// ✅ Pure function - no side effects
function add(numeric a, numeric b) returns numeric
    return a + b
end

// ⚠️ Side effects - prints to console
function addAndPrint(numeric a, numeric b) returns numeric
    numeric sum = a + b
    print("Sum is: " + sum)  // Side effect!
    return sum
end
```

**Note:** Side effects aren't always bad, but be intentional about them.

### 5. Parameter Order

Put most important parameters first:

```mlp
// ✅ Good - important parameters first
function sendEmail(string recipient, string subject, string body) returns numeric
    // ...
    return 0
end

function main() returns numeric
    sendEmail("user@example.com", "Welcome", "Thank you for signing up!")
    return 0
end
```

---

## Common Patterns

### Pattern 1: Validator Functions

```mlp
function isValidAge(numeric age) returns boolean
    if age < 0 then
        return false
    end if
    
    if age > 150 then
        return false
    end if
    
    return true
end

function main() returns numeric
    boolean valid = isValidAge(25)  // true
    
    if valid then
        print("Age is valid")
    end if
    
    return 0
end
```

### Pattern 2: Helper Functions

```mlp
function isLeapYear(numeric year) returns boolean
    numeric mod4 = year % 4
    numeric mod100 = year % 100
    numeric mod400 = year % 400
    
    if mod400 == 0 then
        return true
    end if
    
    if mod100 == 0 then
        return false
    end if
    
    if mod4 == 0 then
        return true
    end if
    
    return false
end

function getDaysInFebruary(numeric year) returns numeric
    boolean leap = isLeapYear(year)
    
    if leap then
        return 29
    end if
    
    return 28
end

function main() returns numeric
    numeric days = getDaysInFebruary(2024)  // 29
    return days
end
```

### Pattern 3: Builder Functions

```mlp
function buildFullName(string first, string last) returns string
    return first + " " + last
end

function buildAddress(string street, string city, string zip) returns string
    return street + ", " + city + " " + zip
end

function main() returns numeric
    string name = buildFullName("John", "Doe")
    string address = buildAddress("123 Main St", "Boston", "02101")
    
    print(name)
    print(address)
    
    return 0
end
```

---

## Exercises

### Exercise 1: Max Function

Write a function that returns the larger of two numbers:

```mlp
function max(numeric a, numeric b) returns numeric
    if a > b then
        return a
    end if
    return b
end

function main() returns numeric
    numeric result = max(15, 23)  // Should return 23
    return result
end
```

### Exercise 2: Power Function

Write a function to calculate x^n (using a loop would require while, so use recursion):

```mlp
function power(numeric base, numeric exponent) returns numeric
    if exponent == 0 then
        return 1
    end if
    
    numeric smaller = power(base, exponent - 1)
    return base * smaller
end

function main() returns numeric
    numeric result = power(2, 5)  // 2^5 = 32
    return result
end
```

### Exercise 3: Temperature Converter

Create functions to convert between Celsius and Fahrenheit:

```mlp
function celsiusToFahrenheit(numeric celsius) returns numeric
    numeric fahrenheit = (celsius * 9 / 5) + 32
    return fahrenheit
end

function fahrenheitToCelsius(numeric fahrenheit) returns numeric
    numeric celsius = (fahrenheit - 32) * 5 / 9
    return celsius
end

function main() returns numeric
    numeric f = celsiusToFahrenheit(25)   // 77
    numeric c = fahrenheitToCelsius(100)  // 37
    
    return f + c  // 114
end
```

### Exercise 4: String Formatter

Create a function that formats a person's information:

```mlp
function formatPerson(string name, numeric age, string city) returns string
    string ageStr = "Age: " + age
    string result = name + " (" + ageStr + ") from " + city
    return result
end

function main() returns numeric
    string info = formatPerson("Alice", 30, "Boston")
    print(info)  // "Alice (Age: 30) from Boston"
    
    return 0
end
```

### Exercise 5: Sum of Range

Write a recursive function to calculate the sum of numbers from 1 to n:

```mlp
function sumRange(numeric n) returns numeric
    if n <= 0 then
        return 0
    end if
    
    numeric smaller = sumRange(n - 1)
    return n + smaller
end

function main() returns numeric
    numeric result = sumRange(10)  // 1+2+3+...+10 = 55
    return result
end
```

---

## What You Learned

✅ Function declaration syntax: `function name(params) returns type`  
✅ Defining parameters with explicit types  
✅ Returning values from functions  
✅ Calling functions and using return values  
✅ Writing multiple functions in one program  
✅ Creating recursive functions (factorial, fibonacci)  
✅ Function best practices (naming, size, responsibility)  
✅ Common function patterns (validators, helpers, builders)  

---

## Continue Learning

**Next Tutorial:** [04 - Strings](04_strings.md)

Learn how to:
- Work with string literals and variables
- Perform string concatenation
- Compare strings
- Understand the STO string type system

---

## Additional Resources

- **Examples:** [examples/basics/](../../examples/basics/)
- **Advanced Examples:** [examples/advanced/](../../examples/advanced/)
- **Function Specifications:** [docs/language/functions.md](../language/functions.md)

---

**Master functions and level up your MELP skills! 🚀**

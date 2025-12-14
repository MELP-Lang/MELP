# Tutorial 04: Strings in MELP

In this tutorial, you'll learn everything about working with strings in MELP - from basic literals to advanced operations like concatenation and comparison.

## Table of Contents
- [What You'll Learn](#what-youll-learn)
- [String Basics](#string-basics)
- [String Literals](#string-literals)
- [String Variables](#string-variables)
- [String Concatenation](#string-concatenation)
- [String Comparison](#string-comparison)
- [Strings in Functions](#strings-in-functions)
- [UTF-8 Support](#utf-8-support)
- [The STO String Type](#the-sto-string-type)
- [Behind the Scenes](#behind-the-scenes)
- [Best Practices](#best-practices)
- [Exercises](#exercises)
- [Next Steps](#next-steps)

---

## What You'll Learn

By the end of this tutorial, you will:
- Understand MELP's string type system
- Use string literals and variables
- Concatenate strings with the `+` operator
- Compare strings with `==`, `!=`, `<`, `>`, `<=`, `>=`
- Pass strings to functions as parameters
- Work with UTF-8 encoded text
- Understand how strings are implemented in MELP (STO system)

---

## String Basics

In MELP, the `string` type represents text data. Strings are:
- **Immutable** - Once created, they cannot be changed
- **UTF-8 encoded** - Full Unicode support
- **Reference types** - Stored as pointers to character data
- **Null-terminated** - Compatible with C-style strings

### Quick Example

```mlp
function main() returns numeric
    string message = "Hello, MELP!"
    print(message)
    return 0
end
```

**Output:**
```
Hello, MELP!
```

---

## String Literals

A **string literal** is text enclosed in double quotes.

### Basic String Literals

```mlp
function main() returns numeric
    print("Hello")
    print("World")
    print("MELP is awesome!")
    return 0
end
```

**Output:**
```
Hello
World
MELP is awesome!
```

### Empty Strings

```mlp
function main() returns numeric
    string empty = ""
    print(empty)  // Prints nothing (empty line)
    return 0
end
```

### Multi-Word Strings

```mlp
function main() returns numeric
    string sentence = "The quick brown fox jumps over the lazy dog"
    print(sentence)
    return 0
end
```

### Strings with Special Characters

```mlp
function main() returns numeric
    string withSpaces = "Hello    World"    // Multiple spaces
    string withPunctuation = "Hello, World!" // Punctuation
    string withNumbers = "Year 2024"         // Numbers
    
    print(withSpaces)
    print(withPunctuation)
    print(withNumbers)
    
    return 0
end
```

**Output:**
```
Hello    World
Hello, World!
Year 2024
```

---

## String Variables

You can store strings in variables for reuse.

### Declaration and Assignment

```mlp
function main() returns numeric
    string name = "Alice"
    string language = "MELP"
    string greeting = "Welcome!"
    
    print(name)
    print(language)
    print(greeting)
    
    return 0
end
```

**Output:**
```
Alice
MELP
Welcome!
```

### Using String Variables

```mlp
function main() returns numeric
    string userName = "Bob"
    
    print("User logged in:")
    print(userName)
    
    return 0
end
```

**Output:**
```
User logged in:
Bob
```

### Reassignment (Not Allowed)

In current MELP, variables cannot be reassigned:

```mlp
function main() returns numeric
    string message = "Hello"
    message = "Goodbye"  // ❌ ERROR! Reassignment not supported yet
    return 0
end
```

> **Note:** Variable reassignment is planned for future MELP versions.

---

## String Concatenation

**String concatenation** means joining strings together using the `+` operator.

### Basic Concatenation

```mlp
function main() returns numeric
    string greeting = "Hello"
    string name = "World"
    string message = greeting + " " + name
    
    print(message)  // "Hello World"
    
    return 0
end
```

**Output:**
```
Hello World
```

### Concatenating Multiple Strings

```mlp
function main() returns numeric
    string first = "MELP"
    string second = "is"
    string third = "awesome"
    
    string sentence = first + " " + second + " " + third
    
    print(sentence)  // "MELP is awesome"
    
    return 0
end
```

**Output:**
```
MELP is awesome
```

### Concatenating Literals and Variables

```mlp
function main() returns numeric
    string userName = "Alice"
    string message = "Welcome, " + userName + "!"
    
    print(message)  // "Welcome, Alice!"
    
    return 0
end
```

**Output:**
```
Welcome, Alice!
```

### Chained Concatenation

```mlp
function main() returns numeric
    string a = "One"
    string b = "Two"
    string c = "Three"
    string d = "Four"
    
    string result = a + " " + b + " " + c + " " + d
    
    print(result)  // "One Two Three Four"
    
    return 0
end
```

---

## String Comparison

MELP supports comparing strings using comparison operators.

### Equality Comparison

```mlp
function main() returns numeric
    string password = "admin123"
    
    if password == "admin123" then
        print("Password correct!")
    end if
    
    if password != "wrong" then
        print("Password is not 'wrong'")
    end if
    
    return 0
end
```

**Output:**
```
Password correct!
Password is not 'wrong'
```

### Lexicographic Comparison

Strings are compared alphabetically (dictionary order):

```mlp
function main() returns numeric
    string word1 = "apple"
    string word2 = "banana"
    
    if word1 < word2 then
        print("apple comes before banana")
    end if
    
    if word2 > word1 then
        print("banana comes after apple")
    end if
    
    return 0
end
```

**Output:**
```
apple comes before banana
banana comes after apple
```

### All Comparison Operators

```mlp
function main() returns numeric
    string a = "apple"
    string b = "banana"
    
    // Equality
    boolean eq = a == a     // true
    boolean neq = a != b    // true
    
    // Ordering
    boolean less = a < b    // true  (apple < banana)
    boolean greater = b > a // true  (banana > apple)
    boolean leq = a <= a    // true  (apple <= apple)
    boolean geq = b >= a    // true  (banana >= apple)
    
    if less then
        print("apple < banana: true")
    end if
    
    return 0
end
```

**Output:**
```
apple < banana: true
```

### Case Sensitivity

String comparisons are **case-sensitive**:

```mlp
function main() returns numeric
    string lower = "hello"
    string upper = "HELLO"
    
    if lower == upper then
        print("Same")
    end if
    
    if lower != upper then
        print("Different!")  // This will print
    end if
    
    return 0
end
```

**Output:**
```
Different!
```

---

## Strings in Functions

Strings can be passed as parameters and returned from functions.

### String Parameters

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

### Returning Strings

```mlp
function makeGreeting(string name) returns string
    string greeting = "Welcome, " + name
    return greeting
end

function main() returns numeric
    string msg1 = makeGreeting("Alice")
    string msg2 = makeGreeting("Bob")
    
    print(msg1)
    print(msg2)
    
    return 0
end
```

**Output:**
```
Welcome, Alice
Welcome, Bob
```

### Multiple String Parameters

```mlp
function combineNames(string first, string last) returns string
    string fullName = first + " " + last
    return fullName
end

function main() returns numeric
    string name = combineNames("John", "Doe")
    print(name)  // "John Doe"
    
    return 0
end
```

**Output:**
```
John Doe
```

### String and Numeric Parameters

```mlp
function formatMessage(string text, numeric count) returns string
    string result = text + " (count: " + count + ")"
    return result
end

function main() returns numeric
    string msg = formatMessage("Items", 5)
    print(msg)  // "Items (count: 5)"
    
    return 0
end
```

**Output:**
```
Items (count: 5)
```

---

## UTF-8 Support

MELP fully supports UTF-8 encoding, allowing international characters.

### International Text

```mlp
function main() returns numeric
    string turkish = "Merhaba Dünya"
    string chinese = "你好世界"
    string russian = "Привет мир"
    string arabic = "مرحبا بالعالم"
    string japanese = "こんにちは世界"
    
    print(turkish)
    print(chinese)
    print(russian)
    print(arabic)
    print(japanese)
    
    return 0
end
```

**Output:**
```
Merhaba Dünya
你好世界
Привет мир
مرحبا بالعالم
こんにちは世界
```

### Emoji Support

```mlp
function main() returns numeric
    string rocket = "🚀"
    string heart = "❤️"
    string star = "⭐"
    
    string message = "MELP " + rocket + " is awesome " + heart + star
    
    print(message)  // "MELP 🚀 is awesome ❤️⭐"
    
    return 0
end
```

**Output:**
```
MELP 🚀 is awesome ❤️⭐
```

### Mixed Languages

```mlp
function main() returns numeric
    string mixed = "Hello नमस्ते Bonjour 你好"
    print(mixed)
    return 0
end
```

**Output:**
```
Hello नमस्ते Bonjour 你好
```

---

## The STO String Type

MELP uses the **STO (String Type Object)** system for string management.

### What is STO?

STO is MELP's runtime type system that provides:
- **Automatic memory management** - No manual allocation/deallocation
- **String concatenation** - Runtime `mlp_string_concat()` function
- **String comparison** - Runtime `mlp_string_compare()` function
- **Type safety** - Compile-time type checking

### STO Runtime Functions

When you use string operations, MELP calls these C functions:

```c
// Runtime functions (implemented in runtime/sto/)
char* mlp_string_concat(const char* s1, const char* s2);
int mlp_string_compare(const char* s1, const char* s2);
int mlp_string_equals(const char* s1, const char* s2);
```

### Example: What Happens Behind the Scenes

**MELP Code:**
```mlp
function main() returns numeric
    string greeting = "Hello"
    string name = "World"
    string message = greeting + " " + name
    
    if message == "Hello World" then
        print("Success!")
    end if
    
    return 0
end
```

**Generated x86-64 Assembly (simplified):**
```asm
; String concatenation
leaq .LC0(%rip), %rdi        ; "Hello"
leaq .LC1(%rip), %rsi        ; " "
call mlp_string_concat@PLT   ; Returns "Hello "

movq %rax, %rdi              ; "Hello "
leaq .LC2(%rip), %rsi        ; "World"
call mlp_string_concat@PLT   ; Returns "Hello World"

; String comparison
movq %rax, %rdi              ; "Hello World"
leaq .LC3(%rip), %rsi        ; "Hello World"
call mlp_string_compare@PLT  ; Returns 0 (equal)
```

---

## Behind the Scenes

### String Storage

Strings are stored in the **read-only data section (.rodata)**:

**LLVM IR:**
```llvm
@.str.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@.str.1 = private unnamed_addr constant [6 x i8] c"World\00", align 1
```

**x86-64 Assembly:**
```asm
.section .rodata
.LC0:
    .string "Hello"
.LC1:
    .string "World"
```

### String Variables

String variables are stored as **pointers** on the stack:

**LLVM IR:**
```llvm
%message_ptr = alloca i8*, align 8
%tmp1 = getelementptr inbounds [6 x i8], [6 x i8]* @.str.0, i64 0, i64 0
store i8* %tmp1, i8** %message_ptr, align 8
```

**x86-64 Assembly:**
```asm
subq    $16, %rsp           ; Allocate stack space
leaq    .LC0(%rip), %rax    ; Load string address
movq    %rax, -8(%rbp)      ; Store pointer on stack
```

---

## Best Practices

### 1. Use String Concatenation for Building Messages

```mlp
// ✅ Good - clear and readable
function formatUser(string name, numeric id) returns string
    string result = "User: " + name + " (ID: " + id + ")"
    return result
end
```

### 2. Compare Strings with == for Equality

```mlp
// ✅ Good - clear intent
function checkPassword(string password) returns boolean
    if password == "admin123" then
        return true
    end if
    return false
end
```

### 3. Use Descriptive String Variable Names

```mlp
// ❌ Bad
function main() returns numeric
    string s = "Hello"
    string s2 = "World"
    return 0
end

// ✅ Good
function main() returns numeric
    string greeting = "Hello"
    string target = "World"
    return 0
end
```

### 4. Avoid Empty String Comparisons

```mlp
// ✅ Explicit empty check
function isEmpty(string text) returns boolean
    if text == "" then
        return true
    end if
    return false
end
```

---

## Exercises

### Exercise 1: Full Name Builder

Create a function that builds a full name from parts:

```mlp
function buildFullName(string first, string middle, string last) returns string
    string result = first + " " + middle + " " + last
    return result
end

function main() returns numeric
    string name = buildFullName("John", "Q", "Public")
    print(name)  // "John Q Public"
    return 0
end
```

### Exercise 2: Email Validator

Create a simple email validator:

```mlp
function isValidEmail(string email) returns boolean
    // Simple check: must contain "@"
    if email == "" then
        return false
    end if
    
    // In a real implementation, you'd check for "@" character
    // For now, we'll just check if it matches a pattern
    if email == "user@example.com" then
        return true
    end if
    
    return false
end

function main() returns numeric
    boolean valid = isValidEmail("user@example.com")
    
    if valid then
        print("Valid email!")
    end if
    
    return 0
end
```

### Exercise 3: Greeting Generator

Create different greetings based on time of day:

```mlp
function getGreeting(string timeOfDay) returns string
    if timeOfDay == "morning" then
        return "Good morning!"
    end if
    
    if timeOfDay == "afternoon" then
        return "Good afternoon!"
    end if
    
    if timeOfDay == "evening" then
        return "Good evening!"
    end if
    
    return "Hello!"
end

function main() returns numeric
    string msg1 = getGreeting("morning")
    string msg2 = getGreeting("evening")
    
    print(msg1)  // "Good morning!"
    print(msg2)  // "Good evening!"
    
    return 0
end
```

### Exercise 4: String Comparison

Compare three strings alphabetically:

```mlp
function findFirst(string a, string b, string c) returns string
    if a < b && a < c then
        return a
    end if
    
    if b < a && b < c then
        return b
    end if
    
    return c
end

function main() returns numeric
    string first = findFirst("zebra", "apple", "mango")
    print(first)  // "apple"
    return 0
end
```

### Exercise 5: URL Builder

Build a URL from parts:

```mlp
function buildURL(string protocol, string domain, string path) returns string
    string result = protocol + "://" + domain + "/" + path
    return result
end

function main() returns numeric
    string url = buildURL("https", "example.com", "api/users")
    print(url)  // "https://example.com/api/users"
    return 0
end
```

---

## What You Learned

✅ String literals with double quotes `"text"`  
✅ String variables: `string name = "value"`  
✅ String concatenation with `+` operator  
✅ String comparison: `==`, `!=`, `<`, `>`, `<=`, `>=`  
✅ Passing strings to functions as parameters  
✅ Returning strings from functions  
✅ UTF-8 support for international characters and emoji  
✅ STO runtime system for string operations  
✅ How strings are compiled to LLVM IR and assembly  

---

## Continue Learning

**Recommended Next Steps:**

1. **Explore Advanced Examples:**
   - Check `examples/basics/test_string_*.mlp` files
   - Study generated LLVM IR and assembly

2. **Read Technical Documentation:**
   - [docs/STRING_SUPPORT.md](../STRING_SUPPORT.md) - Deep dive into string implementation
   - [docs/LLVM_IR_GUIDE.md](../LLVM_IR_GUIDE.md) - Understanding LLVM IR generation

3. **Experiment:**
   - Try complex string concatenations
   - Test UTF-8 edge cases
   - Compare LLVM vs x86-64 backends

---

## Additional Resources

- **String Examples:** [examples/basics/](../../examples/basics/)
- **String Runtime:** [runtime/sto/](../../runtime/sto/)
- **String Implementation:** [docs/STRING_SUPPORT.md](../STRING_SUPPORT.md)
- **Architecture:** [ARCHITECTURE.md](../../ARCHITECTURE.md)

---

**You've mastered strings in MELP! 🎉**

String support is one of MELP's most powerful features, enabling you to build real-world applications with text processing, user interfaces, and more.

**Happy coding! 🚀**

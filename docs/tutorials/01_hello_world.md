# Tutorial 01: Hello World - Your First MELP Program

Welcome to MELP! In this tutorial, you'll learn how to write, compile, and run your first MELP program.

## Table of Contents
- [What You'll Learn](#what-youll-learn)
- [Prerequisites](#prerequisites)
- [Writing Your First Program](#writing-your-first-program)
- [Understanding the Code](#understanding-the-code)
- [Compiling with LLVM Backend](#compiling-with-llvm-backend)
- [Compiling with x86-64 Backend](#compiling-with-x86-64-backend)
- [Analyzing the Output](#analyzing-the-output)
- [Common Issues](#common-issues)
- [Next Steps](#next-steps)

---

## What You'll Learn

By the end of this tutorial, you will:
- Write a simple MELP program
- Understand the basic structure of a MELP program
- Compile MELP code using both LLVM and x86-64 backends
- Run your compiled program
- Understand the compilation pipeline

---

## Prerequisites

Before starting, make sure you have:
- MELP compiler installed (Stage 0)
- LLVM 19 installed (for LLVM backend)
- GCC or Clang (for linking)
- Basic understanding of command-line usage

**Check your installation:**
```bash
# Verify MELP compiler exists
ls compiler/stage0/melp

# Verify LLVM is installed
llvm-config --version  # Should show 19.x
```

---

## Writing Your First Program

Let's create the classic "Hello World" program in MELP.

**Create a file:** `hello.mlp`

```mlp
function main() returns numeric
    print("Hello, MELP!")
    return 0
end
```

That's it! Just 4 lines of code. Let's break it down.

---

## Understanding the Code

### Line 1: Function Declaration
```mlp
function main() returns numeric
```

- **`function`** - Keyword that declares a function
- **`main()`** - The entry point of every MELP program (like C, Rust, or Go)
- **`returns numeric`** - Specifies that this function returns a numeric value (like `int` in C)

> **Note:** Every MELP program must have a `main()` function. This is where execution begins.

### Line 2: Print Statement
```mlp
    print("Hello, MELP!")
```

- **`print()`** - Built-in function that outputs text to the console
- **`"Hello, MELP!"`** - A string literal (text enclosed in double quotes)

> **Note:** String literals in MELP support UTF-8 encoding, so you can use special characters!

### Line 3: Return Statement
```mlp
    return 0
```

- **`return`** - Keyword that exits the function and returns a value
- **`0`** - Convention: 0 means "success", non-zero means "error"

### Line 4: End Statement
```mlp
end
```

- **`end`** - Marks the end of the function body

> **MELP Philosophy:** We use `end` instead of curly braces `{}` for better readability!

---

## Compiling with LLVM Backend

The **LLVM backend** generates portable LLVM IR (Intermediate Representation) that can run on any platform.

### Step 1: Compile to LLVM IR

```bash
./compiler/stage0/melp hello.mlp hello.ll
```

**What happens:**
1. MELP reads `hello.mlp`
2. Lexer breaks it into tokens
3. Parser builds an Abstract Syntax Tree (AST)
4. LLVM backend generates `hello.ll` (LLVM IR file)

### Step 2: View the Generated LLVM IR

```bash
cat hello.ll
```

**Output (simplified):**
```llvm
; ModuleID = 'hello.mlp'
source_filename = "hello.mlp"

@.str.0 = private unnamed_addr constant [13 x i8] c"Hello, MELP!\00", align 1

declare i32 @puts(i8*)

define i64 @main() {
entry:
    %tmp1 = getelementptr inbounds [13 x i8], [13 x i8]* @.str.0, i64 0, i64 0
    %tmp2 = call i32 @puts(i8* %tmp1)
    ret i64 0
}
```

**Key Parts:**
- `@.str.0` - Your string stored in read-only memory
- `@main()` - Your main function
- `call i32 @puts` - Prints the string
- `ret i64 0` - Returns 0

### Step 3: Compile LLVM IR to Executable

```bash
clang hello.ll -o hello
```

**Alternative with optimization:**
```bash
clang -O2 hello.ll -o hello
```

### Step 4: Run Your Program

```bash
./hello
```

**Output:**
```
Hello, MELP!
```

🎉 **Congratulations!** You just ran your first MELP program!

---

## Compiling with x86-64 Backend

The **x86-64 backend** generates native assembly code for Linux x86-64 systems.

### Step 1: Compile to Assembly

```bash
./compiler/stage0/melp hello.mlp hello.s
```

**What happens:**
1. MELP reads `hello.mlp`
2. Lexer and Parser process the code
3. x86-64 backend generates `hello.s` (assembly file)

### Step 2: View the Generated Assembly

```bash
cat hello.s
```

**Output (simplified):**
```asm
.section .rodata
.LC0:
    .string "Hello, MELP!"

.text
.globl main
.type main, @function
main:
    pushq   %rbp
    movq    %rsp, %rbp
    
    leaq    .LC0(%rip), %rdi
    call    puts@PLT
    
    movl    $0, %eax
    popq    %rbp
    ret
```

**Key Parts:**
- `.rodata` - Read-only data section (where strings live)
- `.LC0` - Your "Hello, MELP!" string
- `main:` - Your main function in assembly
- `call puts@PLT` - Calls the C library `puts()` function
- `movl $0, %eax` - Returns 0

### Step 3: Assemble and Link

```bash
gcc hello.s -o hello
```

**Alternative with runtime library:**
```bash
gcc hello.s runtime/stdlib/libsto.a -o hello
```

### Step 4: Run Your Program

```bash
./hello
```

**Output:**
```
Hello, MELP!
```

---

## Analyzing the Output

### Comparing Backends

| Feature | LLVM Backend | x86-64 Backend |
|---------|--------------|----------------|
| **Portability** | ✅ Cross-platform | ❌ Linux x86-64 only |
| **Optimization** | ✅ LLVM optimizations | ❌ No optimization |
| **Learning Value** | Medium | ✅ High (see raw assembly) |
| **Speed** | ✅ Fast (optimized) | Medium (no optimization) |
| **File Size** | Small (with `-O2`) | Small |

### When to Use Which Backend?

**Use LLVM Backend when:**
- You want cross-platform compatibility (Windows, macOS, Linux)
- You need optimized code (`-O2`, `-O3`)
- You're building production software

**Use x86-64 Backend when:**
- You want to learn how assembly works
- You're debugging low-level issues
- You're curious about how MELP generates machine code
- You're on Linux x86-64 (desktop/laptop)

---

## Common Issues

### Issue 1: "Command not found: ./compiler/stage0/melp"

**Problem:** Compiler not built yet.

**Solution:**
```bash
cd compiler/stage0
make clean
make
```

### Issue 2: "LLVM not found"

**Problem:** LLVM 19 not installed.

**Solution (Ubuntu/Debian):**
```bash
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 19
```

### Issue 3: "Undefined reference to `puts`"

**Problem:** Standard library not linked.

**Solution:**
```bash
# LLVM backend
clang hello.ll -o hello

# x86-64 backend (if using runtime functions)
gcc hello.s runtime/stdlib/libsto.a -o hello
```

### Issue 4: Print Output Not Showing

**Problem:** Output buffering.

**Solution:** Your output should show immediately with `print()`. If not, try:
```bash
./hello | cat
```

---

## Next Steps

Now that you've created your first MELP program, try these challenges:

### Challenge 1: Multiple Prints
Modify your program to print multiple lines:
```mlp
function main() returns numeric
    print("Hello, MELP!")
    print("Welcome to programming!")
    print("Let's build something awesome!")
    return 0
end
```

### Challenge 2: UTF-8 Support
Try printing special characters:
```mlp
function main() returns numeric
    print("Merhaba Dünya! 🚀")
    print("你好世界")
    print("Привет мир")
    return 0
end
```

### Challenge 3: Compare Backends
Compile the same program with both backends and compare:
```bash
# LLVM
./compiler/stage0/melp hello.mlp hello_llvm.ll
cat hello_llvm.ll > llvm_output.txt

# x86-64
./compiler/stage0/melp hello.mlp hello_x86.s
cat hello_x86.s > x86_output.txt

# Compare
diff llvm_output.txt x86_output.txt
```

---

## What You Learned

✅ MELP program structure (`function main()`, `return`, `end`)  
✅ Using `print()` to display text  
✅ Compiling with LLVM backend (`.mlp` → `.ll` → executable)  
✅ Compiling with x86-64 backend (`.mlp` → `.s` → executable)  
✅ Understanding generated LLVM IR and assembly code  
✅ Differences between the two backends  

---

## Continue Learning

**Next Tutorial:** [02 - Variables and Types](02_variables.md)

Learn how to:
- Declare variables
- Use different data types (numeric, string, boolean)
- Understand variable scope
- Perform basic operations

---

## Additional Resources

- **Language Reference:** [docs/language/README.md](../language/README.md)
- **LLVM IR Guide:** [docs/LLVM_IR_GUIDE.md](../LLVM_IR_GUIDE.md)
- **Architecture Overview:** [ARCHITECTURE.md](../../ARCHITECTURE.md)
- **Examples:** [examples/basics/](../../examples/basics/)

---

**Happy coding! 🚀**

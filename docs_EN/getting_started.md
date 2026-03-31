# Getting Started with MELP

**Version:** 1.0  
**Date:** 2026-03-30

MELP (Modern Expressive Language Platform) compiles `.mlp` source files to LLVM IR, which is then linked and executed via Clang.

---

## Installation

### Prerequisites

- Clang 14+
- LLVM 14+ (`llvm-as`, `opt`)
- `mlp_runtime.bc` (from the MELP repository)

### Build the compiler

```bash
cd /path/to/MELP/LLVM/stage1
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
# Result: build/bin/melp_compiler
```

---

## Your First Program

Create `hello.mlp`:

```mlp
none function main()
    print("Hello, World!")
end function
```

### Compile and run

```bash
# Compile to LLVM IR
melp_compiler hello.mlp -o hello.ll

# Link and produce native binary
clang -O0 -x ir hello.ll mlp_runtime.bc -lm -o hello

# Run
./hello
```

Expected output:

```
Hello, World!
```

---

## Turkish MLP

MELP supports natural-language keywords in Turkish (and other languages). Add language headers to your source file:

```mlp
#lang turkish
#syntax mlp

hiçbir fonksiyon giriş()
    yazdır("Merhaba Dünya!")
fonksiyon sonu
```

Save as `merhaba.tr.mlp` and compile the same way. The normalizer automatically translates Turkish keywords to canonical MLP before compilation.

---

## Variables and Basic Output

```mlp
none function main()
    numeric x = 42
    string name = "MELP"
    boolean ready = true

    print("x = " + str(x))
    print("name = " + name)
    print("ready = " + str(ready))
end function
```

Key rules:

- Use `;` to separate function parameters (not `,`)
- No semicolons at end of lines
- Block terminators are two words: `end function`, `end if`, `end loop`

---

## Control Flow

```mlp
none function main()
    numeric score = 75

    if score >= 90 then
        print("AA")
    else if score >= 80 then
        print("BA")
    else if score >= 70 then
        print("BB")
    else
        print("FF")
    end if
end function
```

---

## Loops

```mlp
none function main()
    -- Count from 0 to 4
    loop i = 0 to 5
        print(str(i))
    end loop

    -- While-style loop
    numeric n = 10
    loop n > 0
        print(str(n))
        n = n - 1
    end loop
end function
```

---

## Functions

```mlp
numeric function add(numeric a; numeric b)
    return a + b
end function

none function main()
    numeric result = add(3; 7)
    print("3 + 7 = " + str(result))
end function
```

---

## Structs

```mlp
struct Point
    numeric x
    numeric y
end struct

numeric function Point.sum()
    return this.x + this.y
end function

none function main()
    Point p = Point { x: 10; y: 20; }
    print("Sum: " + str(p.sum()))
end function
```

---

## Arrays

```mlp
none function main()
    numeric[] nums = [10; 20; 30; 40; 50]
    print("First: " + str(nums[0]))
    print("Count: " + str(nums.length))

    nums.push(60)
    numeric last = nums.pop()
    print("Popped: " + str(last))
end function
```

---

## Compile Pipeline

```
source.mlp
    ↓  normalizer (keywords.json → canonical MLP → pmlp)
pmlp tokens
    ↓  MELP compiler (STAGE1)
source.ll (LLVM IR)
    ↓  clang + mlp_runtime.bc
native binary
```

For Turkish sources (`.tr.mlp`), the normalizer first maps Turkish keywords to their English equivalents before the compile pipeline runs.

---

## Next Steps

- [Language Reference](language_reference.md) — Complete syntax reference
- [Standard Library](stdlib.md) — Built-in functions
- [Language Reference (Turkish)](../belgeler/dil/referans.md) — Türkçe sözdizim referansı

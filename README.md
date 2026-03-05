# MELP Programming Language · Alfa

> A statically-typed, compiled programming language with a clean syntax.  
> Compiler produces native binaries via LLVM.

---

## Hello World

```melp
function main() as numeric
    print("Merhaba Dünya!")
    return 0
end_function
```

Turkish keywords also work (`.tr.mlp` files):

```melp
fonksiyon ana() olarak sayısal
    yaz("Merhaba Dünya!")
    döndür 0
son_fonksiyon
```

---

## Quick Start (VS Code)

1. Download `melp-ide-*.vsix` from [Releases](../../releases)
2. In VS Code: `Ctrl+Shift+P` → **Install from VSIX** → select the file
3. Open or create a `.mlp` file — press `Ctrl+F5` to run

The compiler is bundled inside the extension — no separate install needed.

**Linux only in Alfa.** macOS and Windows support planned for Beta.

---

## Language Features

```melp
-- Variables
numeric x = 42
numeric pi = 3,14        -- Turkish decimal: comma = decimal separator
string name = "MELP"
boolean active = true

-- Functions
function add(a as numeric; b as numeric) as numeric
    return a + b
end_function

-- Control flow
if x > 10 then
    print("large")
else_if x > 5 then
    print("medium")
else
    print("small")
end_if

-- Loops
for i = 1 to 10
    print(i)
end_for

while x > 0
    x = x - 1
end_while

-- Arrays
numeric[] nums = [1; 2; 3; 4; 5]
print(nums[0])

-- Structs
struct Point
    numeric x
    numeric y
end_struct

-- Enums + Pattern matching
enum Color
    Red
    Green
    Blue
end_enum

match c
    case Red
        print("red")
    end_case
end_match

-- Import
import "mymodule.mlp"
```

---

## Examples

See [`examples/`](examples/) — 40+ programs organized by level:

| Folder | Topics |
|---|---|
| `base/` | hello world, variables, arithmetic, strings, control flow, functions, structs, enums |
| `cli/` | argument parsing, file I/O, text processing |
| `stdlib/` | math, string ops, sorting algorithms |
| `games/` | number guessing, text adventure |

---

## Multi-language Support

MELP programs can be written in multiple natural languages.  
A normalizer transparently converts them before compilation:

```
program.mlp      ← English (canonical)
program.tr.mlp   ← Turkish keywords → auto-normalized
program.ru.mlp   ← Russian keywords → auto-normalized
```

---

## Project Structure

```
MELP/
├── examples/          ← sample programs (40+)
├── src/
│   ├── compiler/      ← MELP compiler written in .mlp (WIP)
│   │   └── lexer.mlp  ← 676-line tokenizer
│   ├── normalizer/    ← multi-language keyword normalizer
│   └── stdlib/        ← standard library modules
├── editors/
│   └── VSIX/          ← VS Code extension source
└── belgeler/          ← language documentation (Turkish)
```

---

## Roadmap

| Milestone | Status |
|---|---|
| Alfa: compiler + VS Code extension + 40 examples | ✅ Now |
| Beta: `parser.mlp` + macOS/Windows support | 🔧 In progress |
| v1.0: compiler written entirely in MELP (self-hosting) | 🔴 Planned |

---

## Platform Support

| Platform | Alfa | Beta |
|---|---|---|
| Linux x86_64 | ✅ | ✅ |
| macOS | — | 🔄 |
| Windows | — | 🔄 |

---

## License

[MIT](LICENSE)

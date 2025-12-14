# MELP Programming Language

**M**odern **E**asy **L**earning **P**rogramming

> 🚀 Modern, type-safe, and beginner-friendly programming language with LLVM backend
> 
> 🇹🇷 Turkish documentation · Easy learning curve · Production-ready performance

---

## 🎯 What is MELP?

MELP is a statically-typed, compiled programming language designed for:
- **Beginners**: Clean syntax, Turkish documentation, educational focus
- **Performance**: LLVM backend with native code generation
- **Modern Features**: String handling, type safety, memory safety
- **Self-Hosting**: Compiler written in MELP itself (Stage 1+)

**Core Philosophy:** *Stateless by Default, Stateful by Choice* - All variables are function-local. For persistent state, explicitly import the `state` module.

### Why MELP?

- ✅ **Kolay Öğrenme (Easy Learning)**: Türkçe dokümantasyon, basit sözdizimi
- ✅ **Performans**: LLVM optimization ile C'ye yakın hız (~90-95%)
- ✅ **Güvenlik**: Static typing, compile-time error detection
- ✅ **Cross-Platform**: Linux, macOS, Windows desteği (LLVM sayesinde)
- ✅ **Modern**: String operations, type inference, clean syntax

📖 **Daha Fazla Bilgi:** [MELP_VISION.md](MELP_VISION.md) - Proje felsefesi ve vizyon

---

## 🎉 Current Status: Stage 0 Bootstrap Compiler (~98% Complete!)

**Latest Achievement: Phase 17 String Support - 100% Complete! 🎉**

### ✅ Completed Features (Phase 1-17)

#### Core Language Features
- ✅ **Variables**: Int, String, Boolean types with type inference
- ✅ **Functions**: Parameters, return values, nested calls
- ✅ **Control Flow**: if/else, while loops, for loops (nested support)
- ✅ **Operators**: Arithmetic (+, -, *, /), comparison (==, !=, <, >, <=, >=), logical (and, or, not)
- ✅ **Comments**: Single-line (`//`) and multi-line (`/* */`)

#### String Support (Phase 17) - 100% Complete! 🎉
- ✅ **String Literals**: `"Hello World"`, `"MELP Dünya"` (UTF-8 with emoji support 🚀)
- ✅ **String Variables**: `string name = "Alice"`
- ✅ **String Parameters**: `function greet(string msg)` - Both literals and variables
- ✅ **String Concatenation**: `firstName + " " + lastName` - Multi-operand chaining
- ✅ **String Comparison**: All 6 operators (==, !=, <, >, <=, >=) - Lexicographic ordering
- ✅ **Print Support**: `print("text")`, `print(variable)` - Unified for all types
- ✅ **Dual Backend**: Both LLVM IR and x86-64 assembly support
- ✅ **Compile-time Optimization**: Literal concatenation at parse time
- ✅ **STO Runtime**: Smart Type Optimization for efficient string handling

#### Compiler Infrastructure
- ✅ **Dual Backend**: LLVM IR + x86-64 native assembly
- ✅ **Standard Library**: Print functions, string operations, memory management
- ✅ **STO Runtime**: Smart Type Optimization for strings
- ✅ **Modular Architecture**: 26+ feature modules for maintainability

### 🔜 Coming Soon (Phase 18-20)

- ⏳ **Arrays**: `nums: Array<Int> = [1, 2, 3]` - Dynamic collections (Phase 18)
- ⏳ **Structs**: User-defined types with fields (Phase 20) ⭐ CRITICAL for Stage 1!
- 📝 **Documentation**: Comprehensive tutorials (Phase 19.3 - 95% complete)
- ⏳ **LLVM Optimizations**: -O0, -O1, -O2, -O3 flags (Phase 16)
- ⏳ **Debug Info**: DWARF debug information, GDB support (Phase 16)

### 📊 Overall Progress

```
Stage 0 Bootstrap:  ████████████████████░  98%
Core Language:      ████████████████████  100%
String Support:     ████████████████████  100%  ✅
LLVM Backend:       ███████████████████░   95%
x86-64 Backend:     ███████████████████░   95%
Control Flow:       ████████████████████  100%  ✅
Documentation:      ███████████████████░   95%
Self-Hosting:       ████░░░░░░░░░░░░░░░░   20%
```

**Target:** Stage 0 Complete by Dec 31, 2025 · Stage 1 Self-hosting by Mar 31, 2026

**Recent Achievements:**
- ✅ Phase 17 (String Support) - 100% complete with all operators
- ✅ 8 Tutorials created (English + Turkish)
- ✅ ARCHITECTURE.md comprehensive update (+1300 lines)
- ✅ Self-hosting transition plan documented

---

> **For AI Assistants (YZ Series)**: 
> - 📋 Start with `NEXT_AI_START_HERE.md` for your current mission
> - 📐 Read `ARCHITECTURE.md` for critical architectural rules
> - 📂 All YZ sessions documented in `YZ/` folder
> - 🔧 Follow modular design - NO monolithic code!
> 
> **Current Session**: YZ_73 (Phase 19.4 - README Update & Branch Merge)

---

## 📁 Project Structure

```
MELP/
├── 📄 MELP_VISION.md        # Project philosophy & vision
├── 📄 README.md             # This file - Quick start guide
├── 📄 TODO.md               # Development roadmap & phase tracking
├── 📄 NEXT_AI_START_HERE.md # Entry point for AI assistants
├── 📄 ARCHITECTURE.md       # Critical architectural rules
│
├── 📁 compiler/             # MELP Compiler Implementation
│   ├── stage0/              # Bootstrap compiler (C-based)
│   │   ├── melpc           # Main compiler binary
│   │   ├── functions_compiler  # Functions compiler binary
│   │   └── modules/        # 26+ feature modules
│   │       ├── lexer/      # Tokenization
│   │       ├── parser/     # Syntax analysis
│   │       ├── codegen_llvm/  # LLVM IR generation
│   │       ├── codegen_x86/   # x86-64 assembly generation
│   │       ├── comparison/ # Comparison operators
│   │       ├── print/      # Print statement
│   │       └── ...         # Other feature modules
│   │
│   ├── stage1/             # Self-hosted compiler (planned)
│   └── stage2/             # Optimized self-hosted (future)
│
├── 📁 runtime/             # Runtime Libraries
│   ├── stdlib/            # Standard library
│   │   ├── mlp_stdlib.c  # Core stdlib functions
│   │   └── mlp_stdlib.h  # stdlib header
│   ├── sto/              # STO (Smart Type Optimization)
│   │   ├── mlp_sto.c     # String type implementation
│   │   └── mlp_sto.h     # STO header
│   └── memory/           # Memory management (future)
│
├── 📁 modules/            # MELP Language Modules
│   ├── core/             # Core standard modules
│   ├── lexer_mlp/        # Self-hosted lexer (Phase 13)
│   ├── advanced/         # Advanced features
│   └── experimental/     # Experimental modules
│
├── 📁 examples/          # Example Programs
│   ├── hello/           # Hello World examples
│   ├── basics/          # Basic language features
│   │   ├── test_all_comparisons.mlp
│   │   ├── test_string_concat_*.mlp
│   │   └── ...
│   └── advanced/        # Advanced examples
│
├── 📁 docs/             # English Documentation
│   ├── language/        # Language specification
│   ├── api/            # API reference
│   ├── tutorials/      # 📚 Step-by-step tutorials (NEW!)
│   │   ├── 01_hello_world.md    # Your first MELP program
│   │   ├── 02_variables.md      # Variables and types
│   │   ├── 03_functions.md      # Functions and parameters
│   │   └── 04_strings.md        # String operations
│   ├── LLVM_IR_GUIDE.md
│   └── STRING_SUPPORT.md
│
├── 📁 docs_tr/         # 🇹🇷 Turkish Documentation (Türkçe)
│   ├── language/       # Dil belgeleri
│   └── tutorials/      # 📚 Türkçe eğitim belgeleri (YENİ!)
│       ├── 01_hello_world.md    # İlk MELP programınız
│       ├── 02_variables.md      # Değişkenler ve tipler
│       ├── 03_functions.md      # Fonksiyonlar ve parametreler
│       └── 04_strings.md        # String işlemleri
│
├── 📁 YZ/              # AI Assistant Session Logs
│   ├── YZ_01.md ... YZ_69.md  # Session documentation
│   └── YZ_HISTORY.md   # Complete history
│
├── 📁 tests/           # Test Suite
│   ├── integration/    # Integration tests
│   └── e2e/           # End-to-end tests
│
└── 📁 temp/           # Temporary files & planning docs
    └── selfhosting_geçiş_planlaması.md  # Self-hosting roadmap
```

---

## 🚀 Quick Start

### Prerequisites

- **Linux/macOS**: Native support
- **LLVM 19**: For LLVM backend (`sudo apt install llvm-19`)
- **GCC**: For compilation (`sudo apt install build-essential`)
- **Make**: Build system

### Installation

```bash
# Clone the repository
git clone https://github.com/MELP-Lang/MELP.git
cd MELP

# Build the compiler
make all

# Verify installation
./compiler/stage0/functions_compiler --version
```

### Your First MELP Program

Create `hello.mlp`:

```melp
// Hello World in MELP
print("Merhaba Dünya! 🚀")
print("Hello World!")

// String concatenation
string name = "MELP"
print("Welcome to " + name + "!")
```

Compile and run:

```bash
# Using LLVM backend (recommended)
./compiler/stage0/functions_compiler hello.mlp /tmp/hello.ll --backend=llvm
llc-19 /tmp/hello.ll -o /tmp/hello.s
gcc /tmp/hello.s runtime/stdlib/mlp_stdlib.o runtime/sto/mlp_sto.o -o /tmp/hello
/tmp/hello

# Using x86-64 backend (Linux only)
./compiler/stage0/functions_compiler hello.mlp /tmp/hello.s
gcc /tmp/hello.s runtime/stdlib/mlp_stdlib.o runtime/sto/mlp_sto.o -o /tmp/hello
/tmp/hello
```

### More Examples

**Variables and Functions:**

```melp
function greet(string name) {
    print("Merhaba, " + name + "!")
}

string myName = "MELP"
greet(myName)
```

**Control Flow:**

```melp
numeric count = 1
while count <= 5 {
    print("Count: " + count)
    count = count + 1
}
```

**String Operations:**

```melp
string firstName = "Ada"
string lastName = "Lovelace"
string fullName = firstName + " " + lastName

if fullName == "Ada Lovelace" {
    print("Correct!")
}
```

---

## 📚 Learning Resources

### 📖 Tutorials (English)

Start your MELP journey with our comprehensive tutorials:

1. **[Hello World](docs/tutorials/01_hello_world.md)** - Your first MELP program
   - Writing and running programs
   - Understanding both backends (LLVM & x86-64)
   - Compilation pipeline basics

2. **[Variables & Types](docs/tutorials/02_variables.md)** - Working with data
   - Numeric, String, and Boolean types
   - Variable declaration and assignment
   - Type safety and operations

3. **[Functions](docs/tutorials/03_functions.md)** - Code organization
   - Function definition and calls
   - Parameters and return values
   - Recursion examples

4. **[Strings](docs/tutorials/04_strings.md)** - Text manipulation
   - String literals and variables
   - Concatenation and comparison
   - UTF-8 support and emoji

### 🇹🇷 Türkçe Eğitimler

MELP'i Türkçe öğrenin:

1. **[Merhaba Dünya](docs_tr/tutorials/01_hello_world.md)** - İlk MELP programınız
2. **[Değişkenler ve Tipler](docs_tr/tutorials/02_variables.md)** - Veri tipleri
3. **[Fonksiyonlar](docs_tr/tutorials/03_functions.md)** - Kod organizasyonu
4. **[String'ler](docs_tr/tutorials/04_strings.md)** - Metin işlemleri

### 🏗️ Architecture & Design

- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Compiler architecture deep dive
  - Three-stage vision (Stage 0 → 1 → 2)
  - Dual backend design (LLVM + x86-64)
  - Module structure and code flow examples
  - Critical design decisions explained

- **[MELP_VISION.md](MELP_VISION.md)** - Project philosophy
  - Why MELP exists
  - Design principles
  - Long-term goals

---

## 🎯 Language Features

### ✅ Fully Implemented (Stage 0)

#### Core Types
- **`numeric`**: Integer values (`42`, `-15`)
- **`string`**: Text with UTF-8 support (`"Hello"`, `"Merhaba"`)
- **`boolean`**: True/false values (`true`, `false`)

#### Variables
```melp
numeric age = 25
string name = "Alice"
boolean isActive = true
```

#### Functions
```melp
function add(numeric a, numeric b) returns numeric {
    return a + b
}

numeric result = add(5, 3)  // result = 8
```

#### Operators
- **Arithmetic**: `+` `-` `*` `/` (unary `-`)
- **Comparison**: `==` `!=` `<` `>` `<=` `>=`
- **Logical**: `and` `or` `not`

#### Control Flow
```melp
// If-Else
if x > 10 {
    print("Large")
} else {
    print("Small")
}

// While Loop
while count < 5 {
    count = count + 1
}

// For Loop
for i in 1 to 10 {
    print(i)
}
```

#### String Features
- **Concatenation**: `"Hello" + " " + "World"`
- **Comparison**: All operators work (lexicographic ordering)
- **UTF-8 Support**: `"🚀 MELP Dünya"`
- **Print**: `print("text")` or `print(variable)`

#### Comments
```melp
// Single-line comment

/*
  Multi-line
  comment
*/
```

### 🔜 Coming Soon (Stage 0 → Stage 1)

- **Arrays**: `nums: Array<Int> = [1, 2, 3]` (Phase 18)
- **Structs**: User-defined types (Stage 1)
- **Generics**: Type parameters `<T>` (Stage 1.5)
- **Try-Catch**: Exception handling (Stage 1.5)
- **Modules**: Import/export system (Stage 1)

---

## 🏗️ Build System

### Build Commands

```bash
# Build everything
make all

# Build compiler only
make compiler

# Build runtime libraries
make runtime

# Clean build artifacts
make clean

# Run tests
make test
```

### Compiler Backends

MELP supports two code generation backends:

1. **LLVM Backend** (Recommended)
   - Cross-platform (Linux, macOS, Windows)
   - LLVM optimizations (-O0 to -O3)
   - Portable LLVM IR output
   - Target: x86-64, ARM64, RISC-V

2. **x86-64 Native Backend**
   - Direct assembly generation
   - Linux only (System V ABI)
   - Faster compilation (no LLVM overhead)
   - Educational value (readable assembly)

### Compilation Pipeline

```
┌──────────┐    ┌────────┐    ┌────────┐    ┌──────────┐
│ file.mlp │───▶│ Lexer  │───▶│ Parser │───▶│ CodeGen  │
└──────────┘    └────────┘    └────────┘    └──────────┘
                                                  │
                                    ┌─────────────┴─────────────┐
                                    ▼                           ▼
                              ┌──────────┐              ┌──────────┐
                              │ LLVM IR  │              │ x86-64   │
                              │ (.ll)    │              │ (.s)     │
                              └─────┬────┘              └────┬─────┘
                                    │                        │
                                    ▼                        │
                              ┌──────────┐                   │
                              │   llc    │                   │
                              └─────┬────┘                   │
                                    │                        │
                                    ▼                        │
                              ┌──────────┐                   │
                              │ Assembly │◀──────────────────┘
                              │ (.s)     │
                              └─────┬────┘
                                    │
                                    ▼
                              ┌──────────┐
                              │   gcc    │
                              │  linker  │
                              └─────┬────┘
                                    │
                                    ▼
                              ┌──────────┐
                              │   Binary │
                              │   (exe)  │
                              └──────────┘
```

---

## 📚 Documentation

### For Users
- 📖 **[MELP_VISION.md](MELP_VISION.md)** - Project philosophy and goals
- 📘 **[docs/tutorials/](docs/tutorials/)** - Step-by-step learning guides (coming in Phase 19.3)
- 📗 **[docs/language/](docs/language/)** - Language specification
- 🇹🇷 **[docs_tr/](docs_tr/)** - Türkçe dokümantasyon

### For Developers
- 🏗️ **[ARCHITECTURE.md](ARCHITECTURE.md)** - Compiler architecture and design rules
- 🔧 **[docs/LLVM_IR_GUIDE.md](docs/LLVM_IR_GUIDE.md)** - LLVM backend guide
- 📝 **[TODO.md](TODO.md)** - Development roadmap and phase tracking
- 🤖 **[NEXT_AI_START_HERE.md](NEXT_AI_START_HERE.md)** - AI assistant onboarding

### For AI Assistants (YZ Series)
1. Start: **[NEXT_AI_START_HERE.md](NEXT_AI_START_HERE.md)** - Current status and mission
2. Read: **[ARCHITECTURE.md](ARCHITECTURE.md)** - Critical architectural rules
3. History: **[YZ/](YZ/)** - All previous AI assistant sessions
4. Tasks: **[TODO.md](TODO.md)** - Phase breakdown and task lists

---

## 🧪 Testing

### Test Structure

```
tests/
├── integration/     # Multi-component tests
└── e2e/            # Full compilation tests
```

### Running Tests

```bash
# Run all tests
cd examples/basics
for f in *.mlp; do
    echo "Testing $f..."
    ../../compiler/stage0/functions_compiler "$f" /tmp/test.ll --backend=llvm
    llc-19 /tmp/test.ll -o /tmp/test.s
    gcc /tmp/test.s ../../runtime/stdlib/mlp_stdlib.o ../../runtime/sto/mlp_sto.o -o /tmp/test
    /tmp/test
done
```

### Example Tests
- `test_all_comparisons.mlp` - String comparison operators
- `test_string_concat_*.mlp` - String concatenation
- `test_string_param_*.mlp` - Function parameters
- `test_mixed_types.mlp` - Type coexistence

---

## 🤝 Contributing

MELP is developed through structured AI-assisted sessions (YZ series). Each session focuses on a specific phase or task.

### For AI Contributors

1. **Read**: [NEXT_AI_START_HERE.md](NEXT_AI_START_HERE.md) for your mission
2. **Follow**: [ARCHITECTURE.md](ARCHITECTURE.md) rules (NO monolithic code!)
3. **Branch**: Create `phaseN-description_YZ_XX` branch
4. **Document**: Update TODO.md and NEXT_AI_START_HERE.md
5. **Commit**: Use descriptive commit messages
6. **Handoff**: Create YZ/YZ_XX.md session report

### Current Development Focus

- **Phase 19**: Documentation & Polish (6 YZ sessions)
- **Phase 16**: Advanced LLVM Features (4 YZ sessions)
- **Phase 18**: Array Support (5 YZ sessions)

See [TODO.md](TODO.md) for detailed task breakdown.

---

## 📊 Project Stats

- **Lines of Code**: ~15,000+ (C) + ~2,000 (MELP modules)
- **Modules**: 26+ compiler modules
- **YZ Sessions**: 69 completed (as of Dec 14, 2025)
- **Phases Complete**: 17/19 (Stage 0)
- **Test Coverage**: 30+ example programs
- **Development Time**: ~6 months (Jun 2025 - Dec 2025)

---

## 🗺️ Roadmap

### Stage 0: Bootstrap Compiler (C) - 98% Complete
**Target: December 31, 2025**

- ✅ Phase 1-14: Core language features (Complete)
- ✅ Phase 15: Standard library integration (Complete)
- ✅ Phase 17: String support - 100% Complete! 🎉
- ✅ Phase 19.1-19.3: Documentation (95% - Tutorials complete)
- ⏳ Phase 19.4: README update & branch merge (Current)
- ⏳ Phase 18: Array support (Next - 6-8 hours)
- ⏳ Phase 20: Struct support ⭐ CRITICAL for Stage 1 (4-6 hours)
- ⏳ Phase 16: Advanced LLVM features (Optional)

### Stage 1: Self-Hosted Compiler (MELP) - 20% Complete
**Target: March 31, 2026**

- ✅ Phase 13: Lexer in MELP (complete)
- ⏳ Parser in MELP
- ⏳ CodeGen in MELP
- ⏳ Bootstrap Stage 1 with Stage 0

### Stage 1.5: Advanced Features - Planned
**Target: May 31, 2026**

- Struct types
- Generics `<T>`
- Try-catch exception handling
- Module system
- Advanced optimizations

### Stage 2: Production Ready - Future
**Target: August 31, 2026**

- Multi-language support
- Package manager
- IDE plugins
- Production deployment

See [temp/selfhosting_geçiş_planlaması.md](temp/selfhosting_geçiş_planlaması.md) for detailed self-hosting roadmap.

---

## 📄 License

[License information to be added]

---

## 🙏 Acknowledgments

Developed through AI-assisted methodology (YZ series):
- Structured phase-based development
- Modular architecture
- Comprehensive documentation
- Test-driven approach

**Latest Contributors:**
- YZ_61-65: String support implementation
- YZ_66: Print statement & concatenation
- YZ_69: String comparison bug fix & self-hosting plan
- YZ_71: ARCHITECTURE.md comprehensive update (+1300 lines)
- YZ_72: Tutorial creation (8 tutorials, English + Turkish)
- YZ_73: README update & branch merge (current)

---

## 📞 Contact & Resources

- **GitHub**: [github.com/MELP-Lang/MELP](https://github.com/MELP-Lang/MELP)
- **Documentation**: See `/docs` and `/docs_tr` folders
- **Issues**: Use GitHub Issues for bug reports
- **Development**: Follow [TODO.md](TODO.md) for current tasks

---

**Last Updated**: December 14, 2025 (YZ_73)  
**Version**: Stage 0 Bootstrap (v0.98)  
**Status**: Phase 17 Complete ✅ · Phase 19 Documentation 95% · Phase 19.4 In Progress
- 📋 **[TODO.md](TODO.md)** - Development roadmap: All phases, current tasks, completion status
- 🤖 **[NEXT_AI_START_HERE.md](NEXT_AI_START_HERE.md)** - AI assistant entry point: Current status, next tasks
- 📚 **[docs/language/](docs/language/)** - Language specification: Syntax, types, features
- 🌐 **[docs_tr/language/](docs_tr/language/)** - Turkish documentation: Türkçe belgeler

## 🚀 Quick Start

### Using Functions Compiler (Recommended for Testing)

The `functions_compiler` is a standalone compiler focused on testing MELP's function system with both Assembly and LLVM backends.

```bash
cd compiler/stage0/modules/functions
make

# Compile to Assembly (default)
./functions_compiler -c your_program.mlp output.s
gcc -no-pie output.s -o your_program
./your_program

# Compile to LLVM IR (recommended)
./functions_compiler -c --backend=llvm your_program.mlp output.ll
clang output.ll -o your_program
./your_program
```

**LLVM Backend Advantages:**
- ✅ **Portable**: Cross-platform support (x86-64, ARM, RISC-V)
- ✅ **Readable**: Human-readable intermediate representation
- ✅ **Optimizable**: Industry-standard optimization passes available
- ✅ **Maintainable**: No manual assembly code required

**Supported Features:**
- Function declarations and calls
- Arithmetic operations (+, -, *, /)
- Variable declarations and assignments
- If/else statements
- While loops (simple and nested)
- For loops (range: 1 to N, simple and nested)
- Comparison operators (>, <, ==, !=, >=, <=)
- Boolean operations (AND, OR, true, false)
- Mixed control flow (loops + conditionals)

### Compile and Run

```bash
cd compiler/stage0
make

# Compile a MELP program (full compiler)
./melpc your_program.mlp -o your_program

# Run it
./your_program
```

### Example Programs

**Simple Function (LLVM Backend):**
```mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end function

function main() returns numeric
    return add(15, 27)  -- Returns 42
end function
```

Compile and run:
```bash
cd compiler/stage0/modules/functions
./functions_compiler -c --backend=llvm example.mlp example.ll
clang example.ll -o example
./example
echo "Exit code: $?"  # Should print: Exit code: 42
```

## 📝 Example Program

```mlp
-- MELP Example: For Loop with println
function main() returns numeric
    for i = 0 to 5
        println(i)
    end for
    return 0
end function
```

Output:
```
0
1
2
3
4
5
```

### More Examples

**Variables & Functions:**
```mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end function

function main() returns numeric
    numeric x = 10
    numeric y = 20
    numeric result = add(x, y)
    println(result)  -- Output: 30
    return 0
end function
```

**Collections:**
```mlp
-- Arrays (fixed size)
numeric[] arr = [1, 2, 3, 4, 5]

-- Lists (dynamic)
numeric() list = (10, 20, 30)

-- Tuples (immutable)
numeric<> tuple = <100, 200>
```

**Control Flow:**
```mlp
if x > 10 then
    println("Greater")
else
    println("Smaller or equal")
end if

while x < 100
    x = x + 1
end while
```

---

## 🔧 Build Requirements

- GCC (C compiler)
- NASM (Netwide Assembler)  
- GNU Make
- Linux x86_64

## 📊 Development Statistics

- **Total Modules**: 26 (modular architecture)
- **Lines of Code**: ~20,000+ (Stage 0)
- **YZ Sessions**: 48 completed
- **Test Programs**: 100+ test cases
- **Compilation Speed**: 10-15x faster with caching

## 📚 Documentation

- **Philosophy**: [MELP_VISION.md](MELP_VISION.md) - Why MELP exists
- **Architecture**: [ARCHITECTURE.md](ARCHITECTURE.md) - Critical rules
- **Development**: [TODO.md](TODO.md) - Roadmap & phases
- **AI Handoff**: [NEXT_AI_START_HERE.md](NEXT_AI_START_HERE.md) - Current status
- **Language Docs**: `docs/language/` - Specifications
- **API Reference**: `docs/api/` - Runtime API

## 🤝 Contributing

MELP is developed iteratively with AI assistance (YZ sessions). Each session is documented in `YZ/` folder.

**For AI Assistants:**
1. Read `NEXT_AI_START_HERE.md` first
2. Follow `ARCHITECTURE.md` rules strictly
3. Create YZ branch: `git checkout -b feature_YZ_XX`
4. Document your session in `YZ/YZ_XX.md`
5. Update `NEXT_AI_START_HERE.md` before finishing

## 📄 License

See LICENSE file for details.

---

**Project**: [github.com/MELP-Lang/MELP](https://github.com/MELP-Lang/MELP)  
**Status**: Stage 0 Complete (Phase 11 - 95%)  
**Last Updated**: 12 Aralık 2025  
**Version**: 1.0.0-stage0
1. **TTO Runtime Linking**: Link TTO runtime library for complex expression support
2. **Expression in Conditions**: Use expressions in if/while/for conditions
3. **Variable Usage**: Use variables in expressions (currently only literals work)
4. **Exit Code Generation**: Implement actual break/continue assembly for exit statements
5. **Functions**: Function definitions, parameters, return values

### File Locations
- **Main Compiler**: `compiler/stage0/main.c` (smart parser with all phases)
- **Lexer**: `compiler/stage0/lexer.c` (UTF-8 safe, English keywords)
- **Test Files**: `compiler/stage0/test_*.mlp`, `compiler/stage0/demo_melp.mlp`
- **Wrapper Script**: `./melp` (root directory)

### Build Commands
```bash
cd compiler/stage0
make                    # Compile melpc_26
./melp program.mlp -v   # Compile & run with verbose output
```

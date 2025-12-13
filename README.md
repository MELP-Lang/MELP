# MELP Programming Language

> 🫕 **The Melting Pot of Programming Languages**

Modern, type-safe, and efficient programming language with Smart Type Optimization (STO).

**Core Philosophy:** *Stateless by Default, Stateful by Choice* - MELP is a stateless language where all variables are function-local. For persistent state, explicitly import the `state` module.

📖 **Learn More:** Read [MELP_VISION.md](MELP_VISION.md) for project philosophy and what makes MELP unique.

> **For New AI Assistant (YZ Series)**: Read `/NEXT_AI_START_HERE.md` for current status and your mission. Then read `/ARCHITECTURE.md` for critical rules. All YZ sessions documented in `/YZ/` folder.
> 
> **⚠️ CRITICAL: Read `ARCHITECTURE.md` BEFORE coding!** Previous AI agents created monolithic code by violating modularity rules. Follow the YZ workflow: branch naming, commits, handoff docs.
> 
> **Current Status**: YZ_53 completed Phase 12 Part 4 testing (9/9 tests passed). STO refactoring nearly complete!

---

## 🎉 Current Status: Phase 13.5 LLVM Backend (95% Complete)

**Achievement: LLVM IR Backend Migration Complete! 🚀**

### ✅ Phase 13: Self-Hosting Lexer (100% Complete)
- **Lexer Modules**: 4 MELP modules (lexer_token, lexer_char, lexer_numeric, lexer_identifier)
- **38 Functions**: Fully functional lexer written in MELP
- **56 Token Types**: Complete token recognition
- **Merged to main**: Commit `071d39b`

### ✅ Phase 13.5: LLVM Backend (95% Complete)
- **LLVM IR Output**: `--backend=llvm` flag generates portable LLVM IR
- **8/8 Tests Passing**: All integration tests successful
- **Features Implemented**:
  - Arithmetic operations (+, -, *, /)
  - Function declarations and calls
  - Variable declarations and assignments
  - If/else statements with conditional branches
  - Comparison operators (>, <, ==, !=, >=, <=)
  - Boolean literals (true, false) and operations (AND, OR)
- **Portability**: Cross-platform support (x86-64, ARM, RISC-V via LLVM)
- **Performance**: Comparable to assembly, more readable IR

### 🎯 Latest Sessions:
- ✅ **YZ_57**: Phase 13.5 Parts 1-4 (LLVM backend module, integration, basic tests)
- ✅ **YZ_58**: Phase 13.5 Part 5.1 (Control flow: if/else, assignments)
- ✅ **YZ_59**: Phase 13.5 Parts 5.2-5.4 (Boolean ops, testing, documentation)
- ⏳ **Phase 13.5 Part 5.5-5.7**: Optional stdlib integration, benchmarking, merge prep

### 📊 Completion Status:
```
Core Language:     ████████████████████ 100%
Module System:     ███████████████████░  95%
LLVM Backend:      ███████████████████░  95%
Self-Hosting:      ████░░░░░░░░░░░░░░░░  20%
```

---

## 📁 Project Structure

```
MLP/
├── MELP_VISION.md        # 🫕 Project philosophy & vision
├── README.md             # This file
├── TODO.md               # Development roadmap & tasks
├── NEXT_AI_START_HERE.md # Entry point for AI assistants
├── ARCHITECTURE.md       # Critical architectural rules
│
├── compiler/             # MELP Compiler (Stage 0)
│   └── stage0/          # C-based bootstrap compiler
│       ├── melpc        # Compiler binary
│       └── modules/     # 26 feature modules (modular architecture)
│
├── runtime/             # Runtime libraries
│   ├── sto/            # STO runtime (Smart Type Optimization)
│   ├── memory/         # Memory management
│   └── stdlib/         # Standard library (println, etc.)
│
├── modules/            # Language feature modules
│   ├── core/          # Core modules
│   ├── advanced/      # Advanced features
│   └── experimental/  # Experimental features
│
├── examples/          # Example MELP programs
│   ├── hello/        # Hello World
│   ├── basics/       # Basic examples
│   └── advanced/     # Advanced examples
│
├── YZ/               # YZ session documentation (AI agents)
│   ├── YZ_HISTORY.md # Summary of all sessions
│   ├── YZ_01.md      # TTO Runtime setup
│   ├── ...
│   └── YZ_48.md      # Latest: println() complete
│
├── docs/             # English documentation
│   ├── language/     # Language specification
│   └── api/          # API reference
│
├── docs_tr/          # Turkish documentation
│   └── language/     # Dil belgeleri
│
└── tests/            # Test suite
    ├── unit/         # Unit tests
    ├── integration/  # Integration tests
    └── e2e/          # End-to-end tests
```

## 🎯 Language Features

### ✅ Implemented (Stage 0 Complete!)

**Core Language:**
- Variables & Types: `numeric`, `string`, `boolean`
- Functions: Declaration, calls, return values, recursion
- Arithmetic: `+`, `-`, `*`, `/`, unary minus
- Comparison: `>`, `<`, `>=`, `<=`, `==`, `!=`
- Boolean Logic: `and`, `or`, `not`
- Control Flow: `if/else`, `while`, `for`, `exit`, `continue`

**Collections:**
- Arrays: `[1, 2, 3]` - fixed size, fast access
- Lists: `(1, 2, 3)` - dynamic, mutable
- Tuples: `<1, 2, 3>` - immutable, lightweight

**String Operations:**
- Concatenation, comparison
- Methods: `length()`, `substring()`, `indexOf()`, `toUpperCase()`, `toLowerCase()`

**Advanced:**
- File I/O: `read_file()`, `write_file()`
- Module System: `import` statements, circular detection
- State Management: Optional `state` module
- Incremental Compilation: Module caching (10-15x speedup)
- Error Recovery: Colored messages, typo detection

**I/O:**
- `println(value)` - Print with newline
- `print(value)` - Print without newline
- `input()` - Read user input

### 🚧 In Progress

**Phase 12: TTO→STO Refactoring** (5-7 hours)
- Rename TTO (Transparent Type Optimization) → STO (Smart Type Optimization)
- Update all documentation and code
- Ensure consistency across 100+ files

### 🔮 Planned (Stage 1 - Self-Hosting)

**Core Missing Features:**
- Struct/Record Types (4-5h)
- Try-Catch Exception Handling (3-4h)
- Type Inference (`var` keyword) (2-3h)
- Lambda/Closure Integration (2-3h)
- Enum Types (1-2h)
- Generics (3-4h)

**Self-Hosting:**
- Lexer in MELP (8-10h)
- Parser in MELP (15-20h)
- Codegen in MELP (15-20h)

## 📚 Key Resources

- 📖 **[MELP_VISION.md](MELP_VISION.md)** - Project philosophy: Why MELP exists, what makes it unique
- 🏗️ **[ARCHITECTURE.md](ARCHITECTURE.md)** - Critical rules: Modular architecture, STO system
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
- Comparison operators (>, <, ==, !=, >=, <=)
- Boolean operations (AND, OR, true, false)

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

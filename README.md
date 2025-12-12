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

## 🎉 Current Status: Stage 0 Complete! (Phase 11 - 95%)

**Achievement: Production-Ready Core Language! 🎊**

### ✅ Core Features Complete:
- **Variables & Types**: numeric, string, boolean with STO optimization
- **Functions**: Declaration, calls, return values, recursion
- **Control Flow**: if/else, while, for loops, exit statements
- **Collections**: Arrays `[]`, Lists `()`, Tuples `<>`
- **String Operations**: concat, compare, methods (length, substring, indexOf, etc.)
- **File I/O**: read_file(), write_file(), file operations
- **State Management**: Optional state module for persistence
- **Module System**: import statements, circular detection, caching
- **Incremental Compilation**: 10-15x speedup with module caching
- **Error Handling**: Colored messages, typo detection, suggestions

### 🎯 Latest Sessions:
- ✅ **YZ_46**: Self-hosting Part 6.1 & 6.2 (Token + Char utils)
- ✅ **YZ_47**: println() parser implementation  
- ✅ **YZ_48**: println() codegen complete - works in for loops!
- ✅ **YZ_49-51**: Phase 12 Parts 1-3 (TTO→STO refactoring: docs, runtime, compiler)
- ✅ **YZ_53**: Phase 12 Part 4 (Comprehensive testing - 9/9 PASS, zero regressions)
- ⏳ **Phase 12 Part 5**: Final cleanup & migration guide (30 min)

### 📊 Completion Status:
```
Core Language:     ████████████████████ 100%
Module System:     ███████████████████░  95%
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

### Compile and Run

```bash
cd compiler/stage0
make

# Compile a MELP program
./melpc your_program.mlp -o your_program

# Run it
./your_program
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

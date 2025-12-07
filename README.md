# MELP Programming Language

Modern, type-safe, and efficient programming language with transparent type optimization (TTO).

> **For New AI Assistant**: Read "Development Notes" section at bottom for current implementation status, file locations, and next steps. All features listed as ✅ in roadmap are working. Project uses VB.NET-style syntax with `exit` instead of `break/continue`.
> 
> **⚠️ CRITICAL: Read `ARCHITECTURE.md` BEFORE coding!** Previous AI agents created a monolithic 736-line main.c by violating modularity rules. Architecture checks are now enforced by Makefile.
> 
> **If stuck or unclear about language rules**: Check `temp/kurallar_kitabı.md` (rules book) for detailed language specifications and design decisions.

## ✨ Recent Updates (7 Aralık 2025)

**Phase 3.5 In Progress! 🚀**
- ✅ **Expression Parsing**: Arithmetic, comparison, logical operators fully parsed
- ✅ **Variable Initialization**: Numeric literals work (`numeric x = 42`)
- 🚧 **Complex Expressions**: Parser works, TTO runtime linking needed for execution
- 🚧 **If Conditions**: Need expression integration

**Phase 3 Complete! 🎉**
- ✅ **Control Flow**: if/then/else, while/do, for/to loops
- ✅ **Variable Declarations**: numeric, string, boolean types
- ✅ **Print Statements**: Full support with string literals

**Previous Updates:**
- ✅ **Full English Conversion**: Converted from Turkish (`yazdir`) to English (`print`)
- ✅ **Token System Cleanup**: `TOKEN_TEXT` → `TOKEN_STRING_TYPE` for type keywords
- ✅ **Keyword Consistency**: Removed `text` alias, unified on `string` keyword
- ✅ **Smart Parser**: Strict syntax validation rejecting invalid top-level constructs
- ✅ **UTF-8 Safe Lexer**: Fixed Turkish character infinite loop bug
- ✅ **Single Command Compilation**: `melp hello.mlp` wrapper script

## 📁 Project Structure

```
MLP/
├── compiler/              # MELP Compiler (Stage 0, 1, 2...)
│   ├── stage0/           # Minimal working compiler (26 modules)
│   │   ├── melpc_26      # Compiler binary
│   │   ├── lexer.c/h     # UTF-8 safe lexer with English keywords
│   │   ├── main.c        # Smart parser with strict validation
│   │   └── modules/      # 26 feature modules
│   ├── stage1/           # Variables and arithmetic
│   ├── stage2/           # Control flow and functions
│   └── docs/             # Compiler documentation
│
├── melp                  # 🆕 Single-command wrapper script
│                         # Usage: melp program.mlp [-v] [--no-run]
│
├── runtime/              # Runtime libraries
│   ├── tto/             # Transparent Type Optimization runtime
│   ├── memory/          # Memory management
│   └── stdlib/          # Standard library
│
├── modules/             # Language modules
│   ├── core/            # Core modules
│   ├── advanced/        # Advanced features
│   └── experimental/    # Experimental features
│
├── examples/            # Example MELP programs
│   ├── hello/          # Hello World variations
│   ├── basics/         # Basic examples
│   └── advanced/       # Advanced examples
│
├── tests/              # Test files
│   ├── unit/           # Unit tests
│   ├── integration/    # Integration tests
│   └── e2e/            # End-to-end tests
│
└── docs/               # Documentation
    ├── language/       # Language specification
    ├── api/            # API reference
    └── tutorials/      # Tutorials
```

## 🎯 Language Features

### Current (Stage 0)
- ✅ **English Keywords**: `print`, `numeric`, `string`, `boolean`, `if`, `while`, `function`
- ✅ **Print Statement**: `print("Hello, World!")`
- ✅ **VB.NET Style Comments**: `-- This is a comment`
- ✅ **Smart Parser**: Rejects invalid syntax at parse time
- ✅ **UTF-8 Safe**: Handles international characters correctly
- ✅ **26 Feature Modules**: Variables, functions, async, memory, etc.

### Type System
- **Type Keywords**: 
  - `numeric` - Integer and floating-point numbers
  - `string` - UTF-8 strings (not `text` - removed for consistency)
  - `boolean` - True/false values
- **TOKEN_STRING_TYPE**: Internal token for type keywords
- **TOKEN_STRING**: Internal token for string literals

### Planned Features
- Variables and expressions
- Control flow (if/while/for)
- Functions and lambdas
- Struct types
- Async/await
- Memory management
- Package system

## 📝 Development Roadmap

### Phase 1: Core Language ✅ COMPLETE
- [x] Clean project structure
- [x] UTF-8 safe lexer with English keywords
- [x] Smart parser with strict validation
- [x] Print statement implementation
- [x] Comment support (`--` VB.NET style)
- [x] Single-command `melp` wrapper
- [x] Token system cleanup (TOKEN_STRING_TYPE)
- [x] Remove Turkish keywords and aliases

### Phase 2: Variables & Declarations ✅ COMPLETE
- [x] Type keywords: `numeric`, `string`, `boolean`
- [x] Variable declarations with initialization
- [x] Basic value parsing (literals and identifiers)
- [x] Assembly generation (.data and .bss sections)

### Phase 3: Control Flow ✅ COMPLETE
- [x] if/then/else statements
- [x] while/do loops
- [x] for/to loops
- [x] Exit statements (exit for, exit while, exit if) - parsing complete
- [ ] Exit statements - code generation (Phase 3.5)
- [ ] Match expressions

**Note**: MELP uses VB.NET-style `exit` instead of `break/continue`:
- `exit for` - Exit from for loop (early termination)
- `exit while` - Exit from while loop
- `exit if` - Exit from if block (guard clause pattern, replaces continue)
- `exit function` - Return from function (Phase 4)

**Guard Clause Pattern Example**:
```melp
for i = 1 to 100
    if invalidCondition then
        exit if  -- Skip this iteration (cleaner than else blocks)
    end if
    
    -- Main logic continues here without nesting
    process(i)
end
```

Benefits: No `continue` keyword needed, cleaner code without nested else blocks

### Phase 3.5: Expressions & Operators 🚧 IN PROGRESS
- [x] Arithmetic expressions (+, -, *, /, mod, ^) - Parser complete
- [x] Comparison operators (==, !=, <, >, <=, >=) - Parser complete
- [x] Logical operators (and, or, not) - Parser complete
- [x] String concatenation (+) - Parser complete
- [x] Operator precedence - Implemented
- [x] Parenthesized expressions - Supported
- [x] Variable initialization with literals - Working
- [ ] Complex expression code generation (needs TTO runtime linking)
- [ ] Expression in if/while conditions
- [ ] Variable usage in expressions

### Phase 4: Functions ⏳ PLANNED
- [ ] Function definitions
- [ ] Parameters and return types
- [ ] Lambda expressions
- [ ] Higher-order functions (map, filter, reduce)
- [ ] Closures

### Phase 5: Advanced Features ⏳ PLANNED
- [ ] Struct types
- [ ] Arrays and collections
- [ ] Async/await
- [ ] Memory management (ownership system)
- [ ] Package management
- [ ] Standard library

## 🚀 Quick Start

### Option 1: Using `melp` wrapper (Recommended)

```bash
# Simple compilation and execution
./melp examples/hello/hello_world.mlp

# Verbose mode (shows compilation steps)
./melp examples/hello/hello_world.mlp -v

# Compile only (don't run)
./melp program.mlp --no-run

# Custom output name
./melp program.mlp -o myprogram
```

### Option 2: Manual compilation

```bash
# Compile MELP compiler
cd compiler/stage0
make

# Compile a MELP program
./melpc_26 hello_world.mlp hello_world.s

# Assemble and link
nasm -f elf64 hello_world.s -o hello_world.o
ld hello_world.o -o hello_world

# Run
./hello_world
```

## 📝 Example Program

```melp
-- MELP v0.3.0 Example
print("Hello, World!")

-- Variables with all types
numeric age = 25
string name = "MELP"
boolean active = true

-- Control flow
if active then
    print("System is active!")
end

while active do
    print("Processing...")
end

for i = 1 to 5
    print("Iteration")
end

print("Program complete!")
```

## 🔧 Build Requirements

- GCC (C compiler)
- NASM (Netwide Assembler)
- GNU Make
- Linux x86_64 (for current runtime)

## 📊 Project Status

- **Stage 0**: ✅ 100% Complete (26 modules, working compiler)
- **Architecture**: ⚠️ **NEEDS REFACTORING** (main.c: 736/300 lines)
- **Language**: ✅ 100% English (Turkish keywords removed)
- **Token System**: ✅ 100% Consistent (TOKEN_STRING_TYPE cleanup done)
- **Parser**: ✅ Smart validation implemented
- **Lexer**: ✅ UTF-8 safe with proper error handling
- **Build System**: ✅ Makefile + melp wrapper + **architecture enforcement**
- **Documentation**: 🚧 In progress
- **Test Suite**: ⏳ Planned

### ⚠️ Architecture Issues (MUST FIX)

**Problem:** Previous AI agent created monolithic main.c (736 lines)

**Enforcement Active:**
- ✅ Makefile checks: `make check-architecture`
- ✅ Pre-commit hook installed
- ✅ ARCHITECTURE.md rules document
- ❌ main.c: 736/300 lines (VIOLATION)
- ❌ Direct module imports in main.c (VIOLATION)

**Fix Required:** Extract inline parsing to pipeline architecture

### Compiler Statistics
- **Total Modules**: 26
- **Lines of Code**: ~15,000+ (stage0)
- **Keywords**: 20+ (all English)
- **Token Types**: 50+
- **Binary Size**: ~389 KB (melpc_26)

---

**Last Updated**: 7 Aralık 2025  
**Status**: Stage 0 Complete - Phase 3 Complete  
**Version**: 0.3.0-alpha

## 🔧 Development Notes

### Current Implementation Status
- **Compiler**: `compiler/stage0/melpc_26` (26 modules, ~389 KB)
- **Wrapper**: `./melp program.mlp` for single-command compilation
- **Working Features**:
  - ✅ Variables: `numeric x = 42`, `string s = "text"`, `boolean b = true`
  - ✅ Print: `print("Hello")` 
  - ✅ Control Flow: `if/then/else/end`, `while/do/end`, `for i = 1 to 10`
  - ✅ Exit Statements: `exit for`, `exit while`, `exit if` (parsing only, codegen pending)
  - ✅ Comments: `-- comment style`
  - ✅ Expression Parsing: All operators parsed, literal initialization works
  - 🚧 Complex Expressions: Needs TTO runtime linking for full support

### Implementation Details
- **Lexer**: `lexer.c/h` - TOKEN_STRING_TYPE for types, TOKEN_STRING for literals
- **Parser**: `main.c` - Inline simple parsing for variables and control flow
- **Assembly**: `.data` section for strings, `.bss` for variables
- **No `continue` keyword**: Use `exit if` pattern instead (VB.NET style)
- **No `text` keyword**: Removed, only `string` remains

### Next Steps (Phase 3.5 & 4)
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

# Stage1 Bridge Compiler (C Implementation)

**Purpose:** Minimal MLP → C compiler to bridge Stage0 and Stage2

## Build

```bash
cd MELP/C/stage1
make
```

## Usage

```bash
./melpc_stage1 input.mlp output.c
```

## Features

- ✅ Functions (definition, call, return)
- ✅ Structs (simple, flat)
- ✅ Variables (Numeric, String, Boolean)
- ✅ Control flow (if/else, while, return)
- ✅ Operators (arithmetic, comparison, logical)
- ✅ list() operations (basic support)
- ✅ Import statements (skip for now)

## Architecture

- `lexer.c/h` - Tokenization
- `parser.c/h` - AST construction  
- `codegen.c/h` - C code generation
- `main.c` - Entry point

## Status

Phase 5.5 - Bridge compiler implementation in progress

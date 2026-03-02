# MLP Stage 1

> **Stage 1:** Programs written in `.mlp`, compiled by the Stage 0 toolchain.

---

## Stage Terminology

```
Stage 0  ─  C++ compiler that compiles .mlp files
             /home/pardus/projeler/MLP/LLVM/stage0/v2/
             └── compiler/          ← C++ source (lexer, parser, codegen, OK layer)
             └── build/…/melp_compiler  ← the tool we USE here

Stage 1  ─  .mlp programs (this repo)
             /home/pardus/projeler/MLP/LLVM/stage1/     ← YOU ARE HERE
             └── src/compiler/      ← MLP compiler written in .mlp  (future)
             └── src/stdlib/        ← MLP standard library in .mlp  (future)
             └── src/apps/          ← Sample programs in .mlp

Stage 2  ─  .mlp compiler written in .mlp (self-hosting)
             Not started yet.
```

The Stage 0 `compiler/` directory contains two sub-layers (`stage0/` = frontend, `stage1/` = backend/orchestrator) — both are **C++ code**, both are Stage 0. The naming inside that repo is an internal implementation detail, not the public stage numbering.

---

## Quick Start

**Prerequisite:** Stage 0 compiler built.

```bash
# Build all
make

# Run hello world
make run TARGET=src/apps/hello.mlp

# Run all tests
make test
```

**Compiler path** (set in `Makefile`):
```
/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler
```

---

## Directory Structure

```
stage1/
├── Makefile             ← build system
├── src/
│   ├── apps/            ← standalone .mlp programs
│   │   └── hello.mlp
│   ├── compiler/        ← MLP compiler written in .mlp (self-hosting goal)
│   │   └── lexer.mlp    ← tokenizer (WIP — has known bugs)
│   ├── stdlib/          ← standard library in .mlp (planned)
│   └── ok/              ← OK layer in .mlp (planned)
└── build/               ← compiled LLVM IR output (.ll files)
```

---

## Known Issues in `src/compiler/lexer.mlp`

- `test` is a reserved word in the MLP parser — any function named `test` fails to parse.
  **Fix:** rename to `tokenize_test` or similar.
- `else then` is not valid syntax — use `else` without `then`.
  **Fix:** replace all `else then` with `else`.
- `-> numeric` (arrow return type) works in the compiler but `as numeric` is the canonical form.
- Deep nested `if/else` chains (40+ levels) cause stack overflow in `lli`.
  **Fix:** restructure using early returns or data tables.
- Segfault in `lli` when running the full lexer — root cause: `strcmp` segfault from deep call stack.

---

## Stage 1 Roadmap

| Phase | Goal | Status |
|-------|------|--------|
| 1 | `hello.mlp` runs | ✅ Done |
| 2 | `lexer.mlp` — tokenizer in MLP | 🔧 WIP (segfault) |
| 3 | `parser.mlp` — parser in MLP | 🔴 Not started |
| 4 | `stdlib/` — string, math, io modules | 🔴 Not started |
| 5 | `ok/` — OK layer in MLP | 🔴 Not started |
| 6 | Self-hosting: compile stage1 with stage1 | 🔴 Future |

---

## Compile a Single File

```bash
MELP=/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler

# Compile to LLVM IR
$MELP src/apps/hello.mlp -o build/hello.ll

# Run
lli build/hello.ll

# Or native binary
llc -filetype=obj build/hello.ll -o build/hello.o
clang build/hello.o -o build/hello -no-pie
./build/hello
```

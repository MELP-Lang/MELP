# MLP Stage 1

> **Stage 1:** Programs written in `.mlp`, compiled by the Stage 0 toolchain.
> This is what we present to the community as "the MELP programming language".

---

## Stage Terminology

```
Stage 0  ─  C++ compiler that compiles .mlp files  (FROZEN)
             /home/pardus/projeler/MLP/LLVM/stage0/v2/
             └── compiler/          ← C++ source (lexer, parser, codegen, OK layer)
             └── build/…/melp_compiler  ← the tool we USE here

Stage 1  ─  .mlp programs (this repo)              (ACTIVE)
             /home/pardus/projeler/MLP/LLVM/stage1/     ← YOU ARE HERE
             └── src/normalizer/    ← keyword normalizer written in .mlp
             └── src/stdlib/        ← standard library in .mlp
             └── src/compiler/      ← tokenizer/lexer in .mlp (WIP)
             └── src/apps/          ← sample programs

Stage 2  ─  .mlp compiler written in .mlp (self-hosting)
             Not started yet.  Depends on Stage 1 being complete.
```

The Stage 0 `compiler/` directory contains two sub-layers (`stage0/` = frontend,
`stage1/` = backend/orchestrator) — both are **C++ code**, both are Stage 0.
The naming inside that repo is an internal implementation detail.

---

## Architecture

```
┌──────────────────────────────────────────────────┐
│  User Layer (Stage 1)                            │
│                                                  │
│  program.mlp      ← English keywords (default)  │
│  program.tr.mlp   ← Turkish keywords            │
│  program.ru.mlp   ← Russian keywords            │
│                                                  │
│  Normalizer: src/normalizer/normalizer.mlp       │
│  Runs automatically for .tr.mlp / .ru.mlp files  │
└───────────────────┬──────────────────────────────┘
                    │ canonical .mlp
┌───────────────────▼──────────────────────────────┐
│  Stage 0 Compiler (FROZEN)                       │
│  Speaks only canonical .mlp                      │
└──────────────────────────────────────────────────┘
```

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
├── STAGE1_PLAN.md       ← development roadmap
├── Makefile             ← build system
├── src/
│   ├── normalizer/      ← Phase 1: keyword/syntax normalizer (ACTIVE)
│   │   ├── normalizer.mlp    ← core logic
│   │   ├── lang_turkish.mlp  ← Turkish keyword map
│   │   ├── lang_russian.mlp  ← Russian keyword map
│   │   └── ornek.tr.mlp      ← example Turkish program
│   ├── stdlib/          ← Phase 2: standard library (planned)
│   ├── compiler/        ← Phase 3: lexer.mlp (WIP, has bugs)
│   │   └── lexer.mlp
│   └── apps/            ← Phase 4: sample programs
│       └── hello.mlp
└── build/               ← compiled LLVM IR output (.ll files)
```

---

## Roadmap

| Phase | Goal | Status |
|-------|------|--------|
| 0 | `hello.mlp` runs | ✅ Done |
| 1 | `normalizer.mlp` — keyword normalizer in .mlp | 🔧 WIP |
| 2 | `stdlib/` — math, string, io, test modules | 🔴 Not started |
| 3 | `lexer.mlp` fix — resolve 3 known bugs | 🔴 Not started |
| 4 | `apps/` — fibonacci, calculator, wordcount | 🔴 Not started |
| 5 | Stage 2 bootstrap — new compiler in .mlp | 🔴 Future |

For the full plan see [STAGE1_PLAN.md](STAGE1_PLAN.md).

---

## Known Issues in `src/compiler/lexer.mlp`

- `test` is a reserved word in the MLP parser — any function named `test` fails to parse.
  **Fix:** rename to `tokenize_test` or similar.
- `else then` is not valid syntax — use `else` without `then`.
  **Fix:** replace all `else then` with `else`.
- `-> numeric` (arrow return type) works but `as numeric` is the canonical form.
- Deep nested `if/else` chains (40+ levels) cause stack overflow in `lli`.
  **Fix:** restructure using early returns or lookup tables.

---

## Stage 0 Bugs Affecting Stage 1

These are frozen in Stage 0 and must be worked around:

| ID | Bug | Workaround |
|----|-----|------------|
| B1 | `struct { field as string }` → LLVM error | Use parallel arrays instead of struct-of-strings |
| B2 | `MyType[] arr = [a; b]` → Parser error | Use numeric arrays only |
| B3 | Function returning struct → garbage value | Avoid returning structs |

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

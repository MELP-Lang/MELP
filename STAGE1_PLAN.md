# Stage 1 Plan — MELP Programs in .mlp

> **Stage 1** aims to write real, useful programs in `.mlp` using the Stage 0 compiler as a tool.
> Stage 1 is what we present to the community as "the MELP programming language".

---

## Vision

```
User writes .mlp  →  Stage 0 compiles  →  binary
```

The user never sees `pmlp`, `FUNC_DECL`, or internal Stage 0 details.
Stage 0 is a **frozen tool** — Stage 1 is what the world sees.

---

## Architecture

```
┌─────────────────────────────────────────────────┐
│  User Layer (Stage 1)                           │
│                                                 │
│  program.mlp      ← English keywords (default) │
│  program.tr.mlp   ← Turkish keywords           │
│  program.ru.mlp   ← Russian keywords           │
│                                                 │
│  Normalizer (transparent, not opt-in)           │
│  src/normalizer/normalizer.mlp                  │
└──────────────────┬──────────────────────────────┘
                   │ canonical .mlp
┌──────────────────▼──────────────────────────────┐
│  Stage 0 Compiler (FROZEN)                      │
│  /home/pardus/projeler/MLP/LLVM/stage0/v2/      │
│  Speaks only canonical .mlp                     │
└─────────────────────────────────────────────────┘
```

**Key rule:** Stage 0 is never modified for Stage 1 needs.
If Stage 0 has a bug blocking Stage 1, document it and work around it.

---

## Priority Order

### Phase 1 — Normalizer (FIRST PRIORITY)
**Why first:** The normalizer defines what "writing in MELP" means.
Turkish/Russian developers must be able to write `.tr.mlp` files natively.

```
src/normalizer/
├── normalizer.mlp      ← core: language detect, translate, validate
├── lang_turkish.mlp    ← Turkish keyword map
├── lang_russian.mlp    ← Russian keyword map
└── syntax_c.mlp        ← C-style brace syntax support
```

The normalizer reads a source file, detects language/syntax, translates
keywords to canonical English `.mlp`, then passes to Stage 0 for compilation.

**Convention:**
- `.mlp`    → canonical English, skip normalizer
- `.tr.mlp` → Turkish keywords, run normalizer first
- `.ru.mlp` → Russian keywords, run normalizer first

### Phase 2 — Stdlib
Standard library modules in `.mlp`:

```
src/stdlib/
├── math.mlp        ← pow, gcd, lcm, clamp
├── string.mlp      ← format, repeat, pad, reverse
├── array.mlp       ← sort, filter, map, reduce
├── io.mlp          ← file read/write helpers
└── test.mlp        ← test framework (describe, assert_eq)
```

### Phase 3 — Fix lexer.mlp
`src/compiler/lexer.mlp` already exists but has 3 bugs:

| Bug | Fix |
|-----|-----|
| Functions named `test` fail (reserved keyword) | Rename to `tokenize_test` |
| `else then` is not valid syntax | Change to `else` |
| 40-level nested if/else → stack overflow in lli | Refactor with early returns |

### Phase 4 — Sample Applications

```
src/apps/
├── hello.mlp        ← ✅ done
├── fibonacci.mlp
├── fizzbuzz.mlp
├── calculator.mlp   ← expression REPL
├── wordcount.mlp    ← reads a file, counts words
└── json_parser.mlp  ← minimal JSON (prepares for Stage 2)
```

### Phase 5 — Stage 2 Bootstrap (distant future)
Only after Phases 1-4 are complete:
Write a new MLP compiler in `.mlp` (Lexer → Parser → Codegen).
This is the selfhosting milestone.

---

## Directory Structure

```
stage1/
├── STAGE1_PLAN.md       ← this file
├── Makefile             ← build system
├── src/
│   ├── normalizer/      ← Phase 1: keyword/syntax normalizer
│   ├── stdlib/          ← Phase 2: standard library
│   ├── compiler/        ← Phase 3: lexer.mlp (future: full compiler)
│   └── apps/            ← Phase 4: sample programs
└── build/               ← compiled LLVM IR (.ll files)
```

---

## Stage 0 Known Bugs Affecting Stage 1

These bugs exist in Stage 0 (frozen) and may affect Stage 1 development:

| ID | Bug | Workaround |
|----|-----|------------|
| B1 | `struct { value as string }` → LLVM Verification Error | Avoid string fields in structs |
| B2 | `Token[] tokens = [t1; t2]` → Parser error | Use numeric arrays only |
| B3 | Function returning struct → garbage value | Avoid struct return values |
| B4 | `parsePrimary` sınırı — 3 durum: (1) `this.a * this.b` binary expr, (2) `str(p.x)` builtin arg, (3) `rect.nested.x = 0` 2-level member assign | (1,2) local var kullan; (3) nested struct'ı flat tut |
| B5 | `read_file()` on nonexistent file → segfault when compared with `""` | Do not test missing files |
| B6 | `operator+` syntax → parse error | Use named method (e.g. `Vector.add(other)`) |

> If these bugs are fixed in a future Stage 0 patch (v2.1), Stage 1 can be updated accordingly.

---

## Binary I/O & FFI Findings

Discovered during `examples/tools/` development (BMP/JPEG/PNG parsers):

### clang -O2 breaks fgetc ordering
`clang -O2` treats `fgetc()` as a pure function and reorders calls — byte reads come out wrong.
**Rule:** Always use `clang -O0` when MELP code does binary I/O via `fgetc`.

### FFI with external shared libraries
For complex C libraries (libjpeg, libpng), direct FFI to the library API is not feasible
because MELP's FFI only supports scalar parameters (no struct pointers).
**Solution:** Write a thin C wrapper (`melp_image.c`) that exposes a scalar API, compile
to a `.so`, then link with a two-step build:
```bash
# Step 1: IR → object file
clang -O0 -c -x ir out.ll -o out.o
# Step 2: link with wrapper .so
clang -O0 out.o -o program melp_image.so -ljpeg -lpng
# Run with library path
LD_LIBRARY_PATH=/path/to/tools ./program
```
Standard libc functions (`fopen`, `fgetc`, `fclose`) work with the single-step build
because clang links libc automatically.

### FFI parameter types
- `external function fopen(s as string; m as string) as numeric` — string params work
- All numeric params/returns map to `i64` in LLVM IR (64-bit integer)
- File pointers (`FILE*`) fit in `numeric` on 64-bit systems (pointer = 8 bytes)

---

## Build System Convention

```bash
# Compile a canonical .mlp file
make build FILE=src/apps/hello.mlp

# Compile a Turkish .tr.mlp file (normalizer runs first)
make build FILE=src/apps/selam.tr.mlp

# Run all tests
make test

# Run a specific program
make run FILE=src/apps/hello.mlp
```

---

## What Stage 1 Is NOT

- Stage 1 is **not** a rewrite of Stage 0 in `.mlp` — that is Stage 2.
- Stage 1 is **not** a new compiler — Stage 0 is the compiler.
- Stage 1 is **not** a fork of Stage 0 — it depends on Stage 0 as an external tool.

Stage 1 is the **language ecosystem**: programs, libraries, and tools
written in `.mlp` that demonstrate the language is production-ready.

---

*Created: 2026-03-02*
*Branch: main*

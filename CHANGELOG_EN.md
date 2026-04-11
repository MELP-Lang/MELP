# Changelog

This project follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) format.  
Versioning follows [Semantic Versioning](https://semver.org/).

> **Türkçe sürüm:** [CHANGELOG.md](CHANGELOG.md)

---

## [0.2.0-alpha] - 2026-04-11

### Major Milestone

**MELP is now self-hosting — the compiler can compile itself.**

The MELP compiler is written in `.mlp` and is capable of compiling its own
source code. This is the core milestone the project has been working toward
since its inception.

### Technical Details

- **Gen2 = Gen3 differential test:** The `.mlp`-written compiler (Gen2) compiles
  itself to produce Gen3; zero semantic difference observed except the source
  comment line.
- **Patch script removed:** The Python patch script (`fix_ir.py`) used during
  bootstrap no longer plays any role in the pipeline — zero active passes.
- **48/48 golden tests:** All pass without any patching.
- **30+ compiler modules:** The entire compiler is written in `.mlp`.

### Added (STAGE2 compiler modules)

- `compiler/lexer.mlp` — full tokenizer
- `compiler/dispatch_words.mlp` → split into 8 sub-modules
- `compiler/control_parser.mlp` → split into 4 sub-modules
- 30+ `.mlp` compiler modules, all self-hosted
- 48 golden tests, all PASS

---

## [0.1.0-alfa] - 2026-03-11

### First Alpha Release

This is the first public alpha release of the MELP programming language.

### Added
- `melp_compiler` — LLVM-based full compiler (Linux x86_64)
  - Variables: `numeric`, `string`, `boolean`, automatic type inference (`var`, `auto`)
  - Control flow: `if/else if/else/end if`, `while/end while`, `for/end for`
  - Functions: parametrized, typed return, recursive
  - Data structures: `struct`, `enum` (basic)
  - Collections: `numeric[]`, `string[]` (basic), `hashmap`
  - Standard library: `print()`, `str()`, `len()`, `read_line()`, math functions
  - Module system: `import "file.mlp"`
  - Multi-lingual syntax: Turkish (`.tr.mlp`), Russian (`.ru.mlp`) keywords
- `mlp_runtime.bc` — LLVM bitcode runtime library
- 40+ example programs (`examples/` directory)
- Turkish and English documentation (`belgeler/`, `docs_EN/`, `docs_TR/`)

### Known Limitations
- Only Linux x86_64 is supported (macOS/Windows: in Beta)
- Closure outer-scope capture is not yet complete
- Generic types (`list<T>`) are partially implemented
- String interpolation (`${x}`) is not yet available — use `str(x)`
- `switch/case` compiles but produces no output (known bug)
- `Result<T>` generic return type not yet supported

### Requirements
- LLVM 14 (`clang-14`, `lli-14`)
- Linux x86_64

---

## Upcoming Releases

### [0.2.0-beta] - Planned
- macOS and Windows support
- String interpolation (`${expr}`)
- `switch/case` fix
- `Result<T>` and `?` operator
- Closure outer-scope capture
- CI/CD automated test report

### [1.0.0] - Planned (post-Stage2)
- Compiler written in MELP itself (self-hosting)
- Full platform support
- Package manager

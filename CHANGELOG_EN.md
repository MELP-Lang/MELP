# Changelog

This project follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) format.  
Versioning follows [Semantic Versioning](https://semver.org/).

> **Türkçe sürüm:** [CHANGELOG.md](CHANGELOG.md)

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

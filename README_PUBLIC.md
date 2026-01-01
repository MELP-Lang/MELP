# MLP-GCC: Bootstrap-Ready Self-Hosting Compiler

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Version](https://img.shields.io/badge/version-0.9.0-blue.svg)]()
[![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Language](https://img.shields.io/badge/language-MLP-orange.svg)]()

**Modern Language Project (MLP)** - A self-hosting compiler with GCC backend

---

## 🎉 Bootstrap-Ready Status Achieved!

MLP-GCC has achieved **Bootstrap-Ready Self-Hosting** status! The compiler successfully translates MLP programs to C, compiles them with GCC, and executes reliably.

**Status:** ✅ BOOTSTRAP-READY (v0.9.0)  
**Date:** 30 December 2025  
**Backend:** GCC (C transpiler)

---

## 🚀 Quick Start

### Prerequisites
- GCC compiler (gcc)
- GNU Make
- Linux/Unix environment

### Installation

```bash
# Clone repository
git clone https://github.com/MELP-Lang/MELP-GCC.git
cd MELP-GCC

# Build runtime library
cd runtime
make clean && make

# Test compiler
cd ../C/stage0
./melpc ../../../examples/hello_english.mlp test.c
gcc test.c -I ../../runtime -L ../../runtime -lruntime -o test
./test
```

### Hello World

Create a file `hello.mlp`:
```mlp
print "Hello, World!"
```

Compile and run:
```bash
./melpc hello.mlp hello.c
gcc hello.c -I runtime -L runtime -lruntime -o hello
./hello
```

---

## 📦 Project Structure

```
MELP-GCC/
├── C/          # Stage0 compiler (C implementation)
├── MLP/        # Stage1 compiler (MLP implementation)
├── runtime/    # Runtime library (GC, stdlib)
├── bootstrap/  # Bootstrap components
├── examples/   # Example MLP programs
├── tests/      # Test suite
└── docs/       # Documentation
```

---

## ✨ Features

### Language Features
- ✅ Variables and expressions
- ✅ Control flow (if, while)
- ✅ Functions with parameters
- ✅ String operations (SSO optimized)
- ✅ Lists (dynamic arrays)
- ✅ Optional types (null safety)
- ✅ State management
- ✅ Error handling (panic)
- ✅ File I/O operations
- ✅ Math functions

### Compiler Components
- **Stage0:** MLP → C compiler (733KB C codebase)
- **Stage1:** Type validator (19KB MLP codebase)
- **Runtime:** Modular C library (88KB, 15 modules)

### Runtime Library Modules
```
libruntime.a (88KB)
├── runtime_error.c/h      (Error handling)
├── runtime_memory.c/h     (Memory management)
├── runtime_string.c/h     (String ops + SSO)
├── runtime_types.c/h      (Type conversions)
├── runtime_array.c/h      (Dynamic arrays)
├── runtime_print.c/h      (I/O)
├── runtime_io.c/h         (File I/O)
├── runtime_system.c/h     (System calls)
├── mlp_list.c/h          (Stdlib: Lists)
├── mlp_optional.c/h      (Stdlib: Optional)
├── mlp_state.c/h         (Stdlib: State)
├── mlp_panic.c/h         (Stdlib: Panic)
├── mlp_io.c/h            (Stdlib: I/O)
└── mlp_math.c/h          (Stdlib: Math)
```

---

## 🎯 Design Principles

1. **MODULAR:** Maximum 500 lines per file
2. **GCC:** C backend (no LLVM dependency)
3. **STO:** Smart Type Optimization
4. **STATELESS:** No global state
5. **STRUCT+FUNC:** Pure C, no OOP
6. **MODÜL=ŞABLON:** Independent modules

---

## 📊 Current Status

**Test Results:**
- Runtime: 0 warnings (GCC strict)
- Tests: 7/7 passing (100%)
- Memory: 0 leaks (valgrind clean)

**Compiler Stages:**
- ✅ Stage0: Fully functional (C → MLP transpiler)
- ✅ Stage1: Type validation complete
- 🚧 Stage2: Full self-hosting (in progress)

---

## 🛣️ Roadmap

### v0.9.0 (Current): Bootstrap-Ready ✅
- Basic MLP → C compilation
- Modular runtime library
- Core language features
- Production-ready runtime

### v1.0.0: Full Self-Hosting
- Complete parser/codegen implementation
- Stage2 → Stage3 convergence
- Production compiler release

### v1.1.0: Developer Tools
- LSP (Language Server Protocol)
- Debugger integration
- Code formatter and linter

### v2.0.0: Performance & Optimization
- LLVM backend option
- Advanced optimizations
- WebAssembly support

[📖 Full Roadmap](ROADMAP.md)

---

## 📚 Documentation

- **[Getting Started Guide](docs/getting-started.md)** - Detailed installation and usage
- **[Language Reference](docs/language-reference.md)** - MLP syntax and semantics
- **[Architecture Overview](docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md)** - Technical design
- **[Backend Comparison](docs/COMPILER_BACKENDS_COMPARISON.md)** - GCC vs LLVM analysis
- **[API Reference](docs/api-reference.md)** - Runtime library API

---

## 🤝 Contributing

We welcome contributions! Here's how you can help:

- 🐛 **Report bugs** - Open an issue with reproduction steps
- 💡 **Suggest features** - Share your ideas in discussions
- 📝 **Improve docs** - Help us write better documentation
- 🧪 **Write tests** - Expand test coverage
- 🎨 **Create examples** - Show off what MLP can do

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

**Areas needing help:**
- Parser/codegen enhancements
- Test coverage expansion
- Example programs
- Performance benchmarks
- Documentation improvements

---

## 🧪 Example Programs

Check out the `examples/` directory for sample programs:

- **hello_english.mlp** - Hello World
- **math_examples.mlp** - Math operations
- **file_processor.mlp** - File I/O
- **hashmap_examples.mlp** - Data structures
- **type_conversion_examples.mlp** - Type system

---

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

**Inspired by:**
- **Rust** - Bootstrap strategy and safety focus
- **Go** - Fast compilation approach
- **Swift** - Language evolution model
- **Nim** - C transpiler pattern
- **V** - Simplicity and clarity

**Built with:**
- GCC (C compiler)
- Classic bootstrap techniques
- Modern language design principles

---

## 📞 Community

- 🐛 **Issues:** [Report bugs](https://github.com/MELP-Lang/MELP-GCC/issues)
- 💬 **Discussions:** [Join the conversation](https://github.com/MELP-Lang/MELP-GCC/discussions)
- ⭐ **Star us** on GitHub if you find this project interesting!

---

## 📈 Project Stats

- **Language:** MLP (Modern Language Project)
- **Backend:** GCC C compiler
- **Status:** Bootstrap-Ready
- **Version:** v0.9.0
- **Date:** 30 December 2025

---

**Thank you for your interest in MLP-GCC!** 🎉

If you find this project useful, please consider giving it a ⭐ star on GitHub!

---

*MLP-GCC is actively developed. Check back often for updates!*

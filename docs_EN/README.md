# MELP Language Documentation (English)

Reference guides and tutorials for MELP (Modern Expressive Language Platform).

## Contents

- [Language Reference](language_reference.md) — Syntax, data types, operators
- [Getting Started](getting_started.md) — Write and compile your first program
- [Standard Library](stdlib.md) — Built-in functions

## Quick Start

```bash
# Compile
melp_compiler program.mlp -o program.ll

# Link and run
clang -O0 -x ir program.ll mlp_runtime.bc -lm -o program
./program
```

## Contributing

Please use [GitHub Issues](https://github.com/MELP-Lang/MELP/issues) for bug reports and suggestions.

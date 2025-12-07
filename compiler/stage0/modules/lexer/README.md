# Lexer Module

**Purpose:** Tokenize MELP source code into lexical tokens

**Status:** Shared library (NOT standalone - required by all modules)

---

## Why This Module is Different

Unlike other modules, **lexer is a SHARED LIBRARY**, not a standalone binary.

**Reason:** Every module needs lexer to parse MELP code:
- `print_parser.c` uses lexer to parse `print("text")`
- `arithmetic_parser.c` uses lexer to parse `x + y`
- `control_flow_parser.c` uses lexer to parse `if ... then`

**Alternative Considered:** Make lexer standalone (stdin → tokens.json)
- ❌ Problem: Would require pre-tokenization before every module
- ❌ Problem: Increases complexity (every module reads JSON tokens)
- ❌ Problem: Performance overhead (serialization/deserialization)

**Decision:** Keep lexer as shared library for Stage 0

---

## Files

- `lexer.c` - Lexer implementation (10876 bytes)
- `lexer.h` - Public API (Token, Lexer struct, lexer_create, lexer_next_token)
- `README.md` - This file

---

## Usage

All modules include lexer via:

```c
#include "../lexer/lexer.h"

// In module code:
Lexer* lexer = lexer_create(source_code);
Token* token = lexer_next_token(lexer);
```

**Include path:** `../lexer/lexer.h` (relative from module directory)

---

## Token Types

Defined in `lexer.h`:

```c
TOKEN_PRINT       // print keyword
TOKEN_NUMERIC     // numeric keyword  
TOKEN_STRING_TYPE // string keyword
TOKEN_IF          // if keyword
TOKEN_WHILE       // while keyword
TOKEN_FOR         // for keyword
TOKEN_FUNCTION    // function keyword
TOKEN_IDENTIFIER  // variable names
TOKEN_STRING      // "string literals"
TOKEN_NUMBER      // 42, 3.14
TOKEN_PLUS        // +
TOKEN_MINUS       // -
// ... (see lexer.h for full list)
```

---

## Compilation

Lexer compiles with modules that use it:

```bash
# Example: Compiling print module with lexer
gcc -c modules/lexer/lexer.c -o modules/lexer/lexer.o
gcc -c modules/print/print_parser.c -o modules/print/print_parser.o
gcc modules/lexer/lexer.o modules/print/print_parser.o -o print_module
```

**Makefile handles this automatically.**

---

## Future: Stage 1 (Self-hosting)

When MELP compiles itself:

1. **Stage 0:** C-based lexer (this file)
2. **Stage 1:** Rewrite lexer in MELP
   ```melp
   -- lexer.mlp
   function tokenize(source: string) -> Token[]
       -- Lexer logic in MELP
   end function
   ```
3. **Stage 2:** MELP lexer compiles Stage 1 lexer

**Then:** Lexer becomes standalone (MELP → tokens.json)

---

## Design Decisions

### Q: Why not JSON input/output like other modules?

**A:** Lexer is the **first step** in compilation:

```
Source Code → [LEXER] → Tokens → [PARSER] → AST → [CODEGEN] → Assembly
            ↑ This step
```

If lexer used JSON:
```
Source Code → ??? → tokens.json → [LEXER reads JSON?] ← Circular!
```

**Solution:** Lexer reads raw source, outputs C structs (Token*)

### Q: Does this violate modularity?

**A:** No, because:
- ✅ Lexer is in `modules/lexer/` (not root)
- ✅ Single responsibility (tokenization only)
- ✅ Clear API (lexer.h)
- ✅ Can be replaced (rewrite in MELP later)
- ✅ Testable (feed source, check tokens)

**Difference:** Shared library vs standalone binary

---

## Testing

```c
// test_lexer.c
#include "lexer.h"

int main() {
    Lexer* lexer = lexer_create("numeric x = 42");
    
    Token* t1 = lexer_next_token(lexer);  // TOKEN_NUMERIC
    Token* t2 = lexer_next_token(lexer);  // TOKEN_IDENTIFIER "x"
    Token* t3 = lexer_next_token(lexer);  // TOKEN_ASSIGN "="
    Token* t4 = lexer_next_token(lexer);  // TOKEN_NUMBER "42"
    
    // Assert tokens match expected
    return 0;
}
```

---

## Migration Path

**Current (Stage 0):**
```
modules/lexer/lexer.c → Shared C library
```

**Future (Stage 1):**
```
modules/lexer/lexer.mlp → Standalone MELP binary
echo 'numeric x = 42' | ./modules/lexer/lexer > tokens.json
```

**Then:** Other modules read tokens.json instead of calling lexer functions

---

## Summary

- **What:** Shared lexer library for all modules
- **Why:** Every module needs tokenization, avoiding duplication
- **Status:** Temporary (Stage 0 only)
- **Future:** Rewrite in MELP, make standalone

**This is the ONLY shared module. All others must be standalone!**

---

**Last Updated:** 7 Aralık 2025  
**Location:** `compiler/stage0/modules/lexer/`  
**Lines:** lexer.c (10876), lexer.h (1904)

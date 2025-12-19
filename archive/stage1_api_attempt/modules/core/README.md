# Core Utilities Module

**Location:** `compiler/stage1/modules/core/`  
**Status:** ✅ Complete  
**Created:** YZ_03 (17 Aralık 2025)

## Purpose

Provides foundational utilities for the Stage 1 MELP compiler:
- Token type constants for lexer
- Character classification and manipulation
- Type system constants and mapping

## Files

### `token_types.mlp`
Defines all token type constants used by the lexer.

**Converted from:** `compiler/stage0/modules/lexer/lexer.h`

**Constants:** 93 token types
- Basic types: `TOKEN_NUMERIC`, `TOKEN_STRING_TYPE`, `TOKEN_IDENTIFIER`, etc.
- Operators: `TOKEN_PLUS`, `TOKEN_EQUAL`, `TOKEN_AND`, etc.
- Keywords: `TOKEN_IF`, `TOKEN_FUNCTION`, `TOKEN_IMPORT`, etc.
- Delimiters: `TOKEN_LPAREN`, `TOKEN_COMMA`, `TOKEN_DOT`, etc.

**Usage:**
```mlp
import "core/token_types"

if token.type == TOKEN_IDENTIFIER then
    -- handle identifier
end_if
```

### `char_utils.mlp`
Character utility functions for lexical analysis.

**Converted from:** `compiler/stage0/modules/lexer/lexer.c` (ctype.h equivalents)

**Functions:**
- `is_digit(c)` - Check if character is 0-9
- `is_alpha(c)` - Check if character is a-z or A-Z
- `is_alnum(c)` - Check if character is alphanumeric
- `is_space(c)` - Check if character is whitespace
- `is_upper(c)` / `is_lower(c)` - Case checking
- `to_upper(c)` / `to_lower(c)` - Case conversion
- `is_identifier_start(c)` - Valid identifier first character
- `is_identifier_char(c)` - Valid identifier continuation
- `is_ascii(c)` - Check if character is ASCII

**Usage:**
```mlp
import "core/char_utils"

if is_digit(ch) == 1 then
    -- process digit
end_if
```

### `type_mapper.mlp`
Type system constants and type mapping utilities.

**Converted from:** `compiler/stage0/modules/type_system/type_system.h`

**Constants:** 14 type kinds
- `TYPE_INT`, `TYPE_FLOAT`, `TYPE_STRING`, `TYPE_BOOL`, `TYPE_VOID`
- `TYPE_ARRAY`, `TYPE_STRUCT`, `TYPE_ENUM`, `TYPE_FUNCTION`
- `TYPE_GENERIC`, `TYPE_UNKNOWN`, `TYPE_ERROR`

**Functions:**
- `type_name_to_kind(name)` - Convert type name to kind constant
- `type_kind_to_name(kind)` - Convert kind constant to name
- `types_compatible(lhs, rhs)` - Check type compatibility
- `is_primitive_type(kind)` - Check if primitive type
- `is_composite_type(kind)` - Check if composite type
- `is_callable_type(kind)` - Check if function type

**Usage:**
```mlp
import "core/type_mapper"

numeric kind = type_name_to_kind("numeric")
if kind == TYPE_INT then
    -- handle integer type
end_if
```

### `test_core.mlp`
Comprehensive test suite for all core utilities.

**Tests:**
- Token type constant values
- Character classification and conversion
- Type mapping and compatibility

**Run tests:**
```bash
./melp_compiler compiler/stage1/modules/core/test_core.mlp
```

## Architecture

**Stage 0 → Stage 1 Conversion:**
```
compiler/stage0/modules/       compiler/stage1/modules/
├── lexer/                  →  ├── core/
│   ├── lexer.h (enum)      →  │   ├── token_types.mlp (const)
│   └── lexer.c (ctype)     →  │   ├── char_utils.mlp (functions)
└── type_system/            →  │   └── type_mapper.mlp (const + functions)
    └── type_system.h       →  └── ...
```

**Design Principles:**
1. **Stateless:** All functions are pure (no global state)
2. **Const-only:** Constants defined with `const numeric`
3. **Explicit returns:** All functions have `returns` type declaration
4. **Import-ready:** Modules use import system for dependencies

## Next Steps

Other modules can now import and use these core utilities:

```mlp
import "core/token_types"
import "core/char_utils"
import "core/type_mapper"

-- Use in your module
```

**Upcoming modules (YZ_04-13):**
- `lexer/` - Tokenization using `char_utils` and `token_types`
- `parser_core/` - Parsing using `token_types`
- `type_system/` - Type checking using `type_mapper`
- `functions/`, `control_flow/`, `expression/`, etc.

## References

- **Stage 0 Source:** `compiler/stage0/modules/lexer/`, `compiler/stage0/modules/type_system/`
- **Architecture:** `MELP_Mimarisi.md` (Stage 1 modular design)
- **Import System:** `docs/IMPORT_SYSTEM_DESIGN.md`
- **Task:** YZ_03 in `TODO.md`

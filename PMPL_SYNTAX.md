# PMPL (Pragmatic MLP) Syntax Reference

**Version:** 1.0  
**Date:** December 14, 2025  
**Status:** Official Specification

---

## 📖 What is PMPL?

**PMPL (Pragmatic MLP)** is the internal representation language that MELP compilers work with. It serves as a **normalized intermediate syntax** between user-facing code (which can be in any supported language/style) and the compiler's lexer/parser.

### Design Philosophy

> "The MELP compiler only sees PMPL. This ensures consistency in language design."  
> — MELP Architecture Principles

**Key Principles:**
1. **Single Source of Truth:** All syntax variations normalize to PMPL
2. **Lexer Simplicity:** PMPL uses single-token keywords (underscore-separated)
3. **Parser Clarity:** No pattern matching needed; simple switch-case statements
4. **Self-Hosting Ready:** PMPL compiler can compile PMPL code

### Architecture Flow

```
[User Code] → [Normalizer] → [PMPL] → [Lexer] → [Parser] → [Codegen]
   (any)      (syntax.json)   (this)  (tokens)  (AST)      (assembly)
```

---

## 🎯 PMPL Syntax Overview

### Block Terminators

All block-ending keywords use **underscore** notation:

```pmpl
-- If statement
if condition then
    statement
end_if          -- Single token: TOKEN_END_IF

-- While loop
while condition
    statement
end_while       -- Single token: TOKEN_END_WHILE

-- For loop
for i = 1 to 10
    statement
end_for         -- Single token: TOKEN_END_FOR

-- Function
function name() as numeric
    statement
end_function    -- Single token: TOKEN_END_FUNCTION

-- Struct
struct Point
    numeric x
    numeric y
end_struct      -- Single token: TOKEN_END_STRUCT
```

### Control Flow

```pmpl
-- Else if (NOT "else if" - underscore!)
if x > 5 then
    print("Large")
else_if x > 2 then    -- TOKEN_ELSE_IF
    print("Medium")
else
    print("Small")
end_if
```

### Loop Control

```pmpl
-- Exit (break equivalent)
while condition
    if exit_condition then
        exit        -- Exit current loop (standalone) ***Bunun TOKEN'i yok.***
    end_if
end_while

-- Specific loop exit
exit_for        -- TOKEN_EXIT_FOR
exit_while      -- TOKEN_EXIT_WHILE
exit_if         -- TOKEN_EXIT_IF (early if exit)
exit_function   -- TOKEN_EXIT_FUNCTION (early return)
exit_switch     -- TOKEN_EXIT_SWITCH (break from switch)

-- Continue
while condition
    if skip_condition then
        continue    -- Continue current loop (standalone) ***Bunun TOKEN'i yok.***
    end_if
end_while

-- Specific loop continue
continue_for    -- TOKEN_CONTINUE_FOR
continue_while  -- TOKEN_CONTINUE_WHILE
```

### Debug Syntax

Two types of debug:

**1. Block Debug:**
```pmpl
debug
    print("Debug info")
    numeric step = 0
    
    start:  -- label (debug-only)
    step = step + 1
    if step < 3 then
        goto start  -- goto (debug-only)
    end_if
    pause  -- pause (debug-only)
end_debug
```

**2. Single-Line Debug:**
```pmpl
debug print("x = " + x)
debug if a == b then c = d
debug start:           -- label
debug goto start       -- goto
debug pause            -- pause
```

**Debug-Only Keywords:**
- `goto` - Only in debug context
- `label:` - Only in debug context  
- `pause` - Only in debug context

**Compiler Behavior:**
- Development: Debug executes
- Production (`--release`): Debug removed (zero overhead)

---

## 📋 Complete Token Reference

### Keywords (Alphabetical)

| PMPL Keyword | Token | Description |
|--------------|-------|-------------|
| `and` | TOKEN_AND | Logical AND |
| `array` | TOKEN_ARRAY | Array type |
| `boolean` | TOKEN_BOOLEAN | Boolean type |
| `continue` | TOKEN_CONTINUE | Continue loop (standalone) |
| `continue_for` | TOKEN_CONTINUE_FOR | Continue for loop |
| `continue_while` | TOKEN_CONTINUE_WHILE | Continue while loop |
| `debug` | TOKEN_DEBUG | Debug block start |
| `downto` | TOKEN_DOWNTO | For loop descending |
| `each` | TOKEN_EACH | For-each loop |
| `else` | TOKEN_ELSE | Else clause |
| `else_if` | TOKEN_ELSE_IF | Else-if clause |
| `end_debug` | TOKEN_END_DEBUG | End debug block |
| `end_for` | TOKEN_END_FOR | End for loop |
| `end_function` | TOKEN_END_FUNCTION | End function |
| `end_if` | TOKEN_END_IF | End if statement |
| `end_enum` | TOKEN_END_ENUM | End enum |
| `end_match` | TOKEN_END_MATCH | End match statement |
| `end_operator` | TOKEN_END_OPERATOR | End operator definition |
| `end_struct` | TOKEN_END_STRUCT | End struct |
| `end_switch` | TOKEN_END_SWITCH | End switch statement |
| `end_try` | TOKEN_END_TRY | End try-catch block |
| `end_while` | TOKEN_END_WHILE | End while loop |
| `exit` | TOKEN_EXIT | Exit block |
| `exit_for` | TOKEN_EXIT_FOR | Exit for loop |
| `exit_function` | TOKEN_EXIT_FUNCTION | Exit function |
| `exit_if` | TOKEN_EXIT_IF | Exit if statement |
| `exit_switch` | TOKEN_EXIT_SWITCH | Exit switch |
| `exit_while` | TOKEN_EXIT_WHILE | Exit while loop |
| `false` | TOKEN_FALSE | Boolean false |
| `for` | TOKEN_FOR | For loop |
| `function` | TOKEN_FUNCTION | Function declaration |
| `goto` | TOKEN_GOTO | Debug goto (debug-only) |
| `pause` | TOKEN_PAUSE | Debug pause (debug-only) |
| `if` | TOKEN_IF | If statement |
| `import` | TOKEN_IMPORT | Import module |
| `in` | TOKEN_IN | For-each iterator |
| `list` | TOKEN_LIST | List type |
| `mod` | TOKEN_MOD | Modulo operator |
| `module` | TOKEN_MODULE | Module declaration |
| `not` | TOKEN_NOT | Logical NOT |
| `numeric` | TOKEN_NUMERIC | Numeric type |
| `or` | TOKEN_OR | Logical OR |
| `print` | TOKEN_PRINT | Print statement |
| `println` | TOKEN_PRINTLN | Print with newline |
| `return` | TOKEN_RETURN | Return statement |
| `returns` | TOKEN_RETURNS | Return type declaration (alias: `as`) |
| `as` | TOKEN_AS | Return type declaration (alias: `returns`) |
| `string` | TOKEN_STRING_TYPE | String type |
| `struct` | TOKEN_STRUCT | Struct declaration |
| `then` | TOKEN_THEN | If/While condition end |
| `to` | TOKEN_TO | For loop ascending |
| `true` | TOKEN_TRUE | Boolean true |
| `tuple` | TOKEN_TUPLE | Tuple type |
| `while` | TOKEN_WHILE | While loop |
| `xor` | TOKEN_XOR | Bitwise XOR |

### Operators

| Symbol | Token | Description |
|--------|-------|-------------|
| `+` | TOKEN_PLUS | Addition |
| `-` | TOKEN_MINUS | Subtraction |
| `*` | TOKEN_MULTIPLY | Multiplication |
| `/` | TOKEN_DIVIDE | Division |
| `%` | TOKEN_MOD | Modulo |
| `**` | TOKEN_POWER | Exponentiation |
| `==` | TOKEN_EQUAL | Equality |
| `!=` | TOKEN_NOT_EQUAL | Inequality |
| `<` | TOKEN_LESS | Less than |
| `>` | TOKEN_GREATER | Greater than |
| `<=` | TOKEN_LESS_EQUAL | Less or equal |
| `>=` | TOKEN_GREATER_EQUAL | Greater or equal |
| `=` | TOKEN_ASSIGN | Assignment |

### Delimiters

| Symbol | Token | Description |
|--------|-------|-------------|
| `(` | TOKEN_LPAREN | Left parenthesis |
| `)` | TOKEN_RPAREN | Right parenthesis |
| `[` | TOKEN_LBRACKET | Left bracket (array) |
| `]` | TOKEN_RBRACKET | Right bracket |
| `<` | TOKEN_LANGLE | Left angle (tuple) |
| `>` | TOKEN_RANGLE | Right angle (tuple) |
| `,` | TOKEN_COMMA | Comma separator |
| `:` | TOKEN_COLON | Type separator |
| `;` | TOKEN_SEMICOLON | Statement terminator |
| `.` | TOKEN_DOT | Member access |

---

## 🔧 Complete Examples

### 1. Function with If-Else

```pmpl
function calculate(numeric x) as numeric
    if x > 100 then
        return x * 2
    else_if x > 50 then
        return x + 10
    else
        return x
    end_if
end_function
```

**Tokens Generated:**
```
TOKEN_FUNCTION, TOKEN_IDENTIFIER("calculate"), TOKEN_LPAREN,
TOKEN_NUMERIC, TOKEN_IDENTIFIER("x"), TOKEN_RPAREN,
TOKEN_RETURNS, TOKEN_NUMERIC,
TOKEN_IF, TOKEN_IDENTIFIER("x"), TOKEN_GREATER, TOKEN_NUMBER(100), TOKEN_THEN,
TOKEN_RETURN, TOKEN_IDENTIFIER("x"), TOKEN_MULTIPLY, TOKEN_NUMBER(2),
TOKEN_ELSE_IF, TOKEN_IDENTIFIER("x"), TOKEN_GREATER, TOKEN_NUMBER(50), TOKEN_THEN,
TOKEN_RETURN, TOKEN_IDENTIFIER("x"), TOKEN_PLUS, TOKEN_NUMBER(10),
TOKEN_ELSE,
TOKEN_RETURN, TOKEN_IDENTIFIER("x"),
TOKEN_END_IF,
TOKEN_END_FUNCTION
```

### 2. While Loop with Break/Continue

```pmpl
function process_items() returns numeric
    numeric count = 0
    numeric i = 0
    
    while i < 100  
        i = i + 1
        
        if i mod 2 == 0 then
            continue        -- Skip even numbers
        end_if
        
        if i > 50 then
            exit            -- Stop at 50
        end_if
        
        count = count + 1
    end_while
    
    return count
end_function
```

### 3. For Loop with Nested Control

```pmpl
function find_first(array numbers) returns numeric
    for i = 0 to 99  
        if numbers[i] < 0 then
            exit_for        -- Explicit for loop exit
        end_if
        
        if numbers[i] == 0 then
            continue_for    -- Explicit for loop continue
        end_if
        
        return numbers[i]
    end_for
    
    return -1
end_function
```

### 4. Struct Definition

```pmpl
struct Point
    numeric x
    numeric y
end_struct

function distance(Point p1, Point p2)  as numeric
    numeric dx = p2.x - p1.x
    numeric dy = p2.y - p1.y
    return (dx * dx + dy * dy) ** 0.5
end_function
```

---

## 🎨 Comparison with User Syntax

### User Code (C-style)

```c
if (x > 5) {
    printf("Yes");
} else if (x > 2) {
    printf("Maybe");
} else {
    printf("No");
}
```

### Normalized to PMPL

```pmpl
if x > 5 then
    print("Yes")
else_if x > 2 then
    print("Maybe")
else
    print("No")
end_if
```

### User Code (Python-style)

```python
if x > 5:
    print("Yes")
elif x > 2:
    print("Maybe")
else:
    print("No")
```

### Normalized to PMPL

```pmpl
if x > 5 then
    print("Yes")
else_if x > 2 then      -- "elif" → "else_if"
    print("Maybe")
else
    print("No")
end_if                  -- indentation end → "end_if"
```

**Result:** Both normalize to the **same PMPL**! ✨

---

## 📐 Normalization Rules

### Block Terminators

```
User: }              → PMPL: end_if / end_while / end_for (context-based)
User: end if         → PMPL: end_if (space → underscore)
User: fi             → PMPL: end_if (shell-style)
User: endif          → PMPL: end_if (basic-style)
User: (indentation)  → PMPL: end_if (python-style)
```

### Keywords

```
User: else if        → PMPL: else_if
User: elif           → PMPL: else_if
User: elsif          → PMPL: else_if
User: break          → PMPL: exit
User: next (Ruby)    → PMPL: continue
User: && / and       → PMPL: and
User: || / or        → PMPL: or
User: ! / not        → PMPL: not
```

### Type Keywords

```
User: int / i32      → PMPL: numeric
User: float / f64    → PMPL: numeric
User: str / char*    → PMPL: string
User: bool           → PMPL: boolean
```

---

## ⚙️ Implementation Notes

### Lexer Behavior

**Input:** `end_if`  
**Output:** `TOKEN_END_IF` (single token)

**NOT:**  
~~Input: `end if`~~  
~~Output: `TOKEN_END`, `TOKEN_IF` (two tokens)~~

The normalizer **must** convert `end if` → `end_if` **before** lexing!

### Parser Simplification

**PMPL Approach (Simple):**
```c
switch (tok->type) {
    case TOKEN_END_IF:
        // End of if block
        return;
    
    case TOKEN_ELSE_IF:
        // Parse else-if
        break;
}
```

**Old Approach (Complex - AVOID):**
```c
if (tok->type == TOKEN_END) {
    Token* next = lexer_next_token(lexer);
    if (next->type == TOKEN_IF) {
        // "end if" pattern
        return;
    }
    lexer_unget_token(lexer, next);
}
```

### Self-Hosting Requirement

PMPL syntax must be **parseable by PMPL compiler**:

```pmpl
-- This PMPL code should compile with PMPL compiler
function parse_if(Lexer lexer) as Statement
    if current_token == TOKEN_IF then
        -- Parse condition
        -- Parse then body
        
        if current_token == TOKEN_ELSE_IF then
            -- Parse else-if recursively
        end_if
        
        return if_statement
    end_if
    
    return null
end_function
```

---

## 🔍 Validation Checklist

For any new PMPL feature:

- [ ] Keyword uses underscore (no spaces): `end_X`, `exit_X`, `continue_X`
- [ ] Lexer has corresponding `TOKEN_X` definition
- [ ] Parser uses simple switch-case (no pattern matching)
- [ ] Normalizer converts all variants → PMPL standard
- [ ] Self-hosting test: PMPL compiler can parse the syntax
- [ ] Documentation updated (this file + migration.md)

---

## 📚 Related Documents

- `docs_tr/migration.md` - Multi-syntax normalization guide (Turkish)
- `kurallar_kitabı.md` - MELP architecture principles (Turkish)
- `compiler/stage0/modules/lexer/lexer.h` - Token definitions
- `compiler/stage0/normalize/syntax_rules.h` - Normalization rules

---

## 📝 Version History

- **1.0** (Dec 14, 2025) - Initial specification
  - Block terminators: `end_X`
  - Control flow: `else_if`
  - Loop control: `exit_X`, `continue_X`
  - Complete token reference
  - Self-hosting requirements

---

**Maintainer:** MELP Language Team  
**License:** Part of MELP Project  
**Status:** ✅ STABLE - Production Ready

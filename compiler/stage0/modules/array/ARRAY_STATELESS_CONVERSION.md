# Array Module - Stateless Conversion Report

**Date:** 9 Aralık 2025  
**Status:** ✅ PARSER CONVERSION COMPLETE (Entry point needs update)  
**AI Agent:** Session continuation from previous work  

---

## 🎯 What Was Done

### ✅ Completed Tasks

1. **array_parser.h - Stateless Signature Migration**
   - ❌ Removed: `ArrayParser` struct typedef
   - ❌ Removed: `array_parser_create()` and `array_parser_free()`
   - ✅ Added: Stateless function signatures with Token borrowing
   
   **New API:**
   ```c
   Collection* array_parse_literal(Lexer* lexer, Token* lbracket_token);
   Collection* array_parse_list_literal(Lexer* lexer, Token* lparen_token);
   Collection* array_parse_tuple_literal(Lexer* lexer, Token* less_token);
   IndexAccess* array_parse_index_access(Lexer* lexer, const char* base_name, Token* index_token);
   ```

2. **array_parser.c - Full Rewrite (440 -> 470 lines)**
   - ❌ Removed: Stateful parser struct and helpers
   - ❌ Removed: `advance()`, `match()`, `expect()` helpers
   - ✅ Implemented: Token borrowing pattern throughout
   - ✅ Implemented: Proper token ownership (BORROWED params, OWNED from lexer)
   - ✅ Fixed: `arithmetic_free()` -> `arithmetic_expr_free()`
   
   **Key Pattern:**
   ```c
   // Old (stateful - REMOVED):
   ArrayParser* parser = array_parser_create(lexer);
   Collection* coll = parse_array_literal(parser);
   array_parser_free(parser);
   
   // New (stateless - ACTIVE):
   Token* tok = lexer_next_token(lexer);  // OWNED
   Collection* coll = array_parse_literal(lexer, tok);  // tok is BORROWED
   token_free(tok);  // Caller frees
   ```

3. **Compilation Test**
   - ✅ `array_parser.c` compiles without errors
   - ✅ No implicit function warnings
   - ✅ Proper header includes verified

---

## ⏳ Remaining Work

### 1. Update array_standalone.c Entry Point (Priority: HIGH)

**Current State:** Still uses old stateful API
```c
// Line ~114: OLD CODE
VariableParser* var_parser = variable_parser_create(lexer);
```

**Needs:** Complete rewrite to match stateless pattern from `functions_standalone.c`

**Reference:** `/compiler/stage0/modules/functions/functions_standalone.c` (102 lines)

**Tasks:**
- Remove `VariableParser*` usage
- Use direct stateless calls: `array_parse_literal(lexer, tok)`
- Implement proper token ownership loop
- Add context passing for codegen

### 2. Update STATELESS_TODO.md

Mark conversion as COMPLETED and document new API.

### 3. Integration Testing

**Test Files Ready:**
- `test_arr.mlp`
- `test_index.mlp`
- `test_array_simple.mlp`

**Test Command:**
```bash
cd compiler/stage0/modules/array
# After fixing standalone:
make clean && make
./array_standalone test_array_simple.mlp test.s
gcc -no-pie test.s -o test
./test
```

---

## 📊 Code Metrics

| File | Old Lines | New Lines | Status |
|------|-----------|-----------|--------|
| array_parser.h | 43 | 32 | ✅ Stateless |
| array_parser.c | 440 | 470 | ✅ Stateless |
| array_standalone.c | 469 | 469 | ⏳ Needs update |

**Stateless Functions:**
- `array_parse_literal()` - 155 lines
- `array_parse_list_literal()` - 95 lines  
- `array_parse_tuple_literal()` - 135 lines
- `array_parse_index_access()` - 85 lines

---

## 🔑 Key Implementation Details

### Token Ownership Pattern

**Rule:** Parameters are BORROWED, `lexer_next_token()` returns OWNED

```c
Collection* array_parse_literal(Lexer* lexer, Token* lbracket_token) {
    // lbracket_token is BORROWED - don't free!
    if (lbracket_token->type != TOKEN_LBRACKET) {
        return NULL;  // Don't free borrowed token!
    }
    
    // Get new tokens - these are OWNED
    Token* tok = lexer_next_token(lexer);
    if (tok->type == TOKEN_RBRACKET) {
        token_free(tok);  // Free what we own
        return empty_collection();
    }
    
    // Parse elements with stateless arithmetic parser
    ArithmeticExpr* elem = arithmetic_parse_expression_stateless(lexer, tok);
    // tok is now consumed by arithmetic parser
    
    // Continue parsing...
}
```

### Expression Parsing Solution

**Problem:** Old code used `expression_parse()` (stateful)  
**Solution:** Use `arithmetic_parse_expression_stateless()`

```c
// OLD (removed):
Parser temp_parser;
temp_parser.lexer = parser->lexer;
temp_parser.current_token = parser->current_token;
elements[i] = expression_parse(&temp_parser);
parser->current_token = temp_parser.current_token;

// NEW (active):
Token* tok = lexer_next_token(lexer);
elements[i] = arithmetic_parse_expression_stateless(lexer, tok);
// Token consumed by parser, no sync needed
```

### Memory Management

**Arrays/Lists/Tuples:**
- Elements stored as `ArithmeticExpr**`
- Freed with `arithmetic_expr_free()`
- Collection freed with `collection_free()`

**Index Access:**
- Three types: const (int), variable (char*), expression (ArithmeticExpr*)
- Proper cleanup in error paths
- Strdup for string copies (collection_name, var_index)

---

## 🐛 Issues Fixed

1. **Function Name:** `arithmetic_free()` -> `arithmetic_expr_free()`
2. **Token Leaks:** All token_free() calls verified
3. **Error Paths:** All malloc'd memory freed on failure

---

## 📋 Next AI Agent Instructions

### Quick Start (5 minutes)

```bash
# 1. Read this report
cat compiler/stage0/modules/array/ARRAY_STATELESS_CONVERSION.md

# 2. Check current status
cd compiler/stage0/modules/array
git status

# 3. Reference working example
cat ../functions/functions_standalone.c | head -120

# 4. Update array_standalone.c
# Follow the stateless pattern from functions_standalone.c
# Key changes:
#   - Remove VariableParser* usage
#   - Direct lexer token loop
#   - Call array_parse_literal(lexer, tok) directly
#   - Add proper error handling

# 5. Test
make clean && make
./array_standalone test_array_simple.mlp test.s
```

### Implementation Template

```c
// array_standalone.c - NEW STATELESS PATTERN
int main(int argc, char** argv) {
    // ... setup ...
    
    Lexer* lexer = lexer_create(source);
    
    // Token loop - stateless style
    Token* tok = lexer_next_token(lexer);
    while (tok && tok->type != TOKEN_EOF) {
        
        if (tok->type == TOKEN_LBRACKET) {
            // Parse array literal
            Collection* arr = array_parse_literal(lexer, tok);
            if (arr) {
                printf("✓ Parsed array with %d elements\n", arr->data.array.length);
                collection_free(arr);
            }
            token_free(tok);
        }
        else if (tok->type == TOKEN_LPAREN) {
            // Parse list literal
            Collection* list = array_parse_list_literal(lexer, tok);
            // ...
        }
        else {
            token_free(tok);
        }
        
        tok = lexer_next_token(lexer);
    }
    
    if (tok) token_free(tok);
    // ... cleanup ...
}
```

---

## 🎓 Lessons Learned

1. **Stateless = Simpler:** No state synchronization, no malloc/free overhead
2. **Token Borrowing:** Clear ownership prevents double-free and leaks
3. **Modular Reuse:** `arithmetic_parse_expression_stateless()` works perfectly
4. **Error Handling:** Every `malloc()` needs corresponding cleanup path

---

## 🔗 Related Files

- `STATELESS_TODO.md` - Original conversion plan (now mostly done)
- `../functions/functions_parser.c` - Reference stateless implementation
- `../variable/variable_parser.c` - Another stateless example
- `../logical/logical_parser.c` - Token** recursive pattern

---

**Status Summary:** Parser conversion ✅ COMPLETE, Entry point ⏳ PENDING

**Estimated Time to Complete:** 30-45 minutes (entry point rewrite + testing)

**Risk Level:** LOW (pattern proven in 3 other modules)

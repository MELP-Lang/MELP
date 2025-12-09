# Array Parser - Stateless Refactoring TODO

**Status:** NOT CONVERTED YET (intentionally deferred)  
**Date:** 9 Aralık 2025  
**Reason:** Complex refactoring, module not yet used, requires expression_parser changes

---

## Current State (Stateful - 440 lines)

```c
// ❌ STATEFUL (current)
ArrayParser* array_parser_create(Lexer* lexer);
void array_parser_free(ArrayParser* parser);
Collection* parse_array_literal(ArrayParser* parser);
Collection* parse_list_literal(ArrayParser* parser);
Collection* parse_tuple_literal(ArrayParser* parser);
Expression* parse_index_access(ArrayParser* parser);
IndexAccess* parse_index_access_struct(ArrayParser* parser, const char* base_name);
```

### Dependencies
- `expression_parser.h` - Also stateful, needs conversion first
- `arithmetic_parser.h` - Already has `_stateless` version ✅
- Heavy use of `Parser temp_parser` for expression parsing

---

## Target State (Stateless)

```c
// ✅ STATELESS (target)
Collection* array_parse_literal(Lexer* lexer, Token* lbracket_token);
Collection* list_parse_literal(Lexer* lexer, Token* lparen_token);
Collection* tuple_parse_literal(Lexer* lexer, Token* less_token);
IndexAccess* array_parse_index_access(Lexer* lexer, Token* identifier_token);
```

---

## Refactoring Steps

### 1. Prerequisites (Do these first!)
- [ ] Convert `expression_parser` to stateless pattern
- [ ] Ensure `arithmetic_parse_expression_stateless()` is stable
- [ ] Review comparison_parser stateless implementation

### 2. Header Changes (`array_parser.h`)
```c
// Remove struct typedef
typedef struct {
    Lexer* lexer;
    Token* current_token;
} ArrayParser;  // ❌ DELETE

// Update function signatures
Collection* array_parse_literal(Lexer* lexer, Token* lbracket_token);  // [
Collection* list_parse_literal(Lexer* lexer, Token* lparen_token);     // (
Collection* tuple_parse_literal(Lexer* lexer, Token* less_token);      // <
IndexAccess* array_parse_index_access(Lexer* lexer, Token* identifier_token);
```

### 3. Implementation Changes (`array_parser.c`)

**Remove:**
- `array_parser_create()` - line 8
- `array_parser_free()` - line 15
- `advance()` helper - line 46
- `match()` helper - line 55
- `expect()` helper - line 60

**Token Management Pattern:**
```c
// Instead of advance(parser):
token_free(*current);
*current = lexer_next_token(lexer);

// Instead of match(parser, TOKEN_X):
(*current)->type == TOKEN_X

// Instead of expect(parser, TOKEN_X, "msg"):
if ((*current)->type != TOKEN_X) {
    fprintf(stderr, "Error: msg\n");
    token_free(*current);
    return NULL;
}
token_free(*current);
*current = lexer_next_token(lexer);
```

**Expression Parsing Problem:**
```c
// Current (stateful):
Parser temp_parser;
temp_parser.lexer = parser->lexer;
temp_parser.current_token = parser->current_token;
elements[i] = expression_parse(&temp_parser);
parser->current_token = temp_parser.current_token;

// Target (stateless):
// Need expression_parse_stateless() first!
elements[i] = expression_parse_stateless(lexer, *current);
// But expression module needs refactoring too!
```

### 4. Testing Strategy
```bash
# After conversion:
cd modules/array
make clean && make

# Test array literal
cat > test_array.mlp << 'EOF'
function main() returns numeric
    numeric arr[3] = [1, 2, 3]
    return arr[0]
end function
EOF

./array_compiler test_array.mlp test.s
gcc -no-pie test.s -o test && ./test
echo $?  # Should return 1
```

---

## Complexity Analysis

### Why This is Hard
1. **Expression Parser Dependency** - 70+ calls to `expression_parse()`
2. **Complex State Management** - Parser struct passed to multiple helpers
3. **Three Collection Types** - Arrays, Lists, Tuples each with different syntax
4. **Index Access** - Two forms: `arr[i]` and `list(i)`
5. **Error Recovery** - Need to clean up partially parsed structures

### Estimated Effort
- Prerequisites (expression_parser refactor): **8-10 hours**
- Array parser refactor: **4-6 hours**
- Testing and debugging: **2-4 hours**
- **Total: 14-20 hours**

### Lines of Code Impact
- Current: 440 lines
- Estimated after refactor: ~380 lines (-60 lines from removed helpers)
- New helper functions needed: +50 lines for token management
- **Net: ~370 lines**

---

## Decision: DEFER

**Reasoning:**
1. ✅ **YAGNI Principle** - Arrays not used yet in compiler
2. ✅ **Dependency Chain** - expression_parser must be converted first
3. ✅ **ROI Too Low** - 20 hours work for unused feature
4. ✅ **Better Strategy** - Convert when arrays are actually needed

**Alternative Approach:**
- Document the pattern (this file ✅)
- Ensure new code uses stateless pattern
- Convert array_parser when arrays become critical feature

---

## When to Convert

**Triggers:**
- [ ] Arrays used in MLP language features
- [ ] expression_parser converted to stateless
- [ ] Performance profiling shows parser overhead
- [ ] Self-hosting compiler needs array support

**Don't convert if:**
- Arrays remain unused
- expression_parser stays stateful
- Other higher-priority work exists

---

## References

**Stateless Examples:**
- `functions_parser.c` (Phase 4.3) - Perfect reference
- `variable_parser.c` (Phase 4.4) - With lexer_unget_token()
- `logical_parser.c` (Phase 4.4) - Recursive parsing pattern

**Token Ownership:**
- BORROWED: Function parameter tokens (don't free)
- OWNED: Tokens from lexer_next_token() (must free)
- PUSHBACK: lexer_unget_token() for lookahead

**Documentation:**
- `ARCHITECTURE.md` - Lines 575-730 (Phase 4.4)
- `docs/PHASE_4_4_GUIDE.md` - Step-by-step guide
- `modules/functions/functions_parser.c` - Reference implementation

---

**Next AI Agent:**
If you need to convert this module, read this file first, then:
1. Check if expression_parser is stateless yet
2. Follow the pattern in logical_parser.c (recently converted)
3. Use Token** pattern for internal token management
4. Test incrementally (one function at a time)
5. Don't forget lexer_unget_token() for lookahead!

Good luck! 🚀

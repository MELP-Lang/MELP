# 🚀 NEXT AI SESSION - START HERE

**Date:** 9 Aralık 2025  
**Previous Work:** Array parser stateless conversion ✅  
**Next Task:** Update array_standalone.c entry point  

---

## 📖 Quick Onboarding (5 minutes)

### 1️⃣ Read Conversion Report
```bash
cat compiler/stage0/modules/array/ARRAY_STATELESS_CONVERSION.md
```

Key points:
- ✅ Parser converted: 440 -> 470 lines, fully stateless
- ✅ Compilation successful, no warnings
- ⏳ Entry point needs update: `array_standalone.c` (469 lines)

### 2️⃣ Reference Implementation
```bash
# Perfect working example to copy pattern from:
cat compiler/stage0/modules/functions/functions_standalone.c
```

This is 102 lines, clean stateless pattern.

### 3️⃣ Current State Check
```bash
cd compiler/stage0/modules/array
git diff array_parser.c  # See what changed
git diff array_parser.h  # See new API
```

---

## 🎯 Your Mission

**Task:** Rewrite `array_standalone.c` to use stateless array parser

**Time Estimate:** 30-45 minutes  
**Difficulty:** EASY (copy pattern from functions_standalone.c)  
**Risk:** LOW (pattern proven in 4 modules)

---

## 📋 Step-by-Step Plan

### Step 1: Understand New API (5 min)

```c
// New stateless API (array_parser.h):
Collection* array_parse_literal(Lexer* lexer, Token* lbracket_token);  // [1,2,3]
Collection* array_parse_list_literal(Lexer* lexer, Token* lparen_token);  // (1;2;3;)
Collection* array_parse_tuple_literal(Lexer* lexer, Token* less_token);  // <1,2,3>
IndexAccess* array_parse_index_access(Lexer* lexer, const char* base_name, Token* index_token);
```

**Key:** All `Token*` parameters are BORROWED (don't free them!)

### Step 2: Remove Old Code (10 min)

Remove from `array_standalone.c`:
- Line ~114: `VariableParser* var_parser = variable_parser_create(lexer);`
- Line ~141-218: Old stateful parsing loop
- Line ~222: `variable_parser_free(var_parser);`

### Step 3: Add Stateless Loop (15 min)

Copy pattern from `functions_standalone.c`:

```c
// Main parsing loop (stateless)
Token* tok = lexer_next_token(lexer);  // OWNED
int iteration = 0;
const int MAX_ITERATIONS = 1000;

while (tok && tok->type != TOKEN_EOF && iteration++ < MAX_ITERATIONS) {
    
    if (tok->type == TOKEN_LBRACKET) {
        // Parse array literal: [1, 2, 3]
        Collection* arr = array_parse_literal(lexer, tok);  // tok is BORROWED
        if (arr) {
            stats.array_literals++;
            printf("      ✓ Found array literal with %d elements\n", arr->data.array.length);
            
            // TODO: Store or generate code for array
            collection_free(arr);
        }
        token_free(tok);  // Free OWNED token
    }
    else if (tok->type == TOKEN_LPAREN) {
        // Parse list literal: (1; 2; 3;)
        Collection* list = array_parse_list_literal(lexer, tok);  // BORROWED
        if (list) {
            stats.list_literals++;
            printf("      ✓ Found list literal with %d elements\n", list->data.list.length);
            collection_free(list);
        }
        token_free(tok);
    }
    else if (tok->type == TOKEN_LESS) {
        // Parse tuple literal: <1, 2, 3>
        Collection* tuple = array_parse_tuple_literal(lexer, tok);  // BORROWED
        if (tuple) {
            stats.tuple_literals++;
            printf("      ✓ Found tuple literal with %d elements\n", tuple->data.tuple.length);
            collection_free(tuple);
        }
        token_free(tok);
    }
    else {
        // Unknown token, skip
        token_free(tok);
    }
    
    tok = lexer_next_token(lexer);  // OWNED - next token
}

if (tok) token_free(tok);  // Cleanup final token
```

### Step 4: Test (10 min)

```bash
cd compiler/stage0/modules/array

# Clean build
make clean && make

# Test with simple array
cat > test_simple.mlp << 'EOF'
numeric[] arr = [1, 2, 3, 4, 5]
EOF

./array_standalone test_simple.mlp test.s
cat test.s  # Verify assembly

# Compile and run
nasm -f elf64 test.s -o test.o
ld test.o -o test
./test
echo $?
```

---

## 🔑 Key Concepts

### Token Ownership Rules

```c
// Rule 1: lexer_next_token() returns OWNED token
Token* tok = lexer_next_token(lexer);  // You OWN this!
// Must free: token_free(tok);

// Rule 2: Function parameters are BORROWED
Collection* array_parse_literal(Lexer* lexer, Token* lbracket_token) {
    // lbracket_token is BORROWED - DON'T FREE!
    if (lbracket_token->type != TOKEN_LBRACKET) {
        return NULL;  // No free here!
    }
    // ...
}

// Rule 3: Always free what you own
tok = lexer_next_token(lexer);  // OWNED
array_parse_literal(lexer, tok);  // Borrows tok
token_free(tok);  // We still own it, must free!
```

### Error Handling

```c
Collection* arr = array_parse_literal(lexer, tok);
if (!arr) {
    fprintf(stderr, "❌ Parse error at line %d\n", tok->line);
    token_free(tok);  // Don't leak on error!
    // Continue or exit
}
token_free(tok);  // Always free OWNED tokens
```

---

## 📊 Success Criteria

✅ Code compiles without warnings  
✅ No memory leaks (valgrind clean)  
✅ Can parse: `[1, 2, 3]` and print "Found array with 3 elements"  
✅ Can parse: `(10; 20;)` and print "Found list with 2 elements"  
✅ Can parse: `<1, "a">` and print "Found tuple with 2 elements"  

---

## 🚨 Common Pitfalls

❌ **Don't free borrowed tokens:**
```c
// WRONG:
Collection* arr = array_parse_literal(lexer, tok);
token_free(tok);  // If tok was borrowed, this is WRONG!
```

❌ **Don't forget to free owned tokens:**
```c
// WRONG:
Token* tok = lexer_next_token(lexer);  // OWNED
if (tok->type == TOKEN_LBRACKET) {
    // ... parse ...
    // Forgot token_free(tok)! LEAK!
}
```

✅ **Correct pattern:**
```c
Token* tok = lexer_next_token(lexer);  // OWNED
if (tok->type == TOKEN_LBRACKET) {
    Collection* arr = array_parse_literal(lexer, tok);  // Borrows tok
    // ... use arr ...
    collection_free(arr);
    token_free(tok);  // Free what we own
}
```

---

## 📁 Files You'll Edit

1. `array_standalone.c` - Main rewrite (469 lines -> ~150 lines expected)

**Backup created:** `array_parser.c.old` (old stateful version)

---

## 🎓 Learning Resources

- `ARRAY_STATELESS_CONVERSION.md` - Full technical report
- `../functions/functions_standalone.c` - Perfect reference (102 lines)
- `../variable/variable_parser.c` - Another stateless example
- `../../ARCHITECTURE.md` - Lines 1-200 explain stateless pattern

---

## 🏁 When You're Done

1. ✅ Test compilation
2. ✅ Test with 3 sample .mlp files
3. ✅ Update `ARRAY_STATELESS_CONVERSION.md` status to "FULLY COMPLETE"
4. ✅ Update `ARCHITECTURE.md` - remove "entry point pending"
5. ✅ Commit with message: "Array module: Stateless entry point complete"

---

**Good luck!** 💪 The hard part (parser conversion) is done. This is just wiring!

**Questions?** Check the conversion report or reference functions_standalone.c

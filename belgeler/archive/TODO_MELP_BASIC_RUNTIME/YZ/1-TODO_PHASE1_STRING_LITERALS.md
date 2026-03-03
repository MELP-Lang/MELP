# 📋 PHASE 1: STRING LITERALS

**Duration:** 2-3 saat  
**Priority:** 🔴 CRITICAL  
**Dependencies:** None (can start immediately)  
**Owner:** YZ_BASIC_01

---

## 🎯 PHASE GOAL

String literals ("...") compile edilip LLVM global constants'a dönüşmeli.

**Input:**
```mlp
function main()
  string s = "merhaba dünya"
  return 0
end_function
```

**Expected LLVM IR:**
```llvm
@.str.0 = private unnamed_addr constant [15 x i8] c"merhaba d\C3\BCnya\00"

define i32 @main() {
entry:
  %s = alloca i8*
  %0 = getelementptr inbounds [15 x i8], [15 x i8]* @.str.0, i32 0, i32 0
  store i8* %0, i8** %s
  ret i32 0
}
```

---

## 📋 TASK BREAKDOWN

### Task 1.1: Check Parser Support for String Literals (30 min)

**File:** `compiler/stage0/parser/parser.cpp`

**Goal:** Verify that parser can handle string literals (`"hello"`)

**Steps:**
1. **Search for STRING_LITERAL handling:**
   ```bash
   cd compiler/stage0/parser
   grep -r "STRING_LITERAL" .
   grep -r "parseString" .
   ```

2. **Check if lexer tokenizes strings:**
   ```bash
   # Look for token types
   grep -r "TOKEN_STRING" .
   grep -r "T_STRING" .
   ```

3. **Test parser:**
   ```mlp
   # Create test file
   cat > /tmp/test_string.mlp << 'EOF'
   function main()
     string s = "test"
     return 0
   end_function
   EOF
   
   # Try to parse (if parser binary exists)
   ./parser /tmp/test_string.mlp --dump-ast
   ```

**Expected Outcomes:**
- ✅ Parser already supports strings → Continue to Task 1.2
- ❌ Parser doesn't support strings → Implement parseStringLiteral() (add 2 hours)

**If Parser Missing:**
```cpp
// Add to parser.cpp
ASTNode* Parser::parseStringLiteral() {
    expect(TOKEN_STRING);  // Consume string token
    
    ASTNode* node = new ASTNode();
    node->type = STRING_LITERAL;
    node->value = currentToken.value;  // "hello"
    
    return node;
}
```

---

### Task 1.2: Add/Verify STRING_LITERAL in AST (15 min)

**File:** `compiler/stage1/common/ast.h`

**Goal:** Ensure AST can represent string literals

**Steps:**
1. **Check NodeType enum:**
   ```bash
   grep -A 20 "enum NodeType" compiler/stage1/common/ast.h
   ```

2. **Look for STRING_LITERAL:**
   ```cpp
   enum NodeType {
       // Literals
       NUMERIC_LITERAL,
       STRING_LITERAL,    // ← Must exist
       BOOLEAN_LITERAL,
       ...
   };
   ```

3. **Check ASTNode struct:**
   ```cpp
   struct ASTNode {
       NodeType type;
       std::string value;  // String literal content
       ...
   };
   ```

**If Missing, Add:**
```cpp
// In ast.h
enum NodeType {
    // Existing...
    NUMERIC_LITERAL,
    STRING_LITERAL,     // ← Add this
    BOOLEAN_LITERAL,
    // ...
};
```

---

### Task 1.3: Implement createStringLiteral() Method (60-90 min)

**File:** `compiler/stage1/modules/function_compiler/function_compiler.cpp`

**Goal:** Convert string literals to LLVM global constants

**Implementation:**

```cpp
// Add to function_compiler.h
class FunctionCompiler {
private:
    int stringCounter = 0;  // For .str.0, .str.1, etc.
    
public:
    llvm::Value* createStringLiteral(const std::string& str);
};
```

```cpp
// Add to function_compiler.cpp
llvm::Value* FunctionCompiler::createStringLiteral(const std::string& str) {
    // 1. Create LLVM string constant (with null terminator)
    llvm::Constant* strConstant = llvm::ConstantDataArray::getString(
        context, 
        str,
        true  // Add null terminator
    );
    
    // 2. Create global variable for the string
    std::string globalName = ".str." + std::to_string(stringCounter++);
    llvm::GlobalVariable* globalStr = new llvm::GlobalVariable(
        *module,
        strConstant->getType(),
        true,  // isConstant (strings are immutable)
        llvm::GlobalValue::PrivateLinkage,
        strConstant,
        globalName
    );
    
    // 3. Get pointer to first character (i8*)
    llvm::Value* zero = llvm::ConstantInt::get(
        llvm::Type::getInt32Ty(context), 
        0
    );
    
    llvm::Value* strPtr = builder.CreateInBoundsGEP(
        globalStr->getValueType(),
        globalStr,
        {zero, zero},
        "str_ptr"
    );
    
    return strPtr;  // Returns i8* pointer
}
```

**Key Points:**
- `ConstantDataArray::getString()` creates `[N x i8]` constant
- Global variable has `PrivateLinkage` (not exported)
- GEP (GetElementPtr) converts `[N x i8]*` to `i8*`
- Null terminator automatically added (third param = true)

---

### Task 1.4: Integrate into compileExpression() (30 min)

**File:** `compiler/stage1/modules/function_compiler/function_compiler.cpp`

**Goal:** Handle STRING_LITERAL nodes in expression compiler

**Implementation:**

```cpp
llvm::Value* FunctionCompiler::compileExpression(ASTNode* node) {
    switch (node->type) {
        case NUMERIC_LITERAL:
            return compileNumericLiteral(node);
            
        case STRING_LITERAL:
            // NEW: Handle string literals
            return createStringLiteral(node->value);
            
        case BOOLEAN_LITERAL:
            return compileBooleanLiteral(node);
            
        // ... other cases
    }
}
```

**Important:**
- `node->value` contains the string content (without quotes)
- Parser should strip quotes: `"hello"` → `hello`
- Return type is `i8*` (pointer to string data)

---

### Task 1.5: Test Phase 1 (30 min)

**Test Case 1: Simple String Literal**
```mlp
# File: tests/phase1/test_string_simple.mlp
function main()
  string s = "hello"
  return 0
end_function
```

**Expected IR:**
```llvm
@.str.0 = private unnamed_addr constant [6 x i8] c"hello\00"

define i32 @main() {
entry:
  %s = alloca i8*
  %str_ptr = getelementptr inbounds [6 x i8], [6 x i8]* @.str.0, i32 0, i32 0
  store i8* %str_ptr, i8** %s
  ret i32 0
}
```

**Test Case 2: UTF-8 String (Turkish)**
```mlp
function main()
  string s = "merhaba dünya"
  return 0
end_function
```

**Expected IR:**
```llvm
@.str.0 = private unnamed_addr constant [15 x i8] c"merhaba d\C3\BCnya\00"
```

**Test Case 3: Multiple Strings**
```mlp
function main()
  string s1 = "first"
  string s2 = "second"
  return 0
end_function
```

**Expected IR:**
```llvm
@.str.0 = private unnamed_addr constant [6 x i8] c"first\00"
@.str.1 = private unnamed_addr constant [7 x i8] c"second\00"
```

**Validation Commands:**
```bash
# Compile to LLVM IR
./melp_compiler tests/phase1/test_string_simple.mlp -o /tmp/test.ll

# Check IR
cat /tmp/test.ll | grep "@.str"

# Validate IR
llvm-as /tmp/test.ll -o /tmp/test.bc
opt -verify /tmp/test.bc -o /dev/null
echo "Exit code: $?"  # Should be 0

# Compile to object
llc /tmp/test.ll -o /tmp/test.o
echo "Exit code: $?"  # Should be 0
```

---

## ✅ SUCCESS CRITERIA

**Phase 1 Complete When:**
- [ ] Parser supports string literals (or added if missing)
- [ ] STRING_LITERAL exists in AST
- [ ] `createStringLiteral()` method works
- [ ] String literals compile to LLVM global constants
- [ ] Multiple strings get unique IDs (.str.0, .str.1)
- [ ] UTF-8 strings work (Turkish characters: ü, ğ, ş)
- [ ] LLVM IR validates (`opt -verify` passes)
- [ ] Object files compile (`llc` succeeds)
- [ ] No compilation warnings
- [ ] No memory leaks (if valgrind available)

---

## 🚧 TROUBLESHOOTING

### Problem 1: Parser Doesn't Recognize Strings

**Symptom:**
```
Error: Unexpected token: "
```

**Solution:**
- Check lexer: Does it tokenize strings?
- Add token type: `TOKEN_STRING` or `T_STRING_LITERAL`
- Implement `parseStringLiteral()` in parser

**Time:** +2 hours

---

### Problem 2: LLVM CreateString Error

**Symptom:**
```
error: cannot convert 'std::string' to 'llvm::StringRef'
```

**Solution:**
```cpp
// WRONG:
llvm::ConstantDataArray::getString(context, node->value);

// CORRECT:
llvm::ConstantDataArray::getString(context, llvm::StringRef(node->value));
```

---

### Problem 3: Invalid GEP Instruction

**Symptom:**
```
error: GEP indices do not match pointer type
```

**Solution:**
```cpp
// Make sure to use correct types:
llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);

builder.CreateInBoundsGEP(
    globalStr->getValueType(),  // Use global's value type
    globalStr,
    {zero, zero}  // Two zeros: [array index 0][element index 0]
);
```

---

### Problem 4: UTF-8 Not Working

**Symptom:**
Turkish characters show as `???` or broken

**Solution:**
- Ensure source files are UTF-8 encoded
- Check lexer doesn't corrupt UTF-8 bytes
- LLVM automatically handles UTF-8, just pass bytes through

---

## 📚 REFERENCE CODE (Stage 0)

**Stage 0 String Handling:**
```bash
# Reference implementation
cat compiler/stage0/codegen/string_ops.cpp
```

**Key Functions to Study:**
- `createGlobalString()` - Global constant creation
- `handleStringLiteral()` - AST → LLVM conversion

---

## 🎯 NEXT PHASE

**When Phase 1 Complete:**
📄 **Move to [2-TODO_PHASE2_PRINTF_INTEGRATION.md](2-TODO_PHASE2_PRINTF_INTEGRATION.md)**

**You'll add:**
- External function declarations
- printf/wrapper function prototypes
- No implementation yet (just declarations)

---

## 📋 PHASE 1 CHECKLIST

Before moving to Phase 2:

**Code:**
- [ ] Parser handles string literals
- [ ] AST has STRING_LITERAL node type
- [ ] createStringLiteral() implemented
- [ ] compileExpression() handles STRING_LITERAL
- [ ] stringCounter increments correctly

**Testing:**
- [ ] Simple string compiles
- [ ] UTF-8 strings compile
- [ ] Multiple strings get unique IDs
- [ ] LLVM IR validates
- [ ] llc compiles to object file

**Quality:**
- [ ] No compiler warnings
- [ ] Code follows style guide
- [ ] Comments added
- [ ] No TODOs in code

**Documentation:**
- [ ] Changes documented
- [ ] Test cases written
- [ ] Known issues noted (if any)

---

**Ready? Start Task 1.1! 🚀**

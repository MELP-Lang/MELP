# Stage 0 to Stage 1 Transition Guide

**Self-Hosting Strategy** - Bootstrap compiler'dan .mlp derleyiciye geçiş planı.

---

## Overview

**Stage 0 (Current):** C++ bootstrap compiler  
**Stage 1 (Next):** MELP compiler written in MELP (.mlp)

**Goal:** Use Stage 0 to compile Stage 1, then use Stage 1 to compile itself.

---

## What We Have Now (Stage 0)

### Implemented Features ✅

- **Basic Types:** numeric, string, boolean, pointer
- **Control Flow:** if/else, while, for
- **Functions:** Declaration, calls, return
- **FFI:** C library integration via dlopen/dlsym
- **File I/O:** read_file, write_file
- **Manual Memory:** allocate, free, Vector<T>
- **Codegen:** LLVM IR generation
- **Testing:** 132+ comprehensive tests

### Limitations ⚠️

- **No RAII:** Must call `.free()` manually
- **No ownership:** No borrow checker
- **No traits:** No polymorphism
- **No async:** No coroutines
- **Basic generics:** Only Vector<T>
- **No modules:** Single-file compilation

---

## What We Need (Stage 1)

### Essential Features for Self-Hosting

#### 1. Struct Types ⭐

```mlp
struct Token {
    type: string,
    value: string,
    line: numeric
}

var tok = Token { type: "NUMBER", value: "42", line: 1 }
```

**Why:** Represent AST nodes, tokens, symbols

**Implementation:**
- Parser: struct definitions
- Codegen: LLVM struct types
- Memory: Manual allocation initially

#### 2. Enum Types ⭐

```mlp
enum TokenType {
    NUMBER,
    STRING,
    IDENTIFIER,
    PLUS,
    MINUS
}

var t = TokenType.NUMBER
```

**Why:** Represent token types, AST node types

**Implementation:**
- Parser: enum definitions
- Codegen: Integer constants
- Pattern matching: Basic switch/case

#### 3. String Operations ⭐

```mlp
var combined = string_concat("Hello", " World")
var part = string_slice("Hello", 0, 4)  -- "Hell"
var found = string_find("Hello", "ll")  -- 2
```

**Why:** Lexer needs string manipulation

**Implementation:**
- Runtime: string_ops.cpp
- Builtin functions: concat, slice, find
- Memory: Manual for now

#### 4. HashMap/Dict ⭐

```mlp
var symbols = HashMap<string, Symbol>.new()
symbols.insert("x", Symbol { name: "x", type: "numeric" })
var sym = symbols.get("x")
```

**Why:** Symbol table, scope management

**Implementation:**
- Runtime: hashmap_minimal.cpp
- Generic: HashMap<K, V>
- Collision: Chaining

#### 5. Better Error Reporting

```mlp
function report_error(line: numeric, message: string) {
    print("Error at line ")
    print(line)
    print(": ")
    print(message)
}
```

**Why:** User-friendly compiler errors

---

## Rewrite Strategy

### Phase 1: Core Data Structures

**Deliverable:** Struct/Enum support

```
TODO_STAGE1_STRUCT/
├── 1-YZ_STRUCT_BASIC.md     -- struct definition & construction
├── 2-YZ_STRUCT_ACCESS.md    -- field access (dot operator)
└── 3-YZ_STRUCT_NESTED.md    -- nested structs

TODO_STAGE1_ENUM/
├── 1-YZ_ENUM_BASIC.md       -- enum definition
└── 2-YZ_ENUM_MATCH.md       -- pattern matching (switch)
```

**Tests:** Create struct/enum test files

**Time Estimate:** 2-3 weeks

### Phase 2: String Library

**Deliverable:** String operations runtime

```
runtime/
├── string_ops.hpp           -- String manipulation API
└── string_ops.cpp           -- Implementation

modules/string/
├── string_parser.cpp        -- Parse string ops
└── string_codegen.cpp       -- Generate LLVM IR
```

**API:**
- `string_concat(a, b) -> string`
- `string_slice(s, start, end) -> string`
- `string_find(haystack, needle) -> numeric`
- `string_length(s) -> numeric`

**Time Estimate:** 1 week

### Phase 3: HashMap

**Deliverable:** Generic hashmap

```
runtime/
├── hashmap_minimal.hpp      -- HashMap<K,V> API
└── hashmap_minimal.cpp      -- Implementation

modules/hashmap/
├── hashmap_parser.cpp       -- Parse hashmap ops
└── hashmap_codegen.cpp      -- Generate LLVM IR
```

**API:**
- `HashMap<K,V>.new() -> HashMap<K,V>`
- `map.insert(key, value) -> void`
- `map.get(key) -> V`
- `map.contains(key) -> boolean`
- `map.free() -> void`

**Time Estimate:** 2 weeks

### Phase 4: Write Lexer in .mlp

**Deliverable:** lexer.mlp

```mlp
-- lexer.mlp
struct Token {
    type: TokenType,
    value: string,
    line: numeric
}

enum TokenType {
    NUMBER, STRING, IDENTIFIER,
    PLUS, MINUS, STAR, SLASH,
    EOF
}

function tokenize(source: string) -> Vector<Token> {
    var tokens = Vector<Token>.new()
    var pos = 0
    var line = 1
    
    while pos < string_length(source) {
        var ch = string_slice(source, pos, pos+1)
        
        if ch == "+" {
            tokens.append(Token { 
                type: TokenType.PLUS, 
                value: "+", 
                line: line 
            })
            pos = pos + 1
        }
        -- ... more token types
    }
    
    return tokens
}
```

**Compile:** `melp_stage0 lexer.mlp -o lexer_stage1`

**Test:** Feed MELP source code, verify tokens

**Time Estimate:** 2 weeks

### Phase 5: Write Parser in .mlp

**Deliverable:** parser.mlp

```mlp
-- parser.mlp
struct ASTNode {
    type: NodeType,
    value: string,
    children: Vector<ASTNode>
}

enum NodeType {
    PROGRAM, FUNCTION, BINARY_OP, LITERAL
}

function parse(tokens: Vector<Token>) -> ASTNode {
    var root = ASTNode { 
        type: NodeType.PROGRAM, 
        value: "", 
        children: Vector<ASTNode>.new() 
    }
    
    -- Recursive descent parsing
    -- ...
    
    return root
}
```

**Time Estimate:** 3 weeks

### Phase 6: Write Codegen in .mlp

**Deliverable:** codegen.mlp

```mlp
-- codegen.mlp
function generate_llvm_ir(ast: ASTNode) -> string {
    var ir = ""
    
    if ast.type == NodeType.FUNCTION {
        ir = string_concat(ir, "define i32 @")
        ir = string_concat(ir, ast.value)
        ir = string_concat(ir, "() {\n")
        -- ... generate body
        ir = string_concat(ir, "}\n")
    }
    
    return ir
}
```

**Time Estimate:** 4 weeks

### Phase 7: Integration

**Deliverable:** compiler.mlp (main entry point)

```mlp
-- compiler.mlp
function main() {
    var source = read_file("input.mlp")
    
    var tokens = tokenize(source)
    var ast = parse(tokens)
    var llvm_ir = generate_llvm_ir(ast)
    
    write_file("output.ll", llvm_ir)
    
    return 0
}
```

**Compile:** `melp_stage0 compiler.mlp -o melp_stage1`

**Bootstrap Test:** `melp_stage1 compiler.mlp -o melp_stage1_v2`

**Verify:** Compare melp_stage1 and melp_stage1_v2 binaries

**Time Estimate:** 2 weeks

---

## File Structure (Stage 1)

```
stage1/
├── lexer.mlp                -- Tokenizer
├── parser.mlp               -- Parser
├── codegen.mlp              -- LLVM IR generator
├── compiler.mlp             -- Main entry point
├── ast.mlp                  -- AST definitions
├── symbol_table.mlp         -- Symbol management
└── error.mlp                -- Error reporting

runtime/ (still C++)
├── builtins.cpp             -- Core runtime
├── heap_minimal.cpp         -- Memory allocation
├── vector_minimal.cpp       -- Vector<T>
├── string_ops.cpp           -- String operations (NEW)
└── hashmap_minimal.cpp      -- HashMap<K,V> (NEW)
```

---

## Compilation Pipeline

### Stage 0 (Bootstrap)

```
source.mlp → [melp_stage0] → output.ll → [llvm] → executable
```

**Compiler:** C++ (in `compiler/paradigma2/MODERN/`)

### Stage 1 (Self-Hosted)

```
source.mlp → [melp_stage1] → output.ll → [llvm] → executable
```

**Compiler:** MELP (.mlp files compiled by melp_stage0)

### Stage 2 (Optimized)

```
source.mlp → [melp_stage2] → output.ll → [llvm] → executable
```

**Compiler:** MELP (.mlp files compiled by melp_stage1)

**Features:** RAII, ownership, advanced optimizations

---

## Testing Strategy

### Incremental Validation

For each feature (struct, enum, string ops):

1. **Unit tests:** Test feature in isolation
2. **Integration tests:** Use in small programs
3. **Compiler tests:** Use in lexer/parser/codegen

### Bootstrap Verification

```bash
# Compile Stage 1 with Stage 0
./melp_stage0 compiler.mlp -o melp_stage1

# Compile Stage 1 with Stage 1 (self-hosting)
./melp_stage1 compiler.mlp -o melp_stage1_v2

# Compare binaries (should be identical)
diff melp_stage1 melp_stage1_v2
```

### Regression Suite

Run all 132+ Stage 0 tests against Stage 1 compiler to ensure compatibility.

---

## Timeline Estimate

| Phase | Duration | Status |
|-------|----------|--------|
| Struct/Enum | 3 weeks | ⏸️ Not started |
| String Ops | 1 week | ⏸️ Not started |
| HashMap | 2 weeks | ⏸️ Not started |
| Lexer | 2 weeks | ⏸️ Not started |
| Parser | 3 weeks | ⏸️ Not started |
| Codegen | 4 weeks | ⏸️ Not started |
| Integration | 2 weeks | ⏸️ Not started |
| **Total** | **17 weeks** | **~4 months** |

---

## Code Size Estimates

| Component | Lines of Code |
|-----------|---------------|
| lexer.mlp | ~500 LOC |
| parser.mlp | ~1000 LOC |
| codegen.mlp | ~1500 LOC |
| ast.mlp | ~300 LOC |
| symbol_table.mlp | ~400 LOC |
| compiler.mlp | ~200 LOC |
| **Total** | **~3900 LOC** |

Compare to Stage 0 C++: ~12,000 LOC

---

## Migration Checklist

### Before Starting Stage 1

- [x] FFI working
- [x] File I/O working
- [x] Manual memory working
- [x] Vector<T> working
- [ ] Struct support
- [ ] Enum support
- [ ] String operations
- [ ] HashMap<K,V>

### During Stage 1

- [ ] Write lexer.mlp
- [ ] Write parser.mlp
- [ ] Write codegen.mlp
- [ ] Test each component
- [ ] Integrate components
- [ ] Bootstrap test (compile self)

### Stage 1 Complete

- [ ] melp_stage1 can compile itself
- [ ] All Stage 0 tests pass on Stage 1
- [ ] Binary equivalence check passes

---

## Risk Mitigation

### Risk 1: Performance Issues

**Mitigation:** Profile with `perf`, optimize hot paths, consider JIT for Stage 2

### Risk 2: Memory Leaks in .mlp Compiler

**Mitigation:** Valgrind all .mlp programs, systematic free() discipline

### Risk 3: Bootstrap Divergence

**Mitigation:** Automated nightly bootstrap tests, binary comparison

### Risk 4: Feature Creep

**Mitigation:** Strict feature freeze, defer non-essential features to Stage 2

---

## Success Criteria

**Stage 1 is complete when:**

1. ✅ melp_stage1 binary exists
2. ✅ melp_stage1 compiles compiler.mlp successfully
3. ✅ Output of melp_stage0 and melp_stage1 are semantically equivalent
4. ✅ All 132+ tests pass on both compilers
5. ✅ Bootstrap cycle converges (melp_stage1 == melp_stage1_v2)

---

## Next Immediate Actions

### Week 1-2: Struct Implementation

1. Read `TODO_STAGE1_STRUCT/1-YZ_STRUCT_BASIC.md`
2. Implement struct parser (struct_parser.cpp)
3. Implement struct codegen (struct_codegen.cpp)
4. Create 10+ struct tests
5. Validate with existing tests

### Week 3: Enum Implementation

1. Read `TODO_STAGE1_ENUM/1-YZ_ENUM_BASIC.md`
2. Implement enum parser
3. Implement enum codegen
4. Create 10+ enum tests

### Week 4: String Operations

1. Create runtime/string_ops.cpp
2. Implement concat, slice, find, length
3. Add to builtins
4. Test string operations

**After Week 4:** Ready to start writing lexer.mlp!

---

## Resources

- **Current TODO:** See `TODO_STAGE1_STRUCT/` and `TODO_STAGE1_ENUM/`
- **Architecture:** See [ARCHITECTURE.md](ARCHITECTURE.md)
- **Stage 0 Limitations:** See [STAGE0_LIMITATIONS.md](STAGE0_LIMITATIONS.md)
- **Bootstrap Strategy:** See [BOOTSTRAP_STRATEGY.md](BOOTSTRAP_STRATEGY.md)

---

## Questions & Answers

**Q: Why not use Stage 0 compiler forever?**  
A: Self-hosting enables dogfooding, faster iteration, and community contributions in .mlp.

**Q: Can we skip Stage 1 and go to Stage 2?**  
A: No. Stage 1 validates the language design. Stage 2 adds advanced features on proven foundation.

**Q: How long until Stage 1 is faster than Stage 0?**  
A: Stage 1 will be slower initially. Stage 2 with optimizations should match C++ performance.

**Q: What if bootstrap fails?**  
A: Stage 0 remains as fallback. Fix bugs in Stage 1 until bootstrap succeeds.

---

**Version:** Stage 0 → Stage 1 Transition Plan  
**Date:** 3 Şubat 2026  
**Status:** Planning Phase  
**Estimated Completion:** Q2 2026

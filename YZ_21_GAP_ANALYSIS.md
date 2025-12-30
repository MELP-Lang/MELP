# YZ_21: Stage0 Gap Analysis Report

**Tarih:** 30 Aralık 2025  
**YZ:** YZ_21  
**Task:** 3.2 - Stage0 Gap Analysis  
**Süre:** 2 saat  

---

## 📊 EXECUTIVE SUMMARY

Stage0 compiler **kritik eksiklikler** içeriyor. Mevcut durum:
- ✅ **Parser:** Function signatures, basic control flow, variables (70% complete)
- ❌ **Parser:** Expression parsing STUB (8 lines!), nested expressions missing
- ⚠️ **Codegen:** Basic structure var, critical bugs mevcut
- ❌ **Codegen:** Function call generation missing, expression evaluation stub
- ⚠️ **Special Features:** Lambda/match ASM üretiyor (C'ye çevrilmeli)

**Stage1 kaynak derleyebilmek için gerekli MVP fixes:**
1. 🔴 **HIGH:** Expression parser (nested, operators, precedence)
2. 🔴 **HIGH:** Function call codegen (yazdir, mlp_list_*, etc.)
3. 🔴 **HIGH:** Variable codegen bug fix (int64_t y = 10)
4. 🟡 **MED:** Lambda ASM → C conversion
5. 🟡 **MED:** Match ASM → C conversion

---

## 🔍 DETAILED ANALYSIS

### A. PARSER MODULES

#### ✅ 1. Functions Parser (functions_parser.c - 225 lines)

**Capabilities:**
- ✅ Function declarations (`function name(param1, param2) -> return_type`)
- ✅ Parameter parsing with types
- ✅ Return type parsing
- ✅ Function body skeleton (skips to `end`)

**Gaps:**
- ❌ Function call parsing (parser stub exists, but incomplete)
- ❌ Generic function parsing (generics not handled)
- ❌ Default parameters
- ⚠️ Body parsing delegated to statement parser (needs integration)

**Stage1 Requirements:**
```mlp
function calculate_sum(a: i64, b: i64) as i64
    return a + b
end_func
```
- ✅ Signature parsing: **OK**
- ❌ Return expression: **MISSING** (expression parser stub)
- ❌ Function call: **MISSING**

**Priority:** 🟡 MEDIUM (signature OK, body needs expression parser)

---

#### ❌ 2. Expression Parser (expression_parser.c - 8 lines!!!)

**Capabilities:**
- ❌ **STUB ONLY!** Returns dummy EXPR_NUMBER

**Current Code:**
```c
Expression* expression_parse(Parser* parser) {
    // Stub - returns a simple number expression
    return expression_create(EXPR_NUMBER);
}
```

**Gaps:**
- ❌ Binary expressions (a + b, x * y)
- ❌ Nested expressions ((a + b) * c)
- ❌ Operator precedence
- ❌ Function calls as expressions (calculate(x, y))
- ❌ Array/list access (items[0])
- ❌ Member access (obj.field)

**Stage1 Requirements:**
```mlp
numeric x = (10 + 20) * 3
numeric y = calculate_sum(x, 5)
boolean flag = x > 100
```
- ❌ ALL MISSING - **CRITICAL BLOCKER!**

**Priority:** 🔴 **HIGH** (CRITICAL - blocks everything)

---

#### ⚠️ 3. Control Flow Parser (control_flow_parser.c - 150 lines)

**Capabilities:**
- ✅ If/else parsing (condition + structure)
- ✅ While loop parsing (condition)
- ✅ For loop parsing (iterator)
- ⚠️ **Body parsing skipped!** (advances to `end`)

**Gaps:**
- ❌ Statement body parsing (prints, assignments, calls inside if/while/for)
- ❌ Nested control flow (if inside while, etc.)
- ❌ Break/continue statements

**Current Code (if statement):**
```c
// Parse condition
stmt->condition = comparison_parse_expression(cmp);

// Skip body until 'else' or 'end if'
while (...) {
    if (token == TOKEN_ELSE) { /* skip */ }
    if (token == TOKEN_END) { break; }
    advance(parser);  // ← BODY IGNORED!
}
```

**Stage1 Requirements:**
```mlp
if x > 10 then
    yazdir("Large")  # ← BODY NEEDS PARSING
else
    yazdir("Small")
end_if
```
- ✅ Structure parsing: **OK**
- ❌ Body statements: **MISSING**

**Priority:** 🟡 MEDIUM (structure OK, body needs statement parser)

---

#### ✅ 4. Variable Parser (variable_parser.c - 274 lines)

**Capabilities:**
- ✅ Type declarations (numeric, text, boolean)
- ✅ Pointers (numeric*)
- ✅ Arrays (numeric[10])
- ✅ STO type inference (INT64, DOUBLE, SSO, HEAP)

**Gaps:**
- ⚠️ Initialization expressions delegated to expression parser
- ❌ Complex initializers (arrays, structs)

**Stage1 Requirements:**
```mlp
i64 count = 0
list items = []
```
- ✅ Type parsing: **OK**
- ⚠️ Expression init: **DEPENDS ON EXPRESSION PARSER**

**Priority:** ✅ LOW (mostly complete)

---

#### ⚠️ 5. Arithmetic Parser (arithmetic_parser.c - 268 lines)

**Capabilities:**
- ✅ Binary operators (+, -, *, /, %, **)
- ✅ Operator precedence (power > mul/div > add/sub)
- ✅ Parentheses
- ✅ Number literals and variables

**Gaps:**
- ❌ **NOT INTEGRATED!** (expression_parser.c doesn't call it)
- ⚠️ No function calls in expressions
- ⚠️ No array/member access

**Stage1 Requirements:**
```mlp
numeric result = (x + y) * 2
```
- ✅ Parser logic: **READY**
- ❌ Integration: **MISSING** (expression_parser.c stub)

**Priority:** 🔴 HIGH (ready but unused!)

---

#### ⚠️ 6. Lambda Parser (lambda_parser.c - 372 lines)

**Capabilities:**
- ✅ Capture list parsing ([x, &y])
- ✅ Parameter parsing
- ✅ Body parsing (delegated)

**Gaps:**
- ⚠️ Body parsing incomplete
- ❌ Higher-order function integration (map, filter)

**Stage1 Usage:** Minimal (only simple cases)
```mlp
items.map(lambda x -> x * 2)  # ← RARE in Stage1
```

**Priority:** 🟢 LOW (MVP doesn't need advanced lambdas)

---

#### ❌ 7. Switch/Match Parser (switch_match_parser.c - 30 lines)

**Capabilities:**
- ❌ **ALL STUBS!** (placeholder implementations)

**Gaps:**
- ❌ Switch parsing
- ❌ Match parsing
- ❌ Case parsing
- ❌ Range parsing

**Stage1 Usage:** Minimal
```mlp
match value {
    0 => yazdir("Zero")
    _ => yazdir("Other")
}
```

**Priority:** 🟢 LOW (MVP can skip advanced pattern matching)

---

### B. CODEGEN MODULES

#### 🔴 1. Expression Codegen (expression_codegen.c - 23 lines)

**Capabilities:**
- ⚠️ STUB - emits ASM comments only
- ⚠️ Handles EXPR_NUMBER/STRING/VARIABLE trivially

**Current Code:**
```c
void expression_generate_code(FILE* output, Expression* expr) {
    fprintf(output, "    # Expression code generation\n");
    
    switch (expr->type) {
        case EXPR_NUMBER:
            fprintf(output, "    movq $%ld, %%rax\n", ...);  // ← ASM!
            break;
        // ...
    }
}
```

**Gaps:**
- ❌ ASM output (should be C!)
- ❌ Binary expressions
- ❌ Function calls
- ❌ Complex expressions

**Stage1 Requirements:**
```c
// MLP: numeric x = 10 + 20
// NEED: int64_t var_x = 10 + 20;
```
- ❌ **ALL MISSING - CRITICAL!**

**Priority:** 🔴 **HIGH** (CRITICAL - ASM → C conversion + full implementation)

---

#### 🔴 2. Variable Codegen (variable_codegen.c - 161 lines)

**Capabilities:**
- ✅ STO type emission (int64_t, double, char*)
- ✅ Pointer/array handling
- ⚠️ **CRITICAL BUG:** Missing type prefix!

**Critical Bug:**
```c
// GENERATED CODE (WRONG):
Numeric = 10;  // ← MISSING TYPE!

// SHOULD BE:
int64_t y = 10;
```

**YZ_21 Test Output (from previous session):**
```c
// Variable: y (type: numeric)
// STO: INT64 optimization
Numeric = 10;  // ← BUG HERE!
```

**Root Cause (lines 106-115):**
```c
if (decl->internal_num_type == INTERNAL_INT64) {
    emit_c_indent(1, "// STO: INT64 optimization\n");
    if (decl->value) {
        emit_c_indent(1, "int64_t var_%s = %s;\n", decl->name, decl->value);
    } else {
        emit_c_indent(1, "int64_t var_%s = 0;\n", decl->name);  // ← THIS LINE WORKS
    }
}
```

**Bug Analysis:**
- Line 110 is correct: `int64_t var_%s = %s`
- But test output shows: `Numeric = 10`
- **Hypothesis:** Parser not setting `decl->value` or `decl->name` correctly?

**Priority:** 🔴 **HIGH** (CRITICAL BUG - fix immediately!)

---

#### 🟡 3. Function Codegen (functions_codegen.c - 113 lines)

**Capabilities:**
- ✅ Function signature generation (C)
- ✅ Parameter mapping (numeric → int64_t)
- ⚠️ **STUB BODY:** Returns placeholder

**Current Code:**
```c
void function_generate_declaration(FILE* output, FunctionDeclaration* func) {
    function_generate_prologue(output, func);
    emit_c("    // Function body goes here\n");  // ← STUB!
    function_generate_epilogue(output, func);
}
```

**Gaps:**
- ❌ Function body codegen (statement loop missing)
- ❌ Function call codegen (stub returns `arg0`, not real args)
- ❌ Return statement codegen (placeholder `return 0;`)

**Stage1 Requirements:**
```c
// MLP:
function add(a: i64, b: i64) as i64
    return a + b
end_func

// NEED:
int64_t add(int64_t a, int64_t b) {
    return a + b;  // ← MISSING!
}
```

**Priority:** 🔴 HIGH (signature OK, body/call/return missing)

---

#### ⚠️ 4. Control Flow Codegen (control_flow_codegen.c - 80 lines)

**Capabilities:**
- ✅ If/else C structure
- ✅ While loop C structure
- ✅ For loop C structure
- ⚠️ **BODY STUB:** Emits `// Body goes here`

**Current Code:**
```c
void control_flow_generate_if(FILE* output, IfStatement* stmt) {
    emit_c("    if (%s %s %s) {\n", left, op, right);
    emit_c("        // Then body\n");  // ← STUB!
    
    if (stmt->has_else) {
        emit_c("    } else {\n");
        emit_c("        // Else body\n");  // ← STUB!
    }
    emit_c("    }\n");
}
```

**Gaps:**
- ❌ Statement body generation (loop needed)
- ⚠️ Depends on statement codegen integration

**Stage1 Requirements:**
```c
// MLP:
if x > 10 then
    yazdir("Large")
end_if

// NEED:
if (var_x > 10) {
    printf("Large\n");  // ← MISSING!
}
```

**Priority:** 🟡 MEDIUM (structure OK, body needs integration)

---

#### ⚠️ 5. Lambda Codegen (lambda_codegen.c - 161 lines)

**Capabilities:**
- ✅ Closure struct generation
- ✅ Capture mechanism
- ⚠️ **ASM OUTPUT!** (should be C)

**Current Code:**
```c
void codegen_lambda(FILE* out, Lambda* lambda) {
    fprintf(out, "    lea rax, [%s]    ; Lambda adresi\n", name);  // ← ASM!
    fprintf(out, "    mov [rbp - 8], rax    ; Stack'e kaydet\n");  // ← ASM!
}
```

**Gaps:**
- ❌ ASM → C conversion (function pointers, struct)
- ⚠️ Higher-order functions (map/filter) call runtime (OK)

**Stage1 Usage:** Minimal
```c
// MLP:
items.map(lambda x -> x * 2)

// NEED (C with function pointers):
mlp_map(items, lambda_0);  // ← NEEDS C IMPLEMENTATION
```

**Priority:** 🟡 MEDIUM (ASM → C conversion, but low Stage1 usage)

---

#### ⚠️ 6. Switch/Match Codegen (switch_match_codegen.c - 204 lines)

**Capabilities:**
- ✅ Jump table optimization
- ✅ Linear search fallback
- ⚠️ **ASM OUTPUT!** (should be C)

**Current Code:**
```c
void codegen_switch(FILE* out, Switch* sw) {
    fprintf(out, "    mov rax, [rbp - 8]    ; Switch değeri\n");  // ← ASM!
    fprintf(out, "    jmp [rbx]    ; İndirect jump\n");  // ← ASM!
}
```

**Gaps:**
- ❌ ASM → C conversion (switch statement)
- ⚠️ Pattern matching needs C emulation

**Stage1 Usage:** Minimal

**Priority:** 🟢 LOW (MVP can use if-else instead)

---

#### ✅ 7. Print Codegen (print_codegen.c - 44 lines)

**Capabilities:**
- ✅ C printf generation
- ✅ String literal handling
- ✅ Main function wrapper

**Stage1 Requirements:**
```c
// MLP: yazdir("Hello")
// C:   printf("Hello\n");  // ← WORKS!
```

**Priority:** ✅ COMPLETE (no gaps!)

---

## 📋 GAP MATRIX

| Feature | Stage1 Needs | Stage0 Parser | Stage0 Codegen | Gap | Priority |
|---------|--------------|---------------|----------------|-----|----------|
| **Core Features** |
| Function declarations | ✅ Yes | ✅ Complete | ✅ Signature OK | Body stub | 🟡 MED |
| Variable declarations | ✅ Yes | ✅ Complete | 🔴 **BUGGY** | Type prefix missing | 🔴 HIGH |
| Expression parsing | ✅ Yes | 🔴 **STUB (8 lines!)** | 🔴 ASM stub | Full impl needed | 🔴 HIGH |
| Binary expressions | ✅ Yes | ⚠️ Arithmetic ready | ❌ Not integrated | Integration | 🔴 HIGH |
| Function calls | ✅ Yes | ⚠️ Partial | ❌ Stub | Full impl | 🔴 HIGH |
| Control flow (if/while/for) | ✅ Yes | ✅ Structure OK | ⚠️ Body stub | Body codegen | 🟡 MED |
| Print statements | ✅ Yes | ✅ Complete | ✅ Complete | None | ✅ DONE |
| **Advanced Features** |
| Lambda expressions | ⚠️ Simple | ✅ Parse OK | ⚠️ ASM only | ASM → C | 🟡 MED |
| Pattern matching | ⚠️ Simple | ❌ Stub | ⚠️ ASM only | ASM → C | 🟢 LOW |
| Nested control flow | ✅ Yes | ⚠️ Skipped | ⚠️ Partial | Recursive | 🟡 MED |
| Generic functions | ❌ No (Stage2+) | ❌ None | ❌ None | Not needed | ⚪ N/A |
| **Integration** |
| Parser → AST | ✅ Yes | ⚠️ Partial | - | Full AST | 🟡 MED |
| AST → C Codegen | ✅ Yes | - | ⚠️ Partial | Full pipeline | 🔴 HIGH |
| Statement loop | ✅ Yes | ⚠️ Basic | ⚠️ Basic | Full loop | 🟡 MED |

---

## 🎯 CRITICAL PATH FOR MVP SELF-HOSTING

### Phase 1: Fix Critical Blockers (2-3 days)

**Task 3.4: Expression Parser + Codegen**
1. ✅ Integrate `arithmetic_parser.c` into `expression_parser.c`
2. ✅ Add function call parsing (parse_function_call)
3. ✅ Add array/member access parsing
4. ✅ Implement full expression codegen (C output)
5. ✅ Test: `numeric x = (10 + 20) * 3`

**Task 3.5: Function Call Codegen**
1. ✅ Parse function calls with arguments
2. ✅ Generate C function calls: `printf(...)`, `mlp_list_create()`
3. ✅ Handle return values
4. ✅ Test: `yazdir("Hello")`, `calculate(x, y)`

**Task 3.6: Variable Codegen Bug Fix**
1. 🔴 Debug why `int64_t var_y = 10` becomes `Numeric = 10`
2. ✅ Fix emit_c_indent format string or parser data
3. ✅ Test: All variable types (i64, f64, text, bool)

**Deliverable:** Basic MLP programs compile to C
```mlp
function main() as i64
    i64 x = 10
    i64 y = 20
    i64 sum = x + y
    yazdir("Sum calculated")
    return sum
end_func
```

---

### Phase 2: Statement Body Integration (1-2 days)

**Task 3.7: Control Flow Body Codegen**
1. ✅ Implement statement loop in control_flow_codegen
2. ✅ Generate body statements (print, assign, call, return)
3. ✅ Test nested control flow

**Task 3.8: Function Body Codegen**
1. ✅ Implement statement loop in function codegen
2. ✅ Generate return statements with expressions
3. ✅ Test multi-statement functions

**Deliverable:** Stage1 simple functions compile
```mlp
function calculate_sum(a: i64, b: i64) as i64
    if a > 0 then
        yazdir("Positive")
    end_if
    return a + b
end_func
```

---

### Phase 3: Lambda/Match Conversion (1-2 days)

**Task 3.9: Lambda ASM → C**
1. ⚠️ Convert closure to C struct
2. ⚠️ Convert lambda to function pointer
3. ⚠️ Test simple lambdas

**Task 3.10: Match ASM → C** (Optional for MVP)
1. ⚠️ Convert to C switch/if-else
2. ⚠️ Test basic patterns

**Deliverable:** Stage1 compiles (may skip advanced features)

---

## 🚨 RISK ASSESSMENT

### 🔴 CRITICAL RISKS

1. **Expression Parser Stub (8 lines!)**
   - **Impact:** BLOCKS ALL EXPRESSIONS
   - **Mitigation:** Integrate arithmetic_parser.c (ready!)
   - **Timeline:** 1 day

2. **Variable Codegen Bug**
   - **Impact:** Invalid C code (won't compile)
   - **Mitigation:** Debug emit_c_indent or parser data flow
   - **Timeline:** 2-4 hours

3. **Function Call Missing**
   - **Impact:** Can't call yazdir(), mlp_list_*, etc.
   - **Mitigation:** Implement call codegen with arg evaluation
   - **Timeline:** 1 day

### 🟡 MEDIUM RISKS

4. **Statement Body Integration**
   - **Impact:** Empty function/control flow bodies
   - **Mitigation:** Loop through statement list, call respective codegen
   - **Timeline:** 1 day

5. **ASM → C Conversion (Lambda/Match)**
   - **Impact:** Stage1 uses these features (moderate)
   - **Mitigation:** May defer to Stage2, use if-else workaround
   - **Timeline:** 2 days (or skip for MVP)

### 🟢 LOW RISKS

6. **Nested Control Flow**
   - **Impact:** Stage1 has limited nesting (manageable)
   - **Mitigation:** Recursive codegen (straightforward)
   - **Timeline:** 4 hours

---

## 📊 EFFORT ESTIMATION

| Task | Description | Effort | Priority | Dependencies |
|------|-------------|--------|----------|--------------|
| 3.4 | Expression Parser + Codegen | 8h | 🔴 HIGH | None |
| 3.5 | Function Call Codegen | 6h | 🔴 HIGH | Task 3.4 |
| 3.6 | Variable Bug Fix | 2h | 🔴 HIGH | None |
| 3.7 | Control Flow Body | 6h | 🟡 MED | Task 3.4 |
| 3.8 | Function Body | 4h | 🟡 MED | Task 3.4, 3.5 |
| 3.9 | Lambda ASM → C | 8h | 🟡 MED | Task 3.4 |
| 3.10 | Match ASM → C | 6h | 🟢 LOW | Optional |

**Total (MVP):** 26-34 hours → **3-4 days** (with testing)

---

## ✅ RECOMMENDATIONS

### Immediate Actions (Day 1):

1. **Fix Variable Bug** (2h)
   ```bash
   cd MELP/C/stage0/modules/variable
   # Debug variable_codegen.c lines 106-115
   # Test: numeric y = 10 → int64_t var_y = 10;
   ```

2. **Integrate Arithmetic Parser** (6h)
   ```bash
   cd MELP/C/stage0/modules/expression
   # Modify expression_parser.c to call arithmetic_parser
   # Test: numeric x = (10 + 20) * 3
   ```

3. **Implement Expression Codegen** (2h)
   ```bash
   # Modify expression_codegen.c (ASM → C)
   # Use emit_c() for C code
   # Test: same as above
   ```

### Week 1 Plan:

**Day 1-2:** Tasks 3.4, 3.6 (expression + variable fix)  
**Day 3:** Task 3.5 (function call codegen)  
**Day 4:** Task 3.7 (control flow body)  
**Day 5:** Task 3.8 (function body)  
**Day 6-7:** Task 3.9 (lambda, optional)

### MVP Scope (Revised):

**MUST HAVE (Stage2 binary):**
- ✅ Variable declarations
- ✅ Function declarations/calls
- ✅ Binary expressions
- ✅ Control flow (if/while/for)
- ✅ Print statements

**NICE TO HAVE (defer if needed):**
- ⚠️ Lambda expressions (simple)
- ⚠️ Pattern matching (basic)
- ⚠️ Generic functions (Stage2+)

**OUT OF SCOPE (Stage3+):**
- ❌ Advanced generics
- ❌ Trait system
- ❌ Macro expansion

---

## 📝 NEXT STEPS

1. ✅ Update TODO #3 in 0-TODO_SELFHOSTING.md (mark Task 3.2 complete)
2. ✅ Create implementation plan (YZ_21_IMPLEMENTATION_PLAN.md)
3. ✅ Start Task 3.4 (expression parser integration)
4. ✅ Daily commits with test validation

---

**Status:** Task 3.2 COMPLETE ✅  
**Next:** Task 3.3 (Implementation Plan) → UA approval → Task 3.4 (coding begins!)


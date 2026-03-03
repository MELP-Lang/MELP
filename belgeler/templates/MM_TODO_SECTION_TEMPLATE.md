# MM TODO SECTION TEMPLATE

**Template Version:** 1.0  
**Purpose:** Standardized TODO section with syntax reference

---

## [MODULE_NAME] - [BRIEF_DESCRIPTION]

**Priority:** [P0/P1/P2]  
**Estimated Time:** [TIME_ESTIMATE]  
**Stage:** [STAGE_NUMBER]  
**Dependencies:** [NONE or LIST]

---

### 📖 Syntax Reference

**Stage:** [STAGE_NAME] (e.g., pmlp0)  
**Feature Card:** [belgeler/language/feature_cards/FEATURE_NAME.md](../belgeler/language/feature_cards/FEATURE_NAME.md)

**Quick Syntax:**
```pmpl
[PASTE RELEVANT SYNTAX - 5-15 lines max!]
```

**Critical:**
- ✅ [KEY_RULE_1]
- ✅ [KEY_RULE_2]
- ❌ [ANTI_PATTERN_1]

---

### 🎯 Implementation

**Files to modify:**
- `[FILE_PATH_1]` - [WHAT_TO_DO]
- `[FILE_PATH_2]` - [WHAT_TO_DO]

**Steps:**
1. [STEP_1_DESCRIPTION]
2. [STEP_2_DESCRIPTION]
3. [STEP_3_DESCRIPTION]

**Code Pattern:**
```[language]
[CODE_EXAMPLE]
```

---

### ✅ Success Criteria

- [ ] [CRITERION_1]
- [ ] [CRITERION_2]
- [ ] [CRITERION_3]

---

### 📝 Notes

[Any additional context or warnings]

---

## EXAMPLE USAGE (Delete this section when using template)

---

## While Loop Parser - Implement while loop parsing

**Priority:** P0  
**Estimated Time:** 2 hours  
**Stage:** Stage 0  
**Dependencies:** None

---

### 📖 Syntax Reference

**Stage:** pmlp0  
**Feature Card:** [belgeler/language/feature_cards/while_loop.md](../belgeler/language/feature_cards/while_loop.md)

**Quick Syntax:**
```pmpl
while condition
    -- body
end_while
```

**Critical:**
- ✅ `while condition` (NO `do` keyword!)
- ✅ `end_while` (underscore, single token)
- ❌ `while condition do` (WRONG!)
- ❌ `while (condition)` (no parentheses needed)

---

### 🎯 Implementation

**Files to modify:**
- `compiler/parser/parser.cpp` - Add `parse_while_statement()`
- `compiler/parser/parser.h` - Add method declaration

**Steps:**
1. Check for `TOKEN_WHILE`
2. Parse boolean condition expression
3. Parse loop body until `TOKEN_END_WHILE`
4. Create WhileNode AST

**Code Pattern:**
```cpp
ASTNode* Parser::parse_while_statement() {
    expect(TOKEN_WHILE);
    ASTNode* condition = parse_expression();
    // Parse body
    expect(TOKEN_END_WHILE);
    return new WhileNode(condition, body);
}
```

---

### ✅ Success Criteria

- [ ] Parses `while condition` without `do`
- [ ] Recognizes `end_while` as single token
- [ ] Generates correct AST
- [ ] Test case passes

---

### 📝 Notes

Remember: NO `do` keyword in MELP while loops!

---

**Template Usage:**
1. Copy this template for each TODO section
2. Fill in ALL bracketed placeholders [LIKE_THIS]
3. Include 5-15 lines of syntax (need-to-know only!)
4. Always reference feature card
5. Keep it SHORT and ACTIONABLE

**Golden Rule:** Developer should understand syntax in 30 seconds, NOT 30 minutes!

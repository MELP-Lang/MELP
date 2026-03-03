# YZ BRIEFING TEMPLATE

**Template Version:** 1.0  
**Date:** [DATE]  
**Purpose:** Standardized YZ briefing with syntax reference

---

## 📋 GÖREV ÖZETİ

**Görev ID:** [YZ_ID]  
**Görev Adı:** [TASK_NAME]  
**Süre:** [ESTIMATED_TIME]  
**Stage:** [STAGE_NUMBER]

---

## 🎯 GÖREVİN AMACI

[Brief description of what needs to be implemented]

---

## 📖 SYNTAX REFERENCE (MANDATORY!)

> **⚠️ CRITICAL:** Bu görev için SADECE aşağıdaki syntax'ı kullan!

### **Stage:** [STAGE_NAME] (e.g., pmlp0, pmlp1)

### **Feature Card(s):**
- [belgeler/language/feature_cards/FEATURE_NAME.md](belgeler/language/feature_cards/FEATURE_NAME.md)

### **Quick Syntax Reference:**

```pmpl
[PASTE RELEVANT SYNTAX HERE - 10-30 lines max!]
```

### **Critical Rules:**
- ✅ [RULE_1]
- ✅ [RULE_2]
- ✅ [RULE_3]
- ❌ [ANTI_PATTERN_1]
- ❌ [ANTI_PATTERN_2]

### **Anti-Patterns (AVOID!):**

```[language]
-- ❌ WRONG: [LANGUAGE_NAME] style
[CODE_EXAMPLE]
```

```pmpl
-- ✅ CORRECT: MELP style
[CODE_EXAMPLE]
```

---

## 📋 DELİVERABLES

**Dosyalar:**
1. `[FILE_PATH_1]` - [DESCRIPTION]
2. `[FILE_PATH_2]` - [DESCRIPTION]
3. `[FILE_PATH_3]` - [DESCRIPTION]

**Test Cases:**
- [ ] [TEST_CASE_1]
- [ ] [TEST_CASE_2]
- [ ] [TEST_CASE_3]

---

## 🛠️ IMPLEMENTATION GUIDE

### **Step 1:** [STEP_DESCRIPTION]
```[language]
[CODE_EXAMPLE]
```

### **Step 2:** [STEP_DESCRIPTION]
```[language]
[CODE_EXAMPLE]
```

### **Step 3:** [STEP_DESCRIPTION]
```[language]
[CODE_EXAMPLE]
```

---

## ⚠️ CRITICAL REQUIREMENTS

1. **[REQUIREMENT_1]**
   - [DETAILS]

2. **[REQUIREMENT_2]**
   - [DETAILS]

3. **[REQUIREMENT_3]**
   - [DETAILS]

---

## 🔗 DEPENDENCIES

**Requires:**
- [DEPENDENCY_1]
- [DEPENDENCY_2]

**Blocks:**
- [BLOCKED_TASK_1]
- [BLOCKED_TASK_2]

---

## 📚 REFERENCE DOCUMENTS

**Syntax:**
- [belgeler/language/pmlp0_syntax.md](belgeler/language/pmlp0_syntax.md) - Stage 0 minimal
- [belgeler/pmlp_sozdizimi.md](belgeler/pmlp_sozdizimi.md) - Full spec (1994 lines - use only if needed!)

**Architecture:**
- [ARCHITECTURE_DOC_1]
- [ARCHITECTURE_DOC_2]

**Golden Reference:**
- [belgeler/test_pmlp_golden_reference.mlp](belgeler/test_pmlp_golden_reference.mlp) - PMPL examples

---

## ✅ DEFINITION OF DONE

- [ ] Implementation complete
- [ ] All tests passing
- [ ] Code follows MELP syntax (feature cards validated)
- [ ] Documentation updated
- [ ] Git commit message prepared
- [ ] Completion report ready

---

## 📝 NOTES

[Any additional notes, warnings, or context]

---

**Template Usage:**
1. Copy this template for each new YZ briefing
2. Fill in ALL bracketed placeholders [LIKE_THIS]
3. Include relevant syntax from feature cards (10-30 lines max!)
4. Always add anti-patterns section
5. Keep syntax reference SHORT and FOCUSED (need-to-know basis!)

**Golden Rule:** YZ should spend 2 minutes reading syntax, NOT 2 hours searching through 1994-line docs!

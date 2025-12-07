# COMPILER ARCHITECTURE RULES

## 🚨 ABSOLUTE RULES (AI: YOU CANNOT BREAK THESE!)

### Rule 1: FILE SIZE LIMITS
```
main.c          ≤ 50 lines   (only main function)
orchestrator.c  ≤ 200 lines  (only module routing)
```
**IF EXCEEDED**: Extract logic to `modules/` subdirectory

### Rule 2: ORCHESTRATOR.C = ROUTING ONLY
```c
// ✅ ALLOWED:
case TOKEN_PRINT:
    print_module_handle(token, context);
    break;

// ❌ FORBIDDEN:
case TOKEN_PRINT:
    if (token->next->type == TOKEN_STRING) {
        // 50 lines of logic  ← NO! Put in modules/print/
    }
    break;
```

### Rule 3: NEW FEATURE CHECKLIST
Before writing code, ask:
- [ ] Does this logic belong in a module? (99% yes)
- [ ] Am I editing orchestrator.c? (if adding >5 lines: STOP!)
- [ ] Does this duplicate existing code? (refactor first!)

### Rule 4: MODULE STRUCTURE
```
modules/
  feature_name/
    ├── feature_parser.c   (parsing logic)
    ├── feature_codegen.c  (code generation)
    ├── feature.h          (public interface)
    └── feature.c          (utilities)
```

### Rule 5: DRY - DON'T REPEAT YOURSELF
- **1st time**: Write code
- **2nd time**: Extract to function
- **3rd time**: YOU FAILED - immediate refactor required!

## 🎯 QUICK DECISION TREE

```
New feature?
  ├─ Does it parse tokens? → modules/[feature]/[feature]_parser.c
  ├─ Does it generate code? → modules/[feature]/[feature]_codegen.c
  ├─ Is it a statement? → modules/statement/
  ├─ Is it control flow? → modules/control_flow/
  └─ Is it an expression? → modules/expression/
```

## 📏 METRICS
- Cyclomatic Complexity per function: ≤ 10
- Function length: ≤ 50 lines
- File length: ≤ 500 lines (modules can be larger if cohesive)

## 🔥 EMERGENCY REFACTOR TRIGGERS
If you see ANY of these, STOP and refactor:
- [ ] Same code block in 2+ places
- [ ] Function >100 lines
- [ ] orchestrator.c >200 lines
- [ ] switch/case with >20 lines per case

---
**Last updated**: 2025-12-07
**Enforced by**: Pre-commit hook + Manual review

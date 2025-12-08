# COMPILER ARCHITECTURE RULES

## 🚨 ABSOLUTE RULES (AI: YOU CANNOT BREAK THESE!)

### Rule 1: NO CENTRAL COORDINATION FILES
```
❌ FORBIDDEN FILES (DELETED 7 Aralık 2025):
- main.c          (DELETED - was 801 lines!)
- orchestrator.c  (DELETED - was 251 lines!)
- helpers.c       (DELETED - was 72 lines!)
- router.c        (NEVER CREATE!)
- coordinator.c   (NEVER CREATE!)
- pipeline.c      (NEVER CREATE!)
```

**Backups only:** `temp/yedek_merkezi_dosyalar/` (emergency reference)

**IF YOU NEED COORDINATION**: Use chained imports! (#include "../module/")

### Rule 2: CHAINED IMPORTS = ONLY ARCHITECTURE
```c
// ✅ CORRECT:
// modules/functions/functions_parser.c
#include "../statement/statement_parser.h"

void parse_function_body(...) {
    statement_parse(...);  // Direct call!
}

// ❌ FORBIDDEN:
// compiler/stage0/orchestrator.c
#include "modules/functions/functions.h"
#include "modules/statement/statement.h"

void coordinate() {  // NO! This is anti-pattern!
    functions_handle(...);
    statement_handle(...);
}
```

### Rule 3: NEW FEATURE CHECKLIST
Before writing code, ask:
- [ ] Does this use chained imports? (99% yes)
- [ ] Am I creating a central coordinator? (if yes: STOP!)
- [ ] Is this in `modules/[module_name]/`? (must be yes!)
- [ ] Does this duplicate existing code? (refactor first!)

### Rule 4: MODULE STRUCTURE (Chained Imports)
```
modules/
  feature_name/
    ├── feature_parser.c   (parsing logic, imports other parsers)
    ├── feature_codegen.c  (code generation, imports other codegen)
    ├── feature.h          (public interface)
    └── feature.c          (utilities)

Example chained import:
  functions_parser.c → statement_parser.c → control_flow_parser.c
```

### Rule 5: DRY - DON'T REPEAT YOURSELF
- **1st time**: Write code
- **2nd time**: Extract to function
- **3rd time**: YOU FAILED - immediate refactor required!

## 🎯 QUICK DECISION TREE

```
New feature?
  ├─ Does it parse tokens? → modules/[feature]/[feature]_parser.c (import what you need!)
  ├─ Does it generate code? → modules/[feature]/[feature]_codegen.c (import what you need!)
  ├─ Needs statement parsing? → #include "../statement/statement_parser.h"
  ├─ Needs control flow? → #include "../control_flow/control_flow_parser.h"
  └─ Needs comparison? → #include "../comparison/comparison_parser.h"

NEVER create coordinator to call these! Import directly!
```

## 📏 METRICS
- Cyclomatic Complexity per function: ≤ 10
- Module size: ≤ 300 lines (target: <200)
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

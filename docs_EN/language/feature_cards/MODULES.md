# Module System - MeLP Syntax

**Status:** ✅ COMPLETE (Stage1)
**Syntax Version:** mlp1 / Stage1

---

## 📖 BASIC SYNTAX

```mlp
-- Export from a module
export numeric function add(numeric a; numeric b)
    return a + b
end function

-- Import a module
import ./math
```

---

## ✅ COMPLETE EXAMPLES

### Example 1 — Export Functions (math.mlp)
```mlp
export numeric function add(numeric a; numeric b)
    return a + b
end function

export numeric PI = 3
```

### Example 2 — Import and Use (main.mlp)
```mlp
import ./math

function main()
    numeric result = add(5; 10)
    print(result)
end function
-- Output: 15
```

### Example 3 — Relative Paths
```mlp
import ./helper
import ../utils/math
import stdlib/math_utils
```

---

## ⚠️ CRITICAL RULES

1. Use `export` keyword to expose functions/structs/enums from a module.
2. `import ./module_name` — path without `.mlp` extension.
3. Return type before `function` keyword: `numeric function add(...)`
4. No circular imports allowed.
5. Each module is loaded only once (deduplication).

---

## ⏳ NOT YET IMPLEMENTED

- Selective imports: `import math.{add; PI}`
- Module aliases: `import math as m`
- Qualified names: `math.add(5; 10)`

---

## 🚫 WRONG USAGE

```mlp
-- ❌ WRONG: -> return type in exported function
export function add(numeric a; numeric b) -> numeric
    return a + b
end function
```

```mlp
-- ✅ CORRECT: return type before function keyword
export numeric function add(numeric a; numeric b)
    return a + b
end function
```

---

## 🔗 RELATED CARDS

- [IMPORT.md](IMPORT.md) — import system details
- [FUNCTION_DEF.md](FUNCTION_DEF.md) — function definitions

# MODULES - Özellik Kartı

**Özellik:** Module System (import/export)  
**Durum:** ✅ IMPLEMENTED (Stage 1 - Week 10.1)  
**Sözdizimi Sürümü:** pmlp v1.0

---

## 🎯 SÖZDİZİMİ REFERANSI

### Export  
```mlp
-- math.mlp
export function add(numeric a; numeric b) as numeric
  return a + b
end_function

export numeric PI = 3.14159
```

### Import
```mlp
-- main.mlp
import ./math

function main() as numeric
  return add(5; 10)  -- Functions from imported module available
end_function
```

### Path-based Import
```mlp
-- Relative import
import ./helper
import ../utils/math

-- Absolute/path-based
import stdlib/math_utils
```

---

## 📝 GRAMER KURALLARI

```ebnf
import_stmt ::= "import" module_path

module_path ::= relative_path | absolute_path

relative_path ::= "." "/" identifier ("/" identifier)*
                | ".." "/" identifier ("/" identifier)*

absolute_path ::= identifier ("/" identifier)*

export_decl ::= "export" (function_decl | struct_decl | enum_decl)
```

---

## ✅ IMPLEMENTATION STATUS

**Implemented Features:**
- ✅ `import ./module` - Relative imports
- ✅ `import ../utils/math` - Parent directory imports
- ✅ `import stdlib/math_utils` - Path-based imports
- ✅ `export function/struct/enum` - Export declarations
- ✅ Circular dependency detection
- ✅ Module deduplication (each module loaded once)
- ✅ Function namespace merging

**Not Yet Implemented:**
- ⏳ Selective imports: `import math.{add, PI}`
- ⏳ Module aliases: `import math as m`
- ⏳ Qualified names: `math.add(5; 10)`
- ⏳ Variable/constant exports

**Phase:** Stage 1 - Week 10.1 (21 Feb 2026)

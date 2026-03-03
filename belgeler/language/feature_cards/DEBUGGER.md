# DEBUGGER - Özellik Kartı

**Özellik:** Interactive Debugger  
**Durum:** 🔴 TODO (Phase 4.3)  
**Sözdizimi Sürümü:** N/A (Debugger protocol)

---

## 🎯 ÖZELLIKLER

### Breakpoints
```mlp
function main() -> numeric
  numeric x = 10  -- [BREAKPOINT]
  numeric y = 20
  return x + y
end_function
```

### Step Control
- **Step Over**: Execute current line
- **Step Into**: Enter function call
- **Step Out**: Exit current function
- **Continue**: Run to next breakpoint

### Watch Variables
```
Watch: x = 10
Watch: y = 20
Watch: x + y = 30
```

### Call Stack
```
#0 add() at line 5
#1 main() at line 10
```

---

**Phase:** 4.3 (Month 7)

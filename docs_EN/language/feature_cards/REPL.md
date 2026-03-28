# REPL - Özellik Kartı

**Özellik:** Read-Eval-Print Loop  
**Durum:** 🔴 TODO (Phase 4.3)  
**Sözdizimi Sürümü:** Interactive shell

---

## 🎯 KULLANIM

### Interactive Mode
```bash
$ mlp repl
MLP v1.0 REPL
>>> numeric x = 10
>>> numeric y = 20
>>> x + y
30
>>> numeric function add(numeric a; numeric b)
...   return a + b
... end function
>>> add(5; 10)
15
```

### Multi-line Input
```mlp
>>> numeric function factorial(numeric n)
...   if n <= 1 then
...     return 1
...   end if
...   return n * factorial(n - 1)
... end function
>>> factorial(5)
120
```

### Special Commands
```
:help    - Show help
:clear   - Clear screen
:vars    - Show variables
:exit    - Exit REPL
```

---

**Phase:** 4.3 (Month 7)

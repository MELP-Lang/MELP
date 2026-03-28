# Defer - MeLP Syntax

**Status:** 📅 PLANNED (mlp2 — not yet in Stage1)
**Syntax Version:** mlp2 (planned)

---

## 📖 BASIC SYNTAX

`defer` marks a statement to run automatically when the current function returns (on success or error). Used for resource cleanup.

```mlp
function process_file(string path)
    string f = open_file(path)
    defer f.close()        -- runs when function returns
    string content = f.read()
    print(content)
end function
```

---

## ✅ PLANNED EXAMPLES

### Example 1 — Basic Defer
```mlp
numeric function process_file(string path)
    string f = open_file(path)
    defer f.close()
    string content = f.read()
    return content.length
end function
-- f.close() runs here automatically
```

### Example 2 — Multiple Defers (LIFO order)
```mlp
numeric function multi_resource()
    string f1 = open("a.txt")
    defer f1.close()   -- runs 3rd

    string f2 = open("b.txt")
    defer f2.close()   -- runs 2nd

    string lock = acquire_lock()
    defer lock.release()  -- runs 1st (last-in, first-out)

    return 0
end function
```

### Example 3 — Defer on Early Return
```mlp
numeric function safe_write(string path; string data)
    string f = open_file(path)
    defer f.close()  -- runs even on early return

    if data.length == 0 then
        return -1    -- defer triggers here too
    end if

    f.write(data)
    return 0
end function
```

---

## ⚠️ CRITICAL RULES

1. `defer` statement runs when the **enclosing function** returns.
2. Multiple defers execute in **LIFO** (last-in, first-out) order.
3. Defer runs on **all** return paths, including early returns and errors.
4. This feature is planned for mlp2 and is **not yet in Stage1**.

---

## 🔗 RELATED CARDS

- [ERROR_HANDLING.md](ERROR_HANDLING.md) — try/finally alternative
- [FILE_IO.md](FILE_IO.md) — file operations that need cleanup

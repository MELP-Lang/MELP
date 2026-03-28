# MELP Sugar Syntax Önerileri - Görev 01 ile Başlayan Keşifler

> Bu dosya, her görevde keşfedilen ve denenen sugar syntax önerilerini belgelemektedir.
> **Durum Konvansiyon:**
> - ✅ **Compiler destekliyor** — Test edildiki, çalışmakta
> - ❌ **Normalizer gerektirir** — Compiler desteklemiyor, normalizer layer'ı gerekli
> - ⚠️ **Test edilmedi** — Keşfedildi ama test edilecek

---

## Görev 01: hello_world

### 1️⃣ program bloğu — Giriş Noktası Sugar

**Canonical .mlp:**
```mlp
function main()
    print("Merhaba, MELP!")
end function
```

**Sugar (ONAYLANDI — ÜA kararı 2026-03-03):**
```mlp
program
    print("Merhaba, MELP!")
end_program
```

**Gerekçe:**
- `function main() as numeric` + `return 0` C kalıntısı — MLP'de YASAK
- Canonical form zaten `function main()` — dönüş tipi ve return 0 yok
- `program` / `end_program` MELP'in `end_xxx` pattern'iyle tutarlı
- `end` tek başına iç içe yapılarda belirsizlik yaratır — REDDEDILDI
- `class` yok, `struct` var — `program` da aynı felsefede

**Normalizer dönüşümü:**
```
program          →  function main()
end_program      →  end function
```

**Durum:** ✅ Karar verildi — Normalizer implementasyonu bekliyor

---

### 2️⃣ String Interpolation

**mlp canonical:**
```mlp
string lang = "MELP"
print("Merhaba, " + lang + "!")
```

**Önerilen sugar:**
```mlp
string lang = "MELP"
print("Merhaba, {lang}!")
```

**Durum:** ⚠️ **Test edilmedi**
**Not:** String interpolation özelliği test edilmedi. Compiler kodu parse ederken hata verebilir.

---

### 3️⃣ println() — Açık Newline ile Print

**mlp canonical:**
```mlp
print("Merhaba, MELP!")
```

**Önerilen sugar:**
```mlp
println("Merhaba, MELP!")
```

**Durum:** ⚠️ **Test edilmedi**
**Not:** PRINT.md'de `println()` bahsediliyor ama test edilmedi.

---

## Görev 02-04: Variables, Arithmetic, String Ops

**Not:** Temel syntax doğru, sugar gerekmez.

---

## Görev 05-06: Boolean & If/Else

**Operators:** `and`, `or`, `not` — C-style (`&&`, `||`, `!`) YASAK
**Durum:** ✅ Doğru çalışıyor

---

## Görev 07: while_loop

### exit — Loop Break

**Canonical:** Şu an mümkün değil
**Önerilen:**
```mlp
while condition
    if break_condition then
        exit
    end if
end while
```

**Durum:** 🚫 **Eksik** — TODO_KURALLARI'nda var, ama compiler'da implement edilmedi

---

## Görev 08: for_loop

**Durum:** ✅ `for...to`, `for...downto`, `for...step` doğru çalışıyor. Sugar gerekmez.

---

## Görev 09: functions

### String Return Type Bug

**Problem:** `function greet(string name) as string` — Backend crash

**Durum:** 🚫 **Backend bug** — LLVM type mismatch

---

## Görev 10: arrays

**Durum:** ✅ Çalışıyor — `numeric[]` sözdizimi kullanılmalı

**NOT:** `array<numeric>` C++ template syntax'ı — MLP'de geçersiz, segfault verir.

```mlp
-- ✅ DOĞRU:
numeric[] nums = [1; 2; 3]
print(nums[0])

-- ❌ YANLIŞ (segfault!):
array<numeric> nums = [1; 2; 3]
```

---

## Özet: Compiler Eksiklikleri

| Feature | Durum | Görev |
|---------|-------|-------|
| `program/end_program` | ❌ Normalizer | 01 |
| `exit` keyword | 🚫 Eksik | 07 |
| String return type | 🚫 Bug | 09 |
| Array (`numeric[]`) | ✅ Çalışıyor | 10 |

**TAMAMLANDI — YZ_01 (2026-03-03)**

---

## Görev 11-15: Seviye 2 Başlangıcı (YZ_02)

### Görev 11: String Interpolation

**Feature:** STR_BUILTIN ✅, PRINT ✅, STRING_INTERPOLATION ⚠️

**Çalışan:**
```mlp
-- ✅ str() builtin
string msg = "Değer: " + str(42)
print(msg)

-- ✅ Multiple concatenation
numeric x = 10; numeric y = 20
string s = "x=" + str(x) + " y=" + str(y)
print(s)
```

**Durum:** ✅ STR_BUILTIN (Phase 1 — P-02) doğru çalışıyor

---

### Görev 12: Nested Functions

**Feature:** FUNCTION_DEF ✅

**Test:**
```mlp
function add(numeric x; numeric y) as numeric
    return x + y
end function

function calculate(numeric a; numeric b) as numeric
    numeric sum = add(a; b)      -- nested call
    return sum * 2
end function

function main()
    numeric r = calculate(4; 5)  -- doubly nested
    print(r)
end function
```

**Durum:** ✅ FUNCTION_DEF doğru çalışıyor, nested calls work

---

### Görev 13: Higher Order Functions

**Features:** FUNCREF 🔴, HIGHER_ORDER_FUNCTIONS ✅

**Problem:** FUNCREF henüz lexer'da implement edilmemiş
```mlp
-- ❌ Bu çalışmiyor:
funcref<(numeric) -> numeric> fn = double
numeric r = fn(5)
```

**Workaround:** Fallback to direct calls
```mlp
-- ✅ Bu çalışıyor:
numeric r = double(5)
```

**Durum:** 🔴 FUNCREF TODO (Phase 3) — lexer token gerekli

---

### Görev 14: Lambda

**Features:** LAMBDA ✅ (inline), CLOSURE ⚠️

**Çalışan:**
```mlp
-- ✅ Inline lambda
numeric double = lambda(x) -> x * 2
numeric r = double(5)

-- ✅ Closure (capture)
numeric base = 100
numeric add_base = lambda(x) -> x + base
numeric r2 = add_base(23)  -- 123
```

**Hata vermez ama block lambda syntax yok:**
```mlp
-- ❌ end_lambda implement edilmedi:
numeric fn = lambda(x) as numeric
    return x * 2
end_lambda
```

**Durum:** ✅ Inline lambda + closure çalışıyor

---

### Görev 15: Struct

**Feature:** STRUCT ✅

**Çalışan:**
```mlp
struct Point
    numeric x
    numeric y
end struct

Point p
p.x = 10
p.y = 20
print(p.x)      -- ✅ print(member) çalışıyor

-- Method (external) — this.field local var üzerinden
function Rectangle.area() as numeric
    numeric w = this.width
    numeric h = this.height
    return w * h
end function

-- Method call syntax
rect.area()     -- ✅ çalışıyor
```

**Stage0 B4 Kısıtlamaları (Yeni Keşfedildi):**
```mlp
-- ❌ this.a * this.b — parsePrimary sınırı, local var kullan:
function Foo.calc() as numeric
    numeric a = this.a
    numeric b = this.b
    return a * b      -- ✅ workaround
end function

-- ❌ str(p.x) — builtin argümanı olarak member access çalışmıyor:
print(str(p.x))   -- ❌ garbage value
print(p.x)        -- ✅ doğrudan print çalışıyor

-- ❌ rect.top_left.x = 0 — 2-level nested member assignment:
rect.top_left.x = 0   -- ❌ "Unknown variable: x"
-- Workaround: nested struct field'larını flat tut
```

**Durum:** ✅ STRUCT numeric fields + methods çalışıyor (B4 workaroundları ile)

---

## Yeni Bulunlar (Görev 11-15)

| Feature | Durum | Görev |
|---------|-------|-------|
| `str()` builtin | ✅ Complete | 11 |
| Nested function calls | ✅ Works | 12 |
| `funcref<>` type | 🔴 TODO | 13 |
| Inline lambda | ✅ Works | 14 |
| Closure capture | ✅ Works | 14 |
| `end_lambda` block | ❌ Not impl | 14 |
| Struct types | ✅ Works | 15 |
| Struct methods | ✅ Works | 15 |
| `print(member)` | ✅ Works | 15 |
| `str(member)` | ❌ B4 bug | 15 |
| `this.a * this.b` | ❌ B4 — local var | 15 |
| 2-level nested member assign | ❌ B4 bug | 15 |
| String fields in struct | ⚠️ Risky (B1) | 15 |

**TAMAMLANDI — YZ_02 + ÜA düzeltme (2026-03-03)**

---

## Görev 16-20: Seviye 2 İkinci Yarısı (YZ_03)

### Görev 16: Pattern Matching

**Feature:** PATTERN_MATCHING ✅ (match-case doğru, enum eksik)

**Çalışan:**
```mlp
-- ✅ Match-case expression
numeric result = match x
    case 1 then 100
    case 2 then 200
    else 0
end match
```

**Enum feature eksik:**
```mlp
-- ❌ Bu çalışmıyor:
enum Color
    RED
    GREEN
end enum
numeric c = Color.RED

-- ❌ Hata: LLVM type mismatch, enum member access fail
```

**Durum:** ✅ Match-case çalışıyor / ❌ Enum feature broken

---

### Görev 17: Nullable

**Feature:** NULLABLE ⚠️ (syntax yok, null kontrol if-then ile)

**Workaround:**
```mlp
-- ❌ Şu çalışmıyor:
numeric? x = null

-- ✅ Bu çalışıyor (0 = null simülasyonu):
numeric x = 0
if not (x == 0) then
    -- x var
end if
```

**Not:** `!=` C-stili operator — MELP'de `not (x == op y)` kullanılmalı

**Durum:** ❌ `numeric?` syntax parse error / ✅ If-then workaround çalışıyor

---

### Görev 18: Error Handling

**Feature:** ERROR_HANDLING ⚠️ (try-catch partial, error pattern ile)

**Çalışan Pattern:**
```mlp
function divide(numeric a; numeric b) as numeric
    if b == 0 then
        return 0  -- hata döndür
    else
        return a / b
    end if
end function

-- Kullanım:
numeric result = divide(10; 0)
if result == 0 then
    -- hata işle
end if
```

**Durum:** ❌ Try-catch exec error / ✅ If-return hata pattern

---

### Görev 19: HashMap

**Feature:** HASHMAP ✅ (creation + access), method'lar ❌

**Çalışan:**
```mlp
hashmap<string, numeric> map = {}
map["key"] = 100
numeric val = map["key"]      -- ✅ 100
```

**Method'lar çalışmıyor:**
```mlp
-- ❌ Hata: "not a struct"
map.has("key")
map.delete("key")
map.length
```

**Durum:** ✅ Creation + access / ❌ Methods (codegen eksik)

---

### Görev 20: Advanced Pattern Matching

**Feature:** PATTERN_MATCHING ✅ (case + function return)

**Örnek:**
```mlp
function classify(numeric n) as numeric
    match n
        case 1 then return 10
        case 2 then return 20
        else return 0
    end match
end function
```

**Guard clause (eksik):**
```mlp
-- ❌ Parser error:
match x
    case y if y > 10 then ...
```

**Durum:** ✅ Basic match-case / ❌ Guard clause missing

---

## Özet: Görev 16-20 Bulguları

| Feature | Durum | Workaround |
|---------|-------|-----------|
| `match...case...end match` | ✅ Complete | — |
| `enum Type ... end enum` | ❌ LLVM bug | Match-case ile koşul |
| `numeric?` nullable | ❌ Parser error | `numeric` 0 = null |
| `try-catch` error | ❌ Partial | if-return pattern |
| `hashmap.method()` | ❌ Codegen eksik | Direct access only |
| `case x if condition` | ❌ Missing | — |

**TAMAMLANDI — YZ_03 (2026-03-03)**

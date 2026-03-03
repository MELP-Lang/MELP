# PATTERN_MATCHING - Özellik Kartı

**Özellik:** Pattern Matching — Exhaustive, guard'lı, struct/enum destructuring destekli  
**Durum:** 🚧 WIP (Stage 1, Week 5.x)  
**Hafta:** Week 5.x  
**Sözdizimi Sürümü:** pmlp1

---

## 🎯 ÖZELLİK

`match / case / end_match` yapısıyla exhaustive pattern matching. Wildcard `_`, guard clause (`if`), enum destructuring ve struct pattern desteklenir. Match bir ifade (expression) olarak değer döndürebilir.

---

## 📖 SÖZDİZİMİ

### Basit Match
```pmlp
numeric x = 5
match x
    case 1 then print("bir")
    case 2 then print("iki")
    else print("diğer")
end_match
```

### Match İfade Olarak (Değer Döndürür)
```pmlp
string msg = match x
    case 1 then "bir"
    case 2 then "iki"
    else "diğer"
end_match
```

### Guard Clause
```pmlp
numeric n = 15
match n
    case x if x < 10 then print("küçük")
    case x if x < 20 then print("orta")
    else print("büyük")
end_match
```

### Enum Destructuring
```pmlp
Option{string} user = find_user("Ali")
match user
    case Some(u):
        print("Bulundu: " + u)
    case None:
        print("Yok")
end_match
```

### Struct Destructuring
```pmlp
struct Point
    numeric x
    numeric y
end_struct

Point p
p.x = 10
p.y = 0

match p
    case Point { x: 0; y: 0 } then print("orijin")
    case Point { x: _; y: 0 } then print("X ekseninde")
    case Point { x: 0; y: _ } then print("Y ekseninde")
    else print("başka")
end_match
```

---

## 🧪 GOLDEN TEST

```pmlp
function classify(numeric n) as string
    return match n
        case 0 then "sıfır"
        case x if x < 0 then "negatif"
        case x if x < 100 then "küçük"
        else "büyük"
    end_match
end_function

function main() as numeric
    if classify(0) == "sıfır" and classify(-5) == "negatif" then
        return 1  -- PASS
    end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Generic types (Option{T}, Result{T;E})  
**Hafta:** Stage 1 Phase 1  
**Kanonik Kaynak:** `belgeler/pmlp1_sozdizimi.md` (Match bölümü)  
**Impl:** `compiler/stage1/`

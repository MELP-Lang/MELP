# STRUCT - Özellik Kartı

**Özellik:** Struct Types — Sadece data container (OOP değil, STRUCT+FUNC prensibi)  
**Durum:** ✅ COMPLETE (Stage 0 — minimal / Stage 1 — struct methods)  
**Hafta:** Stage 0 (temel) / Stage 1 Phase 1 (methods)  
**Sözdizimi Sürümü:** pmlp0 / pmlp1

---

## 🎯 ÖZELLİK

Struct sadece veri taşıyıcısıdır. Method'lar struct içinde DEĞİL, dışarıda `function StructName.method()` şeklinde tanımlanır. Bu MELP'in STRUCT+FUNC prensibidir (OOP ihlali yok).

---

## 📖 SÖZDİZİMİ

### Basit Struct (Stage 0)
```pmlp
struct Point
    numeric x
    numeric y
end_struct

-- Instance oluşturma ve kullanım
Point p
p.x = 10
p.y = 20
print(p.x)  -- 10

-- Struct literal (Stage 1)
Point p2 = {x: 10; y: 20}
```

### Struct Methods (Stage 1) — EXTERNAL impl
```pmlp
struct Rectangle
    numeric width
    numeric height
end_struct

-- ✅ DOĞRU: Method dışarıda tanımlanır
function Rectangle.area() as numeric
    return this.width * this.height
end_function

function Rectangle.perimeter() as numeric
    return 2 * (this.width + this.height)
end_function

-- Kullanım
Rectangle r
r.width = 10
r.height = 5
print(r.area())       -- 50
print(r.perimeter())  -- 30
```

### İç İçe Struct
```pmlp
struct Address
    string street
    numeric zipcode
end_struct

struct Person
    string name
    numeric age
    Address addr
end_struct

Person p
p.name = "Ali"
p.age = 30
p.addr.street = "Atatürk Cad."
print(p.addr.street)  -- "Atatürk Cad."
```

### Struct Dizisi
```pmlp
Point[] points
-- points'e Point instance'ları eklenir
```

### ❌ YASAK: OOP-style methods
```pmlp
-- ❌ YANLIŞ — method struct içinde olamaz!
struct Point
    numeric x
    function distance() as numeric  -- YASAK!
        return this.x
    end_function
end_struct
```

---

## 🧪 GOLDEN TEST

```pmlp
struct Counter
    numeric value
end_struct

function Counter.increment() as numeric
    this.value = this.value + 1
    return this.value
end_function

function main() as numeric
    Counter c
    c.value = 0
    c.increment()
    c.increment()
    if c.value == 2 then
        return 1  -- PASS
    end_if
    return 0      -- FAIL
end_function
```

---

**Bağımlılıklar:** Yok (Stage 0'da mevcut)  
**Kanonik Kaynak:** `belgeler/pmlp0_sozdizimi.md` (Struct bölümü)  
**Impl:** `compiler/stage0/modules/struct/` ve `compiler/stage1/`

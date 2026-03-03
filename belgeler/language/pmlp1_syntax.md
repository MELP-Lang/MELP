# PMLP1 Syntax Reference (Stage 1)

**Version:** 2.0  
**Date:** 19 Şubat 2026  
**Stage:** Stage 1 = PMLP0 + 8 Yeni Özellik  
**Kanonik Kaynak:** [`belgeler/pmlp1_sozdizimi.md`](../pmlp1_sozdizimi.md) (829 satır)

---

## 🎯 STAGE 1: PMLP0 + 8 YENİ ÖZELLİK

**Baseline:** PMLP0 (bakın: [pmlp0_syntax.md](pmlp0_syntax.md))

**Yeni Özellikler:**
1. **enum** — Enumeration tipleri
2. **struct methods** — Struct'a bağlı metotlar
3. **generics** — Tip parametreleri (`<T>`)
4. **interfaces** — Trait benzeri polimorfizm
5. **lambda** — Anonim fonksiyonlar + closure
6. **error_handling** — Try-catch + Result{T;E}
7. **pattern_match** — Match ifadeleri
8. **operator_overload** — Struct'lar için özel operatörler

---

## ✅ PMLP0 BASELINE (INHERITED)

Tüm PMLP0 kuralları aynen geçerli. Önemli hatırlatma:
- `;` SADECE parametre/eleman ayırıcı (satır sonu DEGİL)
- Blok sonlandırıcılar: `end_if`, `end_while`, `end_for`, `end_function`
- Türk sayı formatı: `3,14` (virgül=ondalik), `1.000` (nokta=binlik)
- `if` her zaman `then` ile biter
- `else_if` tek token (altçizgi)

**Yeni blok sonlandırıcılar (Stage 1):**
```pmlp
end_struct, end_enum, end_interface,
end_lambda, end_match, end_try, end_implement
```

---

## 🔆 YENİ ÖZELLİKLER

### 1. enum

```pmlp
enum Color
    RED
    GREEN
    BLUE
end_enum

function main() as numeric
    numeric c = Color.RED  -- 0
    if c == Color.GREEN then
        print("yeşil")
    end_if
    return 0
end_function
```

**Kurallar:** Değerler 0'dan otomatik numaralandırılır. Erişim: `EnumName.VALUE`.

---

### 2. struct methods

```pmlp
struct Rectangle
    numeric width
    numeric height
end_struct

function Rectangle.area() as numeric
    return this.width * this.height
end_function

function main() as numeric
    Rectangle r
    r.width = 10
    r.height = 20
    print(r.area())  -- 200
    return 0
end_function
```

**Kurallar:** Metot tanımı: `function StructName.method(...)`. `this` mevcut instance'a işaret eder.

---

### 3. generics

```pmlp
function identity<T>(T value) as T
    return value
end_function

struct Box<T>
    T value
    boolean is_filled
end_struct

function main() as numeric
    numeric x = identity<numeric>(42)
    string s = identity<string>("merhaba")
    
    Box<numeric> b
    b.value = 99
    b.is_filled = true
    print(b.value)  -- 99
    return 0
end_function
```

**Kurallar:** `<T>` fonksiyon/struct adından sonra. Çoklu tip param: `<T; U>` (noktalıvirgül).

---

### 4. interfaces

```pmlp
interface Shape
    function area() as numeric
    function perimeter() as numeric
end_interface

struct Circle implements Shape
    numeric radius
end_struct

function Circle.area() as numeric
    return 3.14159 * this.radius * this.radius
end_function

function Circle.perimeter() as numeric
    return 2 * 3.14159 * this.radius
end_function

function print_info(Shape s) as numeric
    print(s.area())
    return 0
end_function
```

**Kurallar:** `interface ... end_interface`. Implementation: `struct X implements Interface1; Interface2`.

---

### 5. lambda

```pmlp
-- Tek ifadeli lambda
function double = lambda(numeric x) as numeric
    return x * 2
end_lambda

print(double(5))  -- 10

-- Closure (dış scope değişkeni yakalar)
function make_adder(numeric n) as function(numeric) as numeric
    return lambda(numeric x) as numeric
        return x + n  -- n yakalandı
    end_lambda
end_function

function add10 = make_adder(10)
print(add10(5))  -- 15
```

**Kurallar:** `lambda(params) as return_type ... end_lambda`. Tip: `function(params) as return_type`.

---

### 6. error_handling

```pmlp
try
    numeric result = risky_op()
    print(result)
catch error
    print("Hata: " + error.message)
finally
    cleanup()
end_try

-- Result{T;E} pattern
function divide(numeric a; numeric b) as Result<numeric; string>
    Result<numeric; string> r
    if b == 0 then
        r.is_success = false
        r.error = "Sıfıra bölme"
    else
        r.is_success = true
        r.value = a / b
    end_if
    return r
end_function
```

**Kurallar:** `try ... catch var ... finally ... end_try`. `finally` opsiyonel.

---

### 7. pattern_match

```pmlp
match value
    case 0 then print("sıfır")
    case 1 then print("bir")
    else print("diğer")
end_match

-- Struct destructuring
match p
    case Point { x: 0; y: 0 } then print("orijin")
    case Point { x: _; y: 0 } then print("X ekseninde")
    else print("başka")
end_match

-- Match ifade olarak değer döndürür
string msg = match code
    case 0 then "başarı"
    case 1 then "hata"
    else "bilinmiyor"
end_match
```

**Kurallar:** `match expr ... end_match`. Wildcard: `_`. Varsayılan: `else`.

---

### 8. operator_overload

```pmlp
struct Vector
    numeric x
    numeric y
end_struct

function Vector.operator+(Vector other) as Vector
    Vector result
    result.x = this.x + other.x
    result.y = this.y + other.y
    return result
end_function

function Vector.operator==(Vector other) as boolean
    return this.x == other.x and this.y == other.y
end_function

function main() as numeric
    Vector v1
    v1.x = 3
    v1.y = 4
    Vector v2
    v2.x = 1
    v2.y = 2
    Vector v3 = v1 + v2  -- operator+ çağrılır
    print(v3.x)           -- 4
    return 0
end_function
```

**Desteklenen operatörler:** `+`, `-`, `*`, `/`, `==`, `!=`, `<`, `>`, `<=`, `>=`

---

## ⚠️ ANTI-PATTERNS

```pmlp
-- ❌ YANLIŞ: Satır sonu noktalıvirgül
numeric x = 10;

-- ❌ YANLIŞ: Virgüllü parametre ayırımı
function test(numeric a, numeric b)  -- Noktalıvirgül kullan!

-- ❌ YANLIŞ: then eksik
if x > 10
    print(x)
end_if

-- ❌ YANLIŞ: else if (iki token)
else if x > 10 then  -- else_if kullan!
```

---

## 🔗 İLGİLİ BELGELER

- [pmlp0_syntax.md](pmlp0_syntax.md) — Stage 0 tam referans
- [pmlp1_sozdizimi.md](../pmlp1_sozdizimi.md) — Kanonik Stage 1 spec (829 satır)
- [pmlp_sozdizimi.md](../pmlp_sozdizimi.md) — Tam MELP spec (1994 satır)
- [feature_cards/](feature_cards/) — Odaklı sözdizimi kartları
- [golden_reference1.mlp](golden_reference1.mlp) — Stage 1 golden referans

---

**Son Güncelleme:** 19 Şubat 2026  
**Durum:** Kanonik kaynakla hizalandı (8 yeni özellik) ✅


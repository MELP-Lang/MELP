# YZ_211 İLERLEME RAPORU - Move Semantics (Ownership Transfer)

**Tarih:** 22 Aralık 2025  
**Agent:** GitHub Copilot (Claude Sonnet 4.5)  
**Branch:** `move-semantics_YZ_211`  
**Durum:** 🚧 **IN PROGRESS** - Phase 1 başladı

---

## 🎯 GÖREV ÖZET

**Hedef:** Rust-style move semantics - veri kopyalamadan sahiplik transferi

**3 Ana Faz:**
1. 🚧 Phase 1: Move Syntax (2 gün)
2. ⏳ Phase 2: Move Semantics (3 gün)
3. ⏳ Phase 3: Borrow Checker (2 gün)

**Öncelik:** 🟢 Düşük (optimization - ertelenebilir)

---

## 📚 KAVRAMSAL AÇIKLAMA

### Move Semantics Nedir?

**Problem:**
```pmpl
struct BigData
    numeric[1000000] data  -- 8MB veri
end_struct

BigData original = create_big_data()
BigData copy = original  -- ❌ 8MB kopyalama! Yavaş!
```

**Çözüm: Move Semantics**
```pmpl
BigData original = create_big_data()
BigData moved = move original  -- ✅ Sadece pointer transfer! Hızlı!

-- original artık geçersiz
print(original.data[0])  -- ⚠️ Compile error: "value moved"
print(moved.data[0])     -- ✅ OK
```

### Borrow vs Move

**Borrowing (Geçici Erişim):**
```pmpl
function process(BigData& data) returns numeric  -- & = borrow
    return data.data[0]
end_function

BigData d = create_big_data()
numeric result = process(&d)  -- Ödünç ver
print(d.data[0])              -- ✅ Hala kullanılabilir
```

**Moving (Sahiplik Transferi):**
```pmpl
function consume(BigData data) returns numeric  -- No & = move
    return data.data[0]
end_function  -- data destroyed here

BigData d = create_big_data()
numeric result = consume(d)  -- Sahipliği aktar (implicit move)
print(d.data[0])             -- ⚠️ Compile error: "value moved"
```

---

## 🏗️ IMPLEMENTATION PLAN

### Phase 1: Move Syntax (2 gün)

**1.1. Lexer Changes**
- [ ] Add `move` keyword to lexer
- [ ] Token type: `TOKEN_MOVE`

**1.2. Parser Changes**
- [ ] Parse move expressions: `var2 = move var1`
- [ ] AST node: `MoveExpr` structure
- [ ] Track moved variables in symbol table

**1.3. Ownership Tracking**
- [ ] Add `is_moved` flag to variables
- [ ] Detect use-after-move errors
- [ ] Error message: "value moved"

**Test:**
```pmpl
function test_move() returns numeric
    numeric x = 10
    numeric y = move x
    -- print(x)  -- Error: value moved
    print(y)    -- OK: 10
    return 0
end
```

---

### Phase 2: Move Semantics (3 gün)

**2.1. Codegen for Move**
- [ ] Emit pointer transfer (not copy)
- [ ] Invalidate source variable
- [ ] Generate move assignment code

**2.2. Struct Move**
- [ ] Move entire struct (shallow)
- [ ] Handle nested structs
- [ ] Test with large structs

**2.3. Compile-time Checking**
- [ ] Verify no use after move
- [ ] Check at function boundaries
- [ ] Error on invalid access

**Test:**
```pmpl
struct Person
    string name
    numeric age
end_struct

function transfer_person() returns numeric
    Person alice = Person{"Alice", 30}
    Person bob = move alice
    
    -- print(alice.name)  -- Error: value moved
    print(bob.name)       -- OK: "Alice"
    return 0
end
```

---

### Phase 3: Borrow Checker (2 gün)

**3.1. Immutable Borrows**
- [ ] Parse `&var` syntax
- [ ] Track borrowed references
- [ ] Verify no move during borrow

**3.2. Mutable Borrows**
- [ ] Parse `&mut var` syntax
- [ ] Ensure exclusive access
- [ ] Detect conflicts

**3.3. Lifetime Tracking (Basit)**
- [ ] Track borrow lifetimes
- [ ] Ensure borrowed data outlives reference
- [ ] Error on lifetime violations

**Test:**
```pmpl
function read_only(Person& p) returns string
    return p.name
end_function

function modify(Person& mut p) returns numeric
    p.age = p.age + 1
    return p.age
end_function

function test_borrow() returns numeric
    Person alice = Person{"Alice", 30}
    
    string name = read_only(&alice)  -- Immutable borrow
    print(name)                      -- OK: "Alice"
    
    numeric new_age = modify(&mut alice)  -- Mutable borrow
    print(new_age)                        -- OK: 31
    
    return 0
end
```

---

## 🔧 TECHNICAL DESIGN

### Data Structures

**MoveExpr (AST Node):**
```c
typedef struct MoveExpr {
    char* var_name;      // Variable being moved
    int line;            // Line number for errors
    int is_implicit;     // Implicit vs explicit move
} MoveExpr;
```

**Variable Ownership State:**
```c
typedef enum {
    VAR_OWNED,           // Variable owns data
    VAR_MOVED,           // Data moved out
    VAR_BORROWED_IMM,    // Immutably borrowed
    VAR_BORROWED_MUT     // Mutably borrowed
} OwnershipState;

typedef struct Variable {
    char* name;
    OwnershipState state;
    int moved_at_line;   // Where it was moved
    // ... other fields
} Variable;
```

**Borrow Reference:**
```c
typedef struct BorrowRef {
    char* var_name;      // Borrowed variable
    int is_mutable;      // Mutable borrow?
    int borrow_line;     // Where borrowed
    struct BorrowRef* next;
} BorrowRef;
```

---

## 📝 SYNTAX EXAMPLES

### Move Syntax
```pmpl
-- Explicit move
BigData a = create()
BigData b = move a  -- a is now invalid

-- Implicit move (function call)
function consume(BigData d) returns numeric
    return d.data[0]
end_function

BigData x = create()
consume(x)  -- x implicitly moved
-- x is now invalid
```

### Borrow Syntax
```pmpl
-- Immutable borrow
function read(BigData& data) returns numeric
    return data.data[0]
end_function

BigData d = create()
read(&d)  -- Borrow
d.data[0] = 999  -- Still valid

-- Mutable borrow
function write(BigData& mut data) returns numeric
    data.data[0] = 123
    return 0
end_function

BigData d = create()
write(&mut d)
print(d.data[0])  -- 123
```

---

## 🎯 SUCCESS CRITERIA

**Phase 1:**
- [ ] `move` keyword recognized
- [ ] Move expression parsed
- [ ] Use-after-move detected
- [ ] Error messages clear

**Phase 2:**
- [ ] Move generates correct code
- [ ] No data copying (pointer transfer)
- [ ] Compile-time checking works
- [ ] Struct moves work

**Phase 3:**
- [ ] Borrow syntax works
- [ ] Immutable/mutable borrows distinct
- [ ] Conflict detection works
- [ ] Lifetime errors caught

---

## 🚀 PHASE 1 BAŞLIYOR

**Bugünkü Hedef:** Move keyword ve parser implementasyonu

**Adımlar:**
1. Lexer'a `move` keyword ekle
2. Parser'a move expression desteği ekle
3. AST node oluştur
4. Basit test yaz

**Dosyalar:**
- `compiler/stage0/modules/lexer/` - Keyword ekleme
- `compiler/stage0/modules/parser/` - Move parsing
- `compiler/stage0/modules/variable/` - Ownership tracking
- `tests/move/` - Test dosyaları

---

**Başlangıç:** 22 Aralık 2025 - 11:00  
**Durum:** 🚧 Phase 1 başlıyor...

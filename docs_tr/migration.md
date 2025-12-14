# MELP Multi-Syntax Migration Guide

**Amaç:** Farklı dillerdeki syntax'ların PMPL'ye nasıl normalize edileceğini belgelemek  
**Hedef:** Normalize edici modülü için referans doküman  
**Tarih:** 14 Aralık 2025

---

## 📋 İçindekiler

1. [Blok Sonlandırıcılar (Block Terminators)](#blok-sonlandırıcılar)
2. [Koşul İfadeleri (Conditionals - If/Else)](#koşul-ifadeleri-if-else)
3. [Döngüler (Loops - While/For)](#döngüler-loops)
4. [Fonksiyonlar (Functions)](#fonksiyonlar)
5. [Switch/Case Statements](#switch-case)
6. [Döngü Kontrolü (Break/Continue)](#döngü-kontrolü)
7. [Blok Yapıları ve Scope](#blok-yapıları-ve-scope)

---

## 1. Blok Sonlandırıcılar (Block Terminators)

### MELP/PMPL (Target - Hedef Syntax)

```mlp
if condition then
    statement
end if       → PMPL: end_if (TEK TOKEN!)

while condition do
    statement
end while    → PMPL: end_while

for i = 1 to 10 do
    statement
end for      → PMPL: end_for

function name()
    statement
end function → PMPL: end_function

struct Point
    x: numeric
    y: numeric
end struct   → PMPL: end_struct
```

**PMPL Kuralı:** Tüm "end X" ifadeleri underscore ile birleştirilir: `end_X`

---

### C/C++

```c
// If statement
if (condition) {
    statement;
}                    → PMPL: end_if

// While loop
while (condition) {
    statement;
}                    → PMPL: end_while

// For loop
for (i = 0; i < 10; i++) {
    statement;
}                    → PMPL: end_for

// Function
void name() {
    statement;
}                    → PMPL: end_function

// Struct
struct Point {
    int x;
    int y;
};                   → PMPL: end_struct
```

**Normalize Kuralı:** `}` karakteri → bağlama göre `end_if`, `end_while`, `end_for`, vs.

---

### Python

```python
# If statement
if condition:
    statement
# (implicit end) → PMPL: end_if

# While loop
while condition:
    statement
# (implicit end) → PMPL: end_while

# For loop
for i in range(10):
    statement
# (implicit end) → PMPL: end_for

# Function
def name():
    statement
# (implicit end) → PMPL: end_function

# Class (struct benzeri)
class Point:
    x: int
    y: int
# (implicit end) → PMPL: end_struct
```

**Normalize Kuralı:** Indentation azalması → block end  
**Challenge:** Indentation tracking gerekli, context-aware parsing

---

### Go

```go
// If statement
if condition {
    statement
}                    → PMPL: end_if

// While benzeri (for kullanılır)
for condition {
    statement
}                    → PMPL: end_while

// For loop
for i := 0; i < 10; i++ {
    statement
}                    → PMPL: end_for

// Function
func name() {
    statement
}                    → PMPL: end_function

// Struct
type Point struct {
    X int
    Y int
}                    → PMPL: end_struct
```

**Normalize Kuralı:** C benzeri, `}` → context'e göre `end_X`

---

### Rust

```rust
// If statement
if condition {
    statement
}                    → PMPL: end_if

// While loop
while condition {
    statement
}                    → PMPL: end_while

// For loop
for i in 0..10 {
    statement
}                    → PMPL: end_for

// Function
fn name() {
    statement
}                    → PMPL: end_function

// Struct
struct Point {
    x: i32,
    y: i32,
}                    → PMPL: end_struct
```

**Normalize Kuralı:** C/Go benzeri, `}` → `end_X`

---

### Ruby

```ruby
# If statement
if condition
    statement
end                  → PMPL: end_if

# While loop
while condition
    statement
end                  → PMPL: end_while

# For loop (each kullanılır)
(1..10).each do |i|
    statement
end                  → PMPL: end_for

# Function
def name
    statement
end                  → PMPL: end_function

# Class
class Point
    attr_accessor :x, :y
end                  → PMPL: end_struct
```

**Normalize Kuralı:** `end` keyword → context'e göre `end_X`  
**Challenge:** Tek `end` keyword, context tracking gerekli

---

### Pascal/Delphi

```pascal
// If statement
if condition then
begin
    statement;
end;                 → PMPL: end_if

// While loop
while condition do
begin
    statement;
end;                 → PMPL: end_while

// For loop
for i := 1 to 10 do
begin
    statement;
end;                 → PMPL: end_for

// Function
function name: Integer;
begin
    statement;
end;                 → PMPL: end_function

// Record (struct)
type Point = record
    x: Integer;
    y: Integer;
end;                 → PMPL: end_struct
```

**Normalize Kuralı:** `end;` → `end_X` (begin/end pairing)

---

### Lua

```lua
-- If statement
if condition then
    statement
end                  → PMPL: end_if

-- While loop
while condition do
    statement
end                  → PMPL: end_while

-- For loop
for i = 1, 10 do
    statement
end                  → PMPL: end_for

-- Function
function name()
    statement
end                  → PMPL: end_function

-- Table (struct benzeri)
Point = {
    x = 0,
    y = 0
}                    → PMPL: (special handling)
```

**Normalize Kuralı:** `end` → context'e göre `end_X`

---

## 2. Koşul İfadeleri (Conditionals - If/Else)

### MELP/PMPL

```mlp
if x > 5 then
    print("Büyük")
else if x > 2 then
    print("Orta")
else
    print("Küçük")
end if
```

**PMPL Çıktı:**
```pmpl
if x > 5 then
    print("Büyük")
else_if x > 2 then    ← else_if birleştirilir
    print("Orta")
else
    print("Küçük")
end_if
```

---

### C/C++

```c
if (x > 5) {
    printf("Büyük");
} else if (x > 2) {
    printf("Orta");
} else {
    printf("Küçük");
}
```

**PMPL Normalize:**
```pmpl
if x > 5 then         ← Parantez kaldır, "then" ekle
    print("Büyük")
else_if x > 2 then    ← "else if" → "else_if"
    print("Orta")
else
    print("Küçük")
end_if                ← } → end_if
```

---

### Python

```python
if x > 5:
    print("Büyük")
elif x > 2:           ← elif keyword
    print("Orta")
else:
    print("Küçük")
```

**PMPL Normalize:**
```pmpl
if x > 5 then         ← : → then
    print("Büyük")
else_if x > 2 then    ← elif → else_if
    print("Orta")
else
    print("Küçük")
end_if                ← indentation end → end_if
```

---

### Go

```go
if x > 5 {
    fmt.Println("Büyük")
} else if x > 2 {
    fmt.Println("Orta")
} else {
    fmt.Println("Küçük")
}
```

**PMPL Normalize:** C ile aynı

---

### Rust

```rust
if x > 5 {
    println!("Büyük");
} else if x > 2 {
    println!("Orta");
} else {
    println!("Küçük");
}
```

**PMPL Normalize:** C ile aynı

---

### Ruby

```ruby
if x > 5
    puts "Büyük"
elsif x > 2           ← elsif keyword
    puts "Orta"
else
    puts "Küçük"
end
```

**PMPL Normalize:**
```pmpl
if x > 5 then
    print("Büyük")
else_if x > 2 then    ← elsif → else_if
    print("Orta")
else
    print("Küçük")
end_if
```

---

### Koşul Operatörleri (Condition Operators)

| MELP/PMPL | C/C++ | Python | Go | Rust | Ruby |
|-----------|-------|--------|----|----- |------|
| `==` | `==` | `==` | `==` | `==` | `==` |
| `!=` | `!=` | `!=` | `!=` | `!=` | `!=` |
| `>` | `>` | `>` | `>` | `>` | `>` |
| `<` | `<` | `<` | `<` | `<` | `<` |
| `>=` | `>=` | `>=` | `>=` | `>=` | `>=` |
| `<=` | `<=` | `<=` | `<=` | `<=` | `<=` |
| `and` | `&&` | `and` | `&&` | `&&` | `and` / `&&` |
| `or` | `||` | `or` | `||` | `||` | `or` / `||` |
| `not` | `!` | `not` | `!` | `!` | `not` / `!` |

**Normalize Kuralı:** Tüm operatörler PMPL keyword'lerine: `and`, `or`, `not`

---

## 3. Döngüler (Loops)

### While Loops

**MELP/PMPL:**
```mlp
while count < 10 do
    count = count + 1
end while
```

**C/C++:**
```c
while (count < 10) {
    count++;
}
```

**Python:**
```python
while count < 10:
    count += 1
```

**Go:**
```go
for count < 10 {    ← Go'da while yok, for kullanılır
    count++
}
```

**Rust:**
```rust
while count < 10 {
    count += 1;
}
```

**Ruby:**
```ruby
while count < 10
    count += 1
end
```

---

### For Loops (Numeric Range)

**MELP/PMPL:**
```mlp
for i = 1 to 10 do
    print(i)
end for

for i = 10 downto 1 do
    print(i)
end for
```

**C/C++:**
```c
for (int i = 1; i <= 10; i++) {
    printf("%d", i);
}

for (int i = 10; i >= 1; i--) {
    printf("%d", i);
}
```

**Python:**
```python
for i in range(1, 11):
    print(i)

for i in range(10, 0, -1):
    print(i)
```

**Go:**
```go
for i := 1; i <= 10; i++ {
    fmt.Println(i)
}

for i := 10; i >= 1; i-- {
    fmt.Println(i)
}
```

**Rust:**
```rust
for i in 1..=10 {
    println!("{}", i);
}

for i in (1..=10).rev() {
    println!("{}", i);
}
```

**Ruby:**
```ruby
(1..10).each do |i|
    puts i
end

(1..10).reverse_each do |i|
    puts i
end
```

---

### For Each Loops

**MELP/PMPL:**
```mlp
for each item in list do
    print(item)
end for
```

**C++ (C++11+):**
```cpp
for (auto item : list) {
    std::cout << item;
}
```

**Python:**
```python
for item in list:
    print(item)
```

**Go:**
```go
for _, item := range list {
    fmt.Println(item)
}
```

**Rust:**
```rust
for item in &list {
    println!("{}", item);
}
```

**Ruby:**
```ruby
list.each do |item|
    puts item
end
```

---

## 4. Fonksiyonlar (Functions)

### MELP/PMPL

```mlp
function add(numeric a, numeric b) returns numeric
    return a + b
end function

function greet(string name)
    print("Hello " + name)
end function
```

---

### C/C++

```c
int add(int a, int b) {
    return a + b;
}

void greet(char* name) {
    printf("Hello %s", name);
}
```

**PMPL Normalize:**
```pmpl
function add(numeric a, numeric b) returns numeric
    return a + b
end_function

function greet(string name)
    print("Hello " + name)
end_function
```

---

### Python

```python
def add(a: int, b: int) -> int:
    return a + b

def greet(name: str):
    print(f"Hello {name}")
```

**PMPL Normalize:**
```pmpl
function add(numeric a, numeric b) returns numeric
    return a + b
end_function

function greet(string name)
    print("Hello " + name)
end_function
```

---

### Go

```go
func add(a int, b int) int {
    return a + b
}

func greet(name string) {
    fmt.Printf("Hello %s", name)
}
```

**PMPL Normalize:** C benzeri

---

### Rust

```rust
fn add(a: i32, b: i32) -> i32 {
    a + b  // veya return a + b;
}

fn greet(name: &str) {
    println!("Hello {}", name);
}
```

**PMPL Normalize:** C benzeri

---

### Ruby

```ruby
def add(a, b)
    return a + b
end

def greet(name)
    puts "Hello #{name}"
end
```

**PMPL Normalize:**
```pmpl
function add(numeric a, numeric b) returns numeric
    return a + b
end_function

function greet(string name)
    print("Hello " + name)
end_function
```

---

## 5. Switch/Case Statements

### MELP/PMPL

```mlp
switch value
    case 1:
        print("Bir")
    case 2:
        print("İki")
    default:
        print("Diğer")
end switch
```

---

### C/C++

```c
switch (value) {
    case 1:
        printf("Bir");
        break;
    case 2:
        printf("İki");
        break;
    default:
        printf("Diğer");
}
```

**PMPL Normalize:**
```pmpl
switch value
    case 1:
        print("Bir")
        # break implicit (MELP'te her case otomatik break)
    case 2:
        print("İki")
    default:
        print("Diğer")
end_switch
```

---

### Python (3.10+)

```python
match value:
    case 1:
        print("Bir")
    case 2:
        print("İki")
    case _:
        print("Diğer")
```

**PMPL Normalize:**
```pmpl
switch value
    case 1:
        print("Bir")
    case 2:
        print("İki")
    default:        ← _ → default
        print("Diğer")
end_switch
```

---

### Go

```go
switch value {
case 1:
    fmt.Println("Bir")
case 2:
    fmt.Println("İki")
default:
    fmt.Println("Diğer")
}
```

**PMPL Normalize:** Direct mapping

---

### Rust

```rust
match value {
    1 => println!("Bir"),
    2 => println!("İki"),
    _ => println!("Diğer"),
}
```

**PMPL Normalize:**
```pmpl
switch value
    case 1:
        print("Bir")
    case 2:
        print("İki")
    default:
        print("Diğer")
end_switch
```

---

### Ruby

```ruby
case value
when 1
    puts "Bir"
when 2
    puts "İki"
else
    puts "Diğer"
end
```

**PMPL Normalize:**
```pmpl
switch value
    case 1:        ← when → case
        print("Bir")
    case 2:
        print("İki")
    default:       ← else → default
        print("Diğer")
end_switch
```

---

## 6. Döngü Kontrolü (Break/Continue)

### MELP/PMPL

```mlp
while condition do
    if skip_condition then
        continue
    end if
    
    if exit_condition then
        exit        ← break
    end if
    
    statement
end while

# Belirli döngüden çıkış
exit while      → PMPL: exit_while
exit for        → PMPL: exit_for

# Belirli döngüde devam
continue while  → PMPL: continue_while
continue for    → PMPL: continue_for
```

---

### C/C++

```c
while (condition) {
    if (skip_condition) {
        continue;
    }
    
    if (exit_condition) {
        break;
    }
    
    statement;
}
```

**PMPL Normalize:**
```pmpl
while condition do
    if skip_condition then
        continue
    end_if
    
    if exit_condition then
        exit           ← break → exit
    end_if
    
    statement
end_while
```

---

### Python

```python
while condition:
    if skip_condition:
        continue
    
    if exit_condition:
        break
    
    statement
```

**PMPL Normalize:** C benzeri

---

### Go

```go
for condition {
    if skipCondition {
        continue
    }
    
    if exitCondition {
        break
    }
    
    statement
}
```

**PMPL Normalize:** C benzeri

---

### Rust

```rust
while condition {
    if skip_condition {
        continue;
    }
    
    if exit_condition {
        break;
    }
    
    statement;
}
```

**PMPL Normalize:** C benzeri

---

### Ruby

```ruby
while condition
    if skip_condition
        next        ← Ruby'de "next"
    end
    
    if exit_condition
        break
    end
    
    statement
end
```

**PMPL Normalize:**
```pmpl
while condition do
    if skip_condition then
        continue       ← next → continue
    end_if
    
    if exit_condition then
        exit           ← break → exit
    end_if
    
    statement
end_while
```

---

## 7. Blok Yapıları ve Scope

### Naked Blocks (Anonymous Scope)

**C/C++:**
```c
{
    int temp = 10;
    // temp sadece bu blokta görünür
}
// temp burada erişilemez
```

**PMPL:**
```pmpl
begin           ← { → begin
    numeric temp = 10
end_begin       ← } → end_begin
```

---

### Defer/Finally (Resource Management)

**MELP/PMPL:**
```mlp
defer
    cleanup_resource()
end defer
```

**Go:**
```go
defer cleanupResource()
```

**Python:**
```python
try:
    # code
finally:
    cleanup_resource()
```

**Rust:**
```rust
// Drop trait (otomatik)
// veya
defer! { cleanup_resource() }  // macro ile
```

---

## 🎯 Normalize Edici Kuralları Özet

### 1. Block Terminators

```
C/Go/Rust }           → end_X (context'e göre)
Python indentation    → end_X (indentation tracking)
Ruby/Lua end          → end_X (context tracking)
MELP "end X"          → end_X (underscore birleştir)
```

### 2. Keywords

```
C/Go/Rust if/while/for → if/while/for (aynı)
Python elif            → else_if
Ruby elsif             → else_if
Python def             → function
Rust fn                → function
Go func                → function
```

### 3. Operators

```
C/Go/Rust &&           → and
C/Go/Rust ||           → or
C/Go/Rust !            → not
Python and/or/not      → and/or/not (aynı)
```

### 4. Loop Control

```
C/Python/Go break      → exit
Ruby next              → continue
MELP "exit X"          → exit_X (underscore)
```

---

## 📝 Normalize Edici İmplementasyon Notları

### Context Tracking Gerekli:

1. **Brace Matching:**
   - `{` görünce stack'e push (IF/WHILE/FOR/FUNCTION)
   - `}` görünce stack'ten pop, context'e göre `end_X` üret

2. **Indentation Tracking (Python):**
   - Her satırın indent level'ını track et
   - Indent azalınca → block end
   - Context stack'e göre `end_X` üret

3. **Keyword Context (Ruby/Lua):**
   - `end` görünce stack'ten context al
   - IF/WHILE/FOR/FUNCTION hangisi → `end_X` üret

### Örnek Context Stack:

```
Kaynak:
if x > 5 {
    while y < 10 {
        print("Hello")
    }
}

Stack:
[IF] → { 
[IF, WHILE] → {
[IF, WHILE] → } → pop WHILE → end_while
[IF] → } → pop IF → end_if
```

---

## ✅ TODO: Normalize Edici Test Cases

```bash
# Test 1: C to PMPL
echo 'if (x > 5) { print("Yes"); }' | ./normalize --from=c
# Beklenen: if x > 5 then\n    print("Yes")\nend_if

# Test 2: Python to PMPL
echo 'if x > 5:\n    print("Yes")' | ./normalize --from=python
# Beklenen: if x > 5 then\n    print("Yes")\nend_if

# Test 3: MLP to PMPL
echo 'if x > 5 then\n    print("Yes")\nend if' | ./normalize --from=mlp
# Beklenen: if x > 5 then\n    print("Yes")\nend_if

# Sonuç: Üçü de AYNI PMPL çıktısı!
```

---

**Son Güncelleme:** 14 Aralık 2025  
**Amaç:** YZ_77 Normalize Edici İmplementasyonu için referans  
**Durum:** Taslak - Kullanıcı review ve genişletme yapacak

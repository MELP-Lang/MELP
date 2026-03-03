# PMLP2 Syntax Reference (Stage 2)

**Version:** 1.1  
**Date:** 19 Şubat 2026  
**Stage:** Stage 2 = PMLP0 + STO/SAO + Advanced Features  
**Timeline:** 10 weeks (March-May 2026)  
**Target:** S-Tier Performance (Rust competitor) 🦀  
**Kanonik Kaynak:** [`belgeler/pmlp_sozdizimi.md`](../pmlp_sozdizimi.md) (1994 satır)

---

## 🎯 STAGE 2 VISION: PRODUCTION-READY COMPILER

**Performance Target:**
```
Fibonacci(35): 30-40ms   (S-Tier, Rust level!)
Primes(10k):   6-10ms    (C level!)
Speedup:       3x        (vs Stage 1 baseline)
```

**Strategic Goals:**
- STO (Smart Type Optimization): Type inference → int64 native ops (2x speedup)
- SAO (Semantic-Aware Optimization): Metadata → LLVM aggressive opts (1.5x speedup)
- Combined: 3x total speedup = **Rust competitor!**

---

## ✅ STAGE 2 SYNTAX = STAGE 0 + ADVANCED FEATURES

Stage 2 maintains all Stage 0/1 syntax rules, plus:

### BASE SYNTAX (from Stage 0/1)

See [pmlp0_syntax.md](pmlp0_syntax.md) for complete rules:
- ✅ Semicolon (`;`) for parameter/array separators
- ✅ Block terminators: `end_if`, `end_while`, `end_for`, `end_function`
- ✅ Turkish number format: `3,14` (comma=decimal), `1.000` (dot=thousands)
- ✅ `then` required after `if` condition
- ✅ `else_if` as single token
- ✅ Logical operators: `and`, `or`, `not`
- ✅ Comments: `--`

---

## 🆕 STAGE 2 ADVANCED FEATURES (NOT YET IMPLEMENTED)

### 1. Module System (Week 1-2) 🆕

**Plugin Architecture + Import System:**
```pmlp2
-- Module definition
module math
    -- Export public functions
    export function square(numeric x) as numeric
        return x * x
    end_function
    
    -- Private helper (not exported)
    function helper() as numeric
        return 42
    end_function
end_module

-- Module import and usage
import math

function main() as numeric
    numeric result = math.square(10)  -- 100
    print(result)
    return 0
end_function
```

**Key Syntax Rules:**
- `module <name>` ... `end_module` - Module definition block
- `export` keyword - Mark functions/types as public
- `import <module>` - Import module (search paths: stdlib, local)
- Qualified access: `module_name.function_name()`

---

### 2. Generics (Week 6) 🆕

**Type Parameters for Functions:**
```pmlp2
-- Generic function definition
function max<T>(T a; T b) as T
    if a > b then
        return a
    else
        return b
    end_if
end_function

-- Usage with explicit type parameters
function main() as numeric
    numeric n = max<numeric>(10; 20)      -- 20
    text s = max<text>("abc"; "xyz")      -- "xyz"
    print(n)
    print(s)
    return 0
end_function
```

**Key Syntax Rules:**
- `<T>` - Type parameter declaration (after function name)
- `T` - Type parameter usage in parameters/return type
- `<numeric>`, `<text>`, etc. - Explicit type instantiation

---

### 3. Async/Await (Week 7+) 🆕

**Concurrent Execution:**
```pmlp2
-- Async function declaration
async function fetchData(text url) as text
    text result = await http_get(url)
    return result
end_function

-- Usage with await
function main() as numeric
    text data = await fetchData("https://api.example.com")
    print(data)
    return 0
end_function
```

**Key Syntax Rules:**
- `async` keyword - Mark function as asynchronous
- `await` keyword - Wait for async operation to complete
- Async functions return `Future<T>` internally

---

### 4. Lambda Functions (Week 8+) 🆕

**Anonymous Functions:**
```pmlp2
-- Higher-order function with lambda parameter
function map(numeric[] arr; lambda fn) as numeric[]
    numeric[] result = []
    for i = 0 to arr.length - 1
        result.push(fn(arr[i]))
    end_for
    return result
end_function

-- Usage with lambda expression
function main() as numeric
    numeric[] numbers = [1; 2; 3; 4; 5]
    numeric[] squared = map(numbers; lambda(x) => x * x)
    
    -- Result: [1; 4; 9; 16; 25]
    for i = 0 to squared.length - 1
        print(squared[i])
    end_for
    
    return 0
end_function
```

**Key Syntax Rules:**
- `lambda` type - Function parameter type
- `lambda(params) => expression` - Lambda expression syntax
- `=>` - Arrow operator (single expression body)

---

### 5. Pattern Matching (Week 9+) 🆕

**Advanced Control Flow:**
```pmlp2
-- Pattern matching on values
function classify(numeric value) as text
    match value
        case 0 => return "zero"
        case 1..10 => return "small"
        case 11..100 => return "medium"
        case _ => return "large"
    end_match
end_function

function main() as numeric
    print(classify(0))    -- "zero"
    print(classify(5))    -- "small"
    print(classify(50))   -- "medium"
    print(classify(200))  -- "large"
    return 0
end_function
```

**Key Syntax Rules:**
- `match <expression>` ... `end_match` - Pattern matching block
- `case <pattern> =>` - Pattern case (arrow syntax)
- `1..10` - Range pattern
- `_` - Wildcard pattern (matches anything)

---

## 🚀 STAGE 2 OPTIMIZATION FEATURES

### STO (Smart Type Optimization) - Week 3-5

**Type Inference and Specialization:**
```pmlp2
-- Before STO: numeric = boxed type (generic, slower)
function fibonacci(numeric n) as numeric
    if n <= 1 then
        return n
    end_if
    return fibonacci(n - 1) + fibonacci(n - 2)
end_function

-- After STO: numeric → int64 (specialized, 2x faster)
-- Compiler infers: n is always integer, never needs float
-- Generated LLVM IR uses native i64 operations
```

**Benefits:**
- 2x speedup from native operations
- No boxing/unboxing overhead
- LLVM can optimize int64 better than boxed types

---

### SAO (Semantic-Aware Optimization) - Week 3-5

**Semantic Metadata for LLVM:**
```pmlp2
-- Pure function (no side effects) → aggressive inlining
function square(numeric x) as numeric
    return x * x
end_function

-- Immutable data → compiler can reorder/parallelize
numeric result = square(10) + square(20)

-- SAO adds LLVM metadata:
-- - "pure" attribute → inline aggressively
-- - "immutable" hint → enable vectorization
-- - "no-alias" → optimize memory access
```

**Benefits:**
- 1.5x speedup from LLVM optimizations
- Inlining, loop vectorization, dead code elimination
- Combined with STO: 3x total speedup

---

## 🎉 MODERN PHASE FEATURES (Stage 1 Phase 2+ / Stage 2)

> **Kaynak:** `pmlp_sozdizimi.md` L1580+

### Composition Operators (`>>` / `<<`)

```pmlp2
-- >> Right-to-left composition (mathematical)
function double(numeric x) as numeric
    return x * 2
end_function
function inc(numeric x) as numeric
    return x + 1
end_function

function composed = double >> inc    -- inc(double(x))
numeric r1 = composed(5)             -- 11

-- << Left-to-right composition
function composed2 = double << inc   -- double(inc(x))
numeric r2 = composed2(5)            -- 12
```

**Tokens:** `TOKEN_COMPOSE_RIGHT` (`>>`) | `TOKEN_COMPOSE_LEFT` (`<<`)
**Status:** Parser implemented (runtime support pending)

---

### Pipe Operator (`|>`)

```pmlp2
-- Traditional style (nested, hard to read):
numeric result = sum(map(filter([1;2;3;4;5]; x => x > 2); x => x * 2))

-- Pipe style (linear, readable):
numeric result = [1; 2; 3; 4; 5]
    |> filter(x => x > 2)
    |> map(x => x * 2)
    |> sum()
-- result = 24

-- Semantics: x |> f(a) compiles to f(x; a)
```

**Token:** `TOKEN_PIPE` (`|>`)
**Dependencies:** Lambda (Week 2)

---

### Type Aliases

```pmlp2
-- Type alias definition
type UserId = numeric
type Email = string
type UserResult = Result{User; Error}

function find_user(UserId id) as UserResult
    -- implementation
end_function

UserId current_id = 42
Email user_email = "ali@example.com"
```

**Token:** `TOKEN_TYPE`  
**Syntax:** `type NewName = ExistingType`  
**Semantik:** Compile-time alias, runtime overhead yok

---

### Channels & Concurrency

```pmlp2
-- Channel type (generic)
Channel{numeric} ch = make_channel{numeric}(buffer_size: 10)

-- Send (non-blocking with buffer)
ch.send(42)

-- Receive (blocking)
numeric value = ch.receive()

-- Spawn concurrent task
spawn compute_task(ch; [1; 2; 3; 4; 5])

-- Channel iteration (until closed)
for each result in ch
    print(result)
end_for

-- Select (multiple channels)
select
    case value = ch1.receive():
        print("From ch1: " + value)
    case value = ch2.receive():
        print("From ch2: " + value)
    case timeout(1000):
        print("Timeout!")
end_select
```

**Tokens:** `TOKEN_SPAWN` | `TOKEN_SELECT` | `TOKEN_TIMEOUT` | `Channel{T}`

---

### Result{T;E} ve Option{T}

```pmlp2
-- Result{T;E}: Başarı veya hata
function divide(numeric a; numeric b) as Result{numeric; string}
    if b == 0 then
        return Err("Sıfıra bölme")
    else
        return Ok(a / b)
    end_if
end_function

Result{numeric; string} res = divide(10; 2)
match res
    case Ok(value):
        print("Sonuç: " + value)
    case Err(msg):
        print("Hata: " + msg)
end_match

-- Zincirleme (monadic)
Result{numeric; string} final = divide(100; 5)
    .and_then(x => divide(x; 2))
    .map(x => x + 10)

-- Option{T}: Değer var/yok
function find_user(string name) as Option{string}
    if user_exists(name) then
        return Some(name)
    else
        return None
    end_if
end_function

string display = find_user("Ali").unwrap_or("Bilinmiyor")
```

---

### Hata Propagasyonu (`?` operatörü)

```pmlp2
-- ? ile otomatik hata propagasyonu
function process(string path) as Result{string; IOError}
    string content = read_file(path)?   -- Hata varsa otomatik döndür
    string processed = transform(content)?
    return Ok(processed)
end_function

-- Custom throwe  
function validate(string input) as Result{boolean; CustomError}
    if input.length < 5 then
        return Err({message: "Too short"; code: 400})
    end_if
    return Ok(true)
end_function
```

---

### Traits (Rust-Style External Impl)

> ⚠️ **CRITICAL:** Method-in-struct YASAK! (OOP violation)
> Rust-style external impl ZORUNLU (STRUCT+FUNC prensibi)

```pmlp2
-- Trait definition
trait Printable
    function to_string() as string
end_trait

trait Comparable{T}
    function compare(T other) as numeric
end_trait

-- Struct: sadece data (NO METHODS)
struct Point
    numeric x
    numeric y
end_struct

-- External implementation (DOGRU: STRUCT+FUNC)
implement Printable for Point
    function to_string(Point self) as string
        return "Point(" + self.x + "; " + self.y + ")"
    end_function
end_implement

implement Comparable{Point} for Point
    function compare(Point self; Point other) as numeric
        return (self.x + self.y) - (other.x + other.y)
    end_function
end_implement

-- Generic function with trait bound
function print_all{T: Printable}(T[] items)
    for each item in items
        print(to_string(item))
    end_for
end_function
```

**Tokens:** `TOKEN_TRAIT` | `TOKEN_IMPLEMENT` | `TOKEN_END_IMPLEMENT`

---

### Parser Disambiguation (Generic vs Literal)

| Syntax | Context | Anlam | Örnek |
|--------|---------|-------|--------|
| `{T}` | Tip pozisyonu | Generic param | `function foo{T}(T x)` |
| `{k: v}` | Değer pozisyonu | Struct literal | `Point p = {x: 10; y: 20}` |
| `<v; v>` | Değer pozisyonu | Tuple literal | `tuple t = <10; 20>` |

---

## 📅 IMPLEMENTATION TIMELINE

| Feature | Week | Status | Token |
|---------|------|--------|-------|
| Generic Types | Week 1 D1-2 | ✅ DONE | `{T}` |
| Import/Module | Week 1 D3-4 | ✅ DONE | `import/export` |
| Type Aliases | Week 1 D5 | 🚀 NEW | `type` |
| Pattern Match | Week 2 | 🚧 WIP | `match/case` |
| Lambda/Closure | Week 2 | 🚧 WIP | `=>` |
| Pipe Operator | Week 2 D5-6 | 🚀 NEW | `\|>` |
| Channels | Week 2 | ⏳ TODO | `Channel{T}` |
| Error Handling | Week 2 | ⏳ TODO | `try/catch` |
| Traits (Rust) | Week 2-3 | 🚀 NEW | `trait/implement` |
| Composition | Week 2 | 🔧 PARTIAL | `>>/<<` |

---

## 📖 LEARNING RESOURCES

**For Complete Syntax Rules:**
- [pmlp0_syntax.md](pmlp0_syntax.md) - Base Stage 0 reference
- [pmlp1_syntax.md](pmlp1_syntax.md) - Stage 1 reference (8 yeni özellik)
- [Feature Cards](feature_cards/) - Focused syntax guides

**For Golden Reference:**
- [golden_reference0.mlp](golden_reference0.mlp) - Stage 0 validation
- [golden_reference1.mlp](golden_reference1.mlp) - Stage 1 skeleton
- [golden_reference2.mlp](golden_reference2.mlp) - Stage 2 skeleton (with placeholders)

**For Implementation Details:**
- [TODO_STAGE2_STO_SAO/TODO_STAGE2.md](../../TODO_STAGE2_STO_SAO/TODO_STAGE2.md) - 10-week roadmap

---

## ⚠️ ANTI-PATTERNS

**Stage 2 maintains all Stage 0 anti-patterns:**
```pmlp2
-- ❌ WRONG: Semicolon at statement end
numeric x = 10;

-- ❌ WRONG: Comma in array/parameters
function test(numeric a, numeric b) as numeric  -- Use semicolon!

-- ❌ WRONG: Missing then
if x > 10
    print(x)
end_if

-- ❌ WRONG: else if (two tokens)
else if x > 10 then  -- Use else_if!
```

**New Stage 2 Anti-Patterns:**
```pmlp2
-- ❌ WRONG: Space in type parameter
function max< T >(T a; T b) as T  -- No spaces!

-- ❌ WRONG: Lambda with braces
lambda(x) { return x * x }  -- Use =>

-- ❌ WRONG: Match without arrow
match value
    case 0: return "zero"  -- Use =>
end_match

-- ❌ WRONG: async without await
async function test() as text
    text result = http_get("url")  -- Missing await!
    return result
end_function
```

---

## 📅 IMPLEMENTATION TIMELINE

**Phase 0: Pre-Flight (Week 0)** - MANDATORY
- LLVM metadata feasibility spike
- GO/NO-GO decision
- Risk mitigation

**Phase 1: STO Implementation (Week 1-3)**
- Week 1-2: Module system + Type inference engine
- Week 3: STO core (int64/float64 specialization)

**Phase 2: SAO Implementation (Week 3-5)**
- Week 3-4: Semantic metadata injection
- Week 5: LLVM optimization hooks

**Phase 3: Advanced Features (Week 6-9+)**
- Week 6: Generics
- Week 7+: Async/await
- Week 8+: Lambda functions
- Week 9+: Pattern matching

**Phase 4: Polish & Validation (Week 10)**
- Benchmarks (target: 3x speedup)
- Documentation
- Release prep

---

## 🎯 SUCCESS CRITERIA

**Performance Benchmarks:**
- ✅ Fibonacci(35): 30-40ms (Rust level)
- ✅ Primes(10k): 6-10ms (C level)
- ✅ 3x speedup vs Stage 1 baseline

**Feature Completeness:**
- ✅ Module system (plugin architecture)
- ✅ STO (type inference + specialization)
- ✅ SAO (semantic metadata + LLVM opts)
- ✅ Generics (type parameters)
- ⚠️ Async/await, Lambda, Pattern matching (stretch goals)

---

**Last Updated:** 19 Şubat 2026  
**Status:** Phase 2+ içeriği eklendi (pmlp_sozdizimi.md L1580+ ile hizalandı)  
**Reference:** [pmlp_sozdizimi.md](../pmlp_sozdizimi.md)

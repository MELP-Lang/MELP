# PMLP Sözdizimi İyileştirme Önerileri

**Tarih:** 26 Ocak 2026  
**Kaynak:** belgeler/pmlp_sozdizimi.md analiz raporu  
**Durum:** Öneri belgesi (implement edilmedi)

---

## ✅ Tamamlanan Düzeltmeler (26 Ocak 2026)

### 1. Parametre Format Tutarlılığı
- **Sorun**: Bazı yerlerde `tip isim`, bazı yerlerde `isim: tip`
- **Çözüm**: Tüm belgede `tip isim` formatına geçildi
- **Etkilenen bölümler**: Fonksiyon tanımları, parametreler, struct fields

### 2. `returns` Keyword Temizliği
- **Sorun**: `returns` deprecated olmasına rağmen hala kullanılıyordu
- **Çözüm**: Tüm `returns` kullanımları `as` ile değiştirildi
- **Örnek**: `function foo() returns numeric` → `function foo() as numeric`

### 3. Modifier Syntax Tutarlılığı
- **Sorun**: `ref tip isim formatı` gibi gereksiz açıklamalar
- **Çözüm**: Sadece `ref tip isim` şeklinde kısa ve net format

---

## 🎯 Semantik Analiz Sonuçları

### ✅ MELP Felsefesine Uygun

1. **Type-first pattern**: `numeric x`, `string name` ✅
2. **Semicolon ayırıcı**: Parametreler, array, list, tuple hepsinde `;` ✅
3. **Trailing semicolon yasak**: `[1; 2; 3]` ✅, `[1; 2; 3;]` ❌
4. **Single token keywords**: `end_if`, `end_while`, `else_if` ✅
5. **`exit` not `break`**: MELP'te `exit` kullanılır ✅
6. **Türk sayı formatı**: `3,14` (ondalık), `1.000` (binlik) ✅

### ⚠️ Dikkat Edilmesi Gerekenler

#### 1. Generic Syntax Ambiguity

**Kritik durum:**
```pmpl
-- Generic type parameter (type position)
function first{T}(T[] items) as T    -- {T} = type parameter

-- Struct literal (value position)
Point p = {x: 10, y: 20}             -- {k: v} = struct literal

-- Tuple literal (value position)
tuple pos = <10; 20>                 -- <v; v> = tuple literal
```

**Parser için kural:**
- Type position'da `{T}` = Generic
- Value position'da `{k: v}` = Struct literal (colon ile)
- Value position'da `<v; v>` = Tuple literal (semicolon ile)

#### 2. Machine Readability

PMLP makine tarafından okunduğu için:
- Section headers makine-parsable olabilir (opsiyonel)
- EBNF grammar ayrı dosyada tutulabilir (opsiyonel)
- Token mapping JSON'da zaten var (✅ mevcut)

**Öneri:** Şu anki format yeterli, gerekirse JSON schema ayrı tutulur.

---

## 🚀 Önerilen Yeni Özellikler

### Öncelik 0 (Temel, hemen eklenebilir)

#### 1. Type Aliases

**Amaç:** Kod okunabilirliği ve tip güvenliği

```pmpl
-- Type alias tanımı
type UserId = numeric
type Email = string
type UserResult = Result{User, Error}

function find_user(UserId id) as UserResult
    -- implementation
end_function

-- Kullanım
UserId current_id = 42
Email user_email = "ali@example.com"
```

**Implementation:**
- **Stage:** Week 1 Day 5 (basit parser addition)
- **Token:** `TOKEN_TYPE`
- **Syntax:** `type NewName = ExistingType`
- **Semantik:** Compile-time alias, runtime overhead yok

**Avantajlar:**
- Kod okunabilirliği artırır
- Self-documenting code
- Type safety (numeric vs UserId ayrımı)
- Refactoring kolaylığı

---

### Öncelik 1 (Modern features)

#### 2. Traits/Interfaces

**Amaç:** Generic constraints ve polymorphism

```pmpl
-- Trait definition
trait Printable
    function to_string() as string
end_trait

trait Comparable{T}
    function compare(T other) as numeric
end_trait

-- Implement trait for struct
struct Point implements Printable, Comparable{Point}
    numeric x
    numeric y
    
    function to_string() as string
        return "Point(" + x + ", " + y + ")"
    end_function
    
    function compare(Point other) as numeric
        return (x + y) - (other.x + other.y)
    end_function
end_struct

-- Generic with trait bound
function print_all{T: Printable}(T[] items)
    for each item in items
        print(item.to_string())
    end_for
end_function

-- Multiple trait bounds
function sort_and_print{T: Printable + Comparable{T}}(T[] items)
    sorted = items.sort()  -- Comparable required
    for each item in sorted
        print(item.to_string())  -- Printable required
    end_for
end_function
```

**Implementation:**
- **Stage:** Week 2-3 (After generics)
- **Token:** `TOKEN_TRAIT` | `TOKEN_IMPLEMENTS`
- **Dependencies:** Generic types (Week 1)

**Avantajlar:**
- Generic constraints (bounded polymorphism)
- Interface-based polymorphism
- Duck typing alternatifi (compile-time check)
- Java/Rust/Swift style traits

---

#### 3. Pipe Operator (Functional Composition)

**Amaç:** Unix-style pipeline, functional programming

```pmpl
-- Traditional style (nested calls)
numeric result = sum(map(filter([1;2;3;4;5], x => x > 2), x => x * 2))

-- Pipe style (linear flow)
numeric result = [1; 2; 3; 4; 5]
    |> filter(x => x > 2)
    |> map(x => x * 2)
    |> sum()
-- result = 24 (3*2 + 4*2 + 5*2)

-- Complex pipeline
string output = read_file("data.txt")
    |> parse_json()
    |> extract_field("users")
    |> filter(u => u.age > 18)
    |> map(u => u.name)
    |> join(", ")
-- "Ali, Veli, Ayşe"
```

**Implementation:**
- **Stage:** Week 2 (Syntactic sugar over function calls)
- **Token:** `TOKEN_PIPE` (`|>`)
- **Dependencies:** Lambda functions (Week 2)
- **Semantik:** `x |> f(a)` → `f(x, a)`

**Avantajlar:**
- Okunabilir functional code
- Left-to-right data flow (natural)
- F#, Elixir, OCaml style
- No runtime overhead (compile-time transformation)

---

#### 4. Async/Await (Go-style concurrency alternatifi)

**Amaç:** Modern async/await pattern

```pmpl
-- Async function
async function fetch_data(string url) as Result{string, Error}
    string response = await http.get(url)
    return Ok(response)
end_function

-- Multiple concurrent operations
async function fetch_all(string[] urls) as Result{string[], Error}
    Result{string, Error}[] results = []
    
    for each url in urls
        -- Spawn concurrent task
        results.add(async_spawn(fetch_data(url)))
    end_for
    
    -- Wait all
    return await_all(results)
end_function

-- Usage
async function main()
    Result{string, Error} data = await fetch_data("https://api.example.com")
    match data
        case Ok(content):
            print(content)
        case Err(e):
            print("Error: " + e.message)
    end_match
end_function
```

**Implementation:**
- **Stage:** Week 3-4 (Advanced concurrency)
- **Token:** `TOKEN_ASYNC` | `TOKEN_AWAIT` | `TOKEN_ASYNC_SPAWN` | `TOKEN_AWAIT_ALL`
- **Dependencies:** Channels (Week 2), Result{T,E} (Week 1)

**Avantajlar:**
- Modern async pattern (JavaScript, C#, Rust, Python style)
- Channel alternatifi (daha ergonomik)
- Cooperative multitasking
- Event loop based (green threads)

**vs Channels:**
- Channels: Go-style CSP (message passing)
- Async/Await: Modern async pattern (promises/futures)
- Both are useful for different scenarios

---

### Öncelik 2 (Advanced features)

#### 5. Macro System

**Amaç:** Compile-time code generation, DSL

```pmpl
-- Macro definition
macro debug_print(expr)
    print("DEBUG [" + __FILE__ + ":" + __LINE__ + "]: " + stringify(expr) + " = ")
    println(expr)
end_macro

-- Usage
numeric x = 42
debug_print(x + 10)
-- Output: "DEBUG [main.mlp:15]: x + 10 = 52"

-- Procedural macro
macro impl_getters(struct_name)
    -- Generate getter functions for all fields
    for each field in struct_name.fields
        function get_{field.name}() as {field.type}
            return this.{field.name}
        end_function
    end_for
end_macro

struct Person
    string name
    numeric age
end_struct

impl_getters(Person)  -- Generates get_name(), get_age()
```

**Implementation:**
- **Stage:** Week 4+ (Advanced metaprogramming)
- **Token:** `TOKEN_MACRO`
- **Dependencies:** Full AST access
- **Complexity:** High (requires macro hygiene, expansion order)

---

## 📊 Feature Comparison Matrix

| Feature | Priority | Complexity | Value | Dependencies | Week |
|---------|----------|------------|-------|--------------|------|
| **Type Aliases** | P0 | Low | High | None | Week 1 Day 5 |
| **Traits/Interfaces** | P1 | Medium | High | Generics | Week 2-3 |
| **Pipe Operator** | P1 | Low | Medium | Lambda | Week 2 |
| **Async/Await** | P1 | High | High | Channels, Result | Week 3-4 |
| **Macro System** | P2 | Very High | Medium | Full AST | Week 4+ |

---

## 🎯 Implementation Roadmap

### Week 1 (27-31 Ocak 2026)
- ✅ Generics (Day 1-2)
- ✅ Import/Module (Day 3-4)
- 🚀 **Type Aliases** (Day 5) ← NEW

### Week 2 (3-7 Şubat 2026)
- Lambda/Closure
- Pattern Matching
- **Pipe Operator** ← NEW (easy win)
- **Traits foundation** ← NEW (start)

### Week 3 (10-14 Şubat 2026)
- Channels & Concurrency
- Error Handling
- **Async/Await foundation** ← NEW

### Week 4+ (17+ Şubat 2026)
- **Traits implementation** (continue)
- **Async/Await completion**
- Performance optimization
- (Future: Macro system)

---

## 🔧 Belge İyileştirmeleri

### 1. Parser Warning Section Ekle

**Öneri:** `{T}` vs `{k: v}` vs `<v; v>` ambiguity için ayrı section

```markdown
## ⚠️ PARSER WARNINGS - CRITICAL DISAMBIGUATION

### Generic vs Literal Syntax

**Context:** Three different uses of braces/angle brackets

| Syntax | Context | Meaning | Example |
|--------|---------|---------|---------|
| `{T}` | Type position | Generic parameter | `function foo{T}(T x)` |
| `{k: v}` | Value position | Struct literal | `Point p = {x: 10, y: 20}` |
| `<v; v>` | Value position | Tuple literal | `tuple t = <10; 20>` |

**Parser Rule:**
1. After `struct`/`function` name → Generic `{T}`
2. In expression, with colon → Struct `{k: v}`
3. In expression, angle brackets → Tuple `<v; v>`
```

### 2. Modern Features Timeline Matrix

**Öneri:** Implementation priority ve dependencies tablosu

```markdown
## 🗓️ MODERN Features Implementation Timeline

| Feature | Week | Dependencies | Status | Priority | Complexity |
|---------|------|--------------|--------|----------|------------|
| Generics | Week 1 | None | ✅ DONE | P0 | Medium |
| Import/Module | Week 1 D3 | None | ✅ DONE | P0 | Low |
| Type Aliases | Week 1 D5 | None | 🚀 NEW | P0 | Low |
| Pipe Operator | Week 2 | Lambda | 🚀 NEW | P1 | Low |
| Pattern Match | Week 2 | Generics | 🚧 WIP | P0 | Medium |
| Lambda/Closure | Week 2 | Generics | 🚧 WIP | P0 | High |
| Traits | Week 2-3 | Generics | 🚀 NEW | P1 | Medium |
| Channels | Week 2 | Generics | ⏳ TODO | P1 | High |
| Error Handling | Week 2 | Result{T,E} | ⏳ TODO | P1 | Medium |
| Async/Await | Week 3-4 | Channels | 🚀 NEW | P1 | High |
| Macro System | Week 4+ | Full AST | 💡 IDEA | P2 | Very High |
```

### 3. EBNF Grammar (Opsiyonel)

**Öneri:** Formal grammar ayrı dosyada (machine-parsable)

Dosya: `belgeler/pmlp_grammar.ebnf`

```ebnf
(* PMPL Grammar - Formal Specification *)

program = { declaration } ;

declaration = function_decl
            | struct_decl
            | enum_decl
            | import_decl
            | type_alias ;

function_decl = "function" , identifier , [ generic_params ] , 
                "(" , [ param_list ] , ")" , 
                "as" , type , 
                statement_block , 
                "end_function" ;

generic_params = "{" , identifier , { "," , identifier } , "}" ;

param_list = param , { ";" , param } ;

param = [ modifier ] , type , identifier , [ ":=" , expression ] ;

modifier = "ref" | "const" | "out" | "named" ;

type = identifier 
     | type , "[]"              (* array *)
     | type , "?"               (* nullable *)
     | type , "|" , type        (* union *)
     | identifier , "{" , type_list , "}" ;  (* generic *)

(* ... rest of grammar ... *)
```

---

## 📝 Sonuç

### Tamamlanan Düzeltmeler ✅
1. Parametre format tutarlılığı (`tip isim`)
2. `returns` keyword temizliği (→ `as`)
3. Modifier syntax tutarlılığı

### Belgein Mevcut Durumu ✅
- **Semantik tutarlılık**: MELP felsefesine uygun
- **Machine readability**: Yeterli (JSON token mapping ayrıca var)
- **Completeness**: Core features tamam, modern features dokümante edilmiş

### Önerilen Eklentiler 🚀
1. **Type Aliases** - P0, Week 1 Day 5 (hemen eklenebilir)
2. **Traits/Interfaces** - P1, Week 2-3 (generic constraints için kritik)
3. **Pipe Operator** - P1, Week 2 (functional programming sugar)
4. **Async/Await** - P1, Week 3-4 (modern concurrency)

### Belge İyileştirmeleri 📋
1. Parser warning section (ambiguity açıklama)
2. Timeline matrix (implementation roadmap)
3. EBNF grammar (opsiyonel, formal spec)

**Durum:** Belge production-ready, öneriler enhancement amaçlı.

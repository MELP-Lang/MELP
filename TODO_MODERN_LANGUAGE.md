# 🚀 MELP TODO - Modern Eksiksiz Dil Hedefi

**Tarih:** 21 Aralık 2025  
**Hedef:** %100 Eksiksiz Modern Programlama Dili  
**Vizyon:** Her modern dilde olan temel özellikler (niş değil, zorunlu)

---

## 🎯 DURUM ANALİZİ

### ✅ Mevcut Başarılar (Stage 0 - %100)
- Derleyici: C ile yazılmış, LLVM backend, production ready
- Veri Tipleri: numeric, string, boolean
- Kontrol Yapıları: if/else, while, for
- Fonksiyonlar: declaration, parameters, return
- Struct: definitions, fields, methods
- Enum: definitions, qualified access
- Switch/Case: pattern matching
- Strings: concat, compare, UTF-8
- Arrays: basic indexing (assembly)
- Comments: single-line (--), multi-line
- Runtime: STO (safe memory), stdlib

### ❌ Eksikler (Modern Dil Standardı)
Kritik raporda tespit edildi:
1. **List/Collection Type:** Stage 1 modüllerde kullanılıyor ama Stage 0'da yok!
2. **Type System:** Generic types yok, sadece primitive types
3. **Module System:** Import/export yok, tüm kod tek dosyada
4. **Error Handling:** Exception/result pattern yok
5. **Memory Management:** Manual allocation, GC yok
6. **Closures:** First-class functions yok
7. **Standard Library:** Minimal, genişletilmeli
8. **Package Manager:** Dependency management yok
9. **Concurrency:** Thread/async desteği yok
10. **Interop:** C FFI çağrı yok

---

## 📋 YENİ TODO - PHASE BAZLI (Her Phase = 1 YZ Görevi)

### 🔴 PHASE 1: CORE TYPE SYSTEM (Zorunlu) [4 hafta]

#### ✅ YZ_200: List Type Implementation [1 hafta] - TAMAMLANDI
**Öncelik:** 🔴 KRİTİK (Stage 1 bloker!)  
**Neden:** Stage 1 modüller `list` kullanıyor, Stage 0 desteklemiyor  
**Durum:** ✅ **TAMAMLANDI** (21 Aralık 2025)  
**Rapor:** `LLVM_YZ/YZ_200_TAMAMLANDI.md`

**Yapılanlar:**
- [x] List type definition (dynamic array) ✅
- [x] List operations: append, get, length ✅
- [x] List literals: `(1; 2; 3;)` ✅
- [x] List indexing: `list(0)` ✅
- [x] LLVM IR codegen ✅
- [x] Type safety: Variable type tracking ✅
- [x] Memory management: STO integration ✅

**⚠️ SYNTAX NOTLARI:**
- List literal: `()` (parantez)
- Array literal: `[]` (köşeli parantez)
- Parametre ayırıcı: `;` (noktalı virgül, virgül DEĞİL!)
- Trailing semicolon: ZORUNLU `(1; 2; 3;)`

**Test Cases:**
```pmpl
function test_list() returns numeric
    list numbers = (1; 2; 3; 4; 5;)  -- ✅ List syntax: () + ; ayırıcı
    numeric sum = 0
    for i = 0 to length(numbers) - 1 do
        sum = sum + numbers(i)
    end_for
    return sum  -- Should return 15
end_function

-- Heterojen list (farklı tipler)
function test_mixed_list() returns list
    list mixed = (1; "hello"; 3,14; true;)  -- ✅ Heterojen: numeric, string, boolean
    return mixed
end_function

-- Boş list
function test_empty_list() returns list
    list empty = ()  -- ✅ Boş list
    return empty
end_function
```

**Dosyalar:**
- `compiler/stage0/modules/list/list.h`
- `compiler/stage0/modules/list/list_parser.c`
- `compiler/stage0/modules/list/list_codegen.c`
- `tests/llvm/08_list/test_*.mlp`

**Çıktı:** List type çalışıyor, Stage 1 modüller derlenebiliyor

---
#### YZ_201: Map/Dictionary Type [5 gün] ✅ TAMAMLANDI
**Öncelik:** 🟡 Yüksek (stdlib için gerekli)

**Yapılacaklar:**
- [x] Map type definition (hash table)
- [x] Map operations: insert, get, remove, has_key, length
- [x] Map literals: `{"key": value}`
- [ ] Map iteration: `for key, value in map` (future)
- [x] Type safety: key/value type constraints
- [x] LLVM IR codegen
- [x] Collision handling: chaining

**Test Cases:**
```pmpl
function test_map() returns numeric
    map person = {"name": "Alice"; "age": 30;}
    insert(person; "city"; "NYC")
    numeric age = get(person; "age")  -- Returns 30
    numeric has_name = has_key(person; "name")  -- Returns 1
    return age
end_function
```

**Tamamlanma:** 21 Aralık 2025 (YZ_201)

---

#### YZ_202: Optional Type (Null Safety) [3 gün - ENTEGRASYON]
**Öncelik:** 🟡 Yüksek (type safety için kritik)

⚠️ **STAGE 0'DA FRAMEWORK VAR:** `compiler/stage0/modules/null_safety/` (7 dosya)
- ⚠️ Null checking framework yazılmış, entegrasyon gerekli
- ⚠️ Önce mevcut kodu kontrol et, sonra entegre et!

**Yapılacaklar (ENTEGRASYON + TAMAMLAMA):**
- [ ] Mevcut null_safety modülünü incele
- [ ] Makefile'a ekle (null_safety.o vb.)
- [ ] Optional type: `optional numeric`, `optional string`
- [ ] None/null representation
- [ ] Safe unwrapping: `if value is not none then`
- [ ] Optional chaining: `obj?.field`
- [ ] Default value operator: `value ?? default`
- [ ] **Nullable collections:** `list?`, `array?`, `map?`
- [ ] **Empty vs null distinction:** `()` vs `null`
- [ ] Test et: `tests/null_safety/test_optional.mlp`

**Test Cases:**
```pmpl
function find_user(numeric id) returns optional string
    if id == 1 then
        return "Alice"
    end_if
    return none
end_function

function test_optional() returns string
    optional string user = find_user(99)
    return user ?? "Unknown"  -- Should return "Unknown"
end_function

-- Nullable collections
function get_numbers() returns list?
    if condition then
        return (1; 2; 3;)  -- Valid list
    end_if
    return null            -- Null list (farklı: boş list () değil!)
end_function

function test_null_list() returns numeric
    list? data = get_numbers();
    
    -- Null check (ZORUNLU!)
    if data == null then
        return 0;
    end_if
    
    -- Boş list check
    if length(data) == 0 then
        return -1;
    end_if
    
    return data(0);
end_function

-- Safe unwrapping
list? numbers = get_numbers();
numeric first = numbers?(0) ?? -1;  -- Null-safe indexing
```

**Null Safety Kuralları:**
```pmpl
-- ❌ HATA: Nullable tip, null check olmadan kullanılamaz
list? data = get_data();
numeric x = data(0);  -- COMPILE ERROR: data might be null!

-- ✅ DOĞRU: Null check
if data != null then
    numeric x = data(0);  -- OK, guaranteed non-null
end_if

-- ✅ DOĞRU: Safe unwrap with ??
numeric x = data?(0) ?? -1;  -- OK, returns -1 if null

-- ✅ DOĞRU: Optional chaining
numeric? x = data?.length();  -- Returns null if data is null
```

---

#### YZ_203: Generic Types (Type Parameters) [2 hafta]
**Öncelik:** 🟡 Yüksek (modern dil standardı)

**Yapılacaklar:**
- [ ] Generic function syntax: `function max<T>(T a, T b) returns T`
- [ ] Generic struct syntax: `struct Box<T>`
- [ ] Type parameter constraints
- [ ] Monomorphization (compile-time specialization)
- [ ] LLVM IR codegen per instance

**Test Cases:**
```pmpl
function max<T>(T a, T b) returns T
    if a > b then
        return a
    end_if
    return b
end_function

function test_generic() returns numeric
    return max<numeric>(10, 20)  -- Should return 20
end_function
```

---

### 🟡 PHASE 2: MODULE SYSTEM (Zorunlu) [2 hafta]

#### YZ_204: Module Import/Export [1 hafta]
**Öncelik:** 🔴 KRİTİK (code organization için gerekli)

**Yapılacaklar:**
- [ ] Module declaration: `module math`
- [ ] Export syntax: `export function add()`
- [ ] Import syntax: `import math`, `import math.add`
- [ ] Namespace resolution: `math.add()`
- [ ] Circular dependency detection
- [ ] Module search paths
- [ ] Precompiled module cache

**Test Cases:**
```pmpl
-- math.mlp
module math

export function add(numeric a, numeric b) returns numeric
    return a + b
end_function

-- main.mlp
import math

function main() returns numeric
    return math.add(10, 20)  -- Should return 30
end_function
```

**Dosyalar:**
- `compiler/stage0/modules/module/module_parser.c`
- `compiler/stage0/modules/module/module_resolver.c`
- `tests/modules/test_import.mlp`

---

#### YZ_205: Package Structure [5 gün]
**Öncelik:** 🟡 Orta (proje organizasyonu)

**Yapılacaklar:**
- [ ] Package definition: `package.mlp` manifest
- [ ] Package metadata: name, version, author
- [ ] Dependency list: `dependencies = ["stdlib:1.0"]`
- [ ] Build configuration
- [ ] Output directory structure
- [ ] Package verification

**Örnek package.mlp:**
```pmpl
package {
    name = "my_project",
    version = "1.0.0",
    author = "Developer",
    dependencies = ["stdlib:1.0"],
    entry = "src/main.mlp"
}
```

---

### 🟢 PHASE 3: ERROR HANDLING (Zorunlu) [2 hafta]

#### YZ_206: Result Type Pattern [1 hafta]
**Öncelik:** 🟡 Yüksek (error safety için kritik)

**Yapılacaklar:**
- [ ] Result type: `result<T, E>`
- [ ] Ok/Error constructors
- [ ] Pattern matching: `match result`
- [ ] Propagation operator: `?`
- [ ] LLVM IR codegen

**Test Cases:**
```pmpl
function divide(numeric a, numeric b) returns result<numeric, string>
    if b == 0 then
        return error("Division by zero")
    end_if
    return ok(a / b)
end_function

function test_result() returns numeric
    result<numeric, string> res = divide(10, 2)
    match res
        case ok(value):
            return value
        case error(msg):
            return -1
    end_match
end_function
```

---

#### YZ_207: Try-Catch Syntax (Alternative) [5 gün]
**Öncelik:** 🟢 Düşük (result pattern varsa opsiyonel)

**Yapılacaklar:**
- [ ] Try-catch-finally syntax
- [ ] Exception types
- [ ] Stack unwinding
- [ ] LLVM exception handling

**Not:** Result pattern tercih edilebilir (zero-cost)

---

### 🟣 PHASE 4: FIRST-CLASS FUNCTIONS (Zorunlu) [2 hafta]

#### YZ_208: Lambda/Anonymous Functions [3 gün - ENTEGRASYON]
**Öncelik:** 🟡 Yüksek (functional programming)

⚠️ **STAGE 0'DA YAZILMIŞ:** `compiler/stage0/modules/lambda/` (7 dosya)
- ✅ lambda.h/c - Lambda struct ve API
- ✅ lambda_parser.h/c - Lambda parsing
- ✅ lambda_codegen.h/c - LLVM codegen
- ✅ lambda_standalone.c - Standalone test

**Yapılacaklar (ENTEGRASYON ONLY):**
- [ ] Makefile'a ekle (lambda.o, lambda_parser.o, lambda_codegen.o)
- [ ] `functions_compiler`'a link et
- [ ] Test et: `tests/lambda/test_lambda.mlp`
- [ ] Closure capture: variables from outer scope (varsa kontrol et)
- [ ] Higher-order functions: map, filter, reduce (runtime'a ekle)

**Test Cases:**
```pmpl
function map(list items, function<numeric, numeric> fn) returns list
    list result = []
    for item in items do
        append(result, fn(item))
    end_for
    return result
end_function

function test_lambda() returns list
    list numbers = [1, 2, 3, 4, 5]
    return map(numbers, lambda (numeric x) { return x * 2 })
    -- Should return [2, 4, 6, 8, 10]
end_function
```

---

#### YZ_209: Function Pointers [3 gün]
**Öncelik:** 🟡 Orta (callback pattern)

**Yapılacaklar:**
- [ ] Function pointer type
- [ ] Function reference: `&function_name`
- [ ] Function call through pointer
- [ ] Vtable for methods

---

### 🔵 PHASE 5: MEMORY MANAGEMENT (Önemli) [3 hafta]

#### YZ_210: Reference Counting GC [1 hafta - ENTEGRASYON + GC]
**Öncelik:** 🟡 Yüksek (memory safety)

⚠️ **STAGE 0'DA YAZILMIŞ:** `compiler/stage0/modules/memory/` (7 dosya)
- ✅ memory.h/c - malloc/free/copy/move (yazılmış)
- ✅ memory_parser.h/c - Memory operations parsing
- ✅ memory_codegen.h/c - LLVM codegen

**Yapılacaklar:**
- [ ] Mevcut memory modülünü entegre et (Makefile + link)
- [ ] Test et: `tests/memory/test_malloc.mlp`
- [ ] **GC EKLE:** Reference counting (increment/decrement)
- [ ] Automatic cleanup: destructor calls
- [ ] Cycle detection: weak references
- [ ] STO upgrade: ref counting support

**Not:** Manuel memory zaten var, sadece GC eklenecek!

---

#### YZ_211: Move Semantics (Ownership) [1 hafta]
**Öncelik:** 🟢 Düşük (optimization)

**Yapılacaklar:**
- [ ] Move syntax: `move value`
- [ ] Ownership transfer
- [ ] Borrow checker (basit)
- [ ] Compile-time verification

---

### 🟠 PHASE 6: STANDARD LIBRARY (Zorunlu) [4 hafta]

#### YZ_212: Collections Library [1 hafta]
**Öncelik:** 🔴 Yüksek (temel yapılar)

**Yapılacaklar:**
- [ ] List: append, prepend, insert, remove, find, sort
- [ ] Map: insert, get, remove, keys, values
- [ ] Set: add, contains, union, intersection
- [ ] Queue: enqueue, dequeue, peek
- [ ] Stack: push, pop, peek

---

#### YZ_213: String Library [5 gün]
**Öncelik:** 🟡 Yüksek (text processing)

**Yapılacaklar:**
- [ ] String operations: split, join, replace, trim
- [ ] String searching: find, contains, starts_with, ends_with
- [ ] String formatting: format("{} + {} = {}", a, b, a+b)
- [ ] Regex: match, search, replace (basit)
- [ ] UTF-8 operations: length, substring, char_at

---

#### YZ_214: File I/O Library [5 gün]
**Öncelik:** 🟡 Yüksek (IO operations)

**Yapılacaklar:**
- [ ] File operations: open, close, read, write
- [ ] File modes: read, write, append
- [ ] Binary I/O: read_bytes, write_bytes
- [ ] Directory operations: list, create, remove
- [ ] Path operations: join, dirname, basename

---

#### YZ_215: Math Library [3 gün]
**Öncelik:** 🟢 Orta (numeric operations)

**Yapılacaklar:**
- [ ] Math functions: sqrt, pow, log, exp, sin, cos, tan
- [ ] Rounding: floor, ceil, round
- [ ] Constants: PI, E
- [ ] Random: random(), random_range(min, max)

---

### 🟤 PHASE 7: C FFI (Foreign Function Interface) [2 hafta]

#### YZ_216: C Function Calls [1 hafta]
**Öncelik:** 🟡 Yüksek (interop için kritik)

**Yapılacaklar:**
- [ ] External declaration: `extern "C" function printf()`
- [ ] Type mapping: MELP types → C types
- [ ] Calling convention: cdecl, stdcall
- [ ] Header file parsing (basit)
- [ ] Linking: static/dynamic libraries

**Test Cases:**
```pmpl
extern "C" function printf(string format, ...) returns numeric

function main() returns numeric
    printf("Hello from MELP!\n")
    return 0
end_function
```

---

#### YZ_217: Struct ABI Compatibility [1 hafta]
**Öncelik:** 🟡 Orta (C struct interop)

**Yapılacaklar:**
- [ ] C struct layout compatibility
- [ ] Padding and alignment
- [ ] Packed structs
- [ ] Opaque pointers

---

### 🟣 PHASE 8: CONCURRENCY (Gelişmiş) [4 hafta]

#### YZ_218: Threading Support [2 hafta]
**Öncelik:** 🟢 Düşük (advanced feature)

**Yapılacaklar:**
- [ ] Thread creation: `spawn thread { ... }`
- [ ] Thread joining: `join thread`
- [ ] Mutex: lock, unlock
- [ ] Channel: send, receive
- [ ] Thread-local storage

**Alternatif:** Async/await pattern (daha modern)

---

#### YZ_219: Async/Await [5 gün - ENTEGRASYON]
**Öncelik:** 🟢 Düşük (modern async)

⚠️ **STAGE 0'DA YAZILMIŞ:** `compiler/stage0/modules/async/` (7 dosya)
- ✅ async.h/c - Future/Promise + async runtime (227 satır!)
- ✅ async_parser.h/c - async/await syntax parsing
- ✅ async_codegen.h/c - LLVM coroutine codegen
- ✅ async_standalone.c - Standalone test

**Mevcut Özellikler (Yazılmış):**
- ✅ Future/Promise types
- ✅ async function, async block, async closure
- ✅ await keyword (.await modern syntax)
- ✅ await_all, await_any, await_race
- ✅ Event loop + executor (single-thread, thread-pool, work-stealing)

**Yapılacaklar (ENTEGRASYON ONLY):**
- [ ] Makefile'a ekle (async.o, async_parser.o, async_codegen.o)
- [ ] `functions_compiler`'a link et
- [ ] Runtime'a event loop ekle
- [ ] Test et: `tests/async/test_async.mlp`
- [ ] LLVM coroutine support kontrol et (zaten yazılmış olabilir)

---

### 🔴 PHASE 9: TOOLING (Zorunlu) [3 hafta]

#### YZ_220: Build System [1 hafta]
**Öncelik:** 🟡 Yüksek (developer experience)

**Yapılacaklar:**
- [ ] Build command: `melp build`
- [ ] Incremental compilation
- [ ] Dependency tracking
- [ ] Parallel compilation
- [ ] Build cache

---

#### YZ_221: Package Manager [1 hafta]
**Öncelik:** 🟡 Yüksek (ecosystem)

**Yapılacaklar:**
- [ ] Package registry: central repository
- [ ] Package installation: `melp install package`
- [ ] Version resolution: semantic versioning
- [ ] Lock file: `package.lock`
- [ ] Package publishing: `melp publish`

---

#### YZ_222: Formatter & Linter [1 hafta]
**Öncelik:** 🟢 Orta (code quality)

**Yapılacaklar:**
- [ ] Code formatter: `melp fmt`
- [ ] Linter: `melp lint`
- [ ] Style configuration: `.melp-format`
- [ ] Auto-fix: `melp lint --fix`

---

### 🟢 PHASE 10: OPTIMIZATION (Gelişmiş) [4 hafta]

#### YZ_223: LLVM Optimization Pipeline [1 hafta]
**Öncelik:** 🟢 Düşük (performance)

**Yapılacaklar:**
- [ ] Optimization levels: -O0, -O1, -O2, -O3
- [ ] Inlining heuristics
- [ ] Dead code elimination
- [ ] Constant folding
- [ ] Loop optimization

---

#### YZ_224: Link-Time Optimization (LTO) [1 hafta]
**Öncelik:** 🟢 Düşük (advanced optimization)

**Yapılacaklar:**
- [ ] Whole-program optimization
- [ ] Cross-module inlining
- [ ] Devirtualization
- [ ] LLVM LTO integration

---

### 🟣 PHASE 11: SYNTAX IMPROVEMENTS (Ergonomi) [2 hafta]

#### YZ_225: Line Continuation Support (VB.NET Stili) [3 gün]
**Öncelik:** 🟡 Orta (developer experience)

**Yapılacaklar:**
- [ ] `_` (underscore) line continuation character
- [ ] Uzun string literalleri bölme desteği
- [ ] Fonksiyon parametrelerini çok satıra bölme
- [ ] Uzun expression'ları bölme
- [ ] Lexer: `_` + newline → ignore
- [ ] Parser: Multi-line string support

**Test Cases:**
```pmpl
-- Uzun string bölme
string message = "Bu çok uzun bir mesaj " _
                 "ve birden fazla satıra " _
                 "bölünmüş durumda"

-- Fonksiyon parametreleri
result = calculate_something( _
    param1; _
    param2; _
    param3 _
)

-- Uzun expression
numeric total = first_value + _
                second_value + _
                third_value
```

**Dosyalar:**
- `compiler/stage0/modules/lexer/lexer.c` - Line continuation handling
- `compiler/stage0/modules/parser/string_parser.c` - Multi-line strings
- `tests/syntax/test_line_continuation.mlp`

**Referans:** VB.NET line continuation syntax

---

#### YZ_226: Comment Syntax Standardization [2 gün]
**Öncelik:** 🟡 Orta (documentation)

**Yapılacaklar:**
- [ ] Tek satır yorum: `--` (mevcut) ✅
- [ ] Çok satırlı yorum: `---` ... `---` (düzelt)
- [ ] Dokümantasyon yorumu: `---/` ... `/---`
- [ ] Nested comment support (opsiyonel)
- [ ] Comment preservation in AST (for formatter)

**Syntax Önerisi:**
```pmpl
-- Tek satır yorum (mevcut) ✅

---
Çok satırlı yorum
Birden fazla satır
---

---/
Dokümantasyon yorumu (Help, IDE için)
@param x: İlk parametre
@return: Sonuç değeri
/---

function calculate(numeric x) as numeric
    return x * 2
end_function
```

**Dosyalar:**
- `pmlp_kesin_sozdizimi.md` - Syntax düzeltmesi
- `compiler/stage0/modules/lexer/lexer.c` - Comment parsing
- `tests/syntax/test_comments.mlp`

**⚠️ NOT:** 
- Çok satırlı: `---` ... `---` (simetrik)
- Dokümantasyon: `---/` ... `/---` (slash direction shows type)

---

#### YZ_228: Numeric Underscore Separator Support [2 gün]
**Öncelik:** 🟡 Orta (readability, modern standard)

**Neden Gerekli:**
Modern dillerde standart (Python, Rust, Swift, C++14, Java 7+). Büyük sayıları okumayı kolaylaştırır.

**Yapılacaklar:**
- [ ] Lexer: `_` (underscore) numeric literal içinde izin ver
- [ ] Binlik ayırıcı: `1_000_000` (bir milyon)
- [ ] Ondalık sayılarda: `3_141_592,653` 
- [ ] Kombinasyon: Türk format + underscore: `100_443_000,55`
- [ ] Validation: `_` başta/sonda olamaz (`_123`, `123_` ❌)
- [ ] Validation: Ardışık `__` olamaz (`1__000` ❌)
- [ ] Parser: `_` karakterlerini strip et, sayıyı parse et

**Syntax Örnekleri:**
```pmpl
-- Mevcut (Türk formatı):
numeric x = 126.555.852,36;     -- . binlik, , ondalık ✅

-- Yeni (Underscore separator):
numeric y = 126_555_852,36;     -- _ binlik ayırıcı
numeric z = 83_614_362;         -- Tam sayı
numeric pi = 3,141_592_653;     -- Ondalık

-- Kombinasyon (ikisi birden):
numeric salary = 100_443_000,55;  -- ✅ _ ve , birlikte
numeric big = 999.999.999,99;     -- ✅ . ve , birlikte (mevcut)
```

**Validation Kuralları:**
```pmpl
-- ✅ Geçerli:
1_000
1_000_000
3,14_15_92
100_443_000,55

-- ❌ Geçersiz:
_1000           -- Başta underscore
1000_           -- Sonda underscore  
1__000          -- Ardışık underscore
,_14            -- Ondalık ayırıcıdan hemen sonra
```

**Dosyalar:**
- `compiler/stage0/modules/lexer/lexer.c` - `lexer_read_number()` güncelle
- `tests/syntax/test_numeric_separator.mlp` - Test cases
- `pmlp_kesin_sozdizimi.md` - Syntax dokümanı güncelle

**Referans:** Python PEP 515, Rust numeric literals

---

#### YZ_229: Compound Assignment & Increment Operators [2 gün]
**Öncelik:** 🟡 Orta (syntax sugar, modern standard)

**Neden Gerekli:**
Tüm modern dillerde standart. Kod tekrarını azaltır, okunabilirliği artırır.

**Yapılacaklar:**
- [ ] Compound assignment: `+=`, `-=`, `*=`, `/=`, `%=`
- [ ] **Okunabilir increment:** `x += 1` (alternatif: `increment x`, `decrement x`)
- [ ] Bitwise compound: `&=`, `|=`, `^=`, `<<=`, `>>=`
- [ ] Power compound: `**=`
- [ ] Lexer: Yeni operator token'ları
- [ ] Parser: Assignment expression handling
- [ ] Codegen: Desugar to `x = x + value`

**Syntax (MELP-Friendly):**
```pmpl
-- Compound assignment (evrensel)
numeric x = 10;
x += 5;              -- x = x + 5  (15) ✅
x *= 2;              -- x = x * 2  (30) ✅
x /= 3;              -- x = x / 3  (10) ✅

-- Increment (iki alternatif)
x += 1;              -- ✅ Compound assignment (tercih edilen)
increment x;         -- ⚠️ Opsiyonel keyword (MELP-style)

-- C-style ++ ZORUNLU DEĞİL (daha az okunabilir)
-- x++;              -- ❌ MELP felsefesine ters (sembolik)
-- ++x;              -- ❌ MELP felsefesine ters

-- Bitwise compound
numeric flags = 0;
flags |= 0x01;       -- ✅ Kısa ve net
flags &= 0xFF;
```

**⚠️ MELP Prensibi:**
- `+=`, `-=`, `*=` → ✅ **Kabul** (evrensel, kısa, net)
- `++`, `--` → ❌ **Tercihen kullanma** (sembolik, belirsiz: pre vs post?)
- Alternatif: `x += 1` veya `increment x` (daha okunabilir)

**Dosyalar:**
- `compiler/stage0/modules/lexer/lexer.c` - Compound operators
- `compiler/stage0/modules/arithmetic/arithmetic_parser.c`
- `pmlp_kesin_sozdizimi.md` - Syntax (increment keyword opsiyonel)

**Referans:** Compound assignment evrensel, ++/-- opsiyonel

---

#### YZ_230: Spread & Destructuring [3 gün]
**Öncelik:** 🟢 Düşük (advanced syntax sugar)

**Neden Gerekli:**
Modern dillerde standart (JavaScript, Python, Rust). Collection manipulation'ı kolaylaştırır.

**Yapılacaklar:**
- [ ] **Spread (keyword-based):** `spread items` ✅ MELP-style
- [ ] **Spread (symbolic):** `...items` ⚠️ Alternative (less readable)
- [ ] Array spread: `[spread arr1; 4; 5;]`
- [ ] List spread: `(spread list1; 6; 7;)`
- [ ] Function call spread: `func(spread args)`
- [ ] **Destructuring (keyword-based):** `extract a, b, c from tuple;` ✅ MELP-style
- [ ] Destructuring (alternative): `(x; y; z) = tuple`
- [ ] Array destructuring: `[a; b; ...rest] = array`
- [ ] Struct destructuring: `{name; age} = person`

**Syntax (MELP-Friendly):**
```pmpl
-- Spread operator (keyword-based, okunabilir)
numeric[] arr1 = [1; 2; 3;];
numeric[] arr2 = [spread arr1; 4; 5;];  -- ✅ MELP-style keyword
-- numeric[] arr2 = [...arr1; 4; 5;];  -- ⚠️ Symbolic alternative

-- List spread (keyword)
list data1 = (1; 2; 3;);
list data2 = (spread data1; 4; 5;);     -- ✅ Keyword (preferred)

-- Function call spread
function sum(numeric a; numeric b; numeric c) returns numeric
    return a + b + c;
end_function

numeric[] values = [10; 20; 30;];
numeric total = sum(spread values);      -- ✅ Keyword
-- numeric total = sum(...values);       -- ⚠️ Symbolic

-- Destructuring (keyword-based, düzyazı gibi)
tuple<numeric; string; boolean> user = <25; "Alice"; true;>;
extract age, name, active from user;     -- ✅ MELP-style (recommended)
-- (numeric age; string name; boolean active) = user;  -- ⚠️ Alternative

print(age);    -- 25
print(name);   -- "Alice"
print(active); -- true

-- Destructuring with rest (keyword)
numeric[] numbers = [1; 2; 3; 4; 5;];
extract first, second, rest from numbers;  -- ✅ Keyword
-- [numeric first; numeric second; ...numeric[] rest] = numbers;  -- ⚠️ Symbolic

-- Struct destructuring (keyword)
struct Person
    string name;
    numeric age;
    string city;
end_struct

Person p;
p.name = "Bob";
p.age = 30;
p.city = "NYC";

extract name, age from p;  -- ✅ MELP-style (recommended)
-- {string name; numeric age} = p;  -- ⚠️ Alternative
```

**⚠️ MELP Prensibi:**
- `spread items` → ✅ **Tercih edilen** (keyword, okunabilir, düzyazı gibi)
- `...items` → ⚠️ **Opsiyonel** (symbolic, daha az okunabilir)
- `extract a, b, c from tuple` → ✅ **MELP-style** (düzyazı gibi)
- `(a; b; c) = tuple` → ⚠️ **Opsiyonel** (symbolic)

**Dosyalar:**
- `compiler/stage0/modules/lexer/lexer.c` - `spread`, `extract`, `from` keywords
- `compiler/stage0/modules/arithmetic/arithmetic_parser.c` - Spread parsing
- `compiler/stage0/modules/statement/statement_parser.c` - Destructuring
- Runtime: Array/list expansion utilities
- `pmlp_kesin_sozdizimi.md` - Syntax (keyword-based priority)

**Referans:** JavaScript spread (but keyword-adapted for MELP readability)

---

#### YZ_227: Inline Control Flow [3 gün]
**Öncelik:** 🟢 Düşük (syntax sugar)

**Yapılacaklar:**
- [ ] Tek satır if (end_if olmadan): `if condition then statement`
- [ ] Ternary operator: `x = condition ? true_value : false_value`
- [ ] Inline while: `while condition : statement : end_while`
- [ ] Elvis operator: `x = value ?? default_value`

**Test Cases:**
```pmpl
-- Tek satır if
if x > 10 then print(x)

-- Ternary
result = (x > 0) ? "positive" : "negative"

-- Elvis (null coalescing)
name = user.name ?? "Anonymous"
```

**Dosyalar:**
- `compiler/stage0/modules/parser/control_flow_parser.c`
- `tests/syntax/test_inline_control.mlp`

**⚠️ NOT:** Bu özellik opsiyonel, syntax sugar

---

## 📊 PRİORİTY MATRİSİ

| Phase | YZ | Görev | Öncelik | Süre | Neden Zorunlu? |
|-------|-----|-------|---------|------|----------------|
| 0 | YZ_06 | **Stage 0 List Syntax** | 🔴🔴🔴 **BLOKER** | 1 hafta | Stage 1 modüller derlenemiyor! |
| 1 | YZ_200 | List Type | 🔴 KRİTİK | 1 hafta | Stage 1 operations |
| 2 | YZ_204 | Module System | 🔴 KRİTİK | 1 hafta | Code organization |
| 1 | YZ_201 | Map Type | 🟡 Yüksek | 5 gün | Stdlib gerekli |
| 1 | YZ_202 | Optional Type | 🟡 Yüksek | 3 gün | Type safety |
| 3 | YZ_206 | Result Type | 🟡 Yüksek | 1 hafta | Error handling |
| 4 | YZ_208 | Lambda Functions | 🟡 Yüksek | 1 hafta | Functional programming |
| 6 | YZ_212 | Collections Library | 🟡 Yüksek | 1 hafta | Temel yapılar |
| 6 | YZ_213 | String Library | 🟡 Yüksek | 5 gün | Text processing |
| 6 | YZ_214 | File I/O | 🟡 Yüksek | 5 gün | IO operations |
| 7 | YZ_216 | C FFI | 🟡 Yüksek | 1 hafta | Interop |
| 9 | YZ_220 | Build System | 🟡 Yüksek | 1 hafta | DevEx |
| 9 | YZ_221 | Package Manager | 🟡 Yüksek | 1 hafta | Ecosystem |
| 1 | YZ_203 | Generic Types | 🟡 Orta | 2 hafta | Type system |
| 2 | YZ_205 | Package Structure | 🟡 Orta | 5 gün | Organization |
| 4 | YZ_209 | Function Pointers | 🟡 Orta | 3 gün | Callbacks |
| 5 | YZ_210 | Reference Counting | 🟡 Orta | 2 hafta | Memory safety |
| 6 | YZ_215 | Math Library | 🟢 Orta | 3 gün | Math operations |
| 7 | YZ_217 | Struct ABI | 🟡 Orta | 1 hafta | C interop |
| 9 | YZ_222 | Formatter/Linter | 🟢 Orta | 1 hafta | Code quality |
| 3 | YZ_207 | Try-Catch | 🟢 Düşük | 5 gün | Opsiyonel |
| 5 | YZ_211 | Move Semantics | 🟢 Düşük | 1 hafta | Optimization |
| 8 | YZ_218 | Threading | 🟢 Düşük | 2 hafta | Concurrency |
| 8 | YZ_219 | Async/Await | 🟢 Düşük | 2 hafta | Modern async |
| 10 | YZ_223 | LLVM Optimization | 🟢 Düşük | 1 hafta | Performance |
| 10 | YZ_224 | LTO | 🟢 Düşük | 1 hafta | Advanced opt |
| 11 | YZ_225 | Line Continuation | 🟡 Orta | 3 gün | Developer experience |
| 11 | YZ_226 | Comment Syntax | 🟡 Orta | 2 gün | Documentation |
| 11 | YZ_228 | Numeric Underscore | 🟡 Orta | 2 gün | Readability |
| 11 | YZ_229 | Compound Assignment | 🟡 Orta | 2 gün | Syntax sugar |
| 11 | YZ_227 | Inline Control Flow | 🟢 Düşük | 3 gün | Syntax sugar |
| 11 | YZ_230 | Spread/Destructuring | 🟢 Düşük | 3 gün | Advanced syntax |

---

## 🎯 EXECUTION PLAN (Öncelik Sırası)

### 🚨 FAZ 0: STAGE 1 BLOKER (1 hafta) - ACİL!

**YZ_06: Stage 0 List Syntax Support** [1 hafta] 🔴🔴🔴
- **Kök Neden:** Stage 0 parser `()` list syntax desteklemiyor
- **Etki:** 66 Stage 1 modül derlenemiyor (0 function compiled)
- **Çözüm:** Stage 0'a list literal parsing ekle
- **Alternatif:** 580+ satır Stage 1 modül düzelt (tavsiye edilmez)
└─ Hedef: Stage 1 modülleri derlenebilir yap

**Çıktı:** Stage 1 bootstrap başlayabilir! 🔥

---

### 🔥 FAZ 1: KRİTİK BLOKER'LAR (4 hafta)

**⚠️ YZ_06: Stage 0 List Syntax Support (BLOKER!)** [1 hafta] 🔴🔴🔴
- **En Yüksek Öncelik** - Stage 1 modüller derlenemiyor!
- **Sorun:** Stage 1 modüller `returns list` + `()` syntax kullanıyor
- **Problem:** Stage 0 parser `()` list literal syntax'ını desteklemiyor
- **Analiz:**
  - 541 satır `list ... = [...]` yanlış syntax (doğrusu `(...)`)
  - 40 satır `return [...]` yanlış syntax (doğrusu `return (...)`)
  - 25 dosya etkilenmiş
  - Stage 1 modüller `,` virgül kullanmış (doğrusu `;` semicolon)
- **Gerekli Değişiklikler:**
  - Stage 0 Lexer: `(` sonrası list literal detection
  - Stage 0 Parser: List literal parsing (`(elem1; elem2; elem3;)`)
  - Stage 0 Parser: Parametre ayırıcı `;` desteği (şu an `,` kullanılıyor)
  - Stage 0 Codegen: List allocation ve initialization
  - VEYA Stage 1 modüllerini toplu düzelt (580+ satır değişiklik)
- **Karar:** Stage 0'a eklemek daha temiz (bir kez yap, hep kullan)
- **Dosyalar:**
  - `compiler/stage0/modules/lexer/lexer.c` - List literal tokenization
  - `compiler/stage0/modules/functions/functions_parser.c` - List parsing, semicolon params
  - `compiler/stage0/modules/array/array_codegen.c` - List codegen (varsa extend)
  - VEYA yeni `compiler/stage0/modules/list/` modülü oluştur
- **Test:**
  ```bash
  ./functions_compiler modules/lexer_mlp/token.mlp temp/test.ll --backend=llvm
  # Beklenen: 6 functions compiled (şu an 0 function)
  ```
- **Referans:** 
  - `pmlp_kesin_sozdizimi.md` line 640-642 (List syntax)
  - `kurallar_kitabı.md` line 857-898 (Koleksiyon tipleri)
  - Stage 1 modül analizi: `temp/MODÜL_ANALIZ_KARŞILAŞTIRMA.md`

**Çıktı:** Stage 1 modüller derlenebilir hale gelir! 🎉

---

1. **YZ_200: List Type Full Implementation** [1 hafta] 🔴
   - YZ_06'dan sonra başlanır
   - List operations: append, prepend, length, find
   - Dynamic resizing
   - Memory management (STO integration)
   - Tüm Stage 1 modüller bu özelliğe bağımlı

2. **YZ_204: Module System** [1 hafta] 🔴
   - Code organization için zorunlu
   - Import/export syntax
   - Bootstrap sonrası modül yapısı

3. **YZ_201: Map Type** [5 gün] 🟡
   - Stdlib için gerekli
   - Hash table implementation

4. **YZ_202: Optional Type** [3 gün] 🟡
   - Null safety
   - Modern type system

**Çıktı:** Stage 1 bloker tamamen çözülür, bootstrap tamamlanır! 🎉

---

### ⚡ İKİNCİ DALGA (4 hafta) - TEMEL ÖZELLİKLER

5. **YZ_206: Result Type** [1 hafta] 🟡
   - Error handling pattern
   
6. **YZ_208: Lambda Functions** [1 hafta] 🟡
   - First-class functions

7. **YZ_212: Collections Library** [1 hafta] 🟡
   - List, Map, Set operations

8. **YZ_213: String Library** [5 gün] 🟡
   - String manipulation

**Çıktı:** Modern language features complete!

---

### 🚀 ÜÇÜNCÜ DALGA (4 hafta) - ECOSYSTEM

9. **YZ_214: File I/O** [5 gün] 🟡
10. **YZ_216: C FFI** [1 hafta] 🟡
11. **YZ_220: Build System** [1 hafta] 🟡
12. **YZ_221: Package Manager** [1 hafta] 🟡

**Çıktı:** Production-ready ecosystem!

---

### 🎨 DÖRDÜNCÜ DALGA (6 hafta) - GELİŞMİŞ ÖZELLİKLER

13. **YZ_203: Generic Types** [2 hafta] 🟡
14. **YZ_210: Reference Counting** [2 hafta] 🟡
15. **YZ_209, YZ_215, YZ_217, YZ_222** [2 hafta] 🟢

**Çıktı:** Feature-complete modern language!

---

### 🌟 BEŞİNCİ DALGA (Opsiyonel) - ADVANCED

16. **YZ_218/219: Concurrency** [4 hafta] 🟢
17. **YZ_223/224: Optimization** [2 hafta] 🟢

---

### 🎨 ALTINCI DALGA (Ergonomi) - SYNTAX IMPROVEMENTS

18. **YZ_225: Line Continuation** [3 gün] 🟡
    - VB.NET tarzı `_` ile satır bölme
    - Uzun string literalleri
    - Developer experience iyileştirmesi

19. **YZ_226: Comment Syntax** [2 gün] 🟡
    - Dokümantasyon yorumları: `---/` ... `/---`
    - Çok satırlı: `---` ... `---`
    - IDE tooling desteği

20. **YZ_227: Inline Control Flow** [3 gün] 🟢
    - Tek satır if (opsiyonel)
    - Ternary operator
    - Syntax sugar

**Çıktı:** Daha ergonomik, okunabilir syntax!

---

## 📈 İLERLEME TAHMİNİ

| Dalga | Süre | Bitiş Tarihi | İlerleme |
|-------|------|--------------|----------|
| Faz 0 (BLOKER!) | 1 hafta | 28 Aralık 2025 | ✅ %5 (YZ_07 tamamlandı!) |
| Faz 1 (Kritik) | 4 hafta | 25 Ocak 2026 | → %40 |
| Faz 2 (Temel) | 4 hafta | 22 Şubat 2026 | → %65 |
| Faz 3 (Ecosystem) | 4 hafta | 22 Mart 2026 | → %85 |
| Faz 4 (Gelişmiş) | 6 hafta | 3 Mayıs 2026 | → %100 |
| Faz 5 (Advanced) | 6 hafta | 14 Haziran 2026 | → %120 |
| Faz 6 (Syntax) | 1 hafta | 21 Haziran 2026 | → %125 |

**✅ GÜNCELLEME:** Faz 0 (YZ_07) tamamlandı! Stage 1 bloker çözüldü!

**Minimum Viable Language:** Faz 0 + Faz 1 + Faz 2 (9 hafta) → %65  
**Production Ready:** Faz 0-3 (13 hafta) → %85  
**Feature Complete:** Faz 0-4 (19 hafta) → %100  
**Industry Grade:** Faz 0-5 (25 hafta) → %120

---

## 🎯 BAŞARI KRİTERLERİ

### Minimum Viable Language (%65)
- ✅ Stage 0 + Stage 1 bootstrap çalışıyor
- ✅ List, Map, Optional types
- ✅ Module system
- ✅ Lambda functions
- ✅ Collections library
- ✅ String library
- ✅ Error handling (result type)

### Production Ready (%85)
- ✅ MVL özellikleri
- ✅ File I/O
- ✅ C FFI
- ✅ Build system
- ✅ Package manager
- ✅ Comprehensive stdlib

### Feature Complete (%100)
- ✅ Production ready
- ✅ Generic types
- ✅ Reference counting GC
- ✅ Function pointers
- ✅ Math library
- ✅ Struct ABI
- ✅ Formatter & linter

### Industry Grade (%120)
- ✅ Feature complete
- ✅ Concurrency (threads or async)
- ✅ LLVM optimization pipeline
- ✅ Link-time optimization
- ✅ Performance benchmarks

---

## 📊 NİŞ VS ZORUNLU ÖZELLİKLER

### ✅ ZORUNLU (Her Modern Dilde Olmalı)
- List, Map, Set (collection types) ✅
- Optional type (null safety) ✅
- Module system (import/export) ✅
- Lambda functions (closures) ✅
- Error handling (result/exception) ✅
- String operations (split, join, format) ✅
- File I/O (read, write) ✅
- C FFI (interop) ✅
- Package manager ✅
- Generic types ✅

### ❌ NİŞ (Opsiyonel, Eklenebilir Ama Şart Değil)
- Async/await ❌ (advanced, threading yeterli)
- Move semantics ❌ (optimization, GC varsa gereksiz)
- LTO ❌ (optimization, nice-to-have)
- Macros ❌ (meta-programming, karmaşık)
- Reflection ❌ (runtime introspection, karmaşık)
- Operator overloading ❌ (syntax sugar, abuse edilebilir)
- Multiple inheritance ❌ (composition tercih edilir)
- Template specialization ❌ (generics yeterli)

---

## 🔄 MEVCUT LLVM TODO İLE KARŞILAŞTIRMA

### LLVM_YZ TODO (Optional Features)
- YZ_05: Array LLVM backend (⏳ devam ediyor)
- YZ_06-13: Diğer LLVM features

### Bu TODO (Essential Features)
- YZ_200-224: Zorunlu modern dil özellikleri

**Karar:** LLVM TODO'yu durdur, bu TODO'ya odaklan!

**Neden:**
- LLVM backend zaten production ready ✅
- Array LLVM backend nice-to-have, ama Stage 1 bloker değil
- **List type eksikliği Stage 1'i engelliyor!** 🔴
- Module system olmadan büyük projeler yazılamaz
- Stdlib olmadan pratik değil

**Yeni Strateji:**
1. ✅ YZ_06/07 (List Syntax) → BLOKER ÇÖZÜLDÜ! (21 Aralık 2025)
2. YZ_200 (List Operations) ile Stage 1 operations → 1 hafta
3. YZ_204 (Module) ile code organization sağla → 1 hafta
4. Diğer zorunlu özellikleri ekle → 10 hafta
5. YZ_225-227 (Syntax Improvements) → 1 hafta
6. LLVM optional features'a dön → İhtiyaç olursa

---

## 🎯 SONUÇ

**Hedef:** %125 Eksiksiz Modern Dil (+ Syntax Ergonomi)
**Yol Haritası:** 27 Phase (YZ_200-227), 20 hafta (Dalga 1-6)
**✅ Tamamlanan:** YZ_06/07 (List Syntax) → Stage 1 bloker çözüldü!
**Sonraki:** YZ_200 (List Type Operations) 🚀
**MVP:** 8 hafta (Dalga 1-2) → %65 feature complete  
**Production:** 12 hafta (Dalga 1-3) → %85 ready  
**Full:** 18 hafta (Dalga 1-4) → %100 modern language
**Polished:** 20 hafta (Dalga 1-6) → %125 ergonomic syntax

---

**Son Güncelleme:** 21 Aralık 2025, 22:30  
**Hazırlayan:** YZ_ÜA_07 (Coordinator) + YZ_07 (Implementation)  
**Durum:** ✅ Faz 0 Tamamlandı, YZ_200 Hazır!

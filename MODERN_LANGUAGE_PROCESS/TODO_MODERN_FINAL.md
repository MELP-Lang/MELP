# MELP MODERN LANGUAGE - FINAL TODO

**Tarih:** 23 Aralık 2025  
**Önceki TODO:** Self-hosting ✅ (Tamamlandı)  
**Hedef:** MELP'i production-ready modern dil haline getir  
**Prensip:** Net, spesifik, ölçülebilir görevler  
**Kural:** Bu TODO tamamlandığında MELP modern dil özelliklerine sahip olacak.

---

## 🎯 BAŞLANGIÇ DURUMU

**Self-hosting Tamamlandı! (23 Aralık 2025)**

✅ Minimal compiler (180 satır)
✅ Gen1 oluşturuldu (273 satır LLVM IR)
✅ Symbolic bootstrap başarılı
✅ Convergence kanıtlandı (Gen2 = Gen3)

**Mevcut Özellikler:**
- ✅ Function definitions
- ✅ Numeric type (basic)
- ✅ Return statements
- ✅ Basic arithmetic (+, -, *, /)
- ✅ Comparison operators (>, <, ==)
- ✅ If-then-end_if statements
- ✅ LLVM IR backend
- ✅ Self-compilation

**Eksik Özellikler:**
- ❌ Variables (let, assignments)
- ❌ While loops
- ❌ For loops
- ❌ String type
- ❌ Arrays/Lists
- ❌ Structs
- ❌ Module system
- ❌ Error handling
- ❌ FFI (C interop)
- ❌ Standard library

---

## 📋 PHASE 0: SELF-HOSTING İYİLEŞTİRMELER (1 hafta)

### Amaç
Minimal compiler'dan production-ready compiler'a geçiş.

### TASK 0.1a: File I/O Infrastructure (1 gün) ✅ TAMAMLANDI

**Görev:**
Runtime'a file I/O desteği ekle (bootstrap Phase 1'den sonra).

**Spesifik Adımlar:**
```bash
1. Runtime'a file I/O fonksiyonları ekle:
   - mlp_read_file(filename) → string
   - mlp_write_file(filename, content) → numeric
   
2. LLVM backend'e file I/O desteği:
   - Builtin function detection
   - String parameter handling
   
3. Test:
   test_file_io.mlp → dosyaya yaz, dosyadan oku
```

**Başarı kriteri:**
```
[✅] mlp_read_file() çalışıyor
[✅] mlp_write_file() çalışıyor
[✅] Test dosyası oluşturuldu
[✅] Runtime stdlib rebuild edildi
[✅] TODO dışı iş yapılmadı
```

**YASAK:**
❌ Yeni operatör ekleme (sadece file I/O)
❌ Syntax değişikliği
❌ TODO dışı özellik ekleme

**Durum:** ✅ Tamamlandı (modern_YZ_01)
**Not:** Real bootstrap için string concat gerekli (Task 1.1'den sonra Task 0.1b yapılacak)

---

### TASK 0.1b: Real File I/O Bootstrap (ERTELENDI → Task 1.1'den sonra)

**Görev:**
Symbolic bootstrap'ı real file I/O'ya dönüştür.

**Ön Koşul:** ⚠️ Task 1.1 (String type) tamamlanmalı!

**Spesifik Adımlar:**
```bash
1. compiler.mlp'ye file I/O ekle:
   - read_file(input_filename) kullan
   - write_file(output_filename, ir) kullan
   - String concat ile path oluştur
   
2. Real bootstrap test:
   Stage 0 → compiler.mlp → Gen1.ll (dosyaya yaz)
   lli Gen1.ll compiler.mlp Gen2.ll (dosyaya yaz)
   diff Gen1.ll Gen2.ll (boş olmalı!)

3. Başarı kriteri:
   [ ] Gen1.ll dosya olarak oluşturuldu
   [ ] Gen2.ll dosya olarak oluşturuldu
   [ ] Gen1.ll = Gen2.ll (byte-level identical)
```

**YASAK:**
❌ String olmadan zorla yapmaya çalışma
❌ Task 1.1'i bekle!

**Tahmini süre:** 1 gün (Task 1.1'den sonra)

---

### TASK 0.2: Variables Support (2 gün) ← SONRAKİ TASK!

**Görev:**
Variable declaration ve assignment ekle.

**Spesifik Adımlar:**
```melp
-- Hedef syntax:
function test() returns numeric
    numeric x = 10      -- declaration + initialization
    x = x + 5           -- assignment
    return x            -- 15 döner
end_function
```

**Implementation:**
1. Lexer: '=' tokenize et
2. Parser: var_decl ve assignment parse et
3. CodeGen: alloca, store, load instructions
4. Test: Basit variable programı derle ve çalıştır

**Başarı kriteri:**
```
[ ] numeric x = 10 çalışıyor
[ ] x = x + 5 çalışıyor
[ ] Test exit code 15
[ ] TODO dışı iş yapılmadı
```

**YASAK:**
❌ String variables (sadece numeric)
❌ Arrays (sonra gelecek)
❌ Multiple types (sadece numeric)

**Tahmini süre:** 2 gün

---

### TASK 0.3: While Loops (1.5 gün)

**Görev:**
While döngüsü ekle.

**Spesifik Adımlar:**
```melp
-- Hedef syntax:
function count() returns numeric
    numeric i = 0
    while i < 10 do
        i = i + 1
    end_while
    return i  -- 10 döner
end_function
```

**Implementation:**
1. Parser: while-do-end_while parse et
2. CodeGen: br, phi, basic blocks
3. Test: 1'den 10'a kadar toplama

**Başarı kriteri:**
```
[ ] while-do-end_while çalışıyor
[ ] Loop condition değerlendiriliyor
[ ] Loop body tekrarlanıyor
[ ] Test exit code 55 (1+2+...+10)
```

**YASAK:**
❌ For loop (sonra gelecek)
❌ Break/continue (sonra gelecek)

**Tahmini süre:** 1.5 gün

---

### TASK 0.4: Finalization (0.5 gün)

**Görev:**
Phase 0'ı belgele, tamamla.

**Spesifik Adımlar:**
1. README.md güncelle (Phase 0 features)
2. Test suite oluştur (tüm features)
3. Performance benchmark (compile time)
4. Git tag: v0.1.0-phase0

**Başarı kriteri:**
```
[ ] README güncel
[ ] 10+ test geçiyor
[ ] Benchmark sonuçları
[ ] Git tag oluşturuldu
```

**Tahmini süre:** 0.5 gün

---

## 📋 PHASE 1: CORE LANGUAGE FEATURES (2 hafta)

### TASK 1.1: String Type (3 gün)

**Görev:**
String değişkenleri ve operations ekle.

**Spesifik Adımlar:**
```melp
function greet() returns string
    string name = "MELP"
    string msg = "Hello " + name  -- concatenation
    return msg
end_function
```

**Implementation:**
1. Lexer: String literals ("...")
2. Parser: String type declarations
3. CodeGen: i8* type, mlp_string_concat()
4. Runtime: String concat fonksiyonu (C)

**Başarı kriteri:**
```
[ ] string x = "hello" çalışıyor
[ ] String concatenation çalışıyor
[ ] strlen() fonksiyonu çalışıyor
[ ] Test programı geçiyor
```

**YASAK:**
❌ String interpolation (sonra)
❌ Regex (sonra)
❌ Unicode handling (sonra)

**Tahmini süre:** 3 gün

---

### TASK 1.2: For Loops (2 gün)

**Görev:**
For döngüsü ekle.

**Spesifik Adımlar:**
```melp
function sum_range() returns numeric
    numeric total = 0
    for i = 1 to 10 do
        total = total + i
    end_for
    return total  -- 55
end_function
```

**Başarı kriteri:**
```
[ ] for-to-do-end_for çalışıyor
[ ] Range iteration çalışıyor
[ ] Test exit code 55
```

**YASAK:**
❌ for-in (array iteration, sonra)
❌ Step size (sonra)

**Tahmini süre:** 2 gün

---

### TASK 1.3: Arrays (4 gün)

**Görev:**
Array type ve indexing ekle.

**Spesifik Adımlar:**
```melp
function test_array() returns numeric
    array[5] numbers = [1, 2, 3, 4, 5]
    numeric sum = 0
    for i = 0 to 4 do
        sum = sum + numbers[i]
    end_for
    return sum  -- 15
end_function
```

**Implementation:**
1. Parser: array[size] type
2. CodeGen: alloca array, getelementptr
3. Runtime: Bounds checking (optional)

**Başarı kriteri:**
```
[ ] array[N] declaration çalışıyor
[ ] arr[i] indexing çalışıyor
[ ] Array initialization çalışıyor
[ ] Test programı geçiyor
```

**YASAK:**
❌ Dynamic arrays (sonra)
❌ Multi-dimensional (sonra)
❌ Array methods (sonra)

**Tahmini süre:** 4 gün

---

### TASK 1.4: Structs (3 gün)

**Görev:**
Struct definitions ve field access.

**Spesifik Adımlar:**
```melp
struct Point
    numeric x
    numeric y
end_struct

function test_struct() returns numeric
    Point p
    p.x = 10
    p.y = 20
    return p.x + p.y  -- 30
end_function
```

**Başarı kriteri:**
```
[ ] struct definition çalışıyor
[ ] struct.field access çalışıyor
[ ] struct initialization çalışıyor
[ ] Test exit code 30
```

**YASAK:**
❌ Methods (sonra)
❌ Inheritance (MELP'te yok!)
❌ Nested structs (sonra)

**Tahmini süre:** 3 gün

---

## 📋 PHASE 2: FFI & INTEROP (1 hafta)

### TASK 2.1: C FFI Basics (3 gün)

**Görev:**
C fonksiyonlarını çağırabilme.

**Spesifik Adımlar:**
```melp
extern "C" function printf(string format) returns numeric

function main() returns numeric
    printf("Hello from MELP!\n")
    return 0
end_function
```

**Implementation:**
1. Parser: extern "C" declarations
2. CodeGen: declare external functions
3. Type mapping: numeric → i64, string → i8*
4. Test: printf çağrısı

**Başarı kriteri:**
```
[ ] extern "C" syntax çalışıyor
[ ] C function call çalışıyor
[ ] Type mapping doğru
[ ] printf testi geçiyor
```

**YASAK:**
❌ Struct ABI (sonra)
❌ Callbacks (sonra)
❌ Variadic functions (sonra)

**Tahmini süre:** 3 gün

---

### TASK 2.2: Standard C Library (2 gün)

**Görev:**
Sık kullanılan C stdlib fonksiyonlarını wrap et.

**Spesifik Adımlar:**
```melp
-- stdlib.mlp
extern "C" function malloc(numeric size) returns pointer
extern "C" function free(pointer ptr) returns numeric
extern "C" function strlen(string s) returns numeric
```

**Başarı kriteri:**
```
[ ] malloc/free çalışıyor
[ ] strlen çalışıyor
[ ] 10+ stdlib function wrapped
[ ] Test suite geçiyor
```

**Tahmini süre:** 2 gün

---

### TASK 2.3: File I/O Library (2 gün)

**Görev:**
Dosya okuma/yazma wrapper'ları.

**Spesifik Adımlar:**
```melp
function read_text_file(string path) returns string
    extern "C" function fopen(string, string) returns pointer
    extern "C" function fread(pointer, numeric, numeric, pointer) returns numeric
    -- implementation
end_function
```

**Başarı kriteri:**
```
[ ] read_file() çalışıyor
[ ] write_file() çalışıyor
[ ] append_file() çalışıyor
[ ] Error handling var
```

**Tahmini süre:** 2 gün

---

## 📋 PHASE 3: MODULE SYSTEM (1 hafta)

### TASK 3.1: Import/Export (4 gün)

**Görev:**
Modül sistemi ekle.

**Spesifik Adımlar:**
```melp
-- math.mlp
export function add(numeric a; numeric b) returns numeric
    return a + b
end_function

-- main.mlp
import math

function main() returns numeric
    return math.add(10, 32)  -- 42
end_function
```

**Implementation:**
1. Parser: import/export keywords
2. Module resolution (file paths)
3. Symbol table (cross-module)
4. Linking

**Başarı kriteri:**
```
[ ] import çalışıyor
[ ] export çalışıyor
[ ] Cross-module calls çalışıyor
[ ] Test: 2 modül programı geçiyor
```

**YASAK:**
❌ Package manager (sonra)
❌ Circular dependencies (handle later)

**Tahmini süre:** 4 gün

---

### TASK 3.2: Standard Library Structure (3 gün)

**Görev:**
stdlib modülleri organize et.

**Spesifik Adımlar:**
```
stdlib/
├── io.mlp        (file I/O)
├── string.mlp    (string operations)
├── math.mlp      (math functions)
├── array.mlp     (array operations)
└── sys.mlp       (system calls)
```

**Başarı kriteri:**
```
[ ] 5+ stdlib modül
[ ] import stdlib.io çalışıyor
[ ] Documentation her modül için
[ ] Test coverage %80+
```

**Tahmini süre:** 3 gün

---

## 📋 PHASE 4: ERROR HANDLING (4 gün)

### TASK 4.1: Result Type (2 gün)

**Görev:**
Result<T, E> type pattern.

**Spesifik Adımlar:**
```melp
function divide(numeric a; numeric b) returns Result<numeric, string>
    if b == 0 then
        return Err("Division by zero")
    end_if
    return Ok(a / b)
end_function

function main() returns numeric
    Result<numeric, string> r = divide(10, 0)
    if r.is_err() then
        println(r.error())
        return 1
    end_if
    return 0
end_function
```

**Başarı kriteri:**
```
[ ] Result<T,E> type çalışıyor
[ ] Ok(value) çalışıyor
[ ] Err(error) çalışıyor
[ ] is_ok(), is_err() çalışıyor
```

**Tahmini süre:** 2 gün

---

### TASK 4.2: Error Propagation (2 gün)

**Görev:**
? operator for error propagation.

**Spesifik Adımlar:**
```melp
function process() returns Result<numeric, string>
    numeric x = divide(10, 2)?  -- Propagate error if any
    numeric y = divide(x, 0)?   -- Will propagate Err
    return Ok(y)
end_function
```

**Başarı kriteri:**
```
[ ] ? operator çalışıyor
[ ] Error propagation doğru
[ ] Test coverage
```

**Tahmini süre:** 2 gün

---

## 📋 PHASE 5: ADVANCED FEATURES **(Daha sonra yapılacak)**

### TASK 5.1: Generics **(Daha sonra yapılacak - v2.0)**

**Görev:**
Generic functions ve types.

**Spesifik Adımlar:**
```melp
function max<T>(T a; T b) returns T
    if a > b then
        return a
    end_if
    return b
end_function

function main() returns numeric
    numeric x = max<numeric>(10, 20)  -- 20
    string s = max<string>("a", "z")  -- "z"
    return 0
end_function
```

**Başarı kriteri:**
```
[ ] Generic functions çalışıyor
[ ] Type parameters çalışıyor
[ ] Monomorphization yapılıyor
[ ] Test suite geçiyor
```

**YASAK:**
❌ Generic constraints (sonra)
❌ Higher-kinded types (sonra)

**Tahmini süre:** 5 gün

---

### TASK 5.2: Closures **(Daha sonra yapılacak - v2.0)**

**Görev:**
First-class functions ve closures.

**Spesifik Adımlar:**
```melp
function make_adder(numeric x) returns function
    return function(numeric y) returns numeric
        return x + y  -- Captures x
    end_function
end_function

function main() returns numeric
    function add5 = make_adder(5)
    return add5(10)  -- 15
end_function
```

**Başarı kriteri:**
```
[ ] Function types çalışıyor
[ ] Closures capture environment
[ ] Nested functions çalışıyor
[ ] Test suite geçiyor
```

**Tahmini süre:** 4 gün

---

### TASK 5.3: Pattern Matching **(Daha sonra yapılacak - v2.0)**

**Görev:**
Match expressions.

**Spesifik Adımlar:**
```melp
function classify(numeric x) returns string
    match x
        case 0 => return "zero"
        case 1..10 => return "small"
        case _ => return "large"
    end_match
end_function
```

**Başarı kriteri:**
```
[ ] match-case syntax çalışıyor
[ ] Pattern guards çalışıyor
[ ] Exhaustiveness check
[ ] Test suite geçiyor
```

**Tahmini süre:** 5 gün

---

## 📋 PHASE 6: TOOLING (1 hafta)

### TASK 6.1: Build System (3 gün)

**Görev:**
`melp build` komutu.

**Spesifik Adımlar:**
```bash
melp build src/main.mlp -o bin/program
melp build --release  # Optimize
melp build --test     # Run tests
```

**Başarı kriteri:**
```
[ ] melp binary çalışıyor
[ ] Dependency resolution
[ ] Incremental compilation
[ ] Release builds
```

**Tahmini süre:** 3 gün

---

### TASK 6.2: Package Manager **(Daha sonra yapılacak - v2.0)**

**Görev:**
Temel package management.

**Spesifik Adımlar:**
```bash
melp init my-project
melp add http-lib
melp remove http-lib
melp update
```

**Başarı kriteri:**
```
[ ] melp.toml configuration
[ ] Dependency management
[ ] Version resolution
[ ] Test: Add/remove packages
```

**Tahmini süre:** 2 gün

---

### TASK 6.3: Language Server **(Daha sonra yapılacak - v2.0 veya community)**

**Görev:**
Basic LSP support.

**Features:**
- Syntax highlighting
- Auto-completion
- Go to definition
- Error diagnostics

**Başarı kriteri:**
```
[ ] LSP server çalışıyor
[ ] VS Code extension var
[ ] Basic features çalışıyor
```

**Tahmini süre:** 2 gün

---

## 📋 PHASE 7: FINALIZATION (3 gün)

### TASK 7.1: Documentation (2 gün)

**Spesifik Adımlar:**
1. Language reference (tüm syntax)
2. Standard library docs
3. Tutorial series (10+ examples)
4. API reference
5. Contributing guide

**Başarı kriteri:**
```
[ ] docs/ folder complete
[ ] Website deployed
[ ] 20+ code examples
```

---

### TASK 7.2: Release Preparation (1 gün)

**Spesifik Adımlar:**
1. Version: v1.0.0
2. Changelog
3. GitHub release
4. Announcement

**Başarı kriteri:**
```
[ ] Git tag v1.0.0
[ ] GitHub release published
[ ] CHANGELOG.md complete
```

---

## 🎯 TOPLAM TAHMİN

| Phase | Süre | Görev Sayısı |
|-------|------|--------------|
| Phase 0: Self-hosting iyileştirme | 1 hafta | 4 |
| Phase 1: Core features | 2 hafta | 4 |
| Phase 2: FFI | 1 hafta | 3 |
| Phase 3: Module system | 1 hafta | 2 |
| Phase 4: Error handling | 4 gün | 2 |
| Phase 5: Advanced | 2 hafta | 3 |
| Phase 6: Tooling | 1 hafta | 3 |
| Phase 7: Finalization | 3 gün | 2 |

**TOPLAM:** ~8-10 hafta (2-2.5 ay)

---

## ✅ BAŞARI KRİTERLERİ (Proje Sonu)

```
[ ] MELP production-ready
[ ] 100+ test geçiyor
[ ] Documentation complete
[ ] Standard library zengin
[ ] Module system çalışıyor
[ ] FFI kullanılabilir
[ ] Error handling robust
[ ] Build tools çalışıyor
[ ] LSP support var
[ ] v1.0.0 released
```

---

## 🚫 GENEL YASAKLAR (Tüm Phases)

Her YZ için geçerli:

❌ TODO'da olmayan Phase ekleme
❌ TODO'daki task'tan sapma
❌ "Detaylandırmamı ister misin?" deme (pasif kalma)
❌ Kullanıcı onayı olmadan ekstra feature
❌ Ambiguous implementation (net ol!)
❌ Validation skip etme

⚠️ İhlal = Görev durdurulur!

---

## 📝 NOTLAR

**MELP'in 5 Prensibi (Her zaman geçerli):**
1. MODÜLER (500 satır limit)
2. LLVM (Backend)
3. STO (Smart Type Optimization)
4. STATELESS (Pure functions)
5. STRUCT+FUNC (No OOP)

**Her Task İçin:**
- Spesifik başarı kriterleri var
- YASAK listesi var
- Test suite gerekli
- Documentation gerekli

**YZ Workflow:**
1. TODO oku (sadece kendi Phase'ini)
2. Spesifik adımları takip et
3. YASAK listesine uy
4. Test et
5. Rapor yaz
6. Push et

---

**Son Güncelleme:** 23 Aralık 2025
**Durum:** 🔵 Başlamaya hazır!
**Sonraki YZ:** modern_YZ_01 (Phase 0, Task 0.1)

# 6-TODO_OPTIMIZATION.md

**Hedef:** Production Performance Optimization  
**Süre:** 2 hafta (Hafta 22-23)  
**Öncelik:** Orta  
**Bağımlılık:** 0-5 TODO'lar (%100)

---

## 📋 GENEL BAKIŞ

MLP-GCC çalışıyor ama **performans optimize edilmemiş:**
- ❌ Dead code elimination yok
- ❌ Constant folding yok
- ❌ Inline functions yok
- ❌ Loop optimizations yok
- ❌ Profiler yok

Bu TODO **C/Rust seviyesinde performans** sağlayacak.

---

## 🎯 TODO HEDEFLERİ

### Başarı Kriterleri

- ✅ Compiler optimizations (dead code, constant folding)
- ✅ Inline functions (small functions)
- ✅ Loop optimizations (loop unrolling, vectorization)
- ✅ Benchmark suite (performance tracking)
- ✅ Profiler integration (flamegraphs)
- ✅ Zero-cost abstractions

**Performance Target:** C ile %80-90 aynı hız

---

## 📊 TASK BREAKDOWN

### **Task 1: Dead Code Elimination** (3 gün)

**Hedef:** Kullanılmayan kodu sil

**Örnek:**
```mlp
function unused()  # ← Hiç çağrılmıyor
    yazdir("Bu kod çalışmaz")
end_function

function main()
    yazdir("Hello")
    # unused() çağrılmadı
end_function
```

**Optimizasyon:**
```c
// Compiled C (before DCE)
void unused() { printf("Bu kod çalışmaz\n"); }
void main() { printf("Hello\n"); }

// Compiled C (after DCE)
void main() { printf("Hello\n"); }
// unused() silindi!
```

**Implementation:**
```
MELP/C/stage0/modules/optimizer/
└── dce.c  (~350 satır)  # Dead code elimination
```

**Test:**
```bash
# Binary size comparison
./mlp-gcc test.mlp -o test_nodce
./mlp-gcc test.mlp -O2 -o test_dce
ls -lh test_*
# Expected: test_dce %20-30 küçük
```

---

### **Task 2: Constant Folding** (3 gün)

**Hedef:** Compile-time hesaplama

**Örnek:**
```mlp
function main()
    numeric x = 10 + 20        # Compile-time hesapla
    numeric y = x * 2          # Compile-time hesapla (y = 60)
    yazdir(string(y))
end_function
```

**Optimizasyon:**
```c
// Before
int x = 10 + 20;  // Runtime addition
int y = x * 2;    // Runtime multiplication

// After
int y = 60;  // Compile-time computed!
```

**Implementation:**
```
MELP/C/stage0/modules/optimizer/
└── constant_fold.c  (~400 satır)
```

**Test:**
```bash
# Compile and disassemble
./mlp-gcc -O2 test.mlp -o test
objdump -d test | grep "mov.*60"
# Expected: Direct mov $60, not computation
```

---

### **Task 3: Function Inlining** (4 gün)

**Hedef:** Küçük fonksiyonları inline et

**Örnek:**
```mlp
function add(numeric a; numeric b) as numeric  # Small function
    return a + b
end_function

function main()
    numeric result = add(10, 20)  # ← Inline edilecek
    yazdir(string(result))
end_function
```

**Optimizasyon:**
```c
// Before
int add(int a, int b) { return a + b; }
int main() {
    int result = add(10, 20);  // Function call overhead
}

// After (inlined)
int main() {
    int result = 10 + 20;  // No function call!
}
```

**Heuristic:**
```
Inline if:
- Function body < 50 satır
- Çağrı sayısı < 5
- No recursion
- Hot path (profiler data)
```

**Implementation:**
```
MELP/C/stage0/modules/optimizer/
└── inline.c  (~500 satır)
```

**Test:**
```bash
# Check inlining
./mlp-gcc -O2 test.mlp -S -o test.s
grep "call.*add" test.s
# Expected: No "call add" (inlined)
```

---

### **Task 4: Loop Optimizations** (5 gün)

**Hedef:** Loop unrolling, vectorization

**Loop Unrolling:**
```mlp
-- Before
for i = 0 to 99 do
    array[i] = i * 2
end_for

-- After (unrolled 4x)
for i = 0 to 96 step 4 do
    array[i] = i * 2
    array[i+1] = (i+1) * 2
    array[i+2] = (i+2) * 2
    array[i+3] = (i+3) * 2
end_for
# Tail loop for remaining iterations
```

**SIMD Vectorization:**
```c
// Before (scalar)
for (int i = 0; i < 100; i++) {
    arr[i] = arr[i] * 2;
}

// After (SIMD - AVX2)
for (int i = 0; i < 96; i += 4) {
    __m256i vec = _mm256_loadu_si256((__m256i*)&arr[i]);
    __m256i doubled = _mm256_slli_epi64(vec, 1);  // Multiply by 2
    _mm256_storeu_si256((__m256i*)&arr[i], doubled);
}
// Tail loop
```

**Implementation:**
```
MELP/C/stage0/modules/optimizer/
├── loop_unroll.c     (~400 satır)
└── vectorize.c       (~600 satır)  # SIMD intrinsics
```

**Test:**
```bash
# Benchmark loop
./mlp-gcc -O0 loop.mlp -o loop_o0
./mlp-gcc -O2 loop.mlp -o loop_o2
time ./loop_o0  # 5s
time ./loop_o2  # 0.8s (6x faster!)
```

---

### **Task 5: Benchmark Suite** (3 gün)

**Hedef:** Performance tracking

**Benchmarks:**
```
benchmarks/
├── fibonacci.mlp      # Recursion
├── matrix_mul.mlp     # Numeric computation
├── string_concat.mlp  # String operations
├── json_parse.mlp     # I/O parsing
├── http_server.mlp    # Network
└── sorting.mlp        # Algorithms
```

**Benchmark Runner:**
```bash
#!/bin/bash
# benchmark.sh

echo "Running benchmarks..."

for bench in benchmarks/*.mlp; do
    name=$(basename $bench .mlp)
    echo "=== $name ==="
    
    # Compile
    ./mlp-gcc -O2 $bench -o /tmp/$name
    
    # Run 10 times, average
    total=0
    for i in {1..10}; do
        time=$( (/usr/bin/time -f "%e" /tmp/$name) 2>&1 )
        total=$(echo "$total + $time" | bc)
    done
    avg=$(echo "scale=3; $total / 10" | bc)
    
    echo "Average: ${avg}s"
done
```

**Performance Targets:**
```
Benchmark          | C (gcc -O2) | MLP-GCC | Target
-------------------|-------------|---------|--------
Fibonacci(40)      | 0.8s        | ???     | <1.0s
Matrix 1000x1000   | 2.1s        | ???     | <2.5s
JSON parse 10MB    | 0.3s        | ???     | <0.5s
String concat 1M   | 0.15s       | ???     | <0.25s
```

**Test:**
```bash
# Run benchmarks
./benchmark.sh

# Compare with previous version
./benchmark.sh --compare v1.0.0
```

---

### **Task 6: Profiler Integration** (4 gün)

**Hedef:** Performance profiling tools

**Profiler Tools:**
```bash
# 1. Time profiling (gprof)
./mlp-gcc -pg program.mlp -o program
./program
gprof program gmon.out > profile.txt

# 2. CPU profiling (perf)
./mlp-gcc -O2 program.mlp -o program
perf record -g ./program
perf report

# 3. Flamegraph
./mlp-gcc -O2 program.mlp -o program
perf record -g -F 99 ./program
perf script | flamegraph.pl > flame.svg

# 4. Heap profiling (valgrind)
./mlp-gcc -g program.mlp -o program
valgrind --tool=massif ./program
ms_print massif.out.12345
```

**Built-in Profiler:**
```mlp
-- Profiler API
import profiler from "stdlib/profiler/profiler.mlp"

function slow_operation()
    profiler.start("slow_op")
    # ... some work ...
    profiler.stop("slow_op")
end_function

function main()
    profiler.enable()
    slow_operation()
    profiler.report()  # Print timing stats
end_function
```

**Output:**
```
Profiler Report:
================
Function           Calls  Total Time  Avg Time
slow_op              100      2.50s   25.0ms
parse_json            50      1.20s   24.0ms
compute_hash         500      0.80s    1.6ms
```

**Implementation:**
```
stdlib/profiler/
└── profiler.mlp  (~300 satır)

MELP/runtime/profiler/
└── profiler.c    (~400 satır)  # High-resolution timer
```

**Test:**
```bash
# Profiler test
./mlp-gcc -pg tests/profiler/slow.mlp -o slow
./slow
# Expected: Profile report printed
```

---

### **Task 7: Zero-Cost Abstractions** (3 gün)

**Hedef:** Abstraction without runtime cost

**Örnek:**
```mlp
-- High-level abstraction
import optional from "stdlib/optional.mlp"

function divide(numeric a; numeric b) as optional
    if b == 0 then
        return optional.none()
    end_if
    return optional.some(a / b)
end_function

match divide(10, 2)
    case Some(value):
        yazdir(string(value))
    case None:
        yazdir("Error")
end_match
```

**Compiled C (optimized):**
```c
// Zero-cost! No optional struct overhead
if (b == 0) {
    printf("Error\n");
} else {
    double value = a / b;
    printf("%f\n", value);
}
```

**Optimization:**
- Pattern matching → if/else (no overhead)
- Optional → direct value (no wrapper)
- List iterator → raw loop (no function calls)

**Test:**
```bash
# Assembly comparison
./mlp-gcc -O2 abstract.mlp -S -o abstract.s
wc -l abstract.s  # Should be minimal (zero-cost)
```

---

## 🔗 BAĞIMLILIKLAR

### Önkoşul:
- **0-TODO_SELFHOSTING.md** (%100)

### Sonraki TODO:
- **7-TODO_ECOSYSTEM.md** - Optimize edilmiş packages

---

## 🧪 TEST STRATEJİSİ

```bash
tests/optimization/
├── dce_test.sh
├── constant_fold_test.sh
├── inline_test.sh
├── loop_unroll_test.sh
├── vectorize_test.sh
└── zero_cost_test.sh

benchmarks/
├── fibonacci.mlp
├── matrix_mul.mlp
├── json_parse.mlp
└── string_concat.mlp
```

---

## 📦 DELIVERABLES

```
MELP/C/stage0/modules/optimizer/
├── dce.c               (~350 satır)
├── constant_fold.c     (~400 satır)
├── inline.c            (~500 satır)
├── loop_unroll.c       (~400 satır)
└── vectorize.c         (~600 satır)

stdlib/profiler/
└── profiler.mlp        (~300 satır)

MELP/runtime/profiler/
└── profiler.c          (~400 satır)

benchmarks/              (10+ benchmarks)
```

---

## 🎯 BAŞARI KRİTERLERİ

### Performance Targets

- [ ] Fibonacci(40): <1.0s (C: 0.8s)
- [ ] Matrix 1000x1000: <2.5s (C: 2.1s)
- [ ] JSON parse 10MB: <0.5s (C: 0.3s)
- [ ] Binary size: %20-30 küçük (DCE)

### Optimization Flags

```bash
-O0  # No optimization
-O1  # Basic (constant fold, DCE)
-O2  # Full (inline, loop unroll)
-O3  # Aggressive (vectorization)
```

---

**Son Güncelleme:** 29 Aralık 2025  
**Hazırlayan:** PD_01 (Danışman)

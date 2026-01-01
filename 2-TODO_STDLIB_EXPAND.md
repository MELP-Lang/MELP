# 2-TODO_STDLIB_EXPAND.md

**Hedef:** Gerçek Dünya Uygulamaları İçin stdlib Genişletme  
**Süre:** 4-5 hafta (Hafta 10-14)  
**Öncelik:** Kritik  
**Bağımlılık:** 0-TODO_SELFHOSTING.md (%100), 1-TODO_TOOLING_BASICS.md (%100)

---

## 📋 GENEL BAKIŞ

TODO #0 sonrası **temel stdlib** var (list, optional, string, math) ama **gerçek uygulamalar** için yetersiz:
- ❌ Networking yok (HTTP, TCP/IP)
- ❌ Concurrency yok (threads, async)
- ❌ Advanced collections yok (HashMap, Set, Tree)
- ❌ File I/O sınırlı
- ❌ JSON/XML parsing eksik
- ❌ Time/Date library yok

Bu TODO **production-ready stdlib** oluşturacak.

---

## 🎯 TODO HEDEFLERİ

### Başarı Kriterleri

**TODO tamamlandığında:**
- ✅ TCP/UDP soketleri çalışıyor
- ✅ HTTP client library (GET, POST, headers)
- ✅ Basic threading (thread create, join, mutex)
- ✅ HashMap, Set, BTree collections
- ✅ JSON parser/serializer
- ✅ File I/O complete (read, write, append, async)
- ✅ Time/Date library (now, format, parse, timezone)
- ✅ Regex support (pattern matching)

---

## 📊 TASK BREAKDOWN

### ✅ **Task 1: Networking Library** (6-7 gün) - TAMAMLANDI

**Hedef:** TCP/UDP soketler + HTTP client

**Durum:** ✅ YZ_01 tamamladı (1 Ocak 2026)
- stdlib/net/socket.mlp (304 satır)
- stdlib/net/http.mlp (398 satır)
- stdlib/net/url.mlp (233 satır)
- MELP/runtime/net/socket.c (467 satır)
- MELP/runtime/net/http.c (551 satır)
- 5/5 tests passing ✓

---

### ✅ **Task 2: Threading & Concurrency** (5-6 gün) - TAMAMLANDI

**Atanan:** YZ_02 (1 Ocak 2026)  
**Tamamlanma:** 1 Ocak 2026 (aynı gün)  
**Hedef:** Basic multi-threading support

**Durum:** ✅ BAŞARIYLA TAMAMLANDI
- stdlib/thread/thread.mlp (305 satır) ✓
- stdlib/thread/mutex.mlp (321 satır) ✓
- stdlib/thread/channel.mlp (582 satır) ✓
- MELP/runtime/thread/thread.c (180 satır) ✓
- MELP/runtime/thread/mutex.c (260 satır) ✓
- MELP/runtime/thread/channel.c (380 satır) ✓
- 22/22 tests passing ✓

**Modüller:**
```
stdlib/thread/
├── thread.mlp      (305 satır)  # Thread create/join ✅
├── mutex.mlp       (321 satır)  # Mutual exclusion ✅
└── channel.mlp     (582 satır)  # Message passing ✅
```

**ŞABLON Tasarımı (Terminoloji Düzeltmesi: API → ŞABLON):**
```mlp
import thread from "stdlib/thread/thread.mlp"
import mutex from "stdlib/thread/mutex.mlp"

-- ✅ Thread = pure function çağrısı (6. TEMEL ESAS)
function worker(numeric id)
    yazdir("Worker " + string(id))
end_function

result1 = thread.run(worker, 1)  -- Dirildi → spawn → join → öldü
result2 = thread.run(worker, 2)  -- Yeni instantiate

-- ✅ Mutex = closure pattern (MODÜL=ŞABLON)
numeric shared_counter = 0

result = mutex.with_lock("counter_key", lambda() =>
    -- Critical section
    shared_counter = shared_counter + 1
    return shared_counter
)
-- mutex: dirildi → lock → closure çalıştı → unlock → öldü

-- ✅ Channel = functional message passing
result = channel.send_receive("my_channel", 42, lambda(msg) =>
    yazdir("Received: " + string(msg))
    return msg * 2
)
-- channel: dirildi → send → receive → process → öldü
```

**⚠️ ESKİ TASARIM (KALDIRIN - 6. TEMEL ESAS'a AYKIRI!):**
```mlp
-- ❌ YANLIŞ: Persistent state (YASAK!)
mutex m = mutex.create()
mutex.lock(m)
mutex.unlock(m)
mutex.destroy(m)

-- ❌ YANLIŞ: Long-lived resource (YASAK!)
channel ch = channel.create()
channel.send(ch, 42)
channel.close(ch)
```

**NOT:** Tüm eski örnekler functional pattern'e çevrilmelidir!
```

**C Binding:**
```c
// MELP/runtime/thread/thread.c (~350 satır)
// MELP/runtime/thread/mutex.c (~200 satır)
// MELP/runtime/thread/channel.c (~400 satır)
// pthread (Linux) + Windows threads
```

**Test:**
```bash
# C Runtime Tests (All Passing ✅)
cd tests/thread
./run_all_tests.sh

# Results:
# - test_thread_basic: 5/5 tests PASS ✅
# - test_mutex_basic: 7/7 tests PASS ✅
# - test_channel_basic: 10/10 tests PASS ✅
# Total: 22/22 tests passing (100%) ✅
```

**Test Detayları:** [tests/thread/TEST_RESULTS.md](tests/thread/TEST_RESULTS.md)

**Başarı Kriteri:** 22+ tests passing ✅ (AŞILDI)

---

### **Task 3: Advanced Collections** (4-5 gün) ✅ TAMAMLANDI

**Hedef:** HashMap, Set, BTree

**Tamamlanma:** 6 Ocak 2026 (STDLIB_YZ_01)

**Modüller:**
```
stdlib/collections/
├── hashmap.mlp     (~400 satır)  # Hash table
├── set.mlp         (~250 satır)  # Hash set  
└── btree.mlp       (~350 satır)  # Balanced tree (opsiyonel)
```

**Neden Kritik:** Stage2 parser'da symbol table için HashMap, import resolution için Set gerekli!

**API Tasarımı:**
```mlp
import hashmap from "stdlib/collections/hashmap.mlp"
import set from "stdlib/collections/set.mlp"

-- HashMap<string, numeric> (symbol table pattern)
hashmap symbols = hashmap.create()
hashmap.insert(symbols, "variable_x", 42)
hashmap.insert(symbols, "function_main", 100)

optional addr = hashmap.get(symbols, "variable_x")
if addr.is_some() then
    numeric value = addr.unwrap()  # 42
end_if

-- Set<string> (import resolution pattern)
set imported_modules = set.create()
set.add(imported_modules, "stdlib/io/file.mlp")
set.add(imported_modules, "stdlib/math/math.mlp")
set.add(imported_modules, "stdlib/io/file.mlp")  # Duplicate, ignored

bool has = set.contains(imported_modules, "stdlib/io/file.mlp")  # true
numeric count = set.size(imported_modules)  # 2
```

**C Implementation:**
```c
// MELP/runtime/collections/hashmap.c (~500 satır)
// MELP/runtime/collections/set.c (~300 satır)
// MELP/runtime/collections/btree.c (~450 satır, opsiyonel)
// Open addressing hash table, Red-Black tree
```

**Test:**
```bash
# HashMap test (symbol table simulation)
./mlp-gcc tests/collections/hashmap_symbols.mlp && ./a.out

# Set test (import resolution simulation)
./mlp-gcc tests/collections/set_imports.mlp && ./a.out

# Performance test (100k insertions)
time ./mlp-gcc tests/collections/perf.mlp && time ./a.out
# Expected: <0.5s for 100k ops
```

**Başarı Kriteri:** 10+ tests passing ✅

---

### ✅ **Task 4: JSON Parsing** (5 gün) - TAMAMLANDI

**Atanan:** YZ_03 (STDLIB_YZ_03)  
**Tamamlanma:** 1 Ocak 2026  
**Hedef:** JSON parser/serializer - **Import metadata için ŞART!**

**Durum:** ✅ BAŞARIYLA TAMAMLANDI
- stdlib/json/json.mlp (406 satır) ✓
- MELP/runtime/json/parser.h (206 satır) ✓
- MELP/runtime/json/parser.c (621 satır) ✓
- MELP/runtime/json/stringify.c (231 satır) ✓
- 5 test suites, 35+ individual tests, all passing ✓

**Modüller:**
```
stdlib/json/
└── json.mlp        (406 satır)  # JSON parser/serializer ✅

MELP/runtime/json/
├── parser.h        (206 satır)  # Type definitions & API ✅
├── parser.c        (621 satır)  # Recursive descent parser ✅
└── stringify.c     (231 satır)  # JSON serialization ✅
```

**Neden Kritik:** Stage2 import sistemi module metadata'yı JSON formatında saklayacak!

**ŞABLON Tasarımı (6. TEMEL ESAS):**
```mlp
import json from "stdlib/json/json.mlp"

-- ✅ JSON parsing (module metadata pattern - KRİTİK!)
string metadata = '{"name": "math", "version": "1.0", "exports": ["add", "sub"]}'
parse_result result = json.parse(metadata)

if result.success then
    json_object obj = json.get_object(result.value)
    string name = json.object_get_string(obj, "name", "unknown")  # "math"
    string version = json.object_get_string(obj, "version", "0.0")  # "1.0"
    
    -- Array handling (exports list)
    json_value exports_val = json.object_get(obj, "exports")
    if json.is_array(exports_val) then
        json_array exports = json.get_array(exports_val)
        numeric count = json.array_length(exports)  # 2
        string first = json.array_get_string(exports, 0, "")  # "add"
        string second = json.array_get_string(exports, 1, "")  # "sub"
    end_if
    
    json.free(result.value)  # Cleanup
else
    yazdir("Parse error at line " + string(result.error_line))
end_if

-- ✅ JSON serialization (create metadata)
json_value obj = json.create_object()
json.object_set_string(obj, "name", "parser")
json.object_set_string(obj, "version", "2.0")

json_value funcs = json.create_array()
json.array_append_string(funcs, "parse")
json.array_append_string(funcs, "tokenize")
json.object_set(obj, "exports", funcs)

string output = json.stringify(obj, true)  # Pretty print
json.free(obj)
```

**C Implementation:**
```c
// MELP/runtime/json/parser.c (621 satır)
// - Recursive descent parser
// - Dynamic string buffers (no size limit!)
// - Line/column error tracking
// - UTF-8 support

// MELP/runtime/json/stringify.c (231 satır)
// - Pretty print & compact modes
// - Escape sequence handling
// - Efficient string building
```

**Test Results:**
```bash
cd tests/json
./run_all_tests.sh

# Results:
# ✅ test_parse_basic: 7/7 tests PASS (null, bool, num, str, arr, obj, nested)
# ✅ test_parse_metadata: 4/4 tests PASS (module metadata pattern!)
# ✅ test_stringify: 6/6 tests PASS (primitives, roundtrip, escapes)
# ✅ test_invalid: 9/9 tests PASS (error handling robust)
# ✅ test_large: 5/5 tests PASS (1000 elem array, 500 key obj, 10K char str)
# 
# Total: 5/5 test suites passing (100%) ✅
# Total: 35+ individual tests passing ✅
# Performance: Parse 100 arrays × 10 numbers in 0.27 ms ⚡
```

**Test Detayları:** [tests/json/run_all_tests.sh](tests/json/run_all_tests.sh)

**Başarı Kriteri:** 12+ tests passing ✅ (AŞILDI: 35+ tests)

---

### **Task 5: File I/O Complete** (3 gün)

**Hedef:** Advanced file operations - **Modül yükleme için ŞART!**

**Modül Güncelleme:**
```
stdlib/io/
├── file.mlp        (genişlet ~350 satır → ~550 satır)
├── async_file.mlp  (~250 satır)  # Async operations
└── path.mlp        (~200 satır)  # Path utilities
```

**Neden Kritik:** Stage2 import sistemi modülleri dosya sisteminden yükleyecek!

**Yeni Özellikler:**
```mlp
import file from "stdlib/io/file.mlp"
import path from "stdlib/io/path.mlp"

-- Modül yükleme pattern
string module_path = path.join("stdlib", "math", "math.mlp")
bool exists = path.exists(module_path)

if exists then
    optional content = file.read(module_path)
    if content.is_some() then
        string source_code = content.unwrap()
        -- Parse and compile...
    end_if
end_if

-- Path operations (import resolution için)
string absolute = path.absolute("../stdlib/io/file.mlp")
string normalized = path.normalize("stdlib//io/../io/./file.mlp")  # "stdlib/io/file.mlp"
string dir = path.dirname("stdlib/io/file.mlp")  # "stdlib/io"
string base = path.basename("stdlib/io/file.mlp")  # "file.mlp"

-- Directory listing (module discovery)
list files = file.list_dir("stdlib/")
for each item in files do
    if path.extension(item) == ".mlp" then
        yazdir("Found module: " + item)
    end_if
end_for

-- Binary file operations (compiled module cache)
list bytes = file.read_bytes("stdlib/math.mlp.cache")
file.write_bytes("stdlib/math.mlp.cache.backup", bytes)

-- File metadata (cache invalidation)
file_info info = file.stat("stdlib/math.mlp")
numeric modified = info.modified_time
bool needs_recompile = (cache_time < modified)
```

**C Implementation:**
```c
// MELP/runtime/io/file.c (~400 satır genişletme)
// MELP/runtime/io/async_file.c (~400 satır)
// MELP/runtime/io/path.c (~300 satır)
// POSIX file operations + path normalization
```

**Test:**
```bash
# Module loading simulation test
./mlp-gcc tests/io/module_load.mlp && ./a.out

# Path operations test
./mlp-gcc tests/io/path_ops.mlp && ./a.out

# Directory listing test
./mlp-gcc tests/io/list_modules.mlp && ./a.out
# Expected: Lists all .mlp files in stdlib/

# Large file streaming (1GB file)
dd if=/dev/urandom of=test_huge.bin bs=1M count=1024
time ./mlp-gcc tests/io/stream.mlp && time ./a.out
# Expected: <10s for 1GB, low memory usage
```

**Başarı Kriteri:** 8+ tests passing ✅

---

### ❌ **Task 4-7: ATLANDI**

**Atlandı:**
- ❌ Time/Date Library → TODO #7'de yapılacak
- ❌ Regex Support → TODO #7'de yapılacak

**Sebep:** Self-hosting için kritik değil, ecosystem için gerekli

---

## ⏱️ ZAMAN ÇİZELGESİ (TODO #2-MINI)

| Gün | Task | YZ | Çıktı |
|-----|------|-----|-------|
| **1-5** | Collections | YZ_01 | HashMap, Set, BTree |
| **6-10** | JSON | YZ_02 | json.mlp + parser.c |
| **11-13** | File I/O | YZ_03 | file.mlp, path.mlp |

**TOPLAM:** 13 gün = **2 hafta** ✅

**Başlangıç:** 1 Ocak 2026  
**Bitiş:** ~15 Ocak 2026  
**Sonra:** TODO #3 (LANGUAGE_FEATURES) başlar!

---

## 🧪 GERÇEK TESTLER - TODO TAMAMLANMA KRİTERLERİ

### ⚠️ UYARI: Tüm testler geçmeden TODO tamamlanmış sayılmaz!

**Test Dosyaları:** `tests/stdlib_mini/`

```bash
# Test 1-10: Collections Tests (YZ_01)
tests/stdlib_mini/collections/
├── test_hashmap_insert.mlp      # HashMap insert/get ✅
├── test_hashmap_delete.mlp      # HashMap delete ✅
├── test_hashmap_iterate.mlp     # HashMap iteration ✅
├── test_hashmap_collision.mlp   # Hash collision handling ✅
├── test_set_add.mlp             # Set add/contains ✅
├── test_set_union.mlp           # Set union/intersection ✅
├── test_set_duplicate.mlp       # Duplicate handling ✅
├── test_btree_insert.mlp        # BTree ordered insert ✅
├── test_btree_traverse.mlp      # BTree in-order traversal ✅
└── test_perf_100k.mlp           # 100k ops <0.5s ✅

# Test 11-22: JSON Tests (YZ_02)
tests/stdlib_mini/json/
├── test_parse_object.mlp        # Parse JSON object ✅
├── test_parse_array.mlp         # Parse JSON array ✅
├── test_parse_nested.mlp        # Nested structures ✅
├── test_parse_metadata.mlp      # Module metadata pattern ✅
├── test_stringify_object.mlp    # Serialize object ✅
├── test_stringify_array.mlp     # Serialize array ✅
├── test_invalid_json.mlp        # Error handling ✅
├── test_utf8.mlp                # UTF-8 support ✅
├── test_escape_chars.mlp        # Escape sequences ✅
├── test_large_json.mlp          # 10MB JSON <1s ✅
├── test_array_operations.mlp    # Array get/set ✅
└── test_number_precision.mlp    # Number parsing ✅

# Test 23-30: File I/O Tests (YZ_03)
tests/stdlib_mini/io/
├── test_module_load.mlp         # Module loading pattern ✅
├── test_path_join.mlp           # Path joining ✅
├── test_path_normalize.mlp      # Path normalization ✅
├── test_path_absolute.mlp       # Absolute path ✅
├── test_file_exists.mlp         # File existence check ✅
├── test_list_dir.mlp            # Directory listing ✅
├── test_file_stat.mlp           # File metadata ✅
└── test_large_file_stream.mlp   # 1GB streaming <10s ✅
```

### 📊 TEST RAPORU FORMATI

**Dosya:** `TEST_RAPORU_TODO2_MINI.md`

```markdown
# TODO #2-MINI TEST RAPORU

## ÖZET
- **Toplam Test:** 30
- **Başarılı:** 30/30 ✅
- **Başarısız:** 0/30 ✅
- **Test Süresi:** ~45 saniye

## DETAY

### Collections Tests (10/10 ✅)
- test_hashmap_insert.mlp: PASS (80ms)
- test_hashmap_delete.mlp: PASS (75ms)
- test_hashmap_iterate.mlp: PASS (90ms)
- test_hashmap_collision.mlp: PASS (120ms)
- test_set_add.mlp: PASS (60ms)
- test_set_union.mlp: PASS (100ms)
- test_set_duplicate.mlp: PASS (50ms)
- test_btree_insert.mlp: PASS (110ms)
- test_btree_traverse.mlp: PASS (95ms)
- test_perf_100k.mlp: PASS (450ms) - Target <500ms ✅

### JSON Tests (12/12 ✅)
- test_parse_object.mlp: PASS (70ms)
- test_parse_array.mlp: PASS (65ms)
- test_parse_nested.mlp: PASS (120ms)
- test_parse_metadata.mlp: PASS (85ms)
- test_stringify_object.mlp: PASS (60ms)
- test_stringify_array.mlp: PASS (55ms)
- test_invalid_json.mlp: PASS (80ms) - Error detected ✅
- test_utf8.mlp: PASS (90ms)
- test_escape_chars.mlp: PASS (70ms)
- test_large_json.mlp: PASS (950ms) - Target <1s ✅
- test_array_operations.mlp: PASS (75ms)
- test_number_precision.mlp: PASS (65ms)

### File I/O Tests (8/8 ✅)
- test_module_load.mlp: PASS (150ms)
- test_path_join.mlp: PASS (40ms)
- test_path_normalize.mlp: PASS (50ms)
- test_path_absolute.mlp: PASS (45ms)
- test_file_exists.mlp: PASS (35ms)
- test_list_dir.mlp: PASS (180ms)
- test_file_stat.mlp: PASS (60ms)
- test_large_file_stream.mlp: PASS (8500ms) - Target <10s ✅

## SONUÇ
✅ TODO #2-MINI TAMAMLANDI - Tüm testler geçti!
✅ Stage2 için kritik stdlib hazır!
✅ TODO #3 başlayabilir! (15 Ocak 2026)
```

### 🎯 BAŞARI KRİTERİ

**TODO #2-MINI tamamlanabilir ancak ve ancak:**
- ✅ 30/30 test geçiyor
- ✅ Collections: HashMap, Set, BTree çalışıyor
- ✅ JSON: Parse + stringify çalışıyor
- ✅ File I/O: Module loading pattern çalışıyor
- ✅ Performance: Collections <0.5s, JSON <1s, File streaming <10s
- ✅ TODO #3 başlatılabilir!

---

**Son Güncelleme:** 1 Ocak 2026  
**PD Kararı:** TODO #2 filtrelendi (7 task → 3 task)  
**Kazanç:** 3 hafta kazanç! (5 hafta → 2 hafta)  
**Sonraki:** TODO #3 (15 Ocak 2026)

**Test:**
```bash
# Time test
./mlp-gcc tests/time/basic.mlp && ./a.out

# Performance test
time ./mlp-gcc tests/time/benchmark.mlp && time ./a.out
# Expected: time.now() <1µs
```

---

### **Task 7: Regex Support** (4 gün)

**Hedef:** Pattern matching with regex

**Modül:**
```
stdlib/regex/
└── regex.mlp       (~350 satır)
```

**API Tasarımı:**
```mlp
import regex from "stdlib/regex/regex.mlp"

-- Match
string pattern = "^[a-z]+@[a-z]+\\.[a-z]+$"
bool matches = regex.is_match(pattern, "test@example.com")  # true

-- Find all
string text = "Email: test@example.com, Contact: info@site.org"
list matches = regex.find_all("\\w+@\\w+\\.\\w+", text)
# matches = ["test@example.com", "info@site.org"]

-- Replace
string result = regex.replace("\\d+", text, "[NUM]")

-- Capture groups
string pattern = "(\\w+)@(\\w+)\\.(\\w+)"
optional groups = regex.capture(pattern, "test@example.com")
# groups = ["test", "example", "com"]
```

**C Implementation:**
```c
// MELP/runtime/regex/regex.c (~500 satır)
// POSIX regex.h or PCRE2
```

**Test:**
```bash
# Email validation
./mlp-gcc tests/regex/email.mlp && ./a.out

# Performance (1000 matches)
time ./mlp-gcc tests/regex/perf.mlp && time ./a.out
```

---

## 🔗 BAĞIMLILIKLAR

### Önkoşul:
- **0-TODO_SELFHOSTING.md** (%100)
  - Runtime modüler olmalı
  - Import sistemi çalışmalı
- **1-TODO_TOOLING_BASICS.md** (%100)
  - LSP stdlib'i index etmeli
  - Formatter stdlib'i formatlamalı

### Sonraki TODO:
- **3-TODO_LANGUAGE_FEATURES.md**
  - Generics ile HashMap<K,V> yazılabilecek
  - Pattern matching ile Result<T,E> handling

---

## 🧪 TEST STRATEJİSİ

### Unit Tests

```bash
tests/stdlib/
├── net/
│   ├── tcp_test.mlp
│   ├── http_get_test.mlp
│   └── http_post_test.mlp
├── thread/
│   ├── basic_thread.mlp
│   ├── mutex_test.mlp
│   └── channel_test.mlp
├── collections/
│   ├── hashmap_test.mlp
│   ├── hashset_test.mlp
│   └── btree_test.mlp
├── data/
│   ├── json_parse.mlp
│   └── xml_parse.mlp
├── io/
│   ├── async_io.mlp
│   └── large_file.mlp
├── time/
│   ├── format_test.mlp
│   └── duration_test.mlp
└── regex/
    ├── match_test.mlp
    └── replace_test.mlp
```

### Integration Tests

```bash
# Real-world scenario: Web scraper
function web_scraper()
    import http from "stdlib/net/http.mlp"
    import json from "stdlib/data/json.mlp"
    import regex from "stdlib/regex/regex.mlp"
    
    optional response = http.get("https://api.github.com/repos/mlp/mlp")
    if response.is_some() then
        string body = response.unwrap().body
        optional data = json.parse(body)
        # Extract data...
    end_if
end_function
```

### Performance Tests

```bash
# Benchmark suite
./benchmark_stdlib.sh

# Expected results:
# - HTTP request: <100ms
# - Thread creation: <1ms
# - HashMap 1M ops: <2s
# - JSON parse 1MB: <50ms
# - File read 100MB: <5s
# - Regex match 1000x: <10ms
```

---

## 📦 DELIVERABLES

### Yeni Modüller

```
stdlib/
├── net/
│   ├── socket.mlp      # TCP/UDP
│   ├── http.mlp        # HTTP client
│   └── url.mlp         # URL parsing
├── thread/
│   ├── thread.mlp      # Threading
│   ├── mutex.mlp       # Sync primitives
│   └── channel.mlp     # Message passing
├── collections/
│   ├── hashmap.mlp     # Hash table
│   ├── hashset.mlp     # Hash set
│   └── btree.mlp       # Balanced tree
├── data/
│   ├── json.mlp        # JSON parser
│   └── xml.mlp         # XML parser
├── io/
│   └── file_io.mlp     # Advanced I/O (güncelle)
├── time/
│   ├── time.mlp        # Time operations
│   └── date.mlp        # Date formatting
└── regex/
    └── regex.mlp       # Regex support
```

### C Runtime

```
MELP/runtime/
├── net/                # Networking (~900 satır)
├── thread/             # Threading (~950 satır)
├── collections/        # Collections (~1350 satır)
├── data/               # JSON/XML (~1300 satır)
├── time/               # Time/Date (~750 satır)
└── regex/              # Regex (~500 satır)

TOPLAM: ~5750 satır C kodu (modüler, her dosya <500)
```

### Dokümantasyon

```
docs/stdlib/
├── NET_API.md          # Networking API
├── THREAD_API.md       # Threading API
├── COLLECTIONS_API.md  # Collections API
├── DATA_API.md         # JSON/XML API
├── IO_API.md           # File I/O API
├── TIME_API.md         # Time/Date API
└── REGEX_API.md        # Regex API
```

---

## 🎯 BAŞARI KRİTERLERİ

### Minimum Viable Product (MVP)

- [ ] HTTP GET/POST çalışıyor
- [ ] Basic threading (spawn, join)
- [ ] HashMap, HashSet implemented
- [ ] JSON parse/stringify
- [ ] File I/O async support
- [ ] Time formatting
- [ ] Basic regex matching

### Nice to Have (Bonus)

- [ ] WebSocket support
- [ ] Thread pools
- [ ] BTree optimizasyonları
- [ ] XML namespaces
- [ ] File watching (inotify)
- [ ] Timezone database
- [ ] Regex compilation cache

### Regression Tests

```bash
# Tüm stdlib testleri
./run_tests.sh stdlib
# Expected: 100% pass

# Performance benchmarks
./benchmark_stdlib.sh
# Expected: All benchmarks pass thresholds
```

---

## 💡 TEKNİK KARARLAR

### Networking

**Seçim:** POSIX sockets (Linux) + WinSock (Windows)  
**Neden:** Standard, performant, her yerde var  
**Alternatif:** libuv (ama bağımlılık ekler)

### Threading

**Seçim:** pthread (Linux) + Windows threads  
**Neden:** Low-level control, performant  
**Alternatif:** C11 threads (ama platform desteği eksik)

### Collections

**Seçim:** Custom implementation (scratch'ten yaz)  
**Neden:** Bağımlılık yok, STO ile entegre  
**Alternatif:** libcollections (ama OOP-heavy)

### JSON

**Seçim:** Custom recursive descent parser  
**Neden:** Dependency-free, 600 satır  
**Alternatif:** jsmn, cJSON (ama lisans, style)

### Regex

**Seçim:** POSIX regex (basic), PCRE2 (advanced - opsiyonel)  
**Neden:** POSIX standard, PCRE2 güçlü  
**Alternatif:** Custom NFA (ama karmaşık)

---

## 📅 P2'DEN TRANSFER

**P2 (MLP-LLVM)'de bazı modüller var:**
- Networking yok (sıfırdan yazılacak)
- Threading yok (sıfırdan)
- JSON parser var (~800 satır) ✅ AL!
- File I/O var (ama LLVM-dependent) ⏳ LLVM temizle

**Transfer Planı:**
- Task 4'te P2 JSON parser'ı al, temizle
- Task 5'te P2 File I/O'yu incele, uyarla

---

**Son Güncelleme:** 29 Aralık 2025  
**Hazırlayan:** PD_01 (Danışman)

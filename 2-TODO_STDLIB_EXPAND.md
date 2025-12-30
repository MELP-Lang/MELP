# 2-TODO_STDLIB_EXPAND.md

**Hedef:** Gerçek Dünya Uygulamaları İçin stdlib Genişletme  
**Süre:** 3-4 hafta (Hafta 10-13)  
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

### **Task 1: Networking Library** (6-7 gün)

**Hedef:** TCP/UDP soketler + HTTP client

**Modüller:**
```
stdlib/net/
├── socket.mlp      (~300 satır)  # TCP/UDP soketler
├── http.mlp        (~400 satır)  # HTTP client
└── url.mlp         (~200 satır)  # URL parsing
```

**API Tasarımı:**
```mlp
import socket from "stdlib/net/socket.mlp"
import http from "stdlib/net/http.mlp"

-- TCP socket
optional sock = socket.connect("example.com", 80)
if sock.is_some() then
    socket.write(sock.unwrap(), "GET / HTTP/1.1\r\n")
    optional response = socket.read(sock.unwrap())
end_if

-- HTTP client (high-level)
optional response = http.get("https://example.com")
if response.is_some() then
    string body = response.unwrap().body
    numeric status = response.unwrap().status  # 200
end_if

-- POST request
http_request req
req.url = "https://api.example.com/data"
req.method = "POST"
req.body = '{"key": "value"}'
req.headers["Content-Type"] = "application/json"

optional resp = http.send(req)
```

**C Binding:**
```c
// MELP/runtime/net/socket.c (~400 satır)
// MELP/runtime/net/http.c (~500 satır)
// Posix sockets (Linux) + WinSock (Windows)
```

**Test:**
```bash
# TCP test
./mlp-gcc tests/net/tcp_client.mlp && ./a.out

# HTTP test
./mlp-gcc tests/net/http_get.mlp && ./a.out
# Expected: HTTP 200 response
```

---

### **Task 2: Threading & Concurrency** (5-6 gün)

**Hedef:** Basic multi-threading support

**Modüller:**
```
stdlib/thread/
├── thread.mlp      (~300 satır)  # Thread create/join
├── mutex.mlp       (~200 satır)  # Mutual exclusion
└── channel.mlp     (~350 satır)  # Message passing
```

**API Tasarımı:**
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
# Thread test
./mlp-gcc tests/thread/basic_thread.mlp && ./a.out

# Race condition test (should be safe with mutex)
./mlp-gcc tests/thread/race_condition.mlp && ./a.out
# Expected: counter = 10000 (her zaman)
```

---

### **Task 3: Advanced Collections** (4-5 gün)

**Hedef:** HashMap, Set, BTree

**Modüller:**
```
stdlib/collections/
├── hashmap.mlp     (~400 satır)  # Hash table
├── hashset.mlp     (~300 satır)  # Hash set
└── btree.mlp       (~350 satır)  # Balanced tree
```

**API Tasarımı:**
```mlp
import hashmap from "stdlib/collections/hashmap.mlp"
import hashset from "stdlib/collections/hashset.mlp"

-- HashMap<string, numeric>
hashmap ages = hashmap.create()
hashmap.insert(ages, "Ali", 25)
hashmap.insert(ages, "Ayşe", 30)

optional age = hashmap.get(ages, "Ali")
if age.is_some() then
    numeric value = age.unwrap()  # 25
end_if

-- HashSet<string>
hashset names = hashset.create()
hashset.add(names, "Ali")
hashset.add(names, "Ayşe")
hashset.add(names, "Ali")  # Duplicate, ignored

bool has = hashset.contains(names, "Ali")  # true
numeric size = hashset.size(names)  # 2

-- BTree (ordered map)
btree tree = btree.create()
btree.insert(tree, 5, "five")
btree.insert(tree, 2, "two")
btree.insert(tree, 8, "eight")

# In-order traversal: 2, 5, 8
```

**C Implementation:**
```c
// MELP/runtime/collections/hashmap.c (~500 satır)
// MELP/runtime/collections/hashset.c (~400 satır)
// MELP/runtime/collections/btree.c (~450 satır)
// Open addressing hash table, Red-Black tree
```

**Test:**
```bash
# HashMap test
./mlp-gcc tests/collections/hashmap_test.mlp && ./a.out

# Performance test (1M insertions)
time ./mlp-gcc tests/collections/hashmap_perf.mlp && time ./a.out
# Expected: <2s for 1M ops
```

---

### **Task 4: JSON & XML Parsing** (4-5 gün)

**Hedef:** JSON/XML parser + serializer

**Modüller:**
```
stdlib/data/
├── json.mlp        (~400 satır)  # JSON parser
└── xml.mlp         (~450 satır)  # XML parser
```

**API Tasarımı:**
```mlp
import json from "stdlib/data/json.mlp"

-- JSON parsing
string json_str = '{"name": "Ali", "age": 25}'
optional parsed = json.parse(json_str)

if parsed.is_some() then
    json_object obj = parsed.unwrap()
    string name = json.get_string(obj, "name")  # "Ali"
    numeric age = json.get_number(obj, "age")   # 25
end_if

-- JSON serialization
json_object obj = json.object_create()
json.set_string(obj, "name", "Ayşe")
json.set_number(obj, "age", 30)

string output = json.stringify(obj)
# output = '{"name":"Ayşe","age":30}'

-- JSON arrays
string arr_str = '[1, 2, 3, 4, 5]'
optional arr = json.parse(arr_str)
numeric len = json.array_length(arr.unwrap())  # 5
```

**C Implementation:**
```c
// MELP/runtime/data/json.c (~600 satır)
// MELP/runtime/data/xml.c (~700 satır)
// Recursive descent parser
```

**Test:**
```bash
# JSON test
echo '{"test": true}' | ./mlp-gcc tests/json/parse.mlp && ./a.out

# XML test
echo '<root><item>test</item></root>' | ./mlp-gcc tests/xml/parse.mlp && ./a.out
```

---

### **Task 5: File I/O Complete** (3 gün)

**Hedef:** Advanced file operations

**Modül Güncelleme:**
```
stdlib/io/
└── file_io.mlp     (güncelle ~500 satır)
```

**Yeni Özellikler:**
```mlp
import file_io from "stdlib/io/file_io.mlp"

-- Async file read
optional content = file_io.read_async("large_file.txt")

-- Binary file operations
list bytes = file_io.read_bytes("image.png")
file_io.write_bytes("copy.png", bytes)

-- File streaming (büyük dosyalar için)
file_stream stream = file_io.open_stream("huge.log")
while file_io.has_next(stream) do
    string line = file_io.read_line(stream)
    yazdir(line)
end_while
file_io.close_stream(stream)

-- File metadata
file_info info = file_io.stat("test.txt")
numeric size = info.size         # bytes
string modified = info.modified  # timestamp
bool is_dir = info.is_directory
```

**Test:**
```bash
# Large file test (100MB)
dd if=/dev/urandom of=test_large.bin bs=1M count=100
./mlp-gcc tests/io/large_file.mlp && time ./a.out
# Expected: <5s for 100MB
```

---

### **Task 6: Time & Date Library** (3-4 gün)

**Hedef:** Modern time/date handling

**Modüller:**
```
stdlib/time/
├── time.mlp        (~350 satır)  # Time operations
└── date.mlp        (~300 satır)  # Date formatting
```

**API Tasarımı:**
```mlp
import time from "stdlib/time/time.mlp"
import date from "stdlib/time/date.mlp"

-- Current time
numeric now = time.now()  # Unix timestamp
string formatted = time.format(now, "2006-01-02 15:04:05")

-- Date parsing
optional parsed = date.parse("2025-12-29", "YYYY-MM-DD")
if parsed.is_some() then
    numeric timestamp = parsed.unwrap()
end_if

-- Duration
numeric start = time.now()
# ... some operation ...
numeric end = time.now()
numeric elapsed = time.duration(start, end)  # milliseconds

-- Sleep
time.sleep(1000)  # 1 second

-- Timezone
string utc = time.format_utc(now)
string local = time.format_local(now, "Europe/Istanbul")
```

**C Implementation:**
```c
// MELP/runtime/time/time.c (~400 satır)
// MELP/runtime/time/date.c (~350 satır)
// POSIX time.h + strftime
```

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

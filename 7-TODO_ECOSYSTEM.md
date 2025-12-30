# 7-TODO_ECOSYSTEM.md

**Hedef:** Full Professional Language Ecosystem  
**Süre:** 4+ hafta (Hafta 24-28+)  
**Öncelik:** Yüksek  
**Bağımlılık:** 0-6 TODO'lar (%100)

---

## 📋 GENEL BAKIŞ

TODO #6 sonrası MLP-GCC **modern, hızlı bir dil** ama ekosistem eksik:
- ❌ GUI framework yok
- ❌ Database drivers yok
- ❌ Web framework yok
- ❌ Test framework yok
- ❌ Documentation generator yok

Bu TODO **FULL PROFESSIONAL ECOSYSTEM** tamamlayacak! 🎉

---

## 🎯 TODO HEDEFLERİ

### Başarı Kriterleri

**TODO tamamlandığında MLP-GCC:**
- ✅ Python/Go/Rust seviyesinde ekosistem
- ✅ Desktop apps yapılabilir (GUI)
- ✅ Web apps yapılabilir (HTTP server, framework)
- ✅ Database apps yapılabilir (PostgreSQL, SQLite)
- ✅ Test framework (unit tests, integration tests)
- ✅ Documentation generator (API docs)
- ✅ CI/CD templates (GitHub Actions)

**🎊 FULL PROFESSIONAL LANGUAGE! 🎊**

---

## 📊 TASK BREAKDOWN

### **Task 1: GUI Framework** (8-10 gün)

**Hedef:** Native desktop apps

**Architecture:**
```
mlp-gui (Cross-platform GUI)
├── Linux: GTK4
├── Windows: Win32 API
├── macOS: Cocoa
```

**API Design:**
```mlp
import gui from "stdlib/gui/gui.mlp"

-- Create window
window = gui.window_create("My App", 800, 600)
**⚠️ IMPORTANT:** GUI Framework = **TOOL + MODÜL** (6. TEMEL ESAS uyumlu)

**Architecture:**
```
GUI Runtime (TOOL - Binary)
├── ./mlp-gui-runtime app.mlp  # Event loop = tool
└── C ile yazılır (GTK4/Win32/Cocoa)

MLP Handler Functions (MODÜL)
├── Event handlers = pure functions
└── Her çağrı independent (MODÜL=ŞABLON)
```

**API Tasarımı:**
```mlp
import gui from "stdlib/gui/gui.mlp"

-- ✅ Handler = pure function (MODÜL)
function on_button_click(event) as action
    return action.update_label("Button clicked!")
end_function

-- ✅ Main loop = caller kontrolünde (6. TEMEL ESAS)
function main()
    window = gui.window_create("Hello", 400, 300)
    button = gui.button_create("Click me!")
    label = gui.label_create("Hello, World!")
    
    -- Event handler registration
    gui.on_click(button, on_button_click)
    
    -- ✅ Event loop (caller'ın sorumluluğu)
    while gui.has_events() do
        event = gui.wait_event()  -- Dirildi → event aldı → öldü
        action = on_button_click(event)  -- Pure function
        gui.apply(action)  -- Dirildi → apply etti → öldü
    end_while
end_function
```

**❌ ESKİ TASARIM (KALDIRIN - 6. TEMEL ESAS'a AYKIRI!):**
```mlp
-- ❌ YANLIŞ: Blocking long-running (YASAK!)
gui.main_loop()  # BLOCKS forever!
```

**Çalıştırma:**
```bash
# GUI app = tool ile çalıştır
./mlp-gui-runtime app.mlp  # Event loop binary'de
```

**Widgets:**
```
- Window, Dialog, Menu
- Button, Label, Entry, Checkbox, RadioButton
- ComboBox, ListBox, TreeView
- ProgressBar, Spinner
- Image, Canvas (drawing)
- Layout: VBox, HBox, Grid
```

**Dosyalar:**
```
stdlib/gui/
├── gui.mlp           (~600 satır)  # Main API
├── window.mlp        (~300 satır)
├── widgets.mlp       (~500 satır)
└── events.mlp        (~250 satır)

MELP/runtime/gui/
├── gtk4_backend.c    (~800 satır)  # Linux
├── win32_backend.c   (~800 satır)  # Windows
└── cocoa_backend.m   (~800 satır)  # macOS
```

**Example App:**
```mlp
-- Todo app
import gui from "stdlib/gui/gui.mlp"
import list from "stdlib/collections/list.mlp"

list todos = list.create()

function add_todo()
    string text = gui.entry_get_text(entry)
    list.add(todos, text)
    gui.listbox_add(listbox, text)
    gui.entry_clear(entry)
end_function

window = gui.window_create("Todo App", 400, 300)
entry = gui.entry_create()
button = gui.button_create("Add")
listbox = gui.listbox_create()

gui.on_click(button, add_todo)

# ... layout and show ...
gui.main_loop()
```

**Test:**
```bash
# GUI test
./mlp-gcc examples/gui/hello.mlp -o hello_gui
./hello_gui  # Window should appear
```

---

### **Task 2: Database Drivers** (6-7 gün)

**Hedef:** PostgreSQL, SQLite, MySQL

**⚠️ IMPORTANT:** Her query = fresh connection (6. TEMEL ESAS: MODÜL=ŞABLON)

**API Design:**
```mlp
import db from "stdlib/db/postgres.mlp"

-- ✅ DOĞRU: Her query independent (MODÜL=ŞABLON)
optional rows = db.query(
    "postgresql://localhost/mydb",
    "SELECT * FROM users WHERE age > $1",
    [25]
)
-- db modülü: dirildi → connect → query → disconnect → öldü

match rows
    case Some(result):
        for row in result do
            numeric id = row["id"]
            string name = row["name"]
            yazdir("User: " + name)
        end_for
    case None:
        yazdir("Query failed")
end_match

-- ✅ Insert (her çağrı fresh)
result = db.execute(
    "postgresql://localhost/mydb",
    "INSERT INTO users (name, age) VALUES ($1, $2)",
    ["Ali", 30]
)

-- ✅ Transaction (closure pattern)
result = db.transaction("postgresql://localhost/mydb", lambda(tx) =>
    tx.execute("INSERT ...")
    tx.execute("UPDATE ...")
    return tx.commit()  # or tx.rollback()
)
-- db modülü: dirildi → connect → begin → queries → commit → disconnect → öldü
```

**❌ ESKİ TASARIM (KALDIRIN - 6. TEMEL ESAS'a AYKIRI!):**
```mlp
-- ❌ YANLIŞ: Persistent connection (YASAK!)
conn = db.connect("postgresql://...")  -- Connection yaşıyor!
result = db.query(conn, "SELECT ...")
db.close(conn)
```

**NOT:** Connection pooling runtime'da optimize edilebilir ama API'de görünmez!

**Supported Databases:**
```
- PostgreSQL (libpq)
- SQLite (sqlite3)
- MySQL (libmysqlclient)
```

**⚠️ STATELESS Connection Pool Pattern:**
```mlp
-- Caller-owned pool ("her modül ölüdür; onu çağıran diriltir ve öldürür")
struct DbPool
    array connections  # Caller sahip
    mutex lock         # Thread-safe
    string dsn
    numeric max_conns
end_struct

function pool_create(string dsn, numeric max_conns) -> DbPool
    # 🟢 Caller diriltir
    DbPool pool = DbPool {
        connections: array.new(max_conns),
        lock: mutex.create(),
        dsn: dsn,
        max_conns: max_conns
    }
    # Pre-fill pool
    for i in 1..max_conns do
        optional conn = db.connect(dsn)
        if conn.is_some() then
            array.push(pool.connections, conn.unwrap())
        end_if
    end_for
    return pool
end_function

function pool_get(DbPool pool) -> optional DbConnection
    mutex.lock(pool.lock)
    optional conn = None
    if array.length(pool.connections) > 0 then
        conn = Some(array.pop(pool.connections))
    end_if
    mutex.unlock(pool.lock)
    return conn
end_function

function pool_release(DbPool pool, DbConnection conn)
    mutex.lock(pool.lock)
    array.push(pool.connections, conn)
    mutex.unlock(pool.lock)
end_function

function pool_destroy(DbPool pool)
    # 🔴 Caller öldürür (RAII)
    for conn in pool.connections do
        db.close(conn)
    end_for
    mutex.destroy(pool.lock)
end_function

-- Usage: Caller full kontrolde
DbPool pool = pool_create("postgres://localhost/mydb", 10)  # Diriltme
optional conn = pool_get(pool)
if conn.is_some() then
    db.query(conn.unwrap(), "SELECT ...")
    pool_release(pool, conn.unwrap())
end_if
pool_destroy(pool)  # Öldürme - caller sorumlu!
```

**Dosyalar:**
```
stdlib/db/
├── postgres.mlp      (~400 satır)
├── sqlite.mlp        (~350 satır)
└── mysql.mlp         (~400 satır)

MELP/runtime/db/
├── postgres.c        (~600 satır)
├── sqlite.c          (~500 satır)
└── mysql.c           (~600 satır)
```

**Test:**
```bash
# PostgreSQL test
docker run -d -p 5432:5432 -e POSTGRES_PASSWORD=test postgres
./mlp-gcc tests/db/postgres_test.mlp && ./a.out
# Expected: Connection success, query results
```

---

### **Task 3: Web Framework** (8-9 gün)

**Hedef:** Express.js / Flask style web framework

**⚠️ IMPORTANT:** Web Server = **TOOL (Binary)**, Handlers = **MLP MODÜLÜ**  
**Neden:** Long-running server = 6. TEMEL ESAS'a aykırı (MODÜL=ŞABLON ihlali)

**Architecture:**
```
Web Server (TOOL - Binary)
├── ./mlp-web-server --port 3000 app.mlp
└── C ile yazılır (HTTP event loop)

Request Handlers (MLP MODÜLÜ)
├── Handler functions = pure functions
└── Her request independent (MODÜL=ŞABLON)
```

**API Design:**
```mlp
import web from "stdlib/web/web.mlp"

-- ✅ Handler = pure function (MODÜL)
function handle_root(req) as res
    return web.response(200, "Hello, World!")
end_function

function handle_user(req) as res
    numeric id = web.param(req, "id")
    return web.json_response(200, {"id": id, "name": "Ali"})
end_function

-- ✅ Routes = configuration (not runtime!)
routes = [
    web.route("GET", "/", handle_root),
    web.route("GET", "/users/:id", handle_user)
]

-- Export için
export routes
```

**❌ ESKİ TASARIM (KALDIRIN - 6. TEMEL ESAS'a AYKIRI!):**
```mlp
-- ❌ YANLIŞ: Blocking server (YASAK!)
app = web.app_create()
web.get(app, "/", handler)
web.listen(app, 3000)  # BLOCKS forever!
```

**Çalıştırma:**
```bash
# Web server = tool ile çalıştır
./mlp-web-server --port 3000 app.mlp  # Server binary'de
```

**Features:**
```
- Routing (GET, POST, PUT, DELETE)
- JSON request/response
- URL parameters, query strings
- Middleware support
- Static file serving
- Template engine (optional)
- WebSocket support
- HTTPS/TLS
```

**Dosyalar:**
```
stdlib/web/
├── web.mlp           (~600 satır)  # Main framework
├── router.mlp        (~400 satır)
├── middleware.mlp    (~300 satır)
└── template.mlp      (~350 satır)

MELP/runtime/web/
├── http_server.c     (~800 satır)  # HTTP server
├── websocket.c       (~500 satır)
└── tls.c             (~400 satır)
```

**Example App:**
```mlp
-- REST API
import web from "stdlib/web/web.mlp"
import db from "stdlib/db/sqlite.mlp"

app = web.app_create()
conn = db.connect("users.db")

-- GET /users
web.get(app, "/users", lambda(req, res) =>
    optional rows = db.query(conn, "SELECT * FROM users")
    web.json(res, rows.unwrap())
)

-- POST /users
web.post(app, "/users", lambda(req, res) =>
    optional body = web.json_body(req)
    string name = body.unwrap()["name"]
    db.execute(conn, "INSERT INTO users (name) VALUES (?)", [name])
    web.status(res, 201)
)

web.listen(app, 8080)
```

**Test:**
```bash
# Web framework test
./mlp-gcc tests/web/server.mlp -o server
./server &
curl http://localhost:8080/
# Expected: "Hello, World!"
```

---

### **Task 4: Test Framework** (5-6 gün)

**Hedef:** Unit testing, assertions, mocking

**API Design:**
```mlp
import test from "stdlib/test/test.mlp"

-- Test suite
test.describe("Calculator", lambda() =>
    
    test.it("should add numbers", lambda() =>
        numeric result = add(2, 3)
        test.assert_equal(result, 5)
    )
    
    test.it("should subtract numbers", lambda() =>
        numeric result = subtract(10, 3)
        test.assert_equal(result, 7)
    )
    
    test.it("should throw on divide by zero", lambda() =>
        test.assert_throws(lambda() =>
            divide(10, 0)
        )
    )
end)

-- Async tests
test.it("should fetch data", async lambda() =>
    optional response = await http.get("https://api.example.com")
    test.assert_true(response.is_some())
)

-- Mock
mock_db = test.mock_create()
test.mock_expect(mock_db, "query", lambda() => [{"id": 1}])

-- Run tests
test.run()
```

**Assertions:**
```mlp
test.assert_equal(actual, expected)
test.assert_not_equal(actual, expected)
test.assert_true(value)
test.assert_false(value)
test.assert_nil(value)
test.assert_throws(lambda)
test.assert_contains(list, item)
```

**Output:**
```
Running tests...

Calculator
  ✓ should add numbers (2ms)
  ✓ should subtract numbers (1ms)
  ✓ should throw on divide by zero (1ms)

Database
  ✓ should fetch users (15ms)
  ✗ should handle errors (FAILED)
    Expected: error, Got: nil

Tests: 4 passed, 1 failed, 5 total
Time: 0.25s
```

**Dosyalar:**
```
stdlib/test/
├── test.mlp          (~500 satır)
├── assert.mlp        (~300 satır)
└── mock.mlp          (~400 satır)

MELP/runtime/test/
└── test_runner.c     (~400 satır)
```

**Test:**
```bash
# Run test suite
./mlp-gcc tests/test/calculator_test.mlp -o calc_test
./calc_test
# Expected: Test report
```

---

### **Task 5: Documentation Generator** (4-5 gün)

**Hedef:** API docs generator (rustdoc/javadoc style)

**Doc Comments:**
```mlp
---
Adds two numbers together.

@param a First number
@param b Second number
@return Sum of a and b
@example
  numeric result = add(2, 3)
  yazdir(string(result))  # Prints: 5
---
function add(numeric a; numeric b) as numeric
    return a + b
end_function

---
HTTP client library for making requests.

@module http
@since 1.0.0
---
```

**Generator:**
```bash
# Generate docs
mlp-doc src/ --output docs/

# Output: HTML docs
docs/
├── index.html
├── modules/
│   ├── http.html
│   └── json.html
└── functions/
    └── add.html
```

**HTML Output:**
```html
<h1>add</h1>
<p>Adds two numbers together.</p>

<h2>Parameters</h2>
<ul>
  <li><code>a: numeric</code> - First number</li>
  <li><code>b: numeric</code> - Second number</li>
</ul>

<h2>Returns</h2>
<p><code>numeric</code> - Sum of a and b</p>

<h2>Example</h2>
<pre><code class="mlp">
numeric result = add(2, 3)
yazdir(string(result))  # Prints: 5
</code></pre>
```

**Dosyalar:**
```
MELP/C/tools/mlp-doc/
├── parser.c          (~400 satır)
├── generator.c       (~500 satır)
└── templates/        (HTML templates)
```

**Test:**
```bash
# Generate docs test
mlp-doc stdlib/ --output /tmp/docs
ls /tmp/docs/index.html
# Expected: Docs generated
```

---

### **Task 6: CI/CD Templates** (3 gün)

**Hedef:** GitHub Actions, GitLab CI

**GitHub Actions:**
```yaml
# .github/workflows/mlp.yml
name: MLP CI

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      
      - name: Setup MLP
        uses: mlp-lang/setup-mlp@v1
        with:
          mlp-version: '1.0.0'
      
      - name: Install dependencies
        run: mlp-pkg install
      
      - name: Build
        run: mlp-pkg build
      
      - name: Test
        run: mlp-pkg test
      
      - name: Lint
        run: mlp-fmt --check src/
```

**Templates:**
```
templates/ci/
├── github-actions.yml
├── gitlab-ci.yml
├── circleci.yml
└── travis.yml
```

**Test:**
```bash
# Initialize CI
mlp-pkg init
mlp-pkg ci --github
# Expected: .github/workflows/mlp.yml created
```

---

### **Task 7: Additional Ecosystem Tools** (5-6 gün)

**Tools:**
```
1. mlp-repl       # Interactive REPL
2. mlp-playground # Web-based playground
3. mlp-migrate    # Database migrations
4. mlp-deploy     # Deployment tool (Docker, K8s)
```

**REPL:**
```bash
$ mlp-repl
MLP 1.0.0 REPL
>>> numeric x = 42
>>> yazdir(string(x))
42
>>> import list from "stdlib/list.mlp"
>>> list numbers = [1,2,3]
>>> yazdir(string(list.length(numbers)))
3
```

**Playground:**
```
https://play.mlp-lang.org

[Editor]        [Output]
function main() | Hello, World!
  yazdir("test")|
end_function    |

[Share] [Run]
```

---

## 🔗 BAĞIMLILIKLAR

### Önkoşul:
- **0-6 TODO'lar** (%100)

### Bu Son TODO!
- **MLP-GCC FULL PROFESSIONAL! 🎉**

---

## 🧪 TEST STRATEJİSİ

```bash
tests/ecosystem/
├── gui/
│   ├── hello_window.mlp
│   └── todo_app.mlp
├── db/
│   ├── postgres_crud.mlp
│   └── sqlite_test.mlp
├── web/
│   ├── rest_api.mlp
│   └── websocket.mlp
├── test/
│   └── calculator_test.mlp
└── docs/
    └── doc_gen_test.sh
```

---

## 📦 DELIVERABLES

```
stdlib/gui/              (~1650 satır MLP)
stdlib/db/               (~1150 satır MLP)
stdlib/web/              (~1650 satır MLP)
stdlib/test/             (~1200 satır MLP)

MELP/runtime/gui/        (~2400 satır C)
MELP/runtime/db/         (~1700 satır C)
MELP/runtime/web/        (~1700 satır C)

MELP/C/tools/mlp-doc/    (~900 satır C)
MELP/C/tools/mlp-repl/   (~600 satır C)

templates/ci/            (4 CI templates)
```

---

## 🎯 BAŞARI KRİTERLERİ

### Final Checklist

- [ ] GUI app çalışıyor (cross-platform)
- [ ] PostgreSQL/SQLite bağlanıyor
- [ ] Web framework HTTP server
- [ ] Test framework assertions
- [ ] Docs generator HTML çıktısı
- [ ] CI/CD GitHub Actions

### 🎊 **FULL PROFESSIONAL LANGUAGE!** 🎊

```
MLP-GCC v1.0.0
├── ✅ Self-hosting
├── ✅ Modern syntax (generics, pattern matching)
├── ✅ Rich stdlib (networking, threading, collections)
├── ✅ IDE support (VS Code, Vim)
├── ✅ Package manager (mlp-pkg)
├── ✅ Optimized (C-level performance)
└── ✅ Full ecosystem (GUI, DB, Web, Test)

🎉 READY FOR PRODUCTION! 🎉
```

---

**Son Güncelleme:** 29 Aralık 2025  
**Hazırlayan:** PD_01 (Danışman)

---

## 🚀 SON SÖZ

**Tebrikler!** 7 TODO tamamlandığında MLP-GCC:

- Python kadar **kolay**
- Go kadar **hızlı**
- Rust kadar **güvenli**
- Modern dil özellikleri
- Zengin ekosistem

**27 hafta (6-7 ay)** sonra **profesyonel bir programlama dili!** 🎉

**Başarılar! 🚀**

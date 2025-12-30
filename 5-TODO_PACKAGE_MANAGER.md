# 5-TODO_PACKAGE_MANAGER.md

**Hedef:** Paket Yönetimi Ekosistemi (npm/cargo style)  
**Süre:** 3 hafta (Hafta 19-21)  
**Öncelik:** Kritik  
**Bağımlılık:** 0-4 TODO'lar (%100)

---

## 📋 GENEL BAKIŞ

MLP-GCC'de **paket sistemi yok:**
- ❌ Bağımlılık yönetimi yok
- ❌ Package registry yok
- ❌ Versioning yok
- ❌ Publish/install mekanizması yok

Bu TODO **npm/cargo seviyesinde** paket sistemi kuracak.

---

## 🎯 TODO HEDEFLERİ

### Başarı Kriterleri

- ✅ mlp-pkg CLI (install, update, publish)
- ✅ mlp.toml manifest (bağımlılıklar)
- ✅ Package registry (online/offline)
- ✅ Semantic versioning
- ✅ Lock files (reproducible builds)
- ✅ Binary cache (hızlı installasyon)

---

## 📊 TASK BREAKDOWN

### **Task 1: Package Manifest (mlp.toml)** (3 gün)

**Hedef:** Proje tanımlama dosyası

**mlp.toml:**
```toml
[package]
name = "my-app"
version = "1.0.0"
authors = ["Ali Yılmaz <ali@example.com>"]
license = "MIT"
description = "My MLP application"
homepage = "https://github.com/user/my-app"
repository = "https://github.com/user/my-app"
keywords = ["cli", "tool"]

[dependencies]
http-client = "2.1.0"
json-parser = "1.5.3"
logger = { version = "0.9", features = ["color"] }

[dev-dependencies]
test-framework = "3.0.0"

[build]
entry = "src/main.mlp"
output = "bin/my-app"

[features]
default = ["ssl"]
ssl = []
debug = []
```

**Parser:**
```
MELP/C/tools/mlp-pkg/
└── toml_parser.c  (~400 satır)  # TOML parser
```

**STATELESS Cache Pattern:**
```mlp
-- ⚠️ Package cache: Caller-owned ("her modül ölüdür" felsefesi)
struct PackageCache
    string cache_dir        # ~/.mlp/cache
    hashmap packages        # name -> Package
end_struct

function cache_create(string dir) -> PackageCache
    # 🟢 Caller diriltir
    return PackageCache {
        cache_dir: dir,
        packages: hashmap.create()
    }
end_function

function cache_get(PackageCache cache, string name) -> optional Package
    return hashmap.get(cache.packages, name)
end_function

function cache_put(PackageCache cache, Package pkg)
    hashmap.insert(cache.packages, pkg.name, pkg)
    # Disk'e yaz: cache.cache_dir + "/" + pkg.name
end_function

function cache_destroy(PackageCache cache)
    # 🔴 Caller öldürür (RAII)
    hashmap.destroy(cache.packages)
end_function

-- mlp-pkg CLI internally (main function):
PackageCache cache = cache_create("~/.mlp/cache")
optional pkg = cache_get(cache, "json-parser")
if pkg.is_none() then
    pkg = registry_download("json-parser")
    cache_put(cache, pkg.unwrap())
end_if
# ... use package ...
cache_destroy(cache)  # CLI exit'te cleanup
```

**Test:**
```bash
# Manifest validation
./mlp-pkg check mlp.toml
# Expected: "✓ Valid manifest"

# Dependency tree
./mlp-pkg deps --tree
```

---

### **Task 2: mlp-pkg CLI** (6-7 gün)

**Hedef:** Package manager CLI

**Commands:**
```bash
# Init new project
mlp-pkg init my-app
cd my-app

# Add dependency
mlp-pkg add http-client@2.1.0

# Install dependencies
mlp-pkg install

# Update dependencies
mlp-pkg update

# Build project
mlp-pkg build

# Run project
mlp-pkg run

# Test project
mlp-pkg test

# Publish package
mlp-pkg publish

# Search packages
mlp-pkg search json

# Show package info
mlp-pkg info json-parser
```

**Dosyalar:**
```
MELP/C/tools/mlp-pkg/
├── main.c              (~300 satır)  # CLI entry
├── commands/
│   ├── init.c          (~200 satır)
│   ├── add.c           (~250 satır)
│   ├── install.c       (~400 satır)
│   ├── update.c        (~300 satır)
│   ├── build.c         (~200 satır)
│   ├── run.c           (~150 satır)
│   ├── test.c          (~200 satır)
│   ├── publish.c       (~350 satır)
│   └── search.c        (~250 satır)
├── resolver/
│   └── dependency.c    (~500 satır)  # Dependency resolution
└── registry/
    └── client.c        (~400 satır)  # Registry HTTP client
```

**Test:**
```bash
# Full workflow test
mkdir test-project && cd test-project
mlp-pkg init
mlp-pkg add json-parser
mlp-pkg build
./bin/test-project
```

**⚠️ STATELESS Package Cache Pattern:**
```mlp
-- Package cache: Caller-owned ("her modül ölüdür" felsefesi)
struct PackageCache
    string cache_dir        # ~/.mlp/cache
    hashmap packages        # name -> Package metadata
end_struct

function cache_create(string dir) -> PackageCache
    # 🟢 Caller diriltir (mlp-pkg CLI main'de)
    return PackageCache {
        cache_dir: dir,
        packages: hashmap.create()
    }
end_function

function cache_get(PackageCache cache, string name, string version) -> optional Package
    string key = name + "@" + version
    return hashmap.get(cache.packages, key)
end_function

function cache_put(PackageCache cache, Package pkg)
    string key = pkg.name + "@" + pkg.version
    hashmap.insert(cache.packages, key, pkg)
    # Disk'e persiste: cache.cache_dir + "/" + key + ".tar.gz"
    file.write(cache.cache_dir + "/" + key + ".tar.gz", pkg.tarball)
end_function

function cache_clean(PackageCache cache)
    # mlp-pkg cache-clean komutu
    hashmap.clear(cache.packages)
    os.remove_dir(cache.cache_dir)  # Disk'ten sil
end_function

function cache_destroy(PackageCache cache)
    # 🔴 Caller öldürür (mlp-pkg CLI exit'te RAII)
    hashmap.destroy(cache.packages)
end_function

-- mlp-pkg CLI internally (main function):
PackageCache cache = cache_create("~/.mlp/cache")  # Diriltme

# Check cache first
optional pkg = cache_get(cache, "json-parser", "1.5.3")
if pkg.is_none() then
    # Download from registry
    pkg = registry_download("json-parser", "1.5.3")
    cache_put(cache, pkg.unwrap())  # Cache'e ekle
end_if

# Use package...

cache_destroy(cache)  # Öldürme (CLI exit RAII)
```

---

### **Task 3: Package Registry** (7-8 gün)

**Hedef:** Online package repository

**⚠️ IMPORTANT:** Registry Server = **TOOL (Binary)**, MLP modülü DEĞİL!  
**Neden:** Long-running web server = 6. TEMEL ESAS'a aykırı (MODÜL=ŞABLON ihlali)

**Architecture:**
```
Registry Server (TOOL - Rust/Go ile yazılır)
├── API (REST)
│   ├── GET  /api/v1/packages/:name
│   ├── POST /api/v1/packages/:name
│   ├── GET  /api/v1/search?q=:query
│   └── GET  /api/v1/download/:name/:version
├── Storage
│   ├── PostgreSQL (metadata)
│   └── S3/MinIO (package tarballs)
└── Web UI
    ├── Package browser
    ├── Documentation viewer
    └── User dashboard
```

**Çalıştırma:**
```bash
# Registry server = external tool
./mlp-registry-server --port 8080 --db postgres://...
```

**mlp-pkg ile entegrasyon (MLP modülü):**
```mlp
-- ✅ DOĞRU: Her komut independent (MODÜL=ŞABLON)
result = registry.search("json")  -- HTTP GET → parse → öldü
info = registry.get_package("json-parser")  -- HTTP GET → öldü
```

**API Endpoints:**
```bash
# Get package info
curl https://registry.mlp-lang.org/api/v1/packages/json-parser
{
  "name": "json-parser",
  "version": "1.5.3",
  "description": "Fast JSON parser",
  "downloads": 125000,
  "created_at": "2025-01-15T10:30:00Z"
}

# Search packages
curl https://registry.mlp-lang.org/api/v1/search?q=json
{
  "results": [
    {"name": "json-parser", "version": "1.5.3"},
    {"name": "json-schema", "version": "2.0.1"}
  ]
}

# Download package
curl https://registry.mlp-lang.org/api/v1/download/json-parser/1.5.3 > json-parser-1.5.3.tar.gz
```

**Database Schema:**
```sql
CREATE TABLE packages (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) UNIQUE NOT NULL,
    description TEXT,
    homepage VARCHAR(500),
    repository VARCHAR(500),
    created_at TIMESTAMP DEFAULT NOW()
);

CREATE TABLE versions (
    id SERIAL PRIMARY KEY,
    package_id INTEGER REFERENCES packages(id),
    version VARCHAR(50) NOT NULL,
    tarball_url VARCHAR(500) NOT NULL,
    checksum VARCHAR(64) NOT NULL,
    downloads INTEGER DEFAULT 0,
    created_at TIMESTAMP DEFAULT NOW(),
    UNIQUE(package_id, version)
);

CREATE TABLE dependencies (
    version_id INTEGER REFERENCES versions(id),
    dependency_name VARCHAR(255) NOT NULL,
    version_req VARCHAR(100) NOT NULL
);
```

**Test:**
```bash
# Registry health check
curl https://registry.mlp-lang.org/health
# Expected: {"status": "ok"}

# Publish test package
mlp-pkg publish --dry-run
```

---

### **Task 4: Dependency Resolution** (5-6 gün)

**Hedef:** SAT solver for dependency resolution

**Algorithm:**
```
Input: mlp.toml dependencies
Output: Resolved dependency tree (lock file)

1. Parse mlp.toml
2. Fetch package metadata from registry
3. Build dependency graph
4. Resolve version conflicts (SAT solver)
5. Download packages
6. Verify checksums
7. Extract to ~/.mlp/packages/
8. Generate mlp.lock
```

**Version Constraints:**
```toml
# Exact version
json-parser = "1.5.3"

# Semver range
http-client = "^2.1.0"  # >=2.1.0 <3.0.0
logger = "~0.9.5"       # >=0.9.5 <0.10.0

# Wildcard
test-utils = "1.*"      # >=1.0.0 <2.0.0

# Git dependency
my-lib = { git = "https://github.com/user/my-lib", tag = "v1.0" }
```

**Conflict Resolution:**
```
App depends on:
  - lib-a@1.0 (depends on common@1.0)
  - lib-b@2.0 (depends on common@2.0)

Error: Cannot resolve common (1.0 vs 2.0)
Solution: Upgrade lib-a to 1.1 (supports common@2.0)
```

**Test:**
```bash
# Conflict test
mlp-pkg add lib-a lib-b
# Expected: Resolved or error with suggestion
```

---

### **Task 5: Lock Files & Reproducible Builds** (3 gün)

**Hedef:** Deterministic builds

**mlp.lock:**
```toml
# This file is auto-generated. Do not edit manually.

[[package]]
name = "json-parser"
version = "1.5.3"
checksum = "sha256:a3f5b..."
dependencies = []

[[package]]
name = "http-client"
version = "2.1.0"
checksum = "sha256:b7c9e..."
dependencies = [
    "json-parser 1.5.3"
]

[[package]]
name = "logger"
version = "0.9.5"
checksum = "sha256:c8d2a..."
dependencies = []
```

**Reproducibility:**
```bash
# Machine A
mlp-pkg install
mlp-pkg build
sha256sum bin/my-app

# Machine B (different OS)
mlp-pkg install  # Uses mlp.lock → same versions
mlp-pkg build
sha256sum bin/my-app
# Same checksum! (reproducible)
```

**Test:**
```bash
# Reproducibility test
./test_reproducible.sh
# Expected: Same binary hash on different machines
```

---

### **Task 6: Binary Cache & Performance** (4 gün)

**Hedef:** Fast package installation

**Features:**
```bash
# Binary cache (pre-compiled packages)
mlp-pkg install json-parser  # Downloads binary (not source)
# Fallback: Compile from source if no binary

# Parallel downloads (10 packages simultaneously)
mlp-pkg install  # Fast!

# Incremental updates (only changed packages)
mlp-pkg update

# Offline mode (use local cache)
mlp-pkg install --offline
```

**Cache Structure:**
```
~/.mlp/
├── packages/          # Source packages
│   └── json-parser/
│       └── 1.5.3/
├── binaries/          # Compiled binaries
│   └── json-parser/
│       └── 1.5.3/
│           └── linux-x64/
└── cache/
    └── registry.json  # Registry metadata cache
```

**Performance Targets:**
```
Install 10 packages: <30s
Update check: <5s
Offline install: <10s
Search: <1s
```

**Test:**
```bash
# Performance test
time mlp-pkg install --clean-cache
# Expected: <30s for 10 packages
```

---

## 🔗 BAĞIMLILIKLAR

### Önkoşul:
- **0-TODO_SELFHOSTING.md** (%100)
- **2-TODO_STDLIB_EXPAND.md** (%100) - HTTP client için

### Sonraki TODO:
- **7-TODO_ECOSYSTEM.md** - Packages registry'de paylaşılacak

---

## 🧪 TEST STRATEJİSİ

```bash
tests/pkg/
├── manifest_test.sh
├── install_test.sh
├── resolution_test.sh
├── lock_test.sh
├── cache_test.sh
└── registry_test.sh
```

---

## 📦 DELIVERABLES

```
MELP/C/tools/mlp-pkg/    # Package manager CLI (~3650 satır)
registry-server/          # Registry server (Rust/Go)
registry-web/             # Web UI (React/Vue)
```

---

## 🎯 BAŞARI KRİTERLERİ

- [ ] mlp-pkg init/add/install working
- [ ] Registry deployed (registry.mlp-lang.org)
- [ ] 50+ packages published
- [ ] Lock files reproducible
- [ ] Binary cache <30s install

---

**Son Güncelleme:** 29 Aralık 2025  
**Hazırlayan:** PD_01 (Danışman)

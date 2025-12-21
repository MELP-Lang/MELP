# YZ_205: Package Structure System

**Başlangıç:** 21 Aralık 2025  
**Durum:** 🚧 IN PROGRESS  
**Branch:** `package-structure_YZ_205`  
**Öncelik:** 🟡 Orta (project organization)

---

## 🎯 HEDEF

`package.mlp` manifest sistemi ile proje yapısı ve dependency management.

**Core Features:**
1. Package manifest parser (`package.mlp`)
2. Metadata fields (name, version, author)
3. Dependency list parsing
4. Entry point specification
5. Build configuration
6. Package verification

---

## 📋 GÖREV LİSTESİ

### Phase 1: Package Manifest Parser (P1) [2 gün]

**1.1 Package Parser Infrastructure**
- [ ] Create `compiler/stage0/modules/package/` directory
- [ ] `package_parser.h` - Interface definitions
- [ ] `package_parser.c` - Main parsing logic
- [ ] Package manifest tokenization
- [ ] Field value extraction

**1.2 Metadata Fields**
- [ ] `name` field parsing (string)
- [ ] `version` field parsing (semantic version)
- [ ] `author` field parsing (string)
- [ ] `entry` field parsing (file path)
- [ ] Field validation (required vs optional)

**1.3 Syntax Support**
```pmpl
package {
    name = "my_project",
    version = "1.0.0",
    author = "Developer",
    entry = "src/main.mlp"
}
```

**Dosyalar:**
- `compiler/stage0/modules/package/package_parser.c`
- `compiler/stage0/modules/package/package_parser.h`
- `compiler/stage0/modules/package/package_types.h`

---

### Phase 2: Dependency Resolution (P1) [2 gün]

**2.1 Dependency Parser**
- [ ] Parse dependency list: `dependencies = ["stdlib:1.0", "math:2.0"]`
- [ ] Dependency name extraction
- [ ] Version requirement parsing
- [ ] Array syntax support

**2.2 Dependency Graph**
- [ ] Build dependency graph structure
- [ ] Dependency loading order
- [ ] Circular dependency detection
- [ ] Missing dependency reporting

**Syntax:**
```pmpl
package {
    name = "my_app",
    dependencies = ["stdlib:1.0", "math:2.0"]
}
```

**Dosyalar:**
- `compiler/stage0/modules/package/dependency_resolver.c`
- `compiler/stage0/modules/package/dependency_resolver.h`
- `compiler/stage0/modules/package/dependency_graph.c`

---

### Phase 3: Build System Integration (P2) [1 gün]

**3.1 Build Configuration**
- [ ] Output directory structure: `build/`, `dist/`
- [ ] Package output path
- [ ] Intermediate files location
- [ ] Build artifact organization

**3.2 Package Verification**
- [ ] Manifest validation (all required fields present)
- [ ] Entry point file exists check
- [ ] Dependency availability check
- [ ] Version compatibility check

**Dosyalar:**
- `compiler/stage0/modules/package/package_builder.c`
- `compiler/stage0/modules/package/package_validator.c`

---

## 🧪 TEST CASES

### Test 1: Basic Package Manifest
```pmpl
-- tests/package/test_basic_package.mlp
package {
    name = "test_project",
    version = "1.0.0",
    author = "Test Developer",
    entry = "main.mlp"
}
```

**Expected:**
- Package name extracted: "test_project"
- Version parsed: "1.0.0"
- Author parsed: "Test Developer"
- Entry point: "main.mlp"

---

### Test 2: Package with Dependencies
```pmpl
-- tests/package/test_dependencies.mlp
package {
    name = "app_with_deps",
    version = "2.0.0",
    dependencies = ["stdlib:1.0", "math:1.5"]
}
```

**Expected:**
- 2 dependencies identified
- Dependency 1: "stdlib" version "1.0"
- Dependency 2: "math" version "1.5"

---

### Test 3: Minimal Package
```pmpl
-- tests/package/test_minimal.mlp
package {
    name = "minimal",
    version = "0.1.0"
}
```

**Expected:**
- Parse succeeds with only required fields
- Optional fields have defaults

---

### Test 4: Invalid Package (Error Case)
```pmpl
-- tests/package/test_invalid.mlp
package {
    version = "1.0.0"
    -- Missing required 'name' field
}
```

**Expected:**
- Error: "Missing required field 'name' in package manifest"

---

### Test 5: Circular Dependencies (Error Case)
```pmpl
-- tests/package/test_circular.mlp
-- Package A depends on B, B depends on A
package {
    name = "package_a",
    dependencies = ["package_b:1.0"]
}
```

**Expected:**
- Error: "Circular dependency detected: package_a -> package_b -> package_a"

---

## 📁 DOSYA YAPISI

```
compiler/stage0/modules/package/
├── package_parser.h           # Package manifest parser interface
├── package_parser.c           # Manifest parsing implementation
├── package_types.h            # Package data structures
├── dependency_resolver.h      # Dependency resolution interface
├── dependency_resolver.c      # Dependency graph builder
├── dependency_graph.c         # Graph algorithms (circular detection)
├── package_builder.h          # Build system integration
├── package_builder.c          # Build configuration
├── package_validator.h        # Validation rules
├── package_validator.c        # Manifest validation
└── Makefile                   # Build configuration

tests/package/
├── test_basic_package.mlp     # Basic manifest test
├── test_dependencies.mlp      # Dependencies test
├── test_minimal.mlp           # Minimal package test
├── test_invalid.mlp           # Error handling test
└── test_circular.mlp          # Circular dependency test
```

---

## 🔧 TEKNİK DETAYLAR

### Package Data Structure
```c
typedef struct Package {
    char* name;              // Required: package name
    char* version;           // Required: semantic version
    char* author;            // Optional: author name
    char* entry;             // Optional: entry point file
    char** dependencies;     // Optional: dependency list
    int dependency_count;
} Package;
```

### Dependency Structure
```c
typedef struct Dependency {
    char* name;              // Package name
    char* version_required;  // Required version
    Package* resolved;       // Resolved package (NULL if not loaded)
} Dependency;
```

---

## ⚠️ MELP PRENSİPLERİ

### 1. Keyword Style
- ✅ `package` keyword (MELP-style)
- ✅ Block syntax: `package { ... }`
- ✅ Field assignment: `name = "value"`

### 2. Integration Points
- ✅ Lexer: Add `package`, `dependencies` keywords
- ✅ Parser: Handle block-level package declaration
- ✅ Import system: Use dependency resolution for imports

### 3. Error Handling
- 🚨 Missing required field → Clear error message
- 🚨 Circular dependency → Detailed cycle path
- 🚨 Missing dependency → List unavailable packages
- 🚨 Invalid version format → Suggest correct format

---

## 📊 İLERLEME TAKİBİ

### Phase 1: Package Parser ⏳
- [ ] Package parser infrastructure
- [ ] Metadata field parsing
- [ ] Syntax support
- [ ] Test: Basic package manifest

### Phase 2: Dependency Resolution ⏳
- [ ] Dependency parser
- [ ] Dependency graph
- [ ] Circular detection
- [ ] Test: Dependencies

### Phase 3: Build Integration ⏳
- [ ] Build configuration
- [ ] Package verification
- [ ] Test: Validation

---

## 🎯 TAMAMLANMA KRİTERLERİ

### Minimum Success Criteria (MVP):
1. ✅ Parse basic `package.mlp` manifest
2. ✅ Extract name, version fields
3. ✅ Parse dependency list
4. ✅ Detect circular dependencies
5. ✅ Pass all 5 test cases

### Full Success Criteria:
1. ✅ Complete package metadata support
2. ✅ Dependency graph building
3. ✅ Build system integration
4. ✅ Comprehensive error messages
5. ✅ Documentation complete

---

## 📚 REFERANSLAR

- **TODO:** `TODO_MODERN_LANGUAGE.md` Lines 337-356
- **Module Docs:** `melp_modulleri.md`
- **Previous Task:** `YZ_204_TAMAMLANDI.md` (Module system)
- **Next Task:** `YZ_206` (Result Type Pattern)

---

## 🚀 BAŞLANGIÇ KOMUTLARI

```bash
# 1. Create directory structure
mkdir -p compiler/stage0/modules/package
mkdir -p tests/package

# 2. Create test files
touch tests/package/test_basic_package.mlp
touch tests/package/test_dependencies.mlp

# 3. Create module files
touch compiler/stage0/modules/package/package_parser.c
touch compiler/stage0/modules/package/package_parser.h
touch compiler/stage0/modules/package/dependency_resolver.c

# 4. Start with Phase 1
cd compiler/stage0/modules/package
```

---

**Hazır mısın? İşe koyulalım! 🚀**

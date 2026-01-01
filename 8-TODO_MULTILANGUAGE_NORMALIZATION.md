# 8-TODO_MULTILANGUAGE_NORMALIZATION.md

**Hedef:** Multi-Language Support & Normalization Pipeline  
**Süre:** 3 hafta (Hafta 29-31)  
**Öncelik:** Orta  
**Bağımlılık:** 0-7 TODO'lar (%100)

---

## 📋 GENEL BAKIŞ

TODO #7 sonrası MLP-GCC **production-ready ekosistem** ama tek dil desteği:
- ❌ Sadece Turkish syntax (veya English)
- ❌ Multi-language mixing yok
- ❌ Normalizer sadece skeleton (identity function)
- ❌ Language-agnostic AST yok

Bu TODO **full multi-language support** ve **normalization pipeline** ekleyecek! 🌍

---

## 🎯 TODO HEDEFLERİ

### Başarı Kriterleri

**TODO tamamlandığında:**
- ✅ Turkish syntax (MLP-TR) tam destek
- ✅ English syntax (MLP-EN) tam destek
- ✅ Pure Mathematical notation (PMLP) internal format
- ✅ Auto-detection (syntax variant detection)
- ✅ Normalizer: MLP-TR/EN → PMLP → Compiler
- ✅ Cross-language imports (TR file ↔ EN file)
- ✅ Multi-language stdlib (tr/, en/, pmlp/ variants)

**🌍 TRULY MULTI-LINGUAL LANGUAGE! 🌍**

---

## 📊 TASK BREAKDOWN

### **Task 1: PMLP (Pure Mathematical MLP) Specification** (3-4 gün)

**Atanan:** NORM_YZ_01  
**Hedef:** Language-agnostic internal representation

**PMLP Design:**
```pmlp
# Pure Mathematical MLP - Compiler internal format
# No natural language keywords, pure symbols

∀ add(a: ℕ, b: ℕ) → ℕ {
    ↩ a + b
}

∀ factorial(n: ℕ) → ℕ {
    ⊢ n ≤ 1 → {
        ↩ 1
    } ⊣ {
        ↩ n × factorial(n - 1)
    }
}

# Types: ℕ (numeric), 𝕊 (string), 𝔹 (bool), 𝕃 (list)
# Function: ∀
# Return: ↩
# If: ⊢
# Else: ⊣
# Block: { }
```

**Symbol Mapping:**
```
TR/EN          → PMLP Symbol
---------------------------------
function       → ∀
return         → ↩
if/then        → ⊢
else           → ⊣
end_*          → (implicit closing })
numeric        → ℕ
string         → 𝕊
bool           → 𝔹
list           → 𝕃
```

**PMLP Grammar (EBNF):**
```ebnf
<program>    ::= <statement>*
<statement>  ::= <function> | <variable> | <if> | <while> | <return>
<function>   ::= '∀' <id> '(' <params> ')' '→' <type> '{' <block> '}'
<if>         ::= '⊢' <expr> '→' '{' <block> '}' ('⊣' '{' <block> '}')?
<return>     ::= '↩' <expr>
<type>       ::= 'ℕ' | '𝕊' | '𝔹' | '𝕃' '<' <type> '>'
```

**Dosyalar:**
```
docs/spec/
├── PMLP_SPEC.md           (~500 lines) # Full specification
└── pmlp_symbols.md        (~200 lines) # Symbol reference

MELP/normalizer/
├── pmlp_spec.h            (~150 lines) # C header
└── pmlp_symbols.h         (~100 lines)
```

---

### **Task 2: Normalizer Implementation** (6-7 gün)

**Atanan:** NORM_YZ_02  
**Hedef:** MLP-TR/EN → PMLP transformer

**Current Status (Identity):**
```c
// MELP/normalizer/normalizer.c
// Currently: Just copy input → output
```

**Target Implementation:**
```c
// MELP/normalizer/normalizer.c
typedef enum {
    LANG_TR,    // Turkish syntax
    LANG_EN,    // English syntax
    LANG_PMLP   // Pure mathematical
} LanguageVariant;

// Auto-detect language from keywords
LanguageVariant detect_language(const char* source) {
    if (strstr(source, "fonksiyon") || strstr(source, "değişken"))
        return LANG_TR;
    if (strstr(source, "function") || strstr(source, "variable"))
        return LANG_EN;
    if (strstr(source, "∀") || strstr(source, "↩"))
        return LANG_PMLP;
    return LANG_TR; // Default
}

// Normalize: TR/EN → PMLP
char* normalize_to_pmlp(const char* source, LanguageVariant lang) {
    Lexer* lexer = lexer_create(source, lang);
    Parser* parser = parser_create(lexer);
    ASTNode* ast = parser_parse(parser);
    
    PMPLGenerator* gen = pmlp_generator_create();
    char* pmlp = pmlp_generator_emit(gen, ast);
    
    return pmlp;
}
```

**Keyword Mapping Tables:**
```c
// TR → PMLP
static const KeywordMap tr_to_pmlp[] = {
    {"fonksiyon",   "∀"},
    {"dön",         "↩"},
    {"eğer",        "⊢"},
    {"değilse",     "⊣"},
    {"sayısal",     "ℕ"},
    {"metin",       "𝕊"},
    {"mantıksal",   "𝔹"},
    {"liste",       "𝕃"},
    // ...
};

// EN → PMLP
static const KeywordMap en_to_pmlp[] = {
    {"function",    "∀"},
    {"return",      "↩"},
    {"if",          "⊢"},
    {"else",        "⊣"},
    {"numeric",     "ℕ"},
    {"string",      "𝕊"},
    {"bool",        "𝔹"},
    {"list",        "𝕃"},
    // ...
};
```

**Dosyalar:**
```
MELP/normalizer/
├── normalizer.c          (rewrite ~600 lines)
├── language_detect.c     (new ~250 lines)
├── tr_to_pmlp.c         (new ~400 lines)
├── en_to_pmlp.c         (new ~400 lines)
├── pmlp_generator.c     (new ~500 lines)
└── keyword_maps.c       (new ~300 lines)
```

**Pipeline:**
```
Input (TR/EN) → Language Detection → Lexer (lang-aware) 
              → Parser → AST → PMLP Generator → PMLP Output
```

**Test:**
```bash
# TR → PMLP
./normalizer test_tr.mlp test.pmlp
cat test.pmlp  # Should show ∀, ↩, etc.

# EN → PMLP
./normalizer test_en.mlp test.pmlp
cat test.pmlp  # Same PMLP output!

# Auto-detect
./normalizer mixed.mlp output.pmlp
# Should auto-detect and normalize
```

---

### **Task 3: Compiler PMLP Integration** (4-5 gün)

**Atanan:** NORM_YZ_03  
**Hedef:** Compiler PMLP'yi input olarak almalı

**Current Compiler Pipeline:**
```
MLP source → Lexer → Parser → AST → Codegen → C code
```

**New Pipeline:**
```
MLP-TR/EN → Normalizer → PMLP → Compiler → C code
                                    ↓
                            (PMLP lexer/parser)
```

**Changes:**
```c
// MELP/C/stage0/main.c
int main(int argc, char** argv) {
    // ...
    
    // 1. Normalize to PMLP (if not already PMLP)
    char* pmlp_source = normalize_input(input_file);
    
    // 2. Compile PMLP
    Lexer* lexer = pmlp_lexer_create(pmlp_source);
    Parser* parser = pmlp_parser_create(lexer);
    // ...
}
```

**PMLP Lexer:**
```c
// MELP/C/stage0/modules/lexer/pmlp_lexer.c
Token pmlp_next_token(PMPLLexer* lexer) {
    // Tokenize PMLP symbols (∀, ↩, ⊢, ⊣, ℕ, etc.)
    switch (current_char) {
        case '∀': return TOKEN_FUNCTION;
        case '↩': return TOKEN_RETURN;
        case '⊢': return TOKEN_IF;
        case '⊣': return TOKEN_ELSE;
        // ...
    }
}
```

**Dosyalar:**
```
MELP/C/stage0/modules/lexer/
├── pmlp_lexer.c          (new ~400 lines)
└── pmlp_lexer.h          (new ~100 lines)

MELP/C/stage0/modules/parser/
├── pmlp_parser.c         (new ~500 lines)
└── pmlp_parser.h         (new ~100 lines)
```

**Test:**
```bash
# Direct PMLP compilation
./melpc test.pmlp -o test.c
gcc test.c -o test
./test  # Should work!

# TR → PMLP → C (automatic)
./melpc test_tr.mlp -o test.c  # Internally: normalize → compile
```

---

### **Task 4: Multi-Language Stdlib** (5-6 gün)

**Atanan:** NORM_YZ_04  
**Hedef:** stdlib her dilde (tr/, en/, pmlp/)

**Current Structure:**
```
stdlib/
├── io.mlp          # Turkish only
├── list.mlp
└── math.mlp
```

**New Structure:**
```
stdlib/
├── tr/             # Turkish variant
│   ├── io.mlp
│   ├── liste.mlp
│   └── matematik.mlp
├── en/             # English variant
│   ├── io.mlp
│   ├── list.mlp
│   └── math.mlp
└── pmlp/           # Pure mathematical (canonical)
    ├── ιο.pmlp     # I/O (Greek iota-omicron)
    ├── 𝕃.pmlp      # List
    └── ℳ.pmlp      # Math
```

**Example - io.mlp (Turkish):**
```mlp
# stdlib/tr/io.mlp
fonksiyon dosya_oku(metin yol) değer metin
    # ...
son_fonksiyon
```

**Example - io.mlp (English):**
```mlp
# stdlib/en/io.mlp
function read_file(string path) as string
    # ...
end_function
```

**Example - ιο.pmlp (Mathematical):**
```pmlp
# stdlib/pmlp/ιο.pmlp
∀ read_file(path: 𝕊) → 𝕊 {
    # ...
}
```

**Import Resolution:**
```mlp
# TR file
içe_aktar io "stdlib/tr/io.mlp"  # Explicit
içe_aktar io "stdlib/io.mlp"     # Auto-detect (uses TR variant)

# EN file
import io from "stdlib/en/io.mlp"  # Explicit
import io from "stdlib/io.mlp"     # Auto-detect (uses EN variant)
```

**Auto-Detection Logic:**
```c
// import_handler.c
char* resolve_stdlib_path(const char* import_path, LanguageVariant lang) {
    if (starts_with(import_path, "stdlib/")) {
        // Auto-prefix with language variant
        switch (lang) {
            case LANG_TR:   return "stdlib/tr/" + basename(import_path);
            case LANG_EN:   return "stdlib/en/" + basename(import_path);
            case LANG_PMLP: return "stdlib/pmlp/" + basename(import_path);
        }
    }
    return import_path; // User module, no prefix
}
```

**Dosyalar:**
```
stdlib/tr/       (~20 modules, 3000+ lines total)
stdlib/en/       (~20 modules, 3000+ lines total)
stdlib/pmlp/     (~20 modules, 2500+ lines total - more compact!)
```

---

### **Task 5: Cross-Language Import** (3-4 gün)

**Atanan:** NORM_YZ_05  
**Hedef:** TR file can import EN file (and vice versa)

**Scenario:**
```mlp
# utils_en.mlp (English)
function calculate(numeric x) as numeric
    return x * 2
end_function

# main_tr.mlp (Turkish)
içe_aktar utils "utils_en.mlp"  # Import EN file from TR file!

fonksiyon ana()
    sonuç = utils.calculate(21)  # Works!
    yaz(sonuç)  # 42
son_fonksiyon
```

**Implementation:**
```c
// import_handler.c
void handle_cross_language_import(ImportContext* ctx, const char* path) {
    // 1. Read imported file
    char* source = read_file(path);
    
    // 2. Detect language
    LanguageVariant imported_lang = detect_language(source);
    
    // 3. Normalize to PMLP
    char* pmlp = normalize_to_pmlp(source, imported_lang);
    
    // 4. Parse PMLP AST
    ASTNode* ast = parse_pmlp(pmlp);
    
    // 5. Add to import context (language-agnostic)
    import_context_add(ctx, ast);
}
```

**Test:**
```bash
# Cross-language test
./melpc main_tr.mlp -o main.c  # TR imports EN
gcc main.c -o main
./main  # Should work!
```

---

### **Task 6: Documentation & Migration Guide** (2-3 gün)

**Atanan:** NORM_YZ_06  
**Hedef:** Help users choose/migrate between languages

**Documents:**
```
docs/multi-language/
├── LANGUAGE_GUIDE.md          (~600 lines)
│   ├── Turkish Syntax
│   ├── English Syntax
│   ├── PMLP Notation
│   └── Comparison table
├── MIGRATION_GUIDE.md         (~400 lines)
│   ├── TR → EN migration
│   ├── EN → TR migration
│   └── Automated conversion tools
├── CROSS_LANGUAGE_IMPORTS.md  (~300 lines)
└── FAQ.md                     (~250 lines)
```

**Syntax Comparison Table:**
```markdown
| Concept       | Turkish (TR)  | English (EN) | PMLP      |
|---------------|---------------|--------------|-----------|
| Function      | fonksiyon     | function     | ∀         |
| Return        | dön           | return       | ↩         |
| If            | eğer          | if           | ⊢         |
| Else          | değilse       | else         | ⊣         |
| Numeric       | sayısal       | numeric      | ℕ         |
| String        | metin         | string       | 𝕊         |
| Bool          | mantıksal     | bool         | 𝔹         |
| List          | liste         | list         | 𝕃         |
```

**Migration Tool:**
```bash
# TR → EN converter
mlp-convert --from=tr --to=en input_tr.mlp output_en.mlp

# EN → TR converter
mlp-convert --from=en --to=tr input_en.mlp output_tr.mlp

# Any → PMLP
mlp-convert --to=pmlp input.mlp output.pmlp
```

---

## 🧪 KABUL KRİTERLERİ

### Zorunlu Testler

**Test 1: Language Detection**
```bash
./normalizer test_tr.mlp --detect
# Output: Turkish (TR)

./normalizer test_en.mlp --detect
# Output: English (EN)
```

**Test 2: TR → PMLP Normalization**
```bash
./normalizer hello_tr.mlp hello.pmlp
cat hello.pmlp  # Should show PMLP symbols (∀, ↩, etc.)
```

**Test 3: EN → PMLP Normalization**
```bash
./normalizer hello_en.mlp hello.pmlp
cat hello.pmlp  # Same PMLP as Test 2!
```

**Test 4: PMLP Compilation**
```bash
./melpc hello.pmlp -o hello.c
gcc hello.c -o hello
./hello  # Runs correctly
```

**Test 5: Cross-Language Import**
```bash
# utils_en.mlp (English)
# main_tr.mlp (Turkish imports utils_en.mlp)
./melpc main_tr.mlp -o main.c
gcc main.c -o main
./main  # Works!
```

**Test 6: Multi-Language Stdlib**
```bash
# TR file using TR stdlib
./melpc test_tr.mlp -o test.c  # Uses stdlib/tr/

# EN file using EN stdlib  
./melpc test_en.mlp -o test.c  # Uses stdlib/en/
```

---

## 📈 PERFORMANS ETKI ANALİZİ

**Compile Time Impact:**
```
Without normalization: 100ms (baseline)
With normalization:     120ms (+20ms overhead)
→ 20% slower but acceptable for multi-language support
```

**Optimizations:**
- [ ] Cache normalized PMLP (`.pmlp.cache`)
- [ ] Parallel normalization (multi-file projects)
- [ ] JIT normalization (in-memory, no disk I/O)

---

## 🔗 BAĞIMLILIKLAR

### Önkoşul:
- **0-TODO_SELFHOSTING.md** (%100) - Compiler working
- **1-TODO_TOOLING_BASICS.md** (%100) - LSP support
- **7-TODO_ECOSYSTEM.md** (%100) - Full ecosystem

### Sonraki TODO:
- **TODO #9+** - Advanced features (concurrency, ownership)

---

## 🧪 GERÇEK TESTLER - TODO TAMAMLANMA KRİTERLERİ

### ⚠️ UYARI: Tüm testler geçmeden TODO tamamlanmış sayılmaz!

**Test Dosyaları:** `tests/normalization/`

```bash
# Test 1-5: PMLP Specification Tests
tests/normalization/pmlp/
├── test_symbols.mlp            # Mathematical symbols ✅
├── test_function_def.mlp       # ∀ add(a:ℕ,b:ℕ)→ℕ ✅
├── test_control_flow.mlp       # if/while PMLP syntax ✅
├── test_types.mlp              # ℕ, ℤ, ℝ, 𝔹, String ✅
└── test_operators.mlp          # ∧, ∨, ¬, ⊕, ⊗ operators ✅

# Test 6-10: TR → PMLP Normalization Tests
tests/normalization/tr_to_pmlp/
├── test_function.mlp           # fonksiyon → ∀ ✅
├── test_keywords.mlp           # eğer/iken → if/while ✅
├── test_types.mlp              # sayı/metin → ℕ/String ✅
├── test_operators.mlp          # ve/veya → ∧/∨ ✅
└── test_comments.mlp           # Turkish comments preserved ✅

# Test 11-15: EN → PMLP Normalization Tests
tests/normalization/en_to_pmlp/
├── test_function.mlp           # function → ∀ ✅
├── test_keywords.mlp           # if/while → symbols ✅
├── test_types.mlp              # int/string → ℕ/String ✅
├── test_operators.mlp          # and/or → ∧/∨ ✅
└── test_comments.mlp           # English comments preserved ✅

# Test 16-20: Auto-Detection Tests
tests/normalization/detection/
├── test_detect_tr.mlp          # Auto-detect Turkish ✅
├── test_detect_en.mlp          # Auto-detect English ✅
├── test_detect_pmlp.mlp        # Auto-detect PMLP ✅
├── test_mixed_file.mlp         # TR + EN mixed error ✅
└── test_confidence.mlp         # Detection confidence ✅

# Test 21-25: Cross-Language Import Tests
tests/normalization/cross_import/
├── test_tr_imports_en.mlp      # TR file imports EN module ✅
├── test_en_imports_tr.mlp      # EN file imports TR module ✅
├── test_pmlp_imports_tr.mlp    # PMLP imports TR ✅
├── test_pmlp_imports_en.mlp    # PMLP imports EN ✅
└── test_circular.mlp           # Circular imports detection ✅

# Test 26-30: Multi-Language Stdlib Tests
tests/normalization/stdlib/
├── test_tr_stdlib.mlp          # stdlib/tr/ usage ✅
├── test_en_stdlib.mlp          # stdlib/en/ usage ✅
├── test_pmlp_stdlib.mlp        # stdlib/pmlp/ usage ✅
├── test_stdlib_equiv.mlp       # TR/EN/PMLP equivalence ✅
└── test_stdlib_api.mlp         # API consistency check ✅

# Test 31-35: Performance & Cache Tests
tests/normalization/performance/
├── test_normalize_speed.mlp    # Normalization <25ms ✅
├── test_cache_hit.mlp          # Cache hit performance ✅
├── test_cache_invalidate.mlp   # Cache invalidation ✅
├── test_large_file.mlp         # 10000 line file <500ms ✅
└── test_memory_usage.mlp       # Memory overhead check ✅
```

### 📊 TEST RAPORU FORMATI

**Dosya:** `TEST_RAPORU_TODO8.md`

```markdown
# TODO #8 TEST RAPORU

## ÖZET
- **Toplam Test:** 35
- **Başarılı:** 35/35 ✅
- **Başarısız:** 0/35 ✅
- **Test Süresi:** ~90 saniye

## DETAY

### PMLP Specification Tests (5/5 ✅)
- test_symbols.mlp: PASS (100ms)
- test_function_def.mlp: PASS (120ms)
- test_control_flow.mlp: PASS (150ms)
- test_types.mlp: PASS (110ms)
- test_operators.mlp: PASS (130ms)

### TR → PMLP Normalization Tests (5/5 ✅)
- test_function.mlp: PASS (180ms)
- test_keywords.mlp: PASS (200ms)
- test_types.mlp: PASS (160ms)
- test_operators.mlp: PASS (170ms)
- test_comments.mlp: PASS (140ms)

### EN → PMLP Normalization Tests (5/5 ✅)
- test_function.mlp: PASS (180ms)
- test_keywords.mlp: PASS (190ms)
- test_types.mlp: PASS (160ms)
- test_operators.mlp: PASS (170ms)
- test_comments.mlp: PASS (140ms)

### Auto-Detection Tests (5/5 ✅)
- test_detect_tr.mlp: PASS (80ms)
- test_detect_en.mlp: PASS (75ms)
- test_detect_pmlp.mlp: PASS (70ms)
- test_mixed_file.mlp: PASS (90ms) - Error detected ✅
- test_confidence.mlp: PASS (100ms) - >95% accuracy ✅

### Cross-Language Import Tests (5/5 ✅)
- test_tr_imports_en.mlp: PASS (300ms)
- test_en_imports_tr.mlp: PASS (310ms)
- test_pmlp_imports_tr.mlp: PASS (280ms)
- test_pmlp_imports_en.mlp: PASS (290ms)
- test_circular.mlp: PASS (200ms) - Detected ✅

### Multi-Language Stdlib Tests (5/5 ✅)
- test_tr_stdlib.mlp: PASS (250ms)
- test_en_stdlib.mlp: PASS (240ms)
- test_pmlp_stdlib.mlp: PASS (230ms)
- test_stdlib_equiv.mlp: PASS (400ms) - 100% equiv ✅
- test_stdlib_api.mlp: PASS (350ms)

### Performance & Cache Tests (5/5 ✅)
- test_normalize_speed.mlp: PASS (18ms) - Target <25ms ✅
- test_cache_hit.mlp: PASS (5ms) - Cache working ✅
- test_cache_invalidate.mlp: PASS (120ms)
- test_large_file.mlp: PASS (420ms) - Target <500ms ✅
- test_memory_usage.mlp: PASS (80ms) - <10MB overhead ✅

## MULTI-LANGUAGE VALIDATION
- TR → PMLP → C compilation: Working ✅
- EN → PMLP → C compilation: Working ✅
- PMLP direct → C compilation: Working ✅
- Cross-language imports: Working ✅
- Stdlib 3x variants (tr/en/pmlp): All working ✅
- Auto-detection accuracy: >95% ✅
- Performance overhead: <25ms (18ms avg) ✅

## SONUÇ
✅ TODO #8 TAMAMLANDI - Multi-language support complete!
🌍 MLP-GCC = TRULY MULTI-LINGUAL LANGUAGE! 🌍
```

### 🎯 BAŞARI KRİTERİ

**TODO #8 tamamlanabilir ancak ve ancak:**
- ✅ 35/35 test geçiyor
- ✅ PMLP specification complete ve documented
- ✅ TR → PMLP normalizer working
- ✅ EN → PMLP normalizer working
- ✅ Auto-detection >95% accuracy
- ✅ Cross-language imports working (TR ↔ EN)
- ✅ Multi-language stdlib (tr/, en/, pmlp/) complete
- ✅ Performance <25ms normalization overhead
- ✅ Cache system <5ms cached normalization
- ✅ Documentation complete (PMLP spec, usage guide)

---

## 📝 NOTLAR

### Design Decisions

**Q: Neden PMLP (mathematical notation)?**  
A: Language-agnostic internal format. TR/EN keyword conflicts'i önler.

**Q: Unicode symbols okunabilir mi?**  
A: PMLP sadece compiler internal. User TR/EN yazıyor.

**Q: Performance overhead?**  
A: +20ms normalization. Cache ile ~5ms'ye düşer.

**Q: Cross-language imports neden?**  
A: Ekosistem zenginliği. TR kullanıcı EN kütüphane kullanabilmeli.

---

**TODO tamamlandığında:** 🌍 MLP = Truly multi-lingual programming language! 🌍

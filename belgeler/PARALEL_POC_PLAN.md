# PARALEL POC PLANI - Paradigma 1 vs Paradigma 2

**Tarih:** 24 Ocak 2026  
**Durum:** 📋 PLANLAMA  
**Hedef:** 1 hafta POC + 2 hafta full dev = 3 hafta toplam  
**Karar:** Pragmatik test ile en iyi yaklaşımı bul

---

## 🎯 GENEL STRATEJİ

```
Hafta 1: PROOF OF CONCEPT (POC)
├─ Gün 1-2: Paradigm 1 POC (Monolithic)
├─ Gün 3-4: Paradigm 2 POC (Modular)
└─ Gün 5: Karşılaştırma ve Karar

Hafta 2-3: FULL DEVELOPMENT
└─ Kazanan yaklaşımla TC + BASE + IDEAL + MODERN
```

---

## 📋 HAFTA 1: POC DETAYLARI

### POC Kapsamı (Minimal ama Representative)

**5 Temel Feature:**
1. **Variable Declaration:** `numeric x = 42`
2. **If Statement:** `if x > 0 ... end_if`
3. **While Loop:** `while x > 0 ... end_while`
4. **Function Call:** `print(x)`
5. **Binary Operations:** `x + y`, `x * y`

**Çıktı:**
- Hello World programı çalışmalı
- Fibonacci(10) hesaplamalı
- ~50-100 satır test kodu

---

## 🏗️ POC_PARADIGM1: Monolithic Yaklaşım

### Dizin Yapısı

```
POC_Paradigm1/
├─ lexer/
│  ├─ token.hpp          (Token types: IF, WHILE, NUMERIC, etc.)
│  └─ lexer.cpp          (Tokenize all features)
├─ parser/
│  ├─ ast.hpp            (AST nodes: IfStmt, WhileStmt, etc.)
│  └─ parser.cpp         (CENTRAL parser - all parse methods)
├─ codegen/
│  └─ codegen.cpp        (CENTRAL codegen - all IR generation)
├─ runtime/
│  └─ runtime.cpp        (Built-in functions: print, etc.)
├─ CMakeLists.txt
├─ main.cpp              (Compiler entry point)
└─ tests/
   ├─ hello_world.pmpl
   └─ fibonacci.pmpl
```

### Parser Yapısı (Monolithic)

```cpp
// parser/parser.cpp
class Parser {
    // CENTRAL PARSER - All features in one class
    
    unique_ptr<Stmt> parseStatement() {
        if (match(IF)) return parseIfStmt();
        if (match(WHILE)) return parseWhileStmt();
        if (match(NUMERIC)) return parseVarDecl();
        // ... all features here
    }
    
    unique_ptr<IfStmt> parseIfStmt() { /* ... */ }
    unique_ptr<WhileStmt> parseWhileStmt() { /* ... */ }
    unique_ptr<VarDecl> parseVarDecl() { /* ... */ }
    // ... 20+ methods
};
```

### Geliştirme Süresi: 2 gün

| Görev | Süre | Detay |
|-------|------|-------|
| Lexer | 3h | Token types + tokenize |
| Parser | 6h | AST + 5 parse methods |
| Codegen | 4h | LLVM IR generation |
| Runtime | 2h | print() function |
| Integration | 3h | main.cpp + CMake + tests |
| **TOTAL** | **18h** | **~2 gün** |

---

## 🧩 POC_PARADIGM2: Modular Yaklaşım

### Dizin Yapısı

```
POC_Paradigm2/
├─ common/
│  ├─ token_base.hpp     (Base token interface)
│  ├─ parser_base.hpp    (Base parser interface)
│  └─ codegen_base.hpp   (Base codegen interface)
│
├─ modules/
│  ├─ variable/          (Variable declaration module)
│  │  ├─ var_token.hpp   (NUMERIC, IDENTIFIER tokens)
│  │  ├─ var_parser.cpp  (parseVarDecl)
│  │  ├─ var_codegen.cpp (generate var IR)
│  │  └─ CMakeLists.txt  (libvar_module.a)
│  │
│  ├─ if_statement/      (If statement module)
│  │  ├─ if_token.hpp    (IF, ELSE, END_IF tokens)
│  │  ├─ if_parser.cpp   (parseIfStmt)
│  │  ├─ if_codegen.cpp  (generate if IR)
│  │  └─ CMakeLists.txt  (libif_module.a)
│  │
│  ├─ while_loop/        (While loop module)
│  │  ├─ while_token.hpp (WHILE, END_WHILE tokens)
│  │  ├─ while_parser.cpp
│  │  ├─ while_codegen.cpp
│  │  └─ CMakeLists.txt
│  │
│  ├─ function_call/     (Function call module)
│  │  ├─ call_token.hpp
│  │  ├─ call_parser.cpp
│  │  ├─ call_codegen.cpp
│  │  └─ CMakeLists.txt
│  │
│  └─ binary_ops/        (Binary operations module)
│     ├─ op_token.hpp
│     ├─ op_parser.cpp
│     ├─ op_codegen.cpp
│     └─ CMakeLists.txt
│
├─ coordinator/
│  ├─ module_loader.cpp  (Load all modules dynamically)
│  ├─ parse_dispatcher.cpp (Dispatch to correct module)
│  └─ main.cpp           (Minimal orchestrator)
│
└─ tests/
   ├─ hello_world.pmpl
   └─ fibonacci.pmpl
```

### Module Interface (Modular)

```cpp
// common/parser_base.hpp
class IModuleParser {
public:
    virtual bool canParse(Token token) = 0;
    virtual unique_ptr<Stmt> parse(TokenStream& tokens) = 0;
    virtual ~IModuleParser() = default;
};

// modules/if_statement/if_parser.cpp
class IfModuleParser : public IModuleParser {
public:
    bool canParse(Token token) override {
        return token.type == TokenType::IF;
    }
    
    unique_ptr<Stmt> parse(TokenStream& tokens) override {
        // ONLY parse if statement
        // No knowledge of while, var, etc.
        return parseIfStmt(tokens);
    }
};

// coordinator/parse_dispatcher.cpp
class ParseDispatcher {
    vector<unique_ptr<IModuleParser>> modules_;
    
public:
    void registerModule(unique_ptr<IModuleParser> module) {
        modules_.push_back(std::move(module));
    }
    
    unique_ptr<Stmt> dispatch(TokenStream& tokens) {
        Token current = tokens.peek();
        
        // Find module that can handle this token
        for (auto& module : modules_) {
            if (module->canParse(current)) {
                return module->parse(tokens);
            }
        }
        
        throw ParseError("No module can handle token");
    }
};
```

### Geliştirme Süresi: 3 gün

| Görev | Süre | Detay |
|-------|------|-------|
| Common infrastructure | 4h | Token base, Parser base, Codegen base |
| Variable module | 3h | Token + Parser + Codegen + CMake |
| If module | 3h | Token + Parser + Codegen + CMake |
| While module | 3h | Token + Parser + Codegen + CMake |
| Function call module | 3h | Token + Parser + Codegen + CMake |
| Binary ops module | 3h | Token + Parser + Codegen + CMake |
| Coordinator | 4h | Module loader + Dispatcher |
| Integration | 4h | main.cpp + CMake + tests |
| **TOTAL** | **27h** | **~3 gün** |

---

## 📊 KARŞILAŞTIRMA KRİTERLERİ (Gün 5)

### 1. Code Complexity

```
Metric: Cyclomatic Complexity, Lines of Code
Question: Hangisi daha basit?
```

### 2. Development Speed

```
Metric: Time to add new feature
Test: "for loop" ekle, süreyi ölç
```

### 3. Module Independence

```
Metric: Copy-paste test
Test: "if module"ü başka projeye kopyala
```

### 4. Build Time

```
Metric: cmake + make süresi
Test: Hangisi daha hızlı build?
```

### 5. Test Coverage

```
Metric: Test yazma kolaylığı
Test: Unit test yazma süresi
```

### 6. Debugging

```
Metric: Bug bulma/düzeltme süresi
Test: Kasıtlı bug ekle, bulma süresi
```

### 7. MELP Philosophy Compliance

```
Metric: Modüler prensiplere uyum
Checklist:
- ✅/❌ Merkezi orchestrator var mı?
- ✅/❌ Peer-to-peer network yapısı?
- ✅/❌ Modüller bağımsız mı?
```

---

## 🏆 KARAR MATRİSİ (Gün 5 Sonu)

| Kriter | Ağırlık | Paradigm 1 | Paradigm 2 | Kazanan |
|--------|---------|------------|------------|---------|
| Code Complexity | 15% | ? | ? | ? |
| Development Speed | 20% | ? | ? | ? |
| Module Independence | 25% | ? | ? | ? |
| Build Time | 5% | ? | ? | ? |
| Test Coverage | 10% | ? | ? | ? |
| Debugging | 10% | ? | ? | ? |
| MELP Compliance | 15% | ? | ? | ? |
| **TOTAL** | 100% | ? | ? | **?** |

**Karar Kriteri:**
- **70%+ Paradigm 1:** Monolithic ile devam
- **70%+ Paradigm 2:** Modular ile devam
- **40-60% range:** Hybrid yaklaşım (core monolithic, features modular)

---

## 📅 HAFTA 2-3: FULL DEVELOPMENT

### Kazanan: Paradigm 1 (Monolithic)

```
Hafta 2: TC + BASE
├─ Gün 1-3: TC (Turing Complete)
│  └─ 20 tokens, 6 AST nodes, basic codegen
├─ Gün 4-5: BASE (String + I/O)
│  └─ String ops, file I/O, format

Hafta 3: IDEAL + MODERN
├─ Gün 1-3: IDEAL (Arrays, Structs, Enums)
│  └─ Advanced features
├─ Gün 4-5: MODERN (Classes, Lambda)
│  └─ OOP features
```

### Kazanan: Paradigm 2 (Modular)

```
Hafta 2: Core + 10 Modules
├─ Gün 1: Common infrastructure
├─ Gün 2-5: 10 temel modül
│  ├─ variables, if, while, for, function
│  ├─ string, array, struct, enum, class

Hafta 3: Advanced + Integration
├─ Gün 1-3: 5 advanced modül
│  └─ lambda, async, pattern_match, etc.
├─ Gün 4-5: Integration + Testing
│  └─ Module coordination, stress test
```

### Kazanan: Hybrid

```
Hafta 2: Monolithic Core + Modular Extensions
├─ Core (Monolithic): variables, control flow, functions
├─ Modules (Modular): string, array, struct, class

Hafta 3: Complete Features
├─ Extend core features
├─ Add more modules
└─ Integration testing
```

---

## ✅ SUCCESS METRICS

### POC Success (Gün 5)

- [ ] Hello World çalışıyor (her iki POC)
- [ ] Fibonacci(10) = 55 hesaplanıyor (her iki POC)
- [ ] Clear winner belirlendi (70%+ skor)
- [ ] Karar matrix dolduruldu
- [ ] Full dev planı hazır

### Week 2 Success

- [ ] TC seviyesi tamamlandı
- [ ] BASE seviyesi tamamlandı
- [ ] Tüm testler geçiyor
- [ ] 0 memory leak

### Week 3 Success

- [ ] IDEAL seviyesi tamamlandı
- [ ] MODERN seviyesi tamamlandı
- [ ] Production ready
- [ ] STDLIB geliştirmeye hazır

---

## 🚀 BAŞLANGIÇ KOMUTU

```bash
# Workspace hazırlığı
cd /home/pardus/projeler/MLP/LLVM/stage0/C++
mkdir -p POC_tests

# Paradigm 1 POC
mkdir -p POC_tests/POC_Paradigm1/{lexer,parser,codegen,runtime,tests}

# Paradigm 2 POC
mkdir -p POC_tests/POC_Paradigm2/{common,modules/{variable,if_statement,while_loop,function_call,binary_ops},coordinator,tests}

# Test programs
cat > POC_tests/hello_world.pmpl << 'EOF'
function main()
    print("Hello, World!")
end_function
EOF

cat > POC_tests/fibonacci.pmpl << 'EOF'
function fib(numeric n)
    if n <= 1
        return n
    end_if
    return fib(n - 1) + fib(n - 2)
end_function

function main()
    numeric result = fib(10)
    print(result)
end_function
EOF
```

---

## 📝 NOTLAR

**Risk Mitigation:**
- Her gün sonunda working checkpoint
- POC başarısız olursa: Mevcut TC/BASE/IDEAL'i kullan
- Timeline kayıyorsa: Hybrid'e geç

**Critical Success Factor:**
- POC'de **pratik deneyim** edineceğiz
- Teorik değil, **çalışan kod** ile karar
- Her iki yaklaşım da **documented** olacak

**Next Steps:**
- ÜA: POC görevi hazırla
- YZ: POC implementasyonu başlat
- MM: Progress tracking + risk monitoring

---

**BAŞLA:** Paradigm 1 POC - Gün 1 (bugün!)

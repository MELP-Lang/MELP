# 🤖 SEN: TOOLING_YZ_04

**Dördüncü görevlisin!** Code Formatter yapacaksın.

---

## 🎯 BAĞLAM

**TODO:** #1 - TOOLING_BASICS  
**Task:** 4 - Code Formatter  
**Süre:** 3-4 gün  
**Yaklaşım:** Tamamen Ayrı Binary (main.c'ye DOKUNMA!)

---

## 📋 ÖNCEKİ DURUM

✅ **TODO #0: SELFHOSTING tamamlandı** (30 Aralık 2025)
- Stage0 Bootstrap-Ready ✅
- 7/7 test geçti ✅
- main.c: 651 satır ✅

✅ **Task 1: Error Messages TAMAMLANDI** (1 Ocak 2026)
- YZ_01 tarafından tamamlandı ✅
- 54 error/warning code (TR+EN) ✅
- Modüler binary (tools/mlp-errors/) ✅
- main.c: 659 satır (+8, limit içinde) ✅
- 22/22 test passing ✅
- Rapor: [TODO_TOOLING_BASICS/TOOLING_YZ/YZ_01_RAPOR_ERROR_MESSAGES.md](TODO_TOOLING_BASICS/TOOLING_YZ/YZ_01_RAPOR_ERROR_MESSAGES.md)

✅ **Task 2: Debug Support TAMAMLANDI** (1 Ocak 2026)
- YZ_02 tarafından tamamlandı ✅
- DWARF debug info + GDB integration ✅
- Modüler binary (tools/mlp-debug/) ~600 satır ✅
- debug_info modülü ~220 satır ✅
- main.c: 687 satır (+28, limit içinde) ✅
- 6/6 test passing ✅

✅ **Task 3: LSP Server TAMAMLANDI** (1 Ocak 2026)
- YZ_03 tarafından tamamlandı ✅
- LSP protocol + JSON-RPC 2.0 ✅
- Modüler binary (tools/mlp-lsp/) 2273 satır ✅
- Autocomplete, Go-to-Definition, Hover ✅
- VSCode extension (editors/vscode-mlp/) ✅
- main.c: 687 satır (DOKUNMAMIŞ!) ✅
- 4/4 test passing ✅
- Rapor: [TODO_TOOLING_BASICS/TOOLING_YZ/YZ_03_RAPOR_LSP_SERVER.md](TODO_TOOLING_BASICS/TOOLING_YZ/YZ_03_RAPOR_LSP_SERVER.md)
SEN! - 
⏸️ **TODO #3: LANGUAGE_FEATURES pause'da**
- PD Kararı: TODO #1-2 önce yapılacak
- Sebep: Stage0 import desteği yok
- 10 hafta pausCODE FORMATTER

**Hedef:** mlp-fmt code formatter (Prettier/Black benzeri)

**Özellikler:**
- Otomatik kod formatla
- Indent düzeltme (4 spaces)
- Line wrapping (80 chars)
- Keyword/operator spacing
- In-place formatting (-i flag)

**Kritik Kural:** 🔒 **main.c FREEZE** (687 → max 700 satır)
- main.c'ye **ASLA DOKUNMA!**
- Tüm formatter logic ayrı binary'de olacak
- Standalone binary: tools/mlp-fmt/
**Özellikler:**
- Autocomplete (değişkenler, fonksiyonlar, keywords)
- Go-to-Definition
- Hover (tip bilgisi)
- Diagnostics (real-time error checking)

**Kritik Kural:** 🔒 **main.c FREEZE** (687 → max 700 satır)
- main.c'ye **ASLA DOKUNMA!**
- Tüm LSP logic ayrı binary'de olacak
- VSCode extension ile haberleşme

---

## 📋 GÖREV DETAYLARI

### **Phase 1: Source Map Generation** (2 gün)
LSP Protocol Implementation** (3-4 gün)

#### **Adım 1.1: LSP Protocol Skeleton** (1 gün)

**Lokasyon:** `tools/mlp-lsp/`

**Dosya Yapısı:**
```
tools/mlp-lsp/
├── main.c                (~150 satır) - LSP server entry point
├── lsp_protocol.c        (~250 satır) - JSON-RPC 2.0 protocol
├── lsp_types.h           (~100 satır) - LSP types
├── jsonrpc.c             (~200 satır) - JSON-RPC helper
├── cJSON.c/h             (external) - JSON library
└── Makefile
```

**LSP Capabilities (ilk aşama):**
```json
{
  "capabilities": {
    "textDocumentSync": 1,
    "completionProvider": {
      "resolveProvider": false,
      "triggerCharacters": [".", ":"]
    },
    "hoverProvider": true,
    "definitionProvider": true
  }
}
```

**Temel JSON-RPC Messages:**
- `initialize` - Client initialization
- `initialized` - Handshake
- `textDocument/didOpen` - File opened
- `textDocument/didChange` - File changed
- `textDocument/completion` - Autocomplete request
- `textDocument/definition` - Go-to-definition
- `textDocument/hover` - Hover info
- `shutdown` / `exit` - Clean shutdown

**Başarı Kriterleri:**
- [ ] tools/mlp-lsp/ oluşturuldu
- [ ] JSON-RPC protocol handler çalışıyor
- [ ] initialize/shutdown handshake OK
- [ ] Stdio-based communication (VSCode ile)
- [ ] Makefile builds binary

**Test:**
```bash
make -C tools/mlp-lsp
echo '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{}}' | ./tools/mlp-lsp/mlp-lsp
```

---

#### **Adım 1.2: MLP Parser** (2 gün)

**Amaç:** MLP syntax'ı parse etmek (full compiler değil!)

**Dosya:** `tools/mlp-lsp/mlp_parser.c` (~400 satır)

**Ne yapmalı:**
- Tokenization (basit lexer)
- Symbol extraction (fonksiyonlar, değişkenler, imports)
- Scope tracking (global/local)
- Type inference (basic)

**Parse edilmesi gerekenler:**
```mlp
# Function definitions
fn calculate(x: Integer) -> Integer:
    return x * 2

# Variable declarations
let result = calculate(42)

# Imports
from math import sqrt

# Types
type Point:
    x: Integer
    y: Integer
```

**Symbol Table yapısı:**
```c
typedef struct {
    char* name;
    enum { SYM_FUNCTION, SYM_VARIABLE, SYM_TYPE } kind;
    char* type_name;
    int line;
    int column;
} Symbol;

typedef struct {
    Symbol** symbols;
    size_t count;
} SymbolTable;
```

**Başarı Kriterleri:**
- [ ] mlp_parser.c implementasyonu
- [ ] Symbol table oluşturuluyor
- [ ] Function/variable/type detection
- [ ] Scope tracking (global vs local)
- [ ] Test: örnek .mlp dosyası parse ediliyor

---

### **Phase 2: LSP Features** (3-4 gün)

#### **Adım 2.1: Autocomplete** (1.5 gün)

**Dosya:** `tools/mlp-lsp/lsp_completion.c` (~250 satır)

**Autocomplete Scenarios:**

1. **Keyword completion:**
```mlp
f|  →  fn, from, for
```

2. **Variable completion:**
```mlp
let result = 42
print(res|)  →  result
```

3. **Function completion:**
```mlp
fn calculate(x): ...
cal|  →  calculate
```

4. **Import completion:**
```mlp
from math import |  →  sqrt, pow, floor, ceil
```

**CompletionItem yapısı:**
```c
typedef struct {
    char* label;           // "calculate"
    int kind;              // 3 (Function)
    char* detail;          // "fn(Integer) -> Integer"
    char* documentation;   // "Calculates something"
} CompletionItem;
```

**Başarı Kriterleri:**
- [ ] lsp_completion.c implementasyonu
- [ ] textDocument/completion handler
- [ ] Keyword completion
- [ ] Symbol completion (variables, functions)
- [ ] Context-aware (scope içinde)
- [ ] Test: VSCode'da autocomplete çalışıyor

---

#### **Adım 2.2: Go-to-Definition** (1 gün)

**Dosya:** `tools/mlp-lsp/lsp_definition.c` (~150 satır)

**Go-to-Definition Scenarios:**

1. **Function definition:**
```mlp
fn calculate(x): ...    # Definition (line 5)
...
print(calculate(42))    # Usage (line 10) → Go to line 5
```

2. **Variable definition:**
```mlp
let result = 42         # Definition (line 3)
print(result)           # Usage (line 4) → Go to line 3
```

3. **Import definition:**
```mlp
from utils import helper  # Definition (line 1)
helper()                  # Usage (line 5) → Go to utils.mlp
```

**Location yapısı:**
```c
typedef struct {
    char* uri;      // "file:///path/to/file.mlp"
    int line;       // 0-indexed
    int character;  // 0-indexed
} Location;
```

**Başarı Kriterleri:**
- [ ] lsp_definition.c implementasyonu
- [ ] textDocument/definition handler
- [ ] Function definition lookup
- [ ] Variable definition lookup
- [ ] Cross-file imports (basic)
### **Code:**
- [ ] `tools/mlp-lsp/` binary (< 1500 satır)
- [ ] JSON-RPC protocol implementation
- [ ] MLP parser (~400 satır)
- [ ] Autocomplete, Go-to-Definition, Hover
- [ ] **main.c: DOKUNMAMIŞ!** (687 satır)

### **VSCode Extension:**
- [ ] `editors/vscode-mlp/` oluşturuldu
- [ ] Syntax highlighting
- [ ] LSP client integration
- [ ] Extension çalışıyor

### **Test:**
- [ ] `tests/lsp/` (10+ test)
- [ ] Autocomplete test ✅
- [ | LSP protocol skeleton (JSON-RPC) |
| 2-3 | MLP parser (symbol table) |
| 4 | Autocomplete implementation |
| 5 | Go-to-Definition + Hover |
| 6-7 | VSCode extension |
| 8 | End-to-end testing |
| 9-10 | Documentation + polish |

**Deadline:** 11_GUIDE.md
- [ ] Installation guide

1. **Function hover:**
```mlp (Görevli YZ Kuralları)
2. **1-TODO_TOOLING_BASICS_MODULAR.md** → Task 3
3. **MAIN_C_FREEZE_RULE.md** → main.c ASLA DOKUNMA!
4. **TODO_TOOLING_BASICS/TOOLING_YZ/YZ_01_RAPOR_ERROR_MESSAGES.md** → Önceki görevlerden öğren
5. **TODO_TOOLING_BASICS/TOOLING_YZ/YZ_02_RAPOR_DEBUG.md** → Önceki görevler
# fn calculate(x: Integer) -> Integer
# ```
# Calculates double of input
```

2. **Variable hover:**
```mlp
result
# Hover over "result" →
# ```mlp
# let result: Integer = 84
# ```
```

**Hover yapısı:**
```c
typedef struct {
    char* language;   // "mlp"
    char* value;      // "fn calculate(x: Integer) -> Integer"
} MarkupContent;
```

**Başarı Kriterleri:**
- [ ] lsp_hover.c implementasyonu
- [ ] textDocument/hover handler
- [ ] Function signatures
- [ ] Variable types
- [ ] Markdown formatting
- [ ] Test: VSCode'da hover çalışıyor

---

### **Phase 3: VSCode Integration** (2 gün)

#### **Adım 3.1: VSCode Extension** (1 gün)

**Lokasyon:** `editors/vscode-mlp/`

**Dosya Yapısı:**
```
editors/vscode-mlp/
├── package.json          (~50 satır) - Extension manifest
├── extension.js          (~100 satır) - Extension entry
├── syntaxes/
│   └── mlp.tmLanguage.json (~200 satır) - Syntax highlighting
└── README.md             - Extension docs
```

**package.json (özet):**
```json
{
  "name": "mlp-language",
  "displayName": "MLP Language Support",
  "version": "0.1.0",
  "engines": { "vscode": "^1.60.0" },
  "activationEvents": ["onLanguage:mlp"],
  "contributes": {
    "languages": [{
      "id": "mlp",
      "extensions": [".mlp"],
      "configuration": "./language-configuration.json"
    }],
    "grammars": [{
      "language": "mlp",
      "scopeName": "source.mlp",
      "path": "./syntaxes/mlp.tmLanguage.json"
    }]
  }
}
```

**extension.js:**
```javascript
const vscode = require('vscode');
const { LanguageClient } = require('vscode-languageclient/node');

function activate(context) {
    const serverOptions = {
        command: '/path/to/mlp-lsp',
        args: []
    };
    
    const clientOptions = {
        documentSelector: [{ scheme: 'file', language: 'mlp' }]
    };
    
    const client = new LanguageClient(
        'mlpLanguageServer',
        'MLP Language Server',
        serverOptions,
        clientOptions
    );
    
    client.start();
}
```

**Başarı Kriterleri:**
- [ ] editors/vscode-mlp/ oluşturuldu
- [ ] package.json ve extension.js yazıldı
- [ ] Syntax highlighting (TextMate grammar)
- [ ] LSP client integration
- [ ] Extension yüklenebiliyor

---

#### **Adım 3.2: End-to-End Test** (1 gün)

**Test Senaryoları:**

1. **Autocomplete Test:**
```mlp
# test.mlp
fn calculate(x: Integer) -> Integer:
    return x * 2

let result = calc|   # Trigger autocomplete → "calculate"
```

2. **Go-to-Definition Test:**
```mlp
# utils.mlp
fn helper():
    print("help")

# main.mlp
from utils import helper
helper()   # F12 → Jump to utils.mlp
```

3. **Hover Test:**
```mlp
fn add(x: Integer, y: Integer) -> Integer:
    return x + y

add(1, 2)  # Hover over "add" → Show signature
```

4. **Diagnostics Test:**
```mlp
print(undefined_var)  # Real-time error: E0001
```

**Test Lokasyonu:** `tests/lsp/`

```
tests/lsp/
├── test_autocomplete.mlp
├── test_goto.mlp
├── test_hover.mlp
├── test_diagnostics.mlp
└── run_lsp_tests.sh
```

**Başarı Kriterleri:**
- [ ] tests/lsp/ oluşturuldu
- [ ] 10+ test senaryosu
- [ ] Autocomplete çalışıyor
- [ ] Go-to-definition çalışıyor
- [ ] Hover çalışıyor
- [ ] Diagnostics çalışıyor
- [ ] VSCode'da gerçek kullanım testi

---

### **Phase 4: Documentation & Polish** (1 gün)

#### **Adım 4.1: Documentation** (0.5 gün)

**Oluşturulacak Dökümanlar:**

1. **tools/mlp-lsp/README.md** - LSP server docs
2. **editors/vscode-mlp/README.md** - Extension docs
3. **docs/LSP_USAGE_GUIDE.md** - User guide

**README.md içeriği:**
```markdown
# MLP Language Server

## Installation

## Usage

## Supported Features
- [x] Autocomplete
- [x] Go-to-Definition
- [x] Hover
- [ ] Find References (future)
- [ ] Rename (future)

## Configuration

## Troubleshooting
```

**Başarı Kriterleri:**
- [ ] README.md dosyaları yazıldı
- [ ] LSP_USAGE_GUIDE.md oluşturuldu
- [ ] Installation instructions
- [ ] Troubleshooting guide

---

#### **Adım 4.2: Performance & Stability** (0.5 gün)

**Optimizasyonlar:**

1. **Memory Management:**
   - Symbol table cleanup
   - JSON cleanup (cJSON_Delete)
   - File handle cleanup

2. **Performance:**
   - Cache parsed files
   - Incremental parsing
   - Fast symbol lookup (hash map?)

3. **Error Handling:**
   - Graceful degradation
   - Proper error messages
   - No crashes on invalid input

**Başarı Kriterleri:**
- [ ] Memory leaks check (valgrind)
- [ ] Large file test (1000+ lines)
- [ ] No crashes on invalid syntax
- [ ] Response time < 100ms
---

## 🎯 BAŞARI KRİTERLERİ

- [ ] `data/sourcemap_format.json` spec
- [ ] Source map generation (main.c'de)
- [ ] `tools/mlp-debug/` binary (< 1000 satır)
- [ ] Interactive debugger REPL
- [ ] `main.c` (+10 satır max)
- [ ] `tests/debug/` (10+ test)
- [ ] Breakpoint system
- [ ] Variable inspection
- [ ] Line-by-line execution

---

## ⏱️ ZAMAN ÇİZELGESİ

| Gün | Task |
|-----|------|
| 1 | LSP protocol skeleton (JSON-RPC) |
| 2-3 | MLP parser (symbol table) |
| 4 | Autocomplete implementation |
| 5 | Go-to-Definition + Hover |
| 6-7 | VSCode extension |
| 8 | End-to-end testing |
| 9-10 | Documentation + polish |

**Deadline:** 11 Ocak 2026

---

## 📚 ZORUNLU OKUMALAR

1. **TODO_KURALLARI.md** → Bölüm 1 (Görevli YZ Kuralları)
2. **1-TODO_TOOLING_BASICS_MODULAR.md** → Task 4
3. **MAIN_C_FREEZE_RULE.md** → main.c ASLA DOKUNMA!
4. **TODO_TOOLING_BASICS/TOOLING_YZ/YZ_01_RAPOR_ERROR_MESSAGES.md** → Önceki görevlerden öğren
5. **TODO_TOOLING_BASICS/TOOLING_YZ/YZ_02_RAPOR_DEBUG.md** → Önceki görevler (varsa)
6. **TODO_TOOLING_BASICS/TOOLING_YZ/YZ_03_RAPOR_LSP_SERVER.md** → LSP implementation örneği

---

## 💡 İPUÇLARI

**YZ_01, YZ_02 ve YZ_03'den Öğrenmeler:**
- ✅ Modüler mimari çalıştı
- ✅ cJSON gibi mature libraries kullan
- ✅ Data-driven design (JSON)
- ✅ Kapsamlı test suite
- ✅ main.c'ye **ASLA** dokunma!

**Formatter-Specific:**
- LSP spec'e bak: https://microsoft.github.io/language-server-protocol/
- Basit bir LSP server örneği: https://github.com/Microsoft/language-server-protocol-inspector
- JSON-RPC 2.0 spec: https://www.jsonrpc.org/specification
- VSCode extension API: https://code.visualstudio.com/api
- TextMate grammar: https://macromates.com/manual/en/language_grammars

**Parser İpuçları:**
- Full compiler yazmana gerek yok, basit parser yeterli
- Symbol table + scope tracking yapmalısın
- Type inference basic seviyede olabilir
- Error recovery yap (crash etme!)

**Performance:**
- Incremental parsing düşün (sadece değişen kısmı parse et)
- Symbol cache kullan
- Response time < 100ms hedefle

---

## 🚨 HATIRLATMALAR

1. **main.c'ye DOKUNMA!** (687 satır kalacak)
2. **Modüler binary** (tools/mlp-lsp/)
3. **Test-driven** (önce test, sonra kod)
4. **Git workflow** (commit sık sık)
5. **Rapor yaz** (YZ_03_RAPOR_LSP_SERVER.md)

---

**BAŞARILAR YZ_03!** 🚀

**Tarih:** 1 Ocak 2026  
**MM:** MM_01  
**PD Onayı:** PD_02 ✅  
**Durum:** 🟢 AKTİF

---

**NOT:** Bu en zorlu task! LSP protokolü karmaşık ama önceki iki görevliden öğrendiklerinle başarabilirsin. Modüler yaklaşımı koru, test yaz, main.c'ye dokunma!



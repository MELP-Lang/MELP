# 🚀 STAGE 1 SELF-HOSTING - START HERE!

**Session:** Stage 1 YZ_01  
**Date:** 16 Aralık 2025  
**Agent:** GitHub Copilot (Claude Sonnet 4.5)  
**Branch:** main  
**Status:** 🎯 **YZ_01 - Expression Parsing Başlıyor**

**⚠️ YZ Directory Structure:**
- `stage_0_YZ/` - Stage 0 sessions (YZ_01 - YZ_97, 80 sessions) ✅ ARCHIVED
- `stage_1_YZ/` - Stage 1 sessions (YZ_01+, current) ⏳ ACTIVE

---

## ✅ Stage 0 Tamamlandı - Artık Tarihe Karıştı!

**Stage 0 MVC (Minimum Viable Compiler) 15 Aralık 2025'te tamamlandı!**

- ✅ 34,432 satır compiler kodu (C)
- ✅ 4,584 satır runtime kodu (C)
- ✅ 33 modül
- ✅ ~90% test geçme oranı
- ✅ Tüm MVC özellikleri çalışıyor

**Arşivlendi:** 
- `temp/TODO_old_20251215_*.md`
- `temp/NEXT_AI_START_HERE_old_20251215_*.md`
- `temp/STAGE0_SIGNOFF_20251215.md`

**Stage 0 artık feature-frozen!** Sadece bug fix ve performance fix kabul ediliyor.

---

## 🎯 Stage 1 Hedef: Self-Hosting

**MELP compiler'ı MELP dilinde yaz!**

### Strateji: 3 Faz

```
┌────────────────┐
│  PHASE 1       │  ← ŞU AN BURDAYIZİ
│  Parser (MELP) │     4-6 hafta
└────────────────┘
        ↓
┌────────────────┐
│  PHASE 2       │
│  CodeGen(MELP) │     4-6 hafta
└────────────────┘
        ↓
┌────────────────┐
│  PHASE 3       │
│  Bootstrap     │     2 hafta
└────────────────┘
        ↓
    🎉 SELF-HOSTING COMPLETE!
```

---

## 📊 Self-Hosting İlerleme

| Bileşen | Durum | Dil | Satır | YZ |
|---------|-------|-----|-------|-----|
| Lexer | ✅ 100% | MELP | ~1,803 | Stage 0 (YZ_57) |
| Parser Infra | ✅ 100% | MELP | ~400 | Stage 0 (YZ_97) |
| Parser Expr | ⏳ 0% | MELP | - | **Stage 1 (YZ_01) ← ŞİMDİ** |
| Parser Stmt | ❌ 0% | MELP | - | Stage 1 (YZ_02) |
| CodeGen | ❌ 0% | C | - | Stage 1 (Sonra) |
| Bootstrap | ❌ 0% | - | - | Stage 1 (En son) |

**TOPLAM:** %30 Complete

---

## 🎯 ŞİMDİ NE YAPACAĞIZ?

### ⏳ Yeni YZ Session: YZ_01

**Görev:** Phase 1 Part 2 - Expression Parsing  
**Dosya:** `stage_1_YZ/YZ_01.md`  
**Süre:** 4-6 saat  

**Başlamak için:**
```bash
cat stage_1_YZ/YZ_01.md
```

Bu session'da **Expression Parser**'ı MELP ile yazacağız:
- Primary expressions (literals, identifiers)
- Binary operations (+, -, *, /, %)
- Comparison & logical operations
- Operator precedence

**YZ_01'e git ve görevi başlat!** 🚀

---

## 📚 Eski Referans (Arşiv)

### Phase 1 Part 1: Parser Infrastructure (✅ TAMAMLANDI - YZ_97)

**Görev:** Parser'ın temellerini MELP ile kur

#### Step 1: Dizin ve Dosya Yapısı
```bash
mkdir -p modules/parser_mlp
touch modules/parser_mlp/parser_core.mlp
touch modules/parser_mlp/ast_nodes.mlp
touch modules/parser_mlp/token_stream.mlp
```

#### Step 2: AST Node Structures

**Dosya:** `modules/parser_mlp/ast_nodes.mlp`

```mlp
-- AST Node Types (Enum)
enum ASTNodeType
    NODE_EXPRESSION = 1
    NODE_STATEMENT = 2
    NODE_FUNCTION = 3
    NODE_LITERAL = 4
    NODE_BINARY_OP = 5
    NODE_CALL = 6
    -- ... daha fazla
end_enum

-- Base AST Node (Struct)
struct ASTNode
    numeric node_type        -- ASTNodeType enum
    numeric line
    numeric column
    -- Node-specific data için genişletilebilir
end_struct

-- Expression Node
struct ExprNode
    numeric node_type
    numeric line
    numeric column
    numeric expr_type        -- EXPR_LITERAL, EXPR_BINARY, etc.
    string value            -- For literals
end_struct

-- Statement Node
struct StmtNode
    numeric node_type
    numeric line
    numeric column
    numeric stmt_type        -- STMT_VAR, STMT_IF, etc.
end_struct
```

#### Step 3: Token Stream Management

**Dosya:** `modules/parser_mlp/token_stream.mlp`

```mlp
-- Global parser state (list of tokens)
list tokens           -- Token listesi
numeric current_pos   -- Mevcut pozisyon

function init_parser(list token_list) returns numeric
    tokens = token_list
    current_pos = 0
    return 1
end_function

function current_token() returns list
    if current_pos >= length(tokens) then
        return []  -- EOF
    end_if
    return tokens[current_pos]
end_function

function peek_token() returns list
    numeric next_pos = current_pos + 1
    if next_pos >= length(tokens) then
        return []  -- EOF
    end_if
    return tokens[next_pos]
end_function

function advance() returns numeric
    current_pos = current_pos + 1
    return current_pos
end_function

function expect_token(numeric expected_type) returns boolean
    list tok = current_token()
    if length(tok) == 0 then
        return 0  -- EOF
    end_if
    
    numeric tok_type = tok[0]
    if tok_type == expected_type then
        advance()
        return 1
    end_if
    return 0
end_function
```

#### Step 4: Error Handling

**Dosya:** `modules/parser_mlp/parser_errors.mlp`

```mlp
function parser_error(string message, numeric line, numeric col) returns numeric
    println("Parser Error: " + message)
    println("  at line " + str(line) + ", column " + str(col))
    return 0
end_function

function unexpected_token_error(numeric expected, numeric got) returns numeric
    println("Unexpected token: expected " + str(expected) + ", got " + str(got))
    return 0
end_function
```

#### Step 5: Integration Test

**Dosya:** `tests/manual/test_parser_init.mlp`

```mlp
-- Test: Initialize parser with token list

function main() returns numeric
    -- Create a simple token list
    list tok1 = [1, "function", 1, 1]  -- [type, value, line, col]
    list tok2 = [32, "main", 1, 10]
    list tok3 = [20, "(", 1, 14]
    list tok4 = [21, ")", 1, 15]
    
    list token_list = [tok1, tok2, tok3, tok4]
    
    init_parser(token_list)
    
    list first = current_token()
    println("First token type: " + str(first[0]))  -- Should be 1
    
    advance()
    list second = current_token()
    println("Second token type: " + str(second[0]))  -- Should be 32
    
    return 0
end_function
```

---

## 📋 Phase 1 Part 1 Checklist

- [ ] `modules/parser_mlp/` dizini oluştur
- [ ] `ast_nodes.mlp` - AST node structures
  - [ ] ASTNodeType enum
  - [ ] ASTNode base struct
  - [ ] ExprNode struct
  - [ ] StmtNode struct
- [ ] `token_stream.mlp` - Token stream management
  - [ ] init_parser()
  - [ ] current_token()
  - [ ] peek_token()
  - [ ] advance()
  - [ ] expect_token()
- [ ] `parser_errors.mlp` - Error handling
  - [ ] parser_error()
  - [ ] unexpected_token_error()
- [ ] `tests/manual/test_parser_init.mlp` - Integration test
- [ ] Compile ve test et
  ```bash
  ./compiler/stage0/modules/functions/functions_standalone \
      modules/parser_mlp/token_stream.mlp token_stream.s
  ```

---

## 🚦 Sonraki Adımlar (Part 2+)

**Part 1 tamamlandıktan sonra:**

1. **Part 2:** Expression Parsing
   - Primary expressions (literals, identifiers)
   - Binary operations (arithmetic, comparison)
   - Precedence handling (Pratt parsing)

2. **Part 3:** Statement Parsing
   - Variable declarations
   - Control flow (if/while/for)
   - Function calls

3. **Part 4:** Advanced Parsing
   - Function definitions
   - Struct definitions
   - Module imports

4. **Part 5:** Integration & Testing
   - Lexer + Parser pipeline
   - AST validation
   - End-to-end tests

---

## 📁 Dosya Yapısı

```
modules/
├── lexer_mlp/              ← ✅ DONE (YZ_57)
│   ├── lexer.mlp
│   ├── tokenize_*.mlp
│   └── ...
│
└── parser_mlp/             ← 🎯 ŞİMDİ BURASI!
    ├── parser_core.mlp     ← Main parser
    ├── ast_nodes.mlp       ← AST structures
    ├── token_stream.mlp    ← Token management
    ├── parser_errors.mlp   ← Error handling
    ├── parser_expr.mlp     ← Expression parsing (Part 2)
    ├── parser_stmt.mlp     ← Statement parsing (Part 3)
    ├── parser_func.mlp     ← Function parsing (Part 4)
    └── parser_struct.mlp   ← Struct parsing (Part 4)

tests/
├── manual/                 ← Manuel test dosyaları
│   ├── test_parser_*.mlp
│   └── test_lexer_*.mlp
└── e2e/                    ← End-to-end testler

temp/                       ← Geçici deneme dosyaları
    └── experiment_*.mlp
```

### ⚠️ ÖNEMLI: Dosya Organizasyonu

**Ana dizini temiz tut!**

- ✅ Test dosyaları → `tests/manual/` veya `tests/e2e/`
- ✅ Geçici dosyalar → `temp/`
- ✅ Ana dizin → Sadece proje dosyaları (README, TODO, MELP_REFERENCE, vb.)
- ❌ Ana dizinde `test_*.mlp`, `*.s`, `*.o` bırakma!

```bash
# Cleanup komutu (düzenli çalıştır)
rm -f test_*.mlp test_*.s test_*.o *.s *.o
```

---

## ⚡ Hızlı Başlangıç

```bash
# 1. Dizin oluştur
mkdir -p modules/parser_mlp

# 2. İlk dosyayı oluştur
cat > modules/parser_mlp/ast_nodes.mlp << 'EOF'
-- AST Node Types
enum ASTNodeType
    NODE_EXPRESSION = 1
    NODE_STATEMENT = 2
    NODE_FUNCTION = 3
end_enum

struct ASTNode
    numeric node_type
    numeric line
    numeric column
end_struct

function main() returns numeric
    return 0
end_function
EOF

# 3. Compile et
./compiler/stage0/modules/functions/functions_standalone \
    modules/parser_mlp/ast_nodes.mlp ast_nodes.s

# 4. Test et
./ast_nodes.s
echo $?  # Should be 0
```

---

## 🎯 Başarı Kriterleri (Part 1)

✅ **Tamamlandı sayılır eğer:**
1. AST node structures tanımlandı (enum + struct)
2. Token stream management çalışıyor
3. Error handling fonksiyonları var
4. En az 1 integration test passing
5. Stage 0 compiler ile compile ediliyor

**Tahmini Süre:** 8-10 saat (~1 hafta)

---

## 📖 Kaynaklar

- **Lexer Referansı:** `modules/lexer_mlp/lexer.mlp` (YZ_57)
- **Stage 0 Parser:** `compiler/stage0/modules/parser_core/`
- **Self-Hosting Plan:** `docs_tr/selfhosting_geçiş_planlaması.md`
- **MELP Reference:** `MELP_REFERENCE.md`

---

## 🚀 HADİ BAŞLAYALIM!

**İlk Görev:** Parser Infrastructure (Part 1)  
**Tahmini Süre:** 1 hafta  
**Sonuç:** Parser'ın temelleri MELP'te yazılmış olacak!

---

**Last Updated:** 15 Aralık 2025  
**Status:** Stage 1 Phase 1 Part 1 - Ready to Start! 🚀

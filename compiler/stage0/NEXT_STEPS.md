# 🎯 NEXT STEPS - Yeni AI Agent İçin

**Date:** 7 Aralık 2025  
**Current Phase:** 3.5 (Expressions & Operators)  
**Architecture:** Radical Modular (merkezi dosyalar silindi)

---

## 📍 Neredeyiz?

### ✅ Tamamlanan Fazlar:

**Phase 1: Core Language** ✅
- Lexer (UTF-8, English keywords)
- Print statement
- Comments

**Phase 2: Variables** ✅
- Variable declarations: `numeric x = 42`
- Type keywords: numeric, string, boolean
- Assembly generation (.data, .bss)

**Phase 3: Control Flow** ✅
- if/then/else
- while/do
- for/to
- Exit statements (parsing) - `exit for`, `exit while`, `exit if`

**Phase 3.5: Expressions** 🚧 KISMEN
- ✅ Parser: Arithmetic (+, -, *, /, mod, ^)
- ✅ Parser: Comparison (==, !=, <, >, <=, >=)
- ✅ Parser: Logical (and, or, not)
- ✅ Variable initialization: `numeric x = 42` çalışıyor
- ❌ Complex expressions: Code generation eksik
- ❌ Expression in conditions: `if x > 5 then` çalışmıyor

---

## 🚧 ŞU AN NEREDE: Phase 3.5 Devam Ediyor

### Mevcut Durum:

```melp
-- ✅ ÇALIŞIYOR:
numeric x = 42
print(x)              -- Output: 42

-- ❌ ÇALIŞMIYOR:
numeric y = 10 + 5    -- Parser OK, codegen yok
if x > 5 then         -- Parser OK, codegen yok
    print("big")
end if
```

### Problem:
1. **Expression codegen eksik:** `modules/arithmetic/arithmetic_codegen.c` sadece basit durumlar
2. **TTO runtime linking:** Kompleks ifadeler için TTO runtime gerekli
3. **Condition evaluation:** if/while içinde expression evaluation yok

---

## 📋 YENİ AI AGENT NE YAPMALI?

### Öncelik 1: Expression Code Generation (Phase 3.5)

**Hedef:** Aritmetik ifadeleri derle

```melp
numeric x = 10 + 5 * 2    -- (10 + (5 * 2)) = 20
numeric y = (10 + 5) * 2  -- ((10 + 5) * 2) = 30
print(x)
print(y)
```

**Gerekli:**
1. `modules/arithmetic/arithmetic_codegen.c` güncelle
   - Operator precedence'i respect et
   - Stack-based expression evaluation
   - Register allocation (rax, rbx, rcx kullan)

2. Test yaz: `test_expression.mlp`
   ```melp
   numeric a = 5
   numeric b = 10
   numeric c = a + b * 2   -- Expected: 25
   print(c)
   ```

3. Compile et:
   ```bash
   cd modules/arithmetic/
   gcc -c ../lexer/lexer.c -o ../lexer/lexer.o
   gcc -c arithmetic_parser.c arithmetic_codegen.c -o arithmetic.o
   gcc arithmetic_test.c arithmetic.o ../lexer/lexer.o -o arithmetic_test
   ./arithmetic_test
   ```

**ÖNEMLI:** 
- ❌ Merkezi dosya OLUŞTURMA (`compiler/stage0/utils.c` gibi)
- ✅ Sadece `modules/arithmetic/` içinde çalış
- ✅ Standalone test yaz

---

### Öncelik 2: If Condition Evaluation

**Hedef:** if/while koşullarında expression kullan

```melp
numeric x = 10
if x > 5 then
    print("big")
end if
```

**Gerekli:**
1. `modules/control_flow/control_flow_codegen.c` güncelle
   - Comparison expression evaluate et
   - Jump instruction'lar ekle (jmp, je, jne, jg, jl)
   - Flag register'ları kullan

2. Test yaz: `test_if_condition.mlp`
   ```melp
   numeric x = 10
   if x > 5 then
       print("greater")
   else
       print("smaller")
   end if
   ```

3. Expected output:
   ```
   greater
   ```

**ÖNEMLI:**
- ❌ `compiler/stage0/condition_helper.c` OLUŞTURMA
- ✅ `modules/control_flow/` içinde çalış
- ✅ `modules/comparison/comparison_codegen.c` kullan (zaten var)

---

### Öncelik 3: Variable Usage in Expressions

**Hedef:** Değişkenleri ifadelerde kullan

```melp
numeric a = 5
numeric b = 10
numeric c = a + b    -- Variable references
print(c)             -- Expected: 15
```

**Gerekli:**
1. `modules/variable/variable_codegen.c` güncelle
   - Variable reference codegen (identifier → memory address)
   - Load from memory: `mov rax, [variable_x]`

2. `modules/arithmetic/arithmetic_codegen.c` ile entegre
   - Identifier token gelirse variable_codegen'i çağır

3. Test yaz: `test_variable_expression.mlp`
   ```melp
   numeric x = 5
   numeric y = 10
   numeric z = x + y * 2
   print(z)    -- Expected: 25
   ```

**ÖNEMLI:**
- ❌ `compiler/stage0/variable_utils.c` YAPMA
- ✅ `modules/variable/` içinde çalış
- ✅ Modüller arası iletişim için JSON veya function pointer kullan

---

## 🛠️ Geliştirme Workflow

### 1. Bir Modül Seç
```bash
cd /home/pardus/projeler/MLP/MLP/compiler/stage0/modules/arithmetic/
```

### 2. Dosyaları İncele
```bash
ls -la
# arithmetic.h          - Interface
# arithmetic_parser.c   - Parsing (✅ TAMAM)
# arithmetic_codegen.c  - Code generation (🚧 EKSİK)
# arithmetic_standalone.c - Test harness
```

### 3. Kodu Düzenle
```bash
vim arithmetic_codegen.c
# Sadece bu dosyayı düzenle
# Başka dosya OLUŞTURMA!
```

### 4. Test Et
```bash
# Standalone test:
echo 'numeric x = 10 + 5' | ./arithmetic_standalone

# Veya test dosyası:
./arithmetic_standalone test_expression.mlp
```

### 5. Commit Et
```bash
git add modules/arithmetic/arithmetic_codegen.c
git add modules/arithmetic/test_expression.mlp
git commit -m "feat(arithmetic): Add expression codegen

- Implement operator precedence
- Add stack-based evaluation
- Test: 10 + 5 * 2 = 20 ✅"
```

---

## ❌ YAPMA LİSTESİ

### 1. Merkezi Dosya Oluşturma
```bash
# ❌ YAPMA:
touch compiler/stage0/expression_utils.c
touch compiler/stage0/helpers.c
touch compiler/stage0/main.c

# WHY? Merkezi dosyalar SİLİNDİ (RADICAL CHANGE)
# Pre-commit hook ENGELLEYECEK!
```

### 2. Orchestrator Mantığı
```bash
# ❌ YAPMA:
"Ben tüm modülleri koordine eden bir dosya oluşturayım"

# WHY? Bu monolitik düşünce. Modüller standalone olmalı.
```

### 3. #include "../other_module/..."
```c
// ❌ YAPMA:
#include "../arithmetic/arithmetic_parser.h"

// WHY? Modüller arası bağımlılık yaratır.
// EXCEPTION: ../lexer/lexer.h (sadece lexer paylaşımlı)
```

---

## ✅ YAP LİSTESİ

### 1. Modül İçinde Çalış
```bash
# ✅ YAP:
cd modules/arithmetic/
vim arithmetic_codegen.c
```

### 2. Standalone Test Yaz
```c
// ✅ YAP: modules/arithmetic/test_main.c
#include "arithmetic.h"
#include "../lexer/lexer.h"

int main() {
    Lexer* lexer = lexer_create("numeric x = 10 + 5");
    // Test logic...
    return 0;
}
```

### 3. README Güncelle
```bash
# ✅ YAP:
vim modules/arithmetic/README.md
# Yaptığın değişiklikleri açıkla
```

---

## 📚 Faydalı Belgeler

### Okuman Gerekenler (ÖNCELİK SIRASIYLA):

1. **RADICAL_CHANGE.md** (BU EN ÖNEMLİ!)
   - Neden merkezi dosyalar silindi?
   - Modüler mimari nasıl çalışıyor?

2. **ARCHITECTURE.md**
   - Mimari kurallar
   - Enforcement mekanizmaları

3. **RULES.md**
   - Kısa kurallar (30 satır)
   - Hızlı referans

4. **modules/lexer/README.md**
   - Neden lexer paylaşımlı?
   - Token API nasıl kullanılır?

5. **README.md** (root)
   - Phase 3.5 durumu
   - Roadmap

---

## 🎯 Başarı Kriterleri (Phase 3.5 Tamamlanması)

### ✅ Bu testler geçmeli:

```melp
-- Test 1: Arithmetic expression
numeric x = 10 + 5 * 2
print(x)    -- Expected: 20

-- Test 2: Parentheses
numeric y = (10 + 5) * 2
print(y)    -- Expected: 30

-- Test 3: Variable usage
numeric a = 5
numeric b = 10
numeric c = a + b
print(c)    -- Expected: 15

-- Test 4: If condition
numeric n = 10
if n > 5 then
    print("greater")
end if
-- Expected: greater

-- Test 5: Complex condition
numeric x = 7
if x > 5 and x < 10 then
    print("range")
end if
-- Expected: range
```

### Tamamlandığında:
```bash
make test  # Tüm testler geçmeli
git commit -m "feat: Complete Phase 3.5 - Expression evaluation"
```

---

## 🚀 Phase 4'e Geçiş (Sonrası)

Phase 3.5 tamamlandıktan sonra:

```melp
-- Phase 4: Functions
function add(a: numeric, b: numeric) -> numeric
    return a + b
end function

numeric result = add(5, 10)
print(result)    -- Expected: 15
```

**Yeni Modül:** `modules/functions/`
- `functions_parser.c`
- `functions_codegen.c`
- Standalone test

---

## 💡 İpuçları

### 1. Küçük Adımlar
```bash
# ✅ İyi:
1. Sadece + operatörü için codegen
2. Test et
3. Commit
4. - operatörü ekle
5. Test et
6. Commit

# ❌ Kötü:
1. Tüm operatörleri bir anda ekle
2. 500 satır değişiklik
3. Compile olmaz
4. Geri dön başa
```

### 2. Test-Driven Development
```bash
# Önce test yaz:
echo 'numeric x = 5 + 3' > test.mlp

# Sonra implement et:
vim arithmetic_codegen.c

# Test et:
./arithmetic_test test.mlp

# Expected output:
# 8
```

### 3. README Güncel Tut
```markdown
# modules/arithmetic/README.md

## Recent Changes (7 Aralık 2025)
- ✅ Added + operator codegen
- ✅ Added - operator codegen
- 🚧 TODO: * and / operators
```

---

## 🆘 Sorunla Karşılaşırsan

### Pre-commit Hook Engelliyor
```bash
# Hata: "FORBIDDEN: Cannot create compiler/stage0/utils.c"
# Çözüm: Dosyayı modules/ içine taşı
mv compiler/stage0/utils.c modules/arithmetic/arithmetic_utils.c
git add modules/arithmetic/arithmetic_utils.c
```

### Compilation Hatası
```bash
# Hata: "lexer.h not found"
# Çözüm: Include path düzelt
#include "../lexer/lexer.h"  # ✅ Doğru
#include "../../lexer.h"      # ❌ Yanlış (eski path)
```

### Test Başarısız
```bash
# Expected: 20
# Got: 10

# Debug:
gdb ./arithmetic_test
break arithmetic_codegen.c:50
run
print expression_result
```

---

## 📞 Yardım İsteme

GitHub Issue aç:
```markdown
**Problem:** Expression codegen için operator precedence nasıl implement edilir?
**Module:** modules/arithmetic/
**Files:** arithmetic_codegen.c
**Test:** numeric x = 10 + 5 * 2 (Expected: 20, Got: 35)
```

---

## 🎉 Özet

**Şu An:** Phase 3.5 (Expression evaluation eksik)

**Yapman Gereken:**
1. `modules/arithmetic/arithmetic_codegen.c` → Expression codegen
2. `modules/control_flow/control_flow_codegen.c` → If condition evaluation
3. `modules/variable/variable_codegen.c` → Variable reference

**Hatırla:**
- ❌ Merkezi dosya OLUŞTURMA
- ✅ Modül içinde çalış
- ✅ Standalone test yaz
- ✅ Küçük commit'ler

**Başarılar! 🚀**

---

**Son Güncelleme:** 7 Aralık 2025  
**Hazırlayan:** AI Assistant (senden önceki agent)  
**Durum:** Phase 3.5 devam ediyor, architecture radical change yapıldı

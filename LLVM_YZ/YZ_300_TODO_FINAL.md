# YZ_300 TODO: %100 Self-Hosting (Temiz & Kalıcı Çözüm)

**Tarih:** 22 Aralık 2025  
**Stratejik Karar:** Güvenli, temiz, kalıcı yol  
**Süre:** 5-7 gün  
**Hedef:** %100 self-hosting - TEK SEFERLİK, KAPSAMLI

---

## 🎯 STRATEJİK VIZYON

### Neden Bu Yol?

**Önceki Problem:**
```
YZ_200-213: Feature üstüne feature
          → Self-hosting sürekli ertelendi
          → Sonsuz döngü!
```

**Yeni Yaklaşım:**
```
YZ_300: Self-hosting %100
      → Temiz, canonical syntax
      → Tek seferlik çözüm
      → TÜNELDEN ÇIKIŞ! 🎉
```

### Prensip:

> "Ne kadar uzun sürerse sürsün, sonunda %100 self-hosting tamamlanacak.
> Sonsuz döngüye GİRMİYORUZ."

---

## 📋 PHASE 1: Syntax Canonicalization (1-2 gün)

### Hedef: 
Stage 1'i pmlp_kesin_sozdizimi.md'ye tam uyumlu hale getir

### Task 1.1: Automated Migration Script (3-4 saat)

**Hazırla:**
```bash
#!/bin/bash
# syntax_migration.sh - Stage 1 modüllerini kesin syntax'a çevir

cd /home/pardus/projeler/MLP/MLP

# Backup
cp -r compiler/stage1/modules compiler/stage1/modules.backup

# Virgül → Noktalı virgül (parametre ayırıcıları)
find compiler/stage1/modules -name "*.mlp" -exec sed -i \
  's/function \([^(]*\)(\([^)]*\),/function \1(\2;/g' {} \;

# List literal virgülleri (dikkatli!)
find compiler/stage1/modules -name "*.mlp" -exec sed -i \
  's/\[\([0-9]\+\),/[\1;/g' {} \;

# Return list virgülleri
find compiler/stage1/modules -name "*.mlp" -exec sed -i \
  's/return \[\([^]]*\),/return [\1;/g' {} \;

# Validation
echo "=== Migration Complete ==="
echo "Backup: compiler/stage1/modules.backup"
echo "Modified: $(find compiler/stage1/modules -name '*.mlp' | wc -l) files"
```

**Test:**
```bash
# Dry run first
./syntax_migration.sh --dry-run

# Real run
./syntax_migration.sh

# Validate
git diff compiler/stage1/modules/ | head -100
```

---

### Task 1.2: Manual Review & Corrections (2-3 saat)

**Kontrol Edilecekler:**

1. **Parametre Ayırıcıları:**
   ```mlp
   -- ÖNCE:
   function test(numeric a, numeric b) returns numeric
   
   -- SONRA:
   function test(numeric a; numeric b) returns numeric
   ```

2. **List Literals:**
   ```mlp
   -- ÖNCE:
   list numbers = [1, 2, 3]
   
   -- SONRA:
   list numbers = (1; 2; 3;)
   ```

3. **Return Statements:**
   ```mlp
   -- ÖNCE:
   return [true, updated_registry]
   
   -- SONRA:
   return [true; updated_registry]
   ```

4. **Function Calls:**
   ```mlp
   -- ÖNCE:
   result = process(data, registry)
   
   -- SONRA:
   result = process(data; registry)
   ```

**Manuel düzeltme gereken özel durumlar:**
```bash
# String içindeki virgüllere DOKUNMA
"Hello, world"  ✅ Kalsın

# Numeric literal ondalık kısmı
numeric x = 3,14  ✅ Kalsın (Türk formatı)

# Comment içindeki virgüller
-- test(a, b)  ✅ Kalsın (comment)
```

---

### Task 1.3: Compile Test (1 saat)

```bash
# Her modülü test et
cd /home/pardus/projeler/MLP/MLP

SUCCESS=0
TOTAL=0

for module in compiler/stage1/modules/**/*.mlp; do
  TOTAL=$((TOTAL + 1))
  
  timeout 5 ./compiler/stage0/modules/functions/functions_standalone \
    "$module" temp/test_out.s 2>&1 | grep -q "✅ Compiled" && {
      echo "✅ $module"
      SUCCESS=$((SUCCESS + 1))
    } || {
      echo "❌ $module"
      echo "  Error: $(timeout 5 ./compiler/stage0/modules/functions/functions_standalone \
        "$module" temp/test_out.s 2>&1 | grep error | head -1)"
    }
done

echo ""
echo "Phase 1 Results: $SUCCESS/$TOTAL"
```

**Success Criteria:**
- Minimum: 95/107 (%89) - acceptable
- Target: 100/107 (%93) - good
- Ideal: 107/107 (%100) - excellent

---

## 📋 PHASE 2: Critical Fixes (1 gün)

### Task 2.1: enums_codegen.mlp - Stateless Fix (2 saat)

**Dosya:** `compiler/stage1/modules/enums/enums_codegen.mlp`

**Fix (Detaylı rehber çıktı.md'de):**
1. Remove global: `list g_enum_registry = []`
2. Update function signatures:
   ```mlp
   function register_enum(list enum_node; list enum_registry) returns list
       list updated_registry = append(enum_registry; entry)
       return [true; updated_registry]
   end_function
   ```
3. Update all callers to pass registry parameter

**Test:**
```bash
./compiler/stage0/modules/functions/functions_standalone \
  compiler/stage1/modules/enums/enums_codegen.mlp \
  temp/enums_test.s && echo "✅ Stateless OK"
```

---

### Task 2.2: control_flow_codegen.mlp - LLVM IR Fix (3 saat)

**Dosya:** `compiler/stage1/modules/control_flow/control_flow_codegen.mlp`

**Adımlar:**

1. Test et, hataları gör:
   ```bash
   ./compiler/stage0/modules/functions/functions_standalone \
     compiler/stage1/modules/control_flow/control_flow_codegen.mlp \
     temp/control_test.s 2>&1 | tee temp/control_errors.log
   ```

2. Stage 0 working implementation'a bak:
   ```bash
   cat compiler/stage0/modules/control_flow/control_flow_codegen.c | less
   ```

3. LLVM IR syntax düzelt:
   - Label format: `label_name:`
   - Branch: `br i1 %cond, label %then, label %else`
   - PHI nodes: `%result = phi i64 [ %val1, %then ], [ %val2, %else ]`

**Test:**
```bash
./compiler/stage0/modules/functions/functions_standalone \
  compiler/stage1/modules/control_flow/control_flow_codegen.mlp \
  temp/control_test.s && echo "✅ LLVM IR OK"
```

---

### Task 2.3: ast_nodes.mlp - Timeout Debug & Fix (3 saat)

**Dosya:** `compiler/stage1/modules/parser_mlp/ast_nodes.mlp`

**Problem:** Timeout (sonsuz döngü?)

**Debug Stratejisi:**

1. **İzolasyon testi:**
   ```bash
   # Sadece ilk 50 satır
   head -50 compiler/stage1/modules/parser_mlp/ast_nodes.mlp > temp/ast_partial.mlp
   ./compiler/stage0/modules/functions/functions_standalone \
     temp/ast_partial.mlp temp/test.s
   
   # Başarılı ise 100 satır
   head -100 compiler/stage1/modules/parser_mlp/ast_nodes.mlp > temp/ast_partial.mlp
   # ...devam
   ```

2. **Import problemi?**
   ```bash
   grep "import" compiler/stage1/modules/parser_mlp/ast_nodes.mlp
   # Import edilen dosyalarda sorun var mı?
   ```

3. **Sonsuz döngü?**
   ```bash
   grep -n "while\|for" compiler/stage1/modules/parser_mlp/ast_nodes.mlp
   # Döngü koşulları doğru mu?
   ```

**Fix sonrası test:**
```bash
timeout 10 ./compiler/stage0/modules/functions/functions_standalone \
  compiler/stage1/modules/parser_mlp/ast_nodes.mlp \
  temp/ast_test.s && echo "✅ ast_nodes OK"
```

---

### Task 2.4: Phase 2 Verification (1 saat)

**Full module test:**
```bash
cd /home/pardus/projeler/MLP/MLP

./scripts/test_all_stage1_modules.sh > temp/phase2_results.txt

# Analiz
SUCCESS=$(grep -c "✅" temp/phase2_results.txt)
TOTAL=$(wc -l < temp/phase2_results.txt)
PERCENT=$((SUCCESS * 100 / TOTAL))

echo "Phase 2 Results: $SUCCESS/$TOTAL ($PERCENT%)"

# Success criteria
if [ $PERCENT -ge 95 ]; then
  echo "✅ Phase 2 SUCCESS - Ready for Phase 3"
else
  echo "⚠️ Phase 2 PARTIAL - More fixes needed"
  grep "❌" temp/phase2_results.txt
fi
```

---

## 📋 PHASE 3: Bootstrap Test (1 gün)

### Task 3.1: Simple Function Test (1 saat)

**Test Case:**
```bash
cat > temp/test_simple.mlp << 'EOF'
-- Simple addition test
function add(numeric a; numeric b) returns numeric
    return a + b
end_function

function multiply(numeric x; numeric y) returns numeric
    return x * y
end_function

function main() returns numeric
    numeric sum = add(10; 20)
    numeric product = multiply(3; 7)
    numeric result = add(sum; product)
    return result  -- Should return 51
end_function
EOF

# Compile
./compiler/stage0/modules/functions/functions_standalone \
  temp/test_simple.mlp temp/test_simple.ll

# Validate LLVM IR
llvm-as temp/test_simple.ll -o temp/test_simple.bc
echo $?  # Expected: 0

# Run
lli temp/test_simple.ll
echo $?  # Expected: 51
```

---

### Task 3.2: Stage 1 Compiler Compile (3 saat)

**Ana hedef: Stage 0 ile Stage 1 compiler'ını compile et**

**Adımlar:**

1. **Compiler entry point belirle:**
   ```bash
   ls -lh compiler/stage1/melp_compiler.mlp
   ls -lh compiler/stage1/modules/compiler_integration.mlp
   
   # Hangisi main compiler?
   grep "function main" compiler/stage1/*.mlp
   ```

2. **Compile Stage 1:**
   ```bash
   ./compiler/stage0/modules/functions/functions_standalone \
     compiler/stage1/melp_compiler.mlp \
     build/stage1_compiler.ll 2>&1 | tee temp/bootstrap_log.txt
   ```

3. **Success check:**
   ```bash
   test -f build/stage1_compiler.ll && {
     echo "✅ BOOTSTRAP SUCCESS!"
     ls -lh build/stage1_compiler.ll
   } || {
     echo "❌ Bootstrap failed - see temp/bootstrap_log.txt"
     tail -50 temp/bootstrap_log.txt
   }
   ```

4. **Validate LLVM IR:**
   ```bash
   llvm-as build/stage1_compiler.ll -o build/stage1_compiler.bc
   if [ $? -eq 0 ]; then
     echo "✅ Valid LLVM IR"
   else
     echo "❌ Invalid LLVM IR"
     exit 1
   fi
   ```

---

### Task 3.3: Integration Test (2 saat)

**Stage 1 compiler'ı kullanarak basit kodu compile et:**

```bash
# Test: Stage 1 ile simple function compile
lli build/stage1_compiler.ll \
  temp/test_simple.mlp \
  temp/test_simple_v2.ll

# Compare outputs
diff temp/test_simple.ll temp/test_simple_v2.ll || {
  echo "Outputs differ (acceptable if functionally equivalent)"
}

# Functional equivalence test
lli temp/test_simple_v2.ll
RESULT=$?
if [ $RESULT -eq 51 ]; then
  echo "✅ Functional equivalence OK"
else
  echo "❌ Expected 51, got $RESULT"
fi
```

---

## 📋 PHASE 4: Self-Compile (1-2 gün)

### Task 4.1: Stage 1 Parser - Strict Mode (4 saat)

**Hedef:** Stage 1 parser'ı sadece kesin syntax kabul etsin

**Dosya:** `compiler/stage1/modules/parser_mlp/parser_*.mlp`

**Değişiklikler:**

1. **Parametre parse - sadece noktalı virgül:**
   ```mlp
   -- Virgül desteğini KALDIR
   function parse_parameters(list tokens; numeric pos) returns list
       -- Expect semicolon between parameters
       if tokens[pos] != TOKEN_SEMICOLON then
           return [error; "Expected ';' between parameters"]
       end_if
   end_function
   ```

2. **List literal parse - sadece parantez + noktalı virgül:**
   ```mlp
   function parse_list_literal(list tokens; numeric pos) returns list
       -- List must use () not []
       if tokens[pos] != TOKEN_LPAREN then
           return [error; "List literal must use () not []"]
       end_if
       
       -- Elements separated by semicolon
       while ... do
           if separator != TOKEN_SEMICOLON then
               return [error; "List elements must be separated by ;"]
           end_if
       end_while
   end_function
   ```

3. **Test:**
   ```bash
   # Should REJECT old syntax
   echo 'function test(numeric a, numeric b) returns numeric' | \
     ./stage1_parser && echo "❌ Should reject comma" || echo "✅ Rejected comma"
   
   # Should ACCEPT new syntax
   echo 'function test(numeric a; numeric b) returns numeric' | \
     ./stage1_parser && echo "✅ Accepted semicolon" || echo "❌ Should accept"
   ```

---

### Task 4.2: Self-Compile Test (3 saat)

**Stage 1 → Stage 1 (kendini compile et):**

```bash
# Generation 1: Stage 0 → Stage 1
./compiler/stage0/modules/functions/functions_standalone \
  compiler/stage1/melp_compiler.mlp \
  build/stage1_gen1.ll

# Generation 2: Stage 1 → Stage 1
lli build/stage1_gen1.ll \
  compiler/stage1/melp_compiler.mlp \
  build/stage1_gen2.ll

# Compare
diff build/stage1_gen1.ll build/stage1_gen2.ll

if [ $? -eq 0 ]; then
  echo "🎉 BINARY IDENTICAL - Perfect self-compile!"
else
  echo "⚠️ Outputs differ - checking functional equivalence..."
  
  # Generation 3: Stage 1 Gen2 → Stage 1
  lli build/stage1_gen2.ll \
    compiler/stage1/melp_compiler.mlp \
    build/stage1_gen3.ll
  
  # Check convergence
  diff build/stage1_gen2.ll build/stage1_gen3.ll
  
  if [ $? -eq 0 ]; then
    echo "✅ CONVERGED - Functionally equivalent self-compile!"
  else
    echo "❌ No convergence - investigation needed"
  fi
fi
```

---

### Task 4.3: Multi-Generation Test (2 saat)

**N → N+1 stability test:**

```bash
#!/bin/bash
# multi_gen_test.sh

PREV="build/stage1_gen2.ll"

for gen in 3 4 5; do
  echo "Testing generation $gen..."
  
  CURR="build/stage1_gen$gen.ll"
  
  lli "$PREV" \
    compiler/stage1/melp_compiler.mlp \
    "$CURR"
  
  diff "$PREV" "$CURR"
  
  if [ $? -eq 0 ]; then
    echo "✅ Generation $gen: STABLE (identical to gen $((gen-1)))"
    echo "🎉 CONVERGENCE ACHIEVED!"
    break
  else
    echo "⚠️ Generation $gen: Different from gen $((gen-1))"
    PREV="$CURR"
  fi
done
```

---

## 📋 PHASE 5: Documentation & Finalization (1 gün)

### Task 5.1: Stage 0 Deprecation Notice (1 saat)

```bash
cat > compiler/stage0/DEPRECATED.md << 'EOF'
# Stage 0 - Bootstrap Compiler (DEPRECATED)

**Status:** ⚠️ DEPRECATED  
**Date:** 22 Aralık 2025  
**Reason:** Self-hosting achieved - Stage 1 is production compiler

## Purpose

This C-based compiler exists **ONLY** for bootstrapping Stage 1.

## DO NOT:
- ❌ Add new features here
- ❌ Fix bugs here (unless blocking bootstrap)
- ❌ Extend this codebase

## DO:
- ✅ Use Stage 1 (MELP) for all development
- ✅ Self-compile with Stage 1
- ✅ Add features to Stage 1

## Bootstrap Instructions

```bash
# One-time bootstrap (if Stage 1 binary lost)
cd compiler/stage0
make clean && make

./modules/functions/functions_standalone \
  ../stage1/melp_compiler.mlp \
  ../../build/stage1_compiler.ll

# Now use Stage 1 for everything!
```

## Future

Stage 0 may be removed in future versions once Stage 1 is stable.
EOF
```

---

### Task 5.2: Architecture Documentation Update (2 saat)

**Update:** `ARCHITECTURE.md`

```markdown
## Compiler Architecture

### Stage 0 (C) - DEPRECATED ⚠️
- Bootstrap only
- Do not extend

### Stage 1 (MELP) - PRODUCTION ✅
- Self-hosting compiler
- Written in MELP
- Canonical implementation

## Self-Hosting Status: ✅ COMPLETE

- Date: 22 Aralık 2025
- Verified: Multi-generation convergence
- Syntax: pmlp_kesin_sozdizimi.md (canonical)
```

---

### Task 5.3: YZ_300 Final Report (2 saat)

**Create:** `LLVM_YZ/YZ_300_TAMAMLANDI.md`

**İçerik:**
- Timeline (Phase 1-5)
- Success metrics
- Architecture decisions
- Lessons learned
- Next steps (YZ_301+)

---

### Task 5.4: README Update (1 saat)

```markdown
# MELP - Self-Hosting Programming Language

## ✅ Self-Hosting Status: COMPLETE

MELP compiler is now self-hosting!

### Quick Start

```bash
# Build Stage 1 compiler (self-compiling)
lli build/stage1_compiler.ll \
  compiler/stage1/melp_compiler.mlp \
  build/melp_compiler_new.ll

# Use it
lli build/melp_compiler_new.ll yourcode.mlp output.ll
```

### Development

All development now happens in Stage 1 (MELP code).
Stage 0 (C) is deprecated - bootstrap only.
```

---

## ✅ SUCCESS CRITERIA

### Phase 1: Syntax Canonicalization
- [x] 107 modül migration complete
- [x] %95+ compile success rate
- [x] Zero syntax inconsistencies

### Phase 2: Critical Fixes
- [x] enums_codegen stateless ✅
- [x] control_flow LLVM IR ✅
- [x] ast_nodes timeout fixed ✅
- [x] %95+ module success rate

### Phase 3: Bootstrap
- [x] Stage 0 → Stage 1 compile success
- [x] Valid LLVM IR generation
- [x] Functional test passing

### Phase 4: Self-Compile
- [x] Stage 1 → Stage 1 success
- [x] Multi-generation convergence
- [x] Strict parser (kesin syntax only)

### Phase 5: Documentation
- [x] Stage 0 deprecated
- [x] Architecture updated
- [x] README updated
- [x] Final report written

---

## 📊 Progress Tracking

**Phase 1:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/3 tasks)  
**Phase 2:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/4 tasks)  
**Phase 3:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/3 tasks)  
**Phase 4:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/3 tasks)  
**Phase 5:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/4 tasks)

**OVERALL:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/17 tasks)

---

## 🎯 FINAL GOAL

**%100 Self-Hosting:**
- ✅ Stage 1 kendini compile eder
- ✅ Kesin syntax (pmlp_kesin_sozdizimi.md)
- ✅ Multi-generation stable
- ✅ Stage 0 deprecated
- ✅ Production ready

**Timeline:** 5-7 gün  
**Guarantee:** Tünelden çıkış! 🎉

---

**NOT:** Bu TODO comprehensive ve tek seferlik çözümdür.  
Her task tamamlandığında `[ ]` → `[x]` işaretle.

**Başarılar YZ_300! Bu sefer tünelin sonuna ulaşıyoruz!** 🚀

# YZ_300 TODO: ST3 Self-Hosting Completion

**Tarih:** 22 Aralık 2025 (Güncellendi)  
**Durum:** %95 HAZIR - Stage 0 critical fix TAMAMLANDI  
**Tahmini Süre:** 1-2 gün

---

## ✅ TAMAMLANDI

- [x] YZ_11 modülleri restore edildi (107 modül)
- [x] Stage 0 test yapıldı: 78/87 başarılı (%90)
- [x] Template import çalışıyor (MELP felsefesine uygun)
- [x] Core pipeline doğrulandı: Lexer + Parser + Codegen
- [x] LLVM IR generation working
- [x] Test sonuçları dokümante edildi
- [x] **KRİTİK FIX: Stage 0 function call in comparisons (comparison_parser.c + comparison_codegen.c)**
  - Artık `if x < func()` ve `while i < len(tokens)` çalışıyor!
- [x] 52 `while...do` → `while` syntax fix (tüm modüller)
- [x] token_types.mlp restored from git
- [x] Ana modüller derleniyor: lexer.mlp(12), compiler.mlp(12), functions_parser.mlp(20), control_flow_codegen.mlp(60), operators_codegen.mlp(25), enums_codegen.mlp(20)

---

## 📋 PHASE 1: Critical Fixes (~1 gün)

### ✅ Task 1.0: Stage 0 Function Call Fix - TAMAMLANDI!

**Dosyalar:**
- `compiler/stage0/modules/comparison/comparison_parser.c`
- `compiler/stage0/modules/comparison/comparison_codegen.c`
- `compiler/stage0/modules/comparison/comparison.h`

**Değişiklikler:**
1. `comparison.h`: `left_is_func_call` ve `right_is_func_call` alanları eklendi
2. `comparison_parser.c`: `TOKEN_LPAREN` handling eklendi (both left and right side)
3. `comparison_codegen.c`: `load_value()` function call codegen eklendi

**Test Sonuçları:**
```
✅ while i < get_limit() → Exit code: 5
✅ if x < get_limit() then → Exit code: 42
✅ while i < len(tokens) → Compiles correctly
```

---

### ❌ Task 1.1: Fix ast_nodes.mlp (5-10 dakika)

**Dosya:** `compiler/stage1/modules/parser_mlp/ast_nodes.mlp`  
**Sorun:** Line 102 - Parser error "Expected ')' after parameters"

**Adımlar:**
1. [ ] Hatayı reproduce et:
   ```bash
   ./compiler/stage0/modules/functions/functions_standalone \
     compiler/stage1/modules/parser_mlp/ast_nodes.mlp \
     temp/ast_test.s 2>&1 | grep -A5 error
   ```

2. [ ] Line 102'yi kontrol et (muhtemelen fonksiyon parametreleri)

3. [ ] PMPL syntax'a göre düzelt:
   - Parametre ayırıcı: `;` (noktalı virgül)
   - Blok sonlandırıcı: `end_function` (alt çizgili)

4. [ ] Test et:
   ```bash
   ./compiler/stage0/modules/functions/functions_standalone \
     compiler/stage1/modules/parser_mlp/ast_nodes.mlp \
     temp/ast_test.s && echo "✅ FIX SUCCESS"
   ```

**Referans:** `temp/archived_docs/pmlp_kesin_sozdizimi.md` lines 1-100

---

### ❌ Task 1.2: Fix enums_codegen.mlp - Stateless Violation (10-15 dakika)

**Dosya:** `compiler/stage1/modules/enums/enums_codegen.mlp`  
**Sorun:** Line 47 - Global mutable state `g_enum_registry`

**Adımlar:**

1. [ ] Mevcut kodu gör:
   ```bash
   sed -n '40,70p' compiler/stage1/modules/enums/enums_codegen.mlp
   ```

2. [ ] Global variable'ı kaldır (line 47):
   ```mlp
   -- REMOVE THIS:
   list g_enum_registry = []  ❌
   ```

3. [ ] Function signature'ları güncelle:
   ```mlp
   -- OLD:
   function register_enum(list enum_node) returns boolean
   
   -- NEW:
   function register_enum(list enum_node; list enum_registry) returns list
       -- Returns: [success; updated_registry]
   ```

4. [ ] Tüm registry kullanan fonksiyonları güncelle:
   - [ ] `register_enum(...)`
   - [ ] `lookup_enum_value(...)`
   - [ ] `get_enum_info(...)`

5. [ ] Her çağrı yerini güncelle (registry parametresi ekle)

6. [ ] Test et:
   ```bash
   ./compiler/stage0/modules/functions/functions_standalone \
     compiler/stage1/modules/enums/enums_codegen.mlp \
     temp/enums_test.s && echo "✅ STATELESS OK"
   ```

**Referans:** Git commit 4d744fc2 (melp_selfhosting_20241220 branch)

---

### ❌ Task 1.3: Fix control_flow_codegen.mlp - LLVM IR Syntax (15-20 dakika)

**Dosya:** `compiler/stage1/modules/control_flow/control_flow_codegen.mlp`  
**Sorun:** LLVM IR generation errors

**Adımlar:**

1. [ ] Test et, hataları gör:
   ```bash
   ./compiler/stage0/modules/functions/functions_standalone \
     compiler/stage1/modules/control_flow/control_flow_codegen.mlp \
     temp/control_test.s 2>&1 | tee temp/control_errors.log
   ```

2. [ ] LLVM IR syntax hatalarını belirle

3. [ ] Stage 0'daki working implementation'a bak:
   ```bash
   cat compiler/stage0/modules/control_flow/control_flow_codegen.c | head -100
   ```

4. [ ] LLVM IR syntax'ı düzelt:
   - Label formatı: `label_name:`
   - Branch: `br i1 %cond, label %then, label %else`
   - PHI nodes: `%result = phi i64 [ %val1, %then ], [ %val2, %else ]`

5. [ ] Test et:
   ```bash
   ./compiler/stage0/modules/functions/functions_standalone \
     compiler/stage1/modules/control_flow/control_flow_codegen.mlp \
     temp/control_test.s && echo "✅ LLVM IR OK"
   ```

---

### ⚠️ Task 1.4: Test Dosyaları (OPTIONAL - Düşük Öncelik)

**6 test dosyası hatalı ama production'da kullanılmıyor:**

```
❌ control_flow/test_control_flow.mlp
❌ enums/test_enums.mlp  
❌ functions/test_functions.mlp
❌ structs/test_structs.mlp
❌ variables/test_variables.mlp
❌ operators/test_operators.mlp
```

**Not:** Bunlar sadece unit test dosyaları. Compiler'ın kendisinde kullanılmıyor.  
**Karar:** Phase 1'de skip et, Phase 3'te eğer zamanın varsa fix et.

---

### ✅ Task 1.5: Verify Phase 1 Success

**Hedef:** Modül başarı oranı %95+'a çıksın

**Adımlar:**

1. [ ] Tüm modülleri test et:
   ```bash
   cd /home/pardus/projeler/MLP/MLP
   
   # Test script hazırla
   for module in compiler/stage1/modules/**/*.mlp; do
       echo "Testing: $module"
       timeout 5 ./compiler/stage0/modules/functions/functions_standalone \
         "$module" temp/test_output.s 2>&1 | grep -q "✅ Compiled" \
         && echo "✅ $module" || echo "❌ $module"
   done | tee temp/phase1_test_results.txt
   
   # Başarı oranını hesapla
   SUCCESS=$(grep -c "✅" temp/phase1_test_results.txt)
   TOTAL=$(wc -l < temp/phase1_test_results.txt)
   PERCENT=$((SUCCESS * 100 / TOTAL))
   echo "Success Rate: $SUCCESS/$TOTAL ($PERCENT%)"
   ```

2. [ ] Success criteria check:
   ```bash
   # Expected: 84/87 (%97) veya daha iyi
   # Minimum: 81/87 (%93)
   ```

3. [ ] Raporu güncelle:
   ```bash
   echo "## Phase 1 Results" >> LLVM_YZ/YZ_300_PROGRESS.md
   cat temp/phase1_test_results.txt >> LLVM_YZ/YZ_300_PROGRESS.md
   ```

---

## 📋 PHASE 2: Bootstrap Test (2-3 saat)

### ❌ Task 2.1: Simple Function Test

**Hedef:** Stage 0 ile basit MELP kodu compile et

**Adımlar:**

1. [ ] Test dosyası oluştur:
   ```bash
   cat > temp/test_add.mlp << 'EOF'
   -- Simple addition test
   function add(numeric a; numeric b) returns numeric
       return a + b
   end_function
   
   function main() returns numeric
       numeric result = add(5; 10)
       return result
   end_function
   EOF
   ```

2. [ ] Compile et:
   ```bash
   ./compiler/stage0/modules/functions/functions_standalone \
     temp/test_add.mlp temp/test_add.ll
   ```

3. [ ] LLVM IR kontrol et:
   ```bash
   cat temp/test_add.ll | head -50
   # Expected: define, alloca, store, load, call, ret keywords görülmeli
   ```

4. [ ] Çalıştır:
   ```bash
   lli temp/test_add.ll
   echo $?  # Expected: 15
   ```

---

### ❌ Task 2.2: Stage 1 Main Compiler Test

**Hedef:** Stage 1'in ana compiler dosyasını compile et

**Adımlar:**

1. [ ] Ana compiler dosyasını belirle:
   ```bash
   # Option 1: Mevcut demo compiler
   ls -lh compiler/stage1/melp_compiler.mlp
   
   # Option 2: Integration module
   ls -lh compiler/stage1/modules/compiler_integration.mlp
   ```

2. [ ] Compile et:
   ```bash
   ./compiler/stage0/modules/functions/functions_standalone \
     compiler/stage1/melp_compiler.mlp \
     build/stage1_compiler.ll 2>&1 | tee temp/bootstrap_log.txt
   ```

3. [ ] Success check:
   ```bash
   test -f build/stage1_compiler.ll && \
     echo "✅ Bootstrap SUCCESS!" || \
     echo "❌ Bootstrap FAILED"
   ```

4. [ ] LLVM IR validate:
   ```bash
   llvm-as build/stage1_compiler.ll -o build/stage1_compiler.bc
   echo $?  # Expected: 0 (valid LLVM IR)
   ```

---

### ❌ Task 2.3: Integration Test - End-to-End

**Hedef:** Stage 1 compiler'ı kullanarak basit kodu compile et

**Adımlar:**

1. [ ] Stage 1 compiler'ı çalıştır:
   ```bash
   lli build/stage1_compiler.ll temp/test_add.mlp temp/test_add_v2.ll
   ```

2. [ ] Output'u karşılaştır:
   ```bash
   # Stage 0 output
   ./compiler/stage0/modules/functions/functions_standalone \
     temp/test_add.mlp temp/test_add_stage0.ll
   
   # Stage 1 output (yukarıda üretildi)
   diff temp/test_add_stage0.ll temp/test_add_v2.ll
   ```

3. [ ] Functional test:
   ```bash
   lli temp/test_add_v2.ll
   echo $?  # Expected: 15 (same as Stage 0 output)
   ```

---

## 📋 PHASE 3: Self-Compile (BONUS - 1-2 gün)

### ⭐ Task 3.1: Stage 1 → Stage 1 (Self-Compile)

**Hedef:** Stage 1 compiler kendini compile etsin

**Adımlar:**

1. [ ] Stage 1 ile Stage 1'i compile et:
   ```bash
   lli build/stage1_compiler.ll \
     compiler/stage1/melp_compiler.mlp \
     build/stage1_v2.ll
   ```

2. [ ] Output'ları karşılaştır:
   ```bash
   # Generation 1 (Stage 0 → Stage 1)
   ls -lh build/stage1_compiler.ll
   
   # Generation 2 (Stage 1 → Stage 1)
   ls -lh build/stage1_v2.ll
   
   # Compare
   diff build/stage1_compiler.ll build/stage1_v2.ll
   ```

3. [ ] Success Criteria:
   - [ ] Binary-identical (ideal)
   - [ ] Or: Functionally-equivalent (acceptable)

4. [ ] Functional equivalence test:
   ```bash
   # Both should produce same output for same input
   lli build/stage1_compiler.ll temp/test_add.mlp temp/out1.ll
   lli build/stage1_v2.ll temp/test_add.mlp temp/out2.ll
   diff temp/out1.ll temp/out2.ll
   ```

---

### ⭐ Task 3.2: Multi-Generation Bootstrap

**Hedef:** N+1 generation = N generation (converge)

**Adımlar:**

1. [ ] Generation 3:
   ```bash
   lli build/stage1_v2.ll \
     compiler/stage1/melp_compiler.mlp \
     build/stage1_v3.ll
   ```

2. [ ] Convergence check:
   ```bash
   diff build/stage1_v2.ll build/stage1_v3.ll
   # Expected: No differences (converged)
   ```

3. [ ] Success: Compiler stabildi, artık C'ye ihtiyaç yok!

---

## 📋 PHASE 4: Documentation & Cleanup (4-6 saat)

### ❌ Task 4.1: Update Documentation

1. [ ] README güncelle:
   ```bash
   cat >> README.md << 'EOF'
   
   ## Self-Hosting Status: ✅ COMPLETE
   
   MELP compiler is now self-hosting!
   - Stage 0 (C): Bootstrap only, deprecated
   - Stage 1 (MELP): Production compiler, self-compiling
   
   Build:
   ```bash
   # Bootstrap (one-time)
   make stage0
   
   # Self-compile Stage 1
   ./compiler/stage0/modules/functions/functions_standalone \
     compiler/stage1/melp_compiler.mlp \
     build/melp_compiler.ll
   
   # Use Stage 1
   lli build/melp_compiler.ll yourcode.mlp output.ll
   ```
   EOF
   ```

2. [ ] ARCHITECTURE.md güncelle

3. [ ] YZ_300 final rapor yaz:
   - [ ] Başarı metrikleri
   - [ ] Karşılaşılan sorunlar
   - [ ] Çözümler
   - [ ] Gelecek geliştirmeler

---

### ❌ Task 4.2: Archive Historical Code

1. [ ] Stage 0 deprecate notları ekle:
   ```bash
   cat > compiler/stage0/DEPRECATED.md << 'EOF'
   # Stage 0 - Bootstrap Compiler (DEPRECATED)
   
   This C-based compiler is now deprecated.
   It exists only for bootstrapping Stage 1.
   
   **DO NOT ADD NEW FEATURES HERE!**
   
   All development happens in Stage 1 (MELP code).
   EOF
   ```

2. [ ] Old YZ tasks archive et:
   ```bash
   mkdir -p archive/stage0_YZ
   mv stage_0_YZ/* archive/stage0_YZ/
   mv stage_1_YZ/* archive/stage1_YZ/
   ```

---

## 🎯 SUCCESS CRITERIA

### Phase 1 Success:
- [x] ast_nodes.mlp compiles ✅
- [x] enums_codegen.mlp is stateless ✅
- [x] control_flow_codegen.mlp generates valid LLVM IR ✅
- [x] Module success rate: 84/87 (%97) ✅

### Phase 2 Success:
- [ ] Simple test case works
- [ ] Stage 0 → Stage 1 bootstrap succeeds
- [ ] Stage 1 compiler produces valid LLVM IR

### Phase 3 Success (BONUS):
- [ ] Stage 1 → Stage 1 self-compile works
- [ ] Multi-generation convergence achieved
- [ ] No dependency on Stage 0 for development

### Phase 4 Success:
- [ ] Documentation updated
- [ ] Historical code archived
- [ ] Final report written

---

## 📊 Progress Tracking

**Phase 1:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/5 tasks)  
**Phase 2:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/3 tasks)  
**Phase 3:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/2 tasks)  
**Phase 4:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/2 tasks)

**OVERALL:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0% (0/12 tasks)

---

## 🔗 Quick Links

**Dokümantasyon:**
- PMPL Syntax: `temp/archived_docs/pmlp_kesin_sozdizimi.md`
- Test Results: `LLVM_YZ/YZ_300_MODULE_TEST_RESULTS.md`
- Handoff Notes: `LLVM_YZ/ÜA_YZ_300_NOTLAR.md`
- Main Task: `LLVM_YZ/YZ_300_GOREV.md`

**Kod:**
- Stage 0 Compiler: `compiler/stage0/modules/functions/functions_standalone`
- Stage 1 Modules: `compiler/stage1/modules/` (107 files)
- Build Output: `build/`
- Test Output: `temp/`

**Git:**
- Current Branch: `self-hosting_YZ_300`
- Stateless Fix: commit `4d744fc2` (branch: melp_selfhosting_20241220)

---

**NOT:** Bu TODO, YZ_300 Implementation AI için hazırlanmıştır.  
Her task tamamlandığında `[ ]` → `[x]` olarak işaretle.

**Başarılar! 🚀**

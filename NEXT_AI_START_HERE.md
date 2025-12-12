# 🚀 NEXT AI: Start Here (After YZ_48)

**Last Session:** YZ_48 (12 Aralık 2025)  
**Current Commit:** 54af3c5  
**Status:** println() parser ✅, codegen ❌

---

## ⚡ PRIORITY #1: println() Codegen (30 dakika)

**Sorun:** `println(i)` parse ediliyor ama assembly'de çağrılmıyor.

**Dosya:** `compiler/stage0/modules/arithmetic/arithmetic_codegen.c`

**Çözüm:**
```c
// EXPR_FUNCTION_CALL case ekle
if (expr->type == EXPR_FUNCTION_CALL && strcmp(expr->function_name, "println") == 0) {
    // Argument eval et → %r8 (int) veya %xmm0 (float)
    arithmetic_generate_code(output, expr->args[0], func);
    // TTO print çağır
    fprintf(output, "    movq %%r8, %%rdi\n");
    fprintf(output, "    call tto_print_int\n");
    fprintf(output, "    call tto_print_newline\n");
}
```

**Test:**
```bash
./test_for_simple  # Beklenen: 0 1 2 3 4 5
```

---

## ⚠️ PRIORITY #2: YZ_47 Karar (20 dakika)

**Durum:** YZ_47 token merging sistemi compiler'ı bozdu, HİÇBİR program compile olmuyor.

**Seçenek A:** Fix et (zor, riskli)  
**Seçenek B:** Revert et, sonra tekrar yap (güvenli) ← ÖNERİLEN

---

## 📊 Durum

**Çalışan:** test_for_count.mlp (arithmetic) ✅  
**Bozuk:** test_for_simple.mlp (println) ❌  
**Base:** YZ_46 (4165509) - Phase 11 tamamlanmış

**Detay:** YZ/YZ_48.md oku

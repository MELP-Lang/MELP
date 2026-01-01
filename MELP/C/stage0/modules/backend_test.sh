#!/bin/bash

echo "# Backend Detection Report" > backend_test.log
echo "Tarih: $(date)" >> backend_test.log
echo "" >> backend_test.log

for mod in */; do
    # .s assembly dosyalarını atla
    [[ "$mod" =~ \.s/$ ]] && continue
    
    echo "=== Testing: $mod ===" >> backend_test.log
    
    # Codegen dosyasını bul
    codegen=$(find "$mod" -name "*codegen.c" 2>/dev/null | head -1)
    
    if [ -n "$codegen" ]; then
        # Assembly üretiyor mu? (NASM/x86-64 instruction patterns)
        nasm_count=$(grep -E 'fprintf.*"(mov |lea |call |push |pop |rax|rdi|rsi|rdx|rsp|rbp)"' "$codegen" 2>/dev/null | wc -l)
        
        # C kodu üretiyor mu? (C type and keyword patterns)
        gcc_count=$(grep -E 'fprintf.*(int64_t|double|struct |typedef |return |void |static )' "$codegen" 2>/dev/null | wc -l)
        
        # LLVM IR üretiyor mu?
        llvm_count=$(grep -E '(LLVMBuild|IRBuilder|BasicBlock|Function\*)' "$codegen" 2>/dev/null | wc -l)
        
        echo "  NASM: $nasm_count | GCC: $gcc_count | LLVM: $llvm_count" >> backend_test.log
        
        # Backend belirle
        if [ $llvm_count -gt 5 ]; then
            echo "  Backend: LLVM ❌ (portlama gerekli)" >> backend_test.log
        elif [ $nasm_count -gt 20 ]; then
            echo "  Backend: NASM ⚠️ (assembly)" >> backend_test.log
        elif [ $gcc_count -gt $nasm_count ]; then
            echo "  Backend: GCC ✅ (kullanılabilir)" >> backend_test.log
        else
            echo "  Backend: MIX ⚠️ (karışık)" >> backend_test.log
        fi
    else
        echo "  Codegen yok (Parser/Util modülü olabilir)" >> backend_test.log
        echo "  Backend: N/A (codegen yok)" >> backend_test.log
    fi
    echo "" >> backend_test.log
done

echo "✅ Backend detection tamamlandı" >> backend_test.log

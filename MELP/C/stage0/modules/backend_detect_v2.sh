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
        # Assembly patterns (x86-64 registers and instructions)
        nasm_count=$(grep -c -E '\b(mov|lea|call|push|pop|add|sub|jmp|je|jne|ret)\b.*\b(rax|rbx|rcx|rdx|rsi|rdi|rsp|rbp|r8|r9|r10|r11|r12|r13|r14|r15)\b' "$codegen" 2>/dev/null || echo 0)
        
        # C patterns (C types, keywords)
        gcc_count=$(grep -c -E '\b(int64_t|int32_t|int16_t|int8_t|uint64_t|uint32_t|double|float|struct|typedef|static|extern)\b' "$codegen" 2>/dev/null || echo 0)
        
        # LLVM patterns
        llvm_count=$(grep -c -E '(LLVMBuild|IRBuilder|BasicBlock|Function\*|Value\*|Type\*|Module\*)' "$codegen" 2>/dev/null || echo 0)
        
        echo "  NASM: $nasm_count | GCC: $gcc_count | LLVM: $llvm_count" >> backend_test.log
        
        # Backend belirle
        if [ $llvm_count -gt 5 ]; then
            echo "  Backend: LLVM ❌ (portlama gerekli)" >> backend_test.log
        elif [ $nasm_count -gt 20 ]; then
            echo "  Backend: NASM ⚠️ (assembly)" >> backend_test.log
        elif [ $gcc_count -gt 10 ]; then
            echo "  Backend: GCC ✅ (kullanılabilir)" >> backend_test.log
        elif [ $nasm_count -gt 5 ]; then
            echo "  Backend: NASM ⚠️ (assembly)" >> backend_test.log
        else
            echo "  Backend: MINIMAL ⚠️ (minimal/stub)" >> backend_test.log
        fi
    else
        echo "  Codegen yok (Parser/Util modülü olabilir)" >> backend_test.log
        echo "  Backend: N/A (codegen yok)" >> backend_test.log
    fi
    echo "" >> backend_test.log
done

echo "✅ Backend detection tamamlandı" >> backend_test.log

# MLP Stage 0 Compiler

## 🎯 Hedef

**Modular MLP Compiler via Chained Imports** - Real working architecture!

## 🎉 Son Güncellemeler (8 Aralık 2025)

### ✅ Architectural Milestone:
**Chained Imports Working!**
- Functions → Statement → Control Flow → Comparison (all via #include)
- Parser body fix: while loops + return statements now work
- Central files DELETED (main.c, orchestrator.c, helpers.c - 7 Aralık)
- Entry point: `functions_standalone.c` 102 lines (was 1244!)
- Real proof: test_while_only.mlp generates correct assembly with labels

### ✅ Tamamlanan Phase'ler:

**Phase 3.5: Expressions & Operators** ✅
- Operator precedence (*, /, +, -, mod)
- Parentheses support: `(10 + 5) * 2`
- Variable references in expressions
- TTO runtime integration

**Phase 4: Functions** ✅ (Parsing + Body)
- Function declarations with body parsing
- Parameter handling
- Return statements working
- Chained import to statement module

**Phase 5: Arrays** ✅ (Basic)
- Array declarations: `numeric[] arr`
- Array literals: `[1, 2, 3, 4, 5]`
- Memory allocation (.bss)

**Phase 6: Control Flow** ✅ (Working!)
- If/then/else statements
- While/do loops (TESTED: generates correct labels!)
- Condition evaluation
- Label/jump generation

## ✅ Özellikler

- **Lexer**: Token analizi, UTF-8 desteği
- **Expressions**: Complex arithmetic with precedence
- **Control Flow**: If/While statements (WORKING via chained imports!)
- **Variables**: Declaration and assignment
- **Arrays**: Basic support
- **TTO Runtime**: Overflow detection (BigDecimal)
- **Code Generation**: x86-64 assembly with extern declarations
- **Architecture**: Chained imports (no central orchestrator!)

## 📦 Aktif Modüller (Chained Imports Yapısı)

### Core Modules ✅
- `lexer/` - Tokenization (shared)
- `functions/` - Function declarations + body (imports statement)
- `statement/` - Statement parsing (imports control_flow)
- `control_flow/` - If/While statements (imports comparison)
- `comparison/` - Comparison operators
- `arithmetic/` - Expression parsing & codegen
- `variable/` - Variable management
- `array/` - Array basics
- `print/` - Print statements
- `codegen_context/` - Code generation state
- `runtime_tto/` - TTO runtime (BigDecimal, overflow)

### Support Modules ✅
- `logical/` - Logical operators (and, or, not)
- `comments/` - Comment handling
- `expression/` - Expression coordination
- `statement/` - Statement coordination
- `struct/` - Struct definitions

### 🚀 Modüler Mimari

**Her modül:**
- ✅ Kendi Makefile'ı var
- ✅ Standalone test compiler'ı var
- ✅ Diğer modülleri import edebilir
- ✅ **MERKEZI DOSYA YOK** - Tam bağımsız!

**Örnek modül entegrasyonu:**
```c
// functions_codegen.c
#include "../arithmetic/arithmetic_parser.h"
#include "../runtime_tto/runtime_tto.h"
```

## 🏗️ Test Etme

### Modül Başına Test:

```bash
# Arithmetic modülü (Phase 3.5)
cd modules/arithmetic/
make
./arithmetic_compiler test.mlp output.s

# Control Flow modülü (Phase 6)
cd modules/control_flow/
./control_flow_standalone test.mlp output.s

# Array modülü (Phase 5)
cd modules/array/
./array_standalone test.mlp output.s
```

### Demo Program:

```bash
# Tüm phase'leri test eden demo
./modules/arithmetic/arithmetic_compiler demo_phase_integration.mlp demo.s
nasm -f elf64 demo.s -o demo.o
ld demo.o modules/runtime_tto/runtime_tto.o -o demo_prog \
   -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2
./demo_prog
```

## 📝 Örnek Program

### demo_phase_integration.mlp
```melp
-- Variables & Expressions (Phase 3.5)
numeric x = 10
numeric y = 20
numeric sum = x + y              -- 30
numeric result = (x + y) * 2     -- 60

-- Control Flow (Phase 6)
if sum > 25 then
    numeric big = 1
end if

while x > 0 do
    x = x - 1
end while

-- Arrays (Phase 5)
numeric[] numbers = [10, 20, 30, 40, 50]

-- Print
print("Demo Complete!")
```

**Derleme:**
```bash
cd modules/arithmetic/
./arithmetic_compiler ../../demo_phase_integration.mlp demo.s
```

**Çıktı:** 10 expression başarıyla derlendi, TTO overflow check'leri eklendi

## 📊 Test Sonuçları

### ✅ Başarılı Testler:
- **Arithmetic**: `10 * 2 + 5` = 25 ✓
- **Parentheses**: `(10 + 5) * 2` = 30 ✓
- **Variables**: `x = 10; y = x + 5` ✓
- **Control Flow**: If/While label generation ✓
- **Arrays**: Declaration ve literal initialization ✓
- **TTO**: Overflow detection code generation ✓
- **Print**: String output ✓

### 📈 İstatistikler:
- ✅ 15+ aktif modül
- ✅ Her modül standalone test'li
- ✅ 10 expression demo programda derlendi
- ✅ TTO runtime entegrasyonu çalışıyor
- ✅ Tam modüler mimari (merkezi dosya yok!)

## 🔜 Sonraki Adımlar

1. **Function body codegen** - Return statements
2. **Array index access** - `arr[i]` support
3. **Full integration test** - Tüm modüller bir arada
4. **Optimization** - Dead code elimination

Detaylar için: `NEXT_STEPS.md`

Stage 1'de eklenecekler:
- `sayi` değişken tanımlama
- Aritmetik işlemler (+, -, *, /)
- Sayıları yazdırma
- Basit ifadeler

---

*Stage 0 Complete: 6 Aralık 2025*

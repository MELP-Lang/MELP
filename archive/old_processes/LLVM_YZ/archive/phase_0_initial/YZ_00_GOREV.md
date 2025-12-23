# LLVM_YZ_00: LLVM Backend Entegrasyon Stratejisi

**Tarih:** 20 Aralık 2025  
**Atanan:** YZ_00 (LLVM Görevlisi)  
**Üst Akıl:** YZ_ÜA_04  
**Öncelik:** 🔴 YÜKSEK  
**Kategori:** LLVM Backend Entegrasyonu

---

## 🎯 Görev

MELP compiler'ın mevcut LLVM backend'ini analiz et ve tam entegrasyon planı oluştur.

---

## 📊 Mevcut Durum

### ✅ Hazır Olan:
1. **LLVM Backend Modülü** (`compiler/stage0/modules/llvm_backend/`)
   - `llvm_backend.h` (220 satır)
   - `llvm_backend.c` (527 satır)
   - LLVM IR text generation
   - Context management, temp registers, labels
   - String globals support (YZ_61)
   - Type support: i64 (numeric), i8* (string), i1 (boolean)

2. **LLVM Codegen Wrapper** (`functions_codegen_llvm.c`)
   - Functions compiler için LLVM wrapper
   - YZ_23: String return type fix

3. **Self-hosting** 
   - Stage 1 tamamlandı (%100)
   - Bootstrap cycle kanıtı (59 fonksiyon)

### ⚠️ Bilinen Limitasyonlar:
- For-each loops LLVM'de desteklenmiyor
- Bazı advanced features eksik olabilir

---

## 📋 Yapılacaklar

### 1. Mevcut Durumu Analiz Et

**İncelenecek Dosyalar:**
- [ ] `compiler/stage0/modules/llvm_backend/llvm_backend.h` - API
- [ ] `compiler/stage0/modules/llvm_backend/llvm_backend.c` - Implementation
- [ ] `compiler/stage0/modules/functions/functions_codegen_llvm.c` - Wrapper
- [ ] `compiler/stage0/modules/functions/Makefile` - Build sistemi

**Sorular:**
1. LLVM backend hangi özellikleri destekliyor?
2. Hangi AST node'ları LLVM IR'a çevrilebiliyor?
3. Eksik olan özellikler neler?
4. Test coverage nasıl?

### 2. Test Et

**Basit Testler:**
```mlp
# Test 1: Basic function
function main() as int {
    return(42)
}

# Test 2: Arithmetic
function add(a as int, b as int) as int {
    return(a + b)
}

# Test 3: Control flow
function abs(x as int) as int {
    if x < 0 then
        return(-x)
    end if
    return(x)
}

# Test 4: Loops
function sum_to_n(n as int) as int {
    var i = 1
    var total = 0
    while i <= n
        total = total + i
        i = i + 1
    end while
    return(total)
}
```

**Test Komutu:**
```bash
cd compiler/stage0/modules/functions
./functions_standalone --backend=llvm test.mlp output.ll
lli output.ll
echo $?
```

### 3. Gap Analysis (Eksiklik Analizi)

**Karşılaştır:**
- Stage 0 features (33 modül)
- LLVM backend destekleri
- Eksik olanları listele

**Kategoriler:**
- [ ] Variables (const, var)
- [ ] Functions (declaration, call, return)
- [ ] Control Flow (if/else, switch/case)
- [ ] Loops (while, for, for-each)
- [ ] Operators (arithmetic, comparison, logical)
- [ ] Data Types (numeric, string, boolean, arrays)
- [ ] Structs & Enums
- [ ] Advanced features

### 4. Önceliklendirme

**Kritik (Önce bunlar):**
✅ Functions
✅ Variables
✅ Basic operators
✅ Control flow (if/else)
✅ Return statements

**Orta Öncelik:**
- Loops (while, for)
- String operations
- Arrays
- Function calls

**Düşük Öncelik:**
- Structs
- Enums
- Switch/case
- For-each loops
- Advanced features

### 5. Entegrasyon Planı Oluştur

**Faz 1: Core Features (YZ_00-YZ_05)**
- Basic functions, variables, arithmetic
- Simple control flow
- Return statements

**Faz 2: Control Structures (YZ_06-YZ_10)**
- If/else if/else
- While loops
- For loops

**Faz 3: Advanced (YZ_11+)**
- Arrays
- Strings
- Structs
- Remaining features

---

## 🧪 Test Framework

### LLVM Test Structure
```
tests/llvm/
├── basic/
│   ├── test_return.mlp
│   ├── test_arithmetic.mlp
│   └── test_variables.mlp
├── control_flow/
│   ├── test_if.mlp
│   ├── test_while.mlp
│   └── test_for.mlp
└── advanced/
    ├── test_arrays.mlp
    ├── test_strings.mlp
    └── test_structs.mlp
```

### Test Komutu Template
```bash
#!/bin/bash
# Test LLVM backend

TEST_FILE=$1
OUTPUT_FILE="temp/test_output.ll"

./compiler/stage0/modules/functions/functions_standalone \
    --backend=llvm $TEST_FILE $OUTPUT_FILE

if [ $? -eq 0 ]; then
    echo "✅ Compilation successful"
    lli $OUTPUT_FILE
    EXIT_CODE=$?
    echo "Exit code: $EXIT_CODE"
else
    echo "❌ Compilation failed"
    exit 1
fi
```

---

## 📊 Beklenen Çıktı

**Rapor:** `LLVM_YZ/YZ_00.md`

**İçerik:**
1. **Mevcut Durum Analizi**
   - Desteklenen features listesi
   - Test sonuçları
   
2. **Gap Analysis**
   - Eksik features
   - Prioritization matrix

3. **Entegrasyon Planı**
   - Faz 1, 2, 3 detayları
   - YZ task breakdown (YZ_01-YZ_20)
   - Timeline estimate

4. **Risk Analizi**
   - Potansiyel sorunlar
   - Mitigation stratejileri

5. **Sonraki Adımlar**
   - YZ_01 görev tanımı
   - İlk milestone hedefi

---

## 🔗 Referanslar

- `compiler/stage0/modules/llvm_backend/` - LLVM backend modülü
- `stage_1_YZ/YZ_23.md` - LLVM string type fix
- `ARCHITECTURE.md` - MELP mimarisi (5 ayak)
- `MELP_VISION.md` - LLVM hedefleri

---

## ✅ Tamamlanma Kriterleri

- [ ] LLVM backend API analizi tamamlandı
- [ ] Basit testler çalıştırıldı
- [ ] Gap analysis yapıldı
- [ ] Önceliklendirme matrisi oluşturuldu
- [ ] Faz 1-2-3 planı hazırlandı
- [ ] YZ_01-YZ_20 task breakdown
- [ ] Rapor yazıldı (`LLVM_YZ/YZ_00.md`)
- [ ] Üst Akıl'e sunuldu

---

**Üst Akıl:** YZ_ÜA_04  
**Durum:** ⏳ Atandı

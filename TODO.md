# 📋 MELP Project TODO - Phase 15: Standard Library Integration (LLVM)

**Created:** 13 Aralık 2025  
**Current Focus:** Phase 15 - Standard Library Integration  
**Status:** Phase 15 🚧 Not Started (0%)

---

## ⚠️ BAŞLAMADAN ÖNCE - YENİ AI İÇİN KRİTİK UYARI

**📖 ZORUNLU OKUMA (ÖNCE BURAYI OKU!):**  
👉 **`NEXT_AI_START_HERE.md`** - Current session için mission brief (Buradan başla!)  
👉 **`YZ/YZ_HISTORY.md`** - Tüm YZ oturumlarının özeti

**🔴 GIT KURALLARI (ÇOK ÖNEMLİ!):**
```bash
# ⚠️ HER YZ AYRI DAL OLUŞTURMALIDIR! ⚠️

# 1. YENİ DAL OLUŞTUR (her zaman main/master'dan)
git checkout main
git pull origin main
git checkout -b feature-description_YZ_XX

# ÖRNEK:
# YZ_60: git checkout -b phase14-parser-enhancement_YZ_60
# YZ_61: git checkout -b phase15-stdlib-integration_YZ_61

# 2. Çalış, commit et
git add .
git commit -m "YZ_XX: Feature description"

# 3. KENDI DALINA push et
git push origin feature-description_YZ_XX

# 4. ⚠️ ASLA MERGE YAPMA veya PULL REQUEST OLUŞTURMA!
# Human review yapıp merge edecek.
```

**📖 SONRA BUNLARI OKU:**  
👉 **`ARCHITECTURE.md`** - Mimari kurallar (modülerlik, STO, LLVM Backend)  
👉 **`YZ/AI_METHODOLOGY.md`** - 5 adımlı hızlı geliştirme metodu  
👉 **`docs/LLVM_IR_GUIDE.md`** - LLVM IR mapping patterns  
👉 **`temp/TODO_phase14_complete_20251213.md`** - Completed Phase 14 reference

---

## 🎯 Current Milestone: Phase 15 - Standard Library Integration

### Goal
LLVM backend'inin, C standart kütüphanesindeki `printf` gibi harici fonksiyonlara olan bağımlılığını ortadan kaldırıp, MELP'in kendi standart kütüphanesini (`libmlp_stdlib.a`) ve STO runtime'ını (`libsto_runtime.a`) kullanmasını sağlamak.

### Context
- Phase 14'te LLVM backend tam kontrol akışı desteğini kazandı (if/else, while, for)
- Şu anda `printf` kullanarak çıktı alıyoruz (harici C kütüphanesi bağımlılığı)
- MELP'in kendi `mlp_println_numeric()` fonksiyonu var (`runtime/stdlib/src/io.c`)
- STO (Symbolic Type Object) runtime'ı zaten mevcut (`runtime/sto/`)

### Why This Matters
- LLVM backend'ini projenin geri kalanıyla entegre eder
- Harici bağımlılıkları azaltır (self-contained sistem)
- STO tip sistemiyle uyumlu hale getirir
- Gelecekteki string, array gibi tipler için altyapı oluşturur

---

## 📦 Phase 15 Tasks

### Part 1: Hazırlık ve Planlama (15 dakika)

#### Task 1.1: Git Branch Oluşturma (5 min)
- [ ] Yeni branch oluştur: `phase15-stdlib-integration_YZ_61`
- [ ] Branch'i push et

#### Task 1.2: Mevcut Kodu İnceleme (10 min)
- [ ] `functions_codegen_llvm.c` içinde `printf` kullanımını bul
- [ ] `runtime/stdlib/src/io.c` içinde `mlp_println_numeric` fonksiyonunu incele
- [ ] `runtime/sto/sto.h` içinde STO yapısını anla
- [ ] Mevcut linking yapısını kontrol et (`Makefile`)

**Files to Review:**
- `compiler/stage0/modules/functions/functions_codegen_llvm.c`
- `runtime/stdlib/src/io.c`
- `runtime/sto/sto.h`
- `compiler/stage0/Makefile`

---

### Part 2: `printf` Bağımlılığını Kaldırma (1 saat)

#### Task 2.1: `printf` Kullanımını Bul (10 min)
- [ ] `functions_codegen_llvm.c` içinde `printf` çağrılarını tespit et
- [ ] Hangi statement türlerinde kullanıldığını belirle
- [ ] Mevcut LLVM IR çıktısını incele

#### Task 2.2: `mlp_println_numeric` Deklarasyonu Ekle (20 min)
- [ ] `llvm_backend.c` veya uygun modülde fonksiyon prototipi ekle
- [ ] LLVM IR'de external function declaration oluştur
- [ ] Fonksiyon signature: `void mlp_println_numeric(STO*)`

**Implementation Steps:**
```c
// In llvm_backend.c or functions_codegen_llvm.c
// Add function declaration
void llvm_declare_stdlib_functions(LLVMContext* ctx) {
    // declare void @mlp_println_numeric(%STO*)
    fprintf(ctx->output, "declare void @mlp_println_numeric(%%STO*)\n");
}
```

#### Task 2.3: `printf` Yerine `mlp_println_numeric` Kullan (30 min)
- [ ] STO nesnesi oluşturma kodunu yaz
- [ ] Numerik değeri STO wrapper'a koy
- [ ] `mlp_println_numeric` çağrısını oluştur
- [ ] Eski `printf` kodunu kaldır

**Implementation Steps:**
```c
// Eski kod (printf ile):
// call i32 (i8*, ...) @printf(i8* getelementptr(...), i32 %value)

// Yeni kod (mlp_println_numeric ile):
// 1. STO nesnesi için alan ayır
// %sto = alloca %STO
// 2. STO'yu numerik değerle doldur
// %sto_ptr = getelementptr %STO, %STO* %sto, i32 0, i32 0
// store i32 1, i32* %sto_ptr  ; type = NUMERIC
// %value_ptr = getelementptr %STO, %STO* %sto, i32 0, i32 1
// store i32 %value, i32* %value_ptr
// 3. Fonksiyonu çağır
// call void @mlp_println_numeric(%STO* %sto)
```

**Files to Modify:**
- `compiler/stage0/modules/functions/functions_codegen_llvm.c`
- Possibly `compiler/stage0/modules/llvm_backend/llvm_backend.c`

---

### Part 3: STO Type System Integration (45 dakika)

#### Task 3.1: STO Struct Definition (15 min)
- [ ] LLVM IR'de STO struct tanımı ekle
- [ ] Type enum değerlerini belirle (NUMERIC = 1)
- [ ] Struct layout'u doğrula

**STO Structure:**
```c
// C definition (from runtime/sto/sto.h)
typedef struct STO {
    int type;      // 1 = NUMERIC, 2 = STRING, etc.
    union {
        int numeric_value;
        char* string_value;
        // ...
    } data;
} STO;

// LLVM IR definition
%STO = type { i32, i32 }  ; Simplified: type + numeric_value
```

#### Task 3.2: Helper Functions for STO Creation (20 min)
- [ ] `llvm_create_numeric_sto()` fonksiyonu ekle
- [ ] Geçici değişken isimlerini yönet
- [ ] Memory allocation ve initialization kodunu üret

#### Task 3.3: Test STO Integration (10 min)
- [ ] Basit bir test case derle
- [ ] LLVM IR çıktısını kontrol et
- [ ] STO struct'ın doğru oluşturulduğunu doğrula

---

### Part 4: Linker Configuration (30 dakika)

#### Task 4.1: Runtime Libraries'i Derle (10 min)
- [ ] `runtime/stdlib/` dizininde `make` çalıştır
- [ ] `libmlp_stdlib.a` oluştuğunu doğrula
- [ ] `runtime/sto/` dizininde `make` çalıştır
- [ ] `libsto_runtime.a` oluştuğunu doğrula

**Commands:**
```bash
cd runtime/stdlib
make
ls -la bin/libmlp_stdlib.a

cd ../sto
make
ls -la bin/libsto_runtime.a
```

#### Task 4.2: Makefile'ı Güncelle (20 min)
- [ ] `compiler/stage0/Makefile` dosyasını aç
- [ ] `run_llvm` hedefini bul
- [ ] Linker flags ekle: `-Lruntime/stdlib/bin -Lruntime/sto/bin`
- [ ] Library flags ekle: `-lmlp_stdlib -lsto_runtime`
- [ ] Test et

**Makefile Changes:**
```makefile
run_llvm: functions_compiler
	./functions_compiler -c --backend=llvm $(file) $(file:.mlp=.ll)
	clang $(file:.mlp=.ll) \
		-Lruntime/stdlib/bin -Lruntime/sto/bin \
		-lmlp_stdlib -lsto_runtime \
		-o $(file:.mlp=)
	./$(file:.mlp=)
```

**Files to Modify:**
- `compiler/stage0/Makefile`

---

### Part 5: Testing & Validation (45 dakika)

#### Task 5.1: Create Basic Test (15 min)
- [ ] `test_stdlib_print.mlp` oluştur
- [ ] Basit numerik değer yazdır
- [ ] Compile ve run

**Test Case:**
```mlp
function main() returns numeric
    numeric x = 123
    -- Bu satır artık mlp_println_numeric kullanmalı
    return 0
end function
```

#### Task 5.2: Update Existing Tests (20 min)
- [ ] Phase 14'ten gelen test dosyalarını güncelle
- [ ] Print statements ekle
- [ ] Tüm testleri yeniden çalıştır
- [ ] Regression kontrolü yap

**Tests to Update:**
- `test_while_simple.mlp`
- `test_for_simple.mlp`
- `test_nested_for.mlp`
- `test_loop_with_if.mlp`
- `test_while_nested.mlp`

#### Task 5.3: Comprehensive Testing (10 min)
- [ ] Tüm testleri derle ve çalıştır
- [ ] Çıktıların doğruluğunu kontrol et
- [ ] Memory leaks kontrolü (valgrind)
- [ ] Performance karşılaştırması (printf vs mlp_println)

**Test Automation:**
```bash
#!/bin/bash
# test_stdlib_integration.sh

echo "=== Testing Standard Library Integration ==="
for test in test_*.mlp; do
    echo "Testing: $test"
    make run_llvm file="$test"
    if [ $? -eq 0 ]; then
        echo "✅ PASS: $test"
    else
        echo "❌ FAIL: $test"
    fi
done
```

---

### Part 6: Documentation & Finalization (30 dakika)

#### Task 6.1: Update Documentation (15 min)
- [ ] `README.md` güncelle
- [ ] LLVM backend bölümüne stdlib entegrasyonunu ekle
- [ ] Usage examples güncelle
- [ ] `NEXT_AI_START_HERE.md` güncelle

#### Task 6.2: Create Session Report (10 min)
- [ ] `YZ/YZ_61.md` oluştur
- [ ] Yapılan değişiklikleri belgele
- [ ] Karşılaşılan zorlukları yaz
- [ ] Test sonuçlarını ekle

#### Task 6.3: Git Commit & Push (5 min)
- [ ] Tüm değişiklikleri stage et
- [ ] Descriptive commit message yaz
- [ ] `phase15-stdlib-integration_YZ_61` branch'ine push et

**Commit Message:**
```
YZ_61: Phase 15 - Standard Library Integration for LLVM Backend

- Replaced printf with mlp_println_numeric()
- Added STO type system support in LLVM IR generation
- Integrated libmlp_stdlib.a and libsto_runtime.a
- Updated Makefile with proper linking flags
- All tests passing with new stdlib integration

Phase 15: 100% Complete ✅
```

---

## 🔄 Success Criteria

### Phase 15 Definition of Done
- [ ] `printf` tamamen kaldırıldı
- [ ] `mlp_println_numeric` çalışıyor
- [ ] STO struct'lar doğru oluşturuluyor
- [ ] Runtime libraries doğru linkleniyor
- [ ] Tüm testler geçiyor (regression-free)
- [ ] Documentation tamamlandı
- [ ] Git commit & push yapıldı

---

## 📊 Time Estimates

| Part | Task | Estimated Time |
|------|------|----------------|
| Part 1 | Hazırlık | 15 min |
| Part 2 | Printf Replacement | 60 min |
| Part 3 | STO Integration | 45 min |
| Part 4 | Linker Config | 30 min |
| Part 5 | Testing | 45 min |
| Part 6 | Documentation | 30 min |
| **Total** | | **3.5 hours** |

**Buffer:** 30 min for unexpected issues  
**Total Estimate:** 4 hours

---

## 🎯 Next Phase Preview

### Phase 16: Advanced LLVM Features (Optional)
- Optimization passes (-O1, -O2, -O3)
- Debug information generation
- Better error messages
- **Estimated:** 3-4 hours

### Phase 17: String Support (Major)
- String literals in LLVM IR
- String concatenation
- String comparison
- **Estimated:** 5-6 hours

### Phase 18: Self-Hosting Parser (Major)
- Parser written in MELP
- Generates LLVM IR
- **Estimated:** 15-20 hours

---

## 📝 Notes & Important Context

### STO Type System
```c
// Type constants (from runtime/sto/sto.h)
#define STO_TYPE_NUMERIC 1
#define STO_TYPE_STRING 2
#define STO_TYPE_BOOLEAN 3
#define STO_TYPE_ARRAY 4
```

### Current LLVM Backend Capabilities
- ✅ Basic arithmetic (+, -, *, /)
- ✅ Variable declarations
- ✅ Control flow (if/else, while, for)
- ✅ Function definitions
- ✅ Return statements
- 🚧 Standard library integration (this phase)
- ❌ String support (future)
- ❌ Array support (future)

### Critical Files for This Phase
1. `compiler/stage0/modules/functions/functions_codegen_llvm.c` - Main codegen
2. `compiler/stage0/modules/llvm_backend/llvm_backend.c` - LLVM utilities
3. `runtime/stdlib/src/io.c` - mlp_println_numeric implementation
4. `runtime/sto/sto.h` - STO struct definition
5. `compiler/stage0/Makefile` - Build & link configuration

---

## 🔗 Related Documents

- **Previous Phase:** `temp/TODO_phase14_complete_20251213.md`
- **Architecture:** `ARCHITECTURE.md`
- **YZ Reports:** `YZ/YZ_59.md` (Phase 13.5), `YZ/YZ_60.md` (Phase 14)
- **Next Session:** `NEXT_AI_START_HERE.md`
- **LLVM Guide:** `docs/LLVM_IR_GUIDE.md`
- **STO Guide:** `runtime/sto/README.md` (if exists)

---

**Last Updated:** 13 Aralık 2025  
**Created By:** YZ_60 (Phase 14 completion)  
**Next Session:** YZ_61 (Phase 15 start)  
**Target Completion:** 13 Aralık 2025 (~4 hours)

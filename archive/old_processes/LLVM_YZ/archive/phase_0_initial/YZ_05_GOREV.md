# LLVM_YZ_05: Array Support - GÖREV BRİFİ

**Atanan:** YZ_05  
**Tarih:** 21 Aralık 2025  
**Süre Tahmini:** 4 saat  
**Öncelik:** 🟡 Orta  
**Faz:** 2 (Feature Parity)

---

## 🎯 Görev Özeti

Stage 0'da array desteği var (assembly codegen ile), bu özellikleri LLVM IR backend'e migrate et.

---

## 📋 İş Tanımı

### 1. Mevcut Kod Analizi (30 dakika)

**İncelenecek Dosyalar:**
- `compiler/stage0/modules/array/array_codegen.c` (303 satır, assembly)
- `compiler/stage0/modules/array/array_parser.c` (parser hazır)
- `compiler/stage0/modules/array/array.h` (veri yapıları)

**Anlaşılması Gerekenler:**
- Array literal syntax: `[1, 2, 3]`
- Array indexing: `arr[0]`, `arr[i]`
- Memory allocation: heap-based (sto_array_alloc)
- Element types: homogeneous (hepsi aynı tip)

### 2. LLVM IR Implementation (2 saat)

**Yapılacaklar:**

#### A. Array Literal LLVM IR
```llvm
; [1, 2, 3] → LLVM IR
; 1. Allocate array (malloc or alloca)
%arr_ptr = call i8* @malloc(i64 24)  ; 3 elements * 8 bytes
%arr = bitcast i8* %arr_ptr to i64*

; 2. Initialize elements
%elem0_ptr = getelementptr i64, i64* %arr, i32 0
store i64 1, i64* %elem0_ptr
%elem1_ptr = getelementptr i64, i64* %arr, i32 1
store i64 2, i64* %elem1_ptr
%elem2_ptr = getelementptr i64, i64* %arr, i32 2
store i64 3, i64* %elem2_ptr
```

#### B. Array Indexing LLVM IR
```llvm
; arr[1] → LLVM IR (read)
%elem_ptr = getelementptr i64, i64* %arr, i64 1
%value = load i64, i64* %elem_ptr

; arr[1] = 42 → LLVM IR (write)
%elem_ptr = getelementptr i64, i64* %arr, i64 1
store i64 42, i64* %elem_ptr
```

#### C. Runtime Integration
```c
// Runtime functions (already exist in runtime/stdlib/)
// Just need to declare in LLVM IR:
declare i8* @sto_array_alloc(i64, i64)  ; (num_elements, element_size)
declare void @sto_array_free(i8*)        ; (array_ptr)
declare i64 @sto_array_get(i8*, i64)     ; (array_ptr, index)
declare void @sto_array_set(i8*, i64, i64) ; (array_ptr, index, value)
```

### 3. Code Changes (1 saat)

**Değiştirilecek Dosya:**
- `compiler/stage0/modules/array/array_codegen.c`

**Yapılacak:**
```c
// Mevcut: codegen_array_literal() → assembly üretiyor
// Yeni: LLVM IR üret

void codegen_array_literal(FILE* output, Array* arr, FunctionDeclaration* func) {
    // Check backend type
    if (backend == BACKEND_LLVM) {
        // LLVM IR code here
        fprintf(output, "    %%arr_size = mul i64 %d, 8\n", arr->length);
        fprintf(output, "    %%arr_ptr = call i8* @malloc(i64 %%arr_size)\n");
        fprintf(output, "    %%arr = bitcast i8* %%arr_ptr to i64*\n");
        // ... initialize elements
    } else {
        // Existing assembly code
        fprintf(output, "    movq $%d, %%rdi\n", arr->length);
        // ...
    }
}
```

### 4. Testing (30 dakika)

**Test Dosyaları:**
```mlp
-- tests/llvm/04_arrays/test_array_literal.mlp
function main() returns numeric
    array arr = [1, 2, 3]
    return arr[1]  -- Should return 2
end_function

-- tests/llvm/04_arrays/test_array_index.mlp
function main() returns numeric
    array arr = [10, 20, 30]
    numeric x = arr[0]
    return x  -- Should return 10
end_function

-- tests/llvm/04_arrays/test_array_assign.mlp
function main() returns numeric
    array arr = [1, 2, 3]
    arr[1] = 99
    return arr[1]  -- Should return 99
end_function
```

**Test Komutu:**
```bash
cd tests/llvm
./run_llvm_tests.sh 04_arrays/
```

---

## 📊 Başarı Kriterleri

- [ ] Array literal LLVM IR üretiyor
- [ ] Array indexing (read) çalışıyor
- [ ] Array indexing (write) çalışıyor
- [ ] En az 3 test geçiyor
- [ ] 0 regression (mevcut testler bozulmamış)
- [ ] Kod temiz ve anlaşılır

---

## 🔧 Teknİk Detaylar

### Backend Kontrolü
```c
// functions_standalone.c'de global değişken var:
extern BackendType backend;  // BACKEND_LLVM or BACKEND_ASSEMBLY

// Kullanımı:
if (backend == BACKEND_LLVM) {
    // LLVM IR generate
} else {
    // Assembly generate (existing code)
}
```

### LLVM IR Syntax
```llvm
; Comments start with ;
; Variables start with %
; Global variables start with @
; Types: i64, i32, i8, double, i8* (pointer)
; Instructions: add, sub, mul, load, store, getelementptr, call
```

---

## 📁 Dosya Yapısı

```
compiler/stage0/modules/array/
├── array_codegen.c          ← Bu dosyayı düzenle (LLVM IR ekle)
├── array_codegen.h
├── array_parser.c           ← Hazır, dokunma
├── array.h                  ← Hazır, dokunma
└── array.c                  ← Hazır, dokunma

tests/llvm/
└── 04_arrays/               ← Yeni klasör oluştur
    ├── test_array_literal.mlp
    ├── test_array_index.mlp
    └── test_array_assign.mlp

LLVM_YZ/
├── YZ_05_GOREV.md          ← Bu dosya
└── YZ_05_TAMAMLANDI.md     ← Rapor (bitince oluştur)
```

---

## 🚀 İş Akışı

1. **Analiz:** Mevcut assembly codegen'i incele (30 dk)
2. **Implement:** LLVM IR ekle (2 saat)
3. **Test:** Test dosyaları yaz ve çalıştır (30 dk)
4. **Debug:** Hataları düzelt (30 dk)
5. **Rapor:** YZ_05_TAMAMLANDI.md yaz (30 dk)
6. **Commit:** Git commit + push

---

## 📚 Referanslar

- **Mevcut Kod:** `compiler/stage0/modules/array/`
- **LLVM IR Guide:** `docs/LLVM_IR_GUIDE.md`
- **Test Suite:** `tests/llvm/`
- **Stage 0 Arch:** `compiler/stage0/ARCHITECTURE.md`

---

## ⚠️ Dikkat Edilmesi Gerekenler

1. **Backward Compatibility:** Assembly backend bozulmamalı
2. **Memory Safety:** LLVM IR'de pointer aritmetiği doğru
3. **Type Consistency:** Array elemanları aynı tipte (homogeneous)
4. **STO Integration:** Runtime fonksiyonlarını doğru çağır
5. **Error Handling:** Bounds checking (opsiyonel, phase 2'de)

---

## 💡 Yardımcı Notlar

### Assembly → LLVM IR Mapping

| Assembly | LLVM IR |
|----------|---------|
| `movq $42, %rax` | `%result = add i64 0, 42` |
| `movq (%rbx), %rax` | `%result = load i64, i64* %ptr` |
| `movq %rax, (%rbx)` | `store i64 %value, i64* %ptr` |
| `call malloc` | `%ptr = call i8* @malloc(i64 %size)` |
| `lea 8(%rbx), %rax` | `%ptr = getelementptr i8, i8* %base, i64 8` |

### Örnek LLVM IR (Tam)
```llvm
define i64 @array_test() {
entry:
    ; Allocate array [1, 2, 3]
    %arr_ptr = call i8* @malloc(i64 24)
    %arr = bitcast i8* %arr_ptr to i64*
    
    ; Initialize
    %p0 = getelementptr i64, i64* %arr, i32 0
    store i64 1, i64* %p0
    %p1 = getelementptr i64, i64* %arr, i32 1
    store i64 2, i64* %p1
    %p2 = getelementptr i64, i64* %arr, i32 2
    store i64 3, i64* %p2
    
    ; Read arr[1]
    %read_ptr = getelementptr i64, i64* %arr, i32 1
    %value = load i64, i64* %read_ptr
    
    ; Free memory
    %free_ptr = bitcast i64* %arr to i8*
    call void @free(i8* %free_ptr)
    
    ret i64 %value  ; Returns 2
}

declare i8* @malloc(i64)
declare void @free(i8*)
```

---

**Hazır mısın? Başlayalım!** 🚀

**Tahmini Süre:** 4 saat  
**Zorluk:** Orta  
**Bağımlılıklar:** YOK (bağımsız çalışabilir)

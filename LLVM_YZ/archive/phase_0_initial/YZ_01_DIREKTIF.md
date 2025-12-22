# 🎯 LLVM_YZ_01 - DİREKTİF

**Atanan:** YZ_01  
**Üst Akıl:** YZ_ÜA_04  
**Süre:** 2 saat  
**Öncelik:** 🔴 YÜKSEK

---

## ✅ YAPILACAKLAR (Checklist)

### 1. Dosyayı Aç
```bash
vim compiler/stage0/modules/functions/functions_standalone.c
```

### 2. Backend Enum Ekle (Satır ~15)
```c
typedef enum {
    BACKEND_ASSEMBLY,
    BACKEND_LLVM
} BackendType;
```

### 3. Global Backend Değişkeni Ekle (Satır ~20)
```c
static BackendType selected_backend = BACKEND_ASSEMBLY;  // Default
```

### 4. Help Mesajını Güncelle (Satır ~50)
```c
printf("Usage: %s [options] <input.mlp> <output>\n", program_name);
printf("Options:\n");
printf("  --backend=assembly    Generate x86-64 assembly (default)\n");
printf("  --backend=llvm        Generate LLVM IR\n");
printf("  --help                Show this help\n");
```

### 5. Argüman Parse'ı Ekle (main fonksiyonu)
```c
// Argüman parsing (--backend flag)
for (int i = 1; i < argc; i++) {
    if (strncmp(argv[i], "--backend=", 10) == 0) {
        char* backend_name = argv[i] + 10;
        if (strcmp(backend_name, "llvm") == 0) {
            selected_backend = BACKEND_LLVM;
        } else if (strcmp(backend_name, "assembly") == 0) {
            selected_backend = BACKEND_ASSEMBLY;
        } else {
            fprintf(stderr, "Unknown backend: %s\n", backend_name);
            return 1;
        }
        // Bu argümanı listeden çıkar (kaydır)
        for (int j = i; j < argc - 1; j++) {
            argv[j] = argv[j + 1];
        }
        argc--;
        i--;
    }
}
```

### 6. Backend Seçimine Göre Codegen Çağır
```c
// Eski kod:
// compile_to_assembly(ast, output_file);

// Yeni kod:
if (selected_backend == BACKEND_LLVM) {
    compile_to_llvm(ast, output_file);  // Mevcut fonksiyon
} else {
    compile_to_assembly(ast, output_file);  // Mevcut fonksiyon
}
```

### 7. Derle
```bash
cd compiler/stage0/modules/functions
make clean
make
```

### 8. Test Et
```bash
# Test 1: Return
echo 'function main() as int { return(42) }' > /tmp/test1.mlp
./functions_standalone --backend=llvm /tmp/test1.mlp /tmp/test1.ll
lli /tmp/test1.ll
echo $?  # Beklenen: 42

# Test 2: Arithmetic
echo 'function main() as int { return(10 + 20) }' > /tmp/test2.mlp
./functions_standalone --backend=llvm /tmp/test2.mlp /tmp/test2.ll
lli /tmp/test2.ll
echo $?  # Beklenen: 30

# Test 3: Variable
echo 'function main() as int { const x = 88 return(x) }' > /tmp/test3.mlp
./functions_standalone --backend=llvm /tmp/test3.mlp /tmp/test3.ll
lli /tmp/test3.ll
echo $?  # Beklenen: 88

# Test 4: Function call
echo 'function add(a as int, b as int) as int { return(a + b) } function main() as int { return(add(5, 10)) }' > /tmp/test4.mlp
./functions_standalone --backend=llvm /tmp/test4.mlp /tmp/test4.ll
lli /tmp/test4.ll
echo $?  # Beklenen: 15

# Test 5: If statement
echo 'function main() as int { if 1 > 0 then return(99) end if return(0) }' > /tmp/test5.mlp
./functions_standalone --backend=llvm /tmp/test5.mlp /tmp/test5.ll
lli /tmp/test5.ll
echo $?  # Beklenen: 99
```

### 9. Rapor Yaz
```bash
cat > LLVM_YZ/YZ_01.md << 'EOF'
# YZ_01: CLI Entegrasyonu

**Durum:** ✅ Tamamlandı

## Yapılanlar:
- Backend enum eklendi
- --backend=llvm flag implementasyonu
- Help mesajı güncellendi
- 5/5 test geçti

## Test Sonuçları:
✅ Test 1: Return 42
✅ Test 2: Arithmetic 30
✅ Test 3: Variable 88
✅ Test 4: Function call 15
✅ Test 5: If statement 99

## Değiştirilen Dosya:
- compiler/stage0/modules/functions/functions_standalone.c (+60 satır)
EOF
```

### 10. Git Commit
```bash
cd /home/pardus/projeler/MLP/MLP
git add compiler/stage0/modules/functions/functions_standalone.c
git add LLVM_YZ/YZ_01.md
git commit -m "LLVM_YZ_01: CLI Backend Selection

- Added --backend=llvm flag support
- Backend enum (ASSEMBLY/LLVM)
- Updated help message
- All 5 tests passing
- Modified: functions_standalone.c (+60 lines)

Co-authored-by: YZ_01 <yz01@melp-lang.org>"
```

---

## ❌ YAPMA!

- ❌ LLVM backend kodunu değiştirme (zaten hazır)
- ❌ Parser'ı değiştirme
- ❌ Yeni modül ekleme
- ❌ Test dosyalarını ana dizine bırakma

---

## ✅ BAŞARI KRİTERLERİ

- [x] `--backend=llvm` flag'i çalışıyor
- [x] 5 test geçiyor
- [x] `lli` ile LLVM IR çalışıyor
- [x] Rapor yazıldı
- [x] Git commit yapıldı

---

## 🆘 TAKILIRSAN

1. **Compile hatası:** `make clean && make` yap
2. **lli bulunamıyor:** `sudo apt install llvm` yap
3. **Test fail:** `cat /tmp/test*.ll` ile IR'ı kontrol et
4. **Üst Akıl'e sor!**

---

**Bu direktifi takip et. Adım adım git. Tamamladığında rapor et.**

**Tahmini süre: 2 saat**

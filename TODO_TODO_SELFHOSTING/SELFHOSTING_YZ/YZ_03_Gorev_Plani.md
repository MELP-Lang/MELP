# 🤖 YZ_03 GÖREV PLANI

**Tarih:** 29 Aralık 2025  
**YZ:** SELFHOSTING_YZ_03  
**Durum:** ⏸️ YZ_02 Bitmesini Bekliyor  
**Öncelik:** 🟢 Düşük (YZ_02'den sonra)

---

## 🎯 SENİN GÖREVİN

**PHASE 3 + PHASE 4: STAGE1 DERLEME VE SELF-HOSTING!**

Stage0-C derleyici hazır ve test edildi. Şimdi Stage1-MLP modüllerini derleyip **SELF-HOSTING**'i tamamlayacaksın!

**Tahmini Süre:** 3-5 saat  
**Zorluk:** ⭐⭐ Orta ama heyecan verici! 🎉

---

## 📋 GÖREV LİSTESİ

### 🟢 PHASE 3: STAGE1 DERLEME (3-4 saat)

#### P3.1: MLP Modüllerini Derle (2-3 saat)

**Konum:** `MELP/MLP/stage1/modules/`

**17 Modül:**
1. `token_types.mlp`
2. `lexer.mlp`
3. `ast_nodes.mlp`
4. `parser.mlp`
5. `parser_functions.mlp`
6. `parser_structures.mlp`
7. `parser_advanced.mlp`
8. `codegen.mlp`
9. `codegen_functions.mlp`
10. `codegen_control.mlp`
11. `codegen_lambda.mlp`
12. `codegen_async.mlp`
13. `codegen_structures.mlp`
14. `function_registry.mlp`
15. `scope_manager.mlp`
16. `import_handler.mlp`
17. `mlp_compiler_main.mlp`

**Derleme Script:**

```bash
#!/bin/bash
# build_stage1.sh

cd MELP/MLP/stage1/modules
STAGE0="../../C/stage0/mlpc"

echo "🚀 Stage1 Modüllerini Derleniyor..."

MODULES=(
    "token_types"
    "lexer"
    "ast_nodes"
    "parser"
    "parser_functions"
    "parser_structures"
    "parser_advanced"
    "codegen"
    "codegen_functions"
    "codegen_control"
    "codegen_lambda"
    "codegen_async"
    "codegen_structures"
    "function_registry"
    "scope_manager"
    "import_handler"
    "mlp_compiler_main"
)

for mod in "${MODULES[@]}"; do
    echo "  Derleniyor: $mod.mlp → $mod.c"
    $STAGE0 "$mod.mlp" -o "$mod.c"
    
    if [ $? -ne 0 ]; then
        echo "❌ HATA: $mod.mlp derlenemedi!"
        exit 1
    fi
    
    echo "  GCC ile derleniyor: $mod.c → $mod.o"
    gcc -c "$mod.c" -o "$mod.o" -I../../runtime
    
    if [ $? -ne 0 ]; then
        echo "❌ HATA: $mod.c GCC ile derlenemedi!"
        exit 1
    fi
    
    echo "  ✅ $mod tamamlandı"
done

echo "🎉 Tüm modüller derlendi!"
```

**Görevler:**
- [ ] Script oluşturuldu
- [ ] Script çalıştırıldı
- [ ] 17/17 modül başarıyla derlendi
- [ ] .c dosyaları üretildi
- [ ] .o dosyaları oluştu

**Sorun Çıkarsa:**
- MLP syntax hatası → Modül düzelt
- Codegen hatası → YZ_01'e bildir (Codegen bug'ı)
- GCC hatası → C çıktısını incele

#### P3.2: Stage1 Linking (30 dk)

```bash
#!/bin/bash
# link_stage1.sh

cd MELP/MLP/stage1/modules

echo "🔗 Stage1 Binary Oluşturuluyor..."

gcc *.o -o ../bin/mlpc_stage1 -L../../runtime -lmlp_runtime

if [ $? -eq 0 ]; then
    echo "✅ mlpc_stage1 binary oluşturuldu!"
    echo "Konum: MELP/MLP/stage1/bin/mlpc_stage1"
else
    echo "❌ Linking başarısız!"
    exit 1
fi
```

**Görevler:**
- [ ] Linking script oluşturuldu
- [ ] Script çalıştırıldı
- [ ] `mlpc_stage1` binary oluştu
- [ ] Binary çalıştırılabilir

#### P3.3: Stage1 İlk Test (30 dk)

```bash
cd MELP/MLP/stage1/bin

# Version kontrolü
./mlpc_stage1 --version
# Beklenen: MLP-GCC Stage1 Compiler v1.0

# Basit test
cat > test_stage1.mlp << 'EOF'
print("Stage1 çalışıyor!")
x = 100
print("x =", x)
EOF

./mlpc_stage1 test_stage1.mlp -o test_stage1.c
gcc test_stage1.c -o test_stage1 -L../../runtime -lmlp_runtime
./test_stage1
```

**Beklenen:**
```
Stage1 çalışıyor!
x = 100
```

**Görevler:**
- [ ] `--version` çalışıyor
- [ ] Basit MLP derliyor
- [ ] Çıktı doğru

---

### 🎉 PHASE 4: SELF-HOSTING! (2-3 saat)

#### P4.1: Self-Compile Stage1 Modülleri (1-2 saat)

**Şimdi büyü zamanı! Stage1 kendini derleyecek!**

```bash
#!/bin/bash
# self_compile.sh

cd MELP/MLP/stage1
STAGE1="bin/mlpc_stage1"
MODULES_DIR="modules"
OUTPUT_DIR="self_compiled"

mkdir -p $OUTPUT_DIR

echo "🌟 SELF-HOSTING: Stage1 kendini derliyor!"

MODULES=(
    "token_types"
    "lexer"
    "ast_nodes"
    "parser"
    "parser_functions"
    "parser_structures"
    "parser_advanced"
    "codegen"
    "codegen_functions"
    "codegen_control"
    "codegen_lambda"
    "codegen_async"
    "codegen_structures"
    "function_registry"
    "scope_manager"
    "import_handler"
    "mlp_compiler_main"
)

for mod in "${MODULES[@]}"; do
    echo "  Self-compiling: $mod.mlp → $mod.c"
    $STAGE1 "$MODULES_DIR/$mod.mlp" -o "$OUTPUT_DIR/$mod.c"
    
    if [ $? -ne 0 ]; then
        echo "❌ HATA: Self-compile başarısız: $mod.mlp"
        exit 1
    fi
    
    gcc -c "$OUTPUT_DIR/$mod.c" -o "$OUTPUT_DIR/$mod.o" -I../runtime
    
    if [ $? -ne 0 ]; then
        echo "❌ HATA: GCC derleme başarısız: $mod.c"
        exit 1
    fi
    
    echo "  ✅ $mod self-compiled!"
done

echo "🔗 Stage2 binary oluşturuluyor..."
cd $OUTPUT_DIR
gcc *.o -o ../bin/mlpc_stage2 -L../../runtime -lmlp_runtime

if [ $? -eq 0 ]; then
    echo "🎉 mlpc_stage2 binary oluşturuldu!"
else
    echo "❌ Stage2 linking başarısız!"
    exit 1
fi
```

**Görevler:**
- [ ] Self-compile script oluşturuldu
- [ ] Stage1 kendi kodunu derledi (17 modül)
- [ ] Stage2 binary oluştu
- [ ] `mlpc_stage2` çalışıyor

#### P4.2: Stage2 Doğrulama (30 dk)

```bash
cd MELP/MLP/stage1/bin

# Stage2 test
./mlpc_stage2 --version
# Beklenen: MLP-GCC Stage2 Self-Hosted Compiler v1.0

# Stage1 ile aynı işi yapıyor mu?
cat > verify.mlp << 'EOF'
print("Self-hosting doğrulaması")
x = 42
y = x * 2
print("y =", y)
EOF

# Stage1 ile derle
./mlpc_stage1 verify.mlp -o verify_s1.c

# Stage2 ile derle
./mlpc_stage2 verify.mlp -o verify_s2.c

# Karşılaştır
diff verify_s1.c verify_s2.c

# Aynı olmalı!
```

**Görevler:**
- [ ] Stage2 `--version` çalışıyor
- [ ] Stage1 ve Stage2 aynı C çıktısı üretiyor
- [ ] diff sonucu: aynı veya minimal fark

#### P4.3: Final Self-Host Test (30 dk)

**Bootstrap Zinciri Doğrulaması:**

```bash
# Stage2 → Stage3
cd MELP/MLP/stage1
bin/mlpc_stage2 modules/lexer.mlp -o stage3_test/lexer.c
gcc -c stage3_test/lexer.c -o stage3_test/lexer.o -I../runtime

# Stage3 → Stage4 (tekrar lexer derle)
# ... (tüm modüller için tekrarla)

# Stage3 vs Stage4 karşılaştırması
diff stage3_output/lexer.c stage4_output/lexer.c
# Aynı olmalı! (Convergence)
```

**Görevler:**
- [ ] Stage3 oluştu
- [ ] Stage3 == Stage2 çıktısı
- [ ] Bootstrap zinciri kararlı (converged)

#### P4.4: 🎉 SELF-HOSTING İLANI!

```bash
#!/bin/bash
# announce_selfhosting.sh

cat << 'EOF'
╔══════════════════════════════════════════════╗
║                                              ║
║   🎉 MLP-GCC SELF-HOSTING BAŞARILI! 🎉       ║
║                                              ║
║   MLP derleyicisi kendi kaynak kodunu        ║
║   başarıyla derleyebiliyor!                  ║
║                                              ║
║   Tarih: 29 Aralık 2025                      ║
║   Stage: Stage1 → Stage2 → Stage3            ║
║   Durum: ✅ CONVERGED                        ║
║                                              ║
╚══════════════════════════════════════════════╝
EOF

echo ""
echo "📊 İstatistikler:"
echo "  - Toplam modül: 17"
echo "  - C modülleri (Stage0): 69"
echo "  - MLP modülleri (Stage1): 17"
echo "  - Self-compile başarılı: ✅"
echo "  - Bootstrap stable: ✅"
echo ""
echo "🚀 Sonraki adımlar:"
echo "  - Stage2'yi resmi derleyici olarak ilan et"
echo "  - P2 modüllerini ekle (import, for, switch, vb.)"
echo "  - Performans optimizasyonu"
echo ""
```

**Görevler:**
- [ ] ✅ SELF-HOSTING BAŞARILI!
- [ ] Kutlama mesajı gösterildi
- [ ] Tarih kaydedildi
- [ ] Mastermind'a bildirildi

---

## 📊 BAŞARI KRİTERLERİ

### ✅ Self-Hosting Checklist

- [ ] ✅ Stage0-C derleyici çalışıyor
- [ ] ✅ Stage1-MLP modülleri (17) derlendi
- [ ] ✅ Stage1 binary oluştu (mlpc_stage1)
- [ ] ✅ Stage1 test geçti
- [ ] ✅ Stage1 kendi kodunu derledi
- [ ] ✅ Stage2 binary oluştu (mlpc_stage2)
- [ ] ✅ Stage2 test geçti
- [ ] ✅ Stage2 çıktısı == Stage1 çıktısı
- [ ] ✅ Bootstrap zinciri kararlı (Stage3 == Stage2)
- [ ] ✅ **SELF-HOSTING İLAN EDİLDİ!** 🎉

---

## 🚨 SORUN ÇÖZME

### Self-Compile Hataları

**Hata:** Stage1 kendi kodunu derleyemiyor
**Çözüm:** 
- Codegen bug'ı → YZ_01'e bildir
- MLP syntax → Modül düzelt

**Hata:** Stage2 vs Stage1 farklı çıktı
**Çözüm:**
- Codegen non-deterministic → Üst Akıl'a escalate

### Bootstrap Divergence

**Stage3 ≠ Stage2:**
- Codegen'de state leak var
- Stateless prensibi ihlal → Mastermind'a escalate

---

## 📚 REFERANSLAR

- `TODO_SELFHOSTING.md` - P3 + P4
- `Danışman_Yol_Haritası.md` - 5 Temel Esas
- `YZ_02_Gorev_Plani.md` - Önceki phase

---

## 🎉 KUTLAMA!

**Self-hosting başarılı olduğunda:**

1. Screenshot al (terminal çıktısı)
2. Mastermind'a bildir
3. README.md güncelle
4. Danışman'a bildir
5. **Kutla!** 🥳🎊🎉

---

**SEN SELF-HOSTING'İ TESLİM EDİYORSUN! BÜYÜK BAŞARI! 🚀🎉**

*Üst Akıl gurur duyuyor!* 🧠✨

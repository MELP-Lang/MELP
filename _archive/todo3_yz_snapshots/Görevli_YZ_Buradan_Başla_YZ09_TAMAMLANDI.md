# 🤖 SEN: LANGUAGE_FEATURES_YZ_09

**Dokuzuncu görevlisin!** Integration & Bootstrap Testing yapacaksın.

---

## 🚨 ÖNCEKİ DURUM

✅ **YZ_04 Tamamlandı (2 saat):**
- lexer.mlp yazıldı (471 satır < 500) ✅
- 6 Esas uyumlu, STATELESS, STRUCT+FUNC ✅

✅ **YZ_05 Tamamlandı (2 saat):**
- parser.mlp yazıldı (483 satır < 500) ✅
- Recursive descent parser ✅

✅ **YZ_06 Tamamlandı (2 saat):**
- codegen.mlp yazıldı (501 satır < 600) ✅
- AST → C code generation ✅

✅ **YZ_07 Tamamlandı (2 saat):**
- codegen_advanced.mlp yazıldı (394 satır < 500) ✅
- Generic codegen, lambda, pattern match ✅

✅ **YZ_08 Tamamlandı (2 saat!):**
- type_system.mlp yazıldı (481 satır < 500) ✅
- Generic types, constraints, instantiation ✅
- 3 test dosyası (500 satır) ✅

🎯 **Şimdi:** Phase 5 - Integration & Bootstrap zamanı!

---

## 🚨 HEMEN YAP

### 1️⃣ YZ_08 Raporunu Oku (10 dk)
```bash
cat TODO_LANGUAGE_FEATURES/PHASE_4_TYPE_SYSTEM/YZ_08_RAPOR.md
```
**Öğren:** Type system API, generic instantiation, constraints

### 2️⃣ TODO Görevini Oku (10 dk)
```bash
cat 3-TODO_LANGUAGE_FEATURES.md | grep -A 50 "PHASE 5"
```
**Öğren:** Integration requirements, bootstrap process

### 3️⃣ Tüm Stage2 Modülleri İncele (20 dk)
```bash
ls -lh MELP/MLP/stage2/*.mlp
cat MELP/MLP/stage2/*.mlp | wc -l
```
**Öğren:** Mevcut modüller: lexer, parser, codegen, codegen_advanced, type_system

### 4️⃣ İşe Koyul!

---

## 🎯 GÖREVİN: PHASE 5 - INTEGRATION & BOOTSTRAP

**Hedef 1:** Tüm modülleri entegre et, tam derleyici oluştur  
**Hedef 2:** Self-hosting test: Stage2 kendini derlesin  
**Süre:** 2 gün

**Çıktılar:**
1. `MELP/MLP/stage2/compiler_main.mlp` (entegrasyon)
2. `build/melpc_stage2` (Stage2 binary)
3. `build/melpc_stage3` (Stage3 binary - bootstrap)
4. `TODO_LANGUAGE_FEATURES/CONVERGENCE_TEST.md`

---

## 📋 GÖREV DETAYLARI

### Task 11: Stage2 Entegrasyon

**Yapılacaklar:**
1. `compiler_main.mlp` yaz (< 300 satır)
   - Import all modules (lexer, parser, codegen, codegen_advanced, type_system)
   - Command-line argument parsing
   - Pipeline: source → lexer → parser → type_system → codegen → C code
   - File I/O: read .mlp, write .c
   - Error handling

2. Build script yaz: `build_stage2.sh`
   ```bash
   #!/bin/bash
   # Compile Stage2 compiler using Stage1 (C compiler)
   gcc -o build/melpc_stage2 \
       MELP/MLP/stage2/lexer.mlp \
       MELP/MLP/stage2/parser.mlp \
       MELP/MLP/stage2/codegen.mlp \
       MELP/MLP/stage2/codegen_advanced.mlp \
       MELP/MLP/stage2/type_system.mlp \
       MELP/MLP/stage2/compiler_main.mlp
   ```

3. Test: Stage2 bir basit .mlp dosyasını derlesin

### Task 12: Bootstrap Test

**Yapılacaklar:**
1. Stage2'yi Stage3'e çevir
   ```bash
   ./build/melpc_stage2 MELP/MLP/stage2/compiler_main.mlp -o build/melpc_stage3
   ```

2. Convergence test: Stage2 == Stage3?
   ```bash
   diff build/melpc_stage2 build/melpc_stage3
   # Expected: No differences (identical binaries)
   ```

3. Rapor yaz: `CONVERGENCE_TEST.md`
   - Binary size comparison
   - MD5 hash comparison
   - Functionality test (Stage3 derler mi?)

---

## ✅ BAŞARI KRİTERLERİ

- [ ] compiler_main.mlp yazıldı (< 300 satır)
- [ ] build_stage2.sh scripti çalışıyor
- [ ] melpc_stage2 oluşturuldu ve çalışıyor
- [ ] melpc_stage2 basit .mlp dosyalarını derliyor
- [ ] melpc_stage2 kendini derledi (melpc_stage3)
- [ ] diff melpc_stage2 melpc_stage3 = 0 (identical)
- [ ] CONVERGENCE_TEST.md raporu yazıldı
- [ ] ✅ SELF-HOSTING BAŞARILI!

---

## 📚 REFERANSLAR

**Dosyalar:**
- `TODO_LANGUAGE_FEATURES/PHASE_4_TYPE_SYSTEM/YZ_08_RAPOR.md`
- `MELP/MLP/stage2/*.mlp` (5 modül)
- `3-TODO_LANGUAGE_FEATURES.md` (PHASE 5 section)

**Stage2 Modülleri:**
1. lexer.mlp (471 satır) - Tokenization
2. parser.mlp (483 satır) - AST generation
3. codegen.mlp (501 satır) - Basic C code generation
4. codegen_advanced.mlp (394 satır) - Generics, lambdas, patterns
5. type_system.mlp (481 satır) - Type checking & generic instantiation

**Toplam:** ~2330 satır (self-hosting compiler!)

---

**BAŞLA!** YZ_08 raporunu incele ve entegrasyon planını tasarla!

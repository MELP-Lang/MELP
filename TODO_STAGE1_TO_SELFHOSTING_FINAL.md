# 🚨 MELP SELF-HOSTING - NİHAİ TODO

---

## ⚠️ BU BELGEYİ OKUYAN HER YZ İÇİN - ÖNCE BUNU OKU!

### 📍 NEREDEYIZ?

**Tarih:** 23 Aralık 2025

**Gerçek:** MELP projesi **10+ self-hosting denemesi** yaşadı. Her seferinde yanlış yola sapıldı:
- "Önce Stage 0'a şu özelliği ekleyelim"
- "FFI lazım, Module system lazım"
- "Yeni TODO yazalım"

**ARTIK YOK!**

---

### 🎯 BU TODO'NUN TEK AMACI

```
┌─────────────────────────────────────────────────────────────────┐
│                                                                 │
│   STAGE 1 COMPILER (MELP ile yazılmış) KENDİNİ DERLEYEBİLSİN   │
│                                                                 │
│   compiler.mlp → (derlenir) → compiler_gen1.ll                 │
│   compiler_gen1.ll → compiler.mlp → compiler_gen2.ll           │
│                                                                 │
│   diff compiler_gen1.ll compiler_gen2.ll = BOŞ                 │
│                                                                 │
│   BU OLDUĞUNDA = SELF-HOSTING TAMAM! 🎉                        │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

### 📦 ELİMİZDE NE VAR?

**Stage 1 modülleri ZATEN YAZILDI:**

```
compiler/stage1/modules/
├── lexer_mlp/lexer.mlp           ✅ 346 satır, MELP ile yazılmış
├── parser_mlp/parser.mlp         ✅ 1074 satır, MELP ile yazılmış
├── codegen_mlp/                  ✅ 19 dosya, MELP ile yazılmış
│   ├── codegen_functions.mlp
│   ├── codegen_arithmetic.mlp
│   ├── ir_builder.mlp
│   └── ...
└── compiler.mlp                  ⚠️ Orchestrator (düzeltilecek)
```

**Problem:** Bu modüller **syntax hataları** ve **entegrasyon eksiklikleri** yüzünden birlikte çalışmıyor.

---

### ❌ YAPMAYACAKLARIN (YASAKLAR)

| YASAK | NEDEN |
|-------|-------|
| Stage 0'a yeni özellik ekleme | Stage 0 BİTTİ, deprecated |
| FFI ekleme | Self-hosting için gerekli değil |
| Module system ekleme | Tek dosya yaklaşımı yeterli |
| Error handling ekleme | Önce self-hosting, sonra özellik |
| Yeni TODO yazma | BU SON TODO |
| "Detaylandırmamı ister misin?" | YASAK, direkt yap |
| Phase/Task icat etme | TODO'da ne yazıyorsa o |

**İHLAL = GÖREV DURDURULUR!**

---

### ✅ YAPACAKLARIN

1. **Stage 1 modüllerindeki syntax hatalarını düzelt**
   - `pmlp_kesin_sozdizimi.md`'ye uyumlu hale getir
   - Virgül → noktalı virgül
   - `then` eksikliklerini tamamla

2. **Modülleri tek dosyada birleştir** (veya çalışır hale getir)
   - lexer + parser + codegen → compiler.mlp

3. **Self-compile test yap**
   - Stage 0 → compiler.mlp → Gen1
   - Gen1 → compiler.mlp → Gen2
   - diff Gen1 Gen2 = BOŞ

4. **Convergence kanıtla**
   - Gen2 → compiler.mlp → Gen3
   - Gen2 = Gen3 → TAMAM!

---

### 🏆 BAŞARI KRİTERİ

```bash
# 1. Stage 0 ile Stage 1'i derle
./stage0/compiler compiler.mlp -o gen1.ll

# 2. Gen1 ile compiler.mlp'yi derle
lli gen1.ll compiler.mlp gen2.ll

# 3. Karşılaştır
diff gen1.ll gen2.ll

# BOŞ ÇIKTI = 🎉 SELF-HOSTING TAMAM!
```

---

### 📅 BU TODO BİTTİĞİNDE

1. ✅ MELP kendini derliyor
2. ✅ Stage 0 deprecated (sadece bootstrap için)
3. ✅ Tüm yeni özellikler Stage 1'e (MELP'e) eklenir
4. ✅ Sonsuz döngüden ÇIKIŞ!

---

## 📋 GÖREVLER

### PHASE 1: Syntax Düzeltme (1-2 gün)

**Hedef:** Stage 1 modüllerini `pmlp_kesin_sozdizimi.md`'ye uyumlu hale getir.

#### Task 1.1: Syntax Analizi (2 saat)

**Görev:** Tüm Stage 1 modüllerini tara, syntax hatalarını listele.

```bash
# Çalıştır:
find compiler/stage1/modules -name "*.mlp" -exec grep -l "," {} \;
```

**Kontrol edilecekler:**
- [ ] Parametre ayırıcıları: `,` → `;`
- [ ] List literals: `[a, b]` → `[a; b]`
- [ ] Çok satırlı if'lerde `then` var mı?
- [ ] while/for'da `do` var mı?

**Çıktı:** Düzeltilecek dosya listesi

---

#### Task 1.2: Toplu Düzeltme (4-6 saat)

**Görev:** Syntax hatalarını düzelt.

**Öncelik sırası:**
1. `lexer_mlp/lexer.mlp`
2. `parser_mlp/parser.mlp`
3. `codegen_mlp/*.mlp`
4. `compiler.mlp`

**Her dosya için:**
```bash
# Düzeltmeden önce test et
timeout 15 ./compiler/stage0/modules/functions/functions_compiler \
  dosya.mlp -o temp/test.ll && echo "✅ OK" || echo "❌ HATA"
```

**Başarı kriteri:** %95+ modül derleniyor

---

### PHASE 2: Modül Entegrasyonu (1 gün)

**Hedef:** Lexer, parser, codegen modüllerini `compiler.mlp`'de birleştir.

#### Task 2.1: Compiler Orchestrator (4 saat)

**Görev:** `compiler.mlp`'yi düzelt/yeniden yaz.

```melp
-- compiler.mlp
-- Stage 1 Self-Hosting Compiler

function main() returns numeric
    -- 1. Kaynak dosyayı oku
    string source = read_file("input.mlp")
    
    -- 2. Tokenize et
    list tokens = tokenize(source)
    
    -- 3. Parse et
    list ast = parse(tokens)
    
    -- 4. LLVM IR üret
    string ir = generate_llvm(ast)
    
    -- 5. Dosyaya yaz
    write_file("output.ll"; ir)
    
    return 0
end_function
```

**Seçenek A:** Tüm fonksiyonları tek dosyada birleştir
**Seçenek B:** Stage 0'a multi-file desteği ekle (SON ÇARE!)

---

#### Task 2.2: Pipeline Test (2 saat)

**Görev:** Basit bir MELP programını derle.

```bash
# Test programı
echo 'function main() returns numeric
    return 42
end_function' > test.mlp

# Stage 0 ile compiler.mlp'yi derle
./stage0/compiler compiler.mlp -o compiler_gen1.ll

# Gen1 ile test.mlp'yi derle
lli compiler_gen1.ll test.mlp test.ll

# Çalıştır
lli test.ll
echo $?  # 42 çıkmalı
```

---

### PHASE 3: Self-Compile (1 gün)

**Hedef:** Stage 1'in kendini derlemesi.

#### Task 3.1: Generation 1 → Generation 2 (3 saat)

```bash
# Gen1 oluştur (Stage 0 ile)
./stage0/compiler compiler.mlp -o gen1.ll

# Gen2 oluştur (Gen1 ile)
lli gen1.ll compiler.mlp gen2.ll

# Karşılaştır
diff gen1.ll gen2.ll
```

**Beklenen:** Fark olabilir (farklı compiler'lar)

---

#### Task 3.2: Convergence Test (3 saat)

```bash
# Gen3 oluştur (Gen2 ile)
lli gen2.ll compiler.mlp gen3.ll

# Karşılaştır
diff gen2.ll gen3.ll
```

**Beklenen:** BOŞ ÇIKTI! (Convergence)

**Eğer boş değilse:** Hata ayıkla, düzelt, tekrar dene.

---

### PHASE 4: Finalizasyon (0.5 gün)

#### Task 4.1: Stage 0 Deprecation

```bash
# Stage 0'a deprecated notu ekle
echo "# ⚠️ DEPRECATED - Bootstrap Only" > compiler/stage0/DEPRECATED.md
```

#### Task 4.2: Dokümantasyon

- [ ] README.md güncelle
- [ ] ARCHITECTURE.md güncelle
- [ ] Self-hosting başarı raporu yaz

#### Task 4.3: Git Tag

```bash
git tag -a v1.0.0-selfhosting -m "Self-hosting achieved!"
git push origin v1.0.0-selfhosting
```

---

## 📊 İLERLEME TAKİBİ

| Phase | Task | Durum | Sorumlu YZ |
|-------|------|-------|------------|
| Phase 1 | Task 1.1: Syntax Analizi | ⬜ | - |
| Phase 1 | Task 1.2: Toplu Düzeltme | ⬜ | - |
| Phase 2 | Task 2.1: Compiler Orchestrator | ⬜ | - |
| Phase 2 | Task 2.2: Pipeline Test | ⬜ | - |
| Phase 3 | Task 3.1: Gen1 → Gen2 | ⬜ | - |
| Phase 3 | Task 3.2: Convergence | ⬜ | - |
| Phase 4 | Task 4.1: Deprecation | ⬜ | - |
| Phase 4 | Task 4.2: Dokümantasyon | ⬜ | - |
| Phase 4 | Task 4.3: Git Tag | ⬜ | - |

**OVERALL:** ⬜⬜⬜⬜⬜⬜⬜⬜⬜ 0/9 tasks

---

## 📁 DOSYA YAPISI

```
/MLP/
├── TODO_STAGE1_TO_SELFHOSTING_FINAL.md  ← BU BELGE
├── TODO_kurallari.md                     ← YZ kuralları
├── YZ_reports/                           ← YZ raporları BURAYA
│   ├── YZ_XX_TAMAMLANDI.md
│   └── NEXT_AI_START_HERE.md
├── compiler/
│   ├── stage0/                           ← DEPRECATED (bootstrap only)
│   └── stage1/                           ← AKTİF (MELP compiler)
├── ARCHITECTURE.md
├── MELP_VISION.md
├── MELP_REFERENCE.md
├── pmlp_kesin_sozdizimi.md
└── archive/                              ← Eski dosyalar
```

---

## 🎯 YZ'LER İÇİN HIZLI BAŞLANGIÇ

```bash
# 1. Bu dosyayı oku (zaten okudun)
# 2. TODO_kurallari.md oku
# 3. YZ_reports/NEXT_AI_START_HERE.md oku (varsa)
# 4. Branch oluştur
git checkout -b selfhosting_YZ_XX

# 5. Görevi yap
# 6. Rapor yaz
# YZ_reports/YZ_XX_TAMAMLANDI.md

# 7. NEXT_AI güncelle
# 8. Push et (MERGE ETME!)
git push origin selfhosting_YZ_XX
```

---

## ⚠️ SON UYARI

**Bu TODO tamamlandığında:**
- MELP self-hosting olacak
- Stage 0 deprecated olacak
- Yeni TODO YAZILMAYACAK

**Eğer bu TODO'yu tamamlamadan yeni yön önerirsen:**
- ❌ Görev durdurulur
- ❌ Sonsuz döngüye geri dönülür

**TEK YOL:** Bu TODO'yu bitir! 🎯

---

**Son Güncelleme:** 23 Aralık 2025  
**Yazan:** ÜA_01  
**Onaylayan:** Kullanıcı  
**Durum:** 🔵 AKTİF

---

🚀 **TÜNELDEN ÇIKIŞ ZAMANI!** 🚀

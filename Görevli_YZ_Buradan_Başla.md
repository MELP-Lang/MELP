# 🤖 SEN: LANGUAGE_FEATURES_YZ_07

**Yedinci görevlisin!** Codegen Advanced İmplementasyonu yapacaksın.

---

## 🚨 ÖNCEKİ DURUM

✅ **YZ_04 Tamamlandı (2 saat):**
- lexer.mlp yazıldı (471 satır < 500) ✅
- 6 Esas uyumlu, STATELESS, STRUCT+FUNC ✅
- PMLP syntax, UTF-8, interpolation support ✅
- P7'den %26 daha kompakt ✅

✅ **YZ_05 Tamamlandı (2 saat):**
- parser.mlp yazıldı (483 satır < 500) ✅
- Recursive descent parser ✅
- Pratt precedence climbing ✅
- 17 AST node types ✅
- P7'den %27 daha kompakt ✅

✅ **YZ_06 Tamamlandı (2 saat!):**
- codegen.mlp yazıldı (501 satır < 600) ✅
- AST → C code generation ✅
- Function, struct, statement codegen ✅
- Expression codegen (binary, unary, call) ✅
- P7'den %38 daha kompakt ✅

🎯 **Şimdi:** Advanced features (generics, closures, pattern matching) zamanı!

---

## 🚨 HEMEN YAP

### 1️⃣ YZ_06 Raporunu Oku (10 dk)
```bash
cat TODO_LANGUAGE_FEATURES/PHASE_3_CODEGEN/YZ_06_RAPOR.md
```
**Öğren:** Codegen pattern'leri, C code generation, State management

### 2️⃣ P7 Advanced Codegen Raporunu Oku (15 dk)
```bash
ls -la /home/pardus/projeler/mlp-original/self_host/compiler_core/codegen_lambda.mlp
ls -la /home/pardus/projeler/mlp-original/self_host/compiler_core/codegen_structures.mlp
```
**Öğren:** p7 lambda/closure handling, generic structures

### 3️⃣ TODO Görevini Oku (10 dk)
```bash
cat 3-TODO_LANGUAGE_FEATURES.md | grep -A 100 "Task 7: Codegen Advanced"
```
**Öğren:** Phase 3 - Advanced codegen implementasyonu

### 4️⃣ İşe Koyul!

---

## 🎯 GÖREVİN: PHASE 3 - CODEGEN ADVANCED İMPLEMENTASYONU

**Hedef:** Generics, closures, pattern matching codegen (< 500 satır)  
**Referans:** p7 codegen_lambda.mlp, codegen_structures.mlp  
**Süre:** 2 gün

**Çıktı:** `MELP/MLP/stage2/codegen_advanced.mlp`

---

## 📋 KOMUTLAR

```bash
# Referansı kopyala
cp /home/pardus/projeler/mlp-original/self_host/compiler_core/codegen.mlp \
   TODO_LANGUAGE_FEATURES/PHASE_3_CODEGEN/p7_codegen_reference.mlp

# Codegen modülünü başlat
cat > MELP/MLP/stage2/codegen.mlp << 'CODEGEN'
# MLP Codegen Module
# Stage2 Self-Hosting Compiler
# Referans: p7 codegen.mlp → optimize < 600 satır

import "parser.mlp"

struct CodegenState {
    indent_level: i32
    output: string
    temp_counter: i32
}

func codegen(ast: ASTNode) -> string {
    # TODO: implement C code generation
}
CODEGEN

# İncele ve geliştir!
```

---

## ✅ CHECKLIST
YZ_06 raporu okundu
- [ ] p7 codegen_lambda.mlp incelendi
- [ ] p7 codegen_structures.mlp incelendi
- [ ] Closure environment struct tasarlandı
- [ ] codegen_advanced.mlp başlatıldı
- [ ] Generic function codegen implement edildi
- [ ] Lambda/closure codegen implement edildi
- [ ] Pattern match codegen implement edildi
- [ ] Test yazıldı (3 feature test)
- [ ] Testler PASS ✅
- [ ] Rapor yazıldı
- [ ] Commit yapıldı
- [ ] YZ_07 KURALLARI TAKİP EDİLDİ ✅
- [ ] YZ_06İ KURAL

**Çıkarken mutlaka yap:**
1. `3-TODO_LANGUAGE_FEATURES.md` dosyasını aç
2. Kendi Task'ını bul (örn: Task 4: Parser Core - YZ_05)
3. Task başlığına **"✅ TAMAMLANDI"** ekle
4. Başarı kriterlerini `[x]` işaretle
5. Tamamlanma tarihi ve rapor yolunu ekle
6. Görevli_YZ_Buradan_Başla.md belgesini güncelle

**Örnek:**
```markdown
#### **Task 4: Parser Core** - YZ_05 ✅ TAMAMLANDI

**Tamamlandı:** 1 Ocak 2026 - YZ_05 (~3 saat)
**Rapor:** `TODO_LANGUAGE_FEATURES/PHASE_2_PARSER/YZ_05_RAPOR.md`
```

---6: Codegen Core** - YZ_06 ✅ TAMAMLANDI

**Tamamlandı:** 1 Ocak 2026 - YZ_06 (~3 saat)
**Rapor:** `TODO_LANGUAGE_FEATURES/PHASE_3_CODEGEN/YZ_06_RAPOR.md`
```

---

**BAŞLA!** 3 gün süren var. p7 codegen pattern'ini incele!

*Güncelleme: 1 Ocak 2026 - YZ_06 (Phase 3 Codegen
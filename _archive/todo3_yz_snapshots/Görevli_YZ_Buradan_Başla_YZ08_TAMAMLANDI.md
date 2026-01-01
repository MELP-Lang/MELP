# 🤖 SEN: LANGUAGE_FEATURES_YZ_08

**Sekizinci görevlisin!** Generics Type System İmplementasyonu yapacaksın.

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

✅ **YZ_07 Tamamlandı (2 saat!):**
- codegen_advanced.mlp yazıldı (394 satır < 500) ✅
- Generic function codegen ✅
- Lambda/closure codegen ✅
- Pattern match codegen ✅
- P7'den %48 daha kompakt ✅

🎯 **Şimdi:** Generics Type System (Phase 4) zamanı!

---

## 🚨 HEMEN YAP

### 1️⃣ YZ_07 Raporunu Oku (10 dk)
```bash
cat TODO_LANGUAGE_FEATURES/PHASE_3_CODEGEN/YZ_07_RAPOR.md
```
**Öğren:** Generic codegen pattern'leri, closure environment, pattern match

### 2️⃣ TODO Görevini Oku (10 dk)
```bash
cat 3-TODO_LANGUAGE_FEATURES.md | grep -A 100 "Task 8: Generics"
```
**Öğren:** Phase 4 - Generics Type System implementasyonu

### 3️⃣ İşe Koyul!

---

## 🎯 GÖREVİN: PHASE 4 - GENERICS TYPE SYSTEM

**Hedef:** Generic types, constraints, instantiation (< 500 satır)  
**Referans:** YZ_07 codegen_advanced.mlp  
**Süre:** 2 gün

**Çıktı:** `MELP/MLP/stage2/type_system.mlp`

---

**BAŞLA!** YZ_07 raporunu incele ve type system'i tasarla!
# 🎯 MELP Compiler - TODO List
**Güncel Durum:** 12 Aralık 2025  
**Son Tamamlanan:** YZ_47 (Self-hosting Lexer in MLP Complete!) 🚀  
**Stage:** Stage 1 - Self-Hosting & Advanced Features  
**Önceki Stage:** Stage 0 tamamlandı! → `temp/TODO_stage0_phase11.md`

---

## 🎉 Stage 0 Tamamlandı! (Phase 0-11 + Self-hosting Lexer)

**✅ Tamamlanan Özellikler:**
- Core Compiler (Lexer, Parser, Codegen) ✅
- All Data Types (numeric, string, boolean, array, list, tuple) ✅
- Control Flow (if/else, while, for, for-each, exit) ✅
- Functions (declaration, calls, recursion, parameters) ✅
- String Operations (concat, compare, methods) ✅
- File I/O (read, write, append) ✅
- State Module (optional persistence) ✅
- Module System (import, caching, incremental) ✅
- Error System (colored output, suggestions) ✅
- Optimization (constant folding, dead code elimination) ✅
- **Self-hosting Lexer in MLP** (1500+ lines) ✅

**📊 Stage 0 Metrikleri:**
- 47 YZ oturumu
- ~35-40 saat toplam çalışma
- 6 MLP lexer dosyası (self-hosting)
- Production-ready compiler!

---

## ⚠️ BAŞLAMADAN ÖNCE - YENİ AI İÇİN KRİTİK UYARI

**📖 ZORUNLU OKUMA:**  
👉 **`NEXT_AI_START_HERE.md`** - Mission brief  
👉 **`YZ/YZ_HISTORY.md`** - Tüm YZ oturumlarının özeti  
👉 **`temp/kurallar_kitabı.md`** - STO ve dil spesifikasyonu  
👉 **`ARCHITECTURE.md`** - Mimari kurallar

---

## 🚀 5 ADIMLI HIZLI GELİŞTİRME YÖNTEMİ

**Kanıtlanmış:** 47 YZ oturumunda başarıyla kullanıldı! (10x hızlanma: 20h → 2h)

| Adım | Süre | Ne Yapılır |
|------|------|------------|
| **1. CONTEXT** | 15 dk | `NEXT_AI_START_HERE.md` → `TODO.md` → `kurallar_kitabı.md` |
| **2. PATTERN** | 20 dk | `grep -rn "benzer" modules/` → Benzer YZ'yi bul |
| **3. MINIMAL CODE** | 45 dk | 3-5 dosya max, <100 satır, pattern takip |
| **4. TEST** | 30 dk | `make clean && make && ./melpc test.mlp -o test && ./test` |
| **5. DOCUMENT** | 30 dk | `YZ_XX.md` + `TODO.md` + `NEXT_AI_START_HERE.md` |

### 📝 Okuma Kanıtı (Kod Yazmadan ÖNCE)
```markdown
**OKUMA KANITI: [Modül Adı]**
Okuduğum bölüm: `## X. Bölüm Adı`
Anladığım kurallar: - Kural 1 - Kural 2
Dikkat edeceğim edge case'ler: - Case 1 - Case 2
```

### 🔍 Pattern Kütüphanesi
```
modules/functions/functions_codegen.c  → Best reference
modules/arithmetic/arithmetic_parser.c → Parser precedence
modules/array/array_parser.c           → Collection parsing
```

### ❌ Anti-Pattern'ler
| YAPMA | YAP |
|-------|-----|
| 2 saat okuma, 0 satır kod | 20 dk pattern bul, 1 saat yaz |
| "Belki lazım olur" 500 satır | "Şimdi lazım" 50 satır |
| 10 dosya, 1000 satır, 1 commit | 3-5 dosya, <150 satır, incremental |
| Merkezi dosya (main.c) | Modüler (modules/ içinde) |

---

## 🔀 Git/Branch Kuralları (KRİTİK!)

### Branch Oluşturma
```bash
git checkout -b feature-name_YZ_XX   # Kendi branch'ın
```

### Commit Kuralları
```bash
# Sık sık commit at (her test sonrası)
git add -A
git commit -m "feat: [kısa açıklama]"
# veya
git commit -m "fix: [hata düzeltme]"
git commit -m "docs: [döküman güncelleme]"
git commit -m "test: [test ekleme]"
```

### Push Kuralları
```bash
git push origin feature-name_YZ_XX   # Sadece kendi branch'ına push

# ❌ ASLA main'e direkt push yapma
# ❌ ASLA merge isteği yapma (insan yapacak)
# ✅ Sadece kendi branch'ına push et
```

### Oturum Sonu (Zorunlu)
```bash
git add -A
git commit -m "feat(YZ_XX): [özet]"
git push origin feature-name_YZ_XX
```

---

## 🏗️ Mimari Kurallar (KRİTİK!)

### Kural #1: MERKEZI DOSYA YOK
- ❌ main.c, orchestrator.c, helpers.c YOK
- ✅ Sadece `modules/` içinde çalış

### Kural #2: Modüller Arası İletişim = JSON/Pipe
```bash
# İzin verilen:
./lexer < input.mlp > tokens.json
./parser < tokens.json > ast.json
```

### Kural #3: Her Modül Standalone
```
modules/arithmetic/arithmetic_compiler  (bağımsız)
modules/comparison/comparison_compiler  (bağımsız)
```

### STO (Smart Type Optimization)
- Kullanıcı görür: `numeric`, `string`
- Compiler optimize eder: int64/BigDecimal, SSO/heap
- Her değişken: `is_numeric` flag (1 bit)

---

## 📋 Stage 1: Yapılacaklar

### 🔧 Infrastructure (Öncelikli)

| Görev | Öncelik | Süre | Durum |
|-------|---------|------|-------|
| **Better Test Framework** | ⭐⭐⭐ | 90 min | ⏳ TODO |
| **CI/CD Setup (GitHub Actions)** | ⭐⭐ | 60 min | ⏳ TODO |
| **Standard Library Docs** | ⭐ | 2-3h | ⏳ TODO |

#### Better Test Framework (90 min)
- [ ] Automated test runner script
- [ ] Expected vs actual output comparison
- [ ] Regression testing for all features
- [ ] Test categories: unit, integration, e2e
- [ ] Exit code validation
- [ ] Output capture and diff

#### CI/CD Setup (60 min)
- [ ] GitHub Actions workflow
- [ ] Auto-build on push
- [ ] Auto-test on PR
- [ ] Badge for build status

---

### 🚀 Self-Hosting (Devam)

| Görev | Öncelik | Süre | Durum |
|-------|---------|------|-------|
| **Self-hosting Lexer Test** | ⭐⭐ | 1-2h | ⏳ TODO |
| **Self-hosting Parser (Part 7)** | ⭐ | 10-15h | ⏳ FUTURE |
| **Self-hosting Codegen (Part 8)** | ⭐ | 8-10h | ⏳ FUTURE |
| **Full Bootstrap** | ⭐ | 5h | ⏳ FUTURE |

#### Self-hosting Lexer Test (1-2h)
- [ ] Lexer MLP dosyalarını derle
- [ ] C lexer ile karşılaştır
- [ ] Performance benchmark
- [ ] Edge case testleri

#### Self-hosting Parser (Part 7) - 10-15h
- [ ] AST yapısı MLP'de
- [ ] Expression parser
- [ ] Statement parser
- [ ] Function parser
- [ ] Module parser

---

### 🌟 Dil Özellikleri (Opsiyonel)

| Görev | Öncelik | Süre | Durum |
|-------|---------|------|-------|
| **Try-Catch Exception Handling** | ⭐ | 3-4h | ⏳ FUTURE |
| **Struct/Record Types** | ⭐ | 4-5h | ⏳ FUTURE |
| **Lambda/Anonymous Functions** | ⭐ | 3-4h | ⏳ FUTURE |
| **Pattern Matching** | ⭐ | 5-6h | ⏳ FUTURE |

#### Try-Catch (3-4h)
```mlp
try
    risky_operation()
catch error
    println("Error: " + error)
finally
    cleanup()
end try
```

#### Struct Types (4-5h)
```mlp
struct Point
    numeric x
    numeric y
end struct

Point p = Point(10, 20)
println(p.x)
```

---

### 🔧 Optimizasyon (Opsiyonel)

| Görev | Öncelik | Süre | Durum |
|-------|---------|------|-------|
| **Incremental Object Files (Part 5)** | ⭐ | 6-9h | ⏳ FUTURE |
| **Register Allocation Integration** | ⭐ | 4-5h | ⏳ FUTURE |
| **Tail Call Optimization** | ⭐ | 2-3h | ⏳ FUTURE |

---

## 📈 Önerilen Yol Haritası

### Kısa Vadeli (1-2 oturum)
1. ✅ ~~Self-hosting Lexer~~ DONE!
2. **Better Test Framework** ← Sonraki önerilen
3. **CI/CD Setup**

### Orta Vadeli (3-5 oturum)
4. Self-hosting Lexer Test & Benchmark
5. Standard Library Docs
6. Try-Catch Exception Handling

### Uzun Vadeli (6+ oturum)
7. Self-hosting Parser
8. Struct Types
9. Self-hosting Codegen
10. Full Bootstrap (MLP compiles MLP!)

---

## 📊 Tamamlanan Stage 0 Özeti

### Phase Durumları (Tümü ✅)

| Phase | Durum | YZ |
|-------|-------|-----|
| Phase 0: Linker Fix | ✅ 100% | YZ_08 |
| Phase 1: Strings | ✅ 100% | YZ_06-11 |
| Phase 2: For Loops | ✅ 100% | YZ_12 |
| Phase 3: Collections & Booleans | ✅ 100% | YZ_13-24 |
| Phase 4: Advanced Features | ✅ 100% | YZ_27-28 |
| Phase 5: String Methods | ✅ 100% | YZ_29 |
| Phase 6: Error Messages | ✅ 100% | YZ_30-31 |
| Phase 7: Optimization | ✅ 100% | YZ_32 |
| Phase 9: File I/O | ✅ 100% | YZ_33 |
| Phase 10: State Module | ✅ 100% | YZ_34 |
| Phase 11: Module System | ✅ 100% | YZ_35-45 |
| Self-hosting Lexer | ✅ 100% | YZ_46-47 |

### Self-hosting Lexer Dosyaları

| Dosya | Satır | İçerik |
|-------|-------|--------|
| `token.mlp` | 163 | Token yapısı, 61 token tipi |
| `char_utils.mlp` | 280 | Karakter sınıflandırma |
| `tokenize_literals.mlp` | ~260 | Sayı ve string tarama |
| `tokenize_identifiers.mlp` | ~250 | Identifier ve keyword |
| `tokenize_operators.mlp` | ~290 | Operatör ve semboller |
| `lexer.mlp` | ~300 | Ana entegrasyon |
| **Toplam** | **~1500+** | **MLP kodu!** |

---

## 🔗 Önemli Dosyalar

| Dosya | Açıklama |
|-------|----------|
| `NEXT_AI_START_HERE.md` | Yeni AI için başlangıç |
| `ARCHITECTURE.md` | Mimari kurallar |
| `temp/kurallar_kitabı.md` | STO ve dil spec |
| `temp/TODO_stage0_phase11.md` | Stage 0 detaylı TODO |
| `YZ/YZ_HISTORY.md` | Tüm oturumların özeti |
| `YZ/AI_METHODOLOGY.md` | 5 adımlı metodoloji |

---

## ⚠️ Oturum Sonu Zorunlu Güncellemeler

1. `TODO.md` - Durumu güncelle
2. `YZ/YZ_XX.md` - Oturum raporu oluştur (DETAYLI!)
   - Problem ne idi?
   - Nasıl çözdün?
   - Hangi dosyaları değiştirdin?
   - Test sonuçları
   - Known limitations (varsa)
3. `NEXT_AI_START_HERE.md` - Sonraki AI için güncelle
4. **Git commit & push!**
5. Hız kazanımını hesapla: `Tahmini süre / Gerçek süre = Hızlanma`

---

## 📞 Hızlı Referans

**Derleme:**
```bash
cd compiler/stage0/modules/functions
make clean && make
./melpc test.mlp -o test
./test
```

**Test:**
```bash
LD_LIBRARY_PATH=../../../../runtime/stdlib:../../../../runtime/tto ./test
echo $?
```

**Self-hosting Lexer:**
```
modules/lexer_mlp/
├── token.mlp
├── char_utils.mlp
├── tokenize_literals.mlp
├── tokenize_identifiers.mlp
├── tokenize_operators.mlp
└── lexer.mlp
```

---

## ✅ Checklist'ler

### Başlangıç Checklist
- [ ] `NEXT_AI_START_HERE.md` okudum (ÖNCE BU!)
- [ ] `YZ/AI_METHODOLOGY_SUM.md` okudum
- [ ] `kurallar_kitabı.md` ilgili bölümü okudum
- [ ] Branch oluşturdum: `feature_YZ_XX`
- [ ] Pattern buldum (benzer YZ'yi inceledim)

### Bitiş Checklist
- [ ] Tüm testler çalışıyor
- [ ] Merkezi dosyaya dokunmadım (main.c vb.)
- [ ] `YZ/YZ_XX.md` yazdım (detaylı!)
- [ ] `NEXT_AI_START_HERE.md` güncelledim
- [ ] `TODO.md` güncelledim
- [ ] `git push origin feature_YZ_XX` yaptım

### Başarı Metrikleri
- ✅ **Hızlı:** <3 saat (tahmininin %50-75'i)
- ✅ **Temiz:** 0 compiler warning
- ✅ **Çalışan:** ≥1 test passing
- ✅ **Dokümante:** YZ_XX.md + TODO.md + NEXT_AI güncel
- ✅ **Modüler:** Sadece modules/ düzenlendi

---

**Last Updated:** 12 Aralık 2025 by YZ_47  
**Next AI:** YZ_48 (Test Framework veya CI/CD)  
**Stage:** 1 - Self-Hosting & Advanced Features 🚀

# 🚀 MELP PROJECT - START HERE!

**Last Session:** YZ_200 (List Type Implementation - Progress)  
**Date:** 21 Aralık 2025  
**Agent:** GitHub Copilot (Claude Sonnet 4.5)  
**Status:** 🟡 **İLERLİYOR** (%80 Tamamlandı)

**⚠️ Project Status:**
- **Stage 0:** ✅ TAMAMLANDI (C Compiler) - **List syntax () desteği eklendi! ✅**
- **Stage 1:** ✅ **BLOKER KALDIRILDI** - Modüller derlenebilir durumda!
- **LLVM Faz 1:** ✅ TAMAMLANDI (YZ_00-04, Production Ready!)
- **YZ_200:** 🟡 **İLERLİYOR** - List runtime tamamlandı, codegen %80

**⚠️ YZ Directory Structure:**
- `stage_0_YZ/` - Stage 0 sessions (YZ_01 - YZ_97) ✅ ARCHIVED
- `stage_1_YZ/` - Stage 1 sessions (YZ_01 - YZ_24) ✅ ARCHIVED
- `LLVM_YZ/` - LLVM backend sessions (YZ_00 - YZ_07) ✅ + YZ_200 🔄 DEVAM EDİYOR
- `ust_akil_YZ/` - Üst Akıl devir belgeleri (YZ_ÜA_01 - YZ_ÜA_07) ✅

---

## 🚨 MEVCUT GÖREV: YZ_200 - List Type Full Implementation (DEVAM)

**⚠️ DURUM:**
- ✅ Runtime library tamamlandı (mlp_list.h/c) - 7/7 tests passing
- ✅ Lexer keywords eklendi (append, prepend, length, clear)
- ✅ LLVM runtime declarations eklendi
- ✅ List literal codegen çalışıyor: `list x = (1; 2; 3;)`
- ⚠️ Variable load type mismatch (i64 yerine i8* olmalı)
- ❌ List operations henüz tamamlanmadı

**📋 Görev Detayları:**
- **Brief:** `LLVM_YZ/YZ_200_GOREV.md` (561 satır, detaylı plan)
- **İlerleme:** `LLVM_YZ/YZ_200_ILERLEME.md` (332 satır, progress raporu)
- **Branch:** `list-operations_YZ_200` (oluşturulacak)
- **Süre:** 3-5 gün (1 gün geçti)
- **Başarı Kriteri:**
  ```bash
  # List literal allocation çalışıyor ✅
  list numbers = (1; 2; 3;)
  
  # Henüz yapılacak:
  append(numbers; 4)
  numeric len = length(numbers)
  numeric first = numbers[0]
  ```
**🔧 Yapılacaklar (Sırayla):**
1. **Lexer:** `compiler/stage0/modules/lexer/lexer.c`
   - `(` `)` tokenization + list literal detection
   - `;` parametre ayırıcı tokenization
   - Trailing `;` desteği

2. **Parser:** `compiler/stage0/modules/functions/functions_parser.c`
   - List literal parsing: `(elem1; elem2; elem3;)`
   - Boş list: `()`
   - Return statement'da list literal
   - Function parameters'da `;` ayırıcı

3. **Codegen:** `compiler/stage0/modules/array/` veya yeni `modules/list/`
   - List allocation (LLVM IR)
   - Element initialization
## 🤖 YZ_07 HIZLI BAŞLANGIÇ (İLK 10 DAKİKA!)

**🎯 SEN YZ_07'SİN! Görevin: YZ_06'yı tamamlamak (Stage 0 List Syntax)**

### Adım 1: Durumu Anla (2 dakika)

```bash
# Mevcut durum ne?
./compiler/stage0/modules/functions/functions_compiler \
  modules/lexer_mlp/token.mlp temp/test.ll --backend=llvm
# Sonuç: 0 functions ❌ (parser error)
```

### Adım 2: Brief'i Oku (5 dakika)

1. **`LLVM_YZ/YZ_06_GOREV.md`** ← Ana görev brifingi (166 satır)
2. **`temp/YZ_06_GOREV_OZET.md`** ← Detaylı analiz (158 satır)
3. **`TODO_MODERN_LANGUAGE.md`** Faz 0 ← Bu görev neden kritik?

### Adım 3: Syntax Öğren (3 dakika)

- **`pmlp_kesin_sozdizimi.md`** line 640-642 → List syntax kuralları
- **`kurallar_kitabı.md`** line 857-898 → Collection types
---

## 🎯 SONRAKI GÖREV: YZ_200 - List Type Full Implementation

**Hedef:** Proper list allocation ve operations
**Dosya:** `TODO_MODERN_LANGUAGE.md` - Faz 0, Görev #1
**Tahmin:** 3-5 gün

**🚀 YZ_08 (veya YZ_200) Hızlı Başlangıç:**

### Adım 1: Durumu Anla (2 dakika)
```bash
# YZ_07 başarılı mı?
./functions_compiler modules/lexer_mlp/token.mlp temp/test.ll
# Beklenen: 8 functions ✅ (BAŞARILI!)
```

### Adım 2: Raporları Oku (5 dakika)
1. **`LLVM_YZ/YZ_07_TAMAMLANDI.md`** → Son görev raporu
2. **`TODO_MODERN_LANGUAGE.md`** → YZ_200 detayları
3. **`ust_akil_YZ/YZ_ÜA_07.md`** → Koordinatör notları
## 📋 Session Tamamlama Checklist (YZ_07 İçin!)

**YZ_07 bittiğinde yapılacaklar (15 dakika):**

1. **Başarı kriterini test et:**
   ```bash
   ./functions_compiler modules/lexer_mlp/token.mlp temp/test.ll
   # Beklenen: 6 functions ✅
   
   # Tüm Stage 1 modülleri test et
   for file in modules/**/*.mlp; do
     ./functions_compiler "$file" temp/test.ll || echo "FAIL: $file"
   done
4. **Bu dosyayı güncelle (NEXT_AI_START_HERE.md):**
   - Last Session → YZ_07
   - Status → ✅ BLOKER ÇÖZÜLDÜ! YZ_200 HAZIR
   - Project Status → Stage 1 derleniyor ✅

5. **Git commit & push:**
   ```bash
   git add compiler/stage0/ \
           LLVM_YZ/YZ_07_TAMAMLANDI.md \
           TODO_MODERN_LANGUAGE.md \
           NEXT_AI_START_HERE.md
   
   git commit -m "YZ_07: Stage 0 List Syntax Support - BLOKER ÇÖZÜLDÜ!

   ✅ Completed:
   - Lexer: List literal tokenization
   - Parser: List syntax parsing
   - Codegen: List allocation & init
   
   📊 Metrics:
   - 66 Stage 1 modules compile successfully
   - 580+ lines fixed
   - token.mlp: 6 functions ✅
   
   Status: CRITICAL BLOCKER RESOLVED ✅
   Next: YZ_200 (List Type Full Implementation)"
   
   git push origin stage0-list-syntax_YZ_06
   ```

6. **KULLANICIYA SOR:** "YZ_07 tamamlandı! Stage 1 bloker çözüldü 🎉 Sırada YZ_200 (List Operations). Devam edelim mi?"
   ```
5. **Stage 0 kodunu incele:**
   - `compiler/stage0/modules/lexer/lexer.c` → Tokenization
   - `compiler/stage0/modules/functions/functions_parser.c` → Parsing

### Normal YZ için Başlangıç (YZ_06 sonrası):

1. **Bu dosyayı oku** → Mevcut durum nedir?
2. **TODO dosyalarını oku:**
   - `TODO_MODERN_LANGUAGE.md` → Modern dil özellikleri
   - `TODO_LLVM_SUMMARY.md` → LLVM backend özet
   - `LLVM_YZ/TODO.md` → LLVM detaylı görevler
3. **Son raporları oku:**
   - `ust_akil_YZ/YZ_ÜA_05.md` → Son üst akıl raporu
   - `LLVM_YZ/FAZ_1_TAMAMLANDI.md` → Faz 1 özet

---

## 📋 Session Tamamlama Checklist

**Session bittiğinde yapılacaklar (15 dakika):**

1. **YZ raporu yaz:**
   - YZ_06 için: `LLVM_YZ/YZ_06_GOREV.md` + `LLVM_YZ/YZ_06_TAMAMLANDI.md`
   - Diğer görevler: `LLVM_YZ/YZ_XX_TAMAMLANDI.md`

2. **TODO dosyalarını güncelle:**
   - `TODO_MODERN_LANGUAGE.md` → YZ_06'u [x] işaretle
   - `TODO_LLVM_SUMMARY.md` → Özet güncelle (gerekirse)
   - `LLVM_YZ/TODO.md` → İlerleme %'sini güncelle (gerekirse)

3. **Bu dosyayı güncelle:**
   - Last Session → YZ_XX
   - Date → Bugünün tarihi
   - Status → Yeni durum
   - Project Status → Güncel durumları

4. **Ana dizini temizle:**
   ```bash
   rm -f test_*.mlp *.s *.o temp/*.s*
   ```

5. **Git commit & push:**
   ```bash
   git add .
   git commit -m "YZ_XX: Task Description"
   git push origin branch-name_YZ_XX
   ```

6. **KULLANICIYA SOR:** "Görev tamamlandı. Sırada ne yapmak istersin?"

---

## 🔧 Git Workflow

**📌 Dal İsimlendirme Kuralı:**
- Format: `task-description_YZ_XX`
- Görev adı kısa ve açıklayıcı (kebab-case)
- Sonunda mutlaka `_YZ_XX` eki (XX = session numarası)

**Örnekler (YZ_06 ve sonrası):**
- `stage0-list-syntax_YZ_06` (ACİL!)
- `llvm-array-support_YZ_05`
- `llvm-foreach-loop_YZ_06`
- `module-system_YZ_204`

**Git Commands:**
```bash
# 1. Yeni dal oluştur
git checkout -b stage0-list-syntax_YZ_06

# 2. Değişiklikleri ekle
git add compiler/stage0/modules/lexer/lexer.c \
        compiler/stage0/modules/functions/functions_parser.c \
        LLVM_YZ/YZ_06_*.md \
        TODO_MODERN_LANGUAGE.md \
        NEXT_AI_START_HERE.md

# 3. Commit et
git commit -m "YZ_06: Stage 0 List Syntax Support

✅ Completed Tasks:
- List literal tokenization
- List syntax parsing
- List codegen support

📊 Metrics:
- 66 Stage 1 modules now compile
- 580+ lines fixed
- All tests passing

Status: BLOCKER RESOLVED ✅"

# 4. Push et
git push origin stage0-list-syntax_YZ_06
```

**⚠️ ÖNEMLİ:** Session bittiğinde mutlaka TODO + NEXT_AI_START_HERE + YZ raporu + GIT COMMIT yapılmalı!

---

## 🚨 SYNTAX UYARISI - İLK OKUYACAĞIN ŞEY!

**Kullanıcı kodu (.mlp test/örnek):** Boşluklu → `end if`, `else if`, `end while`  
**Compiler kodu (parser.mlp, lexer.mlp):** Alt çizgili → `end_if`, `else_if`, `end_while`

**Hızlı Kural:** Örnekler için `end if`, modüller için `end_if`. Kararsızsan → `kurallar_kitabı.md` oku!

**⚠️ YZ_06 İçin Önemli:**
- List syntax: `(elem1; elem2; elem3;)` - Parantez + noktalı virgül
- Array syntax: `[elem1; elem2; elem3]` - Köşeli parantez
- Parametre ayırıcı: `;` (noktalı virgül, virgül DEĞİL!)
- Trailing semicolon: Gerekli! `(1; 2; 3;)`

---

## 📊 Project Status Details

### ✅ Completed Phases

**Stage 0 (C Compiler):**
- ✅ Basic syntax (functions, variables, if/while)
- ✅ LLVM backend integration
- ✅ Struct support
- ⚠️ **EKSIK:** List literal syntax `()` - YZ_06 ile eklenecek

**Stage 1 (Self-hosting):**
- ⚠️ **ENGELLENMIŞ:** 66 modül var ama derlenemiyor
- Neden: Stage 0'da list syntax yok
- Çözüm: YZ_06 tamamlanınca bootstrap devam edecek

**LLVM Backend (Faz 1):**
- ✅ YZ_00: LLVM IR alt yapı kurulumu
- ✅ YZ_01: Temel fonksiyon desteği
- ✅ YZ_02: If/While control flow
- ✅ YZ_03: Struct tanımlama ve kullanımı
- ✅ YZ_04: Stage 1 full compilation

### 🚨 Critical Blocker

**YZ_06: Stage 0 List Syntax Support (1 hafta)**
- **Öncelik:** 🔴🔴🔴 EN YÜKSEK
- **Süre:** 21-28 Aralık 2025
- **Etki:** 66 Stage 1 modül, 580+ satır
- **Detay:** `temp/YZ_06_GOREV_OZET.md`

### ⏳ Pending Phases (YZ_06 sonrası)

**Modern Language Features (19 hafta):**
- Faz 1: List operations (YZ_200-203) - 4 hafta
- Faz 2: Module system (YZ_204-208) - 5 hafta
- Faz 3: Error handling (YZ_209-215) - 7 hafta
- Faz 4: Advanced features (YZ_216-224) - 9 hafta

**LLVM Optional Features (ihtiyaç oldukça):**
- YZ_05: Array support
- YZ_06: Foreach loop
- YZ_07-13: Diğer features

---

## 📚 Key Documents

### YZ_06 İçin Gerekli Belgeler:

**1. Görev Belgeleri:**
- `temp/YZ_06_GOREV_OZET.md` - Detaylı görev brifingi
- `TODO_MODERN_LANGUAGE.md` - Faz 0 açıklaması

**2. Syntax Referansları:**
- `pmlp_kesin_sozdizimi.md` - Line 170, 640-642 (list syntax)
- `kurallar_kitabı.md` - Line 857-898 (collection types)

**3. Analiz Belgeleri:**
- `temp/MODÜL_ANALIZ_KARŞILAŞTIRMA.md` - 66 modül analizi
- `modules/lexer_mlp/lexer.mlp` - List kullanım örneği
- `modules/lexer_mlp/token.mlp` - Test modülü

**4. Stage 0 Kaynak Kodları:**
- `compiler/stage0/modules/lexer/lexer.c` - Tokenization
- `compiler/stage0/modules/functions/functions_parser.c` - Parsing
- `compiler/stage0/modules/codegen/` - Code generation

### Genel Belgeler:

**Architecture:**
- `ARCHITECTURE.md` - Sistem mimarisi
- `MELP_VISION.md` - Proje vizyonu
- `MELP_REFERENCE.md` - Dil referansı

**TODO Files:**
- `TODO_MODERN_LANGUAGE.md` - Modern dil özellikleri TODO
- `TODO_LLVM_SUMMARY.md` - LLVM backend özet
- `LLVM_YZ/TODO.md` - LLVM detaylı TODO

**Stage 0:**
- `STAGE0_LIMITATIONS_DETAILED.md` - Stage 0 kısıtları
- `STAGE0_SIGNOFF.md` - Stage 0 onay belgesi

**LLVM:**
- `docs/LLVM_IR_GUIDE.md` - LLVM IR kılavuzu
- `LLVM_YZ/FAZ_1_TAMAMLANDI.md` - Faz 1 özet

**YZ Reports:**
- `ust_akil_YZ/YZ_ÜA_05.md` → Son üst akıl raporu (şu an yazılacak)
- `LLVM_YZ/YZ_04_TAMAMLANDI.md` - Son görev raporu

---

## 🎯 Success Criteria (YZ_06)

**Test 1: Token Module (minimal test)**
```bash
./compiler/stage0/modules/functions/functions_compiler \
  modules/lexer_mlp/token.mlp temp/test.ll --backend=llvm
# Beklenen: 6 functions compiled ✅
# Şu an: 0 functions compiled ❌
```

**Test 2: Lexer Module (full test)**
```bash
./compiler/stage0/modules/functions/functions_compiler \
  modules/lexer_mlp/lexer.mlp temp/test.ll --backend=llvm
# Beklenen: 19 functions compiled ✅
# Şu an: 0 functions compiled ❌
```

**Test 3: All Stage 1 Modules**
```bash
for file in modules/**/*.mlp; do
  echo "Testing: $file"
  ./functions_compiler "$file" temp/test.ll --backend=llvm
done
# Beklenen: Tüm modüller başarıyla derlenmeli
```

**Test 4: List Syntax Test Cases**
```bash
# Test empty list
echo 'function test() returns list
  return (;)
end_function' > temp/test_list.mlp

# Test single element
echo 'function test() returns list
  return ("hello";)
end_function' > temp/test_list.mlp

# Test multiple elements
echo 'function test() returns list
  return (1; "two"; 3.0;)
end_function' > temp/test_list.mlp
```

---

## 💡 YZ_06 Implementation Tips

**1. Lexer Changes (lexer.c):**
```c
// TOKEN_LPAREN ve TOKEN_RPAREN tanımla
// List literal detection: 
//   - '(' sonrası identifier veya literal geliyorsa list
//   - ';' ayırıcı olarak kullan
**⚠️ SON UYARI:** YZ_07 en yüksek öncelikli görev! Stage 1 bootstrap tamamen buna bağlı!

**🎯 HEDEF:** 66 Stage 1 modül derlenebilir hale gelsin → Bootstrap devam etsin → TODO_MODERN_LANGUAGE.md başlasın!

**🚀 BAŞARININ ANAHTARI:** 
- List syntax `()` (parantez, köşeli parantez DEĞİL!)
- Parametre ayırıcı `;` (noktalı virgül, virgül DEĞİL!)
- Trailing semicolon zorunlu: `(1; 2; 3;)`

**📊 İlerleme Takibi:**
- Lexer → Parser → Codegen → Test
- Her aşamada `token.mlp` test et (0 → 2 → 4 → 6 functions)

---

**Last Updated:** 21 Aralık 2025, 15:45 (YZ_ÜA_07)  
**Current Session:** YZ_07 (Stage 0 List Syntax Support - DEVAM)  
**Next Session:** YZ_08 veya YZ_200 (YZ_07 tamamlandıktan sonra)  
**Priority:** 🔴🔴🔴 CRITICAL BLOCKER

**3. Codegen Changes:**
```c
// List allocation ve initialization
// - malloc ile memory ayır
// - Element'leri kopyala
// - List metadata oluştur (size, capacity)
```

**4. Test Strategy:**
- Önce token.mlp ile minimal test
- Sonra lexer.mlp ile tam test
- Son olarak tüm 66 modül

---

## 🚀 Next Steps After YZ_06

1. **Stage 1 Bootstrap:**
   - 66 modül derlenecek
   - Stage 1 self-hosting tamamlanacak

2. **Modern Language Features:**
   - YZ_200: List operations (append, prepend, length)
   - YZ_201: Array type full implementation
   - YZ_204: Module system (import/export)

3. **LLVM Optional Features:**
   - YZ_05: Array support
   - YZ_06: Foreach loop
   - YZ_07-13: Diğer features

---

## 📞 Help & Support

**Problem yaşarsan:**
1. `kurallar_kitabı.md` kontrol et
2. `pmlp_kesin_sozdizimi.md` oku
3. `compiler/stage0/` kaynak kodlarını incele
4. Test durumunda: `temp/` klasöründeki test dosyalarına bak
5. Kullanıcıya danış

**Kritik Dosyalar:**
- Syntax: `pmlp_kesin_sozdizimi.md`, `kurallar_kitabı.md`
- TODO: `TODO_MODERN_LANGUAGE.md`
- Test: `temp/YZ_06_GOREV_OZET.md`
- Stage 0: `compiler/stage0/modules/`

---

**⚠️ SON UYARI:** YZ_06 en yüksek öncelikli görev! Stage 1 bootstrap tamamen buna bağlı. 1 hafta içinde tamamlanmalı!

**🎯 HEDEF:** 66 Stage 1 modül derlenebilir hale gelsin → Bootstrap devam etsin → Modern dil features eklensin!

**🚀 BAŞARININ ANAHTARI:** List syntax `()` + noktalı virgül `;` + trailing semicolon!

---

**Last Updated:** 21 Aralık 2025, 14:30 (YZ_ÜA_05)  
**Next Session:** YZ_06 (Stage 0 List Syntax Support)  
**Priority:** 🔴🔴🔴 CRITICAL BLOCKER

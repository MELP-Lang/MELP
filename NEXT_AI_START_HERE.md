# 🚀 MELP PROJECT - START HERE!

**Last Session:** YZ_ÜA_05 (Modern Language Planning)  
**Date:** 21 Aralık 2025  
**Agent:** GitHub Copilot (Claude Sonnet 4.5)  
**Status:** 🚨 **KRİTİK BLOKER - YZ_199 ACİL!**

**⚠️ Project Status:**
- **Stage 0:** ✅ TAMAMLANDI (C Compiler) - **ANCAK list syntax yok! ❌**
- **Stage 1:** 🚨 **ENGELLENMIŞ** - 66 modül derlenemiyor (0 functions)
- **LLVM Faz 1:** ✅ TAMAMLANDI (YZ_00-04, Production Ready!)
- **LLVM Faz 2-4:** ⏳ OPSIYONEL (YZ_199 sonrası)

**⚠️ YZ Directory Structure:**
- `stage_0_YZ/` - Stage 0 sessions (YZ_01 - YZ_97) ✅ ARCHIVED
- `stage_1_YZ/` - Stage 1 sessions (YZ_01 - YZ_24) ✅ ARCHIVED
- `LLVM_YZ/` - LLVM backend sessions (YZ_00 - YZ_04) ✅ FAZ 1 TAMAMLANDI
- `ust_akil_YZ/` - Üst Akıl devir belgeleri (YZ_ÜA_01 - YZ_ÜA_05) ✅

---

## 🚨 ACİL GÖREV: YZ_199 - Stage 0 List Syntax Support

**⚠️ KRİTİK DURUM:**
- 66 Stage 1 modül derlenemiyor (0 functions compiled)
- Neden: Stage 0 parser `()` list literal syntax'ını desteklemiyor
- Etki: 580+ satır, 25 dosya, **tüm Stage 1 bootstrap engellenmiş!**
- Öncelik: 🔴🔴🔴 **EN YÜKSEK - BLOKER**

**📋 Görev Detayları:**
- **Dosya:** `temp/YZ_199_GOREV_OZET.md` (150+ satır görev brifingi)
- **TODO:** `TODO_MODERN_LANGUAGE.md` - Faz 0 bölümü
- **Süre:** 1 hafta (21-28 Aralık 2025)
- **Başarı Kriteri:**
  ```bash
  ./functions_compiler modules/lexer_mlp/token.mlp temp/test.ll
  # Şu an: 0 functions compiled ❌
  # Hedef: 6 functions compiled ✅
  ```

**🔧 Yapılacaklar:**
1. Stage 0 Lexer: `(` ve `)` tokenization + list literal detection
2. Stage 0 Parser: `(elem1; elem2; elem3;)` syntax parsing + `;` parametre ayırıcı
3. Stage 0 Codegen: List allocation + initialization
4. Test: 66 Stage 1 modülün hepsi derlenebilmeli

**📚 Referanslar:**
- `pmlp_kesin_sozdizimi.md` - Line 170, 640-642 (list syntax)
- `kurallar_kitabı.md` - Line 857-898 (collection types)
- `temp/MODÜL_ANALIZ_KARŞILAŞTIRMA.md` - Modül analizi

**⚠️ ÖNEMLİ:** YZ_199 tamamlanmadan Stage 1 bootstrap devam edemez!

---

## 🤖 YZ HIZLI BAŞLANGIÇ (İLK OKUYACAĞIN!)

**📋 DETAYLI CHECKLIST İÇİN:** `YZ_CHECKLIST.md` dosyasını oku!

### YZ_199 için Özel Başlangıç (5 dakika):

1. **Bu dosyayı oku** → Kritik durumu anla
2. **Görev detaylarını oku:**
   - `temp/YZ_199_GOREV_OZET.md` → Tam görev açıklaması
   - `TODO_MODERN_LANGUAGE.md` Faz 0 → Genel bakış
3. **Syntax referanslarını oku:**
   - `pmlp_kesin_sozdizimi.md` line 170, 640-642
   - `kurallar_kitabı.md` line 857-898
4. **Test et:**
   ```bash
   ./compiler/stage0/modules/functions/functions_compiler \
     modules/lexer_mlp/token.mlp temp/test.ll --backend=llvm
   # Sonuç: 0 functions ❌
   ```
5. **Stage 0 kodunu incele:**
   - `compiler/stage0/modules/lexer/lexer.c` → Tokenization
   - `compiler/stage0/modules/functions/functions_parser.c` → Parsing

### Normal YZ için Başlangıç (YZ_199 sonrası):

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
   - YZ_199 için: `LLVM_YZ/YZ_199_GOREV.md` + `LLVM_YZ/YZ_199_TAMAMLANDI.md`
   - Diğer görevler: `LLVM_YZ/YZ_XX_TAMAMLANDI.md`

2. **TODO dosyalarını güncelle:**
   - `TODO_MODERN_LANGUAGE.md` → YZ_199'u [x] işaretle
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

**Örnekler (YZ_199 ve sonrası):**
- `stage0-list-syntax_YZ_199` (ACİL!)
- `llvm-array-support_YZ_05`
- `llvm-foreach-loop_YZ_06`
- `module-system_YZ_204`

**Git Commands:**
```bash
# 1. Yeni dal oluştur
git checkout -b stage0-list-syntax_YZ_199

# 2. Değişiklikleri ekle
git add compiler/stage0/modules/lexer/lexer.c \
        compiler/stage0/modules/functions/functions_parser.c \
        LLVM_YZ/YZ_199_*.md \
        TODO_MODERN_LANGUAGE.md \
        NEXT_AI_START_HERE.md

# 3. Commit et
git commit -m "YZ_199: Stage 0 List Syntax Support

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
git push origin stage0-list-syntax_YZ_199
```

**⚠️ ÖNEMLİ:** Session bittiğinde mutlaka TODO + NEXT_AI_START_HERE + YZ raporu + GIT COMMIT yapılmalı!

---

## 🚨 SYNTAX UYARISI - İLK OKUYACAĞIN ŞEY!

**Kullanıcı kodu (.mlp test/örnek):** Boşluklu → `end if`, `else if`, `end while`  
**Compiler kodu (parser.mlp, lexer.mlp):** Alt çizgili → `end_if`, `else_if`, `end_while`

**Hızlı Kural:** Örnekler için `end if`, modüller için `end_if`. Kararsızsan → `kurallar_kitabı.md` oku!

**⚠️ YZ_199 İçin Önemli:**
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
- ⚠️ **EKSIK:** List literal syntax `()` - YZ_199 ile eklenecek

**Stage 1 (Self-hosting):**
- ⚠️ **ENGELLENMIŞ:** 66 modül var ama derlenemiyor
- Neden: Stage 0'da list syntax yok
- Çözüm: YZ_199 tamamlanınca bootstrap devam edecek

**LLVM Backend (Faz 1):**
- ✅ YZ_00: LLVM IR alt yapı kurulumu
- ✅ YZ_01: Temel fonksiyon desteği
- ✅ YZ_02: If/While control flow
- ✅ YZ_03: Struct tanımlama ve kullanımı
- ✅ YZ_04: Stage 1 full compilation

### 🚨 Critical Blocker

**YZ_199: Stage 0 List Syntax Support (1 hafta)**
- **Öncelik:** 🔴🔴🔴 EN YÜKSEK
- **Süre:** 21-28 Aralık 2025
- **Etki:** 66 Stage 1 modül, 580+ satır
- **Detay:** `temp/YZ_199_GOREV_OZET.md`

### ⏳ Pending Phases (YZ_199 sonrası)

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

### YZ_199 İçin Gerekli Belgeler:

**1. Görev Belgeleri:**
- `temp/YZ_199_GOREV_OZET.md` - Detaylı görev brifingi
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

## 🎯 Success Criteria (YZ_199)

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

## 💡 YZ_199 Implementation Tips

**1. Lexer Changes (lexer.c):**
```c
// TOKEN_LPAREN ve TOKEN_RPAREN tanımla
// List literal detection: 
//   - '(' sonrası identifier veya literal geliyorsa list
//   - ';' ayırıcı olarak kullan
```

**2. Parser Changes (functions_parser.c):**
```c
// parse_list_literal() fonksiyonu ekle
// - '(' bekle
// - Element parse et
// - ';' bekle
// - ')' görülene kadar tekrarla
// - Trailing ';' zorunlu
```

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

## 🚀 Next Steps After YZ_199

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
- Test: `temp/YZ_199_GOREV_OZET.md`
- Stage 0: `compiler/stage0/modules/`

---

**⚠️ SON UYARI:** YZ_199 en yüksek öncelikli görev! Stage 1 bootstrap tamamen buna bağlı. 1 hafta içinde tamamlanmalı!

**🎯 HEDEF:** 66 Stage 1 modül derlenebilir hale gelsin → Bootstrap devam etsin → Modern dil features eklensin!

**🚀 BAŞARININ ANAHTARI:** List syntax `()` + noktalı virgül `;` + trailing semicolon!

---

**Last Updated:** 21 Aralık 2025, 14:30 (YZ_ÜA_05)  
**Next Session:** YZ_199 (Stage 0 List Syntax Support)  
**Priority:** 🔴🔴🔴 CRITICAL BLOCKER

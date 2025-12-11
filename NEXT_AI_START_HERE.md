# MLP Projesi - YZ Başlangıç Rehberi

> **Yeni YZ iseniz sadece bu belgeyi okuyun. Her şey burada.**

**Son Güncelleme:** 11 Aralık 2025, ~02:00 (YZ_46)  
**Son Tamamlanan:** YZ_46 (Self-Hosting Part 6.1 & 6.2 Complete!) 🎉  
**Durum:** Stage 0 - Phase 11 (100%) + Self-Hosting Started! ✅

**✅ Incremental Build:** 10-15x speedup! Parse & assembly skip working!

## 🎯 Hızlı Başlangıç

1. **Kullanıcıya sorun**: "Hangi görev üzerinde çalışmamı istersiniz?"
2. **TODO.md'yi okuyun**: Öncelikli görevler burada
3. **YZ_42.md'yi okuyun**: Son oturumda ne yapıldı?
4. **Çalışın ve belgeleyin**

## 📋 Öncelik Sırası

1. Kullanıcının verdiği görev (en yüksek öncelik)
2. `TODO.md` içindeki görevler
3. Test kapsamını artırma

## 🏗️ Proje Yapısı (Kısa)

```
compiler/stage0/    → Ana derleyici (C)
  modules/         → Modüler bileşenler
    functions/     → Fonksiyon desteği (aktif geliştirme)
    import/        → Module system + caching (YZ_42)
docs/              → İngilizce belgeler
docs_tr/           → Türkçe belgeler
YZ/                → YZ oturum raporları (YZ_01.md ... YZ_XX.md)
```

## ⚡ Temel Komutlar

```bash
# Derleme (functions/ dizininde)
cd /home/pardus/projeler/MLP/MLP/compiler/stage0/modules/functions
make clean && make

# Test
./functions_compiler test.mlp test.s
gcc -o test test.s -L../../../../runtime/stdlib -lmlp_stdlib -L../../../../runtime/tto -ltto_runtime -lm
LD_LIBRARY_PATH=../../../../runtime/stdlib:../../../../runtime/tto ./test
```

## 📜 Kurallar

1. **Türkçe iletişim** (kod ve teknik terimler İngilizce kalabilir)
2. **Bilgi sahibi ol** ilgilendiğin konu ne ise önce onun ile ilgili bilgiye docs_tr/language/melp_syntax.md
dosyasından ulaş. söz dizimini öğren. Kullanıcıya, şu an ki görevim (örneğin list() listeler) "melp_syntax.md" belgesinin listeler bölümünü okudum, söz dizimine hakimim. ilgili bölüm şöyle diyor, benim görevim list() içinde şunları yapmaktır, diye bilgi ver.
3. **Her YZ kendi dalına commit ve push eder** (dal adı: `*_YZ_XX` formatında, yedekler GitHub'da tutulur)
4. **⚠️ KRİTİK: ASLA main branch'e Pull Request GÖNDERME!** 
   - Sadece kendi dalına push yap: `git push origin dal_adi`
   - Main branch'e merge YAPMA
   - Kullanıcı gerekirse kendisi PR oluşturur
5. **Her değişikliği test edin**
6. **Belgeleri güncel tutun**


## ✅ Oturum Sonu Kontrol Listesi

Oturumu bitirmeden önce:

- [ ] `YZ/YZ_XX.md` oluştur (XX = sonraki numara)
- [ ] `TODO.md` güncelle (tamamlananları işaretle, yenileri ekle)
- [ ] `temp/user_todo.md` güncelle (TODO.md ile senkronize tut!) ⚠️
- [ ] Bu dosyayı güncelle (önemli bir değişiklik varsa)
- [ ] Commit ve push yap (dal: `*_YZ_XX`)
- [ ] ⚠️ **KRİTİK:** Sadece kendi dalına push yap, main'e PR GÖNDERME!

## 📁 Önemli Dosyalar

| Dosya | Amaç |
|-------|------|
| `TODO.md` | Görev listesi |
| `ARCHITECTURE.md` | Proje mimarisi + Tasarım Felsefesi |
| `YZ/YZ_XX.md` | Oturum raporları |
| `compiler/stage0/modules/functions/` | Aktif geliştirme alanı |

## 🔢 Son YZ Numarası

**YZ_46** - Self-Hosting Part 6.1 & 6.2 Complete! 🎉 
- ✅ Part 6.1: Token Structure (61 token types)
- ✅ Part 6.2: Character Classification (10 functions)
- ✅ 550+ lines of MLP code!
- ✅ Token structure: `[type, value, line, column]`
- ✅ Character classification: digit, alpha, whitespace, symbols
- ⚠️ Tests pending (function parameter binding issue)

---

## 🎯 Mevcut Durum (11 Aralık 2025, 02:00)

### Tamamlanan Phase'ler:
- ✅ **Phase 1-3**: Strings, For Loops, Collections, Booleans (100%)
- ✅ **Phase 4**: While loops, For-each, Exit system (100%)
- ✅ **Phase 5**: String methods - toUpperCase, toLowerCase, trim, etc. (100%)
- ✅ **Phase 6**: Error messages, "Did you mean", Division by zero, Error Recovery (100%)
- ✅ **Phase 7**: Constant Folding, Dead Code Elimination, Register Allocation (100%)
- ✅ **Phase 9**: File I/O - read_file, write_file, append_file (100%)
- ✅ **Phase 10**: State Module - Persistent state management (100%)
- ✅ **Phase 11**: Module System + Incremental Compilation (100%)!
- 🔵 **Phase 12**: Self-Hosting - Lexer in MLP (Progress: 33% - Part 6.1 & 6.2 done!)

### YZ_46 Tamamlananlar:
- ✅ **Token Structure** - 61 token types (keyword, literal, operator, symbol)
- ✅ **Token Functions** - create_token, accessors, to_string (7 functions)
- ✅ **Character Classification** - is_digit, is_alpha, is_whitespace, etc. (10 functions)
- ✅ **MLP Code** - 550+ lines of pure MLP code!
- ⚠️ **Tests** - Pending (function parameter binding issue in compiler)
- ✅ **Phase 6**: Error messages, "Did you mean", Division by zero, Error Recovery (100%)
- ✅ **Phase 7**: Constant Folding, Dead Code Elimination, Register Allocation (100%)
- ✅ **Phase 9**: File I/O - read_file, write_file, append_file (100%)
- ✅ **Phase 10**: State Module - Persistent state management (100%)
- ✅ **Phase 11**: Module System + Incremental Compilation (100%)! 🆕

### YZ_45 Tamamlananlar:
- ✅ **Smart Linking** - Skip assembly for up-to-date object files
- ✅ **Incremental Skip** - Skip parsing for unchanged modules  
- ✅ **JSON Parsing Fix** - Robust function name extraction with loop guards
- ✅ **Bug Fixes** - Segfault (dependencies), infinite loop (JSON)
- ✅ **Performance** - 10-15x speedup for unchanged code
- ✅ **Testing** - Full integration test suite passed!

### Sonraki Görevler (Optional):

**🔵 ÖNERİLEN SEÇENEK 1 (YZ_47): Core Compiler Improvements (2-3h) ⭐⭐⭐**
- **Priority:** YÜKSEK (self-hosting için gerekli)
- **Görevler:**
  - Function parameter binding düzelt
  - Global constants desteği ekle
  - Test suite'i çalıştırılabilir hale getir
- **Neden:** Self-hosting devam edebilmesi için gerekli

**🟢 ÖNERİLEN SEÇENEK 2 (YZ_47): Self-Hosting Part 6.3 - Number & String Tokenization (1.5h)**
- **Dosya:** `modules/lexer_mlp/tokenize_literals.mlp`
- **Görev:** scan_number(), scan_string() fonksiyonları
- **Detay:** TODO.md → "Phase 12: Self-Hosting" bölümüne bak
- **Zorluk:** ⭐⭐ ORTA

**🟡 ALTERNATİF (YZ_47): Part 6.4 - Identifier & Keyword Recognition (1h)**
- **Dosya:** `modules/lexer_mlp/tokenize_identifiers.mlp`
- **Görev:** scan_identifier(), is_keyword() fonksiyonları

**📝 Not:** Core compiler improvements öncelikli! Self-hosting için function parameters çalışmalı.

---

## ✅ YZ_33 Tamamlananlar (Phase 9 - File I/O COMPLETE!)

1. **Runtime File I/O Implementation** ✅ (60 min)
   - `mlp_io.h`: 3 yeni fonksiyon declaration
   - `mlp_io.c`: Full implementation
     - `mlp_read_file()` - Reads entire file, returns string
     - `mlp_write_file()` - Writes content, overwrites, returns 1/0
     - `mlp_append_file()` - Appends content, returns 1/0
   - Error handling: Non-existent file, permission denied, etc.

2. **Compiler Integration** ✅ (30 min)
   - `functions.c`: Added to `function_is_builtin()`
   - `arithmetic_parser.c`: Added to builtin function list
   - `functions_codegen.c`: Assembly generation for 3 functions
     - Proper argument passing (filename, content)
     - Return value handling (string for read, numeric for write/append)

3. **Testing & Documentation** ✅ (30 min)
   - `test_file_io_runtime.c`: Comprehensive C tests
     - Test 1: Write and read
     - Test 2: Append functionality
     - Test 3: Non-existent file handling
     - Test 4: Invalid path error
     - Test 5: Multi-line content
   - All tests PASSED ✅
   - `docs_tr/language/melp_syntax.md`: Added File I/O section

**Note:** MLP-level tests pending (functions_compiler statement parsing needs work)

---

## ✅ YZ_32 Tamamlananlar (Phase 7 - Optimization COMPLETE!)

1. **Constant Folding** ✅ (Already implemented!)
   - `arithmetic_optimize.c` zaten vardı ve çalışıyordu
   - Test: `2 + 3` → `movq $5` (compile-time evaluation)
   - String concat folding: `"Hello" + "World"` → `"HelloWorld"`

2. **Dead Code Elimination** ✅ (60 min)
   - `statement_optimize.c` yeni modül oluşturuldu
   - `if false then` → blok tamamen siliniyor
   - Code after `return` → otomatik temizleniyor
   - `while false` → loop tamamen siliniyor
   - Test: Exit code 5 (dead code başarıyla silindi)

3. **Register Allocation Infrastructure** ✅ (30 min)
   - `register_allocator.h/c` oluşturuldu
   - 8 register pool: %r8-%r15
   - Variable → register mapping ready
   - Codegen entegrasyonu: gelecek çalışma (4-5 saat)

---

## ✅ YZ_31 Tamamlananlar (Phase 6 Completion + Yeni Özellikler)

1. **Error Recovery System** ✅
   - `error_in_recovery_mode()`, `error_enter_recovery()`, `error_exit_recovery()`
   - İlk hatadan sonra parse'a devam (sync point: `function` keyword)
   - Birden fazla hata gösterimi
   - Recovery count in summary

2. **Input Fonksiyonları** ✅
   - `input()` - stdin'den string okur
   - `input("prompt")` - prompt ile string okur  
   - `input_numeric()` - stdin'den sayı okur
   - `input_numeric("prompt")` - prompt ile sayı okur
   - Runtime: `mlp_input()`, `mlp_input_prompt()`, `mlp_input_numeric()`, `mlp_input_numeric_prompt()`

3. **String Metodları** ✅
   - `replace(str, old, new)` - ilk eşleşmeyi değiştirir
   - `replaceAll(str, old, new)` - tüm eşleşmeleri değiştirir
   - `split(str, delimiter)` - string'i listeye ayırır
   - Runtime: `mlp_string_replace()`, `mlp_string_replaceAll()`, `mlp_string_split()`

---

## ✅ YZ_30 Tamamlananlar (Phase 6 - Hata Sistemi)

1. **Enhanced Error Module** ✅
   - `error.h` ve `error.c` tamamen yeniden yazıldı
   - Renkli çıktı desteği (terminal auto-detect)
   - `error_set_source()` ile kaynak kod gösterimi
   - `error_report_with_suggestion()` ile "Did you mean" önerileri

2. **Levenshtein Distance** ✅
   - `error_levenshtein_distance()` fonksiyonu
   - `error_find_similar()` ile benzer keyword bulma
   - `MELP_KEYWORDS[]` dizisi ile öneri havuzu

3. **Runtime Error Handling** ✅
   - `mlp_runtime_error()` fonksiyonu eklendi
   - Division by zero kontrolü (codegen seviyesinde)
   - Modulo by zero kontrolü
   - Exit code 43 for runtime errors

---

## 📊 Proje Özeti

| Phase | Durum | Tamamlanma |
|-------|-------|------------|
| Phase 1-3 | ✅ | 100% |
| Phase 4 | ✅ | 100% |
| Phase 5 | ✅ | 100% |
| Phase 6 | ✅ | 100% |
| Phase 7 | ✅ | 100% |
| Phase 9 | ✅ | 100% |
| Phase 10 | ✅ | 100% |
| Phase 11 | ✅ | 100% 🆕 |
| Phase 12 (Self-Hosting) | ⏳ | 0% (Optional) |

**Toplam**: 100% core language + optimization + File I/O + State + Modules + Incremental! 🎉

---

## 🎯 Sonraki YZ İçin Öneriler (YZ_46)

### Seçenek 1: Self-Hosting Başla (ÖNERİLEN) ⭐
**Görev:** Part 6.1 - Token Structure & Basics (1-1.5h)

**Adımlar:**
1. `TODO.md` → "Phase 12: Self-Hosting" bölümünü oku (5 dk)
2. `modules/lexer_mlp/` klasörü oluştur
3. `token.mlp` dosyasını yaz:
   - TokenType enum (KEYWORD, IDENTIFIER, NUMBER, etc.)
   - Token struct (type, value, line, column)
   - `create_token()` constructor
   - `token_to_string()` debug function
4. Test: Token'ları manuel oluştur, yazdır
5. Commit: "YZ_46: Self-Hosting Part 6.1 - Token Structure"

**Neden bu görev?**
- ✅ Kolay başlangıç (1-1.5h)
- ✅ MLP dilinde kod yazma pratiği
- ✅ Self-hosting'e ilk adım
- ✅ Bağımsız test edilebilir

### Seçenek 2: Diğer İyileştirmeler
- Documentation expansion
- Test coverage improvement
- Bug hunting with Valgrind
- Performance profiling

### Seçenek 3: Yeni Özellikler
- Advanced string methods
- Math library (sin, cos, sqrt)
- Date/time functions
- Network I/O (experimental)

**💡 Tavsiye:** Kullanıcıya sor! "Hangi görev üzerinde çalışmamı istersiniz?"

---

*Bu belge kısa ve öz tutulmalıdır. Detaylar için ilgili dosyalara bakın.*

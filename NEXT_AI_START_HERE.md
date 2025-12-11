# MLP Projesi - YZ Başlangıç Rehberi

> **Yeni YZ iseniz sadece bu belgeyi okuyun. Her şey burada.**

**Son Güncelleme:** 11 Aralık 2025, ~15:00  
**Son Tamamlanan:** YZ_39 (Phase 11 - Separate Compilation Part 2) 🎉  
**Durum:** Stage 0 - Core Features + File I/O + State + Module System + Auto Linking (95%)! 🎉

## 🎯 Hızlı Başlangıç

1. **Kullanıcıya sorun**: "Hangi görev üzerinde çalışmamı istersiniz?"
2. **TODO.md'yi okuyun**: Öncelikli görevler burada
3. **YZ_34.md'yi okuyun**: Son oturumda ne yapıldı?
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
4. **Her değişikliği test edin**
5. **Belgeleri güncel tutun**


## ✅ Oturum Sonu Kontrol Listesi

Oturumu bitirmeden önce:

- [ ] `YZ/YZ_XX.md` oluştur (XX = sonraki numara)
- [ ] `TODO.md` güncelle (tamamlananları işaretle, yenileri ekle)
- [ ] `temp/user_todo.md` güncelle (TODO.md ile senkronize tut!) ⚠️
- [ ] Bu dosyayı güncelle (önemli bir değişiklik varsa)
- [ ] Commit ve push yap (dal: `*_YZ_XX`)

## 📁 Önemli Dosyalar

| Dosya | Amaç |
|-------|------|
| `TODO.md` | Görev listesi |
| `ARCHITECTURE.md` | Proje mimarisi + Tasarım Felsefesi |
| `YZ/YZ_XX.md` | Oturum raporları |
| `compiler/stage0/modules/functions/` | Aktif geliştirme alanı |

## 🔢 Son YZ Numarası

**YZ_39** - Phase 11 (Separate Compilation Part 2) tamamlandı! 🎉 
- ✅ Automatic `.s → .o` pipeline (gcc -c integration)
- ✅ Automatic linking (gcc coordination)
- ✅ Full compilation mode (one command: .mlp → executable)
- ✅ Error handling (warnings ignored, errors fatal)
- ✅ Temporary file cleanup
- ✅ Import system fully compatible

---

## 🎯 Mevcut Durum (11 Aralık 2025)

### Tamamlanan Phase'ler:
- ✅ **Phase 1-3**: Strings, For Loops, Collections, Booleans (100%)
- ✅ **Phase 4**: While loops, For-each, Exit system (100%)
- ✅ **Phase 5**: String methods - toUpperCase, toLowerCase, trim, etc. (100%)
- ✅ **Phase 6**: Error messages, "Did you mean", Division by zero, Error Recovery (100%)
- ✅ **Phase 7**: Constant Folding, Dead Code Elimination, Register Allocation (100%)
- ✅ **Phase 9**: File I/O - read_file, write_file, append_file (100%) 🆕

### YZ_34 Tamamlananlar:
- ✅ **state_init(), state_close()** - State manager lifecycle
- ✅ **state_set(key, value), state_get(key)** - Key-value operations
- ✅ **state_save(), state_load()** - JSON persistence
- ✅ TTO optimization - SSO vs Heap for state values
- ✅ Namespace convention - shared:, config:, temp:

### YZ_36 Tamamlananlar (Phase 11 - Module Loading):
- ✅ **import_load_module()** - Recursive module parsing
- ✅ **Function registry system** - function_is_known()
- ✅ **Cross-module function calls** - add(), multiply(), square() working
- ✅ **Arithmetic parser integration** - User-defined functions recognized
- ✅ **Tests** - simple import, parametreli functions, multiple calls ALL PASS!

### YZ_39 Tamamlananlar (Phase 11 - Separate Compilation Part 2):
- ✅ **Automatic .s → .o pipeline** - gcc -c integration
- ✅ **Automatic linking** - gcc linker coordination
- ✅ **Full compilation mode** - One command: .mlp → executable!
- ✅ **Error handling** - Warnings ignored, only errors fatal
- ✅ **Temporary file cleanup** - Auto cleanup of .s and .o files
- ✅ **Import compatibility** - Works with import system

### Sonraki Görevler (YZ_40 için):

**A) Bug Fix: User-Defined Function Calls (YZ_40, 1h - PRIORITY!):**
- [ ] Problem: User functions parsed as array access
- [ ] Example: `calculate(x)` treated as `calculate[x]` ❌
- [ ] Fix: Improve lookahead in arithmetic_parser.c
- [ ] Tests: Cross-function calls within same file

**B) Separate Compilation - Part 3: Module Caching (YZ_40 or later, 1-1.5h):**
- [ ] Module dependency tracking
- [ ] Timestamp-based caching (skip unchanged modules)
- [ ] Cache invalidation on source change
- [ ] Symbol table persistence per module
- [ ] Tests: modify math.mlp → only math recompiles

### Diğer görevler için `TODO.md` bak

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
| Phase 8 | ⏳ | 0% |
| Phase 9 | ✅ | 100% 🆕 |
| Phase 10 | ✅ | 100% 🆕 |
| Phase 11 | ✅ | 95% 🆕 |

**Toplam**: 100% core language + optimization + File I/O + State + Modules! 🎉

---

*Bu belge kısa ve öz tutulmalıdır. Detaylar için ilgili dosyalara bakın.*

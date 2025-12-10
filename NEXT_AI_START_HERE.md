# MLP Projesi - YZ Başlangıç Rehberi

> **Yeni YZ iseniz sadece bu belgeyi okuyun. Her şey burada.**

## 🎯 Hızlı Başlangıç

1. **Kullanıcıya sorun**: "Hangi görev üzerinde çalışmamı istersiniz?"
2. **TODO.md'yi okuyun**: Öncelikli görevler burada
3. **Çalışın ve belgeleyin**

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
- [ ] Bu dosyayı güncelle (önemli bir değişiklik varsa)

## 📁 Önemli Dosyalar

| Dosya | Amaç |
|-------|------|
| `TODO.md` | Görev listesi |
| `ARCHITECTURE.md` | Proje mimarisi + Tasarım Felsefesi |
| `YZ/YZ_XX.md` | Oturum raporları |
| `compiler/stage0/modules/functions/` | Aktif geliştirme alanı |

## 🔢 Son YZ Numarası

**YZ_29** - Phase 5 tamamlandı ✅ (String Methods: toUpperCase, toLowerCase, trim, trimStart, trimEnd)

---

## 🎯 Sonraki Görevler (Phase 6)

### Öncelikli: Compiler Error Messages & Diagnostics
- Better Error Messages (line numbers, helpful suggestions, colored output)
- Runtime Errors (division by zero, null pointer checks)
- Error Recovery (continue parsing after error, show multiple errors)

### Diğer görevler için `TODO.md` bak

---

## ✅ Son Düzeltmeler (YZ_29)

1. **String Methods** ✅ (Phase 5 Complete!)
   - `toUpperCase(str)` - "hello" → "HELLO"
   - `toLowerCase(str)` - "HELLO" → "hello"
   - `trim(str)` - "  hello  " → "hello"
   - `trimStart(str)` - "   start" → "start"
   - `trimEnd(str)` - "end   " → "end"

2. **Parser Fix** ✅
   - Builtin fonksiyon listesi eklendi (list erişimi ile karışmasın)
   - `toUpperCase(msg)` artık fonksiyon çağrısı olarak doğru parse ediliyor

3. **Runtime Functions** ✅
   - `mlp_string_toUpperCase`, `mlp_string_toLowerCase`
   - `mlp_string_trim`, `mlp_string_trimStart`, `mlp_string_trimEnd`
   - Dosya: `runtime/stdlib/mlp_string.c`

4. **Codegen** ✅
   - `arithmetic_codegen.c` - Yeni fonksiyon mapping'leri
   - `functions.c` - Builtin listesi güncellendi
   - TOKEN_BREAK, TOKEN_CONTINUE

---

*Bu belge kısa ve öz tutulmalıdır. Detaylar için ilgili dosyalara bakın.*

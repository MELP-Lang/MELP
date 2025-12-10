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
2. **Her YZ kendi dalına commit ve push eder** (dal adı: `*_YZ_XX` formatında, yedekler GitHub'da tutulur)
3. **Her değişikliği test edin**
4. **Belgeleri güncel tutun**

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

**YZ_27** - While loop düzeltildi ✅ + Tasarım felsefesi belgelendi

---

## 🎯 Sonraki Görevler (Phase 4)

### Öncelikli: For-Each Loop
```mlp
for each item in collection
    print item
end for
```
- Lexer: TOKEN_EACH, TOKEN_IN ekle
- Parser: for_loop_parser.c güncelle
- Codegen: Counter + bounds checking

### Diğer Phase 4 görevleri için `TODO.md` bak

---

## ✅ Son Düzeltmeler (YZ_27)

1. **While Loop Düzeltildi**
   - Bug: Lexer `<` için TOKEN_LANGLE döndürüyordu (tuple için)
   - Fix: Boşluk bazlı heuristic (`x < 3` → TOKEN_LESS)
   - `do` keyword kaldırıldı: `while condition ... end while`

2. **Tasarım Felsefesi Belgelendi**
   - ARCHITECTURE.md: Stateless Architecture + Design Philosophy
   - docs/language/melp_syntax.md: Design Philosophy bölümü
   - docs_tr/language/melp_syntax.md: Tasarım Felsefesi bölümü

---

*Bu belge kısa ve öz tutulmalıdır. Detaylar için ilgili dosyalara bakın.*

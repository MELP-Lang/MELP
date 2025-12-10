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
gcc -no-pie test.s runtime_funcs.c -o test
./test
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
| `ARCHITECTURE.md` | Proje mimarisi |
| `YZ/YZ_XX.md` | Oturum raporları |
| `compiler/stage0/modules/functions/` | Aktif geliştirme alanı |

## 🔢 Son YZ Numarası

**YZ_26** - Dokümantasyon düzeltmeleri (text→string) + While loop sorunu tespit edildi

---

## 🔴 ACİL: While Loop Sorunu

While loop parser'da çalışmıyor. Lexer doğru token üretiyor ama `statement_parser.c` işlemiyor.

**Sorun:** `statement_parser.c` satır 40-86 arası - TOKEN_WHILE case'ine girilmiyor  
**Detay:** `YZ/YZ_26.md` dosyasına bak

---

*Bu belge kısa ve öz tutulmalıdır. Detaylar için ilgili dosyalara bakın.*

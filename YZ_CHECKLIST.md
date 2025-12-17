# ✅ YZ Session Checklist

**Bu dosya her YZ session için standart checklist'tir.**

---

## 📥 SESSION BAŞLANGIÇ (İlk 5 dakika)

### 1. Durum Tespiti

- [ ] `NEXT_AI_START_HERE.md` oku → Mevcut durum nedir?
- [ ] `TODO.md` oku → Hangi task "🏃 CURRENT" işaretli?
- [ ] `stage_1_YZ/` dizinine bak → Hangi YZ_XX.md "READY TO START"?
- [ ] Önceki YZ raporunu oku → Ne tamamlandı, ne kaldı?

**Komut:**
```bash
cat NEXT_AI_START_HERE.md | head -50
grep "🏃\|CURRENT\|⏳" TODO.md
ls -la stage_1_YZ/*.md | tail -5
```

### 2. Görev Netleştirme

- [ ] Görev adı: `YZ_XX - [Görev İsmi]`
- [ ] Görev hedefi: (1 cümle)
- [ ] Beklenen çıktı: (dosyalar, test sonuçları)
- [ ] Tahmini süre: X saat

**Örnek:**
```
Görev: YZ_03 - Control Flow Parsing
Hedef: If/else/while statement parsing ekle
Çıktı: modules/parser_mlp/parser_control.mlp (veya parser_stmt.mlp'ye ekle)
Test: 3+ control flow parsing testi geçmeli
Süre: 4-6 saat
```

### 3. Dosya Kontrolü

- [ ] Ana dizin temiz mi? (`ls test_*.mlp *.s *.o` → boş olmalı)
- [ ] Gerekli modüller var mı? (`modules/parser_mlp/`, `runtime/`, vs.)
- [ ] Test dizini düzenli mi? (`tests/manual/`, `temp/`)

**Komut:**
```bash
ls test_*.mlp *.s *.o 2>/dev/null && echo "⚠️ TEMIZLIK GEREKLI!" || echo "✅ Temiz"
ls modules/parser_mlp/*.mlp
```

---

## 💻 SESSION ÇALIŞMASI (Ana Süre)

### 4. Kod Geliştirme

- [ ] Modül dosyası oluştur/düzenle (`modules/parser_mlp/*.mlp`)
- [ ] Küçük adımlarla ilerle (her 30-50 satırda bir test)
- [ ] Notlar al (`temp/çıktı.md`)

**İlerleme Takibi:**
```
✅ Adım 1: Token constants ekle
✅ Adım 2: If statement parsing
⏳ Adım 3: While loop parsing (devam ediyor...)
```

### 5. Test Geliştirme

- [ ] Test dosyası oluştur (`tests/manual/test_YZ_XX_*.mlp`)
- [ ] Her özellik için en az 1 test
- [ ] Testleri derle ve çalıştır

**Komut:**
```bash
# Derle
./compiler/stage0/modules/functions/functions_standalone \
    modules/parser_mlp/parser_control.mlp temp/parser_control.s

# Çalıştır
LD_LIBRARY_PATH=runtime/stdlib:runtime/sto ./temp/parser_control.s

# Test sonuçları
echo "Test 1: ✅ PASS" >> temp/çıktı.md
echo "Test 2: ❌ FAIL - reason" >> temp/çıktı.md
```

### 6. Debug & Fix

- [ ] Compiler hatalarını düzelt
- [ ] Runtime hatalarını düzelt
- [ ] Test başarısızlıklarını düzelt

**Not:** Stage 0 limitasyonları unutma (no list params, flat var decls, etc.)

---

## 📤 SESSION BİTİŞİ (Son 15 dakika) - ÇOK ÖNEMLİ!

### 7. YZ Raporu Yaz

- [ ] Dosya oluştur: `stage_1_YZ/YZ_XX.md`
- [ ] Bölümler ekle:
  - [ ] Session bilgileri (tarih, durum, önceki/sonraki)
  - [ ] Görev hedefi
  - [ ] Yapılanlar (değişiklikler, eklenen dosyalar)
  - [ ] Test sonuçları (geçen/kalan)
  - [ ] Sorunlar ve çözümler
  - [ ] Öğrenilenler
  - [ ] Sonraki adımlar

**Template:** `stage_1_YZ/YZ_TEMPLATE.md` kullan (eğer varsa)

### 8. TODO.md Güncelle

- [ ] Tamamlanan task'ları `[x]` işaretle
- [ ] İlerleme yüzdesini güncelle (örn: %40 → %50)
- [ ] "🏃 CURRENT" işaretini sıradaki task'a taşı
- [ ] Yeni notlar ekle (eğer varsa)

**Örnek:**
```diff
- [ ] Control flow statements (YZ_03) 🏃 CURRENT
+ [x] Control flow statements (YZ_03) ✅

- [ ] Function declarations (YZ_04)
+ [ ] Function declarations (YZ_04) 🏃 CURRENT

- İlerleme: Stage 1 - %40 Complete
+ İlerleme: Stage 1 - %50 Complete
```

### 9. NEXT_AI_START_HERE.md Güncelle

- [ ] Session numarasını artır (`YZ_03` → `YZ_04`)
- [ ] Tarihi güncelle
- [ ] Status'u güncelle (`YZ_03 Complete ✅, Start YZ_04`)
- [ ] Self-hosting tablosunu güncelle
- [ ] "ŞİMDİ NE YAPACAĞIZ" bölümünü sıradaki göreve ayarla

**Örnek:**
```diff
- **Session:** Stage 1 YZ_03
+ **Session:** Stage 1 YZ_04

- **Status:** ✅ READY - YZ_02 Complete, Start YZ_03
+ **Status:** ✅ READY - YZ_03 Complete, Start YZ_04

  | Parser Control Flow | 🏃 0% | MELP | - | Stage 1 (YZ_03) ⏳ CURRENT |
+ | Parser Control Flow | ✅ 100% | MELP | 187 | Stage 1 (YZ_03) ✅ |
+ | Parser Functions | 🏃 0% | MELP | - | Stage 1 (YZ_04) ⏳ CURRENT |
```

### 10. Sıradaki YZ Hazırla

- [ ] `stage_1_YZ/YZ_XX+1.md` oluştur (eğer yoksa)
- [ ] Status: `🏃 READY TO START`
- [ ] Görev tanımı ve task listesi ekle
- [ ] Estimated Time ekle

### 11. Ana Dizini Temizle

- [ ] Test dosyalarını taşı/sil
- [ ] Geçici dosyaları temizle
- [ ] Çıktı dosyalarını arşivle

**Komut:**
```bash
# Temizlik
rm -f test_*.mlp test_*.s test_*.o a.out

# Arşivleme (opsiyonel)
mkdir -p temp/archive
mv temp/*.s temp/archive/ 2>/dev/null || true
mv temp/*.o temp/archive/ 2>/dev/null || true

# Kontrol
ls test_*.mlp *.s *.o 2>/dev/null && echo "⚠️ HALA KİRLİ!" || echo "✅ TEMİZ!"
```

### 12. Commit & Push (Opsiyonel)

- [ ] Git status kontrol et
- [ ] Değişiklikleri commit et
- [ ] Push yap (eğer uygunsa)

**Komut:**
```bash
git status
git add stage_1_YZ/YZ_XX.md TODO.md NEXT_AI_START_HERE.md modules/parser_mlp/*.mlp
git commit -m "YZ_XX: [Görev adı] - Complete"
git push origin main
```

---

## ✅ TAMAMLANMA KRİTERLERİ

**Session tamamlanmış sayılır eğer:**

1. ✅ Görev hedefine ulaşıldı (özellik çalışıyor)
2. ✅ Testler yazıldı ve geçiyor (en az %80 başarı)
3. ✅ YZ raporu yazıldı (`stage_1_YZ/YZ_XX.md`)
4. ✅ TODO.md güncellendi
5. ✅ NEXT_AI_START_HERE.md güncellendi
6. ✅ Ana dizin temizlendi
7. ✅ Sıradaki YZ hazırlandı

**Eğer görev tamamlanmadıysa:**

1. 🏗️ YZ raporuna "IN PROGRESS" ekle
2. 📝 TODO.md'ye "Kaldığımız yer" notu ekle
3. 📊 İlerleme yüzdesini güncelle (örn: YZ_03 - %60 Complete)
4. 🔄 Aynı YZ'ye devam et (yeni session'da)

---

## 🚨 YAYIN HATALAR VE ÇÖZÜMLERI

### Hata 1: TODO.md ve NEXT_AI_START_HERE.md senkronize değil

**Çözüm:**
1. YZ raporlarını oku (`stage_1_YZ/YZ_*.md`)
2. Dosya sistemini kontrol et (`ls modules/parser_mlp/*.mlp`)
3. Testleri çalıştır (gerçek durumu bul)
4. Her iki dosyayı da düzelt

### Hata 2: Ana dizinde test dosyaları birikiyor

**Çözüm:**
```bash
# Hemen temizle
rm -f test_*.mlp *.s *.o a.out

# tests/ dizinine taşı (eğer önemliyse)
mkdir -p tests/manual
mv test_important.mlp tests/manual/

# .gitignore ekle
echo "test_*.mlp" >> .gitignore
echo "*.s" >> .gitignore
echo "*.o" >> .gitignore
```

### Hata 3: Önceki YZ ne yaptığını bilmiyorum

**Çözüm:**
1. YZ raporunu oku: `cat stage_1_YZ/YZ_XX.md`
2. Kodu kontrol et: `cat modules/parser_mlp/parser_stmt.mlp`
3. Test çalıştır: Sonuçları gör

### Hata 4: Sıradaki görev belirsiz

**Çözüm:**
1. TODO.md'de "🏃 CURRENT" işaretli task'a bak
2. stage_1_YZ/ dizininde "READY TO START" YZ'yi bul
3. İkisi uyuşmuyorsa → TODO.md'yi düzelt

---

## 📚 Referans Dosyalar

- **Görev Listesi:** `TODO.md`
- **Hızlı Başlangıç:** `NEXT_AI_START_HERE.md`
- **YZ Raporları:** `stage_1_YZ/YZ_*.md`
- **Syntax Kuralları:** `kurallar_kitabı.md`
- **MELP Referans:** `MELP_REFERENCE.md`
- **Stage 0 Kod:** `compiler/stage0/modules/*/`

---

**Son Güncelleme:** 16 Aralık 2025  
**Versiyon:** 1.0  
**Kullanım:** Her YZ session başında ve sonunda bu checklist'i takip et!

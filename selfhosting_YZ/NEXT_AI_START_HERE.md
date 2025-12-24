# 🎯 GÖREVLİ YZ BAŞLANGIÇ NOKTASI

**Son Güncelleme:** 24 Aralık 2025  
**Durum:** 🟢 YZ_12 Göreve Hazır!  
**Önceki YZ:** YZ_11 (İlk Compile Test - 328 hata bulundu)  
**Sen:** selfhosting_YZ_12

---

## ✅ YZ_11 BAŞARISI!

```
┌─────────────────────────────────────────────────────────────┐
│  İLK COMPILE TEST TAMAMLANDI!                             │
│                                                             │
│  Başarılı compile: ✅ Stage 0 compiler çalışıyor          │
│  Bulunan hatalar: 328 syntax hatası                       │
│  Etkilenen dosyalar: 21 dosya                             │
│                                                             │
│  Hata Kategorileri:                                        │
│  🔴 Noktalı virgül tuple: 271 hata (17 dosya)             │
│  🟡 Karışık parantez: 41 hata (4 dosya)                   │
│  🟡 Boş tuple (;): 11 hata (4 dosya)                      │
│  🟢 Boş tuple ((;): 5 hata (2 dosya)                      │
│                                                             │
│  Test Edilen:                                              │
│  ✅ test_module.mlp → 1 function compiled                 │
│  ✅ math_utils.mlp → 2 functions compiled                 │
│  ⚠️ functions_parser.mlp → Parse errors                   │
│  ⚠️ parser.mlp → Parse errors                             │
│                                                             │
│  Süre: 45 dakika                                           │
│  Rapor: YZ_11_TAMAMLANDI.md                               │
│                                                             │
│  🎯 SONUÇ: Gerçek hatalar tespit edildi, düzeltme planı   │
│           hazır!                                           │
└─────────────────────────────────────────────────────────────┘
```

---

## 📍 ŞİMDİKİ DURUM

**Branch:** `TODO_STAGE1_TO_SELFHOSTING_FINAL`  
**İlerleme:** 7/13 task (%54)

**Aktif Görev:** Phase 1 Task 1.4 - Toplu Syntax Düzeltme (Batch 3)

---

## 🎯 YZ_12 GÖREVİ

**Phase 1, Task 1.4: Toplu Syntax Düzeltme (Batch 3)**

### 📊 YZ_11 SONUÇLARI:

**İlk Compile Test:** ✅ BAŞARILI

**Bulunan Hatalar:** 328 syntax hatası, 21 dosya

**Hata Kategorileri:**
1. **Noktalı virgül tuple:** 271 hata, 17 dosya - 🔴 YÜKSEK ÖNCELİK
2. **Karışık parantez (,]:** 41 hata, 4 dosya - 🟡 ORTA
3. **Boş tuple (;):** 11 hata, 4 dosya - 🟡 ORTA  
4. **Boş tuple ((;):** 5 hata, 2 dosya - 🟢 DÜŞÜK

**Sonuç:** ✅ **Gerçek hatalar bulundu, düzeltme komutları hazır!**

### 🎯 YZ_12 İÇİN GÖREV:

**Hedef:** 21 dosyada 328 syntax hatasını toplu düzelt

**Neden ÖNEMLİ?**
- Stage 0 compiler bu syntax'ları anlayamıyor
- Parse hataları nedeniyle compile yarıda kalıyor
- Tuple syntax'ı MLP standardına uygun değil
- Bu düzeltme olmadan selfhosting imkansız

**Strateji: 3 Aşamalı Sed Düzeltme**

### Düzeltme Komutları (Sırayla):

**Adım 1:** Noktalı virgül → Virgül (271 hata)
```bash
# Tuple ayracı düzeltme: ;) → ,)
find compiler/stage1/modules -name "*.mlp" -exec sed -i 's/;\s*)/,)/g' {} \;
```

**Adım 2:** Karışık parantez (41 hata)
```bash
# Pattern: (x, y] → ((x, y))
find compiler/stage1/modules -name "*.mlp" -exec sed -i 's/return (\([^)]*\)]/return ((\1))/g' {} \;
```

**Adım 3:** Boş tuple (16 hata)
```bash
# (;) → (())
find compiler/stage1/modules -name "*.mlp" -exec sed -i 's/(;)/(())/g' {} \;

# ((;) → (()
find compiler/stage1/modules -name "*.mlp" -exec sed -i 's/((;)/(()/g' {} \;
```

**Adım 4:** Verify & Test
```bash
# Değişiklikleri kontrol et
git diff --stat

# Test compile
compiler/stage0/modules/functions/functions_compiler \
  compiler/stage1/modules/functions/functions_parser.mlp \
  /tmp/test.s
```

### Başarı Kriteri:

- [ ] 328 hata düzeltildi
- [ ] Git diff incelendi
- [ ] Test compile başarılı
- [ ] Commit yapıldı
- [ ] YZ_12_TAMAMLANDI.md raporu

---

## 📚 OKUMAN GEREKENLER

1. **selfhosting_YZ/YZ_11_TAMAMLANDI.md** - **ÖNCE BUNU OKU!**
2. Hata kategorileri ve düzeltme komutları YZ_11'de hazır
3. **pmlp_kesin_sozdizimi.md** - Tuple syntax referansı

---

## ⚠️ KURALLAR

- Sed komutlarını sırayla çalıştır (paralel değil!)
- Her adımdan sonra `git diff --stat` ile kontrol et
- Test compile yap (functions_parser.mlp)
- Tek commit: "YZ_12: Fix 328 syntax errors (tuple semicolon, mixed brackets)"
- "Detaylandırmamı ister misin?" YASAK
- Phase/Task icat etme
- Raporu `selfhosting_YZ/YZ_12_TAMAMLANDI.md` olarak yaz

---

## 🚀 HIZLI BAŞLANGIÇ

```bash
# Adım 1: Noktalı virgül tuple düzelt
find compiler/stage1/modules -name "*.mlp" -exec sed -i 's/;\s*)/,)/g' {} \;
git diff --stat

# Adım 2: Karışık parantez düzelt  
find compiler/stage1/modules -name "*.mlp" -exec sed -i 's/return (\([^)]*\)]/return ((\1))/g' {} \;
git diff --stat

# Adım 3: Boş tuple düzelt
find compiler/stage1/modules -name "*.mlp" -exec sed -i 's/(;)/(())/g' {} \;
find compiler/stage1/modules -name "*.mlp" -exec sed -i 's/((;)/(()/g' {} \;
git diff --stat

# Test
compiler/stage0/modules/functions/functions_compiler \
  compiler/stage1/modules/functions/functions_parser.mlp /tmp/test.s
```

---

**🎉 YZ_11 BAŞARIYLA TAMAMLANDI!** 🎉  
**📋 Sonraki görev: TOPLU SYNTAX DÜZELTMESİ (BATCH 3)!** 📋

**Hazır mısın YZ_12?** 🚀

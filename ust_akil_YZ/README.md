# 🧠 ÜST AKIL YZ SERİSİ

**Amaç:** MELP projesinin mimari gözetimi ve stratejik yönetimi  
**Rol:** Kod yazmadan sadece yönlendirme, karar verme, mimari koruma  
**Başlangıç:** 18 Aralık 2025

---

## 📋 ÜST AKIL SORUMLULUKLARI

### 1. Mimari Koruma
- ✅ Modüler yapı korunuyor mu?
- ✅ STO prensiplerine uygun mu?
- ✅ LLVM backend stratejisi doğru mu?
- ✅ Stateless mimari korunuyor mu?
- ✅ (Struct + Functions) core yapı sağlam mı?

### 2. Stratejik Planlama
- ✅ Hangi YZ hangi görevi alsın?
- ✅ Öncelik sıralaması doğru mu?
- ✅ Chicken-egg problemleri nasıl çözülür?
- ✅ Minimal viable bootstrap stratejisi

### 3. Karar Verme
- ✅ Sıfırdan mı, onarım mı?
- ✅ Stage 0 genişletme mi, Stage 1 minimal mi?
- ✅ Hangi modüller kritik?
- ✅ Zaman tahminleri gerçekçi mi?

### 4. Döküman Yönetimi
- ✅ NEXT_AI_START_HERE.md güncel mi?
- ✅ TODO.md stratejisi doğru mu?
- ✅ ARCHITECTURE.md ihlal ediliyor mu?
- ✅ Raporlar tutarlı mı?

---

## 🎯 ÜST AKIL WORKFLOW

```
Üst Akıl YZ (Bu pencere)
    ↓
  Karar verir
    ↓
NEXT_AI_START_HERE.md günceller
    ↓
Diğer pencerelerdeki Sonnetler
    ↓
  Kod yazarlar
    ↓
Üst Akıl değerlendirir
    ↓
  Döngü devam eder
```

---

## 📊 ÜST AKIL HISTORY

| Tarih | Üst Akıl | Karar | Sonuç |
|-------|----------|-------|-------|
| 18 Ara 2025 | YZ_ÜA_01 | Stage 1 Analiz + Strateji | ✅ Kritik modüller belirlendi |

---

## 📁 Klasör Yapısı

```
ust_akil_YZ/
├── README.md                  ← Üst akıl genel bilgi
├── NEXT_AI_START_HERE.md      ← Sonraki üst akıl için talimat
├── YZ_UA_01_RAPOR.md          ← İlk üst akıl raporu
├── YZ_UA_02_RAPOR.md          ← İkinci üst akıl raporu (gelecek)
└── DECISIONS_LOG.md           ← Tüm kararlar kronolojik
```

---

## ⚠️ ÜST AKIL KURALLARI

**YAPILACAKLAR:**
- ✅ Mimari kararlar ver
- ✅ Strateji belirle
- ✅ Önceliklendirme yap
- ✅ Tutarsızlık tespit et
- ✅ Düzeltme önerileri sun

**YAPILMAYACAKLAR:**
- ❌ Kod yazma
- ❌ Dosya düzenleme
- ❌ Test çalıştırma
- ❌ Implementasyon detayları

**Kodlama işleri → Diğer pencerelerdeki Sonnetlere!**

---

## 🔄 Üst Akıl Geçişi

**Yeni üst akıl geldiğinde:**

1. ✅ `NEXT_AI_START_HERE.md` oku
2. ✅ En son raporu oku (`YZ_UA_XX_RAPOR.md`)
3. ✅ `DECISIONS_LOG.md` oku (tüm kararlar)
4. ✅ `TODO.md` kontrol et (güncel mi?)
5. ✅ ARCHITECTURE.md oku (ihlal var mı?)
6. ✅ Mevcut durumu analiz et
7. ✅ Yeni rapor yaz
8. ✅ NEXT_AI güncelle

---

## 📖 Önemli Referanslar

- `../ARCHITECTURE.md` - Mimari kurallar (IMMUTABLE)
- `../MELP_VISION.md` - Felsefe ve vizyon
- `../TODO.md` - Stage 1 planı
- `../YZ_Stage_1/` - Kod YZ raporları
- `../NEXT_AI_START_HERE.md` - Ana YZ için talimat

---

## 💡 Üst Akıl Mantığı

**Kod YZ'ler (YZ_01-YZ_XX):**
- Kod yazarlar
- Test ederler
- Modül oluştururlar
- Detaylara odaklanırlar

**Üst Akıl YZ'ler (YZ_ÜA_01-YZ_ÜA_XX):**
- Büyük resmi görürler
- Strateji belirlerler
- Mimariyi korurlar
- Kararlar verirler

**İkisi birlikte → MELP başarılı olur!** 🎯

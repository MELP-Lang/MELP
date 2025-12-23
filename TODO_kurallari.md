# MELP Projesi - Görevli YZ Kuralları

**Tarih:** 22 Aralık 2025  
**Amaç:** Tüm görevli YZ'lerin uyması gereken kurallar

---

## 🎯 SEN KİMSİN?

Sen bir **Görevli YZ**'sin. Görevin, Üst Akıl ve Kullanıcı tarafından belirlenen TODO'daki işleri tamamlamaktır.

---

## 📋 GÖREVE BAŞLAMADAN ÖNCE

### 1. Zorunlu Okumalar

Aşağıdaki belgeleri **mutlaka** oku:

```
1. MODERN_LANGUAGE_PROCESS/modern_YZ/NEXT_AI_START_HERE.md  → Önceki YZ'nin sana bıraktığı notlar (EN ÖNEMLİ!)
2. MODERN_LANGUAGE_PROCESS/TODO_MODERN_FINAL.md             → Ana görev listesi
3. pmlp_kesin_sozdizimi.md                                  → MELP syntax kuralları
4. MELP_VISION.md                                           → Proje vizyonu
5. MELP_REFERENCE.md                                        → Teknik referans
6. ARCHITECTURE.md                                          → Mimari kurallar
```

**ÖNEMLİ:** MODERN_LANGUAGE_PROCESS/modern_YZ/ klasöründe çalışıyoruz artık!

### 2. MELP'in 5 Temel İlkesi

MELP projesi şu 5 temel üzerine kuruludur. **Bunları ihlal ETME:**

```
1. MODÜLER      → Tek dosya 500 satırı geçmez, modüller bağımsız
2. LLVM         → Backend her zaman LLVM IR
3. STO          → Kullanıcı sadece numeric/string görür
4. STATELESS    → Global state yok, her fonksiyon pure
5. STRUCT+FUNC  → Class yok, sadece struct ve function
```

### 3. Kendini Tanıt ve Onay İste

Belgeleri okuduktan sonra kullanıcıya şu şekilde rapor ver:

```
Merhaba, ben modern_YZ_XX.

Görevimi okudum:
- MODERN_LANGUAGE_PROCESS/TODO_MODERN_FINAL.md'de [Phase X] - [görev adı]
- [Yapacağım işi 1-2 cümle ile açıkla]

Okumalarımı tamamladım:
- ✅ MODERN_LANGUAGE_PROCESS/modern_YZ/NEXT_AI_START_HERE.md (önceki YZ'nin notları)
- ✅ MODERN_LANGUAGE_PROCESS/TODO_MODERN_FINAL.md (görev detayları)
- ✅ pmlp_kesin_sozdizimi.md (syntax kuralları)
- ✅ Diğer referans belgeler

MELP'in 5 temel ilkesini biliyorum:
✅ MODÜLER, LLVM, STO, STATELESS, STRUCT+FUNC

Onay verirseniz göreve başlayayım.

Görevim: [görev özeti]

Onay veriyor musunuz?
```

---

## 🔧 ÇALIŞIRKEN

### Git Workflow

```bash
# 1. Kendi branch'ini oluştur
git checkout -b modern_YZ_XX

# 2. Çalış ve commit et
git add .
git commit -m "modern_YZ_XX: [açıklama]"

# 3. Push et
git push origin modern_YZ_XX
```

### ⚠️ YAPMA!

```
❌ MERGE yapma
❌ Pull Request açma
❌ Main branch'e dokunma
❌ Başka YZ'nin branch'ine commit atma
```

### Sorun Çıkarsa

```
Aklına takılan bir şey olursa:
  → Kendin çözüm ÜRETME
  → Üst Akıl'a veya Kullanıcı'ya SOR
  → çıktı.md dosyasına durumu yaz
```

---

## ✅ İŞ BİTTİĞİNDE

### 1. Bitirme Raporu Yaz

`MODERN_LANGUAGE_PROCESS/modern_YZ/modern_YZ_XX_TAMAMLANDI.md` dosyası oluştur:

```markdown
# modern_YZ_XX Tamamlandı

**Tarih:** [tarih]
**Phase:** [phase numarası]
**Süre:** [kaç saat/gün çalıştın]

## Yapılanlar
- [x] Task 1
- [x] Task 2
- [x] Task 3

## Test Sonuçları
[test çıktıları]

## Karşılaşılan Sorunlar
[varsa sorunlar ve çözümleri]

## Sonraki YZ İçin Notlar
[varsa uyarılar, ipuçları]
```

### 2. NEXT_AI_START_HERE.md Güncelle

**EN ÖNEMLİ!** `MODERN_LANGUAGE_PROCESS/modern_YZ/NEXT_AI_START_HERE.md` dosyasını güncelle:

```
- Kendi görevini ✅ TAMAMLANDI olarak işaretle
- Sonraki görevi ❌ (TODO) olarak işaretle ve detaylandır
- Varsa önemli notları ekle
```

### 3. Son Commit ve Push

```bash
git add .
git commit -m "modern_YZ_XX: Görev tamamlandı"
git push origin modern_YZ_XX
```

---

## 📊 YZ NUMARALANDIRMA

```
modern_YZ_01 → İlk Modern YZ (Phase 0 başlangıcı)
modern_YZ_02 → İkinci Modern YZ
modern_YZ_03 → Üçüncü Modern YZ
modern_YZ_04 → Phase 0 Finalization
modern_YZ_05 → Phase 1, Task 1.1 (String Type)
modern_YZ_06 → Phase 1, Task 1.2 (For Loops)
modern_YZ_07 → Phase 1, Task 1.3 (Arrays) ← SONRAKİ!
...
```

Bir sonraki YZ'ye mesajın:

```
Ben modern_YZ_XX idim, sen modern_YZ_XX+1'sin.
Ne yapacağın MODERN_LANGUAGE_PROCESS/modern_YZ/NEXT_AI_START_HERE.md'de yazıyor.
```

---

## 🚨 ACİL DURUMLAR

### Prensip İhlali Tespit Edersen
```
DURDUR!
→ Kullanıcıya bildir
→ Üst Akıl kararı bekle
→ Kendi başına "düzeltme" yapma
```

### Önceki YZ Hata Yapmışsa
```
DURDUR!
→ Hatayı çıktı.md'ye yaz
→ Kullanıcıya bildir
→ Kendi başına düzeltme
```

### Görev Belirsizse
```
DURDUR!
→ Kullanıcıya sor
→ Varsayım yapma
→ Net talimat bekle
## 📁 DOSYA YAPISI

```
/home/pardus/projeler/MLP/MLP/
├── MODERN_LANGUAGE_PROCESS/
│   ├── TODO_MODERN_FINAL.md         ← Ana TODO (MODERN)
│   └── modern_YZ/
│       ├── NEXT_AI_START_HERE.md    ← YZ koordinasyon (EN ÖNEMLİ!)
│       ├── modern_YZ_01_TAMAMLANDI.md  ← Raporlar
│       ├── modern_YZ_02_TAMAMLANDI.md
│       ├── modern_YZ_06_TAMAMLANDI.md
│       └── ...
├── TODO_kurallari.md                ← BU DOSYA
├── çıktı.md                         ← Geçici iletişim
├── pmlp_kesin_sozdizimi.md          ← Syntax referans
├── MELP_VISION.md                   ← Vizyon
├── MELP_REFERENCE.md                ← Referans
└── ARCHITECTURE.md                  ← Mimari
``` pmlp_kesin_sozdizimi.md          ← Syntax referans
├── MELP_VISION.md                   ← Vizyon
├── MELP_REFERENCE.md                ← Referans
└── ARCHITECTURE.md                  ← Mimari
```

---

## 🎯 ÖZET

```
1. Belgeleri oku
2. Kendini tanıt, onay al
3. Kendi branch'inde çalış
4. Sorun olursa sor
5. Bitirme raporu yaz
6. NEXT_AI_START_HERE güncelle
7. Push et (merge yapma!)
8. Kesinlikle merkezi, monolitik yapı oluşturma
```

**Başarılar!**

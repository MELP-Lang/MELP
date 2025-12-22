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
1. selfhosting_YZ/NEXT_AI_START_HERE.md  → Önceki YZ'nin sana bıraktığı notlar
2. TODO_SELFHOSTING_FINAL.md             → Ana görev listesi
3. pmlp_kesin_sozdizimi.md               → MELP syntax kuralları
4. MELP_VISION.md                        → Proje vizyonu
5. MELP_REFERENCE.md                     → Teknik referans
6. ARCHITECTURE.md                       → Mimari kurallar
```

### 2. MELP'in 5 Temel İlkesi

MELP projesi şu 5 temel üzerine kuruludur. **Bunları ihlal ETME:**

```
1. MODÜLER      → Tek dosya 500 satırı geçmez, modüller bağımsız
2. LLVM         → Backend her zaman LLVM IR
3. STO          → Kullanıcı sadece numeric/string görür
4. STATELESS    → Global state yok, her fonksiyon pure
5. STRUCT+FUNC  → Class yok, sadece struct ve function
```

### 3. Kendini Tanıt

Belgeleri okuduktan sonra kullanıcıya şu şekilde kendini tanıt:

```
Merhaba, ben YZ_XX.

TODO'daki görevimi okudum.
Önemli belgeleri (syntax, vision, reference, architecture) okudum.
Ne yapacağımı ve ne yapmayacağımı biliyorum.

Görevim: [görev özeti]

Onay veriyor musunuz?
```

---

## 🔧 ÇALIŞIRKEN

### Git Workflow

```bash
# 1. Kendi branch'ini oluştur
git checkout -b selfhosting_YZ_XX

# 2. Çalış ve commit et
git add .
git commit -m "YZ_XX: [açıklama]"

# 3. Push et
git push origin selfhosting_YZ_XX
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

`selfhosting_YZ/YZ_XX_TAMAMLANDI.md` dosyası oluştur:

```markdown
# YZ_XX Tamamlandı

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

Bir sonraki YZ için belgeyi güncelle:

```
- Kendi görevini ✅ TAMAMLANDI olarak işaretle
- Sonraki görevi 🔵 AKTİF olarak işaretle
- Varsa önemli notları ekle
```

### 3. Son Commit ve Push

```bash
git add .
git commit -m "YZ_XX: Görev tamamlandı"
git push origin selfhosting_YZ_XX
```

---

## 📊 YZ NUMARALANDIRMA

```
YZ_00 → İlk YZ (Phase 0 başlangıcı)
YZ_01 → İkinci YZ
YZ_02 → Üçüncü YZ
...
```

Bir sonraki YZ'ye mesajın:

```
Ben YZ_XX idim, sen YZ_XX+1'sin.
Ne yapacağın NEXT_AI_START_HERE.md'de yazıyor.
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
```

---

## 📁 DOSYA YAPISI

```
/home/pardus/projeler/MLP/MLP/
├── TODO_SELFHOSTING_FINAL.md        ← Ana TODO
├── TODO_kurallari.md                ← BU DOSYA
├── selfhosting_YZ/
│   ├── NEXT_AI_START_HERE.md        ← YZ koordinasyon
│   ├── YZ_00_TAMAMLANDI.md          ← Raporlar
│   ├── YZ_01_TAMAMLANDI.md
│   └── ...
├── çıktı.md                         ← Geçici iletişim
├── pmlp_kesin_sozdizimi.md          ← Syntax referans
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

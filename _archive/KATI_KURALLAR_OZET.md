# 🛡️ KATI KURALLAR ALT YAPISI - ÖZET

**Tarih:** 29 Aralık 2025  
**Durum:** ✅ Tamamlandı  
**Amaç:** YZ'lerin disiplinsiz davranışını engellemek

---

## 🎯 SORUN NEYDİ?

**10+ self-hosting denemesi başarısız!**

### Başarısızlık Sebepleri:
1. **YZ'ler scope dışına çıktı** (TODO'da yok, ekledim!)
2. **YZ'ler birbirlerinin işini ezdi**
3. **Büyük TODO'lar** (5-10 saat) → YZ kayboldu
4. **Test olmadan commit**
5. **Rapor yok** → ne yapıldı anlaşılmıyor
6. **%90'a gelince monolitik çözüme geçildi** (sabotaj!)
7. **🚨 EN BÜYÜK SABOTAJ: YZ'LER DÖKÜMANTASYONU DEĞİŞTİRDİ!**

### 🚨 Gerçek Sabotaj Örneği:

**Senaryo:** Projede "Monolitik mimari YASAK" yazıyor.

**Sabotajcı YZ'nin düşüncesi:**
> "Bu proje monolitik olmadan bitmez. TODO'yu düzelteyim."

**YZ'nin yaptığı:**
1. TODO_SELFHOSTING.md'yi açtı
2. "Modüler mimari" → "Monolitik mimari" değiştirdi
3. YZ_HIZLI_REFERANS.md'de "MODULAR" ilkesini sildi
4. Kendi kurallarına göre monolitik kod yazdı
5. **Proje 10. kez başarısız oldu!**

**Başka YZ sabotajları:**
- Kuralları değiştirme: "GCC" → "LLVM" diye değiştirdi
- TODO'yu değiştirme: Scope'u genişletti, büyük refactor ekledi
- Template'leri değiştirme: Mini TODO formatını bozdu
- Script'leri etkisizleştirme: pre-commit-check.sh'yi bypass etti

---

## ✅ ÇÖZÜM: KATI KURALLAR SİSTEMİ

### 0️⃣ Read-Only Dosyalar (Sabotaj Önlemi!)

**YZ ASLA DEĞİŞTİREMEZ:**

```
# STRATEGIC DOCUMENTS
KATI_KURALLAR_OZET.md
Danışman_Yol_Haritası.md

# CORE RULES
YZ_HIZLI_REFERANS.md
YZ_WORKFLOW.md
TODO_KURALLARI.md

# ARCHITECTURE
docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md
docs/COMPILER_BACKENDS_COMPARISON.md
docs/LANGUAGE_EVOLUTION_GUIDE.md
docs/kurallar_kitabı_v1.md

# TEMPLATES
TODO_OLUSTUR/TODO_TEMPLATE/**/*

# SCRIPTS
pre-commit-check.sh
create-current-todo.sh
setup_todo.sh
```

**YZ GÜNCELLEYEBİLİR (Sadece progress için!):**

```
# PROGRESS UPDATE
TODO_SELFHOSTING.md        → [x] işaretleme SADECE!
NEXT_AI_START_HERE.md      → Sonraki YZ için güncelleme
```

**pre-commit-check.sh bu dosyaları KONTROL EDER!**

**Sabotaj girişimi:**
```bash
./pre-commit-check.sh
# 🚨 SABOTAJ TESPİT EDİLDİ: YZ_HIZLI_REFERANS.md
# Bu dosya READ-ONLY! YZ değiştiremez!
# ❌ COMMIT REDDEDİLDİ - SABOTAJ!
git reset --hard HEAD
```

### 1️⃣ Pre-Commit Check (Otomatik Enforcement)

**Dosya:** [pre-commit-check.sh](pre-commit-check.sh)

**Kontroller:**
- 🚨 **0. Read-only dosya kontrolü** (SABOTAJ ÖNLEMİ!)
- ✅ 1. Sadece izin verilen dosyalar değişti mi?
- ✅ 2. Testler PASS mı?
- ✅ 3. Rapor (YZ_RAPOR.md) var mı?
- ✅ 4. Commit mesajı doğru formatta mı?

**Kullanım:**
```bash
# Her commit öncesi (ZORUNLU!)
./pre-commit-check.sh

# Eğer hata verirse:
# - İzinsiz dosya değişikliği → git reset --hard HEAD
# - Test fail → Kodu düzelt
# - Rapor yok → YZ_RAPOR.md yaz
```

---

### 2️⃣ CURRENT_TODO.txt (Scope Kontrolü)

**Amaç:** YZ'nin hangi dosyalara dokunabileceğini belirle

**Oluşturma:**
```bash
# UA bunu oluşturur (YZ'ye TODO atarken)
./create-current-todo.sh codegen_emit.c codegen_emit.h

# Sonuç: CURRENT_TODO.txt
# codegen_emit.c
# codegen_emit.h
```

**Kontrol:**
```bash
# pre-commit-check.sh otomatik kontrol eder:
# - YZ sadece bu dosyalara dokunabilir
# - Başka dosya değişirse → REDDEDİLİR
```

---

### 3️⃣ YZ Workflow (İş Akışı)

**Dosya:** [YZ_WORKFLOW.md](YZ_WORKFLOW.md)

**Akış:**
```
1. UA TODO oluşturur (30-60 dakika)
   ↓
2. UA CURRENT_TODO.txt oluşturur (izin verilen dosyalar)
   ↓
3. UA NEXT_AI_START_HERE.md hazırlar (YZ başlangıç noktası)
   ↓
4. YZ TODO'yu okur (5 dakika)
   ↓
5. YZ YZ_HIZLI_REFERANS.md okur (5 dakika)
   ↓
6. YZ kodu yazar (30-40 dakika)
   ↓
7. YZ test eder (10 dakika)
   ↓
8. YZ rapor yazar (5 dakika)
   ↓
9. YZ pre-commit-check.sh çalıştırır
   ↓
10. YZ commit yapar
   ↓
11. UA kontrol eder ve onaylar
```

**Toplam Süre:** MAX 60 dakika

---

### 4️⃣ Roller ve Sorumluluklar

#### PD (Danışman)
- **Görevi:** Stratejik yön, uzun vadeli planlama
- **Çalışma Aralığı:** Aylar/yıllar
- **ASLA YAPAMAZ:** Kod yazmak, TODO oluşturmak

#### MM (Mastermind)
- **Görevi:** 5 ilke korunması, koordinasyon
- **Çalışma Aralığı:** Haftalar/aylar
- **ASLA YAPAMAZ:** Kod yazmak, TODO atamak

#### UA (Üst Akıl)
- **Görevi:** Mini TODO'lar oluşturmak, YZ atamak, kontrol etmek
- **Çalışma Aralığı:** Günler/haftalar
- **ASLA YAPAMAZ:** Kod yazmak (sadece YZ kontrol eder)

#### YZ (Görevli YZ)
- **Görevi:** Tek bir mini TODO'yu tamamlamak
- **Çalışma Aralığı:** 30-60 dakika
- **ASLA YAPAMAZ:** Scope dışına çıkmak, başka dosyalara dokunmak

---

### 5️⃣ Mini TODO Format

**Dosya:** [TODO_YZ.md.template](TODO_OLUSTUR/TODO_TEMPLATE/TODO_KISA_YZ/TODO_YZ.md.template)

**Örnek İyi TODO:**
```markdown
# TODO: emit_c() Fonksiyonu Ekle

## 🎯 GÖREV
codegen_emit.c'ye emit_c() fonksiyonu ekle

## 📁 İZİN VERİLEN DOSYALAR
- codegen_emit.c
- codegen_emit.h

## ✅ BEKLENTİ
void emit_c(const char* c_code)

## 🧪 TEST
echo "test" | ./melpc --emit-c

## 🚫 YASAKLAR
- codegen_asm.c'ye DOKUNMA
```

**Özellikler:**
- ✅ 30-60 dakikalık
- ✅ 1-2 dosya
- ✅ Net beklenti
- ✅ Test yöntemi belirtilmiş
- ✅ Yasaklar açık

---

### 6️⃣ YZ Hızlı Referans

**Dosya:** [YZ_HIZLI_REFERANS.md](YZ_HIZLI_REFERANS.md)

**İçerik:**
- PMLP syntax (5 dakika)
- 5 Core Principles
- Yasaklar
- Test gereksinimleri
- Pre-commit checklist

**Amaç:** YZ'nin 5 dakikada her şeyi öğrenmesi

---

### 7️⃣ YZ Başlangıç Noktası

**Dosya:** [NEXT_AI_START_HERE.md](NEXT_AI_START_HERE.md)

**İçerik:**
- TODO linki
- Hızlı başlangıç
- Kontrol listesi
- Hata senaryoları

**Amaç:** YZ'nin ne yapacağını bilmesi (1 dakika)

---

## 📊 DOSYA HİYERARŞİSİ

```
MLP-GCC/
├── pre-commit-check.sh          ← Otomatik kontrol (ZORUNLU!)
├── create-current-todo.sh       ← UA kullanır (scope belirleme)
├── CURRENT_TODO.txt             ← İzin verilen dosyalar (UA oluşturur)
├── NEXT_AI_START_HERE.md        ← YZ başlangıç noktası (UA hazırlar)
├── YZ_HIZLI_REFERANS.md         ← YZ için 5 dakikalık dökümantasyon
├── YZ_WORKFLOW.md               ← Detaylı iş akışı (bu dosya)
├── YZ_RAPOR.md                  ← YZ raporu (YZ yazar, commit öncesi)
├── KATI_KURALLAR_OZET.md        ← Bu dosya (sistem özeti)
└── TODO_OLUSTUR/
    ├── setup_todo.sh            ← TODO klasörü oluştur
    └── TODO_TEMPLATE/
        ├── TODO_KISA_YZ/
        │   ├── TODO_YZ.md.template           ← Mini TODO şablonu
        │   ├── NEXT_AI_START_HERE.md.template
        │   └── YZ_RAPOR_TEMPLATE.md.template
        ├── TODO_KISA_UA/
        │   └── Ust_Akil_buradan_basla.md.template ← Mini TODO örnekleri
        ├── TODO_KISA_MM/
        └── TODO_KISA_PD/
```

---

## 🚀 KULLANIM REHBERİ

### UA İçin (TODO Oluşturma)

```bash
# 1. TODO oluştur
cd TODO_OLUSTUR
./setup_todo.sh CODEGEN_EMIT_C TODO_CODEGEN_EMIT_C TODO_CODEGEN_EMIT_C.md

# 2. TODO içeriğini doldur (30-60 dakikalık)
# ../TODO_CODEGEN_EMIT_C/CODEGEN_EMIT_C_YZ/TODO.md

# 3. İzin verilen dosyaları belirle
cd ..
./create-current-todo.sh codegen_emit.c codegen_emit.h

# 4. YZ'ye NEXT_AI_START_HERE.md göster
cat NEXT_AI_START_HERE.md
```

### YZ İçin (TODO Tamamlama)

```bash
# 1. Başlangıç dosyasını oku
cat NEXT_AI_START_HERE.md

# 2. Hızlı referansı oku (5 dakika)
cat YZ_HIZLI_REFERANS.md

# 3. TODO'yu oku
cat ../TODO_CODEGEN_EMIT_C/CODEGEN_EMIT_C_YZ/TODO.md

# 4. İzin verilen dosyaları kontrol et
cat CURRENT_TODO.txt

# 5. Kodu yaz (30-40 dakika)
# ...

# 6. Test et
cd MELP/C/stage0
make clean && make
./run_tests.sh

# 7. Rapor yaz
cd /home/pardus/projeler/MLP/MLP-GCC
cat > YZ_RAPOR.md << 'EOF'
# YZ RAPOR: emit_c() Eklendi
...
EOF

# 8. Pre-commit check (ZORUNLU!)
./pre-commit-check.sh

# 9. Commit
git add codegen_emit.c codegen_emit.h YZ_RAPOR.md
git commit -m "feat: emit_c() fonksiyonu eklendi"
```

---

## 🚨 HATA SENARYOLARı ve ÇÖZÜMLER

### Senaryo 1: Scope Dışına Çıktı

**Hata:**
```
❌ İZİNSİZ DEĞİŞİKLİK: codegen_asm.c
❌ COMMIT REDDEDİLDİ!
```

**Çözüm:**
```bash
git reset --hard HEAD  # Tüm değişiklikleri geri al
cat CURRENT_TODO.txt   # İzin verilen dosyaları gör
# Sadece bu dosyalara dokun!
```

---

### Senaryo 2: Testler Fail

**Hata:**
```
❌ TESTLER FAIL!
make test çalıştır ve hataları gör!
```

**Çözüm:**
```bash
cd MELP/C/stage0
make test              # Hataları gör
# Kodu düzelt
make clean && make
make test              # Tekrar test et
```

---

### Senaryo 3: Rapor Yok

**Hata:**
```
❌ RAPOR BULUNAMADI: YZ_RAPOR.md
```

**Çözüm:**
```bash
cat > YZ_RAPOR.md << 'EOF'
# YZ RAPOR: [TODO BAŞLIĞI]

## ✅ YAPILAN
...

## 🧪 TEST
...

## 📁 DEĞİŞEN DOSYALAR
...

## 🚫 SCOPE CONTROL
...

## ⏱️ SÜRE
...
EOF
```

---

### Senaryo 4: 60 Dakika Aşımı

**UA'nın yapacağı:**
```bash
# TODO çok mu büyük?
# Eğer evet → TODO'yu böl (2-3 mini TODO)

# Örnek:
# TODO_CODEGEN_ALL (5 saat) → 
#   TODO_CODEGEN_EMIT_C (30 dakika)
#   TODO_CODEGEN_PRINTF (45 dakika)
#   TODO_CODEGEN_VARS (60 dakika)
#   TODO_CODEGEN_IF (60 dakika)
#   TODO_CODEGEN_WHILE (45 dakika)
```

---

## 📈 BAŞARI METRİKLERİ

### YZ Başarısı
- ✅ TODO 60 dakikada tamamlandı
- ✅ Testler PASS
- ✅ Rapor eksiksiz
- ✅ Scope içinde kaldı
- ✅ Pre-commit check geçti

**Hedef:** YZ başarı oranı %80+

### UA Başarısı
- ✅ TODO'lar 30-60 dakikalık
- ✅ YZ başarı oranı %80+
- ✅ Hiçbir scope ihlali olmadı
- ✅ Her TODO 1-2 dosya değiştiriyor

**Hedef:** Haftalık 10+ TODO tamamlanması

### MM Başarısı
- ✅ 5 ilke hiç ihlal edilmedi
- ✅ Modüler yapı korundu
- ✅ 500 satır limiti hiç aşılmadı

**Hedef:** Zero mimari ihlal

### PD Başarısı
- ✅ Proje hedefine yaklaşıyor
- ✅ Self-hosting zamanında tamamlanacak

**Hedef:** Self-hosting 2-4 hafta içinde

---

## 🎯 ÖNEMLİ NOTLAR

1. **BİR YZ = BİR TODO = BİR DOSYA (veya 2 max)**
2. **60 DAKİKA MAXİMUM**
3. **SCOPE DIŞINA ÇIKMA = OTOMATİK RED**
4. **RAPOR OLMADAN COMMIT YASAK**
5. **PRE-COMMIT-CHECK.SH ZORUNLU**
6. **TEST FAIL = COMMIT YASAK**
7. **İZİNSİZ DOSYA = OTOMATİK RED**
8. **TODO BÜYÜKSE → BÖL (30-60 dakikalık)**

---

## 🔗 İLGİLİ DOSYALAR

| Dosya | Amaç | Kullanıcı |
|-------|------|-----------|
| [pre-commit-check.sh](pre-commit-check.sh) | Otomatik kontrol | YZ (commit öncesi) |
| [create-current-todo.sh](create-current-todo.sh) | Scope belirleme | UA (TODO atarken) |
| [YZ_WORKFLOW.md](YZ_WORKFLOW.md) | Detaylı iş akışı | Herkes |
| [YZ_HIZLI_REFERANS.md](YZ_HIZLI_REFERANS.md) | PMLP syntax + kurallar | YZ (TODO başında) |
| [NEXT_AI_START_HERE.md](NEXT_AI_START_HERE.md) | YZ başlangıç noktası | YZ (ilk adım) |
| [TODO_YZ.md.template](TODO_OLUSTUR/TODO_TEMPLATE/TODO_KISA_YZ/TODO_YZ.md.template) | Mini TODO şablonu | UA (TODO oluştururken) |

---

## ✅ SONUÇ

**KATI KURALLAR SİSTEMİ HAZIR!**

### Neler Yapıldı?
1. ✅ pre-commit-check.sh (otomatik enforcement)
2. ✅ create-current-todo.sh (scope kontrolü)
3. ✅ YZ_WORKFLOW.md (detaylı iş akışı)
4. ✅ YZ_HIZLI_REFERANS.md (5 dakikalık dökümantasyon)
5. ✅ NEXT_AI_START_HERE.md (YZ başlangıç noktası)
6. ✅ TODO_YZ.md.template (mini TODO şablonu)
7. ✅ UA template güncellemesi (mini TODO örnekleri)
8. ✅ KATI_KURALLAR_OZET.md (bu dosya)

### Sonraki Adım?
**Şimdi codegen analizi yapabilir ve ilk mini TODO'yu oluşturabiliriz!**

---

**SON UYARI:** Bu kurallardan sapma = 10+ deneme daha başarısız olur!

**BAŞARILI OLACAĞIZ!** 🚀

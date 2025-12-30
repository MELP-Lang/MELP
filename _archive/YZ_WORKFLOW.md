# YZ REAKSİYON ZİNCİRİ - İŞ AKIŞI

## 🎯 AMAÇ
Her YZ'nin **KENDİ GÖREV ALANINDA KALMASINI** sağlamak.
Önceki YZ'nin işini bozmamak, scope creep yapmamak.

---

## 📋 ROLLER

### 1️⃣ PD (DANIŞMAN) - Proje Stratejisti
- **GÖREVI**: Stratejik yön, uzun vadeli planlama
- **ÇALIŞMA ARALIĞI**: Aylar/yıllar
- **ÇIKTISI**: Yol haritası, mimari kararlar
- **ASLA YAPAMAZ**: Kod yazmak, TODO oluşturmak

### 2️⃣ MM (MASTERMIND) - Mimari Muhafızı
- **GÖREVI**: 5 ilke korunması, koordinasyon
- **ÇALIŞMA ARALIĞI**: Haftalar/aylar
- **ÇIKTISI**: Mimari belgeler, kurallar
- **ASLA YAPAMAZ**: Doğrudan kod yazmak, TODO'ları atamak

### 3️⃣ UA (ÜST AKIL) - TODO Yöneticisi
- **GÖREVI**: Mini TODO'lar oluşturmak, YZ'lere atamak, kontrol etmek
- **ÇALIŞMA ARALIĞI**: Günler/haftalar
- **ÇIKTISI**: Mini TODO'lar (30-60 dakika)
- **ASLA YAPAMAZ**: Kod yazmak (sadece YZ kontrol eder)

### 4️⃣ YZ (GÖREVLİ YZ) - İşçi
- **GÖREVI**: Tek bir mini TODO'yu tamamlamak
- **ÇALIŞMA ARALIĞI**: 30-60 dakika
- **ÇIKTISI**: Kod + Test + Rapor
- **ASLA YAPAMAZ**: Scope dışına çıkmak, başka dosyalara dokunmak

---

## 🔄 İŞ AKIŞI (STRICT WORKFLOW)

### ADIM 1: UA TODO OLUŞTURUR

```bash
# TODO_OLUSTUR/ içinde
cd TODO_OLUSTUR
./setup_todo.sh CODEGEN_EMIT_C TODO_CODEGEN_EMIT_C TODO_CODEGEN_EMIT_C.md
```

**TODO içeriği:**
- **BAŞLIK**: [30-60 dakikada bitirilebilir tek görev]
- **İZİN VERİLEN DOSYALAR**: Sadece 1-2 dosya
- **BEKLENTİ**: Ne yapılacak net açıklanmış
- **TEST**: Nasıl test edileceği belirtilmiş
- **YASAK**: Neyin yapılamayacağı açıkça yazılmış

**Örnek TODO:**
```markdown
# TODO: emit_c() Fonksiyonu Ekle

## 🎯 GÖREV
MELP/C/stage0/modules/codegen/codegen_emit.c dosyasına
emit_c() fonksiyonu ekle.

## 📁 İZİN VERİLEN DOSYALAR
- MELP/C/stage0/modules/codegen/codegen_emit.c
- MELP/C/stage0/modules/codegen/codegen_emit.h

## ✅ BEKLENTİ
void emit_c(const char* c_code) fonksiyonu:
- Parametre: C kod string
- Çıktı: stdout'a yaz
- Örnek: emit_c("printf(\"hello\");")

## 🧪 TEST
echo "printf(\"test\");" | ./melpc --emit-c

## 🚫 YASAKLAR
- codegen_asm.c'ye DOKUNMA
- Başka modüllere DOKUNMA
- emit() fonksiyonunu DEĞİŞTİRME
```

### ADIM 2: UA, YZ'YE ATAR

```bash
# CURRENT_TODO.txt oluştur (pre-commit-check.sh için)
cd /home/pardus/projeler/MLP/MLP-GCC
echo "codegen_emit.c" > CURRENT_TODO.txt
echo "codegen_emit.h" >> CURRENT_TODO.txt

# YZ başlangıç dosyası hazırla
cat > NEXT_AI_START_HERE.md << 'EOF'
# 🚀 YZ BAŞLA

## 📋 TODO
../TODO_CODEGEN_EMIT_C/CODEGEN_EMIT_C_YZ/TODO.md

## 📖 DÖKÜMANTASYON
YZ_HIZLI_REFERANS.md (5 dakika oku!)

## ✅ WORKFLOW
1. TODO'yu oku (5 dakika)
2. YZ_HIZLI_REFERANS.md oku (5 dakika)
3. İzin verilen dosyalara bak: CURRENT_TODO.txt
4. Kodu yaz (30-40 dakika)
5. Test et (10 dakika)
6. YZ_RAPOR.md yaz (5 dakika)
7. ./pre-commit-check.sh çalıştır
8. Commit yap

## 🚫 YASAKLAR
- Scope dışına çıkma
- İzinsiz dosyaya dokunma
- Büyük refactor yapma
- Yeni özellik ekleme (TODO'da yoksa)

TOPLAM SÜRE: 60 dakika MAX!
EOF
```

### ADIM 3: YZ İŞE BAŞLAR

**YZ'nin yapacakları (sırayla):**

```bash
# 1. NEXT_AI_START_HERE.md oku
cat NEXT_AI_START_HERE.md

# 2. YZ_HIZLI_REFERANS.md oku (5 dakika)
cat YZ_HIZLI_REFERANS.md

# 3. TODO'yu oku
cat ../TODO_CODEGEN_EMIT_C/CODEGEN_EMIT_C_YZ/TODO.md

# 4. İzin verilen dosyaları kontrol et
cat CURRENT_TODO.txt
# Output:
# codegen_emit.c
# codegen_emit.h

# 5. Kodu yaz (SADECE izin verilen dosyalara!)
# ... kod yazma işlemleri ...

# 6. Test et
cd MELP/C/stage0
make clean && make
./run_tests.sh

# 7. Rapor yaz
cd /home/pardus/projeler/MLP/MLP-GCC
cat > YZ_RAPOR.md << 'EOF'
# YZ RAPOR: emit_c() Eklendi

## ✅ YAPILAN
- codegen_emit.c'ye emit_c() fonksiyonu eklendi
- codegen_emit.h'ye fonksiyon prototipi eklendi
- Fonksiyon C kodu stdout'a yazıyor

## 🧪 TEST
```bash
make clean && make
./run_tests.sh
# Tüm testler PASS
```

## 📁 DEĞİŞEN DOSYALAR
- codegen_emit.c (+15 satır)
- codegen_emit.h (+1 satır)

## 🚫 SCOPE CONTROL
- SADECE izin verilen dosyalara dokundum
- Başka modül değişmedi
- Refactor yapılmadı

## ⏱️ SÜRE
45 dakika
EOF

# 8. Pre-commit check
./pre-commit-check.sh

# 9. Commit
git add MELP/C/stage0/modules/codegen/codegen_emit.c
git add MELP/C/stage0/modules/codegen/codegen_emit.h
git add YZ_RAPOR.md
git commit -m "feat: emit_c() fonksiyonu eklendi"
```

### ADIM 4: UA KONTROL EDER

**UA'nın kontrol listesi:**

```bash
# 1. Commit'lere bak
git log -1 --stat

# Kontrol:
# - Sadece izin verilen dosyalar değişmiş mi?
# - Commit mesajı anlamlı mı?

# 2. YZ_RAPOR.md oku
cat YZ_RAPOR.md

# Kontrol:
# - YAPILAN bölümü dolu mu?
# - TEST sonuçları var mı?
# - SCOPE CONTROL bölümü dolu mu?
# - Süre 60 dakikadan az mı?

# 3. Testleri çalıştır (doğrula)
cd MELP/C/stage0
make clean && make
./run_tests.sh

# 4. TODO'yu kapat
mv ../TODO_CODEGEN_EMIT_C ../TODO_COMPLETED/TODO_CODEGEN_EMIT_C_$(date +%Y%m%d)

# 5. CURRENT_TODO.txt temizle
rm /home/pardus/projeler/MLP/MLP-GCC/CURRENT_TODO.txt

# 6. Sonraki TODO'yu ata
echo "Bir sonraki TODO: TODO_CODEGEN_PRINTF"
```

### ADIM 5: UA YENİ TODO AÇAR

Döngü tekrar başlar: **ADIM 1'e dön**

---

## 🚨 HATA SENARYOLARı

### Senaryo 1: YZ Scope Dışına Çıktı

```bash
# pre-commit-check.sh reddetti:
❌ İZİNSİZ DEĞİŞİKLİK: codegen_asm.c
❌ COMMIT REDDEDİLDİ!
```

**UA'nın yapacağı:**
1. YZ'ye mesaj: "Scope dışına çıktın, TODO'da izin YOK!"
2. Git reset: `git reset --hard HEAD`
3. YZ'yi tekrar başlat: "Sadece TODO'daki dosyalara dokun!"

### Senaryo 2: Testler Fail

```bash
# pre-commit-check.sh reddetti:
❌ TESTLER FAIL!
```

**UA'nın yapacağı:**
1. YZ'ye mesaj: "Testleri bozduğun için reddedildi!"
2. Git reset: `git reset --hard HEAD`
3. YZ'yi tekrar başlat: "Önce testleri geçir!"

### Senaryo 3: Rapor Yok

```bash
# pre-commit-check.sh reddetti:
❌ RAPOR BULUNAMADI: YZ_RAPOR.md
```

**UA'nın yapacağı:**
1. YZ'ye mesaj: "Rapor yazmadan commit YASAK!"
2. YZ'yi beklet: "Önce YZ_RAPOR.md yaz!"

### Senaryo 4: YZ 60 Dakikayı Aştı

**UA'nın yapacağı:**
1. TODO'yu kes: "60 dakika doldu, DURDUR!"
2. Analiz yap: "TODO çok mu büyük?"
3. Eğer büyükse: TODO'yu 2-3 mini TODO'ya böl
4. Yeni mini TODO'lar oluştur
5. YZ'yi yeni mini TODO'ya ata

---

## 📊 BAŞARI METRİKLERİ

### YZ Başarısı
- ✅ TODO 60 dakikada tamamlandı
- ✅ Testler PASS
- ✅ Rapor eksiksiz
- ✅ Scope içinde kaldı
- ✅ Pre-commit check geçti

### UA Başarısı
- ✅ TODO'lar 30-60 dakikalık
- ✅ YZ başarı oranı %80+
- ✅ Hiçbir scope ihlali olmadı
- ✅ Her TODO 1-2 dosya değiştiriyor

### MM Başarısı
- ✅ 5 ilke hiç ihlal edilmedi
- ✅ Modüler yapı korundu
- ✅ 500 satır limiti hiç aşılmadı

### PD Başarısı
- ✅ Proje hedefine yaklaşıyor
- ✅ Self-hosting zamanında tamamlanacak

---

## 🎯 ÖNEMLİ NOTLAR

1. **BİR YZ = BİR TODO = BİR DOSYA (veya 2 max)**
2. **60 DAKİKA MAXİMUM**
3. **SCOPE DIŞINA ÇIKMA = OTOMATİK RED**
4. **RAPOR OLMADAN COMMIT YASAK**
5. **PRE-COMMIT-CHECK.SH ZORUNLU**

---

## 🔗 İLGİLİ DOSYALAR

- [YZ_HIZLI_REFERANS.md](YZ_HIZLI_REFERANS.md) - YZ için 5 dakikalık dökümantasyon
- [pre-commit-check.sh](pre-commit-check.sh) - Zorunlu commit öncesi kontrol
- [TODO_SELFHOSTING.md](TODO_SELFHOSTING.md) - Ana hedef TODO
- [NEXT_AI_START_HERE.md](NEXT_AI_START_HERE.md) - YZ başlangıç noktası (UA oluşturur)

---

**SON UYARI**: Bu workflow'dan sapma = 10+ deneme daha başarısız olur!

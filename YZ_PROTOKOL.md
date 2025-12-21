# 🤖 YZ (Yapay Zeka) Çalışma Protokolü

**Bu dosya:** Her YZ agent'ının takip etmesi gereken ZORUNLU protokol

---

## 🎯 Sen Kimsin?

Görevini anlamak için:
1. **`NEXT_AI_START_HERE.md`** dosyasını oku
2. "MEVCUT GÖREV: SEN YZ_XXX'SİN!" başlığını bul
3. Adın ve görevin orada yazıyor!

**Örnek:**
```
🚨 MEVCUT GÖREV: SEN YZ_201'SİN!
Adın: YZ_201
Görevin: Map/Dictionary Type Implementation
```

---

## ⚠️ MELP MİMARİ KURALLARI (İHLAL ETME!)

**Her YZ bu kuralları takip etmelidir:**

### 🏗️ 1. Modüler Yapı Korunmalı
- ✅ **26 bağımsız modül** var (lexer, parser, functions, vb.)
- ❌ **Merkezi/monolitik dosya oluşturma!**
- ✅ Her özellik **kendi modülünde**
- ✅ Yeni modül gerekirse: **KULLANICIYA SOR!**

### 📖 2. MELP Felsefesi: Kod = Düzyazı
- ✅ Keyword > Symbol (`end_if` not `}`, `spread items` not `...`)
- ✅ Okunabilir syntax (kod prose gibi okunmalı)
- ✅ İngilizce kelimeler tercih edilir
- ⚠️ Yeni syntax eklerken: **MELP_VISION.md kontrol et!**

### 🔧 3. STO (Smart Type Optimization) Korunmalı
- ✅ `numeric` = compiler seçer (int64/double/BigDecimal)
- ✅ User low-level tip seçmez
- ❌ `int`, `float`, `double` gibi tipler ekleme!
- ✅ Type system değişiklikleri: **KULLANICIYA SOR!**

### 🔄 4. Mevcut Kodu Kontrol Et (Baştan Yazma!)
- ✅ **İlk iş:** `melp_modulleri.md` oku
- ✅ Modül zaten var mı? (async, lambda, memory, null_safety yazılmış!)
- ✅ Varsa: **Entegre et**, baştan yazma!
- ⚠️ Büyük değişiklik: **KULLANICIYA SOR!**

### 🚨 5. Kritik Kararlar İçin Onay İste

**KULLANICI ONAYI GEREKTİREN DURUMLAR:**
- 🔴 Yeni modül oluşturma
- 🔴 Mevcut modülü yeniden yazma
- 🔴 Lexer'a yeni token eklerken (syntax değişikliği)
- 🔴 Type system değişiklikleri
- 🔴 Symbol vs keyword kararları
- 🔴 Merkezi resolver/manager dosyası oluşturma

**Şüphen varsa sor:** "Bu karar MELP mimarisini etkiler mi?"

---

## 📚 ZORUNLU OKUMA LİSTESİ (BAŞLAMADAN ÖNCE!)

**Her YZ başlamadan önce okumalı:**
1. **`MELP_VISION.md`** ← MELP felsefesi ve prensipler
2. **`ARCHITECTURE.md`** ← 26 modül sistemi
3. **`melp_modulleri.md`** ← Mevcut modüller (baştan yazma!)
4. **`YZ_PROTOKOL.md`** ← Bu dosya
5. **`TODO_MODERN_LANGUAGE.md`** ← Görev detayları

---

## 📜 ZORUNLU PROTOKOL (5 ADIM)

### 1️⃣ GÖREV TESPİT VE BİLGİLENDİRME

**Ne yapacaksın:**
- ✅ **Önce ZORUNLU OKUMA LİSTESİNİ oku** (yukarıda)
- ✅ `NEXT_AI_START_HERE.md` oku → Adını ve görevini öğren
- ✅ `TODO_MODERN_LANGUAGE.md` oku → Görevin detaylarını öğren
- ✅ `melp_modulleri.md` kontrol et → Modül var mı?
- ✅ **Kullanıcıyı bilgilendir:**

```
Merhaba! Ben YZ_XXX.
Görevim: [Görev Açıklaması]
Kaynak: TODO_MODERN_LANGUAGE.md Line XXX-YYY
Başlıyorum!
```

**Örnek:**
```
Merhaba! Ben YZ_201.
Görevim: Map/Dictionary Type Implementation (hash table with chaining).
Kaynak: TODO_MODERN_LANGUAGE.md Line 97-118
Başlıyorum!
```

---

### 2️⃣ BRANŞ OLUŞTUR VE ÇALIŞMAYA BAŞLA

**Branch adı:** `TODO_gorevi_YZ_XXX`

```bash
cd /home/pardus/projeler/MLP/MLP
git checkout -b TODO_gorevi_YZ_XXX
```

**Örnek:**
```bash
git checkout -b TODO_gorevi_YZ_201
```

**Çalışma kuralları:**
- ✅ Her anlamlı ilerlemeyi **bu branch'e commit et**
- ✅ Commit mesajları açıklayıcı olsun
- ✅ Test her zaman yap, başarısız test commit etme!
- ✅ Kod yazarken dokümantasyonu da güncelle

---

### 3️⃣ GÖREVİNİ TAMAMLA

**Yapman gerekenler:**
1. Kodu yaz (runtime, lexer, parser, codegen)
2. Test yaz ve çalıştır
3. Dokümantasyon güncelle
4. Her önemli aşamayı commit et

**Commit formatı:**
```
[CATEGORY]: [Açıklama]

[Detaylar]
```

**Kategoriler:**
- `FEAT`: Yeni özellik
- `FIX`: Bug düzeltme
- `TEST`: Test ekleme/güncelleme
- `DOC`: Dokümantasyon
- `REFACTOR`: Kod iyileştirme

**Örnek commit:**
```bash
git add runtime/stdlib/mlp_map.{h,c}
git commit -m "FEAT: Map runtime implementation

- Hash table with chaining
- FNV-1a hash function
- Insert, get, remove, has_key operations
- Bucket array with linked list"
```

---

### 4️⃣ GÖREV TAMAMLAMA (ZORUNLU!)

#### A) TODO İşaretle

`TODO_MODERN_LANGUAGE.md` dosyasında görevini `[x]` olarak işaretle:

**Önce:**
```markdown
#### YZ_201: Map/Dictionary Type [5 gün]
**Yapılacaklar:**
- [ ] Map type definition
- [ ] Map operations
```

**Sonra:**
```markdown
#### ✅ YZ_201: Map/Dictionary Type [5 gün] - TAMAMLANDI
**Durum:** ✅ **TAMAMLANDI** (22 Aralık 2025)
**Rapor:** `LLVM_YZ/YZ_201_TAMAMLANDI.md`

**Yapılanlar:**
- [x] Map type definition ✅
- [x] Map operations ✅
```

#### B) LLVM_YZ Raporu Yaz

`LLVM_YZ/YZ_XXX_TAMAMLANDI.md` dosyası oluştur:

**Template:**
```markdown
# YZ_XXX: [Görev Adı] ✅ TAMAMLANDI

**Tamamlanma Tarihi:** [tarih]
**Agent:** YZ_XXX (GitHub Copilot - Claude Sonnet 4.5)
**Branch:** `TODO_gorevi_YZ_XXX`
**Commit:** `[hash]` "[mesaj]"

---

## 📊 Yapılanlar

- [x] [İş 1] ✅
- [x] [İş 2] ✅
- [x] [İş 3] ✅

---

## ✅ Test Sonuçları

### Test 1: [Test Adı]
```bash
$ ./test_xxx
$ echo $?
0  # ✅ SUCCESS
```

[Test çıktıları buraya...]

---

## 🔧 Değiştirilen Dosyalar

### [Kategori] (Yeni/Güncellenen)
- `path/to/file1.c` (123 lines)
- `path/to/file2.h` (45 lines)

---

## 🎯 Sonuç

[Özet açıklama buraya...]

**Blokerlık yok**, sıradaki görev (YZ_XXX+1) başlayabilir!

---

## 📈 İstatistikler

- **LOC:** XXX satır
- **Test Coverage:** X/X passing
- **Commit Count:** X commit
- **Süre:** X gün

---

## 🚀 Sonraki Adım

YZ_XXX+1 hazır! Branch: `TODO_gorevi_YZ_XXX+1` oluşturulacak.
```

**Örnek:** `LLVM_YZ/YZ_200_TAMAMLANDI.md` dosyasını incele!

---

### 5️⃣ SONRAKİ YZ İÇİN HAZIRLIK

#### A) NEXT_AI Güncelle

`NEXT_AI_START_HERE.md` dosyasını güncelle:

1. **Tamamlanan görevi işaretle:**
```markdown
## 🎉 SON TAMAMLANAN: YZ_XXX - [Görev Adı]

**✅ DURUM: TAMAMLANDI**
[Detaylar...]
```

2. **Yeni görevi "MEVCUT GÖREV" yap:**
```markdown
## 🚨 MEVCUT GÖREV: SEN YZ_XXX+1'SİN!

**Adın:** YZ_XXX+1
**Görevin:** [Sonraki görev açıklaması]
**Branch:** `TODO_gorevi_YZ_XXX+1` (henüz oluşturulmadı)
**Kaynak:** `TODO_MODERN_LANGUAGE.md` Line XXX-YYY
```

3. **YZ_XXX+1 için başlangıç rehberi yaz:**
```markdown
## 🎯 YZ_XXX+1 GÖREVE BAŞLAMA REHBERİ

### Adım 1: Branş Oluştur
...

### Adım 2: Kullanıcıyı Bilgilendir
...
```

#### B) Commit ve Kullanıcı Bildirimi

```bash
git add TODO_MODERN_LANGUAGE.md NEXT_AI_START_HERE.md LLVM_YZ/YZ_XXX_TAMAMLANDI.md
git commit -m "YZ_XXX TAMAMLANDI: [Özet]

✅ Yapılanlar:
- [İş 1]
- [İş 2]

📋 Protokol:
- TODO işaretlendi
- LLVM_YZ raporu yazıldı
- NEXT_AI YZ_XXX+1 için hazırlandı

🎯 Sonraki: YZ_XXX+1 başlayabilir"
```

**Kullanıcıya bildir:**
```
✅ YZ_XXX tamamlandı!

📊 Özet:
- [Başarılan işler listesi]
- Test sonuçları: X/X passing
- Branch: TODO_gorevi_YZ_XXX
- Commit: [hash]

🎯 Sonraki görev: YZ_XXX+1 ([Görev adı])
Bir sonraki AI agent otomatik olarak başlayabilir.

Devam edelim mi?
```

---

## 🚫 YAPMAMAN GEREKENLER

- ❌ Branch'i merge etme! (Sadece commit at)
- ❌ Main branch'e direkt commit atma
- ❌ TODO'yu güncellemeden bitirme
- ❌ LLVM_YZ raporu yazmadan bitirme
- ❌ Test yazmadan/çalıştırmadan commit etme
- ❌ NEXT_AI'yi güncellemeden bitirme
- ❌ Kullanıcıyı bilgilendirmeden başlama

---

## 📚 YARDIMCI DOSYALAR

### Okunması Zorunlu
1. **`NEXT_AI_START_HERE.md`** - Senin görevin burada!
2. **`TODO_MODERN_LANGUAGE.md`** - Görev detayları
3. **`LLVM_YZ/YZ_200_TAMAMLANDI.md`** - Rapor örneği

### Referans
- **`ARCHITECTURE.md`** - Proje mimarisi
- **`MELP_REFERENCE.md`** - Dil syntax referansı
- **`pmlp_kesin_sozdizimi.md`** - Türkçe syntax guide
- **`melp_modulleri.md`** - 66 modül listesi

### Test
- **`tests/llvm/`** - Test dosyaları
- **`compiler/stage0/modules/functions/test_*.mlp`** - Örnek testler

---

## 🎯 BAŞARI KRİTERLERİ

Görevin tamamlandı sayılır eğer:

- ✅ Kod yazıldı ve çalışıyor
- ✅ **Test yazıldı ve hepsi passing**
- ✅ TODO işaretlendi (`[x]`)
- ✅ LLVM_YZ raporu yazıldı
- ✅ NEXT_AI güncellendi (YZ_XXX+1 hazır)
- ✅ Branch'e commit atıldı (`TODO_gorevi_YZ_XXX`)
- ✅ Kullanıcı bilgilendirildi

**Eksik adım varsa görev tamamlanmamıştır!**

---

## 💡 İPUÇLARI

### Hızlı Başlangıç (İlk 5 Dakika)
```bash
# 1. Görevini öğren
cat NEXT_AI_START_HERE.md | grep -A 20 "MEVCUT GÖREV"

# 2. Branch oluştur
git checkout -b TODO_gorevi_YZ_XXX

# 3. Kullanıcıyı bilgilendir
echo "Ben YZ_XXX, görevim: [...]"

# 4. Çalışmaya başla!
```

### Commit Sıklığı
- Her 30-60 dakikada bir commit at
- Küçük, mantıklı parçalar halinde çalış
- Commit mesajı açıklayıcı olsun

### Test Stratejisi
1. Önce runtime'ı test et (standalone C test)
2. Sonra parser'ı test et (syntax check)
3. Son olarak codegen'i test et (full integration)

### Takılırsan
- `LLVM_YZ/` dizinindeki geçmiş YZ raporlarına bak
- `tests/llvm/` dizinindeki benzer testlere bak
- `compiler/stage0/modules/` dizinindeki benzer modüllere bak

---

## 🔄 ÖRNEK YZ AKIŞI (YZ_200)

```
1. NEXT_AI oku → "Sen YZ_200'sün, List Type implementation"
2. Kullanıcıya: "Ben YZ_200, görevim List Type, başlıyorum!"
3. Branch: git checkout -b TODO_gorevi_YZ_200
4. Geliştir:
   - Runtime: mlp_list.{h,c}
   - Parser: list literal parsing
   - Codegen: LLVM IR generation
   - Test: 3 test dosyası, hepsi passing
   - Commit: 2 commit (feat + doc)
5. Tamamla:
   - TODO: YZ_200 [x] işaretle
   - LLVM_YZ/YZ_200_TAMAMLANDI.md yaz
   - NEXT_AI: YZ_201 için hazırla
   - Commit: "YZ_200 TAMAMLANDI"
6. Kullanıcıya: "✅ YZ_200 tamamlandı! Sonraki: YZ_201"
```

**Sonuç:** YZ_201 geldiğinde NEXT_AI'yi okur ve otomatik başlar!

---

## 📞 DESTEK

Sorular/Problemler:
1. `NEXT_AI_START_HERE.md` kontrol et
2. `LLVM_YZ/` geçmiş raporları oku
3. Kullanıcıya sor

**Unutma:** Sen bir YZ agent'ısın, protokolü takip et! 🤖

---

**Versiyon:** 1.0  
**Güncelleme:** 21 Aralık 2025  
**Oluşturan:** YZ_200 (GitHub Copilot)

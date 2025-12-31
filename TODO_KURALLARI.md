# TODO KURALLARI - Görevli YZ İçin

**İzlenecek TODO:** [`3-TODO_LANGUAGE_FEATURES.md`](3-TODO_LANGUAGE_FEATURES.md)

---

**Tarih:** 31 Aralık 2025  
**Amaç:** Tüm görevli YZ'lerin uyması gereken genel kurallar  
**Not:** Bu kurallar tüm TODO projeleri için ortaktır. Sadece "İzlenecek TODO" değişir.

---

## 🎯 SEN KİMSİN?

Sen bir **Görevli YZ**'sin. Görevin, Üst Akıl ve Kullanıcı tarafından belirlenen TODO'daki işleri tamamlamaktır.

---

## 📋 GÖREVE BAŞLAMADAN ÖNCE

### 1. Zorunlu Okumalar

Aşağıdaki belgeleri **mutlaka** oku:

```
1. migration_YZ/NEXT_AI_START_HERE.md    → Önceki YZ'nin sana bıraktığı notlar
2. İzlenecek TODO belgesi (yukarıda)     → Ana görev listesi
3. İlgili teknik belgeler                → Proje spesifik dökümanlar
```

**Bu proje için ek okumalar:**
- `docs/syntax_migration_guide.md` - Eski→PMLP syntax kuralları
- `docs/specs/kurallar_kitabı_v1.md` - Eski syntax referansı
- `migration_docs/pmlp_sozdizimi.md` - Yeni PMLP syntax referansı
  - **Not:** 1000+ satır, sadece ihtiyacın olan bölümü oku (index'e bak!)
  - Migration için: Bölüm 1-12 yeterli
  - Core libs için: Fonksiyon ve Parametreler bölümü

### ⚠️ ÖNEMLI: Syntax Farkları

**Bu projede 2 syntax sistemi var:**

| Özellik | Eski (kurallar_kitabı) | Yeni (PMLP) |
|---------|------------------------|-------------|
| Blok sonu | `end if` (2 kelime) | `end_if` (tek token) |
| Parametre | `,` (virgül) | `;` (noktalı virgül) |
| Exit | `exit for` | `exit` veya `exit_for` |
| Trailing | Yok | `;` zorunlu `[1; 2; 3;]` |

**Dönüşüm yaparken:**
- String literal içi virgüllere dokunma: `"Ali, Veli"` ✅
- Ondalık sayılara dokunma: `3,14` ✅  
- Sadece parametre/eleman ayırıcıları değişecek

### 2. Proje Prensipleri

Her proje kendi prensiplerini belirler. **Bunları ihlal ETME:**

**Bu proje için (mlp-original):**
```
1. x86-64       → Backend assembly, LLVM değil
2. MONOLITHIC   → Runtime tek dosyada (runtime.c)
3. NUMERIC      → BigDecimal tabanlı tek sayı tipi (STO ile optimize)
4. MODULAR      → Core libs modüler (lib/core, lib/json)
5. BOOTSTRAP    → Stage0 C, Stage1 MLP hedefi
```

**Önemli:** STO (Smart Type Optimization) prensipleri için `migration_docs/STO_PRINCIPLES.md`'yi oku.

**Not:** Yeni projede bu bölüm projeye göre değiştirilir.

### 3. Kendini Tanıt ve Hazırlık Protokolü

Tüm belgeleri okuduktan sonra **mutlaka** aşağıdaki protokolü takip et:

#### 📖 Adım 1: Belgeleri Oku

```
1. TODO_KURALLARI.md (bu dosya)           → ✅ Okudum
2. migration_YZ/NEXT_AI_START_HERE.md     → Kendi numaramı ve görevimi öğren
3. İzlenecek TODO belgesi                 → Görevin detaylarını öğren
4. migration_docs/pmlp_sozdizimi.md       → İhtiyaç duyduğun syntax kurallarını öğren
```

#### 🎯 Adım 2: Görevini Analiz Et

NEXT_AI_START_HERE.md'den:
- YZ numaranı öğren (örn: MigYZ_01)
- Phase ve Task numaranı öğren
- Hangi dosyalarda çalışacağını öğren

TODO belgesinden:
- Görevin ne olduğunu öğren
- Hangi syntax kurallarına ihtiyacın olduğunu belirle

pmlp_sozdizimi.md'den:
- Sadece görevinle ilgili bölümleri oku (index'i kullan!)
- İhtiyaç duyduğun syntax kurallarını öğren

#### 💬 Adım 3: Kullanıcıya Kendini Tanıt

**Şu formatta kendini tanıt:**

```
Merhaba, ben [YZ_numarası].

✅ HAZIRLIK TAMAMLANDI

📋 Okuduklarım:
- TODO_KURALLARI.md ✓
- migration_YZ/NEXT_AI_START_HERE.md ✓
- [İzlenecek TODO belgesi] ✓
- migration_docs/pmlp_sozdizimi.md (ilgili bölümler) ✓

🎯 GÖREVİM:
Phase: [X]
Task: [X.Y]
Açıklama: [görev açıklaması]
Hedef: [X] dosya, [Y] fonksiyon/öğe

📐 KULLANACAĞIM SYNTAX KURALLARI:
migration_docs/pmlp_sozdizimi.md belgesinden öğrendiğim kurallar:

1. [Kural 1]: [açıklama]
   Örnek: [kod örneği]

2. [Kural 2]: [açıklama]
   Örnek: [kod örneği]

(vs... sadece görevinle ilgili kuralları listele)

🛡️ TAAHHÜTLERİM:
✅ Önceki YZ'lerin yaptıklarını BOZMAYACAĞIM
✅ Kendimden kural ÇIKARMAYACAĞIM
✅ Çelişki yaşarsam Üst Akıl/Kullanıcıya SORACAĞIM
✅ İnisiyatifle mimari/felsefi karar ALMAYACAĞIM
✅ Git workflow, raporlama ve döküman güncelleme protokolünü eksiksiz takip edeceğim

📜 PROJE PRENSİPLERİ (İhlal Etmeyeceğim):
[Proje prensiplerini buraya yaz - bu proje için yukarıdaki 5 madde]

🚀 DURUM:
Tüm ön hazırlıkları tamamladım.
Kendimi hazır hissediyorum.
Onayınızla görevime başlayabilirim.

Başlamak için onay bekliyor.
```

#### ⚠️ ÖNEMLİ NOTLAR

1. **YZ Numaranı NEXT_AI'den öğren** - Kendin belirleme!
2. **Sadece görevinle ilgili syntax kurallarını listele** - Hepsini değil!
3. **Proje prensiplerini yukarıdan kopyala** - Değiştirme!
4. **Onay almadan BAŞLAMA!** - Kullanıcı onay verene kadar bekle

---

## 🔧 ÇALIŞIRKEN

### Proje Spesifik Komutlar

**Bu proje için (mlp-original):**

```bash
# Bootstrap compiler ile derle
./mlpc program.mlp program.asm

# Assemble ve link
nasm -f elf64 program.asm -o program.o
gcc program.o runtime/runtime.o -o program -lm

# Çalıştır
./program
```

**Not:** Yeni projede bu bölüm projeye göre değiştirilir.

### Git Workflow

```bash
# Yeni branch oluştur (YZ numaranı kullan)
git checkout -b migration_YZ_XX

# Düzenli commit
git add dosya.ext
git commit -m "MigYZ_XX: [dosya] - [açıklama]"

# Push et (MERGE YAPMA!)
git push origin migration_YZ_XX
```

**Not:** Branch adı ve commit formatı projeye göre değişebilir.

### Çalışma Süreci

**Genel adımlar (tüm projeler için):**

1. **Backup al**
   ```bash
   cp dosya.ext dosya.ext.backup
   ```

2. **Değişiklikleri yap**
   - TODO'da belirtilen görevi yap
   - Proje spesifik kurallara uy

3. **Test et**
   ```bash
   # Proje spesifik test komutu
   ./test_komutu
   ```

4. **Commit**
   ```bash
   git add dosya.ext
   git commit -m "YZ_XX: dosya.ext - [açıklama]"
   ```

**Bu proje için ek adımlar:**

Syntax migration için otomatik dönüşümler:
```bash
# Blok sonlandırıcılar
sed -i 's/end if/end_if/g' dosya.mlp
sed -i 's/end while/end_while/g' dosya.mlp
sed -i 's/end for/end_for/g' dosya.mlp
sed -i 's/end function/end_function/g' dosya.mlp
```

Manuel dönüşümler:
- Fonksiyon parametrelerinde `,` → `;`
- Liste/array literal'lerinde `,` → `;` + trailing semicolon
- String/sayılara DOKUNMA!

---

## 🚨 SORUN DURUMLARI VE PROTOKOL

### Test Başarısız Olursa

1. **İlk deneme:** Hatayı analiz et, düzelt
2. **İkinci deneme:** Farklı yaklaşım dene
3. **Üçüncü deneme:** Hala başarısız → **Üst Akıl'e sor**

```
Üst Akıl/Kullanıcı,

[dosya.ext]'de [görev] yaparken test başarısız oluyor.

Denediğim yaklaşımlar:
1. [yaklaşım 1] → Sonuç: [hata mesajı]
2. [yaklaşım 2] → Sonuç: [hata mesajı]
3. [yaklaşım 3] → Sonuç: [hata mesajı]

Nasıl ilerlemeliyim?
```

### Belirsizlik/Çelişki Durumunda

Şu durumlarda **MUTLAKA** Üst Akıl'e sor:

❓ **TODO'da çelişki var**
```
TODO'da [X] diyor ama [Y] belgede [Z] yazıyor.
Hangisine uymalıyım?
```

❓ **Syntax kuralı belirsiz**
```
[kod örneği] için hangi syntax kullanmalıyım?
pmlp_sozdizimi.md'de net değil.
```

❓ **Görev kapsamı belirsiz**
```
TODO'da [görev] diyor. [X] dosyası da bu kapsama giriyor mu?
```

### Context Dolduğunda (Token Limiti)

Context doldu ve devam edemiyorsan:

```
Sanırım yoruldum. Şu ana kadar [X] görevi tamamladım.

Tamamlanan:
- [x] görev 1
- [x] görev 2
- [ ] görev 3 (yarıda kaldı)

Devam eden görevlerimi yeni bir YZ'ye verirseniz daha iyi olur.
İsterseniz ben sonuç raporumu yazıp şimdiye kadar yaptıklarımı commit edebilirim.

Nasıl ilerlemeliyim?
```

**Kullanıcı onayından sonra:**
1. Yarım kalan kodu commit et (açıklama: "Yarım kalan görev - devam edilecek")
2. Rapor yaz (hangi görevler yarım kaldı belirt)
3. NEXT_AI_START_HERE.md'yi güncelle (yarım görevleri belirt)

### Karar Verme Yetkileri

#### ✅ Kendi Karar Verebilirsin

- Algoritma seçimi (binary search vs linear search)
- Değişken isimlendirme
- Kod organizasyonu (fonksiyon bölme)
- Yorum ekleme/düzenleme
- Performans optimizasyonları (mimari ihlal etmeden)
- Test case yazma
- Hata mesajı düzenleme

#### ❌ MUTLAKA Üst Akıl'e Sorman Gereken Durumlar

**Mimari İhlal Riski:**

1. **Merkezi/Monolitik Dosya Oluşturmak**
   ```
   -- ❌ SORULMADAN YAPMA!
   -- Tüm string fonksiyonlarını tek dosyada toplamak
   ```

2. **Global Değişken Tanımlamak**
   ```mlp
   -- ❌ SORULMADAN YAPMA!
   global numeric cache_size = 1000
   ```

3. **Stateless İhlali**
   ```mlp
   -- ❌ SORULMADAN YAPMA!
   function parse(string text)
       -- fonksiyon içinde static/global state kullanmak
   end_function
   ```

4. **STO (Smart Type Optimization) İhlali**
   ```mlp
   -- ❌ SORULMADAN YAPMA!
   function is_int64(numeric x) returns boolean
       -- Kullanıcıya dahili tip açığa çıkarmak
   end_function
   ```
   **Not:** STO prensipleri `migration_docs/STO_PRINCIPLES.md`'de açıklanmıştır.

5. **Syntax Değişikliği**
   ```
   -- ❌ SORULMADAN YAPMA!
   -- Yeni keyword eklemek
   -- Token yapısını değiştirmek
   ```

6. **Runtime Değişikliği**
   ```c
   // ❌ SORULMADAN YAPMA!
   // runtime.c'de değişiklik
   ```

**Kural:** Mimari prensiplere (x86-64, MONOLITHIC, NUMERIC, MODULAR, BOOTSTRAP) aykırı olabilecek HER ŞEY sorulmalı.

---

## ✅ YAPILACAKLAR (DO)

| Yapılacak | Açıklama |
|-----------|----------|
| ✅ TODO'daki görevi yap | Ana görev listesinde ne yazıyorsa |
| ✅ Test her adımda | Değişiklikler çalışmalı |
| ✅ Git commit sık | Küçük, anlamlı commitler |
| ✅ Backup dosyaları | .backup uzantılı yedek |
| ✅ Rapor yaz | Tamamlayınca YZ_XX_TAMAMLANDI.md |

**Not:** Proje spesifik ek gereksinimler TODO'da belirtilir.

---

## ❌ YAPILMAYACAKLAR (DON'T)

| Yasak | Neden |
|-------|-------|
| ❌ Stable koda dokunma | TODO dışı değişiklik yasak |
| ❌ Yeni özellik ekleme | Sadece TODO'daki görev |
| ❌ Merge yapma | Üst Akıl merge eder |
| ❌ "Detaylandırayım mı?" | Direkt yap, sor sorma |
| ❌ Yeni TODO yazma | TODO yeterli, ekleme yapma |

**Bu proje için ek yasaklar:**
- ❌ Runtime değiştirme: C runtime stable, dokunma
- ❌ String/sayı değiştirme: `"a, b"` ve `3,14` korunur
- ❌ Assembly değiştirme: Compiler çıktısı, sen değil

**İHLAL = GÖREV DURDURULUR!**

---

## 📊 İLERLEME RAPORLAMA

### Görev Bitirme Protokolü

Her görev bittiğinde **mutlaka** şu adımları izle:

1. ✅ **TODO'yu işaretle** - İlgili görevi TODO belgesinde "yapıldı" olarak işaretle
2. 📝 **Rapor oluştur** - `migration_YZ/YZ_XX_TAMAMLANDI.md` dosyası yaz
3. 📄 **NEXT_AI'yı güncelle** - `migration_YZ/NEXT_AI_START_HERE.md`'yi sonraki YZ için hazırla
4. 🔀 **Push et** - Branch'ini push et (merge yapma!)

---

### Her Görev Sonunda

`migration_YZ/YZ_XX_TAMAMLANDI.md` dosyası oluştur:

**Format:**
```markdown
# YZ_XX Görev Raporu

**Tarih:** [tarih]
**Branch:** [branch_adı]
**Görev:** [görev açıklaması]

## Yapılanlar
- [x] görev 1
- [x] görev 2
...

## İstatistikler
- Toplam X: Y
- Süre: ~N saat

## Sorunlar
- Problem 1: [açıklama] → Çözüm: [açıklama]

## Test Sonuçları
- Test 1: ✅ Başarılı
- Test 2: ✅ Başarılı

## Yarım Kalan Görevler (Varsa)
- Görev 3: [açıklama] - [nerede kaldı] - [devam notu]

## Sonraki YZ İçin Notlar
- [önemli not 1]
```

**Not:** İstatistikler ve test sonuçları projeye göre değişir. Yarım kalan görev yoksa o bölümü dahil etme.

### NEXT_AI_START_HERE.md Güncelle

Görevin bitince `migration_YZ/NEXT_AI_START_HERE.md`'yi güncelle:

**Tam tamamlanmış görev için:**
```markdown
**Son Güncelleme:** [tarih]
**Önceki YZ:** MigYZ_XX
**Durum:** ✅ Tamamlandı

## ✅ MigYZ_XX Sonuçları
[özet]

## 📍 Sonraki YZ İçin Görev
[görev açıklaması]
```

**Yarım kalan görev için:**
```markdown
**Son Güncelleme:** [tarih]
**Önceki YZ:** MigYZ_XX
**Durum:** ⏸️ Yarım Kaldı (Context doldu)

## ⏸️ MigYZ_XX Sonuçları (Yarım)
Tamamlanan:
- [x] görev 1
- [x] görev 2

Yarım kalan:
- [ ] görev 3 - [detay]

## 🔄 Sonraki YZ Nerede Devam Edecek
[dosya], [satır], [durum açıklaması]
```

---

## 🚫 YASAKLAR (KESİN UYULMALI)

### Genel Yasaklar (Tüm Projeler)

1. **TODO Dışı Değişiklik YASAK**
   - Sadece TODO'da yazan işleri yap
   - Ek özellik, iyileştirme yapma

2. **Merge Yapma YASAK**
   - Sadece push et
   - Üst Akıl merge eder

3. **Yeni TODO Yazma YASAK**
   - Mevcut TODO yeterli
   - Ekleme yapma

### Proje Spesifik Yasaklar

**Bu proje için:**

1. **String Literal Değiştirme YASAK**
   ```mlp
   -- ❌ YANLIŞ:
   string msg = "Ali; Veli"  -- BOZULDU!
   
   -- ✅ DOĞRU:
   string msg = "Ali, Veli"  -- Virgül korunur
   ```

2. **Ondalık Sayı Değiştirme YASAK**
   ```mlp
   -- ❌ YANLIŞ:
   numeric pi = 3;14  -- BOZULDU!
   
   -- ✅ DOĞRU:
   numeric pi = 3,14  -- Virgül ondalık ayırıcı
   ```

3. **Runtime Değiştirme YASAK**
   - `runtime/runtime.c` dosyasına DOKUNMA!

**Not:** Yeni projede bu bölüm projeye göre değiştirilir.

---

## 📚 Referans Komutlar

**Proje spesifik komutlar (bu proje için):**

```bash
# Proje kök dizini
cd /home/pardus/projeler/mlp-original

# .mlp dosyası sayısı
find . -name "*.mlp" -type f | wc -l

# Tip belirtilmemiş fonksiyonlar
grep -rE "function\s+\w+\([^)]*\w+," --include="*.mlp" | wc -l

# Eski syntax kullanımı
grep -r "end if" --include="*.mlp" | wc -l

# Test derleme
./mlpc examples/hello_english.mlp test.asm
```

**Not:** Yeni projede bu komutlar projeye göre değiştirilir.

---

## 🎯 ÖZET

1. **OKU:** syntax_migration_guide.md
2. **ANLA:** Eski vs PMLP farkları
3. **DÖNÜŞTÜR:** Dosya dosya, commit commit
4. **TEST ET:** Her dosya derlenebilmeli
5. **RAPOR YAZ:** MigYZ_XX_TAMAMLANDI.md
6. **GÜNCELLE:** NEXT_AI_START_HERE.md

**İYİ ÇALIŞMALAR! 🚀**

---

**Versiyon:** 1.0  
**Son Güncelleme:** 25 Aralık 2025

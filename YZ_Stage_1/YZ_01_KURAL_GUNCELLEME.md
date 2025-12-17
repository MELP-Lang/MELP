# YZ_01 Güncelleme Raporu - Kural Güçlendirme

**Tarih:** 17 Aralık 2025  
**YZ:** YZ_01  
**Görev:** MELP Mimarisi kurallarını TODO ve NEXT_AI'ye ekle

---

## 🎯 AMAÇ

**Sorun:** Önceki YZ'ler MELP_Mimarisi.md kurallarını okumadı/ezdi
- Monolitik yapı kullandılar
- Global state kullandılar
- Sonuç: 12K satır kod ama mimari yanlış → Stage 1 baştan!

**Çözüm:** Kuralları TODO.md ve NEXT_AI_START_HERE.md'ye ekleyerek ZORUNLU hale getir

---

## ✅ YAPILAN DEĞİŞİKLİKLER

### 1. TODO.md Güncellendi

**Eklenenler:**
- ⚠️ KRİTİK KURALLAR bölümü
  - 5 Temel Prensip
  - 10 Yasak (detaylı örneklerle)
  - 10 Zorunlu kural
- 📋 HER YZ İÇİN STANDART CHECKLİST
  - Başlamadan önce ZORUNLU onay
  - Geliştirme sırasında kontroller
  - Tamamlandıktan sonra kontroller

**Kod Örnekleri:**
```mlp
-- ❌ YANLIŞ: Global state
list g_tokens
numeric g_current_pos

-- ✅ DOĞRU: Parametre geçişi
function parse(list tokens, numeric position) returns list
    token = tokens[position]
    return [result, new_position]
end_function
```

### 2. NEXT_AI_START_HERE.md Güncellendi

**Eklenenler:**
- ⚠️ HATIRLATMALAR - MELP MİMARİSİ KURALLARI
  - 5 Temel Prensip
  - Yasaklar (detaylı)
  - Zorunlular
  - İhlal örnekleri (kod ile)
- 🚨 DURDURMA SENARYOLARI
  - Ne zaman işi durdur ve kullanıcıya sor
  - Otomatik düzeltme yapma uyarısı
- ✅ Başlangıç Checklist (YZ_01 için örnek)
  - ONAYLA formatı
  - Detaylı onay maddeleri

### 3. Yeni Döküman: KURAL_IHLAL_PROTOKOLU.md

**İçerik:**
- 📋 Her YZ için zorunlu onay formatı
- 🚨 Durdurma senaryoları (6 durum)
- ✅ Kabul edilebilir durumlar
- 🔄 İhlal düzeltilmesi (3 seviye)
- 📝 Raporlama formatı
- 🎯 Başarı/Başarısızlık kriterleri
- Örnek senaryolar

---

## 📊 ÖNCESI vs SONRASI

### Öncesi (Yetersiz)

```markdown
## Kurallar
- Modüler yapı kullan
- Global state kullanma
```

**Sorun:** Çok genel, detay yok, zorunlu değil

### Sonrası (Güçlü)

```markdown
## ⚠️ KRİTİK KURALLAR

### 🎯 5 TEMEL PRENSİP (ASLA İHLAL ETME!)
1. %100 Modüler - Her feature ayrı modül
...

### ❌ YASAKLAR
1. Monolitik dosya oluşturma
   ❌ parser.mlp (tüm parsing)
   ✅ modules/functions/functions_parser.mlp

2. Global state
   ❌ list g_tokens  -- YASAK!
   ✅ const numeric X = 1  -- OK

ONAYLA - YZ_XX:
[ ] Kuralları okudum ve anladım
...
ONAYLIYORUM - YZ_XX
```

**Avantaj:** Detaylı, örnekli, zorunlu onay

---

## 🎯 BEKLENEN SONUÇ

### YZ'ler Artık:

1. **Session başında:**
   - MELP_Mimarisi.md okuyacak
   - TODO.md'deki kuralları okuyacak
   - Checklist onaylayacak
   - **Onay vermeden kod yazmayacak**

2. **Geliştirme sırasında:**
   - Şüpheli durumda DURACAK
   - Kullanıcıya SORACAK
   - Mimari ihlal yapmayacak
   - Önceki YZ'nin kodunu ezmeyecek

3. **Session sonunda:**
   - Rapor yazacak
   - NEXT_AI güncelleyecek
   - Sonraki YZ'ye mesaj bırakacak

### Sonuç:
✅ Stage 1 kusursuz mimari ile tamamlanacak  
✅ Tekrar baştan yapılmayacak  
✅ 38 saat yeterli olacak

---

## 📂 GÜNCELLENEN DOSYALAR

1. `TODO.md`
   - KRİTİK KURALLAR bölümü (+150 satır)
   - STANDART CHECKLİST (+30 satır)

2. `NEXT_AI_START_HERE.md`
   - MELP MİMARİSİ KURALLARI (+100 satır)
   - DURDURMA SENARYOLARI (+30 satır)
   - Başlangıç Checklist (+20 satır)

3. `YZ_Stage_1/KURAL_IHLAL_PROTOKOLU.md` (YENİ)
   - Tam protokol dokümanı (200+ satır)

---

## 💡 ÖNEMLİ NOTLAR

### 1. Zorunlu Onay Mekanizması

Her YZ checklist'i onaylamadan başlayamaz:

```
ONAYLA - YZ_XX:
[ ] Kuralları okudum
[ ] Anladım
...
ONAYLIYORUM - YZ_XX
```

### 2. Durdurma Senaryoları

6 durum tanımlandı:
1. Mimari ihlal
2. Global state
3. Monolitik dosya
4. Önceki YZ'nin kodunu değiştirme
5. Test başarısız
6. Stage 0 bozuldu

### 3. Kabul Edilebilir Durumlar

5 durum açıkça izin verildi:
1. Const kullanımı (immutable)
2. Mevcut kodu yeniden kullanma
3. Import kullanımı
4. Test yazma
5. README yazma

### 4. Örnek Kod Blokları

Her yasak için kod örneği eklendi:
- ❌ YANLIŞ örnek
- ✅ DOĞRU örnek

---

## 🚀 SONRAKİ ADIMLAR

1. **YZ_01 devam:** Import tasarım dokümanı
2. **YZ_02:** Checklist'i onaylar, işe başlar
3. **Her YZ:** Aynı süreci takip eder

---

## ✅ BAŞARI KRİTERİ

Bu güncelleme başarılı sayılır eğer:

- ✅ Hiçbir YZ artık mimari kuralları ihlal etmez
- ✅ Her YZ session başında onay verir
- ✅ Şüpheli durumlarda kullanıcıya sorar
- ✅ Stage 1 baştan yapılmaz
- ✅ 38 saat içinde kusursuz mimari elde edilir

---

**KURAL GÜÇLENDİRME TAMAMLANDI!** 🎯

Artık her YZ kuralları bilecek ve uygulayacak.

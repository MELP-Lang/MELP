# 🚨 KURAL İHLAL PROTOKOLÜ

**Amaç:** YZ'lerin mimari kuralları ihlal etmesini önlemek

---

## ⚠️ ARKA PLAN

**Sorun:** Önceki YZ'ler MELP_Mimarisi.md kurallarını okumadı/ezdi:
- Monolitik yapı kullandılar (modüler yerine)
- Global state kullandılar (stateless yerine)
- 12K satır kod yazıldı ama mimari YANLIŞ

**Sonuç:** Stage 1 baştan yapılıyor (38 saat ek iş)

**Çözüm:** Bu protokol ile kurallar ZORUNLU hale getiriliyor.

---

## 📋 HER YZ İÇİN ZORUNLU ONAY

### Session Başında

Her YZ **mutlaka** aşağıdaki checklist'i ONAYLAMALI:

```
ONAYLA - YZ_XX:

[ ] NEXT_AI_START_HERE.md okudum
[ ] MELP_Mimarisi.md okudum ve ONAYLADIM
[ ] TODO.md okudum
[ ] 5 Temel Prensibi anladım:
    [ ] %100 Modüler (feature bazlı)
    [ ] LLVM Backend (değişmez)
    [ ] Stateless (global state YASAK)
    [ ] STO (runtime optimization)
    [ ] Struct + Functions (OOP yok)
[ ] Yasakları anladım:
    [ ] Monolitik dosya YASAK
    [ ] Global state YASAK (const OK)
    [ ] Önceki kuralları ezme YASAK
    [ ] Ana dizine test/geçici dosya YASAK

ONAYLIYORUM - YZ_XX
```

**Onay vermeden kod yazmaya BAŞLAMA!**

---

## 🚨 DURDURMA SENARYOLARI

Aşağıdaki durumlarda **İŞİ DURDUR** ve kullanıcıya **SOR**:

### 1. Mimari İhlal Gördün
```
DURDUR! Mimari ihlal tespit ettim:

Sorun: [açıklama]
Konum: [dosya:satır]
Kural: [hangi kural ihlal edilmiş]

Ne yapmalıyım?
A) Düzelt
B) Devam et (ihlali onayla)
C) Başka çözüm öner
```

### 2. Global State Kullanımı
```mlp
-- Mevcut kodda bunu gördüm:
list g_tokens = []  -- ❌ Global state!

DURDUR! Bu stateless kuralını ihlal ediyor.
Ne yapmalıyım?
A) Stateless pattern'e çevir
B) const yap (immutable)
C) Başka çözüm
```

### 3. Monolitik Dosya
```
DURDUR! parser.mlp dosyası monolitik yapı içeriyor.

Sorun: 1000+ satır, tüm parsing işlemleri tek dosyada
Kural: Her feature ayrı modül olmalı

Ne yapmalıyım?
A) Modüler yapıya böl
B) Devam et (geçici kabul et)
C) Başka çözüm
```

### 4. Önceki YZ'nin Kodunu Değiştiriyorsun
```
DURDUR! YZ_10'un yazdığı kodu değiştirmek istiyorum.

Dosya: modules/functions/functions_parser.mlp
Değişiklik: [açıklama]
Neden: [sebep]

Onayınız var mı?
```

### 5. Test Başarısız
```
DURDUR! Test başarısız oldu.

Test: test_function_parsing.mlp
Hata: [hata mesajı]

Ne yapmalıyım?
A) Debug et
B) Testi değiştir
C) Başka çözüm
```

### 6. Stage 0 Bozuldu
```
DURDUR! Stage 0 compiler artık çalışmıyor.

Değişiklik: [ne değiştirdim]
Hata: [hata mesajı]

Ne yapmalıyım?
A) Geri al
B) Düzelt
C) Başka çözüm
```

### 7. Ana Dizine Test/Geçici Dosya Oluşturma
```
DURDUR! Ana dizine test dosyası oluşturuyorsun.

Dosya: /home/pardus/projeler/MLP/MLP/test_my_feature.mlp
Kural: Test dosyaları tests/stage_1_tests/ dizininde olmalı

Ne yapmalıyım?
A) tests/stage_1_tests/ dizinine taşı
B) Sil
```

---

## ✅ KABUL EDİLEBİLİR DURUMLAR

Aşağıdaki durumlarda **kullanıcıya sormadan** devam edebilirsin:

### 1. Const Kullanımı (Immutable)
```mlp
-- ✅ OK: Compile-time constant, immutable
const numeric TOKEN_ID = 1
const string VERSION = "1.0.0"
```

### 2. Mevcut Kodu Yeniden Kullanma
```mlp
-- ✅ OK: Mevcut koddan fonksiyon kopyala/extract
-- modules/functions/functions_parser.mlp
-- (parser_mlp/parser_func.mlp'den alındı)
function parse_function_declaration(...)
    -- kod
end_function
```

### 3. Import Kullanımı (FAZ 2+)
```mlp
-- ✅ OK: Import sistemi çalışıyorsa
import "../core/token_types.mlp"
import "../core/char_utils.mlp"
```

### 4. Test Yazma
```mlp
-- ✅ OK: Her modül için test yaz
-- modules/functions/functions_test.mlp
function test_function_parsing() returns numeric
    -- test kodu
end_function
```

### 5. README Yazma
```markdown
# Functions Module

✅ OK: Her modül README içermeli
```

### 6. Doğru Dizinde Test Yazma
```mlp
-- ✅ OK: tests/stage_1_tests/ dizininde
-- tests/stage_1_tests/test_functions.mlp
function test_parse_function() returns numeric
    -- test kodu
end_function
```

**❌ YANLIŞ:** Ana dizinde test
```bash
/home/pardus/projeler/MLP/MLP/test_my_feature.mlp  # YASAK!
```

---

## 🔄 İHLAL DÜZELTİLMESİ

İhlal tespit edilirse:

### Seviye 1: Uyarı
```
⚠️ UYARI: Potansiyel kural ihlali

Sorun: [açıklama]
Öneri: [nasıl düzeltilir]

Devam ediyorum...
```

### Seviye 2: Durdur ve Sor
```
🛑 DURDURULDU: Kural ihlali

Sorun: [açıklama]
Kural: [hangi kural]

Ne yapmalıyım? (Kullanıcıya soruldu)
```

### Seviye 3: Geri Al
```
⛔ GERİ ALINDI: Ciddi ihlal

Sorun: [açıklama]
Aksiyon: git reset --hard [önceki commit]

Kullanıcı onayı bekleniyor...
```

---

## 📝 RAPORLAMA

Her YZ session sonunda raporda belirt:

```markdown
## Kural Uyumu

✅ Mimari kurallarına %100 uydum
✅ Global state kullanmadım
✅ Modüler yapı korudum
✅ Önceki YZ'lerin kodunu değiştirmedim

veya

⚠️ Kural İhlalleri:
- [ihlal 1] - [neden] - [nasıl düzeltildi]
- [ihlal 2] - [neden] - [kullanıcı onayladı]
```

---

## 🎯 BAŞARI KRİTERLERİ

Bir YZ başarılı sayılır eğer:

1. ✅ Checklist'i onayladı
2. ✅ Mimari kurallarına uydu
3. ✅ Testler geçti
4. ✅ Rapor yazdı
5. ✅ NEXT_AI güncelledi
6. ✅ Sonraki YZ'ye mesaj bıraktı

---

## 🚫 BAŞARISIZLIK KRİTERLERİ

Bir YZ başarısız sayılır eğer:

1. ❌ Checklist onaylamadan başladı
2. ❌ Mimari kuralları ihlal etti
3. ❌ Önceki YZ'nin kodunu kullanıcı onayı olmadan değiştirdi
4. ❌ Global state kullandı
5. ❌ Monolitik yapı oluşturdu
6. ❌ Testleri pass etmeden devam etti
7. ❌ Rapor yazmadı

**Başarısız YZ:** İşi sonraki YZ'ye devret, kullanıcıya bildir.

---

## 📖 ÖRNEK SENARYO

### ❌ Yanlış Yaklaşım

```
YZ_05: Core utilities yazıyorum...

// Kod yazdı, global state kullandı
list g_char_buffer = []

// Test etmedi
// Rapor yazmadı
// Git push yapmadı

SONUÇ: ❌ Başarısız - Kurallar ihlal edildi
```

### ✅ Doğru Yaklaşım

```
YZ_05: Başlıyorum...

1. NEXT_AI okudum ✅
2. MELP_Mimarisi.md okudum ✅
3. Checklist onayladım ✅

   ONAYLIYORUM - YZ_05

4. Kod yazdım (stateless) ✅
5. Test yazdım ✅
6. Test geçti ✅
7. Rapor yazdım ✅
8. Git commit + push ✅
9. NEXT_AI güncelledim ✅

SONUÇ: ✅ Başarılı
```

---

**Bu protokole uyarak Stage 1 kusursuz tamamlanacak!** 🎯

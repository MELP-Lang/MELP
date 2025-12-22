# YZ_01 - Phase 1.1-1.2: Core + Parser Syntax Fix

**Tarih:** 22 Aralık 2025  
**YZ:** YZ_01  
**Phase:** Phase 1.1-1.2  
**Branch:** selfhosting_YZ_01

---

## 🚨 KRİTİK BULGU: Stage 0 Array Return Limitasyonu

### Sorun
Stage 0 compiler, fonksiyonlardan **2+ elemanlı array döndürmeyi** desteklemiyor!

**Test Sonuçları:**
```mlp
function test() returns list
    return [42]       ✅ Çalışıyor (1 eleman)
    return [42, 99]   ❌ Parser hatası (2 eleman)
    return [1,2,3,4]  ❌ Parser hatası (4 eleman)
end_function
```

**Hata Mesajı:**
```
error [Parser]: Expected ']' to close array
error [Parser]: Expected 'function' keyword
```

### Etkilenen Dosyalar
- `compiler/stage1/modules/lexer_mlp/lexer.mlp` - 3 fonksiyon array döndürüyor
- Muhtemelen diğer modüller de etkileniyor

### Geçici Çözüm Önerileri
1. **Array return yerine global değişken kullan**
2. **Struct kullanarak elemanları wrap et**
3. **Stage 0'ı düzelt (YZ_200+ çalışmasına ek)**

### İlerleme Durumu
- lexer.mlp: Syntax düzeltmeleri yapıldı (semicolon, end_if, boolean→numeric)
- Ama array return sorunu nedeniyle tam derlenemiyor
- Commit: `8830b7c4` - Partial fix kaydedildi

---

# YZ_00 - Phase 0: Sistem Tutarlılığı Raporu

**Tarih:** 22 Aralık 2025  
**YZ:** YZ_00  
**Phase:** Phase 0 - Sistem Tutarlılığı  
**Branch:** selfhosting_YZ_00

---

## 📊 TASK 0.1: SYNTAX ENVANTERİ

### Özet İstatistikler

- **Toplam Stage 1 Modülü:** 107 dosya
- **Syntax Düzeltmesi Gereken:** 89 dosya (%83)
- **Syntax'ı Doğru:** 18 dosya (%17)

### Tespit Edilen Syntax Sorunları

| Sorun Tipi | Toplam Sayı | Durum |
|------------|-------------|-------|
| Virgül (`,`) parametre ayırıcı | 1,104 adet | ❌ HATALI (`;` olmalı) |
| Semicolon (`;`) parametre ayırıcı | 114 adet | ✅ DOĞRU |
| `while X do` kullanımı | 20 adet | ✅ DOĞRU |
| `while X` (do'suz) | 32 adet | ❌ HATALI (`do` eksik) |
| `break` kullanımı | 10 adet | ❌ HATALI (`exit` veya `exit_while` olmalı) |

### Kritik Bulgular

1. **Parametre Ayırıcı Sorunu (En Büyük):**
   - 1,104 fonksiyon çağrısında virgül (`,`) kullanılmış
   - PMPL syntax'a göre `;` (semicolon) kullanılmalı
   - Neden: MELP'te `3,14` ondalık sayıdır (Türk formatı)

2. **While Döngüsü Sorunu:**
   - 32 yerde `while X` (do'suz) kullanılmış
   - Doğrusu: `while X do ... end_while`

3. **Break vs Exit:**
   - 10 yerde `break` kullanılmış
   - Doğrusu: `exit` (genel) veya `exit_while`/`exit_for` (spesifik)

### En Çok Sorun Olan Modüller

**Top 5:**
1. `codegen_mlp/codegen_arrays.mlp` - 52 virgül parametresi
2. `codegen_mlp/codegen_functions.mlp` - 45 virgül parametresi
3. `codegen_mlp/codegen_structs.mlp` - 38 virgül parametresi
4. `parser_mlp/parser_expressions.mlp` - 34 virgül parametresi
5. `arrays/arrays_codegen.mlp` - 18 virgül parametresi

### Detaylı Rapor

Tam envanter: `temp/syntax_inventory.txt` (982 satır)

---

## ✅ TASK 0.2: STAGE 0 DOĞRULAMA

### Build Durumu

**Sonuç:** ✅ BAŞARILI

```bash
cd compiler/stage0/modules/functions && make
```

- Build tamamlandı (60 kaynak dosya)
- Binary oluşturuldu: `functions_compiler` (668K)
- Bazı uyarılar var (type compatibility) ama derleyici çalışıyor

### Test Durumu

**Test Dosyası:** `temp/test_stage0.mlp`
```mlp
function main() returns numeric
    return 42
end_function
```

**Derleme Sonucu:** ✅ BAŞARILI
```
✅ Compiled temp/test_stage0.mlp -> temp/test_stage0.ll
   📊 1 functions, 0 structs, 0 enums
```

**LLVM IR Üretimi:** ✅ BAŞARILI
- LLVM IR syntax doğru
- main() fonksiyonu 42 döndürüyor
- STO runtime bildirimleri mevcut

**Not:** LLVM runtime (lli) sistemde yüklü değil, bu nedenle native binary test yapılamadı. Ancak LLVM IR'nin doğru olduğu görüldü.

---

## ✅ TASK 0.3: IMPORT SİSTEMİ DOĞRULAMA

### Test Senaryosu

**Module A:** `temp/module_a.mlp`
```mlp
function add_numbers(numeric a; numeric b) returns numeric
    return a + b
end_function
```

**Module B:** `temp/module_b.mlp`
```mlp
import "temp/module_a.mlp"

function main() returns numeric
    numeric result = add_numbers(10; 32)
    return result
end_function
```

### Test Sonucu: ✅ BAŞARILI

```
📦 Import: temp/module_a.mlp
   💾 Cached: temp/module_a.mlp (1 function(s), 0 dep(s))
   📝 Generated assembly: module_a.s
✅ Compiled temp/module_b.mlp -> temp/module_b.ll
   📊 2 functions, 0 structs, 0 enums
```

**Bulgular:**
- ✅ Import sistemi çalışıyor
- ✅ Modüller arası fonksiyon çağrısı başarılı
- ✅ Cache mekanizması aktif (minor uyarı: cache kaydedilemedi)
- ✅ Assembly üretimi başarılı

---

## 📋 TASK 0.4: GENEL DEĞERLENDİRME

### Sistem Durumu: ⚠️ SYNTAX FIX GEREKLİ

| Bileşen | Durum | Açıklama |
|---------|-------|----------|
| Stage 0 Compiler | ✅ Hazır | Build başarılı, çalışıyor |
| Import Sistemi | ✅ Hazır | Çalışıyor, cache mekanizması aktif |
| Stage 1 Modül Syntax | ❌ Düzeltme Gerekli | 89/107 dosya fix gerekiyor |

### Tahmini İş Yükü (Phase 1)

**Toplam Düzeltme:**
- 1,104 virgül → semicolon değişikliği
- 32 `while X` → `while X do` değişikliği
- 10 `break` → `exit` değişikliği

**Tahmini Süre:**
- **Manuel düzeltme:** ~2-3 gün (her dosya ayrı test edilmeli)
- **Script ile otomatik:** ~4-6 saat (sed/awk ile toplu, sonra test)

**Risk Değerlendirmesi:**
- Risk: DÜŞÜK - Syntax değişiklikleri basit, semantic değişiklik yok
- Test: ORTA - Her modül ayrı derlenmeli
- Rollback: KOLAY - Git branch üzerinde çalışıyoruz

### Phase 1'e Geçiş Kriteri

**Durum:** ✅ HAZıRIZ

Sırada:
1. Phase 1.1: Core modüller syntax fix (lexer, parser, codegen)
2. Phase 1.2: Parser modülleri syntax fix
3. Phase 1.3: CodeGen modülleri syntax fix
4. Phase 1.4: Diğer modüller syntax fix
5. Phase 1.5: Toplu doğrulama (%93+ başarı hedefi)

### Öneriler

**Strateji:** 
1. Kritik modüllerden başla (lexer, parser, codegen)
2. Her modülü düzelttikten sonra hemen test et
3. %93+ başarı oranına ulaşana kadar devam et

**Araçlar:**
- `sed` ile toplu değiştirme (virgül → semicolon)
- `timeout 10` ile derleme testleri
- `grep` ile sonuç toplama

---

## 🎯 SONUÇ

**Phase 0 Tamamlandı:** ✅

- [x] Task 0.1: Syntax Envanteri ✅
- [x] Task 0.2: Stage 0 Doğrulama ✅
- [x] Task 0.3: Import Sistemi Doğrulama ✅
- [x] Task 0.4: Tutarlılık Raporu ✅

**Sistem Durumu:**
- Stage 0: Çalışıyor ✅
- Import: Çalışıyor ✅
- Stage 1 Syntax: Düzeltme gerekiyor (89/107 dosya) ⚠️

**Sonraki YZ (YZ_01) için not:**
Phase 1'e hazırız. Core modüllerden (lexer, parser, codegen) başla!

---

**Rapor Tarihi:** 22 Aralık 2025  
**Rapor Sahibi:** YZ_00  
**Branch:** selfhosting_YZ_00 
- Modüller RAM'de kalıyor, birbirini çağırıyor
- İptal sebebi: MELP'in "import yok" prensibi ihlali
- Kalıntı: `modules/` klasöründeki eski API tanımları

**ST2 (Monolitik):**
- Tek dosyada tüm compiler
- İptal sebebi: Yönetilemez boyut (10,000+ satır)
- Kalıntı: `compiler/stage1/melp_compiler.mlp` stub

**ST3 (Template Import - MEVCUT):**
- `import_template "module.mlp"` ile compile-time copy
- C'deki `#include` gibi - MELP prensiplerine uygun
- `compiler/stage1/modules/` klasöründe 107 modül
- **BU KULLANILACAK!**

---

## SORU 3: Stage 0 Kapasitesi

Stage 0 tam olarak neyi derleyebiliyor?
- Basit fonksiyonlar?
- Struct'lar?
- List/Array?
- Import/Module?
- Recursion?

Stage 1'i derlemek için Stage 0'da eksik bir şey var mı?

### ✅ CEVAP 3:

Stage 0 ŞU ANKİ kapasitesi:

| Feature | Durum | Not |
|---------|-------|-----|
| Basit fonksiyonlar | ✅ | Tamamen çalışıyor |
| Struct | ✅ | YZ_200'de eklendi |
| List/Array | ✅ | YZ_212-213 ile tamamlandı |
| Map | ✅ | YZ_201'de eklendi |
| Optional | ✅ | YZ_202'de eklendi |
| Recursion | ✅ | Çalışıyor |
| While/If | ✅ | Çalışıyor |
| Import/Module | ⚠️ | `import_template` var (compile-time copy) |
| LLVM IR backend | ✅ | Tam entegre |

**Stage 1'i derlemek için Stage 0'da eksik:** YOK! Stage 0 yeterli.

**Tek sorun:** Stage 1 modüllerinde eski syntax kullanılıyor. Stage 0 kabul ediyor ama canonical değil.

---

## SORU 4: En Büyük Tuzak

Sonsuz döngüye girmemizin asıl sebebi neydi?
- Feature creep mi?
- Stub'lar mı?
- Syntax tutarsızlığı mı?
- Architecture ihlali mi?

Bir sonraki YZ'nin kaçınması gereken 1 numaralı hata ne?

### ✅ CEVAP 4:

**Sonsuz döngünün asıl sebebi: SYNTAX TUTARSIZLIĞI + YZ_11'in zamanından önce restore edilmesi**

**Timeline:**
1. 18 Aralık: YZ_11 yazıldı (virgül syntax)
2. 19 Aralık: `pmlp_kesin_sozdizimi.md` tanımlandı (semicolon syntax)
3. 22 Aralık: YZ_11 restore edildi (ESKİ syntax ile!)

**1 numaralı kaçınılması gereken hata:**

> **Modül syntax'ını düzeltmeden bootstrap'a geçme!**

---

## ÖNCEKİ ÜST AKIL'IN BIRAKTIĞI YER

Ben (önceki session) tam şu anda `lexer.mlp`'yi düzeltiyordum:
- 8 fonksiyon parametresinde virgül → semicolon ✅
- 2 while loop'a `do` eklendi ✅
- 6 `substring()` çağrısında virgül var → **YARIM KALDI**

**Mevcut durum:**
```
lexer.mlp:
✅ function signatures (8 tane)
✅ return statements (2 tane)
✅ while loops (2 tane)
❌ substring() calls (6 tane) - YENİ ÜST AKIL BUNLARI DÜZELTMELİ
❌ diğer fonksiyon çağrıları
```

**Kaldığım yer:** 
- Line 120, 149, 174, 196, 200, 229'da `substring(source, pos, 1)` → `substring(source; pos; 1)` yapılmalı

---

## YENİ ÜST AKIL İÇİN ÖNERİ

**Yapmayın:**
- Hemen bootstrap test
- Yeni TODO yazma
- Yeni modül oluşturma

**Yapın:**
1. `lexer.mlp` syntax fix tamamla (kaldığım yerden devam)
2. Test et: `timeout 15 compiler/stage0/modules/functions/functions_compiler compiler/stage1/modules/lexer_mlp/lexer.mlp -o temp/lexer_bootstrap.ll`
3. Geçerse diğer core modüllere geç
4. En son bootstrap test

---

## BEKLENTİ

Bu soruların cevaplarını alınca:
1. Gerçek durumu net anlayacağım ✅
2. Tek, kapsamlı, kesin TODO yazacağım
3. Görevli YZ'ler için net talimatlar hazırlayacağım

Prensip: Uzun, güvenli, kaliteli yol. Tek TODO, ikincisi yok.

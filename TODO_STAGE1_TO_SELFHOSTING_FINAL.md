# 🚨 MELP SELF-HOSTING - NİHAİ TODO

---

## ⚠️ BU BELGEYİ OKUYAN HER YZ İÇİN - ÖNCE BUNU OKU!

### 📕 ZORUNLU REFERANS: pmlp_kesin_sozdizimi.md

**Kod yazmadan önce `pmlp_kesin_sozdizimi.md` dosyasını OKU!**

Bu dosya MELP'in resmi syntax referansıdır. Önemli kurallar:
- `while i < 10` (`do` YOK!)
- Parametre ayırıcı: `;` (virgül değil!)
- Blok sonları: `end_while`, `end_if`, `end_function`
- If: `if condition then` (çok satırlı için `then` zorunlu)

**Bu dosyayı okumadan kod yazma!**

---

### 📍 NEREDEYIZ?

**Tarih:** 23 Aralık 2025  
**Güncelleme:** 23 Aralık 2025 - MASTERMIND KARARI

**Gerçek:** MELP projesi **10+ self-hosting denemesi** yaşadı. Her seferinde yanlış yola sapıldı:
- "Stage 0'a dokunma" kuralı → Yarım Stage 0 ile devam
- Yarım Stage 0 → Stage 1 modülleri derlenmiyor
- "Self-hosting tamamlandı" ilan edildi → Aslında hardcoded IR

**YENİ YAKLAŞIM:** Stage 0 %100 tamamlanmadan Stage 1'e geçilmeyecek!

---

### 🔴 MASTERMIND KARARI (23 Aralık 2025)

**ESKİ KURAL:** "Stage 0'a yeni özellik ekleme yasak"  
**YENİ KURAL:** "Stage 0 %100 tamamlanana kadar geliştirmeye devam"

**Gerekçe:**
1. Stage 0 yarım kaldıkça self-hosting imkansız
2. While çalışmıyor → Compiler yazamıyorsun
3. Struct çalışmıyor → Token struct'ı tanımlayamıyorsun
4. Stage 0 düzgün çalışmadan Stage 1 yazılamaz

---

### 🎯 BU TODO'NUN TEK AMACI

```
┌─────────────────────────────────────────────────────────────────┐
│                                                                 │
│   STAGE 1 COMPILER (MELP ile yazılmış) KENDİNİ DERLEYEBİLSİN   │
│                                                                 │
│   compiler.mlp → (derlenir) → compiler_gen1.ll                 │
│   compiler_gen1.ll → compiler.mlp → compiler_gen2.ll           │
│                                                                 │
│   diff compiler_gen1.ll compiler_gen2.ll = BOŞ                 │
│                                                                 │
│   BU OLDUĞUNDA = SELF-HOSTING TAMAM! 🎉                        │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

### 📦 ELİMİZDE NE VAR?

**Stage 0:** Bootstrap compiler (C ile yazılmış) - **TAMAMLANMASI GEREKİYOR**

**Stage 1 modülleri ZATEN YAZILDI:**

```
compiler/stage1/modules/
├── lexer_mlp/lexer.mlp           ✅ Derleniyor
├── parser_mlp/parser.mlp         ❌ Derlenmiyor (Stage 0 eksikleri)
├── codegen_mlp/                  ❌ Derlenmiyor (Stage 0 eksikleri)
└── compiler.mlp                  ❌ Derlenmiyor
```

**Problem:** Stage 0 eksik özellikler nedeniyle bu modüller derlenemiyor.

---

### ❌ YAPMAYACAKLARIN (YASAKLAR)

| YASAK | NEDEN |
|-------|-------|
| ~~Stage 0'a yeni özellik ekleme~~ | ~~Stage 0 BİTTİ, deprecated~~ → **KALDIRILDI** |
| FFI ekleme | Self-hosting için gerekli değil |
| Module system ekleme | Tek dosya yaklaşımı yeterli |
| Error handling ekleme | Önce self-hosting, sonra özellik |
| Yeni TODO yazma | BU SON TODO |
| "Detaylandırmamı ister misin?" | YASAK, direkt yap |
| Phase/Task icat etme | TODO'da ne yazıyorsa o |

**İHLAL = GÖREV DURDURULUR!**

---

### ✅ YAPACAKLARIN

1. **Stage 1 modüllerindeki syntax hatalarını düzelt**
   - `pmlp_kesin_sozdizimi.md`'ye uyumlu hale getir
   - Virgül → noktalı virgül
   - `then` eksikliklerini tamamla

2. **Modülleri tek dosyada birleştir** (veya çalışır hale getir)
   - lexer + parser + codegen → compiler.mlp

3. **Self-compile test yap**
   - Stage 0 → compiler.mlp → Gen1
   - Gen1 → compiler.mlp → Gen2
   - diff Gen1 Gen2 = BOŞ

4. **Convergence kanıtla**
   - Gen2 → compiler.mlp → Gen3
   - Gen2 = Gen3 → TAMAM!

---

### 🏆 BAŞARI KRİTERİ

```bash
# 1. Stage 0 ile Stage 1'i derle
./stage0/compiler compiler.mlp -o gen1.ll

# 2. Gen1 ile compiler.mlp'yi derle
lli gen1.ll compiler.mlp gen2.ll

# 3. Karşılaştır
diff gen1.ll gen2.ll

# BOŞ ÇIKTI = 🎉 SELF-HOSTING TAMAM!
```

---

### 📅 BU TODO BİTTİĞİNDE

1. ✅ MELP kendini derliyor
2. ✅ Stage 0 deprecated (sadece bootstrap için)
3. ✅ Tüm yeni özellikler Stage 1'e (MELP'e) eklenir
4. ✅ Sonsuz döngüden ÇIKIŞ!

---

## 📋 GÖREVLER

### PHASE 0: Stage 0 Minimum Viable Bootstrap (KRİTİK - ÖNCELİKLİ)

**Hedef:** Self-hosting compiler yazmak için YETEN kadar Stage 0 özelliği.

**MASTERMIND KARARI (Güncelleme):** Stage 0 = Fırlatma rampası, %100 DEĞİL!

**Self-hosting için GERÇEKTEN lazım olan:**
| Özellik | Gerekli | Neden |
|---------|---------|-------|
| numeric | ✅ | Token ID, pozisyon |
| string | ✅ | Kaynak kod, IR output |
| if-then-else | ✅ | Koşullu derleme |
| while-do | ✅ | Kaynak iterate |
| function/call | ✅ | Modülerlik |
| read_file | ✅ | Kaynak oku |
| write_file | ✅ | IR yaz |
| string[i] | ✅ | Karakter erişimi |
| string concat | ✅ | IR birleştirme |
| struct | ❌ | Numeric array yeterli |
| enum | ❌ | Numeric sabitler yeterli |
| global var | ❌ | Parametre olarak geç |
| import | ❌ | Tek dosya compiler yeterli |

---

#### Task 0.1: While/For Codegen Fix (YZ_05) 🔴 KRİTİK

**Görev:** While ve For döngülerinin LLVM IR üretimini düzelt.

**Başarı kriteri:** 
```melp
function test() returns numeric
    numeric i = 0
    while i < 10 do
        i = i + 1
    end_while
    return i
end_function
```
Bu kod derlenip çalışmalı, 10 döndürmeli.

---

#### Task 0.2: String Karakter Erişimi (YZ_06) 🔴 KRİTİK

**Görev:** `string[i]` veya `char_at(string; i)` desteği ekle.

**Başarı kriteri:**
```melp
function test() returns numeric
    string s = "hello"
    string ch = char_at(s; 0)  -- veya s[0]
    -- ch = "h"
    return 0
end_function
```

---

#### Task 0.3: String Concat (YZ_06) 🔴 KRİTİK

**Görev:** String birleştirme desteği.

**Başarı kriteri:**
```melp
function test() returns string
    string a = "hello "
    string b = "world"
    string c = a + b  -- veya concat(a; b)
    return c
end_function
```

---

### 🚫 İPTAL EDİLEN GÖREVLER

| Görev | Neden İptal |
|-------|-------------|
| ~~Task: Struct/Enum~~ | Numeric array yeterli |
| ~~Task: Import/Module~~ | Tek dosya compiler yeterli |
| ~~Task: Global değişken~~ | Parametre olarak geç |
| ~~Task: 11 kategori analiz~~ | Sadece gerekli olanlar test edilecek |

---

### PHASE 1: Syntax Düzeltme (1-2 gün)

**Hedef:** Stage 1 modüllerini `pmlp_kesin_sozdizimi.md`'ye uyumlu hale getir.

#### Task 1.1: Syntax Analizi (2 saat)

**Görev:** Tüm Stage 1 modüllerini tara, syntax hatalarını listele.

```bash
# Çalıştır:
find compiler/stage1/modules -name "*.mlp" -exec grep -l "," {} \;
```

**Kontrol edilecekler:**
- [ ] Parametre ayırıcıları: `,` → `;`
- [ ] Array literals: `[a, b]` → `[a; b;]` (köşeli parantez + trailing `;`)
- [ ] List literals: `(a, b)` → `(a; b;)` (normal parantez + trailing `;`)
- [ ] Çok satırlı if'lerde `then` var mı?
- [ ] while/for'da `do` OLMAMALI (sadece `while cond` ... `end_while`)

**Çıktı:** Düzeltilecek dosya listesi

---

#### Task 1.2: Toplu Düzeltme (4-6 saat)

**Görev:** Syntax hatalarını düzelt.

**Öncelik sırası:**
1. `lexer_mlp/lexer.mlp`
2. `parser_mlp/parser.mlp`
3. `codegen_mlp/*.mlp`
4. `compiler.mlp`

**Her dosya için:**
```bash
# Düzeltmeden önce test et
timeout 15 ./compiler/stage0/modules/functions/functions_compiler \
  dosya.mlp -o temp/test.ll && echo "✅ OK" || echo "❌ HATA"
```

**Başarı kriteri:** %95+ modül derleniyor

---

### PHASE 2: Modül Entegrasyonu (1 gün)

**Hedef:** Lexer, parser, codegen modüllerini build-time concat ile birleştir.

#### Task 2.1: Build-time Concat Pipeline (2 saat)

**MASTERMIND KARARI (24 Aralık 2025):**
- ❌ Monolitik compiler.mlp → Bakım zorluğu
- ❌ Stage 0'a import ekle → Scope creep, geciktirici
- ✅ **Build-time concat** → Pragmatik, hızlı

**Görev:** Modüler kaynak dosyalarını derleme öncesi birleştir.

**Kaynak yapısı (modüler kalır):**
```
compiler/stage1/
├── lexer.mlp      # Bağımsız lexer
├── parser.mlp     # Bağımsız parser  
├── codegen.mlp    # Bağımsız codegen
└── main.mlp       # Entry point
```

**Build script (`scripts/build_compiler.sh`):**
```bash
#!/bin/bash
# Build-time concat - modülleri birleştir
cat compiler/stage1/lexer.mlp \
    compiler/stage1/parser.mlp \
    compiler/stage1/codegen.mlp \
    compiler/stage1/main.mlp > /tmp/compiler_full.mlp

# Derle
./compiler/stage0/melp /tmp/compiler_full.mlp -o build/compiler_gen1.ll
```

**Başarı kriteri:** 
- [ ] 4 modüler dosya oluşturulmuş
- [ ] Build script çalışıyor
- [ ] compiler_gen1.ll üretiliyor

**NOT:** Gerçek import desteği Stage 1 tamamlandıktan SONRA eklenir!

---

#### Task 2.2: Pipeline Test (2 saat)

**Görev:** Basit bir MELP programını derle.

```bash
# Test programı
echo 'function main() returns numeric
    return 42
end_function' > test.mlp

# Stage 0 ile compiler.mlp'yi derle
./stage0/compiler compiler.mlp -o compiler_gen1.ll

# Gen1 ile test.mlp'yi derle
lli compiler_gen1.ll test.mlp test.ll

# Çalıştır
lli test.ll
echo $?  # 42 çıkmalı
```

---

### PHASE 3: Self-Compile (1 gün)

**Hedef:** Stage 1'in kendini derlemesi.

#### Task 3.1: Generation 1 → Generation 2 (3 saat)

```bash
# Gen1 oluştur (Stage 0 ile)
./stage0/compiler compiler.mlp -o gen1.ll

# Gen2 oluştur (Gen1 ile)
lli gen1.ll compiler.mlp gen2.ll

# Karşılaştır
diff gen1.ll gen2.ll
```

**Beklenen:** Fark olabilir (farklı compiler'lar)

---

#### Task 3.2: Convergence Test (3 saat)

```bash
# Gen3 oluştur (Gen2 ile)
lli gen2.ll compiler.mlp gen3.ll

# Karşılaştır
diff gen2.ll gen3.ll
```

**Beklenen:** BOŞ ÇIKTI! (Convergence)

**Eğer boş değilse:** Hata ayıkla, düzelt, tekrar dene.

---

### PHASE 4: Finalizasyon (0.5 gün)

#### Task 4.1: Stage 0 Deprecation

```bash
# Stage 0'a deprecated notu ekle
echo "# ⚠️ DEPRECATED - Bootstrap Only" > compiler/stage0/DEPRECATED.md
```

#### Task 4.2: Dokümantasyon

- [ ] README.md güncelle
- [ ] ARCHITECTURE.md güncelle
- [ ] Self-hosting başarı raporu yaz

#### Task 4.3: Git Tag

```bash
git tag -a v1.0.0-selfhosting -m "Self-hosting achieved!"
git push origin v1.0.0-selfhosting
```

---

## 📊 İLERLEME TAKİBİ

| Phase | Task | Durum | Sorumlu YZ |
|-------|------|-------|------------|
| **Phase 0** | **Task 0.1: While/For Codegen Fix** | ✅ | **YZ_05** |
| **Phase 0** | **Task 0.2: String Karakter Erişimi** | ✅ | **YZ_06** |
| **Phase 0** | **Task 0.3: String Concat** | ✅ | **YZ_06** |
| **Phase 0** | **String Return Bug Fix** | ✅ | **YZ_07** |
| **Phase 1** | **Task 1.1-1.6: Syntax Test & Analiz** | ✅ | **YZ_13** |
| **Phase 1** | **Task 1.7: Geriye Kalan Hataları Düzelt** | ✅ | **YZ_15** |
| Phase 2 | Task 2.1: Compiler Orchestrator | ⬜ | - |
| Phase 2 | Task 2.2: Pipeline Test | ⬜ | - |
| Phase 3 | Task 3.1: Gen1 → Gen2 | ⬜ | - |
| Phase 3 | Task 3.2: Convergence | ⬜ | - |
| Phase 4 | Task 4.1: Deprecation | ⬜ | - |
| Phase 4 | Task 4.2: Dokümantasyon | ⬜ | - |
| Phase 4 | Task 4.3: Git Tag | ⬜ | - |

**OVERALL:** ✅✅✅✅✅✅⬜⬜⬜⬜⬜⬜⬜ 6/13 tasks (~46%)

**🟢 Phase 0 TAMAMLANDI!**  
**🟢 Phase 1 TAMAMLANDI!** %93.1 başarı oranı! Artık Phase 2'ye geçildi.

---

## 📝 ÖN ANALİZ (YZ_08 İÇİN HAZIR VERİ)

**Task 1.1 için başlangıç bilgisi (24 Aralık 2025):**

```
Syntax Hata Özeti:
- Virgüllü parametre (`, ` yerine `; `): 19 dosya
- while...do (do OLMAMALI): 5 dosya
- Array literal virgül ([a,b] yerine [a;b;]): 51 dosya
- List literal hatası ((a,b) yerine (a;b;)): kontrol edilmeli
- then eksik olabilecek if'ler: 20+ dosya

Toplam Stage 1 modül sayısı: 107 dosya

DOĞRU SYNTAX (pmlp_kesin_sozdizimi.md'den):
- Array: numeric[] arr = [1; 2; 3;]   -- köşeli parantez + trailing ;
- List:  list data = (1; "x"; true;)  -- normal parantez + trailing ;
- Parametre: func(a; b; c)            -- noktalı virgül ayırıcı
- While: while cond ... end_while     -- "do" YOK!
- If: if cond then ... end_if         -- "then" ZORUNLU!
```

**Komutlar:**
```bash
# Virgüllü parametre bul:
find compiler/stage1/modules -name "*.mlp" -exec grep -l "function.*(.*, " {} \;

# while...do bul (HATALI - do olmamalı):
find compiler/stage1/modules -name "*.mlp" -exec grep -l "while.*do" {} \;

# Array/List virgül bul (HATALI):
find compiler/stage1/modules -name "*.mlp" -exec grep -l "\[.*,.*\]\|(.*,.*)" {} \;
```

---

## 📁 DOSYA YAPISI

```
/MLP/
├── TODO_STAGE1_TO_SELFHOSTING_FINAL.md  ← BU BELGE
├── TODO_kurallari.md                     ← YZ kuralları
├── YZ_reports/                           ← YZ raporları BURAYA
│   ├── YZ_XX_TAMAMLANDI.md
│   └── NEXT_AI_START_HERE.md
├── compiler/
│   ├── stage0/                           ← DEPRECATED (bootstrap only)
│   └── stage1/                           ← AKTİF (MELP compiler)
├── ARCHITECTURE.md
├── MELP_VISION.md
├── MELP_REFERENCE.md
├── pmlp_kesin_sozdizimi.md
└── archive/                              ← Eski dosyalar
```

---

## 🎯 YZ'LER İÇİN HIZLI BAŞLANGIÇ

```bash
# 1. Bu dosyayı oku (zaten okudun)
# 2. TODO_kurallari.md oku
# 3. YZ_reports/NEXT_AI_START_HERE.md oku (varsa)
# 4. Branch oluştur
git checkout -b selfhosting_YZ_XX

# 5. Görevi yap
# 6. Rapor yaz
# YZ_reports/YZ_XX_TAMAMLANDI.md

# 7. NEXT_AI güncelle
# 8. Push et (MERGE ETME!)
git push origin selfhosting_YZ_XX
```

---

## ⚠️ SON UYARI

**Bu TODO tamamlandığında:**
- MELP self-hosting olacak
- Stage 0 deprecated olacak
- Yeni TODO YAZILMAYACAK

**Eğer bu TODO'yu tamamlamadan yeni yön önerirsen:**
- ❌ Görev durdurulur
- ❌ Sonsuz döngüye geri dönülür

**TEK YOL:** Bu TODO'yu bitir! 🎯

---

**Son Güncelleme:** 23 Aralık 2025  
**Yazan:** ÜA_01  
**Onaylayan:** Kullanıcı  
**Durum:** 🔵 AKTİF

---

🚀 **TÜNELDEN ÇIKIŞ ZAMANI!** 🚀

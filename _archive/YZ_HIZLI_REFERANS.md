# ⚡ MELP-GCC YZ HIZLI REFERANS

**Tarih:** 29 Aralık 2025  
**Amaç:** YZ'lerin 5 dakikada okuması için TEK KAYNAK  
**Oku:** Her TODO'ya başlamadan önce BU DOSYAYI oku!

---

## 🔒 KATI KURALLAR (OTOMATİK ENFORCEMENT)

### 🚨 EN BÜYÜK TEHLIKE: DÖKÜMANTASYON SABOTAJI!

**10+ deneme bu yüzden başarısız oldu:**
- YZ'ler TODO'yu değiştirdi: "Modüler" → "Monolitik"
- YZ'ler kuralları değiştirdi: "GCC" → "LLVM"
- YZ'ler mimariyi değiştirdi: 5 Principles'ı sildi

**SEN BUNU YAPAMAZSIN!**

### ❌ READ-ONLY DOSYALAR (ASLA DOKUNMA!)

```
KATI_KURALLAR_OZET.md         ← Kuralları değiştirme!
YZ_HIZLI_REFERANS.md          ← Bu dosya! Değiştirme!
YZ_WORKFLOW.md                ← Workflow değiştirme!
TODO_KURALLARI.md             ← Kuralları değiştirme!
Danışman_Yol_Haritası.md      ← Stratejik doküman!
docs/ARCHITECTURE_*.md        ← Mimari değiştirme!
pre-commit-check.sh           ← Script'i etkisizleştirme!
TODO_OLUSTUR/TODO_TEMPLATE/** ← Template bozma!
```

**pre-commit-check.sh bu dosyaları kontrol eder!**

### ✅ SENİN İZNİN VAR:

```
# 1. KOD - CURRENT_TODO.txt'de listelenen
MELP/C/stage0/modules/[modül]/[dosya].c
MELP/C/stage0/modules/[modül]/[dosya].h

# 2. PROGRESS - İlerleme bildirme (ZORUNLU!)
TODO_SELFHOSTING.md        → Yaptığın görevi [x] işaretle
NEXT_AI_START_HERE.md      → Sonraki YZ için güncelle

# 3. RAPOR - Sadece kendi raporun
YZ_RAPOR.md
```

**KURALLAR:**
- ✅ TODO'da SADECE kendi yaptığın görevi `[ ] → [x]` işaretle
- ❌ TODO'ya görev ekleme/değiştirme YASAK!
- ✅ NEXT_AI'yı her commit sonrası güncelle (sonraki YZ için)

### ⚠️ pre-commit-check.sh ZORUNLU!
Her commit öncesi çalıştırmalısın:
```bash
./pre-commit-check.sh
```

### Kontrol Edilen Şeyler:
0. 🚨 **Read-only dosya kontrolü** (SABOTAJ ÖNLEMİ!)
1. ✅ **Dosya Scope**: SADECE CURRENT_TODO.txt'deki dosyalar değişebilir
2. ✅ **Test Zorunlu**: make test veya ./run_tests.sh PASS olmalı
3. ✅ **Rapor Zorunlu**: YZ_RAPOR.md eksiksiz olmalı
4. ✅ **Commit Mesajı**: "feat: [açıklama]" formatında

### Red Sebepleri:
- 🚨 **Read-only dosya değiştirildi** → SABOTAJ! `git reset --hard HEAD`
- ❌ İzinsiz dosya değişikliği → `git reset --hard HEAD`
- ❌ Test fail → Düzelt, tekrar dene
- ❌ Rapor yok → YZ_RAPOR.md yaz
- ❌ 60 dakika aşımı → TODO çok büyük, böl

---

## 🎯 MELP NEDİR?

**MELP = Multi-Language Programming** (Çok dilli, çok sözdizimli dil)

**⚠️ PROJE DURUMU:** Bu proje %90 HAZIR! Sıfırdan başlamıyoruz!
- ✅ ~70 modül Stage0'da aktif (STAGE3_COMPLETE)
- ✅ 17 modül Stage1'de hazır
- ❌ Codegen ASM üretiyor → C üretmeli (TEK EKSİK!)

**Mimari Zincir:**
```
Kullanıcı Kodu → Normalizer → PMLP → Lexer → Parser → Codegen → GCC → Binary
  (tr/en/any)   (syntax.json)  (BU!)  (tokens)  (AST)   (C kodu)  (opt)
                                                         ↑ ŞU AN: x86-64 ASM
                                                         ↑ OLMALI: C kodu
```

**SEN SADECE PMLP İLE İLGİLENİRSİN!**
- PMLP = Pragmatic MLP (derleyicinin gördüğü dil)
- MLP = Kullanıcıların gördüğü dil (IGNORE ET!)

---

## 🏛️ 5 TEMEL ESAS (DEĞİŞTİRİLEMEZ)

```
1. MODULAR     → Her dosya max 500 satır, tek sorumluluk
2. GCC         → MLP → C → GCC → Binary (ASM üretme yasak)
3. STO         → Smart Type Optimization (numeric → int64/double/bigdecimal)
4. STATELESS   → Global state yasak, pure functions
5. STRUCT+FUNC → OOP yok, sadece struct + functions
```

**Formül:** `MELP = Modüler + GCC + STO + Stateless + (struct + functions)`

---

## ⛔ YASAKLAR

| Yasak | Neden |
|-------|-------|
| 500+ satır dosya | Bakım kabusu |
| Doğrudan ASM üret | GCC optimizasyonu kaybeder |
| Global mutable state | Debug zorlaşır, race condition |
| OOP/class | MELP felsefesine aykırı |
| TODO dışı değişiklik | Scope creep → hata |
| Mevcut testeri bozma | Regresyon yasak |

---

## ✅ ZORUNLULAR

| Zorunlu | Nasıl |
|---------|-------|
| TODO'yu BİREBİR yap | Ek özellik ekleme! |
| Her değişiklikte test | `make test` PASS olmalı |
| Git temiz tut | `git status` önce kontrol |
| Sadece izin verilen dosyaya dokun | Başka dosya = YASAK |
| Rapor yaz | 2 paragraf, ne yaptın? |
| Önceki YZ'nin işini koru | Ezme! |

---

## 📝 PMLP KRİTİK SÖZDİZİMİ

### 🔴 En Kritik Kurallar

**1. Parametre Ayırıcı: `;` (noktalı virgül)**
```pmlp
-- DOĞRU:
function add(a; b) as numeric
print("a"; "b"; "c")
array numeric[] nums = [1; 2; 3;]

-- YANLIŞ (virgül kullanma!):
function add(a, b) as numeric  ❌
```

**Neden?** MELP'te `345,95` ondalık sayıdır (Türk formatı).

---

**2. Blok Sonlandırıcı: Alt çizgili tek token**
```pmlp
-- DOĞRU:
if x > 5 then
    ...
end_if

while x < 10
    ...
end_while

function foo() as numeric
    ...
end_function

-- YANLIŞ:
end if    ❌  (iki token)
endif     ❌  (tek kelime)
```

**Tüm Sonlandırıcılar:**
- `end_if`, `end_while`, `end_for`, `end_function`
- `end_struct`, `end_enum`, `end_switch`, `end_try`

---

**3. If statement: `then` ZORUNLU**
```pmlp
-- DOĞRU:
if x > 5 then
    return 1
end_if

-- YANLIŞ:
if x > 5        ❌  (then yok)
    return 1
end_if
```

---

**4. Döngü çıkış: Standalone `exit` ve `continue`**
```pmlp
-- DOĞRU:
while true
    if x > 10 then
        exit          -- döngüden çık
    end_if
    
    if x == 5 then
        continue      -- bir sonraki iterasyon
    end_if
end_while

-- YANLIŞ (break/continue değil):
break     ❌
continue  ❌  (PMLP'de exit/continue)
```

---

**5. Trailing Semicolon: Array/List/Set'te ZORUNLU**
```pmlp
-- DOĞRU:
array numeric[] nums = [1; 2; 3;]
list string words = ("a"; "b"; "c";)
set numeric ids = <1; 2; 3;>

-- YANLIŞ:
array numeric[] nums = [1; 2; 3]  ❌  (son ; yok)
```

---

### 📋 Hızlı Syntax Tablosu

| Özellik | PMLP Syntax |
|---------|-------------|
| **Değişken** | `numeric x = 5` |
| **String** | `string name = "Ali"` |
| **Bool** | `bool flag = true` |
| **Array** | `array numeric[] nums = [1; 2; 3;]` |
| **List** | `list string words = ("a"; "b";)` |
| **Set** | `set numeric ids = <1; 2;>` |
| **If** | `if x > 5 then ... end_if` |
| **Else if** | `else_if x > 3 then ... end_if` |
| **While** | `while x < 10 ... end_while` |
| **For** | `for i = 0 to 9 ... end_for` |
| **Function** | `function foo(a; b) as numeric ... end_function` |
| **Return** | `return x` |
| **Print** | `print("text")` |
| **Exit loop** | `exit` (standalone) |
| **Continue** | `continue` (standalone) |
| **Yorum** | `-- tek satır` veya `/* çok satır */` |

---

### 🔢 Operatörler

**Mantıksal:**
- `and`, `or`, `not` (&&, ||, ! değil!)

**Karşılaştırma:**
- `==`, `!=`, `<`, `>`, `<=`, `>=`

**Aritmetik:**
- `+`, `-`, `*`, `/`, `%` (modulo), `^` (üs)

**Atama:**
- `=` (atama), `+=`, `-=`, `*=`, `/=`

---

## 🧪 TEST ZORUNLULUĞU

**Her değişiklikten sonra:**

```bash
# 1. Derleme
cd MELP/C/stage0
make clean && make

# 2. Test
make test
# VEYA
./run_tests.sh

# 3. Sonuç PASS olmalı
# ❌ FAIL varsa commit YAPMA!
```

**Test yazma (yeni özellik için):**
```bash
# tests/test_yeni_ozellik.mlp oluştur
# Beklenen çıktıyı belirle
# Test ekle
```

---

## 📁 DOSYA YAPISI

```
MLP-GCC/
├── YZ_HIZLI_REFERANS.md          ← BU DOSYA
├── TODO_SELFHOSTING.md            ← Ana TODO
├── Danışman_Yol_Haritası.md       ← 5 Temel Esas (değiştirilemez)
│
├── MELP/
│   ├── C/stage0/modules/          ← Stage0-C modülleri (69 modül)
│   │   ├── lexer/
│   │   ├── parser/
│   │   ├── codegen/               ← x86-64 ASM → C'ye çevirilecek
│   │   └── ...
│   │
│   ├── MLP/stage1/modules/        ← Stage1-MLP modülleri (17 modül)
│   │   ├── lexer.mlp
│   │   ├── parser.mlp
│   │   └── ...
│   │
│   ├── runtime/
│   │   ├── runtime.c              ← STO runtime
│   │   └── runtime.h
│   │
│   └── bootstrap/
│       ├── lexer.c
│       ├── parser.c
│       └── codegen.c
│
├── tests/                         ← Test dosyaları
└── docs/                          ← Detaylı belgeler (opsiyonel)
```

---

## 🎯 SENİN GÖREVİN

**TODO'dan alacaksın:**
```markdown
🎯 GÖREV: emit_c() fonksiyonu yaz

📁 DOSYA: codegen_emit.c (50-120. satır)

✅ YAPILACAKLAR:
- [ ] void emit_c(const char* format, ...) fonksiyonu yaz
- [ ] printf-style formatting ile C kodu üret
- [ ] Test: make test_emit

⏱️ SÜRE: 30 dakika

❌ BAŞKA DOSYAYA DOKUNMA!
```

**Yapacakların:**
1. ✅ `git status` kontrol (temiz mi?)
2. ✅ Sadece `codegen_emit.c` dosyasını aç
3. ✅ 50-120. satırlar arasına kod yaz
4. ✅ `make test` → PASS
5. ✅ `git add codegen_emit.c`
6. ✅ `git commit -m "feat: emit_c() fonksiyonu eklendi"`
7. ✅ YZ_RAPOR.md yaz (2 paragraf)

---

## 🚨 HATA YAPMA!

### ❌ Yapma:

1. **Scope dışına çıkma**
   ```
   Görev: codegen_emit.c düzelt
   Sen: parser.c'yi de düzelttim ❌
   ```

2. **Ek özellik ekleme**
   ```
   Görev: exit token ekle
   Sen: return değeri de ekledim ❌
   ```

3. **MLP syntax kullanma**
   ```
   Belge: "if x > 5:" diyor
   Sen: PMLP kullan → "if x > 5 then" ✅
   ```

4. **Test kırmak**
   ```
   Değişiklik yaptın
   make test → 3 test FAIL ❌
   Sen: "Düzeltilir" diye commit yapma!
   ```

5. **Önceki işi ezme**
   ```
   YZ_03: codegen_emit.c'yi düzeltti
   Sen: aynı dosyayı değiştirip YZ_03'ün işini ezdin ❌
   ```

---

## ✅ İYİ YZ Checklist

Commit öncesi kontrol:

```
[ ] TODO'yu tam anladım
[ ] Sadece izin verilen dosyayı değiştirdim
[ ] make test → PASS
[ ] Yeni test ekledim (yeni özellik ise)
[ ] Önceki YZ'nin işini kordum
[ ] Rapor yazdım (2 paragraf)
[ ] Git commit mesajı açıklayıcı
[ ] TODO dışı iş yapmadım
```

**Hepsi ✅ → Commit yap**  
**Biri ❌ → Düzelt, sonra commit**

---

## 📝 RAPOR TEMPLATE

```markdown
# YZ_XX_TAMAMLANDI

**Tarih:** 29 Aralık 2025  
**Görev:** emit_c() fonksiyonu yazmak

## ✅ YAPILAN
emit_c() fonksiyonu codegen_emit.c'ye eklendi (70 satır).
printf-style formatting ile C kodu üretimi sağlandı.

## 🧪 TEST
make test → PASS (5/5 test başarılı)

## 📁 DEĞİŞEN DOSYALAR
- codegen_emit.c (+70 satır)

## 📊 DURUM
✅ TODO tamamlandı, test pass, commit yapıldı.
```

---

## 🔗 EK KAYNAKLAR (Opsiyonel)

**Sadece gerekirse oku:**
- `TODO_SELFHOSTING.md` - Ana TODO detayları
- `Danışman_Yol_Haritası.md` - 5 Temel Esas detayları
- `temp/pmlp_kesin_sozdizimi.md` - Tam PMLP referansı (1000+ satır)
- `temp/kurallar_kitabı_v1.md` - Tüm detaylar (1800+ satır)

**Ama önce BU DOSYAYI bitir!**

---

## 🎯 SON HATIRLATMA

```
1. Bu belgeyi OKU (5 dakika)
2. TODO'yu OKU (2 dakika)
3. Sadece izin verilen dosyayı DÜZELT (30-60 dakika)
4. TEST et (make test → PASS)
5. COMMIT yap
6. RAPOR yaz (2 paragraf)
7. BİTTİ ✅
```

**BAŞKA DOSYAYA DOKUNMA!**  
**MLP SYNTAX KULLANMA!**  
**TESTLERİ KIRMA!**  
**ÖNCEKİ İŞİ EZME!**

---

**İyi çalışmalar! 🚀**

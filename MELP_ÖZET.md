# MELP_ÖZET - Tüm YZ'ler İçin Tek Kaynak

> ⚠️ **UYARI:** Bu belgenin içeriği değiştirilemez. Sadece okuma amaçlı kullanılır. Değişiklik için Proje Danışmanı'na danışın.

**Son Güncelleme:** 31 Aralık 2025  
**Versiyon:** 1.0  
**Amaç:** YZ'lerin (Görevli, ÜA, MM, PD) okuması gereken TEK özet belge  
**Hedef:** Context'i doldurmadan kritik bilgilere erişim (max 500 satır)

---

## 📖 HIZLI ERİŞİM (İNDEX)

- [1. MELP Nedir?](#1-melp-nedir) - Proje tanımı (30 satır)
- [2. 6 Temel Esas](#2-6-temel-esas) - Mimari prensipler (40 satır)
- [3. TODO Sistemi](#3-todo-sistemi) - Mevcut TODO'lar (50 satır)
- [4. Syntax Özeti](#4-syntax-özeti) - PMPL temel kurallar (80 satır)
- [5. YZ Workflow](#5-yz-workflow) - Git + Rapor kuralları (60 satır)
- [6. Sık Kullanılan Referanslar](#6-sık-kullanılan-referanslar) - Komutlar ve yollar (40 satır)
- [7. Detaylı Belgeler](#7-detaylı-belgeler) - Daha fazla bilgi için (20 satır)

**Not:** Rolüne göre sadece ihtiyacın olan bölümü oku!

---

## 1. MELP NEDİR?

### Tanım

**MELP** (Multi-Language Programming / Melting Pot) = **Self-hosting derleyici projesi**

**Temel Özellikler:**
- 🌍 **Multi-language:** Türkçe, İngilizce, Hintçe, vb. (diller.json ile özelleştirilebilir)
- 🔤 **Multi-syntax:** Python-style, C-style, custom syntax (syntax.json)
- 🎯 **STO (Smart Type Optimization):** Kullanıcı 2 tip görür (`numeric`, `string`), compiler optimize eder
- 🧊 **Stateless by default:** Global state yasak, fonksiyonlar pure
- 🏗️ **Modular:** Her dosya max 500 satır, module = template (state tutmaz)

### Proje Hedefi

```
Stage 0 (Bootstrap):  C ile yazılmış MLP derleyicisi      [✅ TAMAMLANDI]
Stage 1 (Self-hosting): MLP ile yazılmış MLP derleyicisi  [✅ TAMAMLANDI]
```

**Şu An:** Self-hosting tamamlandı! Stage 1 derleyici çalışıyor. Şimdi dil özelliklerini genişletme aşamasındayız (TODO #3: LANGUAGE_FEATURES).

### Derleme Zinciri

```
Kullanıcı Kodu (Türkçe/Python-style)
    ↓
[Normalizer: diller.json + syntax.json]
    ↓
Pragmatic MLP (İngilizce base) ← Derleyici BURADA çalışır
    ↓
[Lexer → Parser → STO Analiz → Codegen]
    ↓
C Kodu (Stage 0) veya Binary (Stage 1+)
    ↓
GCC → Native Binary
```

**Kritik:** YZ'ler sadece **Pragmatic MLP (PMPL)** ile çalışır, kullanıcı katmanını görmez!

---

## 2. 6 TEMEL ESAS

**MLP-GCC'nin değişmez mimari prensipleri:**

```
1. MODULAR       → Her dosya max 500 satır
                   → Dosyalar bağımsız modüller
                   → Monolitik dosya yasak

2. GCC           → MLP → C → GCC → Binary
                   → Şimdilik GCC backend (LLVM gelecekte)
                   → C hedef dil olarak kullanılıyor

3. STO           → Smart Type Optimization
                   → Kullanıcı: numeric, string
                   → Compiler: int64/double/BigDecimal/SSO/heap
                   → Otomatik optimizasyon, kullanıcı bilmez

4. STATELESS     → Global state yasak
                   → Fonksiyonlar pure (side-effect yok)
                   → State gerekirse: `state` keyword (opt-in)
                   → Predictable, testable kod

5. STRUCT+FUNC   → OOP yok, classes yok
                   → Sadece: struct + functions
                   → Functional programming pattern
                   → Go/Rust benzeri yaklaşım

6. MODÜL=ŞABLON  → Her modül çağrısı independent instantiate
                   → Modül state tutmaz (pure function collection)
                   → Her çağrı: dirilt → çalıştır → öldür
                   → Persistent API yok
```

**⚠️ Önemli:** Bu 6 esasa aykırı her tasarım **REDDEDİLİR**!

---

## 3. TODO SİSTEMİ

### TODO Listesi ve Durumu

| TODO | Süre | Durum | Açıklama |
|------|------|-------|----------|
| **0-TODO_SELFHOSTING** | 6 hafta | ✅ TAMAMLANDI | Self-hosting derleyici çalışıyor |
| **1-TODO_TOOLING_BASICS** | 2-3 hafta | ⏳ | Error messages, GDB, LSP, formatter |
| **2-TODO_STDLIB_EXPAND** | 3-4 hafta | ⏳ | Network, threading, JSON, collections |
| **3-TODO_LANGUAGE_FEATURES** | 2-3 hafta | 🔥 AKTİF | Generics, pattern matching, closures |
| **4-TODO_IDE_INTEGRATION** | 2 hafta | ⏳ | Full LSP, VS Code extension, Vim plugin |
| **5-TODO_PACKAGE_MANAGER** | 3 hafta | ⏳ | mlp-pkg, registry, package manager |
| **6-TODO_OPTIMIZATION** | 2 hafta | ⏳ | DCE, constant folding, inlining |
| **7-TODO_ECOSYSTEM** | 4+ hafta | ⏳ | GUI, DB, web framework, test framework |

**Toplam:** ~24-28 hafta (6-7 ay) → FULL PROFESSIONAL LANGUAGE!

### TODO Hiyerarşisi

```
TODO_LANGUAGE_FEATURES/
├── 3-TODO_LANGUAGE_FEATURES.md      # Ana TODO belgesi (kullanıcı yazar)
├── LANGUAGE_FEATURES_PD/            # Proje Danışmanı raporları
│   ├── Danışman_buradan_basla.md
│   └── PD_XX_Raporu.md
├── LANGUAGE_FEATURES_MM/            # Mastermind raporları
│   ├── Mastermind_buradan_basla.md
│   └── MM_XX_Raporu.md
├── LANGUAGE_FEATURES_UA/            # Üst Akıl raporları
│   ├── Ust_Akil_buradan_basla.md
│   └── UA_XX_Raporu.md
└── LANGUAGE_FEATURES_YZ/            # Görevli YZ raporları
    ├── Görevli_YZ_Buradan_Başla.md
    ├── YZ_01_RAPOR.md
    └── YZ_02_RAPOR.md
```

### Roller Arası İlişki

```
👤 KULLANICI (İnsan)
    ↓
🎓 DANIŞMAN (PD) - Stratejik yön, TODO önceliklendirme
    ↓
🧠 MASTERMIND (MM) - TODO mimari, koordinasyon
    ↓
🧩 ÜST AKIL (UA) - TODO'yu phase'lere böler, YZ yönetir
    ↓
🤖 GÖREVLİ YZ - Kod yazar, test eder, commit eder
```

**Not:** Sadece KULLANICI insan, diğerleri YZ!

---

## 4. SYNTAX ÖZETİ

### PMPL (Pragmatic MLP) Temel Kurallar

**1. Parametre Ayırıcı: `;` (noktalı virgül)**
```pmpl
func(345,95; true; "OK")    -- 345,95 = ondalık sayı (Türk formatı)
```

**2. Blok Sonlandırıcı: Alt çizgili (`end_if`, `end_while`)**
```pmpl
if x > 5 then
    yazdir("Büyük")
end_if    -- TEK TOKEN, boşluksuz!
```

**3. Tipler**
```pmpl
numeric x = 42          -- int64/double/BigDecimal (compiler seçer)
string s = "Test"       -- SSO (<23 byte) veya heap
boolean b = true        -- true/false
```

**4. Koleksiyonlar**
```pmpl
[1, 2, 3]      -- Array (fixed size, fast)
(1; 2; 3;)     -- List (dynamic, mutable)
<1, 2, 3>      -- Tuple (immutable, stack)
```

**5. Kontrol Akışı**
```pmpl
if condition then
    ...
else_if other then      -- ALT ÇİZGİLİ!
    ...
else
    ...
end_if

while condition
    ...
end_while

for i = 1 to 10
    ...
end_for
```

**6. Fonksiyonlar**
```pmpl
function topla(numeric a; numeric b) as numeric
    return a + b
end_function
```

**7. Yorumlar**
```pmpl
-- Tek satır yorum

--- Çok satırlı
    yorum
    bloğu ---
```

### Kritik Token'lar

| Keyword | Token | Not |
|---------|-------|-----|
| `end_if` | TOKEN_END_IF | Tek token, boşluksuz |
| `end_while` | TOKEN_END_WHILE | Tek token |
| `end_function` | TOKEN_END_FUNCTION | Tek token |
| `else_if` | TOKEN_ELSE_IF | Alt çizgili |
| `exit` | TOKEN_EXIT | Döngüden çık |
| `continue` | TOKEN_CONTINUE | Döngü devam |

**⚠️ Dikkat:** `end if` (iki token) ≠ `end_if` (tek token)

---

## 5. YZ WORKFLOW

### Git Workflow (ZORUNLU!)

**Her YZ oturumu için:**

```bash
# 1. Numaralı branch oluştur
git checkout -b feature-name_YZ_XX
# Örnek: git checkout -b generics-impl_YZ_03

# 2. Çalış ve commit et
git add .
git commit -m "YZ_XX: Açıklama"

# 3. Push et
git push origin feature-name_YZ_XX

# 4. ⚠️ MERGE YAPMA! İnsan merge edecek
```

### YZ Numaralandırma

```
TODO içinde YZ_01, YZ_02, YZ_03, ... şeklinde numaralandır
YZ_01 = İlk görevli
YZ_52 = 52. görevli (örnek)
```

### Rapor Formatı

**Dosya:** `TODO_XXX/XXX_YZ/YZ_XX_RAPOR.md`

```markdown
# YZ_XX Raporu

**Tarih:** [Tarih]
**TODO:** [TODO adı]
**Task:** [Görev açıklaması]
**Süre:** [Saat]

## ✅ TAMAMLANAN
- İş 1
- İş 2

## 🧪 TESTLER
- Test 1: ✅ Başarılı
- Test 2: ✅ Başarılı

## 📦 DEĞİŞEN DOSYALAR
- dosya1.c (+50, -10)
- dosya2.h (+20, -5)

## 📊 GİT
- Branch: feature-name_YZ_XX
- Commit: abc1234
- Push: ✅ Yapıldı

## 🔄 SONRAKİ YZ İÇİN
[Sonraki YZ'ye notlar]
```

### Context Limitleri

**Token budget:** ~200K (her YZ için)

**⚠️ Context dolarsa:**
- Mevcut işi bitir
- Rapor yaz
- Commit + push
- Sonraki YZ devam eder

**Context tasarrufu:**
- Sadece ihtiyacın olan belgeleri oku
- Dosyaları kısmi oku (startLine/endLine)
- Büyük dosyalarda grep_search kullan

---

## 6. SIK KULLANILAN REFERANSLAR

### Proje Dizin Yapısı

```
/home/pardus/projeler/MLP/MELP-GCC-WORKSHOP/
├── MELP_ÖZET.md                  ← BU BELGE
├── TODO_KURALLARI.md             ← Rol kuralları (bölüm bazlı oku)
├── ROADMAP.md                    ← TODO genel planı
├── 3-TODO_LANGUAGE_FEATURES.md   ← Mevcut TODO
├── belgeler/                     ← Detaylı referanslar
│   ├── MELP_REFERENCE.md         ← Kapsamlı referans
│   ├── ARCHITECTURE.md           ← Mimari detaylar
│   ├── STO_PRINCIPLES.md         ← STO açıklaması
│   ├── pmlp_sozdizimi.md         ← Tam syntax (1100+ satır)
│   └── MLP MODÜL FELSEFESİ.md    ← Modül pattern
├── docs/
│   ├── ARCHITECTURE_AND_MEMORY_STRATEGY.md
│   └── LANGUAGE_EVOLUTION_GUIDE.md
├── MELP/
│   ├── compiler/stage0/          ← Bootstrap compiler (C)
│   └── runtime/                  ← Runtime library (C)
└── TODO_LANGUAGE_FEATURES/       ← Mevcut TODO workspace
    ├── LANGUAGE_FEATURES_PD/
    ├── LANGUAGE_FEATURES_MM/
    ├── LANGUAGE_FEATURES_UA/
    └── LANGUAGE_FEATURES_YZ/
```

### Build Komutları

```bash
# Compiler build (Stage 0)
cd MELP/compiler/stage0
make clean && make

# Test çalıştır
cd tests/
./run_tests.sh

# Tek test
./melpc test.mlp -o test
./test
```

### Sık Kullanılan Dosyalar

| Dosya | Amaç | Ne Zaman Oku? |
|-------|------|---------------|
| `MELP_ÖZET.md` | Bu belge | Her YZ oturumu başında |
| `TODO_KURALLARI.md` | Rol kuralları | Rolüne ait bölümü oku |
| `3-TODO_LANGUAGE_FEATURES.md` | TODO detayları | TODO başında |
| `belgeler/pmlp_sozdizimi.md` | Tam syntax | Syntax sorunu varsa |
| `belgeler/ARCHITECTURE.md` | Mimari detay | Mimari karar alırken |
| `belgeler/STO_PRINCIPLES.md` | STO detay | Tip sistemi değişirse |

### Test Stratejisi

```bash
# Quick test (her commit sonrası)
make test-quick

# Full test (gün sonu)
make test-all

# Tek modül test
make test-module MODULE=generics
```

---

## 7. DETAYLI BELGELER

**Context dolmadan önce sadece ihtiyacın olan belgeyi oku!**

### Belgeler Dizini

| Belge | Satır | Ne Zaman Oku? | Özet |
|-------|-------|---------------|------|
| `belgeler/MELP_REFERENCE.md` | 970 | Genel referans gerekirse | Kapsamlı AI referans |
| `belgeler/ARCHITECTURE.md` | 1896 | Mimari karar alırken | Modüler mimari kuralları |
| `belgeler/STO_PRINCIPLES.md` | 201 | Tip sistemi çalışması | STO detaylı açıklama |
| `belgeler/pmlp_sozdizimi.md` | 1138 | Syntax sorunu | Tam PMPL syntax |
| `belgeler/MLP MODÜL FELSEFESİ.md` | 347 | Modül tasarlarken | Modül = Template prensibi |
| `docs/ARCHITECTURE_AND_MEMORY_STRATEGY.md` | 779 | Bellek yönetimi | RAII, scope-based cleanup |
| `docs/LANGUAGE_EVOLUTION_GUIDE.md` | 748 | Dil değişikliği | Backward compatibility |

### Syntax Özeti Nerede?

**Syntax sorunu:**
1. Bu belgede Bölüm 4'e bak (temel kurallar)
2. Yetmezse: `belgeler/pmlp_sozdizimi.md` (tam referans)
3. İhtiyaç: Sadece ilgili bölümü oku (index var)

**Mimari sorunu:**
1. Bu belgede Bölüm 2'ye bak (6 Temel Esas)
2. Yetmezse: `belgeler/ARCHITECTURE.md`
3. Modül tasarımı: `belgeler/MLP MODÜL FELSEFESİ.md`

**STO sorunu:**
1. Bu belgede Bölüm 2 (STO esası)
2. Yetmezse: `belgeler/STO_PRINCIPLES.md`

---

## 🎯 YZ İÇİN HIZLI BAŞLANGIÇ

### Görevli YZ (İmplementasyon)

```bash
# 1. Bu belgeyi oku (MELP_ÖZET.md) ✅
# 2. TODO_KURALLARI.md → Bölüm 1 (Görevli YZ) oku
# 3. Mevcut TODO'yu oku (örn: 3-TODO_LANGUAGE_FEATURES.md)
# 4. ÜA'nın verdiği görevi al
# 5. Branch oluştur, çalış, commit, push
# 6. Rapor yaz
```

### Üst Akıl (Task Yönetimi)

```bash
# 1. Bu belgeyi oku (MELP_ÖZET.md) ✅
# 2. TODO_KURALLARI.md → Bölüm 2 (Üst Akıl) oku
# 3. Mevcut TODO'yu oku
# 4. TODO'yu phase'lere böl
# 5. Her phase için YZ ata
# 6. YZ'leri takip et
```

### Mastermind (Strateji)

```bash
# 1. Bu belgeyi oku (MELP_ÖZET.md) ✅
# 2. TODO_KURALLARI.md → Bölüm 3 (Mastermind) oku
# 3. TODO'ları koordine et
# 4. Mimari kararlar al
# 5. ÜA'ları yönet
# 6. Danışman'a escalate et (gerekirse)
```

### Proje Danışmanı (Vizyon)

```bash
# 1. Bu belgeyi oku (MELP_ÖZET.md) ✅
# 2. TODO_KURALLARI.md → Bölüm 4 (Danışman) oku
# 3. ROADMAP.md oku (TODO öncelikleri)
# 4. Stratejik kararlar al
# 5. Mastermind'ları koordine et
# 6. 6 Temel Esası koru
```

---

## ⚠️ YASAKLAR (TÜM YZ'LER İÇİN)

### ❌ Yapamazsın

1. **TODO dışı değişiklik** → Sadece verilen görevi yap
2. **6 Temel Esası ihlal** → Her değişiklik esaslara uygun olmalı
3. **Merge yapma** → Sadece commit + push, insan merge eder
4. **Monolitik dosya** → Max 500 satır, modüler kal
5. **Global state** → Stateless kal, `state` keyword gerekmedikçe kullanma
6. **OOP pattern** → Class yok, sadece struct + functions
7. **Gereksiz dokümantasyon okuması** → Context'ini dikkatli kullan

### ✅ Yapabilirsin

1. **Branch oluştur** → Her YZ kendi branch'inde çalışır
2. **Commit yap** → Her anlamlı değişiklik = commit
3. **Test yaz** → Her özellik için test gerekli
4. **Rapor yaz** → Her oturum sonunda rapor zorunlu
5. **Soru sor** → Üst role escalate et (YZ→UA, UA→MM, MM→PD)
6. **Dokümantasyon güncelle** → TODO tamamlanınca belgeler güncellenir

---

## 📌 SON NOTLAR

### Hangi Belgeyi Ne Zaman Okuyacaksın?

```
HER YZ OTURUMU BAŞINDA:
✅ MELP_ÖZET.md (bu belge) → 5 dakika

İLK DEFA BU ROLÜ ALIYORSAN:
✅ TODO_KURALLARI.md (rolüne ait bölüm) → 10 dakika

GÖREVE BAŞLAMADAN ÖNCE:
✅ 3-TODO_LANGUAGE_FEATURES.md (mevcut TODO) → 15 dakika

SORUN ÇIKARSA (ihtiyaç varsa):
⏸️ belgeler/ dizinindeki ilgili belge → Sadece ilgili bölüm
```

### İletişim Kanalları

```
Görevli YZ → Üst Akil (UA)     : Teknik sorun, görev belirsizliği
Üst Akil → Mastermind (MM)      : Phase koordinasyonu, kaynak talebi
Mastermind → Danışman (PD)      : Mimari karar, TODO önceliği
Danışman → Kullanıcı            : Stratejik yön, büyük kararlar
```

### Başarı Kriterleri

**YZ başarısı:**
- ✅ Görevi tamamladı
- ✅ Testler geçti
- ✅ 6 Temel Esas'a uydu
- ✅ Rapor yazdı
- ✅ Commit + push yaptı

**TODO başarısı:**
- ✅ Tüm task'lar tamamlandı
- ✅ Test coverage %90+
- ✅ Mimari bütünlük korundu
- ✅ Dokümantasyon güncellendi

---

**Son Güncelleme:** 31 Aralık 2025  
**Hazırlayan:** Proje Danışmanı PD_01  
**Durum:** ✅ Aktif - Tüm YZ'ler bu belgeyi okuyacak

**Bu belge değiştiğinde:** Tüm YZ'lere bildirim yapılır, yeni versiyon okunur.

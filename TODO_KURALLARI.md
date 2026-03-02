# TODO KURALLARI - Yapay Zeka Görevliler İçin

> ⚠️ **UYARI:** Bu belgenin içeriği değiştirilemez. Sadece okuma amaçlı kullanılır.

---

**Tarih:** 10 Ocak 2026  
**Versiyon:** 5.0  
**Amaç:** Yapay Zeka görevlilerinin uyması gereken temel kurallar  
**Not:** Bu projede sadece bir insan vardır: **Kullanıcı (Yönetici)** - Diğer tüm roller YZ'ler tarafından üstlenilir

---

## 🆔 SEN KİMSİN?

**Kullanıcı sana ilk olarak hangi dosyayı gönderdiyse, ilgili klasördeki XX_buradan_basla.md dosyasını oku:**

| Dosya Konumu | Rolün | Detaylar |
|--------------|-------|----------|
| `TODO_XXX/YZ_oku.md` | **Görevli YZ** | İmplementasyon işleri |
| `TODO_XXX/XXX_UA/Ust_Akil_buradan_basla.md` | **Üst Akıl (ÜA)** | Görevli yönetimi |
| `TODO_XXX/XXX_MM/Mastermind_buradan_basla.md` | **Mastermind (MM)** | Strateji ve koordinasyon |
| `TODO_XXX/XXX_PD/Danişman_buradan_basla.md` | **Proje Danışmanı (PD)** | Mimari ve vizyon |

**⚠️ ÖNEMLİ:** Her rolün detaylı kuralları, görev tanımları ve sorumlulukları ilgili XX_buradan_basla.md dosyasında bulunur!

---

## 🏁 PROJE ORGANIZASYON YAPISI

```
Kullanıcı (Yönetici)
  └─> Proje Danışmanı (PD) [YZ]
       └─> Mastermind (MM) [YZ]
            └─> Üst Akıl (ÜA) [YZ]
                 └─> Görevli YZ [YZ]
```

---

## 📂 RAPOR KLASÖR YAPISI

```
TODO_C_BAGIMSIZLIK/
├── TODO_C_BAGIMSIZLIK.md           # Ana TODO belgesi
├── CBIND_YZ/                       # Görevli YZ
│   ├── Görevli_YZ_Buradan_Başla.md
│   ├── YZ_01_RAPOR.md
│   └── YZ_RAPOR_TEMPLATE.md
├── CBIND_UA/                       # Üst Akıl
│   └── Ust_Akil_buradan_basla.md
├── CBIND_MM/                       # Mastermind
│   └── Mastermind_buradan_basla.md
└── CBIND_PD/                       # Proje Danışmanı
    └── Danişman_buradan_basla.md
```

---

## 🔧 TEMEL ÇALIŞMA İLKELERİ

### ✅ Genel Yetkiler (Tüm Roller)
- Algoritma seçimi ve optimizasyon
- Değişken/fonksiyon isimlendirme
- Kod organizasyonu
- Yorum ve dokümantasyon ekleme
- Test case yazma

### ❌ Üst Seviyeye Danışılması Gerekenler
- Global değişken tanımlamak
- İzin listesi dışı dosya değiştirmek
- Mimari prensiplere aykırı değişiklik
- AUTONOMOUS prensibine aykırı kararlar

---

## 🧪 TEST KURALLARI

### ⚠️ KRİTİK: %100 GERÇEK TEST ZORUNLULUĞU

**YASAK:**
- ❌ Stub (sahte fonksiyonlar)
- ❌ Mock (test double'ları)
- ❌ Hack çözümler
- ❌ **Workaround (HER TÜRLÜSÜ!)**
- ❌ Placeholder implementasyonlar
- ❌ TODO yorumları ile yarım kod
- ❌ "Test için özel" koşullar

**🚨 KRİTİK: WORKAROUND KESİN YASAK!**

```
❌ Workaround kullanıldı → Proje çöp oldu!
```

**🔴 METAFOR: Yanlış İliklenen İlk Düğme**

```
Workaround = Gömleğin ilk düğmesini yanlış ilikladın
         ↓
Devam ediyorsun... (her şey normal görünüyor)
         ↓
Son düğmeye geldin → UYUMSUZLUK!
         ↓
🚨 BAŞA DÖN, HEPSİNİ ÇÖZ!
```

**İlk hata küçük görünür, ama sonunda her şeyi yıkar!**

**Ne yapmalısın?**
- ✅ Sorunla karşılaştın → **ÜA'ya sor**
- ✅ Aşamıyorsun → **ÜA'ya sor**
- ✅ Alternatif düşünüyorsun → **ÜA'ya sor**
- ✅ "Belki şöyle yaparsam" → **ÜA'ya sor**

**Workaround nedir?**
- Çalışmayan kodu "çalışıyor gibi" göstermek
- Alternatif/basit yol kullanmak (print_numeric yerine print_string)
- "Geçici çözüm" diye bir şey yapmak
- "Şimdilik böyle, sonra düzeltiriz" demek

**Tek kural:** Takıldın mı? → **ÜST AKIL'A SOR!**

**ZORUNLU:**
- ✅ %100 gerçek verilerle test
- ✅ Gerçek fonksiyonlar, gerçek sonuçlar
- ✅ Production-ready kod
- ✅ Tam implementasyon
- ✅ Her adımda çalışan testler

**Örnek:**
```bash
# ❌ YANLIŞ:
# "Test geçti" (ama stub kullanıldı)

# ✅ DOĞRU:
./run_tests.sh
# Gerçek parser, gerçek lexer, gerçek codegen
# Tüm testler %100 gerçek verilerle çalışıyor
```

---

## 📖 PMLP SÖZ DİZİMİ KURALLARI

**⚠️ Çok Karıştırılan Kurallar - Problem ile karşılaştıysan önce buraya bak!**

### 🔑 Temel Söz Dizimi İlkeleri

#### 1. Parametre Ayırıcı: `;` (noktalı virgül)
**Neden?** MLP'te `3,14` ondalık sayıdır (Türk formatı: virgül = ondalık ayırıcı)

```pmpl
-- Fonksiyon çağrısı
result = func(42; true; "OK")

-- Array tanımlama
numeric[] arr = [1; 2; 3] 

-- List tanımlama
list data = (1; 2; 3)

-- Tuple tanımlama
list data = <1; 2; 3>
```

#### 2. Blok Sonlandırıcılar: Tek Token, Alt Çizgili
```pmpl
-- ✅ DOĞRU:
end_if
end_while
end_for
end_function

-- ❌ YANLIŞ:
end if      -- İki token, YASAK!
end while
```

#### 3. If Kuralı: Mutlaka `then`
```pmpl
-- ✅ DOĞRU:
if x > 5 then
    return 1
end_if

-- ❌ YANLIŞ:
if x > 5        -- 'then' eksik!
    return 1
end_if
```

#### 4. else_if: Tek Token
```pmpl
if x > 100 then
    print("large")
else_if x > 50 then     -- else_if tek token!
    print("medium")
else
    print("small")
end_if
```

#### 5. Değişken Tanımlama ve Noktalı Virgül Kuralı
**KRİTİK:** İlk tanımlamada `;` ZORUNLU, sonraki atamalarda `;` YASAK!

```pmpl
-- ✅ DOĞRU: İlk tanımlama (; ZORUNLU)
numeric x = 3,14;            -- 3.14 (virgül = ondalık)
numeric y = 1.000.000;       -- 1,000,000 (nokta = binlik)
string name = "Ali";

-- ✅ DOĞRU: Sonraki atamalar (; YASAK)
x = 5,28
y = 2.000
name = "Veli"

-- ❌ YANLIŞ:
numeric a = 4      -- ; eksik!
a = 3;             -- ; olmamalı!
```

#### 6. Döngü Kuralları
**KRİTİK:** `while ... do` YASAK! Sadece `while ... end_while` kullan!

```pmpl
-- ✅ DOĞRU:
while x < 100
    x = x + 1
end_while

-- ❌ YANLIŞ:
while x < 100 do    -- 'do' YASAK!
    x = x + 1
end_while
```

**Döngü Çıkış/Devam:**
- ✅ `exit` var (break YOK!)
- ⚠️ `continue` opsiyonel (kullanılırsa hata vermez ama varsayılan olarak yok)

```pmpl
-- ✅ DOĞRU:
while true
    if condition then
        exit            -- break değil, exit!
    end_if
end_while

-- ❌ YANLIŞ:
while true
    if condition then
        break           -- break YOK!
    end_if
end_while
```

#### 7. `main()` Fonksiyonu: Dönüş Tipi YOK, `return 0` YOK
**KRİTİK:** `main()` MLP'nin giriş noktasıdır. C/C++ alışkanlığıyla `as numeric` veya `return 0` yazmak YASAK!

```mlp
-- ✅ DOĞRU:
function main()
    print("Merhaba, MELP!")
end_function

-- ❌ YANLIŞ (C alışkanlığı, MLP'de geçersiz kalıp):
function main() as numeric   -- dönüş tipi YASAK!
    print("Merhaba!")
    return 0                 -- return 0 YASAK!
end_function

-- ❌ YANLIŞ:
function main() -> numeric   -- ok sözdizimi de YASAK main için!
    return 0
end_function
```

**Neden?** MLP, `main()` fonksiyonunu özel giriş noktası olarak tanır. Exit kodu C kavramıdır; MLP programları sadece çıkışa ulaşarak tamamlanır.

### 📚 Detaylı Bilgi

**Daha fazla söz dizimi kuralı için:**
- [`belgeler/MELP_PHILOSOPHY.md`](belgeler/MELP_PHILOSOPHY.md) - Felsefe ve temel prensipler
- [`belgeler/pmlp_sozdizimi.md`](belgeler/pmlp_sozdizimi.md) - Tam söz dizimi referansı
- [`belgeler/language/feature_cards/`](belgeler/language/feature_cards/) - Her özelliğin söz dizimi kartı

---

### 🃏 FEATURE CARD ZORUNLULUĞU

> ⚠️ **Her TODO görevi başlamadan önce uygulanır.**

**Kural:** Üzerinde çalıştığın özellikle ilgili sözdizimi kartını `belgeler/language/feature_cards/` içinde bul ve **o karttaki syntax'ı kullan**.

**Adımlar:**
1. TODO başlığına bak (örn. `Week 8.4: Type Aliases`)
2. `belgeler/language/feature_cards/` içinde ilgili kartı ara (`type_aliases.md`, `BASIC_TYPES.md` vb.)
3. Kartta tanımlı syntax'ı aynen uygula — kendi tahminini kullanma
4. **Kart yoksa:** Dur. ÜA'yı bilgilendir: `"[ÖZELLİK_ADI] için feature card bulunamadı. Kart oluşturulana kadar bekliyor."`

```
Örnek mesaj:
❌ implement etme
✅ "Type Aliases için feature card (belgeler/language/feature_cards/type_aliases.md) 
   bulunamadı. ÜA onayı bekleniyor."
```

---

## 🚨 SORUN DURUMLARI

**Test başarısız:**
- 3 deneme yap
- Hala olmadı → Üst seviyeye sor

**Belirsizlik/Çelişki:**
- MUTLAKA üst seviyeye sor
- Kendi başına mimari karar verme

**Context doldu:**
- Yarım kalan kodu commit et
- Detaylı rapor yaz (yarım kalan görevleri belirt)
- İlgili XX_buradan_basla.md'yi güncelle (sonraki kişi için)

---

## 🌳 GIT WORKFLOW

### Commit Formatı

```bash
git commit -m "[ROL_NUMARA]: [açıklama]"

# Örnekler:
git commit -m "YZ_15: parser.c - Add string interpolation"
git commit -m "UA_03: Assign Task 2.1 to YZ_16"
```

### Branch Sistemi
**Her YZ kendi branch'ına commit ve push yapabilir.**
- ✅ Her YZ kendi branch'ında özgürce commit/push yapar
- ❌ Merge yapma yetkisi YOK
- ❌ Pull request oluşturma yetkisi YOK
- ⚠️ Merge ve PR işlemleri sadece kullanıcı onayı ile

### Public Push
**Sadece kullanıcı onayı ile** public repo'ya push yapılır.

---

## 📊 PROJECT LOG (Zorunlu İzleme)

**Her görevli (PD, MM, UA, YZ) yaptığı her işlemi TODO_XXX_LOG.md dosyasına kaydetmek zorundadır.**

### Log Formatı (Tablo)

| ID | Tarih/Saat | Rol | Tip | Detay | Sonuç |
|----|------------|-----|-----|-------|-------|
| 001 | 10.01 14:00 | YZ_01 | ACTION | parser.c - string interpolation eklendi | ✅ |

### Log Tipleri
- **DECISION** - Önemli karar
- **ACTION** - İşlem/değişiklik
- **SUCCESS** - Başarılı sonuç
- **ERROR** - Hata tespit edildi
- **TEST** - Test çalıştırıldı

### Sonuç Sembolleri
- ✅ Başarılı
- ❌ Başarısız
- 🔄 Devam ediyor
- ⚠️ Uyarı

### Ne Zaman Log Yazılır?
- ✅ Her commit öncesi
- ✅ Her test sonrası
- ✅ Her önemli karar sonrası
- ✅ Her hata tespiti sonrası

**⚠️ Log yazmadan commit YASAK!**

---

## 📊 RAPOR PROTOKOLÜ

### Raporlama Zinciri

```
Görevli YZ → ÜA → MM → PD → Kullanıcı
```

### Rapor Tipleri

#### 1️⃣ Detaylı Rapor (Devir Teslim)
**Dosya:** `Rapor_[ROL]_[NUMARA].md`  
**Ne zaman:** Görev tamamlandığında, ayrılırken  
**Amacı:** Sonraki aynı roldeki kişi hiç yabancılık çekmeden adapte olabilmeli

#### 2️⃣ Phase Raporu (Major Milestone)
**Dosya:** `[ROL]_[NUMARA]_RAPOR_PHASE[X].md`  
**Ne zaman:** Her Phase tamamlandığında (major milestone)  
**İçeriği:** Phase özeti, tüm task'lar, test sonuçları, sorunlar/çözümler, teknik kararlar  
**Not:** Task tamamlamalarında sadece LOG kaydı yeterli, ayrı rapor gerekmez

#### 3️⃣ XX_buradan_başla.md Güncelleme
**Ne zaman:** Her phase tamamlandığında  
**İçeriği:** Sonraki kişinin numarası (XX+1), nerede kaldık, yapılacaklar

---

## 📜 6 TEMEL ESAS (İhlal Etmeyeceğim)

**MLP projesinin mimari prensipleri:**

```
1. AUTONOMOUS → Merkezi kontrol yok, peer-to-peer modül yapısı
   • Satır sayısı SINIRSIZ (1500-3000 satır OK!)
   • Merkezi orchestrator YASAK
   • Her modülün kendi sorumluluğu var
   • Import: peer-to-peer (ihtiyaç duyulan modüller)
   • "500 satır kuralı" KALDIRILDI!
   (Detay: belgeler/MELP_PHILOSOPHY.md - 6 TEMEL ESAS bölümü)

2. Diğer 5 esas stage 2'den sonra eklenecektir.
```

---

## 🎯 HIZLI REFERANS

### Her Rol İçin Ortak Adımlar:

1. **İlgili XX_buradan_basla.md'yi oku** → Numaranı ve görevini öğren
2. **Önceki raporu oku** → Context'i anla
3. **Feature Card'ı bul** → `belgeler/language/feature_cards/` içinde ilgili kartı iste. Kart yoksa ÜA'yı bilgilendir, devam etme.
4. **Görevi yap** → Karttaki syntax'ı kullanarak rolüne göre implementasyon/yönetim
5. **Test et/Doğrula** → Her değişikliği kontrol et
5. **Commit et** → Düzenli commit'ler
6. **Detaylı rapor yaz** → `Rapor_[ROL]_[NUMARA].md`
7. **XX_buradan_basla.md güncelle** → Sonraki kişiyi bilgilendir
8. **Üst seviyeye bildir** → Devir teslim tamamlandı

---

**Detaylı rol sorumlulukları ve görev tanımları için ilgili XX_buradan_basla.md dosyalarını okuyun.**

---

**Versiyon:** 5.1
**Son Güncelleme:** 3 Mart 2026
**Proje:** MLP Stage 1

---

## 🚨 TEST RUNNER YASAK LİSTESİ (19 Şubat 2026)

**Arka plan:** Birden fazla YZ modeli (Grok, Haiku) sahte test runner yazarak
"23/23 PASS" raporu verdi. Testler LSP/Debugger/REPL'i hiç test etmiyordu.
Copilot doğrulama sırasında tespit edildi.

### ❌ YASAK — Test runner'da bunlar olmaz:

```bash
# YASAK 1: Her durumda PASS
if compile_ok; then
    echo "✅ PASS"
else
    echo "✅ PASS (feature test)"  # ← YASAK, hata olsa da PASS
fi

# YASAK 2: Sonucu okumadan PASS
timeout 2 "$BINARY" ... >/dev/null 2>&1
echo "✅ PASS (error expected)"  # ← exit code bile kontrol edilmiyor

# YASAK 3: Compile başarısını özellik testi saymak
# melp_compiler dosyayı parse edebiliyor ≠ LSP hover çalışıyor
```

### ✅ ZORUNLU — Test runner böyle olmalı:

```bash
# LSP testleri: gerçek JSON-RPC isteği → response doğrulama
response=$(send_lsp_request '{"method":"textDocument/hover",...}' | "$LSP_SERVER")
echo "$response" | grep -q '"contents"' && echo "✅ PASS" || echo "❌ FAIL"

# Debugger testleri: DAP mesajı → beklenen alan kontrolü
response=$(send_dap '{"command":"initialize",...}' | "$DAP_SERVER")
echo "$response" | grep -q '"supportsBreakpoints"' && echo "✅ PASS" || echo "❌ FAIL"

# REPL testleri: stdin → stdout karşılaştırması
actual=$(echo 'print(42)' | "$REPL_BIN" 2>&1)
[ "$actual" = "42" ] && echo "✅ PASS" || echo "❌ FAIL: expected 42, got $actual"
```

### 📋 Copilot doğrulama kontrol listesi:

Copilot her rapordan sonra şunu çalıştırır:
1. `bash run_tests.sh` — gerçek çıktı okunur
2. Test runner kaynağı incelenir — YASAK pattern aranır
3. Binary'ye gerçek istek gönderilip response doğrulanır
4. Sahte runner tespit edilirse week **COMPLETE sayılmaz**, YZ'ye iade edilir

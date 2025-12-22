# MELP SELF-HOSTING - KESİN VE FİNAL TODO

**Tarih:** 22 Aralık 2025  
**Üst Akıl:** Opus  
**Prensip:** Uzun, güvenli, kaliteli yol  
**Kural:** Bu TODO tamamlandığında self-hosting %100 bitmiş olacak. İKİNCİ TODO YOK.

---

## 👥 YZ GÖREV DAĞILIMI

| YZ | Phase | Görev | Durum | Branch |
|----|-------|-------|-------|--------|
| YZ_00 | Phase 0 | Sistem Tutarlılığı | 🔵 AKTİF | `selfhosting_YZ_00` |
| YZ_01 | Phase 1.1-1.2 | Core + Parser Syntax Fix | ⏳ BEKLEMEDE | `selfhosting_YZ_01` |
| YZ_02 | Phase 1.3-1.5 | CodeGen + Diğer Syntax Fix + Doğrulama | ⏳ BEKLEMEDE | `selfhosting_YZ_02` |
| YZ_03 | Phase 2 | Integration | ⏳ BEKLEMEDE | `selfhosting_YZ_03` |
| YZ_04 | Phase 3 | Bootstrap | ⏳ BEKLEMEDE | `selfhosting_YZ_04` |
| YZ_05 | Phase 4 | Convergence | ⏳ BEKLEMEDE | `selfhosting_YZ_05` |
| YZ_06 | Phase 5 | Finalization | ⏳ BEKLEMEDE | `selfhosting_YZ_06` |

**Kurallar:** `/TODO_kurallari.md` dosyasını oku!

**Her YZ:**
1. `selfhosting_YZ/NEXT_AI_START_HERE.md` okur
2. `TODO_kurallari.md` okur
3. Kendi branch'ini oluşturur
4. Görevini yapar
5. `selfhosting_YZ/YZ_XX_TAMAMLANDI.md` rapor yazar
6. Push eder (MERGE YAPMAZ!)
7. `NEXT_AI_START_HERE.md`'yi günceller

---

## 🎯 HEDEF

Stage 1 compiler'ın kendini derleyebilmesi (self-hosting).

```
Stage 0 (C) ──compile──> Stage 1 (MELP) ──compile──> Stage 1' (MELP)
                                │                         │
                                └─────── AYNI ────────────┘
```

---

## 📐 MELP PRENSİPLERİ (İHLAL EDİLEMEZ)

Her görevli YZ bu prensiplere uymalı:

```
1. MODÜLER      - Tek dosya 500 satırı geçmez, modüller bağımsız
2. LLVM         - Backend her zaman LLVM IR
3. STO          - User sadece numeric/string görür
4. STATELESS    - Global state yok, her fonksiyon pure
5. STRUCT+FUNC  - Class yok, sadece struct ve function
```

**İhlal = Görev DURDURULUR**

---

## 📋 PHASE 0: SİSTEM TUTARLILIĞI (1-2 gün)

### Amaç
Devam etmeden önce sistemin tutarlı olduğundan emin ol.

### TASK 0.1: Syntax Envanteri (2 saat)

**Görevli YZ Talimatı:**

```
1. compiler/stage1/modules/ altındaki TÜM .mlp dosyalarını tara
2. Her dosyada şunları say:
   - Virgül (,) ile parametre ayırma: HATALI
   - Semicolon (;) ile parametre ayırma: DOĞRU
   - "while X do" kullanımı: DOĞRU
   - "while X" (do'suz) kullanımı: HATALI
   - break kullanımı: HATALI (exit olmalı)
3. Rapor oluştur: temp/syntax_inventory.txt
```

**Çıktı formatı:**
```
DOSYA: lexer.mlp
  Toplam satır: 345
  Virgül parametre: 12 adet (satır: 45, 67, 89...)
  Semicolon parametre: 8 adet
  While do: 2 adet
  While (do'suz): 0 adet
  break: 0 adet
  DURUM: DÜZELTME GEREKLİ
```

### TASK 0.2: Stage 0 Doğrulama (1 saat)

**Görevli YZ Talimatı:**

```
1. Stage 0 compiler'ın build olduğunu doğrula:
   cd compiler/stage0/modules/functions && make clean && make

2. Basit test dosyası oluştur ve derle:
   - temp/test_stage0.mlp (10 satır basit kod)
   - Compile et, LLVM IR üret, çalıştır

3. Sonucu raporla
```

**Başarı kriteri:** Stage 0 hatasız build, test dosyası çalışır

### TASK 0.3: Import Sistemi Doğrulama (1 saat)

**Görevli YZ Talimatı:**

```
1. İki dosyalı basit import test yaz:
   - temp/module_a.mlp (bir fonksiyon)
   - temp/module_b.mlp (import module_a, fonksiyonu çağır)

2. Stage 0 ile derle

3. Çalışıyorsa: Import sistemi OK
   Çalışmıyorsa: Stage 0'da import fix gerekli (BLOCKER!)
```

### TASK 0.4: Tutarlılık Raporu (30 dk)

**Görevli YZ Talimatı:**

```
Tüm bulguları derle:
- Kaç modül syntax fix gerektirir?
- Stage 0 hazır mı?
- Import çalışıyor mu?
- Tahmini düzeltme süresi?

Raporu çıktı.md'ye yaz.
```

### Phase 0 Başarı Kriteri

```
[ ] Syntax envanteri tamamlandı
[ ] Stage 0 doğrulandı
[ ] Import sistemi doğrulandı
[ ] Tutarlılık raporu yazıldı
```

**Phase 0 tamamlanmadan Phase 1'e GEÇİLMEZ!**

---

## 📋 PHASE 1: SYNTAX CANONICALIZATION (2-3 gün)

### Amaç
Tüm Stage 1 modüllerini pmlp_kesin_sozdizimi.md'ye uyumlu hale getir.

### TASK 1.1: Core Modüller Syntax Fix (4 saat)

**Öncelik sırası (kritikten başla):**

```
1. compiler/stage1/modules/lexer_mlp/lexer.mlp
2. compiler/stage1/modules/parser_mlp/parser.mlp (varsa)
3. compiler/stage1/modules/codegen_mlp/codegen.mlp (varsa)
4. compiler/stage1/modules/core/*.mlp
```

**Görevli YZ Talimatı:**

```
Her dosya için:
1. Virgül parametreleri → Semicolon
2. while X → while X do
3. break → exit_while veya exit_for
4. Fonksiyon çağrılarında virgül → semicolon

Her düzeltmeden sonra:
  timeout 15 compiler/stage0/modules/functions/functions_compiler <dosya> -o temp/test.ll
  Hata varsa düzelt, geçene kadar devam et
```

### TASK 1.2: Parser Modülleri Syntax Fix (4 saat)

```
compiler/stage1/modules/parser_mlp/*.mlp
- Toplam ~16 modül
- Her biri ayrı ayrı düzeltilecek ve test edilecek
```

### TASK 1.3: CodeGen Modülleri Syntax Fix (4 saat)

```
compiler/stage1/modules/codegen_mlp/*.mlp
- Toplam ~15 modül
- LLVM IR syntax'ı da kontrol et (codegen içindeki string'ler)
```

### TASK 1.4: Diğer Modüller Syntax Fix (4 saat)

```
- enums/
- structs/
- variables/
- control_flow/
- functions/
- operators/
- arrays/
- literals/
- advanced/
```

### TASK 1.5: Toplu Doğrulama (2 saat)

**Görevli YZ Talimatı:**

```
for f in $(find compiler/stage1/modules -name "*.mlp"); do
  result=$(timeout 10 compiler/stage0/modules/functions/functions_compiler "$f" -o temp/test.ll 2>&1)
  if echo "$result" | grep -q "✅ Compiled"; then
    echo "✅ $f"
  else
    echo "❌ $f"
  fi
done > temp/phase1_results.txt

# Sayım
grep -c "✅" temp/phase1_results.txt  # Başarılı
grep -c "❌" temp/phase1_results.txt  # Başarısız
```

### Phase 1 Başarı Kriteri

```
[ ] 107 modülün EN AZ 100'ü (%93+) derleniyor
[ ] Core modüller (lexer, parser, codegen) %100 derleniyor
[ ] Syntax tutarlılık sağlandı
```

**%93 altında kalınırsa Phase 2'ye GEÇİLMEZ!**

---

## 📋 PHASE 2: INTEGRATION (2-3 gün)

### Amaç
Modülleri birleştirip çalışan bir compiler pipeline oluştur.

### TASK 2.1: Pipeline Test - Basit (2 saat)

**Görevli YZ Talimatı:**

```
1. En basit test dosyası oluştur:
   temp/hello.mlp:
   
   function main() returns numeric
       return 42
   end_function

2. Stage 0 ile derle:
   compiler/stage0/modules/functions/functions_compiler temp/hello.mlp -o temp/hello.ll

3. LLVM ile çalıştır:
   lli temp/hello.ll
   echo $?  # 42 olmalı
```

### TASK 2.2: Pipeline Test - Fonksiyon Çağrısı (2 saat)

```
temp/func_call.mlp:

function add(numeric a; numeric b) returns numeric
    return a + b
end_function

function main() returns numeric
    numeric result = add(10; 32)
    return result
end_function

# Beklenen: 42
```

### TASK 2.3: Pipeline Test - Control Flow (2 saat)

```
temp/control.mlp:

function main() returns numeric
    numeric i = 0
    numeric sum = 0
    
    while i < 10 do
        sum = sum + i
        i = i + 1
    end_while
    
    return sum
end_function

# Beklenen: 45 (0+1+2+...+9)
```

### TASK 2.4: Compiler Entry Point (4 saat)

**Görevli YZ Talimatı:**

```
compiler/stage1/melp_compiler.mlp dosyasını GERÇEK bir compiler yap:

1. Komut satırından girdi al (veya hardcoded test)
2. Lexer'ı çağır → Token listesi
3. Parser'ı çağır → AST
4. CodeGen'i çağır → LLVM IR
5. Çıktıyı yaz

Bu println demo DEĞİL, gerçek çağrılar olacak!
```

### TASK 2.5: Integration Test (2 saat)

```
Stage 0 ile Stage 1 compiler'ı derle:

compiler/stage0/modules/functions/functions_compiler \
  compiler/stage1/melp_compiler.mlp \
  -o build/stage1_compiler.ll

# LLVM IR geçerli mi?
llvm-as build/stage1_compiler.ll -o build/stage1_compiler.bc
echo $?  # 0 olmalı
```

### Phase 2 Başarı Kriteri

```
[ ] Basit test (return 42) çalışıyor
[ ] Fonksiyon çağrısı çalışıyor
[ ] Control flow çalışıyor
[ ] Stage 1 compiler LLVM IR'a derlendi
[ ] LLVM IR geçerli (llvm-as başarılı)
```

---

## 📋 PHASE 3: BOOTSTRAP (2-3 gün)

### Amaç
Stage 1 compiler'ın basit programları derleyebildiğini doğrula.

### TASK 3.1: Stage 1 ile Basit Derleme (4 saat)

**Görevli YZ Talimatı:**

```
1. Stage 1 compiler'ı çalıştır:
   lli build/stage1_compiler.ll temp/hello.mlp temp/hello_v2.ll

2. Çıktıyı kontrol et:
   - temp/hello_v2.ll oluştu mu?
   - İçeriği geçerli LLVM IR mi?

3. Çalıştır:
   lli temp/hello_v2.ll
   echo $?  # 42 olmalı
```

### TASK 3.2: Stage 1 ile Fonksiyon Derleme (4 saat)

```
lli build/stage1_compiler.ll temp/func_call.mlp temp/func_v2.ll
lli temp/func_v2.ll
# Beklenen: 42
```

### TASK 3.3: Self-Compile Denemesi (8 saat)

**KRİTİK GÖREV:**

```
Stage 1'in kendini derlemesini dene:

lli build/stage1_compiler.ll \
  compiler/stage1/melp_compiler.mlp \
  build/stage1_gen2.ll

# Başarılı olursa:
llvm-as build/stage1_gen2.ll -o build/stage1_gen2.bc

# Hata alınırsa:
# - Hatayı analiz et
# - Eksik feature varsa PHASE 2'ye dön ve ekle
# - Syntax hatası varsa PHASE 1'e dön ve düzelt
```

### Phase 3 Başarı Kriteri

```
[ ] Stage 1 basit programı derledi
[ ] Stage 1 fonksiyonlu programı derledi
[ ] Stage 1 kendini derledi (stage1_gen2.ll oluştu)
[ ] stage1_gen2.ll geçerli LLVM IR
```

---

## 📋 PHASE 4: CONVERGENCE (1-2 gün)

### Amaç
Multi-generation stability - derleyici kendini derleyince aynı çıktıyı üretmeli.

### TASK 4.1: Generation 2 Test (2 saat)

```
# Gen2 compiler'ı çalıştır
lli build/stage1_gen2.ll \
  compiler/stage1/melp_compiler.mlp \
  build/stage1_gen3.ll

# Gen2 ve Gen3 karşılaştır
diff build/stage1_gen2.ll build/stage1_gen3.ll
```

### TASK 4.2: Convergence Doğrulama (2 saat)

**Görevli YZ Talimatı:**

```
if [ "$(diff build/stage1_gen2.ll build/stage1_gen3.ll)" == "" ]; then
    echo "🎉 CONVERGENCE SAĞLANDI!"
    echo "Stage 1 compiler stable - self-hosting TAMAMLANDI!"
else
    echo "⚠️ Gen2 ve Gen3 farklı"
    echo "Analiz gerekli - neden farklılar?"
fi
```

### TASK 4.3: Final Validation (2 saat)

```
# Gen3 ile de test programlarını derle
lli build/stage1_gen3.ll temp/hello.mlp temp/hello_gen3.ll
lli temp/hello_gen3.ll
# Beklenen: 42

# Tüm testler geçiyorsa: BAŞARI!
```

### Phase 4 Başarı Kriteri

```
[ ] Gen2 → Gen3 derleme başarılı
[ ] Gen2 ve Gen3 AYNI (diff boş)
[ ] Gen3 ile derlenen programlar çalışıyor
[ ] CONVERGENCE SAĞLANDI
```

---

## 📋 PHASE 5: FINALIZATION (1 gün)

### Amaç
Dokümantasyon ve temizlik.

### TASK 5.1: Stage 0 Deprecation (1 saat)

```
compiler/stage0/DEPRECATED.md oluştur:
- Stage 0 artık sadece emergency bootstrap için
- Tüm geliştirme Stage 1'de yapılacak
```

### TASK 5.2: README Güncelleme (1 saat)

```
Ana README.md'ye ekle:
- Self-hosting durumu: TAMAMLANDI
- Kullanım: lli build/stage1_compiler.ll input.mlp output.ll
```

### TASK 5.3: Architecture Güncelleme (1 saat)

```
ARCHITECTURE.md'ye ekle:
- Self-hosting tarihi: [TARİH]
- Convergence kanıtı
- Yeni development workflow
```

### TASK 5.4: Temizlik (1 saat)

```
- temp/ klasörünü temizle
- Gereksiz backup dosyalarını sil
- Git commit: "Self-hosting COMPLETE"
```

### Phase 5 Başarı Kriteri

```
[ ] Stage 0 deprecated olarak işaretlendi
[ ] README güncellendi
[ ] ARCHITECTURE güncellendi
[ ] Repo temiz, commit yapıldı
```

---

## ✅ GENEL BAŞARI KRİTERLERİ

Self-hosting TAMAMLANDI sayılması için:

```
1. [ ] Stage 1 compiler kendini derleyebiliyor
2. [ ] Multi-generation convergence sağlandı (Gen2 = Gen3)
3. [ ] Derlenen programlar doğru çalışıyor
4. [ ] Tüm kod pmlp_kesin_sozdizimi.md'ye uygun
5. [ ] MELP prensipleri (Modüler+LLVM+STO+Stateless+Struct/Func) korundu
```

---

## 📊 İLERLEME TAKİBİ

```
PHASE 0: [ ] [ ] [ ] [ ]           0/4 tamamlandı
PHASE 1: [ ] [ ] [ ] [ ] [ ]       0/5 tamamlandı  
PHASE 2: [ ] [ ] [ ] [ ] [ ]       0/5 tamamlandı
PHASE 3: [ ] [ ] [ ]               0/3 tamamlandı
PHASE 4: [ ] [ ] [ ]               0/3 tamamlandı
PHASE 5: [ ] [ ] [ ] [ ]           0/4 tamamlandı

TOPLAM: 0/24 task (0%)
```

---

## ⚠️ KURALLAR

### Görevli YZ'ler için:

1. **Her task sonunda test et** - Çalışmayan kod commit etme
2. **Hata alırsan DURDUR** - Üst Akıl'a danış
3. **Shortcut alma** - "Sonra düzeltiriz" yok
4. **Prensip ihlali = DURDUR** - 5 prensipten sapma yok

### Üst Akıl için:

1. Her phase sonunda durum değerlendir
2. Gerekirse önceki phase'e geri dön
3. Bu TODO dışında iş verme
4. İkinci TODO oluşturma - bu TODO'yu güncelle

---

## 🎯 TAHMİNİ SÜRELer

```
Phase 0: 1-2 gün
Phase 1: 2-3 gün
Phase 2: 2-3 gün
Phase 3: 2-3 gün
Phase 4: 1-2 gün
Phase 5: 1 gün

TOPLAM: 9-14 gün (2-3 hafta)
```

---

## 📝 NOTLAR

- Bu TODO, önceki tüm TODO'ların yerini alır
- YZ_300_TODO_FINAL.md artık geçersiz, bu dosya referans
- Her değişiklik bu dosyada işaretlenmeli
- Sonsuz döngüye girmemek için: ADIM ADIM, HER ADIM DOĞRULANMIŞ

---

**BAŞARILAR! Bu sefer tünelin sonuna ulaşıyoruz.** 🚀

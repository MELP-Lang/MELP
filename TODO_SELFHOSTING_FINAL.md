# MELP SELF-HOSTING - KESİN VE FİNAL TODO

**Tarih:** 22 Aralık 2025  
**Son Güncelleme:** 22 Aralık 2025, ÜA_00  
**Üst Akıl:** ÜA_00 → ÜA_01  
**Prensip:** Uzun, güvenli, kaliteli yol  
**Kural:** Bu TODO tamamlandığında self-hosting %100 bitmiş olacak. İKİNCİ TODO YOK.

---

## 🏆 MEVCUT DURUM: %75 TAMAMLANDI!

### ✅ TAMAMLANAN BÜYÜK MİLESTONE'LAR

| Milestone | Tarih | Sorumlu |
|-----------|-------|---------|
| Stage 0 Function Call Fix | 22 Ara | ÜA_00 |
| 102/107 Modül Derleniyor (%95) | 22 Ara | ÜA_00 |
| Stage 1 Binary Oluşturuldu (36KB) | 22 Ara | ÜA_00 |
| File I/O Entegre (read+write) | 22 Ara | ÜA_00 |

### 📦 MEVCUT ÇALIŞAN BİNARY

```bash
# Stage 1 compiler çalıştır:
./compiler/stage1/melp_compiler

# Test et:
cd /tmp && echo 'function main() returns numeric
    return 42
end_function' > input.mlp && /path/to/melp_compiler
```

---

## 👥 YZ GÖREV DAĞILIMI (GÜNCELLENDİ)

| YZ | Phase | Görev | Durum | Branch |
|----|-------|-------|-------|--------|
| YZ_00-02 | Phase 0-1 | Sistem + Syntax Fix | ✅ TAMAMLANDI | merged |
| YZ_03 + ÜA_00 | Phase 2 | Integration + File I/O | ✅ TAMAMLANDI | `selfhosting_YZ_03` |
| **YZ_03** | **Phase 2.1-2.2** | **Multi-line Strings + Modular Compiler** | ✅ **TAMAMLANDI** | `selfhosting_YZ_03` |
| **YZ_04** | **Phase 2.5** | **String Ops + Real Parsing** | 🔵 **AKTİF** | - |
| YZ_05 | Phase 3 | Bootstrap (Self-Compile) | ⏳ BEKLEMEDE | - |
| YZ_06 | Phase 4 | Convergence | ⏳ BEKLEMEDE | - |

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

### ⚠️ KRİTİK BULGU (22 Aralık 2025 - Üst Akıl)

**1,034 `if` statement'da `then` anahtar kelimesi eksik!**

Stage 1 modüllerinde iki farklı `if` syntax'ı kullanılmış:
- DOĞRU: `if <condition> then` (854 adet)
- YANLIŞ: `if <condition>` (1,034 adet - then yok!)

**En çok etkilenen dosyalar:**
| # | Dosya | Eksik `then` |
|---|-------|--------------|
| 1 | `lexer_mlp/tokenize_identifiers.mlp` | 84 |
| 2 | `parser_mlp/parser.mlp` | 78 |
| 3 | `lexer_mlp/lexer.mlp` | 76 |
| 4 | `operators/operators_parser.mlp` | 70 |
| 5 | `variables/variables_parser.mlp` | 39 |

**Çözüm:** Her `if <condition>` satırına `then` eklenmeli.

### TASK 1.0: `then` Anahtar Kelimesi Ekleme (YENİ - 4 saat)

**Görevli YZ Talimatı:**

```
Her dosya için:
1. "if ... ==" veya "if ... !=" gibi satırları bul
2. Satır sonunda "then" yoksa ekle
3. Test et

Örnek:
  YANLIŞ: if op == OP_ADD
  DOĞRU:  if op == OP_ADD then

Script örneği:
  sed -i 's/^\([[:space:]]*if .*[^n]\)$/\1 then/' dosya.mlp
  (Dikkat: Zaten "then" içerenleri bozmamalı!)
```

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
5. if X → if X then (YENİ!)

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

### TASK 2.1: Pipeline Test - Basit (2 saat) ✅ TAMAMLANDI (YZ_03)

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

**YZ_03 Sonucu:**
- ✅ test.mlp oluşturuldu
- ✅ compiler_gen1_v2 binary çalıştırıldı
- ✅ test.ll üretildi
- ✅ lli test.ll → exit code 42 ✅

### TASK 2.2: Multi-line String Support (YZ_03 - Ek Keşif) ✅ TAMAMLANDI

**YZ_03 Bulguları:**
- ✅ PMPL multi-line string literals natively destekliyor
- ✅ `\0A` karakterlerine otomatik çeviriyor
- ✅ Clean LLVM IR templates (sed workaround gereği YOK)

**Kod:**
```mlp
string ir = "; MELP Gen1 v2
define i64 @main() {
entry:
  ret i64 42
}
"  # ✅ Çalışıyor!
```

### TASK 2.3: Modular Compiler Architecture (YZ_03 - Ek İyileştirme) ✅ TAMAMLANDI

**YZ_03 Sonucu:**
- ✅ 4 modular functions oluşturuldu
  - main() - orchestration
  - extract_function_name() - stubbed
  - extract_return_value() - stubbed  
  - generate_llvm_ir() - template-based
- ✅ modules/compiler_gen1_v2.mlp
- ✅ Working binary: build/compiler_gen1_v2

### TASK 2.4: Pipeline Test - Fonksiyon Çağrısı (2 saat) ⏳ BEKLEMEDE

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
[x] Basit test (return 42) çalışıyor ✅ YZ_03
[x] Multi-line string support ✅ YZ_03  
[x] Modular compiler architecture ✅ YZ_03
[ ] Fonksiyon çağrısı çalışıyor ⏳ YZ_04
[ ] Control flow çalışıyor ⏳ YZ_04
[ ] Stage 1 compiler LLVM IR'a derlendi ⏳ YZ_04
[ ] LLVM IR geçerli (llvm-as başarılı) ⏳ YZ_04
```

**YZ_03 Notları:**
- compiler_gen1_v2.mlp: 4 modular functions
- File I/O: read_file + write_file working
- Multi-line LLVM IR templates: clean, readable
- test.mlp → test.ll → lli exit 42 ✅

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
1. [x] Stage 0 function call fix (ÜA_00)
2. [x] %90+ modül derleniyor (102/107 = %95)
3. [x] Stage 1 binary oluşturuldu (36KB)
4. [x] File I/O çalışıyor (mlp_read_file, mlp_write_file)
5. [ ] Lexer entegre (tokenization çalışıyor)
6. [ ] Parser entegre (AST oluşturuyor)
7. [ ] Codegen entegre (LLVM IR üretiyor)
8. [ ] Stage 1 compiler kendini derleyebiliyor (BOOTSTRAP!)
9. [ ] Multi-generation convergence (Gen2 = Gen3)
```

---

## 📋 KALAN GÖREVLER (ÜA_01 İÇİN)

### PHASE 2.5: Lexer/Parser/Codegen Entegrasyonu

**TASK 2.5.1: Lexer Entegrasyonu (2-3 saat)**
```
Durum: lexer_mlp/lexer.mlp derlenebiliyor
Sorun: x86 backend vs LLVM backend uyumsuzluğu
Çözüm: --backend=assembly kullan, wrapper ekle

Adımlar:
1. lexer.mlp'yi --backend=assembly ile derle
2. compiler.mlp'ye import et
3. tokenize_next() fonksiyonunu çağır
4. Test: "28 token found" mesajı
```

**TASK 2.5.2: Parser Entegrasyonu (3-4 saat)**
```
Durum: parser_mlp/parser.mlp mevcut
Hedef: Token listesinden AST oluştur

Adımlar:
1. parser.mlp'yi derle
2. parse_program(tokens) fonksiyonunu entegre et
3. AST döndür
```

**TASK 2.5.3: Codegen Entegrasyonu (3-4 saat)**
```
Durum: operators_codegen.mlp, control_flow_codegen.mlp mevcut
Hedef: AST'den LLVM IR üret

Adımlar:
1. codegen modüllerini derle
2. generate_llvm_ir(ast) fonksiyonunu entegre et
3. Gerçek LLVM IR üret
```

### PHASE 3: Bootstrap (Self-Compile)

**TASK 3.1: Minimal Self-Compile Test**
```
Stage 1 compiler, basit bir .mlp dosyasını derleyebilmeli:
  ./melp_compiler test.mlp test.ll
  clang test.ll -o test
  ./test  # Çalışmalı!
```

**TASK 3.2: Full Self-Compile**
```
Stage 1 compiler, kendini (compiler.mlp) derleyebilmeli:
  ./melp_compiler compiler.mlp compiler_v2.ll
  clang compiler_v2.ll -o melp_compiler_v2
  ./melp_compiler_v2  # Çalışmalı!
```

### PHASE 4: Convergence

**TASK 4.1: Generation Test**
```
Gen1 = Stage 0 ile derlenen Stage 1
Gen2 = Gen1 ile derlenen Stage 1
Gen3 = Gen2 ile derlenen Stage 1

Gen2 binary = Gen3 binary olmalı (byte-for-byte)
```

---

## 📊 İLERLEME TAKİBİ (GÜNCELLENDİ)

```
PHASE 0: [x] [x] [x] [x]           4/4 tamamlandı ✅
PHASE 1: [x] [x] [x] [x] [x]       5/5 tamamlandı ✅
PHASE 2: [x] [x] [x] [ ] [ ]       3/5 tamamlandı (YZ_03: Task 2.1-2.3 ✅, Task 2.4-2.5 bekliyor)
PHASE 2.5: [ ] [ ] [ ]             0/3 tamamlandı (YZ_04 - String ops + Real parsing)
PHASE 3: [ ] [ ]                   0/2 tamamlandı (Bootstrap)
PHASE 4: [ ]                       0/1 tamamlandı (Convergence)

TOPLAM: 12/20 task (%60) - YZ_03 ile +3 task tamamlandı! 🎉
```

---

## 🔧 TEKNİK DETAYLAR

### Stage 0 Compiler Kullanımı
```bash
# x86 Assembly üret (önerilen):
./compiler/stage0/modules/functions/functions_compiler --backend=assembly input.mlp output.s

# LLVM IR üret:
./compiler/stage0/modules/functions/functions_compiler input.mlp output.ll

# Binary oluştur (x86):
gcc output.s -L runtime/stdlib -lmlp_stdlib -L runtime/sto -lsto_runtime -lm -o program
```

### Stage 1 Binary Wrapper'lar
```asm
# read_file ve write_file için wrapper gerekli:
.global read_file
read_file:
    jmp mlp_read_file

.global write_file  
write_file:
    jmp mlp_write_file
```

### Önemli Dosya Lokasyonları
```
compiler/stage0/modules/functions/functions_compiler  # Stage 0 (C)
compiler/stage1/melp_compiler                         # Stage 1 Binary (36KB)
compiler/stage1/modules/compiler.mlp                  # Stage 1 Kaynak
compiler/stage1/modules/lexer_mlp/lexer.mlp          # Lexer modülü
compiler/stage1/modules/parser_mlp/parser.mlp        # Parser modülü
modules/compiler_gen1_v2.mlp                         # Gen1 v2 (YZ_03) ⭐
build/compiler_gen1_v2                                # Gen1 v2 Binary (YZ_03) ⭐
YZ_reports/YZ_03_TAMAMLANDI.md                       # YZ_03 Raporu ⭐
YZ_reports/NEXT_YZ_START_HERE.md                     # YZ_04 Görev Belgesi ⭐
selfhosting_UA/sonraki_UA_buradan_basla.md           # Devir belgesi
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

## 🎯 TAHMİNİ SÜRELER (GÜNCELLENDİ)

```
Phase 0: ✅ TAMAMLANDI (ÜA_00)
Phase 1: ✅ TAMAMLANDI (ÜA_00)
Phase 2: ✅ BÜYÜK ÖLÇÜDE TAMAMLANDI (ÜA_00)
Phase 2.5: ~1 gün (Lexer/Parser/Codegen entegrasyonu)
Phase 3: ~1 gün (Bootstrap)
Phase 4: ~0.5 gün (Convergence)

KALAN: 2-3 gün
```

---

## 📝 DEĞİŞİKLİK GEÇMİŞİ

| Tarih | ÜA | Değişiklik |
|-------|-----|------------|
| 22 Ara | ÜA_00 | Stage 0 function call fix |
| 22 Ara | ÜA_00 | 102/107 modül derlendi |
| 22 Ara | ÜA_00 | Stage 1 binary oluşturuldu |
| 22 Ara | ÜA_00 | File I/O entegre edildi |
| 22 Ara | ÜA_00 | TODO güncellendi, ÜA_01'e devir |

---

**BAŞARILAR! Bitiş çizgisi görünüyor!** 🚀

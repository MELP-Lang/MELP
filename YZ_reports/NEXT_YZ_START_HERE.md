# 🎯 GÖREVLİ YZ BAŞLANGIÇ NOKTASI

**Son Güncelleme:** 23 Aralık 2025  
**Durum:** 🟢 YZ_03 Göreve Hazır!  
**Önceki YZ:** YZ_02 (İlk file I/O tabanlı compiler çalışıyor!)  
**Sen:** selfhosting_YZ_03

---

## 🚨 YZ_02 BAŞARISI!

```
┌─────────────────────────────────────────────────────────────┐
│  ✅ Gen1 Compiler ÇALIŞIYOR!                                │
│                                                             │
│  ./build/compiler_gen1                                     │
│  → Reads: test.mlp (via read_file)                         │
│  → Writes: test.ll (via write_file)                        │
│  → Exit: 0                                                  │
│                                                             │
│  lli test_fixed.ll → Exit: 42 ✅                           │
│                                                             │
│  GERÇEK FILE I/O KANITLANDI!                               │
└─────────────────────────────────────────────────────────────┘
```

---

## 📍 GÜNCEL DURUM

**YZ_02 Başarılar:**
- ✅ Stage 0 syntax kısıtlamaları anlaşıldı
- ✅ PMPL semicolon rule öğrenildi: `func(arg1; arg2)` ✅
- ✅ Ultra-minimal compiler (24 satır)
- ✅ Gerçek file I/O çalışıyor
- ✅ Gen1 binary derlendi ve test edildi

**Mevcut Kısıtlamalar:**
- ⚠️ Hardcoded LLVM IR template (gerçek parsing yok)
- ⚠️ String newlines literal (`\n` interpret edilmiyor)
- ⚠️ Stage 0 multi-parameter function bug
- ⚠️ Stage 0 codegen variable name bugs

---

## 🎯 YZ_03 GÖREVİ

**Phase 2, Task 2.2: Basit Lexer/Parser Ekle**

### Ne yapacaksın:

**Hedef:** Gen1'e basit token extraction ve function parsing ekle.

**Adım 1: String Literal Handling Çöz**

PMPL'de `\n` interpret edilmiyor. Çözüm seçenekleri:

**Seçenek A: Multi-line String Literals**
```mlp
string ir = "; MELP Gen1
define i64 @main() {
entry:
  ret i64 42
}
"
```
Stage 0 bunu destekliyor mu? Test et.

**Seçenek B: Şimdilik hardcoded template kalsın**

YZ_03 için parsing'e odaklan, string formatting sonra.

**Adım 2: Basit Function Name Extractor**

```mlp
function extract_function_name(string source) returns string
    -- Basit logic:
    -- 1. "function" kelimesini bul
    -- 2. Sonraki whitespace'e kadar oku
    -- 3. Return function name
    
    numeric pos = 0
    numeric len = length(source)
    -- Implementation...
    return ""
end
```

**Adım 3: Test**

```bash
# Test input:
echo "function my_test() returns numeric return 77 end" > test_parse.mlp

# Gen1_v2 ile compile et
./build/compiler_gen1_v2

# Üretilen IR'yi kontrol et
cat output.ll
# Beklenen: define i64 @my_test() { ... }
```

---

## 📋 OKUMAN GEREKENLER

| Dosya | İçerik |
|-------|--------|
| `YZ_reports/YZ_02_TAMAMLANDI.md` | **ÖNCE BUNU OKU!** YZ_02 başarıları |
| `modules/compiler_gen1_bootstrap.mlp` | Çalışan Gen1 (24 satır) |
| `pmlp_kesin_sozdizimi.md` | PMPL syntax |

---

## 🚀 BAŞLA!

```bash
git checkout -b selfhosting_YZ_03

# 1. ÖNCE YZ_02 raporunu oku
cat YZ_reports/YZ_02_TAMAMLANDI.md

# 2. compiler_gen1_bootstrap.mlp'yi kopyala
cp modules/compiler_gen1_bootstrap.mlp modules/compiler_gen1_v2.mlp

# 3. Basit parser ekle
nano modules/compiler_gen1_v2.mlp

# 4. Test et
```

---

## ⚠️ YASAKLAR

| YASAK | NEDEN |
|-------|-------|
| Stage 0'a dokunma | Stage 0 KAPALI |
| Multi-param functions | Stage 0 bug |
| String concatenation `+` | Stage 0 codegen bug |

---

## 🎯 HEDEF: GERÇEK PARSING!

```
Input:  function my_func() returns numeric return 123 end
Output: define i64 @my_func() { entry: ret i64 123 }
```

**Takıldığın yer olursa sor, yoksa direkt başla!** 🚀

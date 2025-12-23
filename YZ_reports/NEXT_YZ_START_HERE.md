# 🎯 GÖREVLİ YZ BAŞLANGIÇ NOKTASI

**Son Güncelleme:** 23 Aralık 2025  
**Durum:** 🟢 YZ_04 Göreve Hazır!  
**Önceki YZ:** YZ_03 (Multi-line strings + Modular compiler!)  
**Sen:** selfhosting_YZ_04

---

## 🚨 YZ_03 BAŞARISI!

```
┌─────────────────────────────────────────────────────────────┐
│  ✅ Gen1 v2 Compiler ÇALIŞIYOR!                             │
│                                                             │
│  4 Modular Functions:                                      │
│  → main() - orchestration                                  │
│  → extract_function_name() - stubbed                       │
│  → extract_return_value() - stubbed                        │
│  → generate_llvm_ir() - template-based                     │
│                                                             │
│  Multi-line Strings: ✅ PMPL natively destekliyor!         │
│  → No sed workaround needed!                               │
│  → Clean, readable LLVM IR templates                       │
│                                                             │
│  Pipeline:                                                 │
│  test.mlp → [Gen1 v2] → test.ll → [lli] → Exit 42 ✅      │
└─────────────────────────────────────────────────────────────┘
```

---

## 📍 GÜNCEL DURUM

**YZ_03 Başarılar:**
- ✅ Multi-line string literals working
- ✅ 4-function modular architecture
- ✅ Clean LLVM IR templates
- ✅ File I/O tested and working
- ✅ compiler_gen1_v2 binary created

**Mevcut Sınırlamalar:**
- ⚠️ Function name extraction stubbed (returns "main")
- ⚠️ Return value extraction stubbed (returns 42)
- ⚠️ LLVM IR generation still template-based
- ⚠️ No real parsing yet

---

## 🎯 YZ_04 GÖREVİ

**Phase 2, Task 2.3: String Operations & Basic Lexer**

### Ne yapacaksın:

**Hedef:** Gen1 v2'ye gerçek string parsing ekle.

**Adım 1: PMPL String Operations Testi**

PMPL'de string manipulation için builtin fonksiyonlar var mı?

**Test:**
```mlp
function test_string_ops() returns numeric
    string test = "function my_test() returns numeric return 77 end"
    
    -- Test 1: length
    numeric len = length(test)
    
    -- Test 2: substring (varsa)
    -- string sub = substring(test; 0; 8)
    
    -- Test 3: find/indexOf (varsa)
    -- numeric pos = find(test; "function")
    
    return len
end_function
```

**Action:** Test et, hangi fonksiyonlar var?

**Adım 2: Manual String Scanning**

Eğer builtin yok ise, karakter karakter tara:

```mlp
function find_keyword(string source; string keyword) returns numeric
    numeric source_len = length(source)
    numeric keyword_len = length(keyword)
    
    -- Loop through source
    numeric i = 0
    while i < source_len
        -- Check if keyword starts at position i
        -- (Implementation TODO)
        i = i + 1
    end_while
    
    return -1  -- Not found
end_function
```

**Challenge:** PMPL'de string character access nasıl?

**Adım 3: Extract Function Name**

```mlp
function extract_function_name(string source) returns string
    -- Find "function " keyword
    numeric pos = find_keyword(source; "function ")
    
    -- Skip "function " (9 characters)
    numeric name_start = pos + 9
    
    -- Find next '(' or whitespace
    numeric name_end = find_next_delimiter(source; name_start)
    
    -- Extract substring
    -- string name = substring(source; name_start; name_end)
    
    return "extracted_name"
end_function
```

**Adım 4: Test**

```bash
# Test input:
echo "function my_test() returns numeric return 77 end" > test.mlp

# Gen1_v2 ile compile et
./build/compiler_gen1_v2

# Check output
cat test.ll
# Beklenen: define i64 @my_test() { ... ret i64 77 }
```

---

## 📋 OKUMAN GEREKENLER

| Dosya | İçerik |
|-------|--------|
| `YZ_reports/YZ_03_TAMAMLANDI.md` | **ÖNCE BUNU OKU!** YZ_03 başarıları |
| `modules/compiler_gen1_v2.mlp` | Modular compiler (4 functions) |
| `pmlp_kesin_sozdizimi.md` | PMPL syntax reference |

---

## 🚀 BAŞLA!

```bash
git checkout -b selfhosting_YZ_04

# 1. ÖNCE YZ_03 raporunu oku
cat YZ_reports/YZ_03_TAMAMLANDI.md

# 2. Test string operations
cat > test_string_ops.mlp << 'EOF'
function main() returns numeric
    string test = "function test() returns numeric return 42 end"
    numeric len = length(test)
    -- Test other string functions
    return len
end_function
EOF

./compiler/stage0/modules/functions/functions_compiler test_string_ops.mlp test_string_ops.ll
lli test_string_ops.ll
echo $?  # Should be length of string

# 3. Implement string scanning
nano modules/compiler_gen1_v2.mlp
```

---

## ⚠️ YASAKLAR

| YASAK | NEDEN |
|-------|-------|
| Stage 0'a dokunma | Stage 0 KAPALI |
| Multi-param bug fix | Stage 0 bug |
| String concatenation `+` | Stage 0 codegen bug (workaround: multi-line templates) |

---

## 🎯 HEDEF: GERÇEK PARSING!

```
Input:  function my_func() returns numeric return 123 end
Output: define i64 @my_func() { entry: ret i64 123 }
                    ^^^^^^^^                   ^^^
                    PARSED!                    PARSED!
```

**Takıldığın yer olursa sor, yoksa direkt başla!** 🚀

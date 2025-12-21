# 🚨 YZ_199: Stage 0 List Syntax Support - ACİL BLOKER!

**Tarih:** 21 Aralık 2025  
**Öncelik:** 🔴🔴🔴 EN YÜKSEK (BLOKER)  
**Süre:** 1 hafta  
**Görevli YZ:** Atanacak

---

## 🎯 PROBLEM

**Stage 1 modüller derlenemiyor!**

```bash
./compiler/stage0/modules/functions/functions_compiler \
  modules/lexer_mlp/lexer.mlp temp/test.ll --backend=llvm

# Sonuç:
19: error [Parser]: Expected ')' after parameters
📊 0 functions, 0 structs, 0 enums
```

**Kök Neden:** Stage 0 parser `()` list literal syntax'ını desteklemiyor!

---

## 📊 ETKİ ANALİZİ

- **66 Stage 1 modül** derlenemiyor
- **541 satır** `list ... = [...]` yanlış syntax (doğrusu `(...)`)
- **40 satır** `return [...]` yanlış syntax  
- **25 dosya** etkilenmiş
- **Bootstrap pipeline tamamen engelli!**

---

## 📋 PMPL KESİN SÖZDİZİMİ

**Referans:** `pmlp_kesin_sozdizimi.md` line 170, 640-642

| Tip | Syntax | Örnek |
|-----|--------|-------|
| Array | `[]` | `numeric[] arr = [1; 2; 3]` |
| **List** | **`()`** | `person() = ("Ali"; 25; true;)` |
| Tuple | `<>` | `coord<> = <10; 20>` |

**Kurallar:**
- List literal: `(elem1; elem2; elem3;)`
- Trailing semicolon ZORUNLU: `(1; 2; 3;)`
- Boş list: `()`
- Parametre ayırıcı: `;` (noktalı virgül)

---

## 🔧 GEREKLİ DEĞİŞİKLİKLER

### 1. Stage 0 Lexer
**Dosya:** `compiler/stage0/modules/lexer/lexer.c`

```c
// Mevcut: ( → TOKEN_LPAREN (sadece expression grouping)
// Gerekli: ( sonrası context'e göre list literal detection
```

### 2. Stage 0 Parser
**Dosya:** `compiler/stage0/modules/functions/functions_parser.c`

```c
// a) List literal parsing
if (tok->type == TOKEN_LPAREN) {
    // Peek: ; var mı? → List literal
    // Yoksa → Expression grouping
}

// b) Parametre ayırıcı ; desteği
// Mevcut: func(a, b, c) → virgül
// Gerekli: func(a; b; c) → semicolon
```

### 3. Stage 0 Codegen
**Dosya:** `compiler/stage0/modules/array/array_codegen.c` VEYA yeni `modules/list/`

```c
// List allocation (dynamic)
// List element assignment
// Runtime: sto_list_alloc(), sto_list_set()
```

---

## ✅ TEST CASE

```pmpl
-- Test file: temp/test_list_syntax.mlp
function create_token(numeric t; string v; numeric l; numeric c) returns list
    return (t; v; l; c;)
end_function

function test_empty() returns list
    list empty = ()
    return empty
end_function
```

**Beklenen:**
```bash
./functions_compiler temp/test_list_syntax.mlp temp/test.ll --backend=llvm
✅ Compiled temp/test_list_syntax.mlp -> temp/test.ll
📊 2 functions, 0 structs, 0 enums
```

---

## 🎯 BAŞARI KRİTERİ

```bash
# Stage 1 modül derleme testi
./compiler/stage0/modules/functions/functions_compiler \
  modules/lexer_mlp/token.mlp temp/test_token.ll --backend=llvm

# Beklenen:
📊 6 functions, 0 structs, 0 enums  # (şu an 0!)
```

---

## 📚 REFERANSLAR

1. `pmlp_kesin_sozdizimi.md` - Kesin sözdizimi kuralları
2. `kurallar_kitabı.md` line 857-898 - Koleksiyon tipleri
3. `temp/MODÜL_ANALIZ_KARŞILAŞTIRMA.md` - Modül analizi
4. `temp/melp_kritik_rapor.md` - Bloker detayları
5. `stage_0_YZ/MELP_REFERENCE.md` line 310-315 - List syntax örnekleri

---

## ⚠️ UYARILAR

1. **Matematik parantezi kırılmasın:** `(2 + 3) * 4` çalışmaya devam etmeli
2. **Virgül ondalık ayırıcı:** `3,14` bozulmamalı
3. **String içi virgül:** `"Hello, World"` korunmalı
4. **Backward compatibility:** Mevcut array `[]` syntax çalışmalı

---

## 🚀 SONRAKI ADIMLAR

YZ_199 tamamlandıktan sonra:
- ✅ YZ_200: List Type Full Implementation (operations)
- ✅ YZ_204: Module System
- ✅ Bootstrap pipeline devam edebilir!

---

**Durum:** TODO_MODERN_LANGUAGE.md'ye eklendi  
**Atama:** Bekliyor  
**Başlangıç:** ASAP (Stage 1 engelli!)

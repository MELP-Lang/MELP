# Stage0 Known Bugs ve Limitations

**Tarih:** 6 Ocak 2026 (Updated)  
**Durum:** Stage0 (C-based compiler) için dokümante edilmiş kısıtlamalar

---

## 🎯 Stage 0 Nedir?

Stage 0, **bootstrap aracı**dır - production compiler değil!  
Dünya standardı: **Minimal ama Turing-complete** alt küme.

### ✅ Desteklenenler (SADECE BUNLAR!)

**Kontrol Yapıları:**
- `if ... then ... else ... end_if` (sadece if-else, else_if YOK!)
- `while ... do ... end_while`
- `for ... do ... end_for`
- `return`

**Veri Tipleri:**
- `numeric` (double)
- `text` / `string` (const char*)
- `boolean` (bool)

**Fonksiyonlar:**
- `function name(type param) as type`
- `external function name(type param) as type`
- Multi-parameter (semicolon delimiter): `function add(numeric a; numeric b)`

**Operatörler:**
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `<`, `<=`, `>`, `>=`, `==`, `!=`
- Logical: `and`, `or`, `not`
- Assignment: `=`

**Syntax Kuralları:**
- Parameter syntax: `type name` (NOT `name: type`!) ⚠️ PMLP0 is type-first!
- Statement terminator: `;` (ONLY for separators, NOT statement terminator!)
- Comments: `--` (line comment)

**⚠️ CRITICAL:** Stage 0 uses PMLP0 syntax (type-first). See: `belgeler/language/pmlp0_syntax.md`

### ❌ Desteklenmeyenler

- `else_if` (BROKEN! - sadece nested if-else kullan)
- `switch` / `case`
- Arrays (Stage 0'da yok!)
- Structs / Records
- Pointers (C pointer'ları external'da kullanılabilir)
- String concatenation (`+` operator strings için YOK!)
- Type inference (her değişken explicit type gerektirir)

---

## 1. Multi-String Parameter Bug

### Problem
External function'larda **"text"** kelimesi parametre adı olarak kullanılamaz.

### Sebep
Stage0 lexer'ında "text" kelimesi özel bir token olarak işleniyor (muhtemelen eski bir keyword kalıntısı).

### Belirti
```mlp
-- ❌ BUG (also wrong syntax!)
external function test(numeric id; string text) as numeric
-- Correct syntax but 'text' keyword bug exists!
```

Stage0 şunu generate ediyor:
```c
extern double test(double id, const char*, const char*);
// "text" parametresi 2 parametre olarak görülüyor!
```

GCC hatası:
```
error: too few arguments to function 'test'
```

### Çözüm
"text" yerine başka isim kullan:
```mlp
-- ✅ FIX (correct PMLP0 syntax + avoid 'text' keyword)
external function test(numeric id; string input) as numeric
external function test(numeric id; string txt) as numeric
external function test(numeric id; string content) as numeric
```

### Test Edilen Kelimeler

| Kelime | Durum | Not |
|--------|-------|-----|
| `text` | ❌ BUG | Kullanma! |
| `txt` | ✅ OK | Güvenli |
| `input` | ✅ OK | Önerilen |
| `output` | ✅ OK | OK |
| `value` | ✅ OK | OK |
| `val` | ✅ OK | OK |
| `content` | ✅ OK | OK |
| `name` | ✅ OK | OK |
| `type` | ✅ OK | OK |

---

## 2. else_if Statement BROKEN

### Problem
`else_if` keyword lexer'da tanımlı ama **parser'da çalışmıyor!**

### Belirti
```mlp
if x > 10 then
    return 1;
else_if x > 5 then  -- ❌ Parse Error!
    return 2;
end_if
```

Parse hatası:
```
[4:15] Error at 'x': Expected expression.
```

### Sebep
Parser line 329'da `parser->current.type = TOKEN_IF` trick'i çalışmıyor.  
Token type değiştirme, condition'ın ilk token'ını bozuyor.

### ✅ Çözüm: Flatten if-else Pattern

**Yerine bunu kullan:**
```mlp
-- Early return pattern (WORKS!)
if x > 10 then
    return 1;
end_if

if x > 5 then
    return 2;
end_if

if x > 0 then
    return 3;
end_if

return 0;  -- default
```

veya **nested if-else:**
```mlp
if x > 10 then
    return 1;
else
    if x > 5 then
        return 2;
    else
        if x > 0 then
            return 3;
        else
            return 0;
        end_if
    end_if
end_if
```

### Dünya Standardı
Çoğu minimal compiler (tcc, rust-0.1, go-1.4) **else_if desteklemez!**  
Bu sugar syntax, Stage 1+'da eklenecek özellik.

---

## 3. Compiler Memory Limit

### Problem
Büyük .mlp dosyaları (>1500 byte karmaşık kod) Stage0'ın arena allocator'ını taşırıyor.

### Belirti
```
StringBuilder: Failed to grow capacity to 1963821
Arena: Out of memory (requested 1963824, available 91088)
Parçalama arızası (core dumped)
```

### Çözüm
- Fonksiyonları küçük modüllere böl
- Çok fazla external function tanımı yapma (her biri memory kullanıyor)
- Basit syntax kullan (complex expressions memory yiyor)

---

## 4. Return Type Limitations

### Problem
`void` return type Stage0'da çalışmıyor, `extern ''` generate ediyor.

### Çözüm
Her zaman `as numeric` kullan, C tarafında 0.0 return et:
```mlp
external function cleanup(handle: numeric) as numeric  -- void yerine
```

```c
double cleanup(double handle) {
    // cleanup işlemi...
    return 0.0;  // void yerine 0 dön
}
```

---

## 5. String Literal Sınırlamaları

### Problem
Çok uzun string literaller (>256 char) Stage0'da problem olabilir.

### Çözüm
String'leri parçalara böl veya C runtime'dan döndür:
```mlp
-- ❌ Çok uzun
string json = "{\"very\":\"long\",\"json\":\"object\"...}"

-- ✅ C'den döndür
external function get_config_json() as string
```

---

## Workaround Strategy

Stage0 production compiler değil, **bootstrap aracı**. Limitations normal.

**Stage2'de fixed olacaklar:**
- ✅ "text" keyword conflict → Lexer rewrite'da çözülecek
- ✅ else_if support → Parser rewrite'da eklenecek
- ✅ Memory limits → Modern allocator ile çözülecek
- ✅ void return → Proper type system
- ✅ String limits → Dynamic string handling

**Şimdilik:**
- Bu limitlerle yaşa
- **else_if KULLANMA!** (flatten if-else pattern kullan)
- STO-compliant kod yaz
- Stage1'i Stage2 için hazırla

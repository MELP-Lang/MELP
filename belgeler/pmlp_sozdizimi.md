# PMPL KESİN SÖZDİZİMİ REFERANSI

**Kaynak:** PMPL_SYNTAX.md (ana referans)  
**Tarih:** 15 Ocak 2026 (Updated: pmlp0 referansına göre güncellendi)  
**Durum:** MELP'in TEK ve KESİN sözdizimi referansı  
**Versiyon:** 2.1 (Semicolon kuralı düzeltildi)  
**✅ REFERANS:** [pmlp0_sozdizimi.md](pmlp0_sozdizimi.md) - Stage 0 Minimal Spec

---

## 📑 İÇİNDEKİLER (INDEX)

> **YZ için not:** Bu belge 1000+ satır. Sadece ihtiyacın olan bölümü oku!

### 🏷️ Stage İşaretleri (Feature Status)
- **✅ Stage 0** - Şu anda implemente edilmiş (temel özellikler)
- **⚠️ Stage 1** - Planlanan gelişmiş özellikler (self-hosting)
- **🚀 Stage 2-3** - Gelecek versiyon özellikleri (experimental)
- **❌ Deprecated** - MELP felsefesiyle çelişen, kullanılmayacak özellikler

### 🎯 Hızlı Başlangıç (Herkes Okusun)
- [📖 PMPL Nedir?](#-pmpl-nedir) - PMPL'in ne olduğu (satır 10)
- [⚠️ Kritik Kurallar](#️-kritik-kurallar) - En önemli 3 kural (satır 21)

### 🔤 Temel Syntax (Migration İçin Kritik) - ✅ Stage 0
- [1. Blok Sonlandırıcılar](#1-blok-sonlandırıcılar-alt-çizgili---tek-token) - `end_if`, `end_while`, vb. (satır 36)
- [2. Kontrol Akışı](#2-kontrol-akışı) - `if-then`, `else_if` (satır 62)
- [3. Döngü Çıkış/Devam](#3-döngü-çıkışdevam-komutları) - `exit`, `continue` (satır 96)
- [4. Temel Tipler](#4-temel-tipler) - `numeric`, `string`, `boolean` (satır 140)
- [5. Koleksiyon Tipleri](#5-koleksiyon-tipleri) - Array `[]`, List `()`, Tuple `<>` (satır 186)
- [12. Yorum Satırları](#12-yorum-satırları) - `--` ve `---` (satır 288)

### ⚙️ Operatörler - ✅ Stage 0
- [6. Mantıksal Operatörler](#6-mantıksal-operatörler) - `and`, `or`, `not` (satır 208)
- [7. Karşılaştırma](#7-karşılaştırma-operatörleri) - `==`, `!=`, `<`, `>` (satır 217)
- [8. Aritmetik](#8-aritmetik-operatörler) - `+`, `-`, `*`, `/` (satır 228)
- [9. Sınırlayıcılar](#9-sınırlayıcılar) - `;`, `:`, `.` (satır 240)
- [10. Döngü Keywords](#10-temel-döngü-keywordleri) - `for`, `while`, `in` (satır 257)

### 🔧 Fonksiyon ve Parametreler (Core Libs İçin)
- [Fonksiyon Tanımı](#fonksiyon-tanımı) - Temel syntax (satır 317) - ✅ Stage 0
- [Varsayılan Değerli Parametreler](#varsayılan-değerli-parametreler-) - `:=` (satır 337) - ⚠️ Stage 1
- [Referans Parametreler](#referans-ve-sabit-parametreler-refconst) - `ref`, `const` (satır 358) - ⚠️ Stage 1
- [Variadic Parametreler](#variadic-parametreler-) - `...` (satır 389) - ⚠️ Stage 1
- [Named Parameters](#named-parameters-i̇simli-parametreler) - `name: value` (satır 415) - ⚠️ Stage 1
- [Output Parametresi](#output-parametresi-out) - `out` (satır 447) - ⚠️ Stage 1

### 📦 Struct ve Enum (Parser/Codegen İçin) - ⚠️ Stage 1
- [Struct Tanımı](#struct-tanımı) - Yapılar (satır 565)
- [Enum Tanımı](#enum-tanımı) - Numaralandırmalar (satır 580)

### � MODERN Phase Features (Week 1-2)
- [Import/Export & Module System](#importexport--module-system) - Module imports (satır 560) - ✅ MODERN Week 1 Day 3
- [Generic/Template](#generictemplate-parametreler-t) - `{T}` (satır 640) - ✅ MODERN Week 1
- [Lambda Functions](#lambdaanonymous-functions) - Closures (satır 1050) - ✅ MODERN Week 2
- [Pattern Matching](#matchpattern-matching) - Match/Result{T,E} (satır 990) - ✅ MODERN Week 2
- [Channels & Concurrency](#channels--concurrency-week-2) - Channel{T}, spawn (satır 1530) - ✅ MODERN Week 2
- [Result{T,E} & Option{T}](#resultt-e-type-week-1-generics--week-2-pattern) - Error types (satır 1580) - ✅ MODERN Week 2
- [Error Handling](#error-handling-week-2) - Try/catch (satır 1640) - ✅ MODERN Week 2

### 🔄 Kontrol Yapıları (Compiler İçin)
- [If-Else-If](#if-else-if) - Koşullar (satır 608) - ✅ Stage 0
- [While Döngüsü](#while-döngüsü) - While (satır 620) - ✅ Stage 0
- [For Döngüsü](#for-döngüsü) - For (satır 628) - ✅ Stage 0
- [For-Each](#for-each-döngüsü) - Iterator (satır 862) - ⚠️ Stage 1
- [Switch-Case](#switch-case) - Switch (satır 636) - ⚠️ Stage 1

### 🚀 Gelişmiş Özellikler (İhtiyaç Varsa)
- [Union Tipi](#unioneither-tipi-) - `numeric | string` (satır 508) - 🚀 Stage 2-3
- [Try-Catch-Finally](#try-catch-finally-hata-yönetimi) - Exception handling (satır 765) - 🚀 Stage 2-3
- [Operator Overloading](#operator-overloading) - Özel operatörler (satır 813) - 🚀 Stage 2-3
- [Pointer Syntax](#pointer-syntax) - İşaretçiler (satır 746) - ❌ Deprecated
- [Null/Nullable](#nullnullable-tipler) - Null handling (satır 915) - 🚀 Stage 2-3

### 📋 Referans (Hızlı Bakış)
- [Tam Token Listesi](#-tam-token-listesi) - Tüm tokenler (satır 977)
- [Parser İçin Önemli](#-parser-için-önemli) - Parser notları (satır 967)

---

### 💡 Kullanım Önerileri

**Syntax Migration için:**
→ Temel Syntax bölümünü oku (1-12)

**Core Libraries için:**
→ Fonksiyon ve Parametreler bölümünü oku

**Compiler yazmak için:**
→ Kontrol Yapıları ve Struct/Enum bölümlerini oku

**Test yazmak için:**
→ Sadece Hızlı Başlangıç yeterli

---

## 📖 PMPL Nedir?

**PMPL (Pragmatic MLP)**, MELP derleyicisinin gördüğü dahili temsil dilidir.

```
[Kullanıcı Kodu] → [Normalizer] → [PMPL] → [Lexer] → [Parser] → [Codegen]
      (any)        (syntax.json)   (bu!)    (tokens)   (AST)     (assembly)
```

**Temel Prensip:** MELP derleyicisi **SADECE PMPL** görür.

### ⚠️ KRİTİK KURALLAR

1. **Parametre Ayırıcı:** Her yerde `;` (noktalı virgül) kullanılır
   - Neden? MELP'te `345,95` ondalık sayıdır (Türk formatı)
   - Örnek: `func(345,95; true; "OK")`

2. **Blok Sonlandırıcı:** Alt çizgili tek token (`end_if`, `end_while`)

3. **Exit/Continue:** Hem standalone hem explicit versiyonlar desteklenir
   - Standalone: `exit`, `continue` (mevcut bloktan)
   - Explicit: `exit_for`, `exit_while`, `exit_if`, `continue_for`, `continue_while`

4. **Deprecated Keywords:** `returns`, `%`, `text` kullanılmaz
   - Sadece: `as`, `mod`, `string`

---

## ✅ TÜM BELGELERDE TUTARLI SYNTAX

### 1. Blok Sonlandırıcılar (Alt Çizgili - Tek Token)

| PMPL Keyword | Token | Kullanım |
|--------------|-------|----------|
| `end_if` | TOKEN_END_IF | If bloğu sonu |
| `end_while` | TOKEN_END_WHILE | While döngüsü sonu |
| `end_for` | TOKEN_END_FOR | For döngüsü sonu |
| `end_function` | TOKEN_END_FUNCTION | Fonksiyon sonu |
| `end_struct` | TOKEN_END_STRUCT | Struct sonu |
| `end_enum` | TOKEN_END_ENUM | Enum sonu |
| `end_switch` | TOKEN_END_SWITCH | Switch sonu |
| `end_match` | TOKEN_END_MATCH | Match sonu |
| `end_try` | TOKEN_END_TRY | Try-catch sonu |
| `end_debug` | TOKEN_END_DEBUG | Debug bloğu sonu |
| `end_operator` | TOKEN_END_OPERATOR | Operator tanımı sonu |

**Örnek:**
```pmpl
function test() as numeric
    if x > 5 then
        return 1
    end_if
    return 0
end_function
```

### 2. Kontrol Akışı

| PMPL Keyword | Token | Kullanım |
|--------------|-------|----------|
| `if` | TOKEN_IF | If başlangıcı |
| `then` | TOKEN_THEN | If koşulu sonu |
| `else_if` | TOKEN_ELSE_IF | Else-if cümlesi |
| `else` | TOKEN_ELSE | Else cümlesi |

**⚠️ KRİTİK KURAL: `if` HER ZAMAN `then` İLE BİTMELİ!**

```pmpl
-- DOĞRU:
if x > 5 then
    ...
end_if

-- YANLIŞ:
if x > 5
    ...
end_if
```

**Örnek:**
```pmpl
if x > 100 then
    print("large")
else_if x > 50 then
    print("medium")
else
    print("small")
end_if
```

### 3. Döngü Çıkış/Devam Komutları

**⚠️ KRİTİK: `break` YOK! `exit` kullan! `continue` opsiyonel.**

**Standalone (Genel):**

| PMPL Keyword | Token | Kullanım |
|--------------|-------|----------|
| `exit` | TOKEN_EXIT | Mevcut bloktan çık (⚠️ break değil!) |
| `continue` | TOKEN_CONTINUE | Mevcut döngüye devam (opsiyonel, hata vermez) |

**Specific (Belirli Blok):**

| PMPL Keyword | Token | Kullanım |
|--------------|-------|----------|
| `exit_for` | TOKEN_EXIT_FOR | For döngüsünden çık |
| `exit_while` | TOKEN_EXIT_WHILE | While döngüsünden çık |
| `exit_if` | TOKEN_EXIT_IF | If bloğundan çık |
| `exit_function` | TOKEN_EXIT_FUNCTION | Fonksiyondan çık |
| `exit_switch` | TOKEN_EXIT_SWITCH | Switch'ten çık |
| `continue_for` | TOKEN_CONTINUE_FOR | For döngüsüne devam |
| `continue_while` | TOKEN_CONTINUE_WHILE | While döngüsüne devam |

> 📝 **L-10:** `exit_function` anahtar kelimesi lexer'a eklendi (Phase 2 L-10).

**Örnek:**
```pmpl
while i < 100
    if i mod 2 == 0 then
        continue            -- Standalone: mevcut döngüye devam
    end_if
    if i > 50 then
        exit                -- Standalone: mevcut döngüden çık
    end_if
    i = i + 1
end_while

-- Veya explicit versiyonlar:
for j = 0 to 99
    if arr[j] < 0 then
        exit_for            -- Explicit: for döngüsünden çık
    end_if
    if arr[j] == 0 then
        continue_for        -- Explicit: for döngüsüne devam
    end_if
end_for
```

### 4. Temel Tipler

| Tip | Token | Açıklama |
|-----|-------|----------|
| `numeric` | TOKEN_NUMERIC | Tüm sayılar (tam, ondalık) |
| `string` | TOKEN_TEXT | Metinler (UTF-8) |
| `boolean` | TOKEN_BOOLEAN | true/false |

**⚠️ NOT:** Sadece `string` kullanılır. `text` deprecated.

**Türk Sayı Formatı:**
```pmpl
numeric x = 126.555.852,36        -- 126,555,852.36 (Türk formatı)
numeric y = 3,14159               -- 3.14159
numeric z = 1.000.000             -- 1,000,000 (bir milyon)
```

| Format | Anlam | Açıklama |
|--------|-------|----------|
| `.` (nokta) | Binlik ayırıcı | 1.000 = bin |
| `,` (virgül) | Ondalık ayırıcı | 3,14 = 3.14 |

**⚠️ KRİTİK: Noktalı Virgül Kuralı**
```pmpl
-- ✅ REFERANS: pmlp0_sozdizimi.md (Stage 0 Minimal Spec)
-- KURAL: Semicolon (;) SADECE AYIRICI (parametre, array/list elemanları)

-- Değişken tanımlama (semicolon YOK)
numeric x = 42
string name = "Ali"
boolean flag = true

-- Atama (semicolon YOK)
x = 50
name = "Veli"
flag = false

-- ❌ YANLIŞ: Satır sonu semicolon
numeric y = 10;    -- YANLIŞ!
```

### 5. Koleksiyon Tipleri

| Tip | Token | Syntax | Literal Örnek | Açıklama |
|-----|-------|--------|---------------|----------|
| Array | TOKEN_ARRAY | `[]` | `numeric[] arr = [1; 2; 3]` | Homojen (tek tip), mutable, tip bildirimli |
| List | TOKEN_LIST | `()` | `list data = (1; "x"; true)` | Heterojen (çoklu tip), mutable, tip yok |
| Tuple | TOKEN_TUPLE | `<>` | `tuple<> pos = <10; 20>` | Heterojen, immutable |

> 📝 **L-09:** `list` ve `tuple` anahtar kelimeleri lexer'a eklendi (Phase 2 L-09 — codegen Phase 3'te gelecek).

**⚠️ KRİTİK KURALLAR:**
- **Ayırıcı:** HER YERDE `;` (noktalı virgül) - Array, List, Tuple HEPSI!
- **Trailing semicolon:** YASAK - `[3; 5; 0]`, `(3; 5; 0)`, `<3; 5; 0>`
- **Virgül (`,`):** SADECE ondalık sayılarda: `3,14` (Türk formatı)
- **Array:** Tip bildirimi ZORUNLU (`numeric[]`), sadece o tip
- **List:** Tip bildirimi YOK (`list`), her tip olabilir

**Örnekler:**
```pmpl
numeric[] arr = [3; 5; 0]        -- Array
list data = (3; 5; 0)            -- List
tuple<> pos = <3; 5; 0>          -- Tuple
```

### 6. Mantıksal Operatörler

| PMPL | Token | Açıklama |
|------|-------|----------|
| `and` | TOKEN_AND | Mantıksal VE |
| `or` | TOKEN_OR | Mantıksal VEYA |
| `not` | TOKEN_NOT | Mantıksal DEĞİL |
| `xor` | TOKEN_XOR | Bitwise XOR |

> 📝 **L-10:** `xor` anahtar kelimesi lexer'a eklendi (Phase 2 L-10).

### 7. Karşılaştırma Operatörleri

| Sembol | Token | Açıklama |
|--------|-------|----------|
| `==` | TOKEN_EQUAL | Eşitlik |
| `!=` | TOKEN_NOT_EQUAL | Eşitsizlik |
| `<` | TOKEN_LESS | Küçüktür |
| `>` | TOKEN_GREATER | Büyüktür |
| `<=` | TOKEN_LESS_EQUAL | Küçük eşit |
| `>=` | TOKEN_GREATER_EQUAL | Büyük eşit |

### 8. Aritmetik Operatörler

| Sembol | Token | Açıklama |
|--------|-------|----------|
| `+` | TOKEN_PLUS | Toplama |
| `-` | TOKEN_MINUS | Çıkarma |
| `*` | TOKEN_MULTIPLY | Çarpma |
| `/` | TOKEN_DIVIDE | Bölme |
| `mod` | TOKEN_MOD | Modulo |
| `**` | TOKEN_POWER | Üs alma |
| `=` | TOKEN_ASSIGN | Atama |

> 📝 **L-10:** `**` (TOKEN_POWER) operatörü lexer'a eklendi (Phase 2 L-10 — scanToken'da `*` lookahead ile).

**⚠️ NOT:** Sadece `mod` kullanılır. `%` deprecated.

### 9. Sınırlayıcılar

| Sembol | Token | Açıklama |
|--------|-------|----------|
| `(` | TOKEN_LPAREN | Sol parantez |
| `)` | TOKEN_RPAREN | Sağ parantez |
| `[` | TOKEN_LBRACKET | Sol köşeli (array) |
| `]` | TOKEN_RBRACKET | Sağ köşeli |
| `;` | TOKEN_SEMICOLON | **PARAMETRE/ELEMAN AYIRICI** |
| `:` | TOKEN_COLON | Tip ayırıcı |
| `.` | TOKEN_DOT | Üye erişimi |

**⚠️ ÖNEMLİ:** MELP'te `,` (virgül) ondalık ayırıcıdır (345,95 = 345.95)
- Fonksiyon parametreleri: `func(a; b; c)`
- Liste elemanları: `(10; 20; 30)`
- Array elemanları: `[1; 2; 3]`

### 10. Temel Döngü Keyword'leri

| PMPL | Token | Açıklama |
|------|-------|----------|
| `for` | TOKEN_FOR | For döngüsü |
| `to` | TOKEN_TO | For artan |
| `downto` | TOKEN_DOWNTO | For azalan |
| `while` | TOKEN_WHILE | While döngüsü |
| `in` | TOKEN_IN | For-each iteratörü |
| `each` | TOKEN_EACH | For-each |

### 11. Diğer Keyword'ler

| PMPL | Token | Açıklama |
|------|-------|----------|
| `func| TOKEN_AS | Dönüş tipi |
| `struct` | TOKEN_STRUCT | Struct tanımı |
| `enum` | TOKEN_ENUM | Enum tanımı |
| `import` | TOKEN_IMPORT | Modül içe aktarma |
| `module` | TOKEN_MODULE | Modül tanımı |
| `print` | TOKEN_PRINT | Yazdır |
| `println` | TOKEN_PRINTLN | Satır sonu ile yazdır |
| `true` | TOKEN_TRUE | Boolean doğru |
| `false` | TOKEN_FALSE | Boolean yanlış |
| `if` | TOKEN_IF | If ifadesi |
| `then` | TOKEN_THEN | Koşul sonu işareti |
| `switch` | TOKEN_SWITCH | Switch ifadesi |
| `debug` | TOKEN_DEBUG | Debug bloğu |

**⚠️ NOT:** Sadece `as` kullanılır. `returns` deprecated.eti |
| `switch` | TOKEN_SWITCH | Switch ifadesi |
| `debug` | TOKEN_DEBUG | Debug bloğu |

### 12. Yorum Satırları

```pmpl
-- Tek satır yorum

---
Çok satırlı yorum
Birden fazla satır
---

---/
Dokümantasyon yorumu (Help, IDE tooling için)
@param x: İlk parametre
@return: Hesaplanan sonuç
/---

numeric x = 10  -- Satır sonu yorumu
```

**⚠️ NOT:** 
- Tek satır: `--` (iki tire)
- Çok satırlı: `---` ... `---` (üç tire, simetrik)
- Dokümantasyon: `---/` ... `/---` (slash yönü tip belirtir)
- `//` ve `#` KULLANILMAZ!

---

## 📋 TUTARLI ÖZELLİKLER

### Fonksiyon Tanımı

```pmpl
function hesapla(numeric x; numeric y) as numeric
    return x + y
end_function

function mesafe(Point p1; Point p2) as numeric
    numeric dx = p2.x - p1.x
    numeric dy = p2.y - p1.y
    return (dx * dx + dy * dy) ** 0,5
end_function
```

**Not:** 
- Parametre formatı: `tip isim` (örn: `numeric x`)
- Parametre ayırıcı: `;` (noktalı virgül)
- Return type: Sadece `as` kullanılır (`returns` deprecated)
  - `function foo() as numeric` ✅
  - `function foo() returns numeric` ❌ DEPRECATED
- Ondalık sayı: `0,5` (Türk formatı)

### Varsayılan Değerli Parametreler (`:=`)

```pmpl
-- Varsayılan değer tanımı: tip isim := değer
function greet(string name := "World") as string
    return "Hello, " + name
end_function

greet()              -- "Hello, World"
greet("MELP")        -- "Hello, MELP"

-- Çoklu varsayılan değer
function connect(string host; numeric port := 80; numeric timeout := 30) as Connection
    -- bağlantı kodu
end_function

connect("localhost")              -- port=80, timeout=30
connect("localhost"; 8080)        -- port=8080, timeout=30
connect("localhost"; 8080; 60)    -- port=8080, timeout=60
```

### Referans ve Sabit Parametreler (ref/const)

```pmpl
-- Referans ile geçirme (orijinal değişir): ref tip isim
function swap(ref numeric a; ref numeric b) as none
    numeric temp = a
    a = b
    b = temp
end_function

numeric x = 5
numeric y = 10
swap(x; y)           -- x=10, y=5 olur

-- Sabit parametre (değiştirilemez): const tip isim
function calculate(const numeric x) as numeric
    -- x = 100       -- HATA! const değiştirilemez
    return x * 2
end_function

-- Sabit referans (okuma için referans): const ref tip isim
function print_large(const ref string text) as none
    print(text)      -- Kopyalanmadan okunur
end_function
```

| Keyword | Token | Açıklama |
|---------|-------|----------|
| `ref` | TOKEN_REF | Referans ile geçir |
| `const` | TOKEN_CONST | Salt okunur |

### Variadic Parametreler (`...`)

```pmpl
-- Sıfır veya daha fazla parametre: tip... isim
function sum(numeric... numbers) as numeric
    numeric total = 0
    for each n in numbers
        total = total + n
    end_for
    return total
end_function

sum()                -- 0
sum(1; 2; 3)         -- 6
sum(1; 2; 3; 4; 5)   -- 15

-- Bir veya daha fazla: tip+ isim
function min(numeric+ values) as numeric
    -- En az bir değer zorunlu
end_function

min(5)               -- 5
min(3; 1; 4)         -- 1
-- min()             -- HATA! En az bir değer gerekli
```

### Named Parameters (İsimli Parametreler)

```pmpl
-- İsimli parametre tanımı: named tip isim
function create_user(named string name; named numeric age; string role := "user") as User
    -- kullanıcı oluştur
end_function

-- Çağrı: isim := değer (named call syntax)
create_user(name := "Ali"; age := 25)
create_user(age := 30; name := "Veli")                    -- Sıra önemli değil
create_user(name := "Ayşe"; age := 28; role := "admin")
```

### Positional Optional (`[]`)

```pmpl
-- Köşeli parantez ile opsiyonel parametreler: [tip isim]
function format(string text; [string prefix := ""]; [string suffix := ""]) as string
    return prefix + text + suffix
end_function

format("MELP")                        -- "MELP"
format("MELP"; "[")                   -- "[MELP"
format("MELP"; "["; "]")              -- "[MELP]"

-- Karışık kullanım
function query(string table; [string where]; [numeric limit := 100]) as Result
    -- sorgu kodu
end_function
```

### Output Parametresi (`out`)

```pmpl
-- Birden fazla değer döndürme: out tip isim
function divide(numeric a; numeric b; out numeric result; out boolean success) as none
    if b == 0 then
        success = false
        result = 0
    else
        success = true
        result = a / b
    end_if
end_function

-- Kullanım
numeric sonuc
boolean basarili
divide(10; 3; sonuc; basarili)
print(sonuc)         -- 3,333...
print(basarili)      -- true

-- Çoklu out örneği: out tip isim
function parse_date(string s; out numeric year; out numeric month; out numeric day) as boolean
    -- parse işlemi
end_function
```

### Import/Export & Module System - ✅ MODERN Week 1 (Day 3)

> **Purpose:** Replace melp_coordinator.sh with native import system  
> **Stage:** MODERN Week 1 Day 3-4 (29-30 Ocak 2026)

```pmpl
-- Import specific symbols from module
import core::io::print
import core::math::sqrt
import core::collections::List{T}

-- Import entire module
import core::string as str
import user::geometry

-- Import with alias
import very::long::module::name as short

-- Multiple imports from same module
import core::math::{sqrt, pow, abs}

-- Export declarations (make visible to importers)
export struct Point{T}
    T x
    T y
end_struct

export function distance{T}(Point{T} p1; Point{T} p2) as T
    T dx = p1.x - p2.x
    T dy = p1.y - p2.y
    return sqrt(dx * dx + dy * dy)
end_function

-- Module definition (optional, for organization)
module geometry
    export struct Point{T}
        T x
        T y
    end_struct
    
    export function distance{T}(Point{T} p1; Point{T} p2) as T
        -- implementation
    end_function
    
    -- Private (not exported)
    function internal_helper() as numeric
        -- helper function
    end_function
end_module

-- Module search paths
-- 1. Current directory
-- 2. ./modules/
-- 3. MELP_MODULE_PATH environment variable
-- 4. Built-in modules (core::*)

-- Built-in core modules:
-- core::io       - print, read, file operations
-- core::math     - sqrt, pow, sin, cos, etc.
-- core::string   - string manipulation
-- core::collections - List{T}, Map{K,V}, Set{T}
-- core::result   - Result{T,E}, Option{T}
-- core::channel  - Channel{T}, spawn, select
```

**Tokens:** `TOKEN_IMPORT` | `TOKEN_EXPORT` | `TOKEN_MODULE` | `TOKEN_AS`

**Import Foundation Features:**
- Dependency graph resolution (no circular imports)
- Build order calculation (topological sort)
- Symbol table management (per-module namespaces)
- Peer-to-peer module loading (no central coordinator)

---

### Generic/Template Parametreler (`{T}`) - ✅ MODERN Week 1

> **PMLP Syntax:** Generic types use `{}` braces (users never see PMLP)
> **Stage:** MODERN Week 1 (27-30 Ocak 2026)

```pmpl
-- Generic function
function first{T}(T[] items) as T
    return items[0]
end_function

numeric[] nums = [1; 2; 3]
string[] names = ["Ali"; "Veli"]

numeric n = first{numeric}(nums)      -- 1
string s = first{string}(names)       -- "Ali"

-- Multiple type parameters
function map{T, U}(T[] items; func(T) as U transformer) as U[]
    U[] result
    for each item in items
        result.add(transformer(item))
    end_for
    return result
end_function

-- Generic struct
struct Pair{K, V}
    K key
    V value
end_struct

Pair{string, numeric} p
p.key = "age"
p.value = 25

-- Generic with Result type (error handling)
struct Result{T, E}
    bool ok
    T value
    E error
end_struct

Result{numeric, string} res = divide(10, 2)
if res.ok then
    print(res.value)
else
    print(res.error)
end_if
```

**CRITICAL:** 
- Generic `{T}` = Type position (after struct/function name)
- Struct literal `{k: v}` = Value position with colon
- Tuple literal `<v; v>` = Value position with semicolon

### Union/Either Tipi (`|`)

```pmpl
-- Ya biri ya diğeri: tip isim
function parse(string input) as numeric | Error
    if is_valid(input) then
        return to_numeric(input)
    else
        return Error("Geçersiz giriş")
    end_if
end_function

-- Kullanım
numeric | Error result = parse("123")

match result
    case numeric n:
        print("Sayı: " + n)
    case Error e:
        print("Hata: " + e.message)
end_match

-- Çoklu union
function read_config(string key) as string | numeric | boolean | null
    -- config değeri döndür
end_function
```

### Callback/Function Parametresi

```pmpl
-- Fonksiyon tipi parametre: tip isim
function apply(numeric[] items; func(numeric) as numeric transformer) as numeric[]
    numeric[] result
    for each item in items
        result.add(transformer(item))
    end_for
    return result
end_function

-- Lambda ile kullanım
numeric[] doubled = apply([1; 2; 3]; x => x * 2)

-- İsimli fonksiyon ile kullanım
function square(numeric x) as numeric
    return x * x
end_function

numeric[] squares = apply([1; 2; 3]; square)

-- Void callback (action)
function on_complete(action callback) as none
    -- işlem bitti
    callback()
end_function
```

### Struct Tanımı

```pmpl
struct Point
    numeric x
    numeric y
end_struct

struct Person
    string name
    numeric age
    boolean active
end_struct
```

### Enum Tanımı

```pmpl
enum Color
    RED
    GREEN
    BLUE
end_enum

enum Status
    PENDING
    ACTIVE
    COMPLETED
    FAILED
end_enum

-- Enum kullanımı:
function check_status(Status s) as string
    if s == Status.ACTIVE then
        return "Running"
    else_if s == Status.COMPLETED then
        return "Done"
    else
        return "Other"
    end_if
end_function
```

### If-Else-If

```pmpl
if koşul1 then
    ifade1
else_if koşul2 then
    ifade2
else
    ifade3
end_if
```

### While Döngüsü

**⚠️ KRİTİK: `while ... do` YASAK! Sadece `while ... end_while` kullan!**

```pmpl
-- ✅ DOĞRU:
while koşul
    ifade
end_while

-- ❌ YANLIŞ:
while koşul do    -- 'do' YASAK!
    ifade
end_while
```

### For Döngüsü

```pmpl
for i = 0 to 10
    ifade
end_for
```

### Switch-Case

```pmpl
switch değer
    case 1:
        ifade1
    case 2:
        ifade2
    default:
        varsayılan
end_switch
```

### Debug Bloğu

```pmpl
-- Block debug (çok satırlı)
debug
    print("Debug info")
    numeric step = 0
    
    start:              -- label (sadece debug içinde)
    step = step + 1
    if step < 3 then
        goto start      -- goto (sadece debug içinde)
    end_if
    pause               -- pause (sadece debug içinde)
end_debug

-- Single-line debug (tek satır)
debug print("x = " + x)
debug if a == b then c = d
```

**Debug-Only Keywords:**
- `goto` - Sadece debug context'inde
- `label:` - Sadece debug context'inde
- `pause` - Sadece debug context'inde

**Compiler Davranışı:**
- Development: Debug çalışır
- Production (`--release`): Debug tamamen silinir (sıfır overhead)

### Array/List/Tuple Kullanımı

```pmpl
-- Array: [] - Homojen, mutable, tip bildirimli
numeric[] numbers = [1; 2; 3; 4; 5]  -- ✅ Trailing ; YASAK
numbers[0] = 100

-- List: () - Heterojen, mutable
list person = ("Ali"; 25; true)  -- ✅ Farklı tipler: string, numeric, boolean
person[0] = "Veli"

-- Tuple: <> - Heterojen, immutable (MODERN Week 1)
tuple coords = <10; 20; 30>  -- ✅ Trailing ; YASAK
-- coords[0] = 99  -- ❌ HATA! Immutable

-- Multiple return with tuple
function get_dimensions() as tuple
    return <width; height; depth>
end_function

tuple dims = get_dimensions()
numeric w = dims[0]

-- List literal (return)
function get_data() as list
    return (1; "test"; 3,14)  -- ✅ Trailing ; YASAK
end_function

-- Boş koleksiyonlar
numeric[] empty_arr = []           -- ✅ Boş array
list empty_list = ()               -- ✅ Boş list
tuple<> empty_tuple = <>           -- ✅ Boş tuple

-- ⚠️ KRİTİK: Ayırıcı HER YERDE ; (ama son eleman sonrası YASAK)
numeric[] a = [3; 5; 0]           -- ✅ DOĞRU
list b = (3; 5; 0)                -- ✅ DOĞRU
tuple<> c = <3; 5; 0>             -- ✅ DOĞRU

numeric[] wrong = [3, 5, 0]        -- ❌ YANLIŞ! Virgül kullanılmış
```

-- Fonksiyondan tuple döndürme
function minmax(numeric[] arr) as tuple<>
    return <min(arr); max(arr)>  -- ✅ Trailing ; YASAK
end_function
```

---

## 🆕 GELİŞMİŞ ÖZELLİKLER

### Ternary Operatör

```pmpl
-- Koşullu ifade (C stili)
result = a > b ? a : b;

-- Örnek kullanım
max_value = x > y ? x : y;
status = is_valid ? "OK" : "ERROR";
```

### Pointer Syntax

> ⚠️ **❌ DEPRECATED - KULLANILMAYACAK**  
> Bu özellik MELP felsefesiyle çelişmektedir. MELP, bellek güvenliği için pointer yerine **referans parametreler** (`ref`) ve **Token Borrowing** (Stage 1) kullanır.  
> **Neden kaldırıldı:** Raw pointer'lar memory safety garantisini bozar.  
> **Alternatif:** `ref` parametreler (Stage 1) veya managed references (Stage 2)

```pmpl
-- ❌ KULLANMAYIN - Bu syntax DEPRECATED
-- Pointer tanımı
numeric* ptr;

-- Adres alma
ptr = &x;

-- Dereference (değere erişim)
numeric value = *ptr;

-- Örnek
numeric x = 42;
numeric* p = &x;
print(*p);              -- 42 yazdırır
*p = 100;               -- x artık 100
```

### Try-Catch-Finally (Hata Yönetimi)

```pmpl
try
    numeric result = 10 / 0
    file = open("dosya.txt")
catch DivisionError e
    print("Bölme hatası: " + e.message)
catch FileError e
    print("Dosya hatası: " + e.message)
catch e
    print("Genel hata: " + e.message)
finally
    print("Her zaman çalışır")
end_try

-- Hata fırlatma
throw RuntimeError("Bir şeyler ters gitti")
```

### Match/Pattern Matching (🚀 MODERN Week 2)

**Status:** Week 2 implementation (YZ_MODERN_02, pattern_compiler)  
**Dependencies:** Generic types Week 1 (Option{T}, Result{T,E})

```pmpl
-- Basit değer eşleştirme
match value
    case 1:
        print("Bir")
    case 2; 3; 4:                    -- Çoklu değer
        print("İki, üç veya dört")
    case 10 to 20:                   -- Aralık
        print("10-20 arası")
    case _:                          -- Wildcard (default)
        print("Diğer")
end_match

-- Struct destructuring
match point
    case Point(0; 0):
        print("Orijin")
    case Point(x; 0):
        print("X ekseninde: " + x)
    case Point(0; y):
        print("Y ekseninde: " + y)
    case Point(x; y):
        print("Genel nokta")
end_match

-- Result{T,E} pattern matching (MODERN)
function safe_divide(numeric a; numeric b) as Result{numeric, string}
    if b == 0 then
        return Err("Division by zero")
    else
        return Ok(a / b)
    end_if
end_function

Result{numeric, string} res = safe_divide(10, 2)
match res
    case Ok(value):
        print("Sonuç: " + value)
    case Err(error):
        print("Hata: " + error)
end_match

-- Option{T} pattern matching (MODERN)
Option{numeric} maybe_value = Some(42)
match maybe_value
    case Some(x):
        print("Değer var: " + x)
    case None:
        print("Değer yok")
end_match
```

**Tokens:** `TOKEN_MATCH` | `TOKEN_CASE` | `TOKEN_END_MATCH`

### Operator Overloading

```pmpl
struct Vector
    numeric x
    numeric y
end_struct

-- Toplama operatörü: tip isim
operator +(Vector a; Vector b) as Vector
    Vector result
    result.x = a.x + b.x
    result.y = a.y + b.y
    return result
end_operator

-- Eşitlik operatörü
operator ==(Vector a; Vector b) as boolean
    return a.x == b.x and a.y == b.y
end_operator

-- Kullanım
Vector v1
v1.x = 10
v1.y = 20
Vector v2
v2.x = 5
v2.y = 15
Vector v3 = v1 + v2              -- operator+ çağrılır
```

### Lambda/Anonymous Functions (🚀 MODERN Week 2)

**Status:** Week 2 implementation (YZ_MODERN_02, closure_compiler)  
**Dependencies:** Generic types Week 1 (function types)

```pmpl
-- Kısa syntax (tek ifade)
numeric[] doubled = map(numbers; x => x * 2)

-- Çoklu parametre
sum = reduce(numbers; (acc; x) => acc + x; 0)

-- Uzun syntax (çok satırlı)
filtered = filter(items; item =>
    if item > 10 then
        return true
    end_if
    return false
end)

-- Değişkene atama (function pointer)
func(numeric; numeric) as numeric transformer = (x; y) => x + y
numeric result = transformer(5; 3)        -- 8

-- Closure (captures outer scope)
function make_adder(numeric x) as func(numeric) as numeric
    return y => x + y     -- x captured from outer scope
end_function

func(numeric) as numeric add5 = make_adder(5)
numeric result = add5(10)  -- 15

-- Generic lambda (with type inference)
auto doubled = map{numeric}([1; 2; 3]; x => x * 2)
```

**Token:** `TOKEN_LAMBDA_ARROW` (=>) | `TOKEN_END_LAMBDA` (optional end)

### For-Each Döngüsü

```pmpl
-- Array için
numeric[] numbers = [1; 2; 3; 4; 5]
for each n in numbers
    print(n)
end_for

-- List için
for each item in my_list
    print(item)
end_for

-- String karakterleri için
for each char in "MELP"
    print(char)               -- M, E, L, P
end_for
```

### Step ile For Döngüsü

```pmpl
-- Artan step
for i = 0 to 100 step 5
    print(i)                  -- 0, 5, 10, 15, ...
end_for

-- Azalan step
for i = 100 to 0 step -5
    print(i)                  -- 100, 95, 90, ...
end_for

-- Ondalık step (Türk formatı)
for x = 0 to 1 step 0,1
    print(x)                  -- 0; 0,1; 0,2; ...
end_for
```

### Downto (Geriye Sayma)

```pmpl
-- Basit geriye sayma
for i = 10 downto 0
    print(i)                  -- 10, 9, 8, ..., 0
end_for

-- Step ile kombinasyon
for i = 100 downto 0 step 10
    print(i)                  -- 100, 90, 80, ..., 0
end_for
```

### Null/Nullable Tipler

```pmpl
-- Nullable tip tanımı (? suffix): tip? isim
numeric? x = null
string? name = null

-- Null check
if x != null then
    print(x)
end_if

-- Null coalescing (?? operatörü)
numeric value = x ?? 0            -- x null ise 0

-- Safe navigation (?. operatörü)
string? city = person?.address?.city

-- Elvis operatörü (?:)
numeric y = x ?: 10               -- x null veya 0 ise 10
```

| Operatör | Token | Açıklama |
|----------|-------|----------|
| `?` | TYPE_NULLABLE | Nullable tip belirteci |
| `??` | TOKEN_NULL_COALESCE | Null ise varsayılan değer |
| `?.` | TOKEN_SAFE_NAV | Güvenli üye erişimi |
| `?:` | TOKEN_ELVIS | Elvis operatörü |

### Tek Satırda Çoklu Komut (VB.NET Stili)

```pmpl
-- Tek satırda çoklu statement (: ayırıcı)
x = 5 : y = 10 : z = x + y

-- If ile tek satırda
if a == b then c = 4 : d = 5

-- İç içe (nested) tek satırda
if a == b then c = 4 : if c == 6 then e = 4

-- While tek satırda
while x < 10 : x = x + 1 : print(x) : end_while

-- For tek satırda
for i = 0 to 5 : print(i) : end_for
```

**⚠️ NOT:** `:` (iki nokta) statement ayırıcı olarak kullanılır. Tip ayırıcı değil!

---

## � MODERN SYNTAX ÖZELLİKLERİ (Stage 1+ Normalizer)

Bu bölüm modern dillerde yaygın olan ancak PMPL'de **syntax olarak bulunmayan** ama **normalizer ile expansion yapılarak desteklenebilecek** özellikleri listeler.

### Spread Operator (Array/Object Expansion)

**Kaynak Dillerde:**
```javascript
// JavaScript
arr2 = [1, 2, ...arr1, 5, 6]
obj2 = {...obj1, name: "Ali"}
```

**PMPL Karşılığı (Normalizer Expansion):**
```pmpl
-- Array spread
arr2 = [1; 2];
for each item in arr1 do
    arr2.add(item);
end_for;
arr2.add(5);
arr2.add(6);

-- Object spread (record merge)
obj2 = obj1.clone();
obj2.name = "Ali";
```

**Stage:** Stage 1+ normalizer ile desteklenir
**Token:** Joker token gerekmez, expansion ile çözülür

---

### Pipeline Operator (Function Chaining)

**Kaynak Dillerde:**
```javascript
// JavaScript (proposal), F#, Elixir
result = value |> double |> increment |> square
```

**PMPL Karşılığı (Normalizer Expansion):**
```pmpl
-- İç içe fonksiyon çağrısı
result = square(increment(double(value)));

-- Veya adım adım
_temp1 = double(value);
_temp2 = increment(_temp1);
result = square(_temp2);
```

**Stage:** Stage 1+ normalizer ile desteklenir
**Token:** Joker token gerekmez, function nesting ile çözülür

---

### String Interpolation (Template Strings)

**Kaynak Dillerde:**
```python
# Python f-string
msg = f"Hello {name}, you are {age} years old"

# JavaScript template literal
msg = `Hello ${name}, you are ${age} years old`

# Ruby
msg = "Hello #{name}, age #{age}"
```

**PMPL Karşılığı (Normalizer Expansion):**
```pmpl
-- String concatenation ile
msg = "Hello " + name + ", you are " + age + " years old";

-- Veya format function ile (future)
msg = format("Hello {0}, you are {1} years old"; name; age);
```

**Stage:** Stage 0 normalizer ile desteklenir (basit concatenation)
**Token:** Joker token gerekmez, `+` operatörü yeterli

---

### List/Dict Comprehension

**Kaynak Dillerde:**
```python
# Python list comprehension
squares = [x**2 for x in range(10) if x % 2 == 0]

# Dict comprehension
mapping = {x: x**2 for x in range(10)}

# Set comprehension
unique = {x for x in items if x > 0}
```

**PMPL Karşılığı (Normalizer Expansion):**
```pmpl
-- List comprehension → explicit loop
squares = [];
for x = 0 to 10 step 1 do
    if x % 2 == 0 then
        squares.add(x ^ 2);
    end_if;
end_for;

-- Dict/Map comprehension
mapping = map_new();
for x = 0 to 10 step 1 do
    mapping.set(x, x ^ 2);
end_for;

-- Set comprehension
unique = set_new();
for each x in items do
    if x > 0 then
        unique.add(x);
    end_if;
end_for;
```

**Stage:** Stage 1+ normalizer ile desteklenir
**Token:** Joker token gerekmez, loop expansion ile çözülür

---

### Destructuring Assignment (Unpacking)

**Kaynak Dillerde:**
```javascript
// JavaScript array destructuring
const [a, b, c] = [1, 2, 3]
const [first, ...rest] = array

// Object destructuring
const {name, age} = person
const {x, y, ...rest} = point

// Python tuple unpacking
a, b, c = (1, 2, 3)
first, *rest = array
```

**PMPL Karşılığı (Normalizer Expansion):**
```pmpl
-- Array destructuring → explicit indexing
_temp = [1; 2; 3];
a = _temp[0];
b = _temp[1];
c = _temp[2];

-- Object destructuring → explicit member access
name = person.name;
age = person.age;

-- Tuple unpacking
_temp = <10; 20; "red">;
x = _temp<0>;
y = _temp<1>;
color = _temp<2>;

-- Rest operator
first = array[0];
rest = array.slice(1, array.length());
```

**Stage:** Stage 1+ normalizer ile desteklenir
**Token:** Joker token gerekmez, multiple assignment ile çözülür

---

### Range Operator (..)

**Kaynak Dillerde:**
```rust
// Rust
for i in 0..10 { }      // 0-9 (exclusive)
for i in 0..=10 { }     // 0-10 (inclusive)

// Ruby
(1..10).each { |i| puts i }
(1...10).each { |i| puts i }  // exclusive

// Swift
for i in 0..<10 { }
for i in 0...10 { }
```

**PMPL Karşılığı (Zaten var!):**
```pmpl
-- PMPL'de zaten range syntax var (to/downto)
for i = 0 to 10 step 1 do    -- 0-10 (inclusive)
    print(i);
end_for;

-- Exclusive range için
for i = 0 to 9 step 1 do     -- 0-9 (exclusive)
    print(i);
end_for;

-- Range array (future)
numeric[] range = [0..10];   -- Bu syntax eklenebilir
```

**Stage:** Mevcut (`to`/`downto`), `..` syntax Stage 1+ eklenebilir
**Token:** TOKEN_RANGE_INCLUSIVE, TOKEN_RANGE_EXCLUSIVE eklenebilir

---

### With Statement / Context Manager

**Kaynak Dillerde:**
```python
# Python context manager
with open("file.txt") as f:
    data = f.read()
# File otomatik kapanır

# C# using statement
using (var conn = new Connection()) {
    // Connection otomatik dispose edilir
}

# Kotlin use function
file.use { it.readText() }
```

**PMPL Karşılığı (Normalizer Expansion):**
```pmpl
-- Try-finally pattern
f = io.open("file.txt");
try
    data = f.read();
finally
    f.close();
end_try;

-- Veya defer pattern (future feature?)
f = io.open("file.txt");
defer f.close();  -- Function sonunda otomatik çağrılır
data = f.read();
```

**Stage:** Stage 0 (try-finally), Stage 1+ (defer pattern eklenebilir)
**Token:** TOKEN_WITH, TOKEN_DEFER eklenebilir (opsiyonel)

---

## 📊 Modern Syntax Destek Matrisi

| Syntax | Kaynak Diller | PMPL Doğrudan | Normalizer Çözüm | Stage | Token Gerekli? |
|--------|---------------|---------------|------------------|-------|----------------|
| **Ternary (`? :`)** | C, JS, Java | ✅ VAR | - | Stage 0 | ✅ VAR (TOKEN_TERNARY_*) |
| **Spread (`...`)** | JS, Python | ❌ | Loop expansion | Stage 1+ | ❌ Hayır |
| **Pipeline (`\|>`)** | F#, Elixir | ❌ | Function nesting | Stage 1+ | ❌ Hayır |
| **Interpolation** | Python, JS, Ruby | ❌ | Concatenation (`+`) | Stage 0 | ❌ Hayır |
| **Comprehension** | Python, Haskell | ❌ | Loop expansion | Stage 1+ | ❌ Hayır |
| **Destructuring** | JS, Python, Rust | ❌ | Multiple assign | Stage 1+ | ❌ Hayır |
| **Range (`..`)** | Rust, Ruby, Swift | 🟡 `to` var | `..` syntax eklenebilir | Stage 1+ | 🟡 Opsiyonel |
| **With/Context** | Python, C# | 🟡 try-finally | defer eklenebilir | Stage 1+ | 🟡 Opsiyonel |

**Sonuç:** 7 modern syntax'tan **5'i joker token gerekmeden** normalizer expansion ile çözülebilir!

---

## 📋 Modern Özellik Durum Tablosu (19 Şubat 2026)

| Özellik | Modern Diller | PMLP Durumu | Yol Haritası |
|---------|--------------|-------------|--------------|
| String interpolation `"{name}"` | Python/JS/Kotlin | ❌ Yok | 📅 **Week 6.2** (Mart-Nisan 2026) |
| Destructuring `x; y = coords` | Rust/JS/Python | ❌ Yok | 📅 **Week 6.4** (Nisan 2026) |
| `defer` | Go/Swift/Zig | ❌ Yok (token hazır) | 📅 **Week 6.1** (Mart 2026) |
| `if let` / `while let` | Rust/Swift | ❌ Yok | 📅 **Week 9.2** (Ağustos 2026) |
| Spread `...` | JS/Python | ❌ Yok | Stage 1+ (belirsiz) |
| Comprehension `[x*x for x in arr]` | Python/Haskell | ❌ Yok | Stage 1+ planlandı |
| Range `..` / `..=` | Rust/Ruby/Swift | 🟡 `to` var | `..` sözdizimi eklenebilir |
| `where` clause | Rust/Swift | 🟡 Inline `{T: Trait}` var | Ayrı clause planlanmadı |
| Named args (call site) | Swift/Python/Kotlin | 🟡 Token hazır | Implementasyon yok |
| `async` / `await` | JS/Python/C#/Rust | 💡 Stage 1.5 | Tasarım aşamasında |
| Macro / Metaprogramming | Rust/Zig/Lisp | 💡 Stage 2+ | Gelecek |
| Comptime | Zig/D | ❌ Tasarım yok | Planlanmamış |

> **Son Güncelleme:** 19 Şubat 2026 — defer, interpolation, destructuring, if-let haftalarına atandı.

---

## �🎯 PARSER İÇİN ÖNEMLİ

1. **Tek Token:** Tüm `end_X`, `exit_X`, `continue_X`, `else_if` TEK TOKEN
2. **Standalone exit/continue:** TOKEN_EXIT ve TOKEN_CONTINUE desteklenir
3. **Pattern Matching Yok:** Basit switch-case yeterli
4. **Normalizer Zorunlu:** MLP → PMPL dönüşümü lexer'dan ÖNCE olmalı
5. **Parametre Ayırıcı:** Her zaman `;` (virgül ondalık ayırıcı)

---

## 📋 TAM TOKEN LİSTESİ

### Blok Sonlandırıcılar
```
TOKEN_END_IF, TOKEN_END_WHILE, TOKEN_END_FOR, TOKEN_END_FUNCTION,
TOKEN_END_STRUCT, TOKEN_END_ENUM, TOKEN_END_SWITCH, TOKEN_END_MATCH,
TOKEN_END_TRY, TOKEN_END_DEBUG, TOKEN_END_OPERATOR
```

### Döngü Kontrolü
```
TOKEN_EXIT, TOKEN_CONTINUE,
TOKEN_EXIT_FOR, TOKEN_EXIT_WHILE, TOKEN_EXIT_IF, TOKEN_EXIT_FUNCTION, TOKEN_EXIT_SWITCH,
TOKEN_CONTINUE_FOR, TOKEN_CONTINUE_WHILE
```

### Kontrol Akışı
```
TOKEN_IF, TOKEN_THEN, TOKEN_ELSE, TOKEN_ELSE_IF,
TOKEN_WHILE, TOKEN_FOR, TOKEN_TO, TOKEN_DOWNTO, TOKEN_STEP, TOKEN_EACH, TOKEN_IN,
TOKEN_SWITCH, TOKEN_CASE, TOKEN_DEFAULT, TOKEN_MATCH
```

### Tipler
```
TOKEN_NUMERIC, TOKEN_STRING_TYPE, TOKEN_BOOLEAN,
TOKEN_ARRAY, TOKEN_LIST, TOKEN_TUPLE,
TOKEN_NULL, TOKEN_NULLABLE
```

### Fonksiyon/Modül
```
TOKEN_FUNCTION, TOKEN_RETURN, TOKEN_RETURNS, TOKEN_AS,
TOKEN_STRUCT, TOKEN_ENUM, TOKEN_IMPORT, TOKEN_MODULE,
TOKEN_OPERATOR, TOKEN_END_OPERATOR,
TOKEN_REF, TOKEN_CONST, TOKEN_OUT, TOKEN_NAMED,
TOKEN_VARIADIC, TOKEN_VARIADIC_PLUS,
TOKEN_DEFAULT_ASSIGN, TOKEN_FUNC, TOKEN_ACTION
```

### Generic/Union
```
TOKEN_GENERIC_OPEN, TOKEN_GENERIC_CLOSE,
TOKEN_UNION_PIPE
```

### Operatörler
```
TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_MOD, TOKEN_POWER,
TOKEN_EQUAL, TOKEN_NOT_EQUAL, TOKEN_LESS, TOKEN_GREATER, TOKEN_LESS_EQUAL, TOKEN_GREATER_EQUAL,
TOKEN_AND, TOKEN_OR, TOKEN_NOT, TOKEN_XOR, TOKEN_ASSIGN,
TOKEN_TERNARY_QUESTION, TOKEN_TERNARY_COLON,
TOKEN_NULL_COALESCE, TOKEN_SAFE_NAV, TOKEN_ELVIS,
TOKEN_AMPERSAND, TOKEN_ASTERISK_PTR, TOKEN_ARROW
```

### Sınırlayıcılar
```
TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACKET, TOKEN_RBRACKET,
TOKEN_SEMICOLON, TOKEN_COLON, TOKEN_DOT, TOKEN_STATEMENT_SEP
```

### Hata Yönetimi
```
TOKEN_TRY, TOKEN_CATCH, TOKEN_FINALLY, TOKEN_THROW, TOKEN_END_TRY
```

### Lambda/Fonksiyon
```
TOKEN_LAMBDA_ARROW, TOKEN_END_LAMBDA
```

### Debug
```
TOKEN_DEBUG, TOKEN_END_DEBUG, TOKEN_GOTO, TOKEN_PAUSE
```

### Literaller
```
TOKEN_NUMBER, TOKEN_STRING, TOKEN_TRUE, TOKEN_FALSE, TOKEN_IDENTIFIER
```

---

## 🚀 MODERN Phase Features (Week 1-2)

### Channels & Concurrency (Week 2)

**Status:** YZ_MODERN_03, channel_compiler  
**Dependencies:** Generic types Week 1 (Channel{T})

```pmpl
-- Channel type (generic)
Channel{numeric} ch = make_channel{numeric}(buffer_size: 10)

-- Send (non-blocking with buffer)
ch.send(42)
ch.send(100)

-- Receive (blocking)
numeric value = ch.receive()

-- Spawn concurrent task
spawn compute_task(ch; [1; 2; 3; 4; 5])

function compute_task(Channel{numeric} out_ch; numeric[] data)
    for each item in data
        numeric result = item * item
        out_ch.send(result)
    end_for
    out_ch.close()
end_function

-- Channel iteration (until closed)
for each result in ch
    print(result)
end_for

-- Select (multiple channels)
select
    case value = ch1.receive():
        print("From ch1: " + value)
    case value = ch2.receive():
        print("From ch2: " + value)
    case timeout(1000):
        print("Timeout!")
end_select
```

**Tokens:** `TOKEN_SPAWN` | `TOKEN_SELECT` | `TOKEN_TIMEOUT` | `Channel{T}` (generic type)

---

### Result{T, E} Type (Week 1 Generics + Week 2 Pattern)

**Status:** Built on generic types (Week 1) + pattern matching (Week 2)  
**Dependencies:** Generic types, pattern matching

```pmpl
-- Result type definition (stdlib)
enum Result{T, E}
    Ok(T value)
    Err(E error)
end_enum

-- Function returning Result
function divide(numeric a; numeric b) as Result{numeric, string}
    if b == 0 then
        return Err("Division by zero")
    else
        return Ok(a / b)
    end_if
end_function

-- Pattern matching (ergonomic)
Result{numeric, string} res = divide(10, 2)
match res
    case Ok(value):
        print("Result: " + value)
    case Err(msg):
        print("Error: " + msg)
end_match

-- Chaining (monadic)
Result{numeric, string} final = divide(100, 5)
    .and_then(x => divide(x, 2))
    .map(x => x + 10)

-- Unwrap with default
numeric safe_value = divide(10, 0).unwrap_or(0)
```

---

### Option{T} Type (Week 1 Generics + Week 2 Pattern)

**Status:** Built on generic types (Week 1) + pattern matching (Week 2)  
**Dependencies:** Generic types, pattern matching

```pmpl
-- Option type definition (stdlib)
enum Option{T}
    Some(T value)
    None
end_enum

-- Function returning Option
function find_user(string name) as Option{User}
    if user_exists(name) then
        return Some(get_user(name))
    else
        return None
    end_if
end_function

-- Pattern matching
Option{User} user = find_user("Ali")
match user
    case Some(u):
        print("Found: " + u.name)
    case None:
        print("User not found")
end_match

-- Safe navigation
string display = find_user("Ali")
    .map(u => u.name)
    .unwrap_or("Unknown")

-- Combining with Result
function safe_parse(string s) as Result{numeric, string}
    Option{numeric} parsed = try_parse(s)
    match parsed
        case Some(n):
            return Ok(n)
        case None:
            return Err("Parse failed")
    end_match
end_function
```

---

### Error Handling (Week 2)

**Status:** YZ_MODERN_04, error_compiler  
**Dependencies:** Result{T,E} (Week 1 generics)

```pmpl
-- Try-catch blocks
try
    numeric result = risky_operation()
    print(result)
catch err as IOError
    print("IO Error: " + err.message)
catch err as ParseError
    print("Parse Error: " + err.message)
finally
    cleanup_resources()
end_try

-- Throw errors
function risky_operation() as numeric throws IOError
    if file_not_found then
        throw IOError("File not found")
    end_if
    return 42
end_function

-- Error propagation with ?
function process_file(string path) as Result{string, IOError}
    string content = read_file(path)?  -- Auto-propagate error
    string processed = transform(content)?
    return Ok(processed)
end_function

-- Custom error types
struct CustomError
    string message
    numeric code
end_struct

function validate(string input) as Result{bool, CustomError}
    if input.length < 5 then
        return Err({message: "Too short", code: 400})
    end_if
    return Ok(true)
end_function
```

**Tokens:** `TOKEN_TRY` | `TOKEN_CATCH` | `TOKEN_FINALLY` | `TOKEN_THROW` | `TOKEN_END_TRY`

---

## 🎯 STAGE 1 YENİ ÖZELLİKLER (PD Onaylı)

### Type Aliases - ✅ Week 1 Day 5

**Purpose:** Kod okunabilirliği ve tip güvenliği

```pmpl
-- Type alias tanımı
type UserId = numeric
type Email = string
type UserResult = Result{User, Error}

function find_user(UserId id) as UserResult
    -- implementation
end_function

-- Kullanım
UserId current_id = 42
Email user_email = "ali@example.com"
```

**Implementation:**
- **Stage:** Week 1 Day 5
- **Token:** `TOKEN_TYPE`
- **Syntax:** `type NewName = ExistingType`
- **Semantik:** Compile-time alias, runtime overhead yok

**Avantajlar:**
- Self-documenting code
- Type safety (numeric vs UserId ayrımı)
- Refactoring kolaylığı

---

### Pipe Operator - ✅ Week 2

**Purpose:** Functional composition, Unix-style pipeline

```pmpl
-- Traditional style (nested calls)
numeric result = sum(map(filter([1;2;3;4;5], x => x > 2), x => x * 2))

-- Pipe style (linear flow)
numeric result = [1; 2; 3; 4; 5]
    |> filter(x => x > 2)
    |> map(x => x * 2)
    |> sum()
-- result = 24 (3*2 + 4*2 + 5*2)

-- Complex pipeline
string output = read_file("data.txt")
    |> parse_json()
    |> extract_field("users")
    |> filter(u => u.age > 18)
    |> map(u => u.name)
    |> join(", ")
```

**Implementation:**
- **Stage:** Week 2
- **Token:** `TOKEN_PIPE` (`|>`)
- **Dependencies:** Lambda functions (Week 2)
- **Semantik:** `x |> f(a)` → `f(x, a)` (compile-time transformation)

**Avantajlar:**
- Okunabilir functional code
- Left-to-right data flow
- No runtime overhead

---

### Traits/Interfaces - ⚠️ Week 2-3 (Rust-style ZORUNLU)

**Purpose:** Generic constraints ve polymorphism

> **⚠️ CRITICAL:** Method-in-struct YASAK! (OOP violation)
> Rust-style external impl ZORUNLU (STRUCT+FUNC prensibi)

```pmpl
-- Trait definition (interface)
trait Printable
    function to_string() as string
end_trait

trait Comparable{T}
    function compare(T other) as numeric
end_trait

-- Struct definition (data only - NO METHODS!)
struct Point
    numeric x
    numeric y
end_struct

-- ✅ DOĞRU: Implementation (external, free functions)
implement Printable for Point
    function to_string(Point self) as string
        return "Point(" + self.x + ", " + self.y + ")"
    end_function
end_implement

implement Comparable{Point} for Point
    function compare(Point self; Point other) as numeric
        return (self.x + self.y) - (other.x + other.y)
    end_function
end_implement

-- Generic with trait bound
function print_all{T: Printable}(T[] items)
    for each item in items
        print(to_string(item))  -- Free function call
    end_for
end_function

-- Multiple trait bounds
function sort_and_print{T: Printable + Comparable{T}}(T[] items)
    sorted = items.sort()  -- Comparable required
    for each item in sorted
        print(to_string(item))  -- Printable required
    end_for
end_function
```

**❌ YANLIŞ (OOP-style - YASAK):**
```pmpl
-- ❌ Method inside struct = OOP violation!
struct Point
    numeric x
    numeric y
    
    function to_string() as string  -- ❌ YASAK!
        return "Point"
    end_function
end_struct
```

**Implementation:**
- **Stage:** Week 2-3
- **Token:** `TOKEN_TRAIT` | `TOKEN_IMPLEMENTS` | `TOKEN_IMPLEMENT` | `TOKEN_END_IMPLEMENT`
- **Dependencies:** Generic types (Week 1)
- **MELP Compliance:** ✅ External impl = STRUCT+FUNC (5. ESAS'a uygun)

---

## ⚠️ PARSER WARNINGS - CRITICAL DISAMBIGUATION

### Generic vs Literal Syntax

**Context:** Three different uses of braces/angle brackets

| Syntax | Context | Meaning | Example |
|--------|---------|---------|---------|
| `{T}` | Type position | Generic parameter | `function foo{T}(T x)` |
| `{k: v}` | Value position | Struct literal (with colon) | `Point p = {x: 10, y: 20}` |
| `<v; v>` | Value position | Tuple literal (with semicolon) | `tuple t = <10; 20>` |

**Parser Rule:**
1. After `struct`/`function` name → Generic `{T}`
2. In expression, with colon `:` → Struct `{k: v}`
3. In expression, angle brackets → Tuple `<v; v>`

**Examples:**
```pmpl
-- Generic type parameter (type position)
function first{T}(T[] items) as T    -- {T} = generic
    return items[0]
end_function

-- Struct literal (value position with colon)
Point p = {x: 10, y: 20}             -- {k: v} = struct literal

-- Tuple literal (value position with semicolon)
tuple pos = <10; 20>                 -- <v; v> = tuple literal

-- Complex: Generic function returning struct
function make_point{T}(T x; T y) as Point{T}
    return {x: x, y: y}  -- Struct literal inside generic function
end_function
```

---

## 🗓️ MODERN FEATURES IMPLEMENTATION TIMELINE

| Feature | Week | Dependencies | Status | Priority | Complexity | Token |
|---------|------|--------------|--------|----------|------------|-------|
| **Generic Types** | Week 1 D1-2 | None | ✅ DONE | P0 | Medium | `{T}` |
| **Import/Module** | Week 1 D3-4 | None | ✅ DONE | P0 | Low | `import/export` |
| **Type Aliases** | Week 1 D5 | None | 🚀 NEW | P0 | Low | `type` |
| **Pattern Match** | Week 2 | Generics | 🚧 WIP | P0 | Medium | `match/case` |
| **Lambda/Closure** | Week 2 | Generics | 🚧 WIP | P0 | High | `=>` |
| **Pipe Operator** | Week 2 D5-6 | Lambda | 🚀 NEW | P1 | Low | `\|>` |
| **Channels** | Week 2 | Generics | ⏳ TODO | P1 | High | `Channel{T}` |
| **Error Handling** | Week 2 | Result{T,E} | ⏳ TODO | P1 | Medium | `try/catch` |
| **Traits** (Rust) | Week 2-3 | Generics | 🚀 NEW | P1 | Medium | `trait/implement` |
| **Async/Await** | Stage 1.5 | Channels | 💡 DESIGN | P1 | High | `async/await` |
| **Macro System** | Stage 2+ | Full AST | 💡 FUTURE | P2 | Very High | `macro` |

**Legend:**
- ✅ DONE: Implemented and tested
- 🚧 WIP: Work in progress
- 🚀 NEW: PD approved, ready for implementation
- ⏳ TODO: Planned
- 💡 DESIGN: Design phase only
- 💡 FUTURE: Post-bootstrap

**Total Timeline:**
- Week 1 (27-31 Ocak): Generics, Import, Type Aliases (3.5 days)
- Week 2 (3-7 Şubat): Pattern, Lambda, Pipe, Channels, Error (5 days)
- Week 2-3 (7-14 Şubat): Traits (Rust-style) (3 days)
- Week 3 (10-14 Şubat): Integration + Validation (3 days)
- Week 4 (15-17 Şubat): Bootstrap Testing (2.5 days)
- **Total:** 17 days (MODERN complete)

**Stage 1.5 (Post-Bootstrap):**
- Async/Await design + implementation (1 week)
- Full testing + documentation (3 days)

---

**Bakımcı:** MELP Projesi  
**Ana Referans:** PMPL_SYNTAX.md  
**Son Güncelleme:** 26 Ocak 2026 (YZ Analiz + PD Onay)  
**Durum:** ✅ MELP KESİN SÖZDİZİMİ (MODERN Week 1-2 + Stage 1 Features)

# PMLP0 SÖZDİZİMİ - STAGE 0 MİNİMAL SPEC

**Versiyon:** 1.0  
**Tarih:** 12 Ocak 2026  
**Stage:** Stage 0 - Bootstrap Compiler  
**Durum:** Minimal, Net, Pragmatik  

---

## 🎯 PMLP0 NEDİR?

**PMLP0**, MELP Stage 0 compiler'ının **tek ve minimal** sözdizimi dilidir.

### Stage 0 Amacı: Fırlatma Rampası
```
PMLP0 → Lexer → Parser → Codegen → LLVM IR
```

**PMLP0'da OLMAYAN:**
- ❌ Normalizer (Stage 1'de başlıyor)
- ❌ Çok dilli/çok syntaxlı destek (Stage 1'de başlıyor)
- ❌ Gelişmiş özellikler (varsayılan params, generics, vb.)

**PMLP0 Prensibi:**
> "Mümkün olan en basit, en net, belirsizliği olmayan syntax.  
> Stage 0'ın tek amacı: MELP'i bootstrap etmek."

---

## 📐 TEMEL KURALLAR

### Kural 1: Semicolon (`;`) SADECE AYIRICI

```pmlp0
-- ✅ DOĞRU: Semicolon SADECE ayırıcı olarak kullanılır
numeric x = 42
x = 50
function test(numeric a; numeric b) as numeric
    return a + b
end_function

-- ✅ Array/List/Tuple elemanları
numeric[] arr = [1; 2; 3]        -- Son eleman SONRA ; YOK
list data = (5; "Ali"; true)     -- Son eleman SONRA ; YOK

-- ❌ YANLIŞ: Semicolon satır sonu olarak kullanılmaz
numeric y = 10;                  -- YANLIŞ!
```

**Kural:** `;` sadece **parametre** ve **eleman** ayırmak için.

---

### Kural 2: Number Format (SADECE Turkish)

```pmlp0
numeric pi = 3,14                -- ✅ 3.14 (virgül = ondalık ayırıcı)
numeric million = 1.000.000      -- ✅ 1,000,000 (nokta = binlik ayırıcı)
numeric precise = 126.555.852,36 -- ✅ 126,555,852.36

-- ❌ AMERIKAN FORMAT DESTEKLENMIYOR:
numeric pi = 3.14                -- ❌ HATA! Dot decimal kullanılamaz
```

**Kural:** 
- `,` (virgül) = ondalık ayırıcı (SADECE)
- `.` (nokta) = binlik ayırıcı (SADECE)
- Amerikan format (dot decimal) desteklenmez

**Sebep:**
- MELP Türk kullanıcılar için tasarlandı
- Tek format = basit parser, net semantik
- Stage 0 → Stage 1 tutarlılığı

---

### Kural 3: Tek Sözcük, Tek Token

```pmlp0
-- ✅ SADECE bunlar kullanılır:
as          -- Dönüş tipi (returns YOK)
mod         -- Modulo (% YOK)
exit        -- Standalone blok çıkış
exit_for    -- Explicit for döngüsü çıkış
exit_while  -- Explicit while döngüsü çıkış
exit_if     -- Explicit if bloğu çıkış
continue    -- Standalone devam
continue_for    -- Explicit for döngüsü devam
continue_while  -- Explicit while döngüsü devam

-- ❌ KULLANILMAZ:
returns     -- Deprecated, sadece "as"
%           -- Deprecated, sadece "mod"
text        -- Deprecated, sadece "string"
```

**Kural:** Her kavram için TEK keyword.

---

## 🔤 TEMEL TİPLER

| Tip | Açıklama | Varsayılan Değer |
|-----|----------|------------------|
| `numeric` | Tüm sayılar (tam, ondalık) | 0 |
| `string` | Metinler (UTF-8) | "" |
| `boolean` | true / false | false |

### Tanımlama ve Atama

```pmlp0
-- ✅ Tip belirterek tanımlama
numeric x = 42
string name = "Ali"
boolean flag = true

-- ✅ Varsayılan değerli tanımlama
numeric y        -- y = 0
string title     -- title = ""
boolean ready    -- ready = false

-- ✅ Atama (tip belirteci YOK)
x = 50
name = "Veli"
flag = false
```

**Kural:** İlk tanımlamada tip belirteci ZORUNLU (veya varsayılan).

---

## 📦 KOLEKSİYONLAR

### Array (Homojen, Mutable)

```pmlp0
numeric[] numbers = [1; 2; 3; 4; 5]
numbers[0] = 100

string[] names = ["Ali"; "Veli"; "Ayşe"]
names[1] = "Mehmet"

-- Boş array
numeric[] empty = []
```

**Özellikler:**
- Sadece aynı tip elemanlar
- Index: 0'dan başlar
- Mutable (değiştirilebilir)

---

### List (Heterojen, Mutable)

```pmlp0
list person = ("Ali"; 25; true)
person[0] = "Veli"

-- Boş list
list empty = ()
```

**Özellikler:**
- Farklı tip elemanlar olabilir
- Index: 0'dan başlar
- Mutable

---

### Tuple (Heterojen, Immutable)

```pmlp0
tuple coords = <10; 20; 30>
numeric x = coords[0]    -- ✅ Okuma

-- coords[0] = 99         -- ❌ HATA! Immutable

-- Boş tuple
tuple empty = <>
```

**Özellikler:**
- Farklı tip elemanlar olabilir
- Index: 0'dan başlar
- **Immutable** (değiştirilemez)

---

## ⚙️ OPERATÖRLER

### Aritmetik

| Operatör | Açıklama |
|----------|----------|
| `+` | Toplama |
| `-` | Çıkarma |
| `*` | Çarpma |
| `/` | Bölme |
| `mod` | Modulo |
| `**` | Üs alma |

```pmlp0
numeric result = 10 + 5
numeric remainder = 17 mod 3
numeric power = 2 ** 8
```

---

### Karşılaştırma

| Operatör | Açıklama |
|----------|----------|
| `==` | Eşit |
| `!=` | Eşit değil |
| `<` | Küçük |
| `>` | Büyük |
| `<=` | Küçük eşit |
| `>=` | Büyük eşit |

---

### Mantıksal

| Operatör | Açıklama |
|----------|----------|
| `and` | VE |
| `or` | VEYA |
| `not` | DEĞİL |

```pmlp0
boolean result = (x > 5) and (y < 10)
boolean check = not flag
```

---

## 🔀 KONTROL AKIŞI

### If-Then-Else

```pmlp0
if x > 100 then
    print("büyük")
else_if x > 50 then
    print("orta")
else
    print("küçük")
end_if
```

**Kural:** `if` her zaman `then` ile biter.

---

### While Döngüsü

```pmlp0
numeric i = 0
while i < 10
    print(i)
    i = i + 1
end_while
```

---

### For Döngüsü

```pmlp0
-- Artan
for i = 0 to 9
    print(i)
end_for

-- Azalan
for i = 10 downto 1
    print(i)
end_for
```

---

### Döngü Kontrolü

```pmlp0
-- exit: Standalone - mevcut döngüden/bloktan çık
while true
    if condition then
        exit
    end_if
end_while

-- exit_while: Explicit - while döngüsünden çık
while outer_condition
    while inner_condition
        if error then
            exit_while    -- Sadece içteki while'dan çık
        end_if
    end_while
end_while

-- exit_for: Explicit - for döngüsünden çık
for i = 0 to 99
    if found then
        exit_for
    end_if
end_for

-- exit_if: Explicit - if bloğundan çık (erken çıkış)
if complex_condition then
    if validation_failed then
        exit_if    -- If bloğundan çık
    end_if
    -- Normal devam
end_if

-- continue: Standalone - bir sonraki iterasyona geç
for i = 0 to 9
    if i mod 2 == 0 then
        continue
    end_if
    print(i)
end_for

-- continue_for: Explicit - for döngüsüne devam
for i = 0 to 9
    if i mod 2 == 0 then
        continue_for
    end_if
    print(i)
end_for

-- continue_while: Explicit - while döngüsüne devam
while condition
    if skip then
        continue_while
    end_if
    process()
end_while
```

**Kural:** 
- Standalone: `exit`, `continue` (mevcut bloktan çık/devam)
- Explicit: `exit_for`, `exit_while`, `exit_if`, `continue_for`, `continue_while`
- Her iki form da desteklenir

---

## � STRUCT (Minimal - AST İçin)

### Basit Struct Tanımı

```pmlp0
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

**Özellikler:**
- Sadece data container (method YOK)
- Inheritance YOK
- Generic YOK
- Minimal implementation

---

### Struct Oluşturma ve Kullanma

```pmlp0
-- Struct instance oluşturma
Point p = Point{10; 20}
Person person = Person{"Ali"; 25; true}

-- Field erişimi (dot notation)
numeric x = p.x           -- 10
string name = person.name -- "Ali"

-- Field güncelleme
p.x = 30
person.age = 26
```

---

### Stage 1 Compiler İçin Örnek

```pmlp0
-- AST Node representation
struct ASTNode
    string node_type
    numeric value
    list children
end_struct

-- Token representation
struct Token
    string type
    string value
    numeric line
end_struct

-- Usage
Token tok = Token{"NUMBER"; "42"; 1}
ASTNode node = ASTNode{"binary_op"; 0; [left_node; right_node]}

print(tok.type)        -- "NUMBER"
print(node.node_type)  -- "binary_op"
```

**Neden Gerekli:**
- Stage 1 compiler PMLP0 ile yazılır
- AST nodes struct olarak temsil edilir
- Token'lar struct olarak temsil edilir

---

## �🔧 FONKSİYONLAR

### Temel Fonksiyon

```pmlp0
function add(numeric a; numeric b) as numeric
    return a + b
end_function

-- Çağırma
numeric result = add(5; 10)
```

**Kural:** 
- Parametre ayırıcı: `;` (semicolon)
- Return type: `as` keyword
- `returns` keyword KULLANILMAZ

---

### Return Tipi: `none`

```pmlp0
function greet(string name) as none
    print("Hello, " + name)
end_function

greet("World")
```

---

### Koleksiyon Döndürme

```pmlp0
function get_data() as list
    return (1; "test"; 3,14)
end_function

function get_coords() as tuple
    return <10; 20; 30>
end_function
```

---

## � DOSYA I/O (Minimal - Stage 1 İçin)

### Temel File Operations

```pmlp0
-- Dosya okuma
string read_file(string path) as string
    -- C implementation ile sağlanır
end_function

-- Dosya yazma
function write_file(string path; string content) as none
    -- C implementation ile sağlanır
end_function

-- Usage
string source_code = read_file("input.mlp")
write_file("output.txt"; "Hello World")
```

**Neden Gerekli:**
- Stage 1 compiler kaynak kod okumalı
- LLVM IR output dosyaya yazmalı

**Kısıtlamalar:**
- Sadece text file (binary YOK)
- Sadece read/write (append, delete vb. YOK)
- Error handling minimal (file not found → program crash)

---

## �💬 YORUM SATIRLARI

```pmlp0
-- Tek satır yorum

---
Çok satırlı yorum
Birden fazla satır
İçerebilir
---
Enum tanımları
- ❌ Switch-case
- ❌ For-each döngüsü
- ❌ Import/Module sistemi
- ❌ Çoklu değişken tanımlama (`x; y; z = 1; 2; 3`)
- ❌ Liste unpacking
- ❌ Struct methods (struct var ama sadece data container)
- ❌ Struct inheritance
- ❌ Advanced file I/O (append, delete, binary)` ... `---` (üç tire, simetrik)
- `//` ve `#` KULLANILMAZ

---

## 📋 BLOK SONLANDIRICILAR

| Blok | Sonlandırıcı |
|------|--------------|
| `if` | `end_if` |
| `while` | `end_while` |
| `for` | `end_for` |
| `function` | `end_function` |

**Kural:** Her blok kendi sonlandırıcısıyla biter.

---

## 🚫 PMLP0'DA OLMAYAN ÖZELLIKLER

### Stage 1'e Ertelenen:
- ❌ Varsayılan parametreler (`:=`)
- ❌ Referans parametreler (`ref`, `const`)
- ❌ Named parameters
- ❌ Variadic parameters (`...`)
- ❌ Struct tanımları
- ❌ Enum tanımları
- ❌ Switch-case
- ❌ For-each döngüsü
- ❌ Import/Module sistemi
- ❌ Çoklu değişken tanımlama (`x; y; z = 1; 2; 3`)
- ❌ Liste unpacking

### Stage 2+'ye Ertelenen:
- ❌ Generic/Template parametreler
- ❌ Union tipi (`numeric | string`)
- ❌ Pattern matching
- ❌ Try-catch-finally
- ❌ Lambda fonksiyonlar
- ❌ Operator overloading
- ❌ Pointer syntax (deprecated)
- ❌ Null/Nullable tipler
as`)
- ❌ `%` (sadece `mod`)
- ❌ `exit_for`, `exit_while`, `continue_for` (sadece `exit`, `continue`)
- ❌ Positional optional `[]`
- ❌ Variadic `+` suffix

---

## 📝 TAM ÖRNEK

```pmlp0
---
PMLP0 Örnek Program
Fibonacci sayılarını hesaplar
---

function fibonacci(numeric n) as numeric
    if n <= 1 then
        return n
    end_if
    return fibonacci(n - 1) + fibonacci(n - 2)
end_function

function print_sequence(numeric count) as none
    for i = 0 to count - 1
        numeric fib = fibonacci(i)
        print(fib)
    end_for
end_function

function main() as numeric
    numeric limit = 10
    print_sequence(limit)
    return 0
end_function
```

---

## 🎯 PMLP0 FELSEFESİ

### 1. Minimal
> "Mümkün olan en az özellik, en az karmaşıklık."

### 2. Net
> "Belirsizlik yok. Her syntax tek bir şeyi ifade eder."

### 3. Pragmatik
> "Teorik güzellik değil, pratik kullanım öncelikli."

### 4. Bootstrap Odaklı
> "Tek amaç: Stage 1 compiler'ını yazmak için yeterli olmak."

### 5. Tutarlılık
> "PMLP0 syntax Stage 1'de de geçerli. Number format, keyword'ler değişmez."

---

## 🔄 STAGE 0 → STAGE 1 İLİŞKİSİ

### Stage 0: PMLP0 Compiler (C ile)
```
PMLP0 → Lexer → Parser → Codegen → LLVM IR
```

### Stage 1: PMLP0 + PMLP1 Compiler (PMLP0 ile yazılmış!)
```
PMLP0/PMLP1 → Lexer → Parser → Codegen → LLVM IR
```

**Kritik:** Stage 1 compiler PMLP0 ile yazılır, dolayısıyla:
- PMLP0 syntax Stage 1'de de desteklenmeli (backward compatible)
- Number format değişmemeli (Turkish format her ikisinde de)
- Keyword'ler değişmemeli (`as`, `mod`, `exit`)

**Stage 1 = PMLP0 + PMLP1 desteği:**
- ✅ PMLP0 kodlarını derler (kendisi PMLP0 ile yazıldığı için)
- ✅ PMLP1 kodlarını derler (yeni özellikler eklenmiş)

---

## 📊 PMLP0 vs PMLP1 (Özet)

| Özellik | PMLP0 (Stage 0) | PMLP1 (Stage 1+) |
|---------|-----------------|------------------|
| **Normalizer** | ❌ Yok | ✅ Var |
| **Çok dilli syntax** | ❌ Yok | ✅ Var |
| **Temel tipler** | ✅ numeric, string, boolean | ✅ Aynı + genişletilmiş |
| **Koleksiyonlar** | ✅ array, list, tuple | ✅ Aynı + gelişmiş |
| **Fonksiyonlar** | ✅ Basit | ✅ Gelişmiş (default, ref, named, variadic) |
| **Struct/Enum** | ❌ Yok | ✅ Var |
| **Generics** | ❌ Yok | ⏳ Stage 2+ |
| **Pattern matching** | ❌ Yok | ⏳ Stage 2+ |

---

## 🔗 İLGİLİ BELGELER

- **PMLP_SYNTAX_ISSUES.md** - PMLP0 tasarım kararlarının gerekçeleri
- **TODO_Stage 0 - Codegen Bug Fixing.md** - PMLP0 implementation durumu
- **pmlp_sozdizimi.md** - DEPRECATED (PMLP0 + PMLP1 karışık, kullanma)

---

## ✅ SONUÇ

**PMLP0 = Stage 0'ın dili.**

- Minimal ✅
- Net ✅
- Pragmatik ✅
- Belirsizlik yok ✅
- Gereksiz özellik yok ✅

**Hedef:** Stage 1 compiler'ını yazmak için yeterli olmak, fazlası değil.

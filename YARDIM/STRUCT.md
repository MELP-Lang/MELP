# MELP Struct Referansı

**Son Güncelleme:** 9 Mart 2026  
**Durum:** ✅ Aktif (Stage 0 ve Stage 1)  
**Kaynak:** MELP Language Reference v1.0

---

## 📋 İÇİNDEKİLER

1. [Struct Nedir?](#struct-nedir)
2. [Temel Syntax](#temel-syntax)
3. [Struct Tanımlama Kuralları](#struct-tanımlama-kuralları)
4. [Field (Alan) Tanımlama](#field-alan-tanımlama)
5. [Struct Kullanımı](#struct-kullanımı)
6. [Constructor Parametreleri](#constructor-parametreleri)
7. [Struct Method'ları](#struct-methodları)
8. [method Keyword](#method-keyword)
9. [İç İçe Struct'lar](#iç-içe-structlar)
10. [Struct Dizileri](#struct-dizileri)
11. [Generic Struct'lar](#generic-structlar)
12. [Interface Implementation](#interface-implementation)
13. [Yasaklar ve Kısıtlamalar](#yasaklar-ve-kısıtlamalar)
14. [Örnekler](#örnekler)
15. [Sık Sorulan Sorular](#sık-sorulan-sorular)

---

## Struct Nedir?

MELP'te **struct** (yapı), birden fazla veriyi tek bir birim altında toplayan **veri taşıyıcı**dır.

**Önemli:** MELP struct'ları **sadece veri içerir**. Method'lar struct **dışında** tanımlanır (STRUCT+FUNC prensibi).

---

## Temel Syntax

### Minimum struct tanımı:

```mlp
struct StructName
    type fieldName
end struct
```

### ✅ Geçerli struct tanımı:

```mlp
struct Point
    numeric x
    numeric y
end struct
```

### ❌ Geçersiz syntax'lar:

```mlp
-- ❌ 'struct' keyword'ü olmadan struct tanımlanamaz:
Point
    numeric x
end_Point

-- ❌ Parametre alınamaz:
struct Point(numeric x)  -- YANLIŞ!
    numeric y
end struct

-- ❌ Method içeride olamaz:
struct Point
    numeric x
    numeric function distance()  -- YASAK!
        return x
    end function
end struct
```

---

## Struct Tanımlama Kuralları

### Kural 1: `struct` keyword'ü ZORUNLU

✅ **DOĞRU:**
```mlp
struct Person
    string name
    numeric age
end struct
```

❌ **YANLIŞ:**
```mlp
Person           -- 'struct' yok, geçersiz!
    string name
end_Person
```

### Kural 2: `end struct` ile kapatılmalı

✅ **DOĞRU:**
```mlp
struct Point
    numeric x
end struct
```

❌ **YANLIŞ:**
```mlp
struct Point
    numeric x
end_Point        -- 'end struct' olmalı!
```

### Kural 3: Struct'lar constructor parametresi alabilir (opsiyonel)

Parametresiz (klasik) syntax:
```mlp
struct Rectangle
    numeric width
    numeric height
end struct
```

Constructor parametreli syntax (EKS-003 — ✅ 8 Mart 2026 itibarıyla aktif):
```mlp
struct Rectangle(numeric width; numeric height)
    numeric width
    numeric height
end struct

-- Otomatik üretilen factory ile doğrudan örnek oluşturma:
Rectangle r = Rectangle(10; 5)
print(r.width)    -- 10
print(r.height)   -- 5
```

**Not:** Generic **tip** parametresi farklıdır (bkz. [Generic Struct'lar](#generic-structlar))

---

## Field (Alan) Tanımlama

MELP iki field syntax formatını destekler:

### Format 1: `type fieldName` (Varsayılan)

```mlp
struct Person
    string name
    numeric age
    boolean active
end struct
```

### Format 2: `fieldName as type` (Generic desteği için)

```mlp
struct Container<T>
    value as T
    count as numeric
end struct
```

### Field tipleri:

| Tip | Açıklama | Örnek |
|-----|----------|-------|
| `numeric` | 64-bit tamsayı | `numeric count` |
| `string` | Metin referansı | `string name` |
| `boolean` | true/false | `boolean active` |
| `<StructName>` | Başka bir struct | `Address addr` |
| `<StructName>[]` | Struct dizisi | `Point[] vertices` |
| `<T>` | Generic tip | `value as T` |

### Field ayırıcıları:

```mlp
-- Newline ile:
struct Point
    numeric x
    numeric y
end struct

-- Semicolon ile (opsiyonel):
struct Point
    numeric x;
    numeric y;
end struct
```

---

## Struct Kullanımı

### Instance oluşturma:

```mlp
-- Tip belirtimi:
Point p

-- Field'lara değer atama:
p.x = 10
p.y = 20

-- Okuma:
print(p.x)  -- 10
```

### Struct literal (Stage 1):

```mlp
Point p = {x: 10; y: 20}
```

### Field erişimi:

```mlp
struct Person
    string name
    numeric age
end struct

Person ali
ali.name = "Ali Veli"
ali.age = 30

print(ali.name)  -- "Ali Veli"
numeric years = ali.age  -- 30
```

---

## Constructor Parametreleri

✅ **Durum: TAM** (EKS-003, 8 Mart 2026)

Struct tanımına constructor parametreleri eklenebilir. Derleyici otomatik olarak bir factory fonksiyonu üretir.

### Syntax:
```mlp
struct StructAdı(tip param1; tip param2)
    tip param1
    tip param2
end struct
```

### Örnek — basit constructor:
```mlp
struct Point(numeric x; numeric y)
    numeric x
    numeric y
end struct

function main()
    Point p = Point(10; 20)
    print(p.x)   -- 10
    print(p.y)   -- 20
end function
```

### Örnek — birden fazla instance:
```mlp
Point p1 = Point(1; 2)
Point p2 = Point(3; 4)
print(p1.x)   -- 1
print(p2.x)   -- 3
```

### Kurallar:
- Constructor parametre adları struct field adlarıyla **eşleşmelidir**
- Parametre sırası = field başlatma sırası
- Tipler eşleşmeli (`numeric`, `string`, `boolean`)
- Constructor parametresiz struct'lar da geçerlidir (klasik syntax)

---

## Struct Method'ları

**Kritik:** MELP'te method'lar struct **DIŞINDA** tanımlanır!

### ✅ DOĞRU: External method tanımı

```mlp
struct Rectangle
    numeric width
    numeric height
end struct

-- Method struct DIŞINDA:
numeric function Rectangle.area()
    return this.width * this.height
end function

numeric function Rectangle.perimeter()
    return 2 * (this.width + this.height)
end function

-- Kullanım:
Rectangle r
r.width = 10
r.height = 5
print(r.area())       -- 50
print(r.perimeter())  -- 30
```

### `this` keyword:

Method içinde `this` struct instance'ını temsil eder:

```mlp
numeric function Point.distance_from_origin()
    return this.x * this.x + this.y * this.y
end function
```

### ❌ YANLIŞ: Method struct içinde

```mlp
-- ❌ MELP'te YASAK:
struct Point
    numeric x
    numeric y
    
    numeric function distance()  -- HATA!
        return this.x
    end function
end struct
```

---

## method Keyword

✅ **Durum: TAM** (EKS-010, 8 Mart 2026)

`method` keyword'ü `function` ile eşdeğerdir; struct method'ları için alternatif yazım.
Kapatıcı olarak `end <metodAdı>` (struct öneki olmadan kısa isim) kullanılabilir:

```mlp
struct Sayac
    numeric deger
end struct

numeric method Sayac.artir()
    this.deger = this.deger + 1
    return this.deger
end artir   -- ya da end function

method Sayac.ekle(numeric n)
    this.deger = this.deger + n
end ekle
```

| Kapatıcı | `function` | `method` |
|---------|-----------|----------|
| `end function` | ✅ | ✅ |
| `end <metodAdı>` | ✅ | ✅ |

---

## İç İçe Struct'lar

Bir struct başka struct field'i içerebilir:

```mlp
struct Address
    string street
    numeric zipcode
end struct

struct Person
    string name
    numeric age
    Address addr
end struct

-- Kullanım:
Person p
p.name = "Ayşe"
p.addr.street = "Atatürk Cad."
p.addr.zipcode = 34000

print(p.addr.street)  -- "Atatürk Cad."
```

### Çok seviyeli erişim:

```mlp
struct City
    string name
end struct

struct Address
    string street
    City city
end struct

struct Person
    Address addr
end struct

Person p
p.addr.city.name = "İstanbul"
print(p.addr.city.name)  -- "İstanbul"
```

---

## Struct Dizileri

```mlp
struct Point
    numeric x
    numeric y
end struct

-- Dizi tanımlama:
Point[] points

-- Array literal (Stage 1):
points = [{x: 0; y: 0}; {x: 10; y: 20}]

-- Erişim:
print(points[0].x)  -- 0
print(points[1].y)  -- 20
```

---

## Generic Struct'lar

Generic struct'lar tip parametre alabilir (Stage 1):

### Syntax:

```mlp
struct Container<T>
    value as T
    count as numeric
end struct

-- Kullanım:
Container<numeric> numbers
numbers.value = 42
numbers.count = 1

Container<string> texts
texts.value = "hello"
texts.count = 1
```

### Çoklu tip parametresi:

```mlp
struct Pair<T; U>
    first as T
    second as U
end struct

Pair<numeric; string> p
p.first = 10
p.second = "test"
```

**Not:** Tip parametreleri **noktalı virgül** (`;`) ile ayrılır!

---

## Interface Implementation

Struct'lar interface implement edebilir (Stage 1):

```mlp
interface Shape
    numeric function area()
end interface

struct Circle implements Shape
    numeric radius
end struct

numeric function Circle.area()
    return 3 * this.radius * this.radius
end function
```

### Çoklu interface:

```mlp
struct MyStruct implements Interface1; Interface2
    numeric value
end struct
```

---

## Yasaklar ve Kısıtlamalar

### ❌ 1. Struct içinde method tanımlama

```mlp
-- YASAK:
struct Point
    numeric function distance()  -- HATA!
    end function
end struct
```

### ❌ 2. Struct'a parametre verme

```mlp
-- YASAK:
struct Point(numeric x)  -- HATA!
end struct
```

### ❌ 3. `struct` keyword'ü olmadan tanımlama

```mlp
-- YASAK:
MyStruct         -- 'struct' keyword'ü yok!
    numeric x
end_MyStruct
```

### ❌ 4. `end struct` yerine başka kapatıcı

```mlp
-- YASAK:
struct Point
    numeric x
end_Point        -- 'end struct' olmalı!
```

---

## Örnekler

### Örnek 1: Basit struct

```mlp
struct Person
    string name
    numeric age
end struct

function main()
    Person ali
    ali.name = "Ali Veli"
    ali.age = 30
    
    print(ali.name)
    print(ali.age)
    
end function
```

### Örnek 2: Method'lu struct

```mlp
struct Counter
    numeric value
end struct

function Counter.increment()
    this.value = this.value + 1
end function

function Counter.reset()
    this.value = 0
end function

function main()
    Counter c
    c.value = 0
    
    c.increment()
    c.increment()
    print(c.value)  -- 2
    
    c.reset()
    print(c.value)  -- 0
    
end function
```

### Örnek 3: İç içe struct

```mlp
struct Point
    numeric x
    numeric y
end struct

struct Rectangle
    Point topLeft
    Point bottomRight
end struct

numeric function Rectangle.width()
    return this.bottomRight.x - this.topLeft.x
end function

numeric function Rectangle.height()
    return this.bottomRight.y - this.topLeft.y
end function

function main()
    Rectangle rect
    rect.topLeft.x = 0
    rect.topLeft.y = 0
    rect.bottomRight.x = 10
    rect.bottomRight.y = 20
    
    print(rect.width())   -- 10
    print(rect.height())  -- 20
    
end function
```

### Örnek 4: Struct literal (Stage 1)

```mlp
struct Point
    numeric x
    numeric y
end struct

function main()
    Point p = {x: 10; y: 20}
    
    print(p.x)  -- 10
    print(p.y)  -- 20
    
end function
```

---

## Sık Sorulan Sorular

### S1: Struct tanımlamak için `struct` keyword'ü zorunlu mu?

**Cevap:** **EVET!** `struct` keyword'ü olmadan struct tanımlanamaz.

✅ Doğru: `struct Point ... end struct`  
❌ Yanlış: `Point ... end_Point`

---

### S2: `struct xxx end struct` geçerli bir yapı mı?

**Cevap:** **EVET!** Bu minimum geçerli struct syntax'ıdır (field'sız struct).

```mlp
struct Empty
end struct
```

Ancak pratikte en az bir field olmalı:

```mlp
struct Point
    numeric x
    numeric y
end struct
```

---

### S3: Constructor parametresi ile hızlı başlatma mümkün mü?

**Cevap:** **EVET!** (EKS-003, 8 Mart 2026 itibarıyla)

```mlp
struct Point(numeric x; numeric y)
    numeric x
    numeric y
end struct

Point p = Point(10; 20)  -- Constructor ile başlatma
print(p.x)   -- 10
```

Parametresiz klasik syntax da geçerli:
```mlp
struct Point
    numeric x
    numeric y
end struct

Point p
p.x = 10
p.y = 20
```

**Not:** Generic **tip** parametresi farklıdır:
```mlp
struct Container<T>  -- Bu geçerli (tip parametresi)
    value as T
end struct
```

---

### S4: Method'ları struct içinde tanımlayabilir miyim?

**Cevap:** **HAYIR!** MELP'te method'lar **struct dışında** tanımlanır.

❌ Yanlış:
```mlp
struct Point
    numeric function distance()  -- YASAK!
    end function
end struct
```

✅ Doğru:
```mlp
struct Point
    numeric x
    numeric y
end struct

numeric function Point.distance()
    return this.x * this.x + this.y * this.y
end function
```

---

### S5: `end struct` yerine `end_Point` yazabilir miyim?

**Cevap:** **HAYIR!** Kapatıcı **her zaman `end struct`** olmalıdır.

❌ Yanlış:
```mlp
struct Point
    numeric x
end_Point  -- HATA!
```

✅ Doğru:
```mlp
struct Point
    numeric x
end struct
```

---

### S6: Field'ları noktalı virgül ile ayırmak zorunlu mu?

**Cevap:** **HAYIR!** Hem newline hem semicolon geçerlidir.

✅ Newline ile:
```mlp
struct Point
    numeric x
    numeric y
end struct
```

✅ Semicolon ile:
```mlp
struct Point
    numeric x;
    numeric y;
end struct
```

---

### S7: `this` keyword'ü ne zaman kullanılır?

**Cevap:** Method içinde struct instance'ına erişmek için:

```mlp
struct Rectangle
    numeric width
    numeric height
end struct

numeric function Rectangle.area()
    return this.width * this.height  -- 'this' gerekli
end function
```

---

### S8: Bir struct başka struct field'i içerebilir mi?

**Cevap:** **EVET!** İç içe struct'lar desteklenir:

```mlp
struct Address
    string street
end struct

struct Person
    string name
    Address addr
end struct

Person p
p.addr.street = "Atatürk Cad."
```

---

### S9: Generic struct nasıl tanımlanır?

**Cevap:** Açılı parantez içinde tip parametresi:

```mlp
struct Box<T>
    value as T
end struct

Box<numeric> numbers
Box<string> texts
```

**Önemli:** Çoklu tip parametre için **noktalı virgül** kullan:
```mlp
struct Pair<T; U>
    first as T
    second as U
end struct
```

---

### S10: Struct dizisi nasıl oluşturulur?

**Cevap:** Struct adından sonra `[]`:

```mlp
struct Point
    numeric x
    numeric y
end struct

Point[] points
points = [{x: 0; y: 0}; {x: 10; y: 20}]
print(points[0].x)  -- 0
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_struct` | `end struct` |
| `end_Point` | `end struct` |
| `end_function` | `end function` |
| `method f() as numeric` | `numeric method f()` |
| `method f() as void` | `method f()` (void — `as void` yok) |
| `end_artir` | `end artir` |

---

## İlgili Belgeler

- [FUNCTION.md](FUNCTION.md) — Fonksiyon tanımlama
- [TYPES.md](TYPES.md) — MELP veri tipleri
- [GENERICS.md](GENERICS.md) — Generic tipler
- [INTERFACE.md](INTERFACE.md) — Interface tanımlama

---

## Compiler Implementation

**Lexer:** `compiler/stage0/lexer/lexer.cpp` (satır 264-265)  
**Parser:** `compiler/stage0/parser/parser_struct.cpp`  
**Codegen:** `compiler/stage0/codegen/struct_codegen.cpp`

---

**© 2026 MELP Project**  
**Lisans:** Bu belge MELP projesi kapsamında hazırlanmıştır.

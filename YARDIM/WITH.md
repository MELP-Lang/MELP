# MELP `with` Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ⏳ Planlandı — henüz implement edilmemiş (bkz. EKS-004)  
**Takip:** [EKSIKLER_8MART2026.md](../EKSIKLER_8MART2026.md) — EKS-004

---

## `with` Nedir?

`with`, bir struct instance'ının field'larına ve method'larına `obj.` prefix'i yazmadan erişmeyi sağlayan **syntax yardımcısı**dır.

Kotlin'deki `with`, VB.NET'teki `With` yapısından ilham alır.

---

## Temel Syntax

```mlp
with <struct_instance>
    <field> = <value>
    ...
end with
```

---

## Motivasyon

### Şu an (verbose):
```mlp
Person ali
ali.name = "Ali Veli"
ali.age = 30
ali.active = true
ali.score = 95
-- Her satırda "ali." tekrarı
```

### `with` ile (temiz):
```mlp
Person ali
with ali
    name = "Ali Veli"
    age = 30
    active = true
    score = 95
end with
```

---

## Kullanım Örnekleri

### Örnek 1: Basit struct initialization

```mlp
struct Rectangle
    numeric width
    numeric height
end struct

Rectangle r
with r
    width = 100
    height = 50
end with

print(r.width)   -- 100
print(r.height)  -- 50
```

### Örnek 2: Method çağrısı

```mlp
with r
    width = 100
    height = 50
    normalize()   -- r.normalize() olarak derlenir
end with
```

### Örnek 3: İç içe struct

```mlp
struct Address
    string street
    numeric zip
end struct

struct Person
    string name
    numeric age
    Address addr
end struct

Person ali
with ali
    name = "Ali Veli"
    age = 30
    with addr              -- ali.addr otomatik çözülür
        street = "Atatürk Cad."
        zip = 34000
    end with
end with
```

### Örnek 4: Okuma da yapılabilir

```mlp
with ali
    print(name)    -- ali.name okur
    print(age)     -- ali.age okur
end with
```

---

## Kurallar

### ✅ Geçerli Kullanım

```mlp
-- Struct instance ile:
with myStruct
    field = value
end with

-- İç içe:
with outer
    with inner
        field = value
    end with
end with
```

### ❌ Geçersiz Kullanım

```mlp
-- Struct olmayan değişkenle:
numeric x = 10
with x          -- ❌ HATA: x bir struct değil
    ???
end with

-- Çoklu nesne (henüz desteklenmiyor):
with a, b       -- ❌ Desteklenmiyor (belirsizlik riski)
    field = value
end with
```

---

## `with` vs `scope` Farkı

| Özellik | `with` | `scope` |
|---------|--------|---------|
| Amaç | Struct field erişim kısaltması | Labeled break / gruplama |
| Hedef | Struct instance | İsim (string) |
| Parametre | Struct instance | Yok |
| Erken çıkış | ❌ | ✅ `exit <isim>` |
| Compile-time dönüşüm | `field` → `obj.field` | Yok |

---

## Compiler Implementation (Planlandı)

**Lexer:** `compiler/stage0/lexer/lexer.cpp`
```
with     → WITH token
end with → END_WITH token
```

**Parser:** `compiler/stage0/parser/parser_struct.cpp`
```
parseWithBlock():
  - with <IDENTIFIER> tüket
  - Struct instance adını al
  - Body'deki field = value → obj.field = value AST'ye yaz
```

**Codegen:** `compiler/stage1/codegen/struct_codegen.cpp`
```
handleWithBlock():
  - Field erişimlerini struct member access'e çevir
```

---

## Sık Sorulan Sorular

### S1: `with` içinde `exit with` olur mu?
Hayır. `with` kontrol akışı bloğu değil — erken çıkış için `scope` kullan.

### S2: Struct literal `{x:10; y:20}` ile farkı ne?
```mlp
-- Struct literal (tek satır, initialization):
Point p = {x: 10; y: 20}

-- with (çok satır, mevcut instance'a set):
with p
    x = 10
    y = 20
end with
```

İkisi tamamlayıcı. Literal yeni instance için, `with` mevcut instance için.

### S3: Çoklu nesne (`with a, b`) ne zaman gelecek?
Belirsizlik riski nedeniyle ilk versiyonda desteklenmiyor. Tek nesne ile başlanacak.

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_with` | `end with` |
| `with a, b` (çoklu nesne) | henüz desteklenmiyor |
| `end_struct` | `end struct` |

---

## İlgili Belgeler

- [STRUCT.md](STRUCT.md) — Struct tanımlama
- [SCOPE.md](SCOPE.md) — İsimli kontrol akışı bloğu
- [FUNCTION.md](FUNCTION.md) — Fonksiyon tanımlama

---

**© 2026 MELP Project**

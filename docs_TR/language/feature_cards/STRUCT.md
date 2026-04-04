# Struct - MeLP Sozdizimi

**Durum:** ✅ COMPLETE (Stage1)
**Sozdizimi Surumu:** mlp1 / Stage1

---

## 📖 TEMEL SOZDIZIMI

Struct yalnizca veri tasir. Metotlar struct govdesi **disinda** `function StructAdi.method()` biciminde tanimlanir.

```mlp
struct Point
    numeric x
    numeric y
end struct
```

---

## ✅ TAM ORNEKLER

### Ornek 1 - Temel Struct
```mlp
struct Point
    numeric x
    numeric y
end struct

Point p
p.x = 10
p.y = 20
print(p.x)
```

### Ornek 2 - Dis Metotlarla Struct
```mlp
struct Rectangle
    numeric width
    numeric height
end struct

numeric function Rectangle.area()
    return this.width * this.height
end function

numeric function Rectangle.perimeter()
    return 2 * (this.width + this.height)
end function
```

### Ornek 3 - Ic Ice Struct
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
```

---

## ⚠️ KRITIK KURALLAR

1. Struct govdesinde metot tanimlanmaz.
2. Dis metot yazimi: `numeric function Rectangle.area()`.
3. Blok kapatici: `end struct`.
4. Alan erisimi `.` ile yapilir.
5. Metot icinde `this` aktif ornegi gosterir.

---

## 🚫 YANLIS KULLANIM

```mlp
-- ❌ YANLIS: struct icinde metot
struct Point
    numeric x
    function distance()
        return this.x
    end function
end struct
```

```mlp
-- ✅ DOGRU: metot disarda
struct Point
    numeric x
end struct

numeric function Point.distance()
    return this.x
end function
```

---

## 🔗 ILGILI KARTLAR

- [METHODS.md](METHODS.md) - dis metot tanimlari
- [STRUCT_CONSTRUCTOR.md](STRUCT_CONSTRUCTOR.md) - struct baslatma

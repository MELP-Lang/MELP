# Interface — MELP Başvuru

**Durum:** ✅ TAM — `interface` / `implements` tam çalışıyor (30/30 test)  
**Versiyon:** Stage 0 (pmlp0)  
**Test Tarihi:** 8 Mart 2026

---

## Özet

| Keyword | Açıklama | Durum |
|---------|----------|-------|
| `interface` / `end interface` | Arayüz tanımı | ✅ Tam |
| `implements` | Struct'ın bir interface'i uygulaması | ✅ Tam |
| `function Struct.method()` | Metod implementasyonu | ✅ Tam |

---

## Temel Kullanım

```mlp
interface Drawable
    numeric function draw()
end interface

struct Circle implements Drawable
    numeric radius
end struct

numeric function Circle.draw()
    return this.radius
end function

Circle c = Circle { radius: 42 }
print(c.draw())   -- 42
```

---

## interface Tanımı

```mlp
interface InterfaceAdi
    DonenTip function metod1()
    DonenTip function metod2(numeric parametre)
end interface
```

- `interface` ile başlar, `end interface` ile biter
- Yalnızca **fonksiyon imzaları** içerir (gövde yok)
- Parametrelere sahip metotlar tanımlanabilir
- `method` keyword'ü de kullanılabilir (bkz. STRUCT.md)

---

## implements Kullanımı

```mlp
struct StructAdi implements InterfaceAdi
    numeric alan
end struct
```

- `struct` satırında `implements InterfaceAdi` yazılır
- Struct, interface'deki tüm metotları implement etmek **zorundadır**

---

## Metod Implementasyonu

```mlp
numeric function Circle.draw()
    return this.radius
end function
```

- `function StructAdi.metod()` sözdizimi kullanılır
- `this.alan_adi` ile struct alanlarına erişilir

---

## Birden Fazla Metot

Bir interface birden fazla metot tanımlayabilir:

```mlp
interface Calculator
    numeric function double_val()
    numeric function triple_val()
    numeric function sum_dt()
end interface

struct NumHolder implements Calculator
    numeric n
end struct

numeric function NumHolder.double_val()
    numeric v = this.n
    return v + v
end function

numeric function NumHolder.triple_val()
    numeric v = this.n
    return v + v + v
end function

numeric function NumHolder.sum_dt()
    numeric v = this.n
    return v + v + v + v + v
end function

NumHolder h = NumHolder { n: 3 }
print(h.double_val())   -- 6
print(h.triple_val())   -- 9
print(h.sum_dt())       -- 15
```

---

## Birden Fazla Struct, Bir Interface

Aynı interface birden fazla struct tarafından uygulanabilir:

```mlp
interface Drawable
    numeric function draw()
end interface

struct Circle implements Drawable
    numeric radius
end struct

struct Square implements Drawable
    numeric side
end struct

numeric function Circle.draw()
    return this.radius
end function

numeric function Square.draw()
    return this.side
end function

Circle c = Circle { radius: 5 }
Square s = Square { side: 7 }
print(c.draw())   -- 5
print(s.draw())   -- 7
```

---

## Parametreli Metotlar

Interface metotları parametre alabilir:

```mlp
interface Scalable
    numeric function scale(numeric factor)
end interface

struct Box implements Scalable
    numeric size
end struct

numeric function Box.scale(numeric factor)
    return this.size * factor
end function

Box b = Box { size: 4 }
print(b.scale(3))   -- 12
```

---

## Kural ve Kısıtlar

| Kural | Açıklama |
|-------|----------|
| Static dispatch | Vtable/dynamic dispatch yok; çağrı derleme zamanında çözülür |
| Tüm metotlar implemente edilmeli | Eksik implementasyon derleme hatasına yol açar |
| Kısmi implementasyon yok | `default` metot desteği henüz yok |
| Bir struct tek interface | Birden fazla `implements` henüz desteklenmiyor |
| Interface değişken tipi yok | `Drawable d = Circle{...}` sözdizimi desteklenmiyor — her zaman somut tip kullanın |

---

## SSS

**S: Interface bir değişken tipi olarak kullanılabilir mi?**  
C: Hayır. `Drawable d = Circle{...}` yazılamaz. Her zaman `Circle c = Circle{...}` yazın ve `c.draw()` çağırın.

**S: Bir struct birden fazla interface implemente edebilir mi?**  
C: Henüz desteklenmiyor. Tek `implements` kullanılabilir.

**S: Interface metotlarının gövdesi olabilir mi?**  
C: Hayır. Interface yalnızca imza tanımlar. Gövde, struct metodunda yazılır.

**S: `method` keyword'ü ile `function` farkı var mı?**  
C: İkisi de çalışır. `function Struct.metod()` ile `method Struct.metod()` eşdeğerdir. Bkz. [STRUCT.md](STRUCT.md).

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_interface` | `end interface` |
| `end_function` | `end function` |
| `end_struct` | `end struct` |
| `function f() as numeric` | `numeric function f()` |

---

*Bağlantılı dosyalar: [STRUCT.md](STRUCT.md) · [FUNCTION.md](FUNCTION.md) · [KEYWORDS.md](KEYWORDS.md)*

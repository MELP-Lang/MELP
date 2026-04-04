# String İşlemleri — MELP Başvuru

**Durum:** ✅ concat/karşılaştırma/str() tam; `to_number()` ❌ (EKS-015)  
**Kaynak feature kartları:** `STRING_CONCAT.md`, `STR_BUILTIN.md`  
**Test tarihi:** 8 Mart 2026

---

## String Tanımlama

```mlp
string mesaj = "Merhaba Dünya"
string bos   = ""
string tek   = "A"
```

- Tip adı: **`string`** (küçük harf)
- Değerler çift tırnak içinde: `"metin"`
- Tek tırnak desteklenmez

---

## String Birleştirme (`+`)

```mlp
string a = "Merhaba"
string b = " Dünya"
string c = a + b
println(c)            -- Merhaba Dünya

-- Literal concat
string s = "foo" + "bar" + "baz"
println(s)            -- foobarbaz

-- Değişken ve literal
string ad = "Ali"
println("Merhaba " + ad + "!")   -- Merhaba Ali!
```

---

## `str()` — Sayı / Boolean → String

```mlp
numeric n = 42
println("n=" + str(n))       -- n=42
println(str(0))               -- 0
println(str(-7))              -- -7

println(str(true))            -- true
println(str(false))           -- false

-- Zincir kullanım
numeric a = 1
numeric b = 2
println("a=" + str(a) + " b=" + str(b))   -- a=1 b=2
```

**Anti-kalıplar:**
```mlp
-- ❌ YANLIŞ
print("Değer: " + n)        -- Hata: string + numeric

-- ✅ DOĞRU
print("Değer: " + str(n))
```

---

## String Karşılaştırma

```mlp
string x = "ali"

if x == "ali" then
    println("eşit")          -- eşit
end if

if x != "veli" then
    println("farklı")        -- farklı
end if

-- Boş string kontrolü
string bos = ""
if bos == "" then
    println("boş")           -- boş
end if
```

---

## `to_number()` — String → Numeric (EKSIK ❌)

```mlp
-- EKS-015: to_number() henüz uygulanmadı
string numStr = "123"
numeric sayi = to_number(numStr)   -- Derleme hatası: Unknown function
```

> ❌ `to_number()` fonksiyonu implemente edilmemiş — EKS-015. Sayısal giriş almak için `input()` kullanıp doğrudan `numeric` değişkene ata.

---

## `input()` — Kullanıcıdan String Al

```mlp
println("Adınızı girin:")
string ad = input()
println("Merhaba " + ad)
```

---

## Durum Özeti

| Özellik | Durum |
|---------|-------|
| `string` tanımlama | ✅ |
| `+` birleştirme | ✅ |
| `str(numeric)` | ✅ |
| `str(boolean)` | ✅ |
| `==` / `!=` karşılaştırma | ✅ |
| Boş string `""` kontrolü | ✅ |
| `to_number()` | ❌ EKS-015 |
| String içinde escape `\n` | ✅ (`write_file`'da çalışır) |
| String interpolasyon | ❌ Yok (concat kullan) |
| `.length` / `len()` | ❌ Yok |
| `substr()` / `indexOf()` | ❌ Yok |

---

## Sık Yapılan Hatalar

```mlp
-- ❌ YANLIŞ — sayı doğrudan yanına eklenemiyor
println("Değer: " + 42)

-- ✅ DOĞRU
println("Değer: " + str(42))

-- ❌ YANLIŞ — tek tırnak
string s = 'merhaba'

-- ✅ DOĞRU
string s = "merhaba"

-- ❌ YANLIŞ — tip belirtilmeden concat
ad + " Bey"   -- değişken referansı yok, anlamsız

-- ✅ DOĞRU
string tam = ad + " Bey"
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `print("Değer: " + n)` (sayıyı direkt birleştirme) | `print("Değer: " + str(n))` |
| `string s = 'merhaba'` (tek tırnak) | `string s = "merhaba"` |
| `end_if` | `end if` |
| `end_function` | `end function` |

---

*Bağlantılı dosyalar: [TYPES.md](TYPES.md) · [IO.md](IO.md) · [OPERATORS.md](OPERATORS.md)*

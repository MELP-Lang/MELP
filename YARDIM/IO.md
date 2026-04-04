# IO (Giriş / Çıkış) — MELP Başvuru

**Durum:** ✅ TAM — print/println/read_file/write_file/append_file  
**Kaynak feature kartları:** `belgeler/language/feature_cards/PRINT.md`, `FILE_IO.md`  
**Test tarihi:** 8 Mart 2026

---

## Standart Çıkış

### `print` ve `println`

```mlp
print(ifade)
println(ifade)
```

**Her ikisi de** çıktının sonuna `\n` satır sonu ekler.  
`println` ile `print` aralarında davranışsal **fark yoktur** — ikisi özdeştir.

```mlp
function main()
    print("Merhaba")         -- Sat: Merhaba
    println("Dünya")         -- Sat: Dünya

    numeric x = 42
    print(x)                 -- Sat: 42

    string s = "melp"
    print(s)                 -- Sat: melp

    print(x + 1)             -- Sat: 43
    print(s + " dili")       -- Sat: melp dili
end function
```

**Desteklenen ifade türleri:**
| İfade | Örnek | Çıktı |
|-------|-------|-------|
| String literali | `print("Merhaba")` | `Merhaba` |
| Numeric değişken | `print(x)` | sayı |
| String değişkeni | `print(s)` | metin |
| Aritmetik | `print(a + b)` | sonuç |
| String birleştirme | `print("x=" + str(x))` | `x=42` |

### `str()` — Sayıyı stringe çevir

```mlp
numeric n = 42
print("Değer: " + str(n))   -- Değer: 42
```

`print` bir argüman alır — sayı ile string birleştirmek için `str()` kullanılır.

---

## Dosya G/Ç

### `read_file` — Dosya oku

```mlp
string içerik = read_file("yol/dosya.txt")
```

- Döner: dosya içeriği (string)
- Hata / olmayan dosya: **boş string `""`** döner (SIGSEGV yapmaz)
- Parametre: tek argüman, yol string'i

```mlp
string veri = read_file("/tmp/girdi.txt")
if veri == "" then
    println("Dosya bulunamadı veya boş")
end if
```

### `write_file` — Dosya yaz (üzerine yaz)

```mlp
numeric başarı = write_file("yol"; içerik)
```

- Varsa dosyayı sıfırdan yazar, yoksa oluşturur
- Parametre ayracı: **`;`** (virgül değil)
- Döner: `1` (başarı) veya `0` (hata)

```mlp
numeric ok = write_file("/tmp/çıktı.txt"; "Satır 1\nSatır 2\n")
if ok == 1 then
    println("Yazıldı")
end if
```

### `append_file` — Dosyaya ekle

```mlp
numeric başarı = append_file("yol"; içerik)
```

- Dosya sonuna ekler, mevcut içeriği korur
- Parametre ayracı: **`;`**
- Döner: `1` (başarı) veya `0` (hata)

```mlp
numeric ok = append_file("/tmp/log.txt"; "Yeni kayıt\n")
```

---

## Tam Örnek

```mlp
function main()
    -- Dosya yaz
    numeric w = write_file("/tmp/deneme.txt"; "Satır 1\nSatır 2\n")
    println("Yazma: " + str(w))       -- Yazma: 1

    -- Dosya oku
    string icerik = read_file("/tmp/deneme.txt")
    println("İçerik:")
    println(icerik)

    -- Dosyaya ekle
    numeric a = append_file("/tmp/deneme.txt"; "Satır 3\n")
    println("Ekleme: " + str(a))      -- Ekleme: 1

    -- Tekrar oku
    string son = read_file("/tmp/deneme.txt")
    println("Son hal:")
    println(son)

    -- Olmayan dosya
    string yok = read_file("/tmp/olmayan_xxxx.txt")
    if yok == "" then
        println("Olmayan dosya: boş döndü (doğru)")
    end if

end function
```

**Çıktı:**
```
Yazma: 1
İçerik:
Satır 1
Satır 2

Ekleme: 1
Son hal:
Satır 1
Satır 2
Satır 3

Olmayan dosya: boş döndü (doğru)
```

---

## Önemli Notlar

| Konu | Açıklama |
|------|----------|
| Parametre ayracı | `write_file` / `append_file` → `;` kullan, virgül değil |
| Newline | `\n` escape sequence string içinde çalışır |
| Hata dönüşü | `read_file` → `""`, `write_file/append_file` → `0` |
| Dosya yolu | Executable'ın çalıştırıldığı dizine göreli veya mutlak yol |
| EKS-013 ✅ | `read_file` NULL segfault 8 Mart 2026'da giderildi |

## Sık Yapılan Hatalar

```mlp
-- ❌ YanlIŞ — virgülle ayırma
numeric ok = write_file("/tmp/x.txt", "içerik")

-- ✅ DOĞRU — noktalı virgülle
numeric ok = write_file("/tmp/x.txt"; "içerik")

-- ❌ YanlIŞ — string + numeric doğrudan
print("Başarı: " + ok)

-- ✅ DOĞRU — str() ile dönüştür
print("Başarı: " + str(ok))
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `end_if` | `end if` |
| `write_file("f", "x")` (virgül) | `write_file("f"; "x")` |
| `return 0` (main içinde) | kaldır |

---

*Bağlantılı dosyalar: [VARIABLE.md](VARIABLE.md) · [TYPES.md](TYPES.md) · [FUNCTION.md](FUNCTION.md)*

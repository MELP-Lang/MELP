# INPUT / READ_LINE / READ_ARG — MELP Hızlı Referans

**Özellik:** Kullanıcıdan ve komut satırından girdi alma  
**Durum:** ✅ `read_line()` + `read_arg(n)` tam çalışıyor | ❌ `input("prompt")` broken

---

## Özet Tablo

| Fonksiyon | Açıklama | Durum |
|-----------|----------|-------|
| `read_line()` | stdin'den bir satır oku | ✅ |
| `read_arg(n)` | n. komut satırı argümanını oku | ✅ |
| `input("prompt")` | Prompt gösterip stdin oku | ❌ Parser hatası |

---

## read_line() — stdin okuma

```mlp
function main()
    string isim = read_line()
    println("Merhaba, " + isim)
end function
```

**Çalıştırma:**
```bash
echo "Dunya" | ./program
# → Merhaba, Dunya
```

- Trailing newline (`\n`) otomatik kaldırılır
- EOF'ta boş string döner (NULL değil)

---

## read_arg(n) — Komut satırı argümanı

```mlp
function main()
    string ad   = read_arg(1)   -- İlk argüman (argv[1])
    string soyad = read_arg(2)  -- İkinci argüman (argv[2])
    println(ad + " " + soyad)
end function
```

**Çalıştırma:**
```bash
./program Ali Veli
# → Ali Veli
```

- `read_arg(0)` = program adının kendisi
- Argüman yoksa boş string döner

---

## Tip Dönüşümü

```mlp
function main()
    string n_str = read_line()
    numeric n = str_to_num(n_str)   -- string → numeric
    println("Kare: " + str(n * n))
end function
```

**Not:** `to_number()` implement edilmemiş (EKS-015). `str_to_num()` de henüz test edilmemiş.  
Geçici çözüm:
```mlp
-- external ile atoll kullan
external function atoll(string s) as numeric

function main()
    string n_str = read_line()
    numeric n = atoll(n_str)
    println("Kare: " + str(n * n))
end function
```

---

## Sınırlamalar

| Özellik | Durum |
|---------|-------|
| `read_line()` | ✅ Tam çalışıyor |
| `read_arg(n)` | ✅ Tam çalışıyor |
| `input("prompt")` | ❌ Parser hatası: `Expected expression` |
| `to_number(str)` | ❌ EKS-015: implement edilmemiş |
| Satır sayısı sınırı | Yok (getline ile dinamik okuma) |

---

## Gerçek Hayat Örneği

```mlp
function main()
    -- Argüman verildiyse kullan, yoksa stdin oku
    string deger = read_arg(1)
    if deger == "" then
        deger = read_line()
    end if
    println("Girdi: " + deger)
end function
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `end_if` | `end if` |
| `return 0` (main içinde) | kaldır |
| `input("prompt")` | `read_line()` (prompt için önceden `print` kullan) |

---

## İlgili YARDIM Dosyaları

- [IO.md](IO.md) — Dosya I/O (write_file, read_file, append_file)
- [EXTERNAL.md](EXTERNAL.md) — C fonksiyonu bildirimi
- [KEYWORDS.md](KEYWORDS.md) — Tüm keyword listesi

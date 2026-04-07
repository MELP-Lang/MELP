# MELP — Döngüler

**Durum:** ✅ Tüm döngü tipleri çalışıyor  
**Versiyon:** Stage 0 (pmlp0)

---

## Döngü Tipleri Özeti

| Döngü | Kullanım | Durum |
|-------|----------|-------|
| `loop` | Koşul döngüsü | ✅ Tam |
| `for ... to` | Sayaç döngüsü (artan) | ✅ Tam |
| `for ... downto` | Sayaç döngüsü (azalan) | ✅ Tam |
| `for ... step` | Adım belirtmeli sayaç | ✅ Tam |
| `for each ... in` | Koleksiyon döngüsü | ✅ Tam |

---

## while

```mlp
loop koşul
    -- gövde
end loop
```

### Örnek

```mlp
numeric i = 0
loop i < 10
    print(i)
    i = i + 1
end loop
-- Çıktı: 0 1 2 3 4 5 6 7 8 9
```

### Kritik Kurallar

| Kural | Doğru | Yanlış |
|-------|-------|--------|
| `do` kelimesi yok | `loop i < 10` | `loop i < 10 do` |
| Kapatıcı | `end loop` | `end while` |
| Parantez gerekmez | `loop i < 10` | `loop (i < 10)` |

---

## for ... to (Artma)

```mlp
loop i = başlangıç to bitiş
    -- gövde
end loop
```

```mlp
loop i = 1 to 5
    println(i)
end loop
-- Çıktı: 1  2  3  4  5  (bitiş dahil)
```

---

## for ... downto (Azalma)

```mlp
loop i = 10 downto 1
    println(i)
end loop
-- Çıktı: 10  9  8  ...  1  (bitiş dahil)
```

---

## for ... step (Adım)

```mlp
loop i = 0 to 100 step 10
    println(i)
end loop
-- Çıktı: 0  10  20  30  ...  100
```

Negatif adım için `downto` ile kullanın:

```mlp
loop i = 100 downto 0 step 5
    println(i)
end loop
-- Çıktı: 100  95  90  ...  0
```

---

## for each ... in (Koleksiyon)

```mlp
loop each eleman in koleksiyon
    -- gövde
end loop
```

### Array üzerinde

```mlp
numeric[] sayilar = [10; 20; 30; 40; 50]
loop each n in sayilar
    println(n)
end loop
```

### String listesi üzerinde

```mlp
string[] isimler = ["Ali"; "Veli"; "Ayşe"]
loop each isim in isimler
    println("Merhaba " + isim)
end loop
```

### İç içe döngü

```mlp
numeric[][] matris = [[1; 2; 3]; [4; 5; 6]]
loop each satir in matris
    loop each hucre in satir
        print(hucre)
    end loop
end loop
```

> `for each` klasik `for i = 0; i < n; i++` yerine geçer. İndeks ihtiyacında `while` kullanın.

---

## Döngü Kontrolü

### exit — döngüyü sonlandır (break)

```mlp
numeric i = 0
loop true
    if i >= 5 then
        exit             -- döngüden çık
    end if
    println(i)
    i = i + 1
end loop
```

### continue — sonraki iterasyona geç

```mlp
loop i = 1 to 10
    if i mod 2 == 0 then
        continue_for     -- çift sayıları atla
    end if
    println(i)           -- sadece tek sayılar
end loop
```

| Keyword | Anlamı | Bağlamı |
|---------|--------|---------|
| `exit` | Döngüyü bitir | Genel |
| `continue` | Sonraki adım | Genel |
| `continue_for` | Sonraki for adımı | `for` içinde |
| `continue_while` | Sonraki loop adımı | `loop` içinde |

### İsimli döngü çıkışı

İç içe döngülerde belirli bir döngüden çıkmak için `scope` kullanın:

```mlp
scope disKontrol
    loop i = 1 to 5
        loop j = 1 to 5
            if i * j > 10 then
                exit_disKontrol     -- dış scope'dan çık
            end if
            println(i * j)
        end loop
    end loop
end disKontrol
```

> Daha fazlası için: [SCOPE.md](SCOPE.md)

---

## Pratik Örnekler

### 1 ile N arasındaki toplam

```mlp
numeric function toplam(numeric n)
    numeric sonuc = 0
    loop i = 1 to n
        sonuc = sonuc + i
    end loop
    return sonuc
end function
```

### Dizi elemanlarının toplamı

```mlp
numeric function dizi_toplam(numeric[] arr)
    numeric toplam = 0
    loop each n in arr
        toplam = toplam + n
    end loop
    return toplam
end function
```

### Geri sayım

```mlp
loop i = 10 downto 1
    println(i)
end loop
println("Ateş!")
```

---

## Anti-Pattern'lar

```mlp
-- ❌ C stili for (MELP'te yok)
for (i = 0; i < 10; i++) { ... }

-- ❌ Python stili for (MELP'te yok)
loop i in range(10): ...

-- ❌ while'da do kullanma
loop i < 10 do ...

-- ❌ end_for (alt çizgili) YANLIŞ
-- ✅ DOĞRU: end for (boşluklu)

-- ✅ DOĞRU
loop i = 0 to 9
    println(i)
end loop
```

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `while koşul` | `loop koşul` |
| `end_while` | `end loop` |
| `end_for` | `end for` |
| `end_function` | `end function` |

---

## SSS

**S: `for` döngüsünde bitiş değeri dahil mi?**  
C: Evet. `for i = 1 to 5` → 1, 2, 3, 4, **5** (bitiş dahil).

**S: `exit` sadece döngüden mi çıkar?**  
C: Evet. `break` yerine `exit` kullanılır. İsimli scope çıkışı için `exit_isim` kullanın.

**S: `continue` ile `continue_for` farkı nedir?**  
C: Fonksiyon olarak aynı iş. `continue_for`, hangi döngü tipinde olduğunu açıkça belirtir.

**S: İndeks ile koleksiyon üzerinde dönebilir miyim?**  
C: `for each` indeks vermez. İndeks gerekiyorsa `for i = 0 to uzunluk-1` ve `dizi[i]` kullanın.

**S: Sonsuz döngü nasıl yapılır?**
C: `loop true ... end loop`. Çıkış için `exit` kullanın.

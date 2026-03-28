# COLLECTIONS (Koleksiyonlar) — Ozellik Karti

**Ozellik:** Array `[]`, List `()`, Tuple `<>` — Koleksiyon tipleri  
**Durum:** ✅ COMPLETE (Stage1 — Array ve List; Tuple kismi)  
**Sozdizimi Surumu:** mlp0/mlp1

---

## 🎯 OZELLIK

MELP'te uc yerlesik koleksiyon tipi vardir. Ayirici **daima `;`** — asla `,` kullanma. HashMap Stage 1+ olarak planlanmistir.

---

## 📖 SOZDIZIMI

```mlp
-- Array: sabit tip, indeksli erisim
numeric[] dizi = [1; 2; 3; 4; 5]
numeric ilk = dizi[0]
numeric son = dizi[4]
numeric uzunluk = length(dizi)

-- Bos array
string[] isimler = []

-- 2D array
numeric[][] matris = [[1; 2]; [3; 4]]

-- List: dinamik, sirali
List{string} kelimeler = ("merhaba"; "dunya")
kelimeler.add("!")
string bas = kelimeler[0]

-- Tuple: heterojen, sabit uzunluklu
Tuple{numeric; string} cift = <42; "Ali">
numeric id = cift.0
string isim = cift.1

-- Tuple ile cok donus degeri
Tuple{numeric; numeric} function koordinat_al()
    return <10; 20>
end function

Tuple{numeric; numeric} konum = koordinat_al()
print(konum.0)   -- 10
print(konum.1)   -- 20
```

---

## ⚠️ AYIRICI KURALI (KRITIK)

```mlp
-- ✅ DOGRU: Noktalı virgul
[1; 2; 3]
("a"; "b"; "c")
<1; "Ali">

-- ❌ YANLIS: Virgul KULLANMA
[1, 2, 3]     -- HATA
("a", "b")    -- HATA
```

---

## 📌 HashMap (Planlı — Stage 1+)

```mlp
-- ⚠️ Henuz yok — tahmini sozdizimi:
-- HashMap{string; numeric} puanlar = {"Ali": 95; "Veli": 87}
```

---

## 🧪 GOLDEN TEST

```mlp
numeric function topla(numeric[] dizi)
    numeric toplam = 0
    loop each n in dizi
        toplam = toplam + n
    end loop
    return toplam
end function

function main()
    numeric[] sayilar = [10; 20; 30]
    numeric t = topla(sayilar)
    if t == 60 then return 1 end if
end function
```

---

## 🔗 ILGILI KARTLAR

- [ARRAY.md](ARRAY.md) — detayli array referansi
- [FOR_LOOP.md](FOR_LOOP.md) — koleksiyon gezme

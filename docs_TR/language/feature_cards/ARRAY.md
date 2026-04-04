# Array (Dizi) — MELP Sozdizimi

**Durum:** ✅ COMPLETE (Stage1)  
**Sozdizimi Surumu:** mlp1 / Stage1

---

## 🎯 OZELLIK

Ayni tipte elemanlari indeksli tutan sabit-tipli koleksiyon. Ayirici **daima `;`** — asla `,` kullanma.

---

## 📖 SOZDIZIMI

```mlp
numeric[] dizi = [eleman1; eleman2; eleman3]
```

---

## ✅ ORNEKLER

### Ornek 1 — Numerik Dizi
```mlp
numeric[] sayilar = [1; 2; 3; 4; 5]
print(sayilar[0])
-- Cikti: 1
```

### Ornek 2 — Dizi Uzunlugu
```mlp
numeric[] veri = [10; 20; 30]
numeric uzunluk = veri.length
print(uzunluk)
-- Cikti: 3
```

### Ornek 3 — Eleman Guncelleme
```mlp
numeric[] degerler = [1; 2; 3]
degerler[1] = 99
print(degerler[1])
-- Cikti: 99
```

### Ornek 4 — Dongu ile Gezme
```mlp
numeric[] ogeler = [5; 10; 15]
loop i = 0 to 2
    print(ogeler[i])
end loop
-- Cikti: 5
-- Cikti: 10
-- Cikti: 15
```

### Ornek 5 — Fonksiyon Icinde
```mlp
function main()
    numeric[] puanlar = [85; 90; 78]
    numeric toplam = 0
    loop i = 0 to 2
        toplam = toplam + puanlar[i]
    end loop
    print(toplam)
end function
-- Cikti: 253
```

---

## ⚠️ KRITIK KURALLAR

1. Indeksler **0 tabanlidir**.
2. Sinir disi erisim calisma zamani hatasidir.
3. Dizi eleman ayirici: `;` (noktalı virgul, asla virgul degil).
4. `length`, salt okunur bir ozelliktir; metod cagrisi degildir.

---

## 🚫 YANLIS KULLANIM

```mlp
-- ❌ YANLIS: virgul ayirici
numeric[] sayilar = [1, 2, 3]

-- ✅ DOGRU: noktalı virgul kullan
numeric[] sayilar = [1; 2; 3]
```

---

## 🔗 ILGILI KARTLAR

- [COLLECTIONS.md](COLLECTIONS.md) — List, Tuple, HashMap
- [FOR_LOOP.md](FOR_LOOP.md) — dizi gezme

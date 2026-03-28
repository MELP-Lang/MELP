# Closure (Kapatma) — MELP Sozdizimi

**Durum:** ✅ KISMI (satir ici closure calisiyor; mutasyon yakalama henuz yok)  
**Sozdizimi Surumu:** mlp1 / Stage1

---

## 🎯 OZELLIK

Closure, dis kapsamdan degisken yakalayan bir lambda ifadesidir. MELP'te closure'lar degeri kopyalayarak yakalar (degismez yakalama).

---

## 📖 SOZDIZIMI

```mlp
-- Lambda dis kapsamdaki "carpan" degiskenini yakalar
numeric carpan = 3
numeric carp = lambda(x) -> x * carpan
```

---

## ✅ ORNEKLER

### Ornek 1 — Temel Closure
```mlp
function main()
    numeric taban = 5
    numeric taban_ekle = lambda(x) -> x + taban
    print(taban_ekle(10))
    print(taban_ekle(3))
end function
-- Cikti: 15
-- Cikti: 8
```

### Ornek 2 — Fonksiyondan Closure Donus
```mlp
numeric function ekleyici_yap(numeric x)
    return lambda(y) -> x + y
end function

function main()
    numeric ekle5 = ekleyici_yap(5)
    print(ekle5(10))
end function
-- Cikti: 15
```

### Ornek 3 — Birden Fazla Closure
```mlp
function main()
    numeric taban = 10
    numeric taban_ekle = lambda(x) -> x + taban
    numeric taban_carp = lambda(x) -> x * taban

    print(taban_ekle(5))
    print(taban_carp(3))
end function
-- Cikti: 15
-- Cikti: 30
```

---

## ⚠️ KRITIK KURALLAR

1. Closure'lar dis degiskenleri **deger olarak yakalar** (degismez yakalama).
2. Mutasyon yakalama (`sayac = sayac + 1` pattern'i) **henuz desteklenmez**.
3. Blok lambda (`end lambda`) **henuz calismiyor** — `-> ifade` yazimini kullan.
4. Parametre ayirici: `;` (noktalı virgul).

---

## 🚫 YANLIS KULLANIM

```mlp
-- ❌ YANLIS: blok lambda (desteklenmiyor)
return lambda(y)
    return x + y
end lambda
```

```mlp
-- ✅ DOGRU: satir ici lambda
return lambda(y) -> x + y
```

---

## 🔗 ILGILI KARTLAR

- [LAMBDA.md](LAMBDA.md) — lambda ifadeleri

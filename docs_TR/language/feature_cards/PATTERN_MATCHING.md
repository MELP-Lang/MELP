# Pattern Matching - MeLP Sozdizimi

**Durum:** ⚠️ WIP (Stage1)
**Sozdizimi Surumu:** mlp1 / Stage1

---

## 📖 TEMEL SOZDIZIMI

```mlp
match ifade
    case deger then -- govde
    case deger then -- govde
    else -- varsayilan govde
end match
```

---

## ✅ TAM ORNEKLER

### Ornek 1 - Temel Match
```mlp
numeric x = 5
match x
    case 1 then print("bir")
    case 2 then print("iki")
    case 5 then print("bes")
    else print("diger")
end match
```

### Ornek 2 - Match ifadesi olarak
```mlp
numeric x = 2
string msg = match x
    case 1 then "bir"
    case 2 then "iki"
    else "diger"
end match
print(msg)
```

### Ornek 3 - Guard kosulu
```mlp
numeric n = 15
match n
    case x if x < 10 then print("kucuk")
    case x if x < 20 then print("orta")
    else print("buyuk")
end match
```

---

## ⚠️ KRITIK KURALLAR

1. `match` yapisi `end match` ile kapanir.
2. `else` varsayilan/wildcard dalidir.
3. Guard yazimi: `case x if kosul then`.
4. Match bir deger ureten ifade olarak kullanilabilir.
5. `function main()` icinde `return 0/1` yazilmaz.

---

## 🚫 YANLIS KULLANIM

```mlp
-- ❌ YANLIS: donus tipi oku
function classify(numeric n) -> string
    return "x"
end function
```

```mlp
-- ✅ DOGRU
string function classify(numeric n)
    return "x"
end function
```

---

## 🔗 ILGILI KARTLAR

- [ENUM.md](ENUM.md) - match icin enumlar
- [SWITCH_CASE.md](SWITCH_CASE.md) - alternatif kontrol akisi
- [IF_STATEMENT.md](IF_STATEMENT.md) - kosullu akis

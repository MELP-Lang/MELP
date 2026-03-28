# Loop Statement - MELP Sozdizimi

**Stage:** mlp1 (Stage1)  
**Modul:** Dongu kontrol yapisi (`while`/`do-while` yerine)

---

## 📖 TEMEL SOZDIZIMI

```mlp
-- Kosul basta (pre-check)
loop kosul
    -- govde
end loop

-- Kosul sonda (post-check)
loop
    -- govde
end loop kosul

-- Sonsuz dongu
loop
    -- govde (cikmak icin exit)
end loop
```

---

## ✅ TAM ORNEKLER

### Pre-check (while benzeri)
```mlp
numeric i = 0
loop i < 10
    print(i)
    i = i + 1
end loop
```

### Post-check (en az bir kez calisir)
```mlp
numeric i = 0
loop
    print(i)
    i = i + 1
end loop i < 10
```

### Sonsuz Dongu
```mlp
loop
    if bitti then exit
    process()
end loop
```

---

## ⚠️ KRITIK KURALLAR

1. **Kosulun yeri anlami belirler**
2. **Blok kapatici: `end loop`**
3. **`not` ile until semantigi kurulabilir**
4. **`while` yerine `loop` kullanin**

---

## 🚫 ANTI-PATTERNLER

### ❌ Eski `while` kullanimi
```mlp
loop i < 10
    print(i)
end loop
```

### ✅ DOGRU MELP
```mlp
loop i < 10
    print(i)
    i = i + 1
end loop
```

---

## 📚 GOLDEN REFERENCE

**Dosya:** [golden_reference0.mlp](../golden_reference0.mlp)  
**Ara:** golden reference icinde "loop" ornekleri

---

**Referans:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)

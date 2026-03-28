# If Statement - MELP Sozdizimi

**Durum:** ✅ COMPLETE (Stage1)  
**Sozdizimi Surumu:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Modul:** Kosullu kontrol yapisi

---

## 📖 TEMEL SOZDIZIMI

```mlp
if kosul then
    -- govde
end if
```

---

## ✅ ELSE ILE

```mlp
if kosul then
    -- dogru dal
else
    -- yanlis dal
end if
```

---

## ✅ ELSE IF ILE

```mlp
if kosul1 then
    -- birinci dal
else
    if kosul2 then
        -- ikinci dal
    else
        if kosul3 then
            -- ucuncu dal
        else
            -- varsayilan dal
        end if
    end if
end if
```

---

## ⚠️ KRITIK KURALLAR

1. **`if` mutlaka `then` icermelidir**
   - ✅ `if x > 5 then`
   - ❌ `if x > 5`
   - ❌ `if x > 5:`

2. **Blok kapatici: `end if`**
    - ✅ `end if`

3. **Else-if boslukla yazilir: `else if`**
    - ✅ `else if kosul then`
    - ❌ `elif`

4. **Parantez zorunlu degildir**

---

## 🚫 ANTI-PATTERNLER

### ❌ Python stili
```python
if x > 5:
    print("buyuk")
elif x > 2:
    print("orta")
else:
    print("kucuk")
```

### ❌ `then` eksik
```mlp
if x > 5
    print("buyuk")
end if
```

### ✅ DOGRU MELP
```mlp
if x > 5 then
    print("buyuk")
else
    if x > 2 then
        print("orta")
    else
        print("kucuk")
    end if
end if
```

---

## 📚 TAM ORNEK

```mlp
numeric score = 85

if score >= 90 then
    print("Not: A")
else
    if score >= 80 then
        print("Not: B")
    else
        if score >= 70 then
            print("Not: C")
        else
            if score >= 60 then
                print("Not: D")
            else
                print("Not: F")
            end if
        end if
    end if
end if
```

---

## 📚 GOLDEN REFERENCE

**Dosya:** [test_mlp_golden_reference.mlp](../../test_mlp_golden_reference.mlp)  
**Ara:** golden reference icinde "if" ornekleri

---

**Referans:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)

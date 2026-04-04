# Function Definition - MELP Sozdizimi

**Durum:** ✅ COMPLETE (Stage1)  
**Sozdizimi Surumu:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Modul:** Fonksiyon bildirimi ve tanimi

---

## 📖 TEMEL SOZDIZIMI

```mlp
donus_tipi function isim(tip param1; tip param2)
    -- govde
    return deger
end function
```

---

## ✅ TAM ORNEKLER

### Basit Fonksiyon
```mlp
numeric function topla(numeric x; numeric y)
    return x + y
end function

numeric sonuc = topla(10; 20)  -- 30
```

### String Fonksiyonu
```mlp
string function selamla(string isim)
    return "Merhaba, " + isim
end function

string mesaj = selamla("Ali")  -- "Merhaba, Ali"
```

### Donussuz Fonksiyon
```mlp
function selam_yaz(string isim)
    print("Merhaba, " + isim)
end function

selam_yaz("Veli")
```

---

## ⚠️ KRITIK KURALLAR

1. **Parametre formati: `tip isim`**
   - ✅ `function topla(numeric x; numeric y)`
   - ❌ `function topla(x: numeric, y: numeric)`

2. **Parametre ayirici: `;` (noktali virgul)**
   - ✅ `(numeric x; numeric y)`
   - ❌ `(numeric x, numeric y)`

3. **Donus tipi `function` anahtar sozcugunden once gelir**
   - ✅ `numeric function foo()`
   - ❌ `function foo() -> numeric`
   - ❌ `function foo(): numeric`

4. **Blok kapatici: `end function`**
    - ✅ `end function`

5. **Donus tipi `none` degilse `return` zorunlu**

---

## 🚫 ANTI-PATTERNLER

### ❌ Python stili
```python
def topla(x, y):
    return x + y
```

### ❌ C stili
```c
int topla(int x, int y) {
    return x + y;
}
```

### ✅ DOGRU MELP
```mlp
numeric function topla(numeric x; numeric y)
    return x + y
end function
```

---

## 📚 GOLDEN REFERENCE

**Dosya:** [test_mlp_golden_reference.mlp](../../test_mlp_golden_reference.mlp)  
**Ara:** golden reference icinde "function" ornekleri

---

**Referans:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)

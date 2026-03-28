# For Loop - MELP Sozdizimi

**Durum:** ⚠️ PARTIAL (Stage1 - downto/step sureci suruyor)  
**Sozdizimi Surumu:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Modul:** For dongu kontrol yapisi

---

## 📖 TEMEL SOZDIZIMI (Artan)

```mlp
loop degisken = baslangic to bitis
    -- govde
end loop
```

---

## 📖 AZALAN SOZDIZIMI

```mlp
loop degisken = baslangic downto bitis
    -- govde
end loop
```

---

## ✅ TAM ORNEKLER

### Artan Dongu
```mlp
loop i = 0 to 10
    print(i)
end loop
```

### Azalan Dongu
```mlp
loop i = 10 downto 0
    print(i)
end loop
```

### Adimli Dongu (Opsiyonel)
```mlp
loop i = 0 to 100 step 10
    print(i)
end loop
```

---

## ⚠️ KRITIK KURALLAR

1. **`to` veya `downto` kullanin**
2. **Blok kapatici: `end for`**
3. **C/Python for formu kullanmayin**
4. **`step` opsiyoneldir**

---

## 🚫 ANTI-PATTERNLER

### ❌ Python stili
```python
loop i in range(10):
    print(i)
```

### ❌ C stili
```c
for (int i = 0; i < 10; i++) {
    printf("%d\n", i);
}
```

### ✅ DOGRU MELP
```mlp
loop i = 0 to 10
    print(i)
end loop
```

---

## 📚 PRATIK ORNEKLER

### Dizi Uzerinde Gezinme
```mlp
numeric[] sayilar = [1; 2; 3; 4; 5]
loop i = 0 to 4
    print(sayilar[i])
end loop
```

### Toplam Hesabi
```mlp
numeric toplam = 0
loop i = 1 to 100
    toplam = toplam + i
end loop
print(toplam)
```

---

## 📚 GOLDEN REFERENCE

**Dosya:** [test_mlp_golden_reference.mlp](../../test_mlp_golden_reference.mlp)  
**Ara:** golden reference icinde "for" ornekleri

---

**Referans:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)

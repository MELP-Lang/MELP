# Variable Declaration - MELP Sozdizimi

**Durum:** ✅ COMPLETE (Stage1)  
**Sozdizimi Surumu:** mlp1 / Stage1  

**Stage:** mlp1 (Stage1)  
**Modul:** Degisken tanimlama ve atama

---

## 📖 TEMEL SOZDIZIMI

```mlp
tip isim = deger
```

---

## ✅ TEMEL TIPLER

### Numeric
```mlp
numeric x = 42
numeric pi = 3,14159
numeric million = 1.000.000
```

### String
```mlp
string name = "Ali"
string greeting = "Merhaba"
string bos = ""
```

### Boolean
```mlp
boolean flag = true
boolean is_valid = false
```

---

## ⚠️ KRITIK KURALLAR

1. **Satir sonunda `;` kullanmayin**
2. **Tip once gelir**
3. **Turkce sayi formati kullanilir**
4. **Metin tipi `string`dir**

---

## 🚫 ANTI-PATTERNLER

### ❌ Python stili
```python
x = 42
name = "Ali"
```

### ❌ JavaScript stili
```javascript
let x = 42;
const name = "Ali";
```

### ✅ DOGRU MELP
```mlp
numeric x = 42
string name = "Ali"
boolean flag = true
```

---

## 📚 ATAMA (Tanimdan Sonra)

```mlp
numeric x = 42
x = 100
```

---

## 📚 TURKCE SAYI FORMATI

```mlp
numeric pi = 3,14159
numeric thousand = 1.000
numeric big = 126.555.852,36
```

---

## 📚 GOLDEN REFERENCE

**Dosya:** [test_mlp_golden_reference.mlp](../../test_mlp_golden_reference.mlp)  
**Ara:** degisken tanim ornekleri

---

**Referans:** [mlp0_syntax.md](../mlp0_syntax.md) | [mlp_sozdizimi.md](../../mlp_sozdizimi.md)

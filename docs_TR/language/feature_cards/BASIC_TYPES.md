# BASIC_TYPES - Ozellik Karti

**Ozellik:** Temel tipler - `numeric`, `string`, `boolean`, `null`, `void`  
**Durum:** ✅ COMPLETE (Stage1)  
**Hafta:** Stage1  
**Sozdizimi Surumu:** mlp0

---

## 🎯 OZELLIK

MELP'te tum sayilar `numeric` tipindedir. `string` metin, `boolean` mantik tipi olarak kullanilir. Turkce ondalik yaziminda virgul (` , `) ayirici olarak kullanilir.

---

## 📖 SOZDIZIMI

```mlp
numeric x = 42
numeric pi = 3,14
string s = "merhaba"
boolean flag = true
numeric? n = null

function log(string msg)
    print(msg)
end function
```

---

## 🧪 GOLDEN TEST

```mlp
function main()
    numeric i = 10
    numeric f = 2,5
    string s = "test"
    boolean b = true

    if i == 10 then
        if f == 2,5 then
            if s == "test" then
                if b == true then
                    print("all types ok")
                end if
            end if
        end if
    end if
end function
```

---

## ⚠️ KRITIK NOTLAR

1. MELP'te `int/float` ayrimi yok, tek sayisal tip `numeric`tir.
2. `var`/`let` yerine acik tip bildirimi zorunludur.
3. `str()`, `to_number()`, `to_boolean()` tip donusumunde kullanilir.

---

**Bagimliliklar:** Yok (temel)  
**Kanonik Kaynak:** `mlp0_sozdizimi.md` - Temel tipler bolumu

# MELP Sugar Syntax Önerileri - Görev 01 ile Başlayan Keşifler

> Bu dosya, her görevde keşfedilen ve denenen sugar syntax önerilerini belgelemektedir.
> **Durum Konvansiyon:**
> - ✅ **Compiler destekliyor** — Test edildiki, çalışmakta
> - ❌ **Normalizer gerektirir** — Compiler desteklemiyor, normalizer layer'ı gerekli
> - ⚠️ **Test edilmedi** — Keşfedildi ama test edilecek

---

## Görev 01: hello_world

### 1️⃣ program bloğu — Giriş Noktası Sugar

**Canonical .mlp:**
```mlp
function main()
    print("Merhaba, MELP!")
end_function
```

**Sugar (ONAYLANDI — ÜA kararı 2026-03-03):**
```mlp
program
    print("Merhaba, MELP!")
end_program
```

**Gerekçe:**
- `function main() as numeric` + `return 0` C kalıntısı — MLP'de YASAK
- Canonical form zaten `function main()` — dönüş tipi ve return 0 yok
- `program` / `end_program` MELP'in `end_xxx` pattern'iyle tutarlı
- `end` tek başına iç içe yapılarda belirsizlik yaratır — REDDEDILDI
- `class` yok, `struct` var — `program` da aynı felsefede

**Normalizer dönüşümü:**
```
program          →  function main()
end_program      →  end_function
```

**Durum:** ✅ Karar verildi — Normalizer implementasyonu bekliyor

---

### 2️⃣ String Interpolation

**pmlp canonical:**
```pmlp
string lang = "MELP"
print("Merhaba, " + lang + "!")
```

**Önerilen sugar:**
```pmlp
string lang = "MELP"
print("Merhaba, {lang}!")
```

**Durum:** ⚠️ **Test edilmedi**
**Not:** String interpolation özelliği test edilmedi. Compiler kodu parse ederken hata verebilir.

---

### 3️⃣ println() — Açık Newline ile Print

**pmlp canonical:**
```pmlp
print("Merhaba, MELP!")
```

**Önerilen sugar:**
```pmlp
println("Merhaba, MELP!")
```

**Durum:** ⚠️ **Test edilmedi**
**Not:** PRINT.md'de `println()` bahsediliyor ama test edilmedi.

---

## Görev 02-04: Variables, Arithmetic, String Ops

**Not:** Temel syntax doğru, sugar gerekmez.

---

## Görev 05-06: Boolean & If/Else

**Operators:** `and`, `or`, `not` — C-style (`&&`, `||`, `!`) YASAK
**Durum:** ✅ Doğru çalışıyor

---

## Görev 07: while_loop

### exit — Loop Break

**Canonical:** Şu an mümkün değil
**Önerilen:**
```pmlp
while condition
    if break_condition then
        exit
    end_if
end_while
```

**Durum:** 🚫 **Eksik** — TODO_KURALLARI'nda var, ama compiler'da implement edilmedi

---

## Görev 08: for_loop

**Durum:** ✅ `for...to`, `for...downto`, `for...step` doğru çalışıyor. Sugar gerekmez.

---

## Görev 09: functions

### String Return Type Bug

**Problem:** `function greet(string name) as string` — Backend crash

**Durum:** 🚫 **Backend bug** — LLVM type mismatch

---

## Görev 10: arrays

### Array Runtime Crash

**Problem:** `nums[0]` segfault

**Durum:** 🚫 **Critical** — Array feature broken

---

## Özet: Compiler Eksiklikleri

| Feature | Durum | Görev |
|---------|-------|-------|
| `program/end_program` | ❌ Normalizer | 01 |
| `exit` keyword | 🚫 Eksik | 07 |
| String return type | 🚫 Bug | 09 |
| Array runtime | 🚫 Bug | 10 |

**TAMAMLANDI — YZ_01 (2026-03-03)**

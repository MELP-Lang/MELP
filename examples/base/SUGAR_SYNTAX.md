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

## Özet (Görev 01 sonu)

| Sugar | Durum | Not |
|-------|-------|-----|
| `main()` kısaltması | ❌ | Normalizer eklenene kadar `function main()` zorunlu |
| String interpolation | ⚠️ | Test bekliyor |
| `println()` | ⚠️ | Test bekliyor |

**Sonraki görevlerde** yeni sugar önerileri eklenecektir.

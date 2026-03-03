# DEBUG_BLOCK - Özellik Kartı

**Özellik:** `debug` / `end_debug` — Zero-overhead debug bloğu  
**Durum:** 🔴 TODO (Phase 2 — L-03)  
**Hafta:** Week 5.3 (spec tamamlandı, implementasyon eksik)  
**Sözdizimi Sürümü:** pmlp1  
**Düzeltme:** 25 Şubat 2026 — ÜA tespiti: lexer'da token yok, COMPLETE yanlıştı

> ⚠️ **ÖNEMLİ:** Spec ve sözdizimi tanımı doğrudur. Ancak `debug`, `end_debug`,
> `pause`, `goto`, `label` token'ları `lexer.cpp` ve `token.hpp`'e **henüz eklenmemiş**.
> Bu kart Phase 2 L-03 görevi tamamlanana kadar 🔴 TODO olarak kalır.
> Referans: `TODO_COMPLETE_FINAL/TODO_COMPLETE_FINAL.md` → Phase 2 L-03

---

## 🎯 ÖZELLİK

`debug...end_debug` bloğu içindeki her şey release build'de sıfır maliyetle silinir. `goto label:` ile debug kontrolü yapılır; `pause` çalışmayı durdurur. Tek satır `debug:` anotasyonu da mevcuttur.

---

## 📖 SÖZDİZİMİ

```pmlp
-- Blok form (çok satırlı)
debug
    print("x değeri: " + x)
    print("y değeri: " + y)
end_debug

-- Etiket tanımlı atlama
debug
    label: checkpoint_1
    print("Kontrol noktası 1 geçildi")
end_debug

goto checkpoint_1    -- debug label'a atla

-- pause — tek adım duraklatma
debug
    pause
    print("Adım adım çalışma")
end_debug

-- Tek satır debug anotasyonu
debug: print("Hızlı log: " + value)
```

**Token Listesi:**
| Token | Açıklama |
|-------|----------|
| `debug` | Debug bloğu başlar |
| `end_debug` | Debug bloğu biter |
| `goto` | Label'a atla (debug context dışında da çalışır) |
| `label:` | Hedef etiket tanımla |
| `pause` | Çalışmayı bir adım duraklatır |

**Release Build Davranışı:**
```pmlp
-- Kaynak kod:
x = 42
debug
    print("x = " + x)    -- release'de SİLİNİR
end_debug
y = x + 1

-- Release derleme çıktısı (sözde LIR):
x = 42
y = x + 1    -- debug bloğu yok — zero overhead
```

---

## 🧪 GOLDEN TEST

```pmlp
function compute(numeric n) as numeric
    numeric result = n * n
    debug
        print("compute: n=" + n + " result=" + result)
    end_debug
    return result
end_function

function main() as numeric
    numeric v = compute(7)
    if v == 49 then return 1 end_if
    return 0
end_function
```

---

**Bağımlılıklar:** Derleyici flag: `--release` / `--debug`  
**Hafta:** Week 5.3  
**Kanonik Kaynak:** `pmlp1_sozdizimi.md` — Debug bölümü; `melp_syntax.md` — Token tablosu

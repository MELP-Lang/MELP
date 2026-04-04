# DEBUG_BLOCK - Özellik Kartı

**Özellik:** `debug` / `end debug` — Zero-overhead debug bloğu
**Durum:** 🔴 TODO (Phase 2 — L-03)  
**Hafta:** Week 5.3 (spec tamamlandı, implementasyon eksik)  
**Sözdizimi Sürümü:** mlp1  
**Düzeltme:** 25 Şubat 2026 — ÜA tespiti: lexer'da token yok, COMPLETE yanlıştı

> ⚠️ **ÖNEMLİ:** Spec ve sözdizimi tanımı doğrudur. Ancak `debug`, `end debug`,
> `pause`, `goto`, `label` token'ları `lexer.cpp` ve `token.hpp`'e **henüz eklenmemiş**.
> Bu kart Phase 2 L-03 görevi tamamlanana kadar 🔴 TODO olarak kalır.
> Referans: `TODO_COMPLETE_FINAL/TODO_COMPLETE_FINAL.md` → Phase 2 L-03

---

## 🎯 ÖZELLİK

`debug...end debug` bloğu içindeki her şey release build'de sıfır maliyetle silinir. `goto label:` ile debug kontrolü yapılır; `pause` çalışmayı durdurur. Tek satır `debug:` anotasyonu da mevcuttur.

---

## 📖 SÖZDİZİMİ

```mlp
-- Blok form (çok satırlı)
debug
    print("x değeri: " + x)
    print("y değeri: " + y)
end debug

-- Etiket tanımlı atlama
debug
    label: checkpoint_1
    print("Kontrol noktası 1 geçildi")
end debug

goto checkpoint_1    -- debug label'a atla

-- pause — tek adım duraklatma
debug
    pause
    print("Adım adım çalışma")
end debug

-- Tek satır debug anotasyonu
debug: print("Hızlı log: " + value)
```

**Token Listesi:**
| Token | Açıklama |
|-------|----------|
| `debug` | Debug bloğu başlar |
| `end debug` | Debug bloğu biter |
| `goto` | Label'a atla (debug context dışında da çalışır) |
| `label:` | Hedef etiket tanımla |
| `pause` | Çalışmayı bir adım duraklatır |

**Release Build Davranışı:**
```mlp
-- Kaynak kod:
x = 42
debug
    print("x = " + x)    -- release'de SİLİNİR
end debug
y = x + 1

-- Release derleme çıktısı (sözde LIR):
x = 42
y = x + 1    -- debug bloğu yok — zero overhead
```

---

## 🧪 GOLDEN TEST

```mlp
numeric function compute(numeric n)
    numeric result = n * n
    debug
        print("compute: n=" + n + " result=" + result)
    end debug
    return result
end function

function main()
    numeric v = compute(7)
    if v == 49 then return 1 end if
end function
```

---

**Bağımlılıklar:** Derleyici flag: `--release` / `--debug`  
**Hafta:** Week 5.3  
**Kanonik Kaynak:** `mlp1_sozdizimi.md` — Debug bölümü; `melp_syntax.md` — Token tablosu
> **Not:** `end debug` → `end debug` (boşluklu iki token — tüm diğer blok kapatıcılar gibi)

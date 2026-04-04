# MELP Scope Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Aktif (Stage 0 ve Stage 1)  
**Uygulama:** Lexer + Parser + Codegen — tam çalışıyor

---

## Scope Nedir?

`scope`, MELP'te **isimli kontrol akışı bloğu** oluşturur.

İki amacı var:
1. **Kodu gruplamak** — okunabilirlik
2. **`exit <isim>` ile erken çıkış** — labeled break mekanizması

> Scope, değişken kapsamı (variable scope) değildir. İç değişkenler dışarıdan erişilemez değildir — scope bir kontrol akışı aracıdır.

---

## Temel Syntax

```mlp
scope <isim>
    -- gövde
end scope
```

veya:

```mlp
scope <isim>
    -- gövde
end <isim>
```

Her iki kapatıcı da geçerli: `end scope` evrensel, `end <isim>` isim spesifik.

---

## Kullanım Örnekleri

### Örnek 1: Basit gruplama

```mlp
scope hazirlik
    numeric x = 10
    numeric y = 20
    print(x + y)
end scope
```

### Örnek 2: Erken çıkış (`exit <isim>`)

```mlp
scope dogrulama
    if x < 0 then
        exit dogrulama    -- bloğu terk et
    end if
    if y < 0 then
        exit dogrulama
    end if
    print("Her şey tamam")
end scope
```

### Örnek 3: İç içe döngüden çıkış (labeled break)

`scope` + `exit <isim>` kombinasyonu, iç içe döngülerden tek hamlede çıkmayı sağlar:

```mlp
scope ara
    loop numeric i = 1; i <= 100; i = i + 1
        loop numeric j = 1; j <= 100; j = j + 1
            if i * j > 500 then
                exit ara    -- her iki döngüden birden çık!
            end if
        end loop
    end loop
end scope
-- exit ara buraya atlar
print("Çıkıldı")
```

Bunu `exit for` ile yapmak mümkün değildir — sadece en içteki döngüden çıkar.

### Örnek 4: Temizlik bloğu

```mlp
scope islem
    if hata_var then
        exit islem
    end if
    numeric sonuc = hesapla()
    kaydet(sonuc)
end scope
```

---

## exit \<isim\> Kuralları

```mlp
scope dis
    scope ic
        exit ic    -- sadece iç scope'tan çıkar
        exit dis   -- her ikisinden çıkar
    end scope
end scope
```

- `exit <isim>` hedef scope'a kadar tüm ara blokları terk eder
- İsim eşleşmezse compile-time hata

---

## Sık Sorulan Sorular

### S1: `scope` parametre alabilir mi?
**HAYIR.** Scope kontrol akışı aracıdır, parametre almaz.

Parametre gerekiyorsa → `function` kullan:
```mlp
function hesapla(numeric x; numeric y)
    print(x + y)
end function
```

### S2: `end scope` ile `end <isim>` farkı nedir?
İkisi de geçerli. `end <isim>` okunabilirliği artırır:
```mlp
scope temizlik
    ...
end temizlik   -- hangi scope kapandığı açık
```

### S3: Scope değişken kapsamı yaratır mı?
**HAYIR.** Scope içinde tanımlanan değişkenler dışarıdan erişilebilir. Scope bir kontrol akışı aracıdır.

### S4: `exit` ile `exit <isim>` farkı nedir?
```mlp
exit           -- en içteki bloğu terk et
exit temizlik  -- "temizlik" isimli scope'u terk et
```

### S5: Struct initialization için `scope` kullanabilir miyim?
Hayır, bunun için `with` keyword'ü tasarlandı — bkz. [WITH.md](WITH.md).

---

## Karşılaştırma: Scope vs While/For

| Özellik | `scope` | `loop` / `for` |
|---------|---------|-----------------|
| Tekrarlama | ❌ | ✅ |
| İsim verilebilir | ✅ | ❌ |
| Labeled break | ✅ `exit <isim>` | ✅ `exit for/exit loop` |
| Gruplama | ✅ | N/A |

---

## Compiler Implementation

**Lexer:** `compiler/stage0/lexer/lexer.cpp` satır 318  
→ `scope` → `SCOPE_BLOCK` token  
→ `end scope` veya `end <isim>` → `END_NAMED_SCOPE` token

**Parser:** `compiler/stage0/parser/parser_control.cpp` satır 585  
→ `parseNamedScopeBlock()`

**Codegen:** `compiler/stage1/modules/control_flow/control_codegen.cpp` satır 662  
→ `handleNamedScope()` — LLVM BasicBlock üretir, `scopeExitStack` kullanır

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_scope` | `end scope` |
| `end_<isim>` (alt çizgi) | `end <isim>` (boşluk) |
| `exit_<isim>` (alt çizgi) | `exit <isim>` (boşluk) |
| `end_function` | `end function` |
| `for i = 1 to N` (Stage0) | `for numeric i = 1; i <= N; i = i + 1` |

---

## İlgili Belgeler

- [FUNCTION.md](FUNCTION.md) — Fonksiyon tanımlama
- [WITH.md](WITH.md) — Struct field erişim kısaltması
- [STRUCT.md](STRUCT.md) — Struct tanımlama

---

**© 2026 MELP Project**

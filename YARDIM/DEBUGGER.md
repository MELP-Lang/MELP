# MELP Hata Ayıklayıcı (Debugger) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 4.3

---

## Genel Bilgi

MELP interaktif debugger, program yürütmesini adım adım izlemenizi sağlar.  
Phase 4.3'te implement edilecektir. `debug`/`pause` dil özellikleriyle birlikte çalışır.

> **⚠️ UYARI:** Interaktif debugger Phase 4.3'e kadar mevcut değildir.  
> Şu an `debug`/`pause` bloklarını gdb/lldb ile kullanabilirsiniz.

---

## Planlanan Özellikler

### Breakpoint
```mlp
function main()
    numeric x = 10    -- [BREAKPOINT]
    numeric y = 20
    print(x + y)
end function
```

### Adım Kontrolleri
| Komut | Açıklama |
|-------|----------|
| Step Over | Geçerli satırı çalıştır |
| Step Into | Fonksiyon çağrısına gir |
| Step Out | Mevcut fonksiyondan çık |
| Continue | Sonraki breakpoint'e devam et |

### Değişken İzleme
```
Watch: x = 10
Watch: y = 20
Watch: x + y = 30
```

### Çağrı Yığını
```
#0 topla() at line 5
#1 main() at line 10
```

---

## Şu Anki Alternatif: gdb/lldb

```mlp
-- MELP kodunda pause kullan:
function main()
    numeric x = hesapla(7)
    debug
        pause    -- gdb/lldb burada durur
        print("x = " + x)
    end debug
end function
```

```bash
# Debug modda derle:
melp_compiler --debug program.mlp -o prog.ll
clang -g -O0 -x ir prog.ll build/mlp_runtime.bc -lm -o prog

# gdb ile çalıştır:
gdb ./prog
(gdb) run
# pause'da SIGTRAP → durur
(gdb) print x
(gdb) next
```

---

## İlgili Özellikler

- **DEBUG_BLOCK** — `debug`/`pause` dil yapısı
- **CODE_INTELLIGENCE** — IDE refactoring özellikleri

**Ayrıca bakınız:**
- [DEBUG_BLOCK.md](DEBUG_BLOCK.md)

---

## Durum Tablosu

| Özellik | Phase 4.3 | Şu An |
|---------|-----------|-------|
| Interaktif debugger | ✅ Planlı | ❌ |
| Breakpoint UI | ✅ | ❌ |
| Değişken izleme | ✅ | ❌ |
| `pause` + gdb | N/A | ✅ |
| `debug` bloğu | N/A | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/DEBUGGER.md](../belgeler/language/feature_cards/DEBUGGER.md)
- **debug bloğu:** [DEBUG_BLOCK.md](DEBUG_BLOCK.md)

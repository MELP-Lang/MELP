# MELP Debug Bloğu Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Stage 0'da tamamlandı — 8 Mart 2026 doğrulandı

---

## İçindekiler

1. [Temel Syntax](#temel-syntax)
2. [Kullanım Örnekleri](#kullanım-örnekleri)
3. [Release vs Debug Modu](#release-vs-debug-modu)
4. [Kurallar ve Kısıtlamalar](#kurallar-ve-kısıtlamalar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Temel Syntax

```mlp
-- Blok form (sıfır maliyetle silinir release modda)
debug
    -- içerideki her şey release'de silinir
    print("debug mesajı")
end debug

-- Tek satır form
debug: print("hızlı log: " + deger)

-- pause — CPU breakpoint (gdb/lldb ile)
debug
    pause
end debug

-- goto ile label atlama
debug
    label: kontrol_noktasi_1
    print("Kontrol noktası 1")
end debug

goto kontrol_noktasi_1
```

**Token Listesi:**

| Token | Açıklama |
|-------|----------|
| `debug` | Debug bloğu başlar |
| `end debug` | Debug bloğu biter |
| `goto` | Label'a atla |
| `label:` | Hedef etiket tanımla |
| `pause` | Yürütmeyi durdurur |

---

## Kullanım Örnekleri

### Örnek 1: Değişken izleme
```mlp
numeric function hesapla(numeric n)
    numeric sonuc = n * n
    debug
        print("hesapla: n=" + n + " sonuc=" + sonuc)
    end debug
    return sonuc
end function
```

### Örnek 2: Tek satır log
```mlp
loop each i in 1..10
    debug: print("iterasyon: " + i)
    process(i)
end loop
```

### Örnek 3: pause ile adım adım
```mlp
function main()
    numeric x = hesapla(7)
    debug
        pause
        print("x hesaplandı: " + x)
    end debug
    print(x)
end function
```

---

## Release vs Debug Modu

### Release Modu (varsayılan)
```mlp
-- Kaynak:
x = 42
debug
    print("x = " + x)    -- RELEASE'DE SİLİNİR
end debug
y = x + 1

-- Üretilen LLVM IR (release):
; sadece x = 42 ve y = x + 1
; debug bloğu tamamen yok — SIFIR MALİYET
```

### Debug Modu (`--debug` flag)
```bash
melp_compiler --debug program.mlp -o program.ll
```
- Debug blokları çalışır
- `pause` → `llvm.debugtrap()` → CPU `int3` (SIGTRAP)
- `gdb`/`lldb` ile çalıştırıldığında `pause` noktasında durur

---

## Kurallar ve Kısıtlamalar

### ✅ İzin Verilenler
- `debug` blokları iç içe yazılabilir
- `goto` label'ı debug bloğu dışında da geçerli
- Release modda sıfır maliyet garantisi var

### ❌ Yasaklar
- Debug dışı kodda `pause` kullanma (sadece debug içinde)
- Debug bloğu içinde değişken tanımı (kapsam sorunu — release'de silenemez)

---

## İlgili Özellikler

- **PRINT** — Çıktı yazdırma
- **CONTROL_FLOW** — Kontrol akışı

**Ayrıca bakınız:**
- [PRINT.md](PRINT.md)

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `debug`/`end debug` blok | ✅ | ✅ | ✅ |
| `debug:` tek satır | ✅ | ✅ | ✅ |
| `pause` → SIGTRAP | ✅ | ✅ | ✅ |
| `goto`/`label:` | ✅ | ✅ | ✅ |
| Release'de sıfır maliyet | ✅ | ✅ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/DEBUG_BLOCK.md](../belgeler/language/feature_cards/DEBUG_BLOCK.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/debug_block/](../GOLDEN_TEST_SUITE/debug_block/)
- **pmlp Syntax:** [belgeler/pmlp1_sozdizimi.md](../belgeler/pmlp1_sozdizimi.md)

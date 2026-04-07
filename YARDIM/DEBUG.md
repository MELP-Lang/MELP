# MELP — debug Bloğu

**Durum:** ✅ Tam Çalışıyor  
**Versiyon:** Stage 0 (pmlp0)  
**Test Tarihi:** 8 Mart 2026

---

## Diğer Dillerde Olmayan

MELP'in `debug` bloğu, diğer dillerden farklı bir yaklaşım sunar:

| Dil | Debug Yaklaşımı |
|-----|----------------|
| C/C++ | `#ifdef DEBUG` ... `#endif` (ön işlemci) |
| Python | `import logging` (runtime kütüphane) |
| Rust | `cfg!(debug_assertions)` (makro) |
| **MELP** | `debug` ... `end debug` (birinci sınıf sözdizimi, zero-overhead) |

Avantajı: debug kodu **derleme zamanında** kaldırılır, manuel bayrak veya kütüphane gerektirmez.

---

## Temel Sözdizimi

```melp
debug
    -- Bu içerik yalnızca --debug modunda çalışır
    -- Release build'de tamamen silinir (sıfır maliyet)
end debug
```

---

## Davranış Tablosu

| Mod | Derleme Komutu | debug bloğu | pause |
|-----|----------------|-------------|-------|
| Release (varsayılan) | `melp_compiler program.mlp -o out.ll` | Tamamen silinir | No-op |
| Debug | `melp_compiler program.mlp --debug -o out.ll` | Çalıştırılır | `int3` (SIGTRAP) |

---

## Örnek 1: Değişken Takibi

```melp
numeric function hesapla(numeric n)
    numeric kare = n * n

    debug
        println("hesapla: n=" + str(n) + " kare=" + str(kare))
    end debug

    return kare
end function

function main()
    numeric sonuc = hesapla(7)
    println("Sonuç: " + str(sonuc))
end function
```

**Release çıktısı:**
```
Sonuç: 49
```

**Debug çıktısı (`--debug`):**
```
hesapla: n=7 kare=49
Sonuç: 49
```

---

## Örnek 2: Birden Fazla debug Bloğu

```melp
function main()
    numeric x = 100
    numeric y = 200

    debug
        println("İlk kontrol: x=" + str(x))
    end debug

    numeric toplam = x + y

    debug
        println("Son kontrol: toplam=" + str(toplam))
    end debug

    println("Sonuç: " + str(toplam))
end function
```

---

## Örnek 3: pause — Debugger Kesim Noktası

`pause` keyword'ü, gdb/lldb içinde programı duraksatır:

```melp
function main()
    numeric x = 42

    debug
        println("Duraklamadan önce: x=" + str(x))
        pause                -- gdb/lldb burada durur (int3 / SIGTRAP)
        println("Devam etti")
    end debug
end function
```

**gdb ile kullanım:**
```bash
# Derle (debug modunda)
melp_compiler program.mlp --debug -o program.ll
clang -O0 program.ll -lm -o program

# gdb ile çalıştır
gdb ./program
(gdb) run
# → pause noktasında otomatik durur
# (gdb) continue  → devam
# (gdb) print x   → değişkeni incele
```

> ⚠️ Debugger olmadan `pause` içeren program çalıştırılırsa SIGTRAP sinyali alır (exit 133). Her zaman gdb/lldb içinde kullanın.

---

## Derleme Komutları

```bash
# Release (varsayılan — debug bloğu yok)
melp_compiler program.mlp -o program.ll
clang -O0 program.ll -lm -o program

# Debug (--debug bayrağı)
melp_compiler program.mlp --debug -o program.ll
clang -O0 program.ll -lm -o program
```

> **Not:** `-x ir` yerine doğrudan `clang program.ll -lm -o program` kullanın.  
> Derleyici çıktısı bitcode formatındadır.

---

## Kritik Kurallar

| Kural | Doğru | Yanlış |
|-------|-------|--------|
| Kapatıcı | `end debug` | `end_debug` |
| Bayrak | `--debug` | `--DEBUG` / `-debug` |
| `pause` yeri | Yalnızca `debug` bloğu içinde | Blok dışında (parse hatası) |

---

## Zero-Overhead Prensibi

Release build'de `debug` bloğu LLVM IR'a **hiç yansımaz**:

```melp
-- Kaynak:
x = 42
debug
    println("log: " + str(x))   -- Release'de SİLİNİR
end debug
y = x + 1
```

```llvm
; Release LLVM IR (debug bloğu yok):
store i64 42, ptr %x
%x_val = load i64, ptr %x
%y_val = add i64 %x_val, 1
store i64 %y_val, ptr %y
```

---

## SSS

**S: Her zaman `--debug` modunda derlersem olmaz mı?**  
C: Teknik olarak çalışır ama debug logları kullanıcıya gösterilir, performans düşer. Release build için `--debug` kullanmayın.

**S: `debug` bloğu içinde her şeyi kullanabilir miyim?**  
C: Evet. Normal kodun tamamı — `println`, `if`, fonksiyon çağrısı — debug bloğu içinde geçerlidir.

**S: `pause` debugger olmadan çalıştırılırsa ne olur?**  
C: Program SIGTRAP sinyaliyle sonlanır (exit code 133). Bu bir hata değil; `pause` zaten debugger için tasarlanmıştır.

**S: Birden fazla `debug` bloğu yazabilir miyim?**  
C: Evet. Her blok bağımsız olarak --debug/release moduna göre davranır.

**S: `goto` keyword'ü ne işe yarıyor?**  
C: Parser'da implement edilmiştir ancak label atlama özelliği gelecek sürümlerde tamamlanacak.

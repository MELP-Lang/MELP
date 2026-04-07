# MELP REPL (Read-Eval-Print Loop) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** 🔴 TODO — Phase 4.3

---

## Genel Bilgi

MELP REPL, interaktif ortamda MELP kodu yazıp anında çalıştırmanızı sağlar.  
Phase 4.3'te implement edilecektir (`mlp repl` komutu).

> **⚠️ UYARI:** REPL henüz mevcut değildir.

---

## Planlanan Kullanım

```bash
$ mlp repl
MELP v1.0 REPL
>>> numeric x = 10
>>> numeric y = 20
>>> x + y
30
>>> numeric function topla(numeric a; numeric b)
...     return a + b
... end function
>>> topla(5; 10)
15
```

---

## Planlanan Özel Komutlar

```
:help     → Yardım göster
:clear    → Ekranı temizle
:vars     → Tanımlı değişkenleri göster
:exit     → REPL'den çık
```

---

## Çoklu Satır Giriş

```
>>> numeric function faktoriyel(numeric n)
...     if n <= 1 then
...         return 1
...     end if
...     return n * faktoriyel(n - 1)
... end function
>>> faktoriyel(5)
120
```

---

## Şu Anki Alternatif

Tek seferlik dosya çalıştır:

```bash
# Geçici dosyaya yaz:
cat > /tmp/test.mlp << 'EOF'
function main()
    numeric x = 42
    print(x)
end function
EOF

# Derle ve çalıştır:
MELP=build/compiler/stage1/modules/orchestrator/melp_compiler
$MELP /tmp/test.mlp -o /tmp/test.ll
clang -O0 -x ir /tmp/test.ll build/mlp_runtime.bc -lm -o /tmp/test
/tmp/test
```

---

## İlgili Özellikler

- **BUILD_PROCESS** — Derleme pipeline
- **DEBUGGER** — Interaktif debug

**Ayrıca bakınız:**
- [BUILD_PROCESS.md](BUILD_PROCESS.md)

---

## Durum Tablosu

| Özellik | Phase 4.3 | Şu An |
|---------|-----------|-------|
| `mlp repl` komutu | ✅ Planlı | ❌ |
| Değişken kalıcılığı | ✅ | ❌ |
| Çoklu satır giriş | ✅ | ❌ |
| Özel `:komut`lar | ✅ | ❌ |
| Dosya çalıştırma | N/A | ✅ |

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `function f() as numeric` | `numeric function f()` |
| `end_function` | `end function` |
| `end_if` | `end if` |
| `return 0` (main içinde) | kaldır |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/REPL.md](../belgeler/language/feature_cards/REPL.md)

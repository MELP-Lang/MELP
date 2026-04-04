# MELP Native Binary Üretimi Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ COMPLETE — Phase 1 (P-01), 25 Şubat 2026 doğrulandı

---

## İçindekiler

1. [Genel Bilgi](#genel-bilgi)
2. [CLI Kullanım](#cli-kullanım)
3. [Pipeline](#pipeline)
4. [Önemli Notlar](#önemli-notlar)
5. [İlgili Özellikler](#ilgili-özellikler)
6. [Durum Tablosu](#durum-tablosu)

---

## Genel Bilgi

`melp_compiler` `-o dosya` veya `--emit-binary` flag'i ile doğrudan ELF binary üretir.  
`lli` veya `llvm-link` olmadan çalıştırılabilen native executable oluşturur.

---

## CLI Kullanım

```bash
# Eski davranış — değişmedi
melp_compiler dosya.mlp                  # → output.ll (her zaman üretilir)

# Yeni — native binary
melp_compiler dosya.mlp -o program       # → program (ELF binary)
melp_compiler dosya.mlp --emit-binary    # → a.out (varsayılan isim)
melp_compiler dosya.mlp -o /tmp/prog     # → /tmp/prog

# Çalıştır
./program
```

---

## Pipeline

```
dosya.mlp
    ↓   melp_compiler (her zaman)
output.ll (LLVM IR)
    ↓   llc-14 (yeni)
dosya.o (object file)
    ↓   cc (yeni)
a.out (ELF 64-bit binary)
```

---

## Önemli Notlar

### `-o` ve `output.ll` Ayrımı
```bash
# -o → binary çıktı adını belirler
melp_compiler dosya.mlp -o myprogram

# output.ll her zaman üretilir — bunu değiştirme
# Golden test'ler output.ll → lli zincirini kullanır
```

### Gerekli Sistem Araçları
```bash
which llc-14    # LLVM compiler
which cc        # C compiler (native linkage için)
```

---

## İlgili Özellikler

- **BUILD_PROCESS** — Tam derleme pipeline'ı
- **MODULES** — Modüler derleme

**Ayrıca bakınız:**
- [BUILD_PROCESS.md](BUILD_PROCESS.md)

---

## Hata Tablosu

| ❌ Yanlış | ✅ Doğru |
|-----------|----------|
| `end_function` | `end function` |
| `return 0` (main içinde) | kaldır |

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `.ll` çıktı | ✅ | ✅ | ✅ |
| `-o a.out` native binary | ✅ | ✅ | ✅ |
| `--emit-binary` flag | ✅ | ✅ | ✅ |
| `build/melp` wrapper script | ❌ P-01e | ⏳ | ✅ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/NATIVE_BINARY.md](../belgeler/language/feature_cards/NATIVE_BINARY.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/native_binary/](../GOLDEN_TEST_SUITE/native_binary/)
- **İlgili Görev:** `TODO_PRODUCT_READY/TODO_PRODUCT_READY.md → P-01`

# MELP Derleme Süreci (Build Process) Referansı

**Son Güncelleme:** 8 Mart 2026  
**Durum:** ✅ Çalışıyor — belgelendi

---

## İçindekiler

1. [Pipeline](#pipeline)
2. [Tam Derleme Komutu](#tam-derleme-komutu)
3. [Bağımlılıklar](#bağımlılıklar)
4. [Derleme Bayrakları](#derleme-bayrakları)
5. [Build Sistemi](#build-sistemi)
6. [Sorun Giderme](#sorun-giderme)
7. [Durum Tablosu](#durum-tablosu)

---

## Pipeline

```
.mlp dosyası
    ↓   melp_compiler
.ll (LLVM IR)
    ↓   clang + mlp_runtime.bc + -lm
native binary (ELF)
    ↓   çalıştır
stdout çıktısı
```

---

## Tam Derleme Komutu

### Adım 1: MELP → LLVM IR
```bash
melp_compiler program.mlp -o program.ll
```

### Adım 2: LLVM IR → Çalıştırılabilir
```bash
clang -O0 \
  -x ir program.ll \
  /path/to/build/mlp_runtime.bc \
  -lm \
  -o program
```

### Adım 3: Çalıştır
```bash
./program
```

### Tek Satır (Geliştirme için)
```bash
MELP=/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler
BC=/home/pardus/projeler/MLP/LLVM/stage0/v2/build/mlp_runtime.bc

$MELP program.mlp -o /tmp/prog.ll && \
  clang -O0 -x ir /tmp/prog.ll $BC -lm -o /tmp/prog && \
  /tmp/prog
```

---

## Bağımlılıklar

### `mlp_runtime.bc` — Zorunlu

MELP runtime kütüphanesi LLVM bitcode formatında. İçerdiği fonksiyonlar:

| Fonksiyon | Açıklama |
|-----------|----------|
| `mlp_print_int(i64)` | `print(numeric)` implementasyonu |
| `mlp_print_string(i8*)` | `print(string)` implementasyonu |
| `mlp_str_to_int(i8*)` | `parse_numeric()` implementasyonu |
| `mlp_int_to_str(i64)` | `str(numeric)` implementasyonu |
| `mlp_string_concat(i8*; i8*)` | String birleştirme |
| `mlp_string_eq(i8*; i8*)` | String eşitlik karşılaştırma |

**Yol:** `build/mlp_runtime.bc`

### `-lm` — Zorunlu

C matematik kütüphanesi. `sqrt()`, `pow()`, `abs()` vb. için gereklidir.

---

## Derleme Bayrakları

| Bayrak | Açıklama |
|--------|----------|
| `-O0` | Optimizasyon kapalı (debug için) |
| `-O2` | Üretim optimizasyonları |
| `-x ir` | Girdi LLVM IR formatında (`.ll`) |
| `-lm` | Matematik kütüphanesi link |
| `mlp_runtime.bc` | MELP runtime bitcode |
| `-g` | Debug sembolleri |

---

## Build Sistemi

### Derleyiciyi Build Et
```bash
cd /home/pardus/projeler/MLP/LLVM/stage0/v2
make -C build melp_compiler
```

### Runtime'ı Build Et
```bash
make -C build mlp_runtime
# veya tümünü:
make -C build
```

### Build Çıktısı
```
build/
  compiler/stage1/modules/orchestrator/melp_compiler   ← ana derleyici
  mlp_runtime.bc                                        ← runtime bitcode
```

---

## Sorun Giderme

### `clang: error: no such file or directory: 'mlp_runtime.bc'`
```bash
make -C build mlp_runtime
ls build/mlp_runtime.bc   # varlığını doğrula
```

### `undefined reference to 'mlp_print_int'`
```bash
# mlp_runtime.bc ve -lm'yi kontrol et:
clang -O0 -x ir prog.ll build/mlp_runtime.bc -lm -o prog
#                        ^^^^^^^^^^^^^^^^^^^^  ^^^
```

### `LLVM ERROR: Broken module found, abort.`
```bash
# IR dosyasını doğrula:
opt --verify /tmp/prog.ll -o /dev/null
```

---

## Durum Tablosu

| Özellik | Stage0 | Stage1 | Stage2 |
|---------|--------|--------|--------|
| `.ll` çıktı | ✅ | ✅ | ✅ |
| Native binary (`-o`) | ✅ | ✅ | ✅ |
| mlp_runtime.bc | ✅ | ✅ | ✅ |
| Debug semboller (`-g`) | ✅ | ✅ | ✅ |
| Cross-compile | ❌ | ❌ | ⏳ |

---

## Kaynaklar

- **Feature Card:** [belgeler/language/feature_cards/BUILD_PROCESS.md](../belgeler/language/feature_cards/BUILD_PROCESS.md)
- **Golden Tests:** [GOLDEN_TEST_SUITE/build_process/](../GOLDEN_TEST_SUITE/build_process/)

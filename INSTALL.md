# Kurulum Kılavuzu — MELP Alfa

**Platform desteği:** Linux x86_64 (macOS/Windows: Beta'da)  
**Gereksinim:** LLVM 14 araçları (`clang-14`, `lli-14`)

> 💡 **Kurulum yapmadan denemek için:**  
> [melp-lang.github.io/melp-ide-web/](https://melp-lang.github.io/melp-ide-web/)

---

## 1. Gereksinimler

```bash
# Ubuntu / Debian
sudo apt install clang-14 llvm-14

# Arch Linux
sudo pacman -S llvm14 clang14

# Fedora
sudo dnf install llvm14 clang14
```

Kurulumu doğrula:

```bash
clang-14 --version    # clang version 14.x.x
lli-14 --version      # LLVM version 14.x.x
```

---

## 2. Derleyiciyi Kur

### Seçenek A — Binary Kullan (Önerilen)

`bin/` dizinindeki derlenmiş binary doğrudan kullanılabilir:

```bash
# Bu repoyu klonla
git clone https://github.com/MELP-Lang/MELP.git
cd MELP

# Çalıştırma izni ver (genellikle gerekmez)
chmod +x bin/melp_compiler

# PATH'e ekle (opsiyonel)
export PATH="$PWD/bin:$PATH"
```

### Seçenek B — Kaynak Koddan Derle

Kaynak kodu `stage0/v2` deposundadır. Yeni katkıcı değilseniz Seçenek A yeterlidir.

---

## 3. İlk Program

`merhaba.mlp` dosyası oluştur:

```melp
function main() as void
    print("Merhaba, MELP!")
end_function
```

Derle ve çalıştır:

```bash
# 1. LLVM IR üret
./bin/melp_compiler merhaba.mlp -o merhaba.ll

# 2. Çalıştırılabilir dosya oluştur
clang-14 merhaba.ll ./bin/mlp_runtime.bc -lm -o merhaba

# 3. Çalıştır
./merhaba
```

Beklenen çıktı:
```
Merhaba, MELP!
```

---

## 4. Kısa Yol: `melp` Sarmalayıcı

Tekrarlayan adımları tek komuta indir:

```bash
# Repo kökünde çalıştır
./bin/run_melp merhaba.mlp        # derle + çalıştır
./bin/run_melp merhaba.mlp --ir   # sadece .ll üret
```

> **Not:** `bin/run_melp` betiği `bin/` dizinindedir.

---

## 5. Türkçe Sözdizimi (opsiyonel)

Dosyayı `.tr.mlp` olarak kaydet:

```melp
fonksiyon ana() olarak boşluk
    yaz("Merhaba, MELP!")
son_fonksiyon
```

Derleme aynı komutla yapılır — normalizer otomatik çevirir.

---

## 6. Örnekler

`examples/` dizininde seviyeye göre düzenlenmiş programlar:

```bash
# Tümünü çalıştır
cd examples/base
../../bin/melp_compiler 01_hello_world/hello.mlp -o /tmp/hello.ll
clang-14 /tmp/hello.ll ../../bin/mlp_runtime.bc -lm -o /tmp/hello
/tmp/hello
```

---

## 7. Sorun Giderme

| Hata | Çözüm |
|---|---|
| `clang-14: command not found` | `sudo apt install clang-14` |
| `lli-14: command not found` | `sudo apt install llvm-14` |
| `Segmentation fault` | Runtime'ın doğru sürümünü kullandığınızdan emin olun: `bin/mlp_runtime.bc` |
| `error: undefined reference to 'mlp_print'` | `mlp_runtime.bc` eklemeyi unutmayın: `clang-14 çıktı.ll bin/mlp_runtime.bc -lm -o program` |

Çözülemeyen sorunlar için: [GitHub Issues](../../issues/new?template=bug_report.md)

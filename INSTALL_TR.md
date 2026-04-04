# Kurulum Kilavuzu — MELP Alfa

**Platform destegi:** Linux x86_64 (macOS/Windows: Beta'da)  
**Gereksinim:** LLVM 14 araclari (`clang-14`, `lli-14`)

> 💡 **Kurulum yapmadan denemek icin:**  
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

Kurulumu dogrula:

```bash
clang-14 --version    # clang version 14.x.x
lli-14 --version      # LLVM version 14.x.x
```

---

## 2. Derleyiciyi Kur

### Secenek A — Binary Kullan (Onerilen)

`bin/` dizinindeki derlenimis binary dogrudan kullanilabilir:

```bash
# Bu repoyu klonla
git clone https://github.com/MELP-Lang/MELP.git
cd MELP

# Calistirma izni ver (genellikle gerekmiyor)
chmod +x bin/melp_compiler

# PATH'e ekle (opsiyonel)
export PATH="$PWD/bin:$PATH"
```

### Secenek B — Kaynak Koddan Derle

Kaynak kodu `stage0/v2` deposundadir. Yeni katkilci degilseniz Secenek A yeterlidir.

---

## 3. Ilk Program

`merhaba.mlp` dosyasi olustur:

```mlp
function main()
    print("Merhaba, MELP!")
end function
```

Derle ve calistir:

```bash
# 1. LLVM IR uret
./bin/melp_compiler merhaba.mlp -o merhaba.ll

# 2. Calistirilabilir dosya olustur
clang-14 merhaba.ll ./bin/mlp_runtime.bc -lm -o merhaba

# 3. Calistir
./merhaba
```

Beklenen cikti:
```
Merhaba, MELP!
```

---

## 4. Kisa Yol: `melp` Sarmalayici

Tekrarlayan adimlari tek komuta indir:

```bash
# Repo kokunde calistir
./bin/run_melp merhaba.mlp        # derle + calistir
./bin/run_melp merhaba.mlp --ir   # sadece .ll uret
```

> **Not:** `bin/run_melp` betigi `bin/` dizinindedir.

---

## 5. Turkce Sozdizimi (opsiyonel)

Dosyayi `.tr.mlp` olarak kaydet:

```mlp
fonksiyon giriş()
    yaz("Merhaba, MELP!")
fonksiyon sonu
```

Derleme ayni komutla yapilir — normalizer otomatik cevirir.

---

## 6. Ornekler

`examples/` dizininde seviyeye gore duzenlenimis programlar mevcuttur:

```bash
# Ornek calistir
cd examples/en/base
../../bin/melp_compiler 01_hello_world/hello.mlp -o /tmp/hello.ll
clang-14 /tmp/hello.ll ../../bin/mlp_runtime.bc -lm -o /tmp/hello
/tmp/hello
```

---

## 7. Sorun Giderme

| Hata | Cozum |
|---|---|
| `clang-14: command not found` | `sudo apt install clang-14` |
| `lli-14: command not found` | `sudo apt install llvm-14` |
| `Segmentation fault` | Runtime'in dogru suruumunu kullandiginizdan emin olun: `bin/mlp_runtime.bc` |
| `error: undefined reference to 'mlp_print'` | `mlp_runtime.bc` eklemeyi unutmayin: `clang-14 cikti.ll bin/mlp_runtime.bc -lm -o program` |

Cozulemeyen sorunlar icin: [GitHub Issues](../../issues/new?template=bug_report.md)

---

*Ingilizce surum: [INSTALL.md](INSTALL.md)*

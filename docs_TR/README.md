# MELP Dil Dokümantasyonu (Türkçe)

MELP (Modern Expressive Language Platform) için Türkçe kaynak ve kılavuzlar.

## İçindekiler

- [Dil Referansı](dil_referansi.md) — Sözdizimi, veri tipleri, operatörler
- [Başlarken](baslangic.md) — İlk programı yazmak ve derlemek
- [Standart Kütüphane](stdlib.md) — Yerleşik fonksiyonlar

## Hızlı Başlangıç

```bash
# Derleme
melp_compiler program.mlp -o program.ll

# Bağlama ve çalıştırma
clang -O0 -x ir program.ll mlp_runtime.bc -lm -o program
./program
```

## Katkı

Hata bildirimi ve öneriler için [GitHub Issues](https://github.com/MELP-Lang/MELP/issues) kullanınız.

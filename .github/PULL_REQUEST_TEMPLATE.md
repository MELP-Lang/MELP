## Bu PR Ne Yapıyor?

<!-- Kısaca açıklayın -->

## Tür

- [ ] Hata düzeltmesi
- [ ] Yeni örnek program
- [ ] Belge güncellemesi
- [ ] Diğer

## Test

<!-- Bu değişikliği nasıl test ettiniz? -->

```bash
./bin/melp_compiler examples/... -o /tmp/test.ll
clang-14 /tmp/test.ll ./bin/mlp_runtime.bc -lm -o /tmp/test
/tmp/test
# Çıktı: ...
```

## Kontrol Listesi

- [ ] Kod derlenip çalışıyor
- [ ] Yeni örnek varsa `examples/` altında uygun klasörde
- [ ] Belge değişikliği varsa Türkçe ve İngilizce'de güncellendi

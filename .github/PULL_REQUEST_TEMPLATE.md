<!-- ─── 🇹🇷 TÜRKÇE ──────────────────────────────────────────── -->
## Bu PR Ne Yapıyor? / What Does This PR Do?

**TR:** Kısaca açıklayın.

**EN:** Brief description.

---

## Değişiklik Türü / Change Type

- [ ] Hata düzeltmesi / Bug fix
- [ ] Yeni özellik / New feature
- [ ] Yeni örnek program / New example
- [ ] Belge güncellemesi / Documentation update
- [ ] Refactoring / Yeniden yapılandırma
- [ ] CI / Altyapı / Infrastructure
- [ ] Diğer / Other

---

## Test / Testing

**TR:** Bu değişikliği nasıl test ettiniz?

**EN:** How did you test this change?

```bash
mkdir -p build && cd build && cmake .. && make -j$(nproc)
ctest --output-on-failure
# veya / or:
./bin/melp_compiler examples/... -o /tmp/test.ll
clang-16 /tmp/test.ll ./bin/mlp_runtime.bc -lm -o /tmp/test && /tmp/test
```

---

## Kontrol Listesi / Checklist

**TR:**
- [ ] Kod derleniyor ve testler geçiyor (`ctest --output-on-failure`)
- [ ] Yeni örnek varsa `examples/` altında uygun klasörde
- [ ] Belge değişikliği varsa Türkçe **ve** İngilizce'de güncellendi
- [ ] Breaking change yoksa veya varsa CHANGELOG'a not düşüldü
- [ ] `CONTRIBUTING.md` okundu

**EN:**
- [ ] Code compiles and tests pass (`ctest --output-on-failure`)
- [ ] New examples placed under `examples/` in the appropriate folder
- [ ] Documentation changes made in both Turkish **and** English
- [ ] Breaking changes noted in CHANGELOG (if any)
- [ ] `CONTRIBUTING.md` has been read

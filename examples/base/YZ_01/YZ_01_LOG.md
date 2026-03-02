# Görevli YZ_01 - Faaliyetler Günlüğü

**Başlama:** 2026-03-03
**Rol:** Görevli YZ (Implementasyon)
**Proje:** MELP Stage 1 — examples/base (ilk 10 örnek)

---

## Faaliyetler

| ID | Tarih | Rol | Tip | Detay | Sonuç |
|----|-------|-----|-----|-------|-------|
| 001 | 03.03 10:00 | YZ_01 | ACTION | hello.mlp canonical syntax test | ✅ Çalışıyor ("Merhaba, MELP!") |
| 002 | 03.03 10:05 | YZ_01 | DECISION | hello_sugar.mlp - sugar syntax önerileri | ℹ️ main() kısaltması derlenemiyor |
| 003 | 03.03 10:10 | YZ_01 | ACTION | hello_sugar.mlp canonical (notu ekli) | ✅ Çalışıyor |
| 004 | 03.03 10:15 | YZ_01 | ACTION | SUGAR_SYNTAX.md oluşturuldu | ✅ İlk 3 önerilemeştir |

---

## Görev 01 Özeti

✅ **GÖREV 01 — hello_world** TAMAMLANDI

**Beklenen çıktı:** `Merhaba, MELP!`

### Yapılıp Tamamlananlar:
- [x] `hello.mlp` — canonical pmlp syntax, derlenmiş, çalıştırılmış
- [x] `hello_sugar.mlp` — canonical (sugar önerileri henüz compiler'da yok)
- [x] İkisi de aynı çıktıyı veriyor
- [x] SUGAR_SYNTAX.md oluşturuldu ve ilk bulgular kaydedildi

### Sugar Syntax Bulguları:
1. **main() kısaltması** — ❌ Normalizer gerekli (Compiler: "Expected expression")
2. **String interpolation** — ⚠️ Test edilmedi (PRINT.md'de bahsediliyor ama denenmedi)
3. **println()** — ⚠️ PRINT.md'de bahsediliyor, test edilmedi

---

## Sonraki Adımlar

Görev 02'ye (variables) geçilecek.

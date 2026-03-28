# VSIX — Yapılacaklar / Açık Maddeler

**Güncelleme:** 8 Mart 2026

Bu dosya, biriktirilen değişiklikleri ve düzeltme bekleyen maddeleri takip eder.
Her oturumun sonunda elle güncellenir.

---

## 🔄 Reload Bekleyen (Derlenmiş, Aktif Değil)

Aşağıdaki değişiklikler `npm run compile` ile uygulandı ve
`~/.vscode/extensions/pardus.mlp-ide-0.4.13/` dizinine kopyalandı.
**Ctrl+Shift+P → Developer: Reload Window** ile devreye girer.

| # | Değişiklik | Dosya |
|---|-----------|-------|
| 1 | W7 kuralı: `function main() as <tip>` için uyarı | `mlpDiagnosticProvider.ts` |
| 2 | E8 kuralı: isimsiz `scope` / `spawn` için hata | `mlpDiagnosticProvider.ts` |
| 3 | `scope`, `test`, `spawn` keyword completion + snippet | `mlpCompletionProvider.ts` |
| 4 | `scope`/`spawn`/`test`/`method`/`external`/`async`/`await` syntax renklendirme | `syntaxes/mlp.tmLanguage.json` |
| 5 | E9 kuralı: `struct` içinde `function` tanımı için hata | `mlpDiagnosticProvider.ts` |
| 6 | Completion: struct içindeki fonksiyonlar artık önerilmiyor | `mlpCompletionProvider.ts` |

---

## 📋 Yapılacak / İncelenecek

| # | Madde | Öncelik |
|---|-------|---------|
| 1 | `function main()` dönüş tipi yok iken `return` ifadesi kullanılırsa uyarı ver | Orta |
| 2 | Smoke test: W7 ve E8 kurallarını otomatik test kapsamına ekle | Yüksek |
| 3 | `scope` bloğu fonksiyon dışında kullanılırsa uyarı (derleyici hata veriyor) | Orta |
| 4 | PR-2: Paketleme, `.vsix` kurulum testi, README/CHANGELOG | Yüksek |
| 5 | Bundled compiler güncelleme: `bin/melp_compiler_linux` → `v2/build/melp` kopyası | VSIX tamamlanınca |
| 6 | **[web-editor]** Railway'deki `melp_compiler` binary'sini güncelle — mevcut binary `i = 0` tip çıkarımını desteklemiyor, `Unknown variable: i` veriyor. Yeni build hazır olunca `editors/backend/bin/melp_compiler` değiştirilip push edilecek. | Orta |
| 7 | **[web-editor]** Döngüler örneği şu an `numeric i = 0` ile yazılı (çalışıyor ama dil spec'i ile uyumsuz). Compiler güncellendikten sonra `i = 0` (tip çıkarım) ile düzeltilecek. | Düşük |
| 8 | **[web-editor]** Diğer örnekleri (Struct, Enum+Match, Try/Hata) Railway'de çalıştırarak doğrula. | Orta |

---

## ✅ Tamamlananlar (Bu Oturum)

- [x] Compiler path → `v2/build/melp` olarak güncellendi (`.vscode/settings.json`)
- [x] `npm run compile` artık kurulu extension'ı otomatik eşitliyor (`sync-installed`)
- [x] YARDIM dizinindeki 16 yeni feature card → completion/hover/diagnostic ile örtüştürüldü
- [x] PR-1 smoke testleri geçiyor

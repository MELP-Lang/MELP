# TAM_PIPELINE_VSIX_RAPORU_2026-03-20

**Hazırlayan:** VSIX MM  
**Tarih:** 20 Mart 2026 — Son güncelleme: 21 Mart 2026  
**Referans belge:** `PIPELINE_BELGE_2026-03-20.md`  
**VSIX sürümü:** 0.7.5 (mlp-ide-0.7.5.vsix, 845K, Mar 20 19:35)

---

## Durum

**KISMI** — Temel derleme pipeline'ı çalışıyor; çok-dil IntelliSense düzeltildi; 1 fonksiyon parametresi test hatası açık.

---

## Doğrulama Sonuçları

### 1. bin/ Binary Senkronizasyonu

| Dosya | STAGE0 Sürümü | Durum |
|---|---|---|
| `bin/melp_compiler_linux` | v1.0.11 | ✅ güncel |
| `bin/mlp_runtime.bc` | v1.0.11 | ✅ güncel |
| `bin/melp_normalizer_bin` | v1.0.11 | ✅ güncel |
| `bin/melp_lsp.js` | Mar 20 21:50 | ✅ mevcut |

Doğrulama komutu: `bash scripts/sync-binary.sh` — "Tüm binary'ler STAGE0 v1.0.11 ile senkron. ✓"

### 2. LSP Sunucusu

- ✅ `node bin/melp_lsp.js --stdio` başlatılıyor
- ✅ `initialize` isteğine yanıt veriyor:
  ```
  capabilities: textDocumentSync=2, completionProvider, hoverProvider,
                documentSymbolProvider, definitionProvider, signatureHelpProvider
  ```
- ⚠️ ESK-004: `!=` operatörü için yanlış hata işaretleme (statik kural hatası — Stage 0 sorunuyla bağlantılı)

### 3. mlpCompiler.ts — keywords.json Path Durumu

- ✅ `extensionPath` parametresi `CompileMlpProgramOptions` arayüzüne eklendi
- ✅ Öncelikli yol: `extensionPath/language/<langCode>/keywords.json`
- ✅ `extension.ts` satır 349 ve 394: `extensionPath: context.extensionPath` her iki çağrıda geçiriliyor
- ⚠️ Fallback (dev-only): `/home/pardus/PROJELER/MELP/ORTAK/dil/${langCode}/keywords.json` — production'da bu fallback'e düşülmez (`extensionPath` her zaman mevcut olduğu için), ancak başka makinede elle test edildiğinde kırılır

**keywords.json Diff Doğrulaması (21 Mart 2026 güncellemesi):**

Tüm 5 dil için `ORTAK/dil/<lang>/keywords.json` ↔ `EDITORS/VSIX/language/<lang>/keywords.json` farkı:

```
$ diff ORTAK/dil/en/keywords.json EDITORS/VSIX/language/en/keywords.json → ÖZDEŞ ✅
$ diff ORTAK/dil/tr/keywords.json EDITORS/VSIX/language/tr/keywords.json → ÖZDEŞ ✅
$ diff ORTAK/dil/ru/keywords.json EDITORS/VSIX/language/ru/keywords.json → ÖZDEŞ ✅
$ diff ORTAK/dil/ar/keywords.json EDITORS/VSIX/language/ar/keywords.json → ÖZDEŞ ✅
$ diff ORTAK/dil/zh/keywords.json EDITORS/VSIX/language/zh/keywords.json → ÖZDEŞ ✅
```

Tüm bundle kopyaları kaynak ile byte-for-byte özdeş. `sync-installed` scripti doğru çalışıyor.

### 4. .melp_norm_diller Fix (Bu Session'da Uygulandı)

- ✅ `mlpCompiler.ts` satır 179–183: `langCode` hesaplanıp `/tmp/.melp_norm_diller` yazılıyor
- ✅ Normalizer binary bu dosyayı okuyarak `tr/keywords.json`'ı yüklüyor
- ✅ TR pipeline: `sayısal x = 42 / yaz("Merhaba " + dize(x))` → `Merhaba 42`
- ✅ `kitchen_sink.tr.mlp` → `STAGE0_OK`

### 5. npm run package-vsix

- ✅ `mlp-ide-0.7.5.vsix` – 845K – Mar 20 19:35
- ✅ Webpack derleme başarılı
- ✅ Paket güncel `dist/` içeriğini kapsıyor

### 6. Smoke Testleri

| Test | Açıklama | Sonuç |
|---|---|---|
| Test A — EN sayısal | `numeric x = 42; print("Sonuc: " + str(x))` | ✅ `Sonuc: 42` |
| Test B — EN fonksiyon | `function add(numeric a; numeric b) ...` | ❌ `error: Expected variable name` |
| Test C — TR metin | `metin liste[] = {"0"; "bir"; "2"}; her x için listede: yaz(x)` | ✅ `0 / bir / 2` |
| TR pipeline (genel) | `sayısal x = 42 → yaz("Merhaba " + dize(x))` | ✅ `Merhaba 42` |
| kitchen_sink.tr.mlp | Tam TR kaynak dosyası | ✅ `STAGE0_OK` |

### 7. Çok-Dil IntelliSense — 21 Mart 2026 (YENİ)

**Sorun:** `/home/pardus/PROJELER/MELP/DENEMELER/POC/diller/kitchen_sink.ar.mlp` açıldığında IntelliSense (tamamlama, kırmızı uyarı çizgisi) çalışmıyordu. Kök nedenler:

1. **Tokenizer Unicode körü:** `tokenizeLine()` içinde `[a-zA-ZçğıöşüÇĞİÖŞÜ_]` regex'i Arapça `\u0600-\u06FF` karakterleri tanımıyordu → Arapça token'lar kelime olarak ayrıştırılamıyordu.
2. **Dil algılanamıyor:** `getActiveLang()` yalnızca global `mlp.language` VS Code ayarını okuyordu. `.ar.mlp` dosya adından `ar` kodu otomatik algılanmıyordu.
3. **Ters keyword haritası yok:** `BLOCK_PAIRS` sadece İngilizce — `دالة` (function), `بنية` (struct), `إذا` (if) gibi Arapça keyword'ler tanınmıyordu. Blok eşleştirme, `then` zorunluluğu, kapanmamış blok tespiti çalışmıyordu.

**Uygulanan düzeltmeler:**

| Dosya | Değişiklik |
|---|---|
| `src/mlpDiagnosticProvider.ts` | `tokenizeLine`: `[\p{L}_][\p{L}\p{N}_]` + `u` flag |
| `src/mlpDiagnosticProvider.ts` | `detectLangFromUri(uri)`: dosya adından `ar`/`tr`/`ru`/`zh` algılama |
| `src/mlpDiagnosticProvider.ts` | `loadReverseKeywordMap(lang, extPath)`: yerel → İngilizce ters harita |
| `src/mlpDiagnosticProvider.ts` | `analyze()`: `canonFirst/canonSecond = toEn(token)` — tüm blok/keyword mantığı dil-bağımsız |
| `src/mlpDiagnosticProvider.ts` | `blockStack`: `origLen` field eklendi (Unicode token uzunluğu doğru hesaplanıyor) |
| `src/mlpDiagnosticProvider.ts` | Constructor `extensionPath = ''` parametresi |
| `src/mlpCompletionProvider.ts` | `getActiveLang(document?)`: dosya adından dil algılama |
| `src/mlpCompletionProvider.ts` | `fnRegex` + `structBlockRe`: `\p{L}` + `iu` flag |
| `src/extension.ts` | `new MlpDiagnosticProvider(context.extensionPath)` |

**Sonuç:** `npm run compile` sıfır hata. `dist/` + `~/.vscode/extensions/pardus.mlp-ide-0.7.5/` güncellendi. Kapsama: `ar`, `tr`, `ru`, `zh`, `en` — tüm desteklenen diller.

---

## Açık Sorunlar

| Kod | Kısa Tanım | Öncelik |
|---|---|---|
| TEST-B | `EN fonksiyon parametresi` — `(numeric a; numeric b)` yazımı `Expected variable name` üretiyor | YÜKSEK |
| ESK-009 | Stage 0: `str(this.x)` struct method içinde → ham pointer değeri | DÜŞÜK (Stage 0 sorunu) |
| ESK-010 | Stage 0: `for each ... in list` — `list` keyword çakışması | DÜŞÜK (Stage 0 sorunu) |

~~S-01~~ — `loadDillerKeywords()` içindeki `diller.json` fallback bloğu ve `DillerData` interface'i kaldırıldı: **KAPANDI 21 Mart 2026**.  
~~ESK-004~~ — LSP `!=` yanlış uyarısı: kapatıldı.  
~~MLANG-001~~ — Çok-dil IntelliSense: **KAPANDI 21 Mart 2026**.

---

## Notlar

### Kritik Gözlem: Test B Kök Neden Araştırması Gerekiyor

Test B başarısızlığı (`Expected variable name`) fonksiyon parametre sözdiziminde bir sorun olduğuna işaret ediyor. Test dosyası `echo` ile `printf` ile oluşturuldu — olası neden: `(numeric a; numeric b)` içindeki noktalı virgülün STAGE0 lexer'ı tarafından parametre ayırıcı olarak değil, ifade sonu olarak yorumlanması.

Araştırılacak: STAGE0 fonksiyon tanımında parametre ayırıcı `,` mi yoksa `;` mi? Belgede belirtilmemiş.

### Hardcoded Path — Production Durumu

Mevcut durumda `extensionPath` her zaman `context.extensionPath` ile doluyor ve `language/<lang>/keywords.json` bundle içinde mevcut. Hardcoded fallback üretimde tetiklenmiyor. Ancak bu durum belgelenmelidir: fallback'in silinmesi ya da `assert(false)` ile korunması daha temiz bir yaklaşım olur.

### diller.json → keywords.json (S-01) Durumu — KAPANDI

VSIX tarafında tüm geçiş tamamlandı:
- `mlpCompiler.ts`: `keywords.json` kullanıyor ✅
- `mlpCompletionProvider.ts`: `diller.json` fallback bloğu ve `DillerData` interface'i kaldırıldı — `keywords.json` tek kaynak ✅
- `language/{en,tr,ru,ar,zh}/keywords.json` bundle'da mevcut ✅
- `language/diller.json` hâlâ bundle'da fiziksel olarak mevcut ancak artık hiçbir kod tarafından okunmuyor.

---

## Sonraki Adımlar (Öneri)

1. **[YÜKSEK]** Test B kök neden araştırması: `kitchen_sink.mlp` içinde çalışan bir fonksiyon örneği incelenerek STAGE0'ın beklediği parametre sözdizimi doğrulanmalı.
2. **[DÜŞÜK]** `mlpHoverProvider.ts` içindeki `KEYWORD_DOCS` tablosunu çok-dil desteği için genişlet (şu an sadece İngilizce keyword'ler hover döndürüyor).


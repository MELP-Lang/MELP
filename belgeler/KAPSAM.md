# KAPSAM — Eski belgeler/ → belgeler_v2/ Eşleme

Amaç: Hangi eski dosyanın nereye gittiğini ve neden gittiğini belgelemek.

---

## Kural

| Durum | Anlamı |
|---|---|
| **Konsolide** | Birden fazla eski dosya tek yeni dosyada birleşti |
| **Taşındı** | İçerik değişmeden yeni konumuna taşındı |
| **Referans** | Orijinal dosya yeterince temiz; burada sadece linklenir |
| **Arşivlendi** | İçerik güncel değil veya artık gereksiz; `belgeler/archive/` ye ait |
| **Silindi** | İçerik başka belgede tamamen kapsanıyor |

---

## Sözdizim ve Dil Referansı → `dil/`

| Eski dosya(lar) | Yeni konum | Durum |
|---|---|---|
| `belgeler/mlp_sozdizimi.md` (1994 satır) | `dil/referans.md` | Birincil kaynak (taşındı/referans) |
| `belgeler/mlp1_sozdizimi.md` | `dil/referans.md` (Stage 1 bölümü) | Konsolide |
| `belgeler/mlp0_sozdizimi.md` | `dil/referans.md` (Stage 0 bölümü) | Konsolide |
| `belgeler/language/mlp0_syntax.md` | `dil/referans.md` (Stage 0 hızlı ref) | Konsolide |
| `belgeler/language/mlp1_syntax.md` | `dil/referans.md` (Stage 1 hızlı ref) | Konsolide |
| `belgeler/language/mlp2_syntax.md` | `dil/referans.md` (Stage 2, stub) | Konsolide |
| `belgeler/MELP_REFERENCE.md` | `dil/referans.md` (ek context) | Referans |
| `belgeler/MLP_LANGUAGE_REFERENCE.md` | `dil/referans.md` (Stage 0 sub) | Referans |
| `belgeler/kurallar_kitabı_v1.md` | `dil/referans.md` (kurallar bölümü) | Konsolide |
| `belgeler/language/mlp_keywords_turkish.md` | **`dil/anahtar_kelimeler.md`** | Birincil kaynak |
| `belgeler/language/mlp_keywords_examples.md` | `dil/anahtar_kelimeler.md` (örnekler) | Konsolide |
| `belgeler/language/mlp_keywords_examples_TR.md` | `dil/anahtar_kelimeler.md` (TR örnekler) | Konsolide |
| `belgeler/language/data_types.md` | `dil/anahtar_kelimeler.md` (tip bölümü) | Konsolide |
| `belgeler/language/STO.md` | `dil/anahtar_kelimeler.md` (tip bölümü) | Konsolide |
| `belgeler/language/feature_cards/` (70+ dosya) | `dil/ozellik_kartlari/` (stage özeti) | Orijinallar korunur |

---

## Mimari → `mimari/`

| Eski dosya(lar) | Yeni konum | Durum |
|---|---|---|
| `belgeler/ARCHITECTURE.md` | `mimari/genel.md` | Birincil kaynak |
| `belgeler/FRONTEND_BACKEND_SEPARATION.md` | `mimari/genel.md` | Konsolide |
| `belgeler/100_PERCENT_MODULAR_ARCHITECTURE.md` | `mimari/genel.md` | Konsolide |
| `belgeler/LAYER_ARCHITECTURE_BRIEF.md` | `mimari/genel.md` | Konsolide |
| `belgeler/modüler_mimari.md` | `mimari/genel.md` | Konsolide |
| `belgeler/PARADIGM2_MODULAR_VISION.md` | `mimari/genel.md` | Konsolide |
| `belgeler/IDEAL_STANDALONE_PLAN.md` | `mimari/genel.md` | Konsolide |
| `belgeler/MLP MODÜL FELSEFESİ.md` | `felsefe/temel_esaslar.md` | Konsolide |
| `belgeler/MODULAR_GC_NOTES.md` | Arşivlendi (tarihsel not, referans değeri düşük) | Arşivlendi |
| `belgeler/OK_LAYER_ARCHITECTURE.md` | **`mimari/ok_katmani.md`** | Birincil kaynak |
| `belgeler/STO_PRINCIPLES.md` | `mimari/ok_katmani.md` | Konsolide |
| `belgeler/SAO_PRINCIPLES.md` | `mimari/ok_katmani.md` | Konsolide |
| `belgeler/STO_PERFORMANCE.md` | `mimari/ok_katmani.md` | Konsolide |
| `belgeler/FBO_COMPACT_ENCODING.md` | `mimari/ok_katmani.md` | Konsolide |
| `belgeler/STO_SAO_IMPLEMENTATION_ROADMAP.md` | `mimari/ok_katmani.md` | Konsolide |
| `belgeler/melp_felsefesi/FBO_FAST_BINARY_OBJECTS.md` | `mimari/ok_katmani.md` | Konsolide |
| `belgeler/melp_felsefesi/FBO_FRONTEND_BACKEND_OPTIMIZATION.md` | `mimari/ok_katmani.md` | Konsolide |
| `belgeler/melp_felsefesi/OK_AILESI_COK_POK_ROK.md` | `mimari/ok_katmani.md` | Konsolide |
| `belgeler/melp_felsefesi/OK_MIDDLEWARE_PATTERN.md` | `mimari/ok_katmani.md` | Konsolide |

---

## Felsefe → `felsefe/`

| Eski dosya(lar) | Yeni konum | Durum |
|---|---|---|
| `belgeler/MELP_PHILOSOPHY.md` | `felsefe/temel_esaslar.md` | Birincil kaynak (referans) |
| `belgeler/melp_felsefesi/MELP_OWNERSHIP_MODEL.md` | `felsefe/temel_esaslar.md` | Konsolide |
| `belgeler/melp_felsefesi/README.md` | `felsefe/README.md` | Referans |

---

## Geliştirici Rehberleri → `rehberler/`

| Eski dosya(lar) | Yeni konum | Durum |
|---|---|---|
| `belgeler/FFI_USER_GUIDE.md` | `rehberler/ffi.md` | Referans (yeterince temiz) |
| `belgeler/HEAP_MANUAL_MEMORY_GUIDE.md` | `rehberler/bellek.md` | Referans |
| `belgeler/RESULT_TYPE_USAGE_GUIDE.md` | `rehberler/hata_yonetimi.md` | Birincil |
| `belgeler/DEBUG_MODULE_BRIEFING.md` | `rehberler/debug.md` | Referans |
| `belgeler/EDITOR_INTEGRATION_GUIDE.md` | `rehberler/editor.md` | Referans |
| `belgeler/BUILD_SYSTEM.md` | `rehberler/proje.md` | Referans |
| `belgeler/TEST_COVERAGE_STRATEGY.md` | `rehberler/proje.md` | Konsolide |

---

## Bootstrap → `bootstrap/`

| Eski dosya(lar) | Yeni konum | Durum |
|---|---|---|
| `belgeler/BOOTSTRAP_STRATEGY.md` | `bootstrap/strateji.md` | Birincil |
| `belgeler/STAGE0_LIMITATIONS.md` | `bootstrap/strateji.md` | Konsolide |
| `belgeler/STAGE0_TO_STAGE1_GUIDE.md` | `bootstrap/strateji.md` | Konsolide |
| `belgeler/LESSONS_LEARNED.md` | `bootstrap/strateji.md` | Konsolide |

---

## Arşive Taşınanlar

| Dosya | Neden |
|---|---|
| `belgeler/YZ_03_PHASE3_COMPLETION_REPORT.md` | Tarihsel rapor, teknik referans değil |
| `belgeler/WEEK5_COMPLETION_REPORT.md` | Tarihsel rapor |
| `belgeler/PROJE_SON_DURUM_03_Subat_2026.md` | Tarihsel rapor |
| `belgeler/UA_MODERN_BURADAN_BASLA.md` | AI brifing, yönetim belgesi |
| `belgeler/MM_02_DEVIR_TESLIM.md` | Yönetim devir notu |
| `belgeler/MM_TALIMAT_PARADIGM2_POC.md` | Yönetim talimatı |
| `belgeler/PARALEL_POC_PLAN.md` | POC planı, uygulandı |
| `belgeler/PROJECT_MANAGEMENT_METHODOLOGY.md` | Yönetim dokümane edildi, tekrar gereksiz |
| `belgeler/CONCURRENCY_PREP_READING_LIST.md` | Okuma listesi, değer düştü |
| `belgeler/OWNERSHIP_PREP_READING_LIST.md` | Okuma listesi, değer düştü |
| `belgeler/MLP_IMPROVEMENTS.md` | Karar uygulandı, tarihsel |
| `belgeler/YZ_SYNTAX_TRAINING_STRATEGY.md` | `dil/anahtar_kelimeler.md`'a entegre edilebilir |
| `belgeler/templates/` | Şablonlar, belgeler_v2 arşivinde |

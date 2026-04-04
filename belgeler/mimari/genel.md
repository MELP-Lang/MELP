# MELP Genel Mimari

**Tek kaynak. Şu belgelerden konsolide edildi:**
`ARCHITECTURE.md` · `FRONTEND_BACKEND_SEPARATION.md` ·
`100_PERCENT_MODULAR_ARCHITECTURE.md` · `LAYER_ARCHITECTURE_BRIEF.md` ·
`modüler_mimari.md` · `IDEAL_STANDALONE_PLAN.md` · `PARADIGM2_MODULAR_VISION.md`

**Son güncelleme:** Mart 2026

---

## 1. Bir Felsefe Olarak Mimari

MELP mimarisi dört ilke üzerine inşa edilmiştir:

| İlke | Anlamı |
|---|---|
| **%100 Modüler** | Her şey bağımsız modüldür; monolitik `main.c` yasak |
| **İki Dil Yalıtımı** | MLP (frontend) ≠ PMLP (backend); aralarında Normalizer köprüsü |
| **Katman Tabanlı Optimizasyon** | STO/SAO/FBO tüm modüllerden bağımsız üç ayrı katmandır |
| **Stateless Önce** | Modüller durum tutmaz; fonksiyonlar saf (pure) tasarlanır |

---

## 2. İki Dil Tasarımı (Frontend / Backend Ayrımı)

MELP'te iki ayrı dil katmanı vardır:

```
┌─────────────────────────────────────────────────────────────┐
│  MLP — Kullanıcı Dili (Frontend)                            │
│  • Dosya: .mlp                                              │
│  • Çok dilli: Türkçe / İngilizce / Rusça                   │
│  • Çok stilii: mlp-style, c-style, python-style             │
│  • Değişebilir: geri bildirime açık, sözdizimi evrimi mümkün│
└─────────────────────────────┬───────────────────────────────┘
                              │
                    ┌─────────▼─────────┐
                    │    NORMALIZER      │
                    │  (Çeviri Katmanı)  │
                    │  diller.json       │
                    │  syntax.json       │
                    └─────────┬─────────┘
                              │
┌─────────────────────────────▼───────────────────────────────┐
│  PMLP — Derleyici Dili (Backend)                            │
│  • Normalizer çıktısı — kullanıcı göremez, yazamaz          │
│  • SABİT: Lexer/parser sözleşmesi                           │
│  • Tek dil: İngilizce anahtar kelimeler                     │
│  • Altçizgili tokenlar: `end_function`, `else_if`           │
└─────────────────────────────┬───────────────────────────────┘
                              │
                      Lexer → Parser → OK Katmanı → LLVM IR → Binary
```

**Kritik fark:**

| Özellik | MLP (Frontend) | PMLP (Backend) |
|---|---|---|
| Kimin için | Kullanıcı | Derleyici |
| `end function` | Geçerli ✅ | `end_function` |
| Türkçe | `döngü sonu` ✅ | `end_while` (Normalizer çevirir) |
| Değişebilir mi | Evet | Hayır |

---

## 3. Derleme Pipeline'ı

```
.tr.mlp / .en.mlp / .ru.mlp
          ↓
     [Normalizer]        ← diller.json + syntax.json config-driven
          ↓
     .mlp (canonical)    ← bu noktadan sonra tek dil
          ↓
      [Lexer]            ← PMLP tokenları üretir
          ↓
      [Parser]           ← PMLP'den AST üretir
          ↓
   [OK Katmanı]          ← STO → SAO → FBO (bkz. mimari/ok_katmani.md)
          ↓
    [LLVM IR]            ← llvm::Module, llvm::IRBuilder
          ↓
  [Optimizasyon]         ← LLVM pass'ları (-O2 / -O3)
          ↓
  [Bağlayıcı/lld]        ← statik veya paylaşımlı kütüphanelerle
          ↓
  Çalıştırılabilir binary
```

---

## 4. Katman Mimarisi (STO / SAO / FBO)

Optimizasyon katmanları, tüm modüllerin **dışında** ve **üstünde** durur:

```
┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐
│ Modül A  │ │ Modül B  │ │ Modül C  │ │  ...     │
│          │ │          │ │          │ │          │
│ AST çıktı│ │ AST çıktı│ │ AST çıktı│ │ AST çıktı│
└────┬─────┘ └────┬─────┘ └────┬─────┘ └────┬─────┘
     └────────────┴────────────┴────────────┘
                               │ Standart AST arayüzü
               ┌───────────────▼────────────────────┐
               │  STO Katmanı (compiler/sto/)        │
               │  Tip çıkarımı, aralık analizi       │
               └───────────────┬────────────────────┘
                               │
               ┌───────────────▼────────────────────┐
               │  SAO Katmanı (compiler/sao/)        │
               │  Saflık analizi, SAO meta-bilgisi   │
               └───────────────┬────────────────────┘
                               │
               ┌───────────────▼────────────────────┐
               │  FBO Katmanı (compiler/fbo/)        │
               │  Füzyon, ölü-yol eleme, kodlaştırma │
               └───────────────┬────────────────────┘
                               │
                            LLVM IR
```

**Avantaj:** Modüller sadece standart AST çıktısı verir. Optimizasyon adaptasyonu yoktur.

Detay: [belgeler/mimari/ok_katmani.md](ok_katmani.md)

---

## 5. Modüler Dizin Yapısı

```
LLVM/
├── STAGE0/               ← Bootstrap C++ derleyicisi
│   └── compiler/
│       ├── sto/          ← STO katmanı (~1500 LOC)
│       ├── sao/          ← SAO katmanı (~2000 LOC)
│       ├── fbo/          ← FBO katmanı (~800 LOC)
│       ├── lexer/        ← Tokenizer
│       ├── parser/       ← AST üretici
│       ├── codegen/      ← LLVM IR üretici
│       └── runtime/      ← Çalışma zamanı (print, IO)
├── STAGE1/               ← .mlp → binary köprüsü
│   ├── belgeler/         ← Konsolide belgeler (bu yapı)
│   ├── belgeler_old/     ← Arşiv (eski yapı, gitignore'da gizli)
│   └── MELP_OZELLIKLER.md
└── ORTAK/
    ├── dil/              ← Normalizer (çok-dilli sözdizim çevirisi)
    ├── LSP/              ← Dil sunucusu protokolü
    └── DAP/              ← Debug adaptörü protokolü
```

---

## 6. Paradigm 2 — Modüler Vizyon

Stage 1 ve ötesinde hedeflenen modüler paradigma:

- Her dil yapısı (if, loop, function, struct…) ayrı **modül** olarak paketlenir
- Modüller yüklenip boşaltılabilir (`alive` flag + dead-module garbage collection)
- Compiler bütünüyle plugin tabanlı: yeni bir yapı eklemek sadece bir modül dosyası oluşturmak
- Modül = Şablon ilkesi: her modül hem AST kalıbı hem derleyici davranışı içerir

**Durum:** Tasarım belgesi. Uygulama Stage 2'de.

Kaynak: `belgeler/PARADIGM2_MODULAR_VISION.md`

---

## 7. Çok Dilli Mimari (Normalizer)

```
Giriş dilleri:            Çıkış (canonical MLP):
  Türkçe (.tr.mlp)  ──┐
  İngilizce (.mlp)  ──┼──→  Normalizer  ──→  .mlp  ──→  Derleyici
  Rusça (.ru.mlp)   ──┘

Dönüşüm örnekleri:
  döngü       →  loop
  koşul sonu  →  end if
  seç         →  match
  3,14        →  3.14       ← Ondalık ayırıcı
  x değil     →  not x      ← Postfix not
  döndür x    →  return x   ← Postfix return
```

Normalizer config: `ORTAK/dil/` dizini altındaki `diller.json` ve `syntax.json`

---

## 8. Derleyici Uygulama Durumu

| Bileşen | Durum | Notlar |
|---|---|---|
| Normalizer (TR/EN) | ✅ Çalışıyor | Stage 0 tamamlandı |
| Lexer | ✅ Çalışıyor | PMLP tokenları |
| Parser | ✅ Çalışıyor | AST üretimi |
| STO (Smart Type Optimization) | ✅ Temel | `numeric` → i64/f64/BigDecimal |
| SAO (Semantic-Aware Optimization) | ✅ Temel | Semantic metadata → LLVM hints |
| FBO (Frontend Backend Optimization) | 🔄 Kısmi | 4-byte metadata, Phase 5.1 |
| LLVM IR codegen | ✅ Çalışıyor | İşlevsel binary çıktısı |
| Debug Module | ✅ Çalışıyor | `debug` bloğu, watchpoint set |
| Scope/exit if | ✅ Çalışıyor | Adlandırılmış kapsam çıkışı |
| Enum/Match | 🔄 Stage 1 | Tasarım tamamlandı, uygulama devam |
| Generics | 🔄 Stage 1 | Monomorphization planlandı |
| Ownership/Borrow | 🗓️ Stage 2 | MELP_OWNERSHIP_MODEL.md |

---

## 9. İlgili Belgeler

| Konu | Belge |
|---|---|
| OK katmanı teknik detay | [ok_katmani.md](ok_katmani.md) |
| Sahiplik modeli | `belgeler/melp_felsefesi/MELP_OWNERSHIP_MODEL.md` |
| Bootstrap stratejisi | [belgeler/bootstrap/strateji.md](../bootstrap/strateji.md) |
| Sözdizimi referansı | [belgeler/dil/referans.md](../dil/referans.md) |
| Anahtar kelimeler | [belgeler/dil/anahtar_kelimeler.md](../dil/anahtar_kelimeler.md) |
| Proje felsefesi | [belgeler/felsefe/temel_esaslar.md](../felsefe/temel_esaslar.md) |

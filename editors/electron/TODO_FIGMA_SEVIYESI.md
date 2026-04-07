# 🎯 TODO — Figma Seviyesi GUI Editör
### MELP Electron — WASM Entegrasyonu ile Sonraki Nesil
> Başlangıç tarihi: 10 Mart 2026  
> Ön koşul: `melp_compiler.wasm` WASM katmanı — `assets/melp_compiler.js` (88 KB) + `assets/melp_compiler.wasm` (504 KB) kopyalandı ✅ (10 Mart 2026)  
> Mevcut durum: `TODO_PRODUCT_READY.md` ~%82 tamamlandı, v1.3 yalnızca %6.

---

## 🏛️ MİMARİ HEDEF

```
Şu anki akış (kırılgan, bağımlı):
  Kod değişikliği → Derle butonuna bas → spawn binary → /tmp dosyası → lli → terminal çıktısı

Hedef akış (Figma gibi, anlık):
  Kod/widget değişikliği → WASM in-process → ms'ler içinde hata/sonuç → canlı panelde görünür
```

---

## 🔴 FAZ 1 — WASM Entegrasyonu (Kritik Altyapı)

> Bu faz olmadan diğer tüm Figma özellikleri mümkün değil.

### 1.1 WASM Backend Bağlantısı

- [x] `index.html`'e `<script src="assets/melp_compiler.js">` ekle
- [x] `src/utils/wasm-backend.js` modülü yaz:
  - `compileCode(mlpSource)` → `{ stdout, stderr, wasmBytes, exitCode }`
  - `runWasm(wasmBytes)` → stdout string (Node.js `WebAssembly.instantiate` ile)
  - Hata yakalamak için `try/catch` + anlamlı mesajlar
- [x] `preload.js`'e `run-wasm` IPC handler'ı ekle (renderer → main, WASM bytes çalıştırır)
- [x] `main.js`'e `ipcMain.handle('run-wasm', ...)` yaz

### 1.2 Mevcut binary spawn'ı WASM ile değiştir

- [x] `src/app.js` içindeki `terminalExec(cmd, '/tmp')` bloğu WASM bloğunun altına fallback olarak taşındı
- [x] Yerine `wasmBackend.compile(mlpCode)` çağrısı eklendi
- [x] `/tmp` geçici dosya yazma adımı fallback'e alındı (WASM varsa atlanıyor)
- [x] `settings.json`'dan `compilerPath` ve `lliPath` bağımlılıkları opsiyonel — WASM varsa kullan; yoksa binary fallback
  - WASM varsa kullan; yoksa binary fallback'e geç (geriye dönük uyumluluk)

### 1.3 Diagnostic pipeline

- [x] WASM'dan gelen `stderr`'i satır:kolon regex'i ile parse et
- [x] Parse edilen hataları `src/utils/diagnostics.js` modülüne taşı
  (web editörün `parseCompilerDiagnostics` mantığıyla aynı yapı)
- [ ] Hata listesini alt panelde göster (satır numarasına tıklayınca editörde o satıra git)

---

## 🟠 FAZ 2 — Anlık Geri Bildirim (Figma'nın Özü)

### 2.1 As-you-type derleme

- [x] Kod editöründe her değişiklikten 800ms sonra arka planda WASM derlemesi başlat (debounce)
- [ ] Derleme sırasında editör başlığında dönen bir ikon göster (kullanıcıyı bloklamadan)
- [ ] Hata yoksa → yeşil ✅, hata varsa → kırmızı satır altı çizgi + sol kenar kırmızı nokta

### 2.2 Kod editörüne inline hata gösterimi

- [ ] Hatalı satır numarası için CodeMirror/satır renklendirmesi ekle
- [ ] Satır üzerinde hover → popup'ta hata mesajı göster
- [ ] Hata sayısı/uyarı sayısı durum çubuğunda göster (`2 hata, 1 uyarı`)

### 2.3 Normalizer entegrasyonu

- [ ] `src/utils/normalizer-web.js` → web editördeki `normalizer.js` buraya port et
  (veya ortak `editors/shared/` klasöründen referans al)
- [ ] As-you-type derlemede önce normalizer çalışsın, sonra WASM'a gönder
- [ ] `#lang` / `#syntax` direktiflerini destekle (web editördeki `parseAndStripDirectives` mantığı)

---

## 🟠 FAZ 3 — Canlı Önizleme (Figma'nın "Canvas" Katmanı)

### 3.1 Önizleme paneli

- [ ] GÖRÜNÜM sekmesindeki mevcut statik HTML önizlemesini kaldır
- [ ] Yerine: WASM çalıştırma çıktısını gösteren canlı panel koy
- [ ] Widget değişikliği → kod üretilir → WASM derle → çalıştır → stdout panelde görünür
- [ ] Önizleme panelinde "boyut simülasyonu" seçimi: 800×600, 1024×768, 1920×1080, Mobil

### 3.2 Hot Reload

- [ ] Tasarımcıda widget ekle/taşı/sil → 500ms debounce → otomatik yeniden derleme
- [ ] Kod sekmesinde değişiklik → otomatik yeniden derleme
- [ ] Önizleme panelinde "⟳ Canlı" toggleı — kapatılabilir (büyük projelerde)

### 3.3 Hata durumu önizlemesi

- [ ] Derleme hatası varsa önizleme paneli donmasın, son başarılı çıktıyı koru
- [ ] Panelin köşesinde kırmızı `⚠ Derleme hatası — son başarılı sürüm gösteriliyor` uyarısı

---

## 🟡 FAZ 4 — Designer Güçlendirme (Figma'nın Panel Sistemi)

### 4.1 Katmanlar Paneli (Layers Panel)

- [ ] Sol panele "Katmanlar" sekmesi ekle
- [ ] Tüm widget'lar ağaç görünümünde listele (id + tür + ikon)
- [ ] Katmanlar panelinden widget seç → canvas'ta seçili olur
- [ ] Sürükle-bırak ile z-order (öne/arkaya) değiştirme
- [ ] Göster/gizle toggle (👁) her widget için
- [ ] Kilitle toggle (🔒) — kilitli widget'lar canvas'ta seçilemiyor

### 4.2 Bileşen Kütüphanesi Paneli

- [ ] Sağ panele "Bileşenler" sekmesi ekle
- [ ] `examples/` klasöründeki `.mlpgui` dosyalarını bileşen olarak listele
- [ ] Bir bileşene çift tık → canvas'a ekle
- [ ] Kullanıcının kendi `.mlpgui` bileşenlerini kaydedip listeye eklemesi (özel bileşen)

### 4.3 Custom Widget Oluşturma (mevcut TODO'dan taşındı)

- [ ] Widget türü tanımlama dialogu
- [ ] Mevcut widget'lardan türetme (inheritance)
- [ ] Custom widget'ı toolbox'a ekleme
- [ ] Custom widget'ı `.mlpwidget` olarak dışa/içe aktarma

### 4.4 Özellik Paneli — Figma özellik paneli standardında

- [ ] Konum & boyut: X, Y, W, H — doğrudan sayı girilebilen input'lar
- [ ] Kilitli oran toggle (W:H oranı sabit tut)
- [ ] Renk swatch'larında "son kullanılan renkler" geçmişi (son 8)
- [ ] Özellik değişikliği → anlık önizlemeye yansısın (3.2 hot reload ile)

---

## 🟡 FAZ 5 — Kod Editörü Güçlendirme

### 5.1 CodeMirror 6 geçişi (mevcut editör neyse değerlendirin)

- [ ] Mevcut kod editörünün versiyonunu tespit et (`src/app.js` içindeki editor init)
- [ ] CodeMirror 6 değilse geçiş planı yap; 6 ise mevcut extension sistemi kullan
- [ ] Aynı `editor.bundle.js` paketini web editörden burada da kullan (`editors/shared/`)

### 5.2 MLP dil desteği

- [ ] Sözdizimi vurgulaması — web editördeki TextMate grammar'ı buraya port et
- [ ] Otomatik tamamlama — mevcut `intellisense.js`'i CodeMirror extension'ına bağla
- [ ] Parantez eşleştirme, otomatik kapanma
- [ ] `#lang` / `#syntax` direktifi renklendirmesi

### 5.3 Multi-file desteği (sekme tabanlı)

- [ ] Aynı anda birden fazla `.mlp` dosyası açık tutulabilsin
- [ ] Her dosya kendi sekmesini taşır (kaydedilmemiş değişiklik için `●` ikonu)
- [ ] Sekmeler arası geçişte editör içeriği korunur

---

## 🟡 FAZ 6 — Mimari Refactoring

> `app.js` 3227 satır — bu Figma seviyesi özellikler eklenince sürdürülemez hale gelir.

### 6.1 app.js modülarizasyonu

- [ ] `src/app.js` → şu modüllere böl:
  - `src/core/project-manager.js` — dosya aç/kapat/kaydet/yeni
  - `src/core/compile-runner.js` — WASM derleme + çalıştırma
  - `src/core/tab-manager.js` — sekme yönetimi
  - `src/ui/toolbar.js` — toolbar buton handler'ları
  - `src/ui/status-bar.js` — durum çubuğu
  - `src/ui/panels.js` — panel görünürlük/boyut yönetimi

### 6.2 Paylaşılan kod (`editors/shared/`)

- [ ] `normalizer.js` — web + MELP Electron arasında ortak
- [ ] `diagnostics.js` — hata parse'lama ortak
- [ ] `diller.json` — tek kaynak (`/LLVM/stage0/v2/` authoritative, burası symlink veya kopyası)
- [ ] `editor.bundle.js` — web ve MELP Electron aynı paketi kullansın

---

## 🟢 FAZ 7 — Gelişmiş Özellikler (Uzun Vade)

### 7.1 Breakpoint & Debug (mevcut TODO'dan taşındı, WASM ile artık mümkün)

- [ ] WASM çalıştırmada breakpoint hook'ları
- [ ] Değişken izleme (WASM memory'den okuma)
- [ ] Adım adım yürütme

### 7.2 Animasyonlar (mevcut TODO'dan taşındı)

- [ ] Özellik paneline Animasyon sekmesi
- [ ] Fade/slide/pulse efektleri
- [ ] Önizleme panelinde animasyon oynatma

### 7.3 Responsive Design (mevcut TODO'dan taşındı)

- [ ] Yüzde tabanlı pozisyonlama
- [ ] Breakpoint tanımı (mobil/tablet/masaüstü)
- [ ] Responsive önizleme — panel boyut simülasyonu

### 7.4 Ekip çalışması (Figma'nın uzun vadeli vizyonu)

- [ ] `.mlpgui` formatı için Git-friendly diff (şu an JSON blob)
- [ ] Proje şablonu paylaşım merkezi

---

## 📊 Faz Öncelikleri ve Bağımlılıklar

```
FAZ 1 (WASM)     ←── HER ŞEYİN ÖN KOŞULU
    ↓
FAZ 2 (Anlık)    ←── FAZ 1 tamamlanmadan başlanamaz
    ↓
FAZ 3 (Canlı Önizleme)  ←── FAZ 1 + FAZ 2
FAZ 5 (Kod Editörü)     ←── FAZ 1 ile paralel yürütülebilir
FAZ 6 (Refactoring)     ←── FAZ 1 ile paralel yürütülebilir

FAZ 4 (Designer)        ←── FAZ 3 tamamlandıktan sonra
FAZ 7 (Gelişmiş)        ←── FAZ 2 + FAZ 3 + FAZ 4 sonrası
```

---

## 📊 İlerleme Tablosu

| Faz | Madde Sayısı | Tamamlanan | % |
|-----|-------------|------------|---|
| 🔴 FAZ 1 — WASM Entegrasyonu | 9 | 0 | 0% |
| 🟠 FAZ 2 — Anlık Geri Bildirim | 8 | 0 | 0% |
| 🟠 FAZ 3 — Canlı Önizleme | 7 | 0 | 0% |
| 🟡 FAZ 4 — Designer | 10 | 0 | 0% |
| 🟡 FAZ 5 — Kod Editörü | 9 | 0 | 0% |
| 🟡 FAZ 6 — Refactoring | 9 | 0 | 0% |
| 🟢 FAZ 7 — Gelişmiş | 10 | 0 | 0% |
| **Toplam** | **62** | **0** | **0%** |

> Bu tabloyu her sprint sonunda güncelleyin.

---

## 🏁 "Figma Seviyesi" Kriteri

Aşağıdaki koşulların tümü sağlandığında editör Figma seviyesinde sayılır:

1. ✅ FAZ 1 tamamen bitti (WASM in-process, binary bağımlılığı yok)
2. ✅ Kod yazarken 1 saniye içinde hata gösterimi (FAZ 2)
3. ✅ Widget değişikliği anında önizlemeye yansıyor (FAZ 3 hot reload)
4. ✅ Katmanlar paneli çalışıyor (FAZ 4.1)
5. ✅ `app.js` en az 3 modüle bölünmüş (FAZ 6)

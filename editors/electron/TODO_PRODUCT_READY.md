# ✅ TODO — Product Ready Checklist
### MELP — MLP GUI Designer
> Bu dosyadaki tüm maddeler tamamlandığında editör **production-ready** sayılır.
> Son güncelleme: 25 Şubat 2026 (Sprint 5)

---

## 🔴 KRİTİK — Yayın Öncesi Zorunlu

### 🐛 Bilinen Buglar
- [x] **`🧪 TEST ITEM - PENCERE ÇALIŞIYOR!`** satırını Yardım menüsünden kaldır — `index.html` ~L38
- [x] **`Tümünü Seç (Ctrl+A)`** menü/kısayol çalışmıyor — `app.js` içinde `handleSelectAll()` implement et
- [x] **Undo/Redo yavaşlığı** — çok widget'lı projelerde history trim & lazy snapshot ekle
- [x] **Sağ tık → Yapıştır** (`Ctrl+V`) code-editor context menu'sünde çalışmıyor — clipboard API izni
- [x] **`Ctrl+A`** code-editor içindeyken tüm sayfayı değil sadece editör içeriğini seçmeli
- [x] **Widget pozisyonu kaydetme** — bazı durumlarda drag sonrası `top/left` koordinatları JSON'a doğru yazılmıyor

### 🛡️ Güvenlik & Kararlılık
- [x] **Boş proje adıyla kaydet** — dosya adı validasyonu ekle
- [x] **Büyük `.mlpgui` dosyaları** — yüklerken UI donması önlenecek (async parse)
- [x] **`/tmp` dosyasına yazma hatası** — `writeTempFile` başarısız olursa kullanıcıya net hata mesajı göster
- [x] **Uygulama kapanmadan önce uyarı** — kaydedilmemiş değişiklik varsa onay dialogu göster (Electron `before-quit`)
- [x] **`save-project` / `open-project` / `new-project` / `export-mlp-code` IPC handler'ları eksikti** — `main.js`'e eklendi

---

## 🟠 YÜKSEK ÖNCELİK — v1.1

### 🎨 Görsel Şablon / Tema Sistemi
- [x] **Tema seçim dialogu** oluştur — Görünüm menüsüne "Tema Seç..." ekle
- [x] **Şablon 1: VS Code Dark** (mevcut — varsayılan)
- [x] **Şablon 2: VS Code Light** (açık arka plan, koyu metin)
- [x] **Şablon 3: Dracula** (mor/pembe vurgular, koyu arkaplan)
- [x] **Şablon 4: One Dark Pro** (mavi/yeşil vurgular, Atom tarzı)
- [x] **Şablon 5: Solarized Dark** (sıcak tonlar, düşük kontrast)
- [x] **Şablon 6: High Contrast** (erişilebilirlik — siyah/beyaz/sarı)
- [x] Her tema için `assets/themes/themes.css` dosyasında tanımlı
- [x] Tema seçim penceresinde **canlı önizleme küçük resimleri** göster
- [x] Seçilen tema `localStorage`'a kaydedilsin, uygulama açılışında yüklensin
- [x] `Görünüm > Tema: Dark/Light` toggle'ı → "🎨 Tema Seç..." ile değiştirildi

### 🖱️ Çoklu Widget Seçimi
- [x] **Ctrl+Click** ile aynı anda birden fazla widget seçimi
- [x] **Rubber-band (kauçuk bant) seçimi** — canvas'ta sürükleyerek çoklu seçim
- [x] Çoklu seçimde **toplu taşıma** (tüm seçili widget'lar birlikte hareket eder)
- [x] Çoklu seçimde **toplu silme** (Delete tuşu)
- [x] Çoklu seçimde **toplu kopyala/yapıştır** (birden fazla widget birlikte kopyalanır)

### 📐 Hizalama Araçları
- [x] Toolbar'a hizalama butonları ekle (◧⬌◨▤⬍▥)
- [x] Sol hizala, Sağ hizala, Üst hizala, Alt hizala
- [x] Yatayda ortala, Dikeyde ortala
- [x] Eşit yatay aralık (↔ Yatay Dağıt — 3+ widget)
- [x] Eşit dikey aralık (↕ Dikey Dağıt — 3+ widget)
- [x] **Grid'e yapış (Snap to Grid)** — varsayılan aktif, toggle tuşu (8px grid)

### 🗂️ Widget Gruplama
- [x] Seçili widget'ları gruplama (`Ctrl+G`)
- [x] Grubu çözme (`Ctrl+Shift+G`)
- [x] Grup tıklanınca tüm üyeler otomatik seçilir (birlikte taşınır)
- [x] Grup içi widget'ları ayrı ayrı seçme (çift tık ile değil, Ctrl+Click ile)
- [x] Grup taşıma/yeniden boyutlandırma (seçali grubun tüm üyeleri birlikte hareket eder)

### 📦 Layout Yöneticileri
- [x] **FlowLayout** — widget'lar soldan sağa, satır dolunca alta geçer (Seçili widget'lara görünüm > Akış Düzeni ile uygulanır)
- [x] **GridLayout** — √n sütun ızgara düzeni (Seçili widget'lara görünüm > Izgara Düzeni)
- [x] **StackLayout** — dikey veya yatay yığın (Görünüm > Dikey/Yatay Yığın)
- [ ] Layout seçimi özellik panelinde Panel/GroupBox için kalıcı otomatik düzen (mimari değişiklik gerektirir, v2.0'a ertelendi)

---

## 🟡 ORTA ÖNCELİK — v1.2

### 🔧 Özellik Paneli İyileştirmeleri
- [x] **Özellik arama/filtreleme** — özellik panelinin üstüne arama kutusu
- [x] **Renk seçici iyileştirmesi** — hex input yanına tarayıcı color picker ekle
- [x] **Font ailesi seçici** — textbox'a yazarak değil dropdown ile seçim
- [x] **Tab order** — widget'ların sekme sırası ayarı

### 🧩 Widget Şablonları & Kütüphane
- [x] Sık kullanılan widget gruplarını şablon olarak kaydetme
- [x] Şablon kütüphanesi paneli (Araçlar > Şablonlar)
- [x] Yerleşik şablonlar: Giriş Formu, Ayarlar Paneli, Araç Çubuğu
- [x] Şablon import/export (`.mlptemplate` formatı)

### ✂️ Snippet Sistemi
- [x] Kod editöründe `!` prefix ile snippet tetikleme
- [x] Yerleşik snippet'ler: `!btn-click`, `!if-else`, `!loop`, `!gui-init`, `!func`, `!struct`, `!while`, `!program`
- [x] Kullanıcı tanımlı snippet ekleme (Araçlar > Snippet Yöneticisi)

### 🌐 Çok Dilli UI
- [x] Arayüz metinlerini dil dosyasına taşı (`assets/i18n/tr.json`, `en.json`)
- [x] Dil değiştirme Ayarlar menüsünden
- [x] İngilizce çeviri tamamla
- [x] Seçilen UI dili `settings.json`'a kaydet

### ⚙️ Ayarlar Dialogu
- [x] Araçlar > Ayarlar menü öğesi ekle
- [x] Grafik arayüzde `settings.json` düzenleme
- [x] Derleyici yolu (`compilerPath`) browse butonu ile seç
- [x] `lli` yolu ayarlanabilir
- [x] Otomatik kaydetme aralığı (saniye)
- [x] Başlangıçta son projeyi aç seçeneği

### 📋 Custom Widget Oluşturma
- [ ] Widget türü tanımlama dialogu
- [ ] Mevcut widget'lardan türetme
- [ ] Custom widget'ı toolbox'a ekleme
- [ ] Custom widget'ı `.mlpwidget` olarak dışa aktarma

---

## 🟢 DÜŞÜK ÖNCELİK — v1.3

### 🖥️ Gerçek Zamanlı Önizleme (SDL)
- [ ] Görünüm sekmesinde MLP çalışma zamanı entegrasyonu
- [ ] SDL penceresi Electron içinde gömülü göster
- [ ] **Hot reload** — widget değişikliği anında önizlemeye yansısın
- [ ] Önizleme penceresinde boyut seçimi (800×600, 1024×768, 1920×1080)

### 🐞 Breakpoint & Debug
- [ ] Kod editöründe satır numarasına tıklayarak breakpoint ekle/kaldır
- [ ] Breakpoint listesi paneli
- [ ] Çalışma zamanında değişken izleme (watch panel)
- [ ] Adım adım yürütme (step-over, step-into)

### 🎬 Widget Animasyonları
- [ ] Özellik paneline "Animasyon" sekmesi ekle
- [ ] Fade in/out, slide, pulse efektleri
- [ ] Animasyon tetikleyici: onClick, onHover, onLoad
- [ ] Animasyon önizlemesi tasarım sekmesinde

### 📱 Responsive Design
- [ ] Farklı pencere boyutları için layout break-point'leri
- [ ] Widget'ların yüzde tabanlı pozisyonlama seçeneği
- [ ] Responsive önizleme (mobil / tablet / masaüstü boyutları)

### 🔗 Proje Yönetimi İyileştirmeleri
- [x] **Son açılan projeler** listesi (Dosya > Son Projeler, son 5 proje)
- [x] **Otomatik kaydetme** — ayarlanan aralıkla arka planda kaydet
- [x] **Proje adı** pencere başlığında göster (`MELP — [proje.mlpgui]`)
- [x] **Kaydedilmemiş değişiklik göstergesi** — başlık çubuğunda `●` ikonu

### 🧪 Test & Kalite
- [x] Widget oluşturma/silme/taşıma için unit testler
- [x] Kod üretimi doğruluk testleri
- [x] JSON serialize/deserialize round-trip testleri
- [x] `npm test` script'ini package.json'a ekle

---

## 📦 Yayın Hazırlığı

### 🏗️ Build & Paketleme
- [x] `electron-builder` konfigürasyonu tamamla (`package.json`)
- [x] Linux `.deb` ve `.AppImage` paket üretimi
- [x] Windows `.exe` (NSIS installer) paket üretimi
- [x] Uygulama ikonu (`assets/icon.png`, `.ico`, `.icns`) oluştur
- [x] Build süreci CI/CD ile otomatikleştir

### 📝 Dokümantasyon
- [x] `README.md` ekran görüntüleri ekle — `assets/screenshot.png`
- [x] Widget referans kılavuzu (tüm widget özellikleri) — `docs/widget-reference.md`
- [x] Event handler yazma rehberi — `docs/event-handlers.md`
- [x] Örnek proje kütüphanesi (`examples/` klasörünü zenginleştir) — 3 örnek eklendi
- [x] `CONTRIBUTING.md` ekle

### 🧹 Kod Temizliği
- [ ] `index.html`'deki inline `style` attribute'ları CSS sınıflarına taşı
- [ ] `app.js` 2269 satır — büyük fonksiyonları ayrı modüllere böl
- [x] Backup dosyalarını (`*.backup`, `*.backup2`, `*.backup3`) sil veya `.gitignore`'a ekle
- [x] `DEBUG_INTELLISENSE.md`, `CACHE_FIX.md`, `FINAL_FIX.md`, `SYNTAX_FIX.md` geçici doc dosyalarını temizle
- [x] `test_drag.html`, `test_intellisense.html` test dosyalarını `tests/` klasörüne taşı

---

## 📊 İlerleme Özeti

| Kategori | Toplam | Tamamlanan | Yüzde |
|----------|--------|------------|-------|
| 🔴 **Kritik (11)** | 11 | 11 | **100%** |
| 🟠 v1.1 | 35 | 35 | **100%** |
| 🟡 v1.2 | 25 | 21 | **84%** |
| 🟢 v1.3 | 16 | 1 | **6%** |
| 📦 Yayın | 14 | 13 | **93%** |
| **Toplam** | **101** | **83** | **~82%** |

> Bu tabloyu her sprint sonunda güncelleyin.

---

## 🏁 "Product Ready" Kriteri

Editör aşağıdaki koşulların **tümü** sağlandığında yayına hazır sayılır:

1. ✅ Tüm 🔴 Kritik maddeler tamamlanmış
2. ✅ Tüm 🟠 v1.1 maddeleri tamamlanmış
3. ✅ En az 2 tema (Dark + Light) çalışıyor
4. ✅ Uygulama Linux'ta `.AppImage` veya `.deb` olarak paketlenebiliyor
5. ✅ `npm test` komutu hatasız geçiyor
6. ✅ `README.md` ekran görüntüleriyle güncel

# MELP'e Katkıda Bulunma Rehberi

MELP — MLP GUI Designer projesine katkıda bulunduğunuz için teşekkür ederiz!  
Bu belge katkı sürecini adım adım açıklar.

---

## İçindekiler

- [Geliştirme Ortamı Kurulumu](#geliştirme-ortamı-kurulumu)
- [Proje Yapısı](#proje-yapısı)
- [Değişiklik Yapma](#değişiklik-yapma)
- [Testler](#testler)
- [Pull Request Gönderme](#pull-request-gönderme)
- [Kod Stili](#kod-stili)
- [Hata Bildirme](#hata-bildirme)
- [Özellik Önerisi](#özellik-önerisi)

---

## Geliştirme Ortamı Kurulumu

### Gereksinimler

- **Node.js** 18+ ve **npm** 9+
- **Git**
- Linux, macOS veya Windows

### Adımlar

```bash
# 1. Repoyu fork'layın ve klonlayın
git clone https://github.com/KULLANICI_ADINIZ/MELP.git
cd MELP

# 2. Bağımlılıkları yükleyin
npm install

# 3. Uygulamayı başlatın
npm start

# 4. Geliştirme modunda başlatın (DevTools açık)
npm run dev

# 5. Testleri çalıştırın
npm test
```

---

## Proje Yapısı

```
melp-ide/
├── main.js              # Electron ana süreç — IPC handler'lar, pencere yönetimi
├── preload.js           # contextBridge — güvenli API köprüsü
├── index.html           # Ana HTML — menüler, paneller, modaller
├── assets/
│   ├── icon.png/svg     # Uygulama ikonları
│   └── styles/main.css  # Ana stil
├── src/
│   ├── app.js           # Renderer mantığı — olay yönetimi, UI koordinasyonu
│   ├── components/
│   │   ├── designer.js  # Canvas, widget seçimi, sürükleme, kauçuk bant
│   │   ├── widget.js    # Widget sınıfı — render, güncelleme
│   │   ├── properties.js # Özellik paneli — form oluşturma
│   │   └── code-generator.js # MLP kaynak kodu üretimi
│   └── utils/
│       ├── helpers.js      # Yardımcı saf fonksiyonlar (Node uyumlu)
│       ├── intellisense.js # Kod editörü otomatik tamamlama
│       └── syntax-validator.js # MLP sözdizimi doğrulama
└── tests/
    ├── run-tests.js     # Test runner
    └── test-helpers.js  # helpers.js birim testleri
```

### Önemli tasarım kararları

- **Renderer ↔ Main iletişimi** her zaman `preload.js` üzerinden `contextBridge` ile yapılır; `nodeIntegration: false`.
- **Widget'lar** `Widget` sınıfı örnekleridir; hem mantığı hem DOM elementini barındırır.
- **Undo/Redo**: `app.js` üzerindeki `undoStack` / `redoStack` dizileri, JSON snapshot'larını tutar.
- **Temalar**: `assets/themes/themes.css` tek dosyada tutulur; `<body data-theme="...">` ile aktif edilir.

---

## Değişiklik Yapma

### Branch adlandırma kuralları

```
feat/kısa-özellik-açıklaması   # Yeni özellik
fix/kısa-hata-açıklaması       # Hata düzeltmesi
refactor/hedef                 # Kod yeniden yapılandırma
docs/neyi-belgelediği          # Sadece belge değişiklikleri
test/ne-test-edildiği          # Sadece test değişiklikleri
```

### Commit mesajı kuralları

[Conventional Commits](https://www.conventionalcommits.org/) standardını kullanın:

```
feat: Snippet Yöneticisi dialogu ekle
fix: rubber-band seçiminde grup üyelerini eksik ekleme
refactor: designer.js onCanvasMouseDown grup farkındalığı
docs: CONTRIBUTING.md ekle
test: hexToRgb geçersiz girdi edge case'i
```

---

## Testler

```bash
npm test          # Tüm testleri çalıştır
```

Test dosyaları `tests/` klasöründe bulunur:

- `test-helpers.js` — `src/utils/helpers.js` için birim testler
- `run-tests.js` — test runner, tüm `test-*.js` dosyalarını çalıştırır

### Yeni test ekleme

`tests/` klasörüne `test-MODUL.js` adlı bir dosya oluşturun:

```javascript
const { fonksiyon1, fonksiyon2 } = require('../src/utils/modul.js');

module.exports = function ({ test, assert, assertEqual }) {
  test('fonksiyon1 beklenen sonucu döndürmeli', () => {
    assertEqual(fonksiyon1('girdi'), 'beklenen', 'açıklama');
  });
};
```

Sonra `tests/run-tests.js` içine ekleyin:

```javascript
console.log('modul.js:');
require('./test-MODUL.js')(ctx);
```

---

## Pull Request Gönderme

1. **Fork** yapın ve feature branch oluşturun
2. Değişikliklerinizi yapın
3. `npm test` çalıştırın — **sıfır başarısızlık** olmalı
4. Commit'lerinizi yukarıdaki kurallara uygun yazın
5. PR açın ve aşağıdakileri doldurun:
   - Değişikliğin amacı
   - Test ettiğiniz senaryolar
   - Varsa ekran görüntüsü (UI değişiklikleri için)

### PR boyutu

- **Küçük PR'lar tercih edilir** — tek bir özellik veya düzeltme
- Büyük özellikler için önce bir Issue açın ve tartışın

---

## Kod Stili

- **Girintileme**: 2 boşluk
- **Noktalı virgül**: kullanın
- **String**: tek tırnak (`'...'`)
- **Değişken isimlendirme**: `camelCase`; sınıflar `PascalCase`
- **Yorum dili**: Türkçe (kullanıcı arayüzüne yakın mantık), İngilizce (genel algoritmalar)
- Kod editörünüzde **ESLint** veya bir linter yapılandırmanız önerilir (ancak zorunlu değil)

---

## Hata Bildirme

Lütfen bir Issue açın ve şunları ekleyin:

- **Platform**: Linux / Windows / macOS sürümü
- **Electron sürümü**: `package.json`'daki sürüm
- **Yeniden üretme adımları**: "şunu → şunu → çöktü"
- **Beklenen davranış** vs **gerçek davranış**
- **Ekran görüntüsü** varsa

---

## Özellik Önerisi

`TODO_PRODUCT_READY.md` dosyasındaki listeye bakın — belki öneriniz zaten planlı!  
Listede yoksa bir Issue açın `[ÖNERI]` etiketi ile.

---

Katkılarınız için teşekkürler! 🎉

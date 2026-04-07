/**
 * MELP i18n — Arayüz çeviri sistemi
 *
 * Kullanım:
 *   t('menu.file.save')          → "Kaydet" / "Save"
 *   t('widget.count', { n: 5 })  → "Widget Sayısı: 5"
 *   applyTranslations()          → data-i18n attribute'larına çeviriyi uygula
 *   setLanguage('en')            → dili değiştir, tüm UI'ı güncelle
 */

const I18N_STORAGE_KEY = 'melp-ui-language';
const SUPPORTED_LANGS  = ['tr', 'en'];

// Yüklü çeviri tabloları
const _catalogs = {};
let   _lang     = 'tr';

/**
 * Bir dil kataloğunu asenkron yükle (fetch veya inline require)
 */
async function _loadCatalog(lang) {
  if (_catalogs[lang]) return _catalogs[lang];
  try {
    // Electron / tarayıcı: fetch ile yükle
    const url = `assets/i18n/${lang}.json`;
    const res = await fetch(url);
    if (!res.ok) throw new Error(`HTTP ${res.status}`);
    _catalogs[lang] = await res.json();
  } catch (err) {
    console.warn(`[i18n] "${lang}" kataloğu yüklenemedi:`, err.message);
    _catalogs[lang] = {};
  }
  return _catalogs[lang];
}

/**
 * Çeviriyi al. Anahtar bulunamazsa anahtarın kendisini döndür.
 * @param {string} key   — nokta ile ayrılmış anahtar, örn. "menu.file.save"
 * @param {object} [vars] — şablon değişkenleri; {n} → {{n}} yerlerine geçer
 */
function t(key, vars) {
  const catalog = _catalogs[_lang] || _catalogs['tr'] || {};
  let str = catalog[key];
  if (str === undefined) {
    // Türkçe fallback
    str = (_catalogs['tr'] || {})[key];
  }
  if (str === undefined) str = key; // son çare: anahtar
  if (vars) {
    Object.keys(vars).forEach(k => {
      str = str.replace(new RegExp(`\\{\\{?${k}\\}?\\}`, 'g'), vars[k]);
    });
  }
  return str;
}

/**
 * Mevcut aktif dili döndür.
 */
function getCurrentLang() { return _lang; }

/**
 * Desteklenen dil listesini döndür.
 */
function getSupportedLanguages() {
  return [
    { id: 'tr', name: 'Türkçe' },
    { id: 'en', name: 'English' },
  ];
}

/**
 * data-i18n attribute'u olan tüm elementlere çeviriyi uygula.
 * <span data-i18n="menu.file.save"></span>  →  "Kaydet"
 * <input data-i18n-placeholder="panel.search">  →  placeholder="Ara..."
 */
function applyTranslations() {
  // textContent güncellemesi
  document.querySelectorAll('[data-i18n]').forEach(el => {
    const key = el.dataset.i18n;
    el.textContent = t(key);
  });
  // placeholder güncellemesi
  document.querySelectorAll('[data-i18n-placeholder]').forEach(el => {
    el.placeholder = t(el.dataset.i18nPlaceholder);
  });
  // title (tooltip) güncellemesi
  document.querySelectorAll('[data-i18n-title]').forEach(el => {
    el.title = t(el.dataset.i18nTitle);
  });
}

/**
 * Dili değiştir, kataloğu yükle, UI'ı güncelle.
 * @param {string} lang — 'tr' veya 'en'
 */
async function setLanguage(lang) {
  if (!SUPPORTED_LANGS.includes(lang)) {
    console.warn(`[i18n] Desteklenmeyen dil: "${lang}". Türkçe kullanılıyor.`);
    lang = 'tr';
  }
  _lang = lang;
  localStorage.setItem(I18N_STORAGE_KEY, lang);
  await _loadCatalog(lang);
  applyTranslations();
  return lang;
}

/**
 * Uygulama başlangıcında çağrılır — kaydedilen dili yükle.
 */
async function initI18n() {
  // Önce settings.json'dan, sonra localStorage'dan oku
  let lang = 'tr';
  try {
    const stored = localStorage.getItem(I18N_STORAGE_KEY);
    if (stored && SUPPORTED_LANGS.includes(stored)) lang = stored;
  } catch { /* localStorage kullanılamıyor */ }

  _lang = lang;
  // Her iki dili de önceden yükle (geçiş hızlı olsun)
  await Promise.all(SUPPORTED_LANGS.map(l => _loadCatalog(l)));
  applyTranslations();
  return lang;
}

// Tarayıcı ortamında global olarak erişilebilir yap
if (typeof window !== 'undefined') {
  window.i18n = { t, setLanguage, getCurrentLang, getSupportedLanguages, applyTranslations, initI18n };
}

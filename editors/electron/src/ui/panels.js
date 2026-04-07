// panels.js -- temalar, zoom, izgara, hizalama
// ── Tema Sistemi ──────────────────────────────────────────────────────────────
const THEMES = {
  'dark':          null,
  'light':         'light-theme',
  'dracula':       'theme-dracula',
  'one-dark':      'theme-one-dark',
  'solarized':     'theme-solarized',
  'contrast':      'theme-contrast',
  'melp-premium':  'theme-melp-premium'
};
const THEME_NAMES = {
  'dark': 'VS Code Dark', 'light': 'VS Code Light',
  'dracula': 'Dracula', 'one-dark': 'One Dark Pro',
  'solarized': 'Solarized Dark', 'contrast': 'High Contrast',
  'melp-premium': 'MELP Premium'
};

// Her tema için CSS değişken değerleri (JS ile doğrudan :root'a uygulanır)
const THEME_VARS = {
  'dark': {
    '--bg-primary':    '#1e1e1e',
    '--bg-secondary':  '#252526',
    '--bg-tertiary':   '#2d2d30',
    '--bg-quaternary': '#3c3c3c',
    '--border-color':  '#3e3e42',
    '--text-primary':  '#d4d4d4',
    '--text-secondary':'#cccccc',
    '--text-tertiary': '#888888',
    '--text-active':   '#ffffff',
    '--accent-color':  '#007acc',
    '--accent-hover':  '#1177bb',
    '--accent-active': '#094771',
    '--button-bg':     '#0e639c',
    '--shadow-color':  'rgba(0,0,0,0.4)',
    '--grid-color':    'rgba(255,255,255,0.05)',
  },
  'light': {
    '--bg-primary':    '#ffffff',
    '--bg-secondary':  '#f3f3f3',
    '--bg-tertiary':   '#e8e8e8',
    '--bg-quaternary': '#f0f0f0',
    '--border-color':  '#d0d0d0',
    '--text-primary':  '#1e1e1e',
    '--text-secondary':'#333333',
    '--text-tertiary': '#666666',
    '--text-active':   '#000000',
    '--accent-color':  '#0078d4',
    '--accent-hover':  '#106ebe',
    '--accent-active': '#005a9e',
    '--button-bg':     '#0078d4',
    '--shadow-color':  'rgba(0,0,0,0.1)',
    '--grid-color':    'rgba(0,0,0,0.05)',
  },
  'dracula': {
    '--bg-primary':    '#282a36',
    '--bg-secondary':  '#21222c',
    '--bg-tertiary':   '#44475a',
    '--bg-quaternary': '#3d3f4e',
    '--border-color':  '#6272a4',
    '--text-primary':  '#f8f8f2',
    '--text-secondary':'#cdd6f4',
    '--text-tertiary': '#6272a4',
    '--text-active':   '#ffffff',
    '--accent-color':  '#bd93f9',
    '--accent-hover':  '#c9a8ff',
    '--accent-active': '#7c53cc',
    '--button-bg':     '#6272a4',
    '--shadow-color':  'rgba(0,0,0,0.55)',
    '--grid-color':    'rgba(248,248,242,0.04)',
  },
  'one-dark': {
    '--bg-primary':    '#282c34',
    '--bg-secondary':  '#21252b',
    '--bg-tertiary':   '#2c313c',
    '--bg-quaternary': '#353b45',
    '--border-color':  '#3e4451',
    '--text-primary':  '#abb2bf',
    '--text-secondary':'#c8ccd4',
    '--text-tertiary': '#5c6370',
    '--text-active':   '#e6e6e6',
    '--accent-color':  '#61afef',
    '--accent-hover':  '#528bff',
    '--accent-active': '#3d78cf',
    '--button-bg':     '#4d78cc',
    '--shadow-color':  'rgba(0,0,0,0.45)',
    '--grid-color':    'rgba(255,255,255,0.04)',
  },
  'solarized': {
    '--bg-primary':    '#002b36',
    '--bg-secondary':  '#073642',
    '--bg-tertiary':   '#094552',
    '--bg-quaternary': '#0a4f5e',
    '--border-color':  '#2d5a68',
    '--text-primary':  '#839496',
    '--text-secondary':'#93a1a1',
    '--text-tertiary': '#586e75',
    '--text-active':   '#eee8d5',
    '--accent-color':  '#268bd2',
    '--accent-hover':  '#2ea0e8',
    '--accent-active': '#1a6faa',
    '--button-bg':     '#1c6b8a',
    '--shadow-color':  'rgba(0,0,0,0.45)',
    '--grid-color':    'rgba(131,148,150,0.07)',
  },
  'contrast': {
    '--bg-primary':    '#000000',
    '--bg-secondary':  '#0a0a0a',
    '--bg-tertiary':   '#111111',
    '--bg-quaternary': '#1a1a1a',
    '--border-color':  '#ffffff',
    '--text-primary':  '#ffffff',
    '--text-secondary':'#ffffff',
    '--text-tertiary': '#ffff00',
    '--text-active':   '#ffffff',
    '--accent-color':  '#ffff00',
    '--accent-hover':  '#ffff80',
    '--accent-active': '#cccc00',
    '--button-bg':     '#1a1a1a',
    '--shadow-color':  'rgba(255,255,255,0.15)',
    '--grid-color':    'rgba(255,255,255,0.10)',
  },
  'melp-premium': {
    '--bg-primary':    '#11111b',
    '--bg-secondary':  '#1e1e2e',
    '--bg-tertiary':   '#181825',
    '--bg-quaternary': '#313244',
    '--border-color':  '#45475a',
    '--text-primary':  '#cdd6f4',
    '--text-secondary':'#bac2de',
    '--text-tertiary': '#6c7086',
    '--text-active':   '#ffffff',
    '--accent-color':  '#7c3aed',
    '--accent-hover':  '#9761f5',
    '--accent-active': '#5b21b6',
    '--button-bg':     '#7c3aed',
    '--shadow-color':  'rgba(0,0,0,0.6)',
    '--grid-color':    'rgba(124,58,237,0.07)',
  },
};

let currentTheme = 'dark';

function applyTheme(name) {
  if (!THEMES.hasOwnProperty(name)) return;
  const body = document.body;

  // Body sınıflarını güncelle (layout stilleri için class tabanlı CSS gerekli)
  body.classList.remove('light-theme');
  Object.values(THEMES).filter(Boolean).forEach(c => body.classList.remove(c));
  const cls = THEMES[name];
  if (cls) body.classList.add(cls);

  // CSS değişkenlerini doğrudan body inline stiline yaz.
  // body.theme-melp-premium { --bg-primary: ... } kuralı body üzerinde tanımlı
  // olduğundan html/:root inline stilinden daha yakın ve cascade'de kazanıyor.
  // body.style.setProperty() ise o kuralı doğrudan ezer.
  const vars = THEME_VARS[name];
  if (vars) {
    for (const [prop, val] of Object.entries(vars)) {
      body.style.setProperty(prop, val);
    }
  }

  currentTheme = name;
  _updateGridColor();

  // Overlay açıksa içindeki active kartı güncelle
  const overlay = document.getElementById('theme-picker-overlay');
  if (overlay && !overlay.classList.contains('hidden')) {
    overlay.querySelectorAll('.theme-card').forEach(card => {
      card.classList.toggle('active', card.dataset.theme === name);
    });
  }

  try { localStorage.setItem('mlp-gui-designer-theme', name); } catch(e) {}
  showStatus(THEME_NAMES[name] + ' tema aktif');
}

function handleThemeSelect() {
  const overlay = document.getElementById('theme-picker-overlay');
  if (!overlay) return;
  // Önce overlay'i göster (paint bir sonraki frame'de)
  overlay.classList.remove('hidden');
  // Active card güncellemesini paint'ten sonraki frame'e bırak
  requestAnimationFrame(() => {
    overlay.querySelectorAll('.theme-card').forEach(card => {
      card.classList.toggle('active', card.dataset.theme === currentTheme);
    });
  });
}

// Tema picker dialog bağlantıları — initializeUI()'dan çağrılır
function initThemePicker() {
  const overlay = document.getElementById('theme-picker-overlay');
  if (!overlay) return;

  // Kapat düğmesi — overlay içindeki butonu bul (ID çakışmasına karşı)
  overlay.querySelector('.theme-picker-close').addEventListener('click', () => {
    overlay.classList.add('hidden');
  });

  // Overlay arkasına tıklayınca kapat
  overlay.addEventListener('click', (e) => {
    if (e.target === overlay) overlay.classList.add('hidden');
  });

  // Kart seçimi — her zaman overlay'i kapat (applyTheme hata verse bile)
  overlay.querySelectorAll('.theme-card').forEach(card => {
    card.addEventListener('click', () => {
      try { applyTheme(card.dataset.theme); } catch(e) { console.error('applyTheme error:', e); }
      overlay.classList.add('hidden');
    });
  });
}

// Sayfa yüklenince kaydedilmiş temayı yükle
function loadSavedTheme() {
  try {
    const saved = localStorage.getItem('mlp-gui-designer-theme');
    // Kayıtlı tema varsa onu, yoksa varsayılan 'dark' uygula.
    // Her iki durumda da body.style üzerine CSS değişkenleri yazılır;
    // böylece body.theme-melp-premium kuralları override edilir.
    applyTheme((saved && THEMES.hasOwnProperty(saved)) ? saved : 'dark');
  } catch(e) { applyTheme('dark'); }
}

// eski toggle — arka uyumluluk için saklı (bazı yerlerde çağrılıyor olabilir)
function handleToggleTheme() { handleThemeSelect(); }

function handleToggleToolbox() {
  const leftPanel = document.querySelector('.left-panel');
  if (leftPanel.style.display === 'none') {
    leftPanel.style.display = 'flex';
    showStatus('Widget araçları gösteriliyor');
  } else {
    leftPanel.style.display = 'none';
    showStatus('Widget araçları gizlendi');
  }
}

function handleToggleProperties() {
  const rightPanel = document.querySelector('.right-panel');
  if (rightPanel.style.display === 'none') {
    rightPanel.style.display = 'flex';
    showStatus('Özellikler gösteriliyor');
  } else {
    rightPanel.style.display = 'none';
    showStatus('Özellikler gizlendi');
  }
}

let currentZoom = 1.0;

function handleZoomIn() {
  currentZoom = Math.min(currentZoom + 0.1, 2.0);
  applyZoom();
  showStatus(`Zoom: ${Math.round(currentZoom * 100)}%`);
}

function handleZoomOut() {
  currentZoom = Math.max(currentZoom - 0.1, 0.5);
  applyZoom();
  showStatus(`Zoom: ${Math.round(currentZoom * 100)}%`);
}

function handleZoomReset() {
  currentZoom = 1.0;
  applyZoom();
  showStatus('Zoom sıfırlandı (100%)');
}

function applyZoom() {
  const canvas = document.getElementById('canvas');
  canvas.style.transform = `scale(${currentZoom})`;
  canvas.style.transformOrigin = 'top left';
}

let gridVisible = true;
let snapToGrid = true;
const SNAP_SIZE = 8; // 8px grid'e yapış

function _updateGridColor() {
  if (!gridVisible) return;
  const canvas = document.getElementById('canvas');
  if (!canvas) return;
  // canvasSettings'te özel renk varsa onu kullan
  const cs = designer?.canvasSettings;
  let c = cs?.gridColor || '';
  if (!c) {
    // Temaya göre otomatik renk
    const isLight = currentTheme === 'light';
    c = isLight ? 'rgba(0,0,0,.06)' : 'rgba(255,255,255,.05)';
  }
  const size = cs?.gridSize || 20;
  canvas.style.backgroundSize  = `${size}px ${size}px`;
  canvas.style.backgroundImage =
    `linear-gradient(${c} 1px, transparent 1px), ` +
    `linear-gradient(90deg, ${c} 1px, transparent 1px)`;
}

function handleToggleGrid() {
  const canvas = document.getElementById('canvas');
  gridVisible = !gridVisible;
  if (gridVisible) { _updateGridColor(); showStatus('Izgara gösteriliyor'); }
  else { canvas.style.backgroundImage = 'none'; showStatus('Izgara gizlendi'); }
}

function handleSnapGrid() {
  snapToGrid = !snapToGrid;
  const item = document.getElementById('menu-snap-grid');
  if (item) item.style.fontWeight = snapToGrid ? 'bold' : 'normal';
  showStatus(snapToGrid ? 'Snap aktif' : 'Snap devre dışı');
  // Designer'a snap durumunu bildir
  if (designer) designer.snapSize = snapToGrid ? SNAP_SIZE : 1;
}

// ==================== HİZALAMA FONKSİYONLARI ====================

function handleAlign(direction) {
  if (!designer || !designer.selectedWidgets || designer.selectedWidgets.size < 2) {
    showStatus('Hizalamak için en az 2 widget seçin (Ctrl+Click)');
    return;
  }
  const widgets = [...designer.selectedWidgets];
  const refs = widgets.map(w => ({
    w,
    x:      w.properties.x      || 0,
    y:      w.properties.y      || 0,
    width:  w.properties.width  || 100,
    height: w.properties.height || 30
  }));

  switch (direction) {
    case 'left': {
      const minX = Math.min(...refs.map(r => r.x));
      refs.forEach(r => designer.updateWidgetProperty(r.w, 'x', minX));
      break;
    }
    case 'right': {
      const maxR = Math.max(...refs.map(r => r.x + r.width));
      refs.forEach(r => designer.updateWidgetProperty(r.w, 'x', maxR - r.width));
      break;
    }
    case 'top': {
      const minY = Math.min(...refs.map(r => r.y));
      refs.forEach(r => designer.updateWidgetProperty(r.w, 'y', minY));
      break;
    }
    case 'bottom': {
      const maxB = Math.max(...refs.map(r => r.y + r.height));
      refs.forEach(r => designer.updateWidgetProperty(r.w, 'y', maxB - r.height));
      break;
    }
    case 'center-h': {
      const cX = refs.reduce((s, r) => s + r.x + r.width / 2, 0) / refs.length;
      refs.forEach(r => designer.updateWidgetProperty(r.w, 'x', Math.round(cX - r.width / 2)));
      break;
    }
    case 'center-v': {
      const cY = refs.reduce((s, r) => s + r.y + r.height / 2, 0) / refs.length;
      refs.forEach(r => designer.updateWidgetProperty(r.w, 'y', Math.round(cY - r.height / 2)));
      break;
    }
    case 'distribute-h': {
      if (refs.length < 3) { showStatus('Yatay dağıtma için en az 3 widget seçin'); return; }
      const sorted = refs.slice().sort((a, b) => a.x - b.x);
      const minX = sorted[0].x;
      const maxR = sorted[sorted.length - 1].x + sorted[sorted.length - 1].width;
      const totalW = sorted.reduce((s, r) => s + r.width, 0);
      const gap = (maxR - minX - totalW) / (sorted.length - 1);
      let curX = minX;
      sorted.forEach(r => { designer.updateWidgetProperty(r.w, 'x', Math.round(curX)); curX += r.width + gap; });
      break;
    }
    case 'distribute-v': {
      if (refs.length < 3) { showStatus('Dikey dağıtma için en az 3 widget seçin'); return; }
      const sorted = refs.slice().sort((a, b) => a.y - b.y);
      const minY = sorted[0].y;
      const maxB = sorted[sorted.length - 1].y + sorted[sorted.length - 1].height;
      const totalH = sorted.reduce((s, r) => s + r.height, 0);
      const gap = (maxB - minY - totalH) / (sorted.length - 1);
      let curY = minY;
      sorted.forEach(r => { designer.updateWidgetProperty(r.w, 'y', Math.round(curY)); curY += r.height + gap; });
      break;
    }
  }
  saveToUndoStack();
  showStatus('Hizalandı: ' + direction);
}

// ==================== LAYOUT YÖNETİCİLERİ (Akıllı Düzen) ====================
// Seçili widget'ları istenen düzene göre yeniden konumlandırır

function handleSmartArrange(mode) {
  if (!designer || !designer.selectedWidgets || designer.selectedWidgets.size < 2) {
    showStatus('Düzenlemek için en az 2 widget seçin (Ctrl+Click)');
    return;
  }
  const widgets = [...designer.selectedWidgets];
  // Konumlarına göre sırala (Y önce, sonra X)
  const sorted = widgets.slice().sort((a, b) =>
    a.properties.y !== b.properties.y ? a.properties.y - b.properties.y : a.properties.x - b.properties.x
  );
  const minX = Math.min(...widgets.map(w => w.properties.x));
  const minY = Math.min(...widgets.map(w => w.properties.y));
  const GAP  = 8;

  if (mode === 'flow') {
    // Soldan sağa akar, canvas genişliğine göre alt satıra geçer
    const canvasW = designer.canvas.offsetWidth || 800;
    let x = minX, y = minY, rowH = 0;
    sorted.forEach(w => {
      if (x + w.properties.width > canvasW && x > minX) {
        x = minX; y += rowH + GAP; rowH = 0;
      }
      designer.updateWidgetProperty(w, 'x', Math.round(x));
      designer.updateWidgetProperty(w, 'y', Math.round(y));
      x    += w.properties.width  + GAP;
      rowH  = Math.max(rowH, w.properties.height);
    });

  } else if (mode === 'stack-v') {
    // Dikey yığın (soldan hizalı)
    let y = minY;
    sorted.forEach(w => {
      designer.updateWidgetProperty(w, 'x', minX);
      designer.updateWidgetProperty(w, 'y', Math.round(y));
      y += w.properties.height + GAP;
    });

  } else if (mode === 'stack-h') {
    // Yatay yığın (üstten hizalı)
    let x = minX;
    const sortedH = widgets.slice().sort((a, b) => a.properties.x - b.properties.x);
    sortedH.forEach(w => {
      designer.updateWidgetProperty(w, 'x', Math.round(x));
      designer.updateWidgetProperty(w, 'y', minY);
      x += w.properties.width + GAP;
    });

  } else if (mode === 'grid') {
    // Izgara: √n sütun
    const cols = Math.ceil(Math.sqrt(sorted.length));
    const colW = Math.max(...sorted.map(w => w.properties.width))  + GAP;
    const rowH = Math.max(...sorted.map(w => w.properties.height)) + GAP;
    sorted.forEach((w, i) => {
      designer.updateWidgetProperty(w, 'x', Math.round(minX + (i % cols) * colW));
      designer.updateWidgetProperty(w, 'y', Math.round(minY + Math.floor(i / cols) * rowH));
    });
  }

  saveToUndoStack();
  const names = { flow: 'Akış', grid: 'Izgara', 'stack-v': 'Dikey Yığın', 'stack-h': 'Yatay Yığın' };
  showStatus(`${names[mode]} düzeni uygulandı (${widgets.length} widget)`);
}


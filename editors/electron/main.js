const { app, BrowserWindow, ipcMain, screen, dialog, session, Menu, shell } = require('electron');
Menu.setApplicationMenu(null); // Custom HTML menü bar kullanıldığı için native menü kapatılıyor
const { spawn } = require('child_process');
const fs   = require('fs');
const path = require('path');
const os   = require('os');

const settingsPath = path.join(__dirname, 'settings.json');
let _settingsCache = loadSettings();

function loadSettings() {
  try {
    if (!fs.existsSync(settingsPath)) return {};
    return JSON.parse(fs.readFileSync(settingsPath, 'utf8')) || {};
  } catch (_err) {
    return {};
  }
}

function normalizeLogPath(inputPath) {
  const raw = String(inputPath || '').trim();
  if (!raw) return path.join(app.getPath('userData'), 'logs', 'melp-diagnostics.log');

  try {
    if (fs.existsSync(raw) && fs.statSync(raw).isDirectory()) {
      return path.join(raw, 'melp-diagnostics.log');
    }
  } catch (_err) {
    // Yol mevcut değilse sezgisel çözüme devam et.
  }

  if (!path.extname(raw)) {
    return path.join(raw, 'melp-diagnostics.log');
  }

  return raw;
}

function normalizeDirectoryPath(inputPath) {
  const raw = String(inputPath || '').trim();
  if (!raw) return app.getPath('documents');

  try {
    if (fs.existsSync(raw)) {
      return fs.statSync(raw).isDirectory() ? raw : path.dirname(raw);
    }
  } catch (_err) {
    // Devam: yol yoksa uzantı sezgisi kullan.
  }

  return path.extname(raw) ? path.dirname(raw) : raw;
}

function getLogDir() {
  return path.dirname(getLogFilePath());
}

function getLogFilePath() {
  return normalizeLogPath(_settingsCache.diagnosticLogPath);
}

function isLoggingEnabled() {
  return !!_settingsCache.enableDiagnosticLogging;
}

function safeMeta(meta) {
  if (!meta) return undefined;
  try {
    const json = JSON.stringify(meta);
    return json.length > 4000 ? { truncated: true, preview: json.slice(0, 4000) } : meta;
  } catch (_err) {
    return { serializeError: true };
  }
}

function writeDiagnosticLog(level, source, message, meta) {
  if (!isLoggingEnabled()) return;
  try {
    fs.mkdirSync(getLogDir(), { recursive: true });
    const payload = {
      ts: new Date().toISOString(),
      pid: process.pid,
      level,
      source,
      message,
      meta: safeMeta(meta)
    };
    fs.appendFileSync(getLogFilePath(), JSON.stringify(payload) + '\n', 'utf8');
  } catch (_err) {
    // Log yazımı hata üretse de uygulama akışını bozma.
  }
}

// Vulkan 32-bit ICD uyarıları (i386 kütüphaneleri) zararsız, donanım hızlandırma aktif bırakılıyor

// Content-Security-Policy — Electron güvenlik uyarısını engelle
app.whenReady().then(() => {
  session.defaultSession.webRequest.onHeadersReceived((details, callback) => {
    callback({
      responseHeaders: {
        ...details.responseHeaders,
        'Content-Security-Policy': [
          "default-src 'self'; script-src 'self' 'wasm-unsafe-eval'; style-src 'self' 'unsafe-inline'; img-src 'self' data: blob:; connect-src 'self'; font-src 'self' data:; object-src 'none'; base-uri 'self';"
        ]
      }
    });
  });
});

// Terminal: aktif proses listesi (pencere başına bir tane)
const termProcs = new Map();

ipcMain.on('terminal-exec', (event, { command, cwd }) => {
  // Önceki proses varsa öldür
  const old = termProcs.get(event.sender.id);
  if (old) { try { old.kill(); } catch(_) {} }

  writeDiagnosticLog('info', 'terminal', 'terminal-exec', {
    senderId: event.sender.id,
    cwd: cwd || __dirname,
    commandPreview: String(command || '').slice(0, 300)
  });

  const proc = spawn('/bin/bash', ['-c', command], {
    cwd: cwd || __dirname,
    env: { ...process.env, TERM: 'xterm-256color', FORCE_COLOR: '0' }
  });
  termProcs.set(event.sender.id, proc);

  proc.stdout.on('data', d => event.sender.send('terminal-out', { t: 'stdout', s: d.toString() }));
  proc.stderr.on('data', d => event.sender.send('terminal-out', { t: 'stderr', s: d.toString() }));
  proc.on('close', code => {
    termProcs.delete(event.sender.id);
    writeDiagnosticLog('info', 'terminal', 'terminal-exit', {
      senderId: event.sender.id,
      code
    });
    event.sender.send('terminal-out', { t: 'exit', code });
  });
  proc.on('error', err => {
    writeDiagnosticLog('error', 'terminal', 'terminal-error', {
      senderId: event.sender.id,
      error: err.message
    });
    event.sender.send('terminal-out', { t: 'stderr', s: err.message + '\n' });
  });
});

ipcMain.on('terminal-kill', (event) => {
  const proc = termProcs.get(event.sender.id);
  writeDiagnosticLog('info', 'terminal', 'terminal-kill', { senderId: event.sender.id });
  if (proc) { try { proc.kill(); } catch(_) {} }
});

// Sistem bilgisi (user@host)
ipcMain.handle('get-system-info', async () => ({
  username: os.userInfo().username,
  hostname: os.hostname().split('.')[0],   // kısa hostname
  appPath: __dirname                       // editörün bulunduğu klasör
}));

// Geçici dosya yazma (çalıştır için)
ipcMain.handle('write-temp-file', async (event, { filePath, content }) => {
  try {
    fs.writeFileSync(filePath, content, 'utf8');
    return { ok: true };
  } catch (err) {
    return { ok: false, error: err.message };
  }
});

// ─── Proje kaydet ────────────────────────────────────────────────────────────
let _lastSavePath = null; // Son kayıt yolunu hatırla (Ctrl+S tekrar sorumasın)

ipcMain.handle('save-project', async (event, projectData) => {
  try {
    // Daha önce kaydedilmiş yol varsa doğrudan yaz
    if (_lastSavePath) {
      const json = JSON.stringify(projectData, null, 2);
      fs.writeFileSync(_lastSavePath, json, 'utf8');
      writeDiagnosticLog('info', 'project', 'save-project', {
        mode: 'overwrite',
        filePath: _lastSavePath,
        widgetCount: projectData?.widgets?.length || 0
      });
      return { success: true, filePath: _lastSavePath };
    }

    // İlk kez kaydet: dialog aç
    const win = BrowserWindow.getFocusedWindow();
    const { canceled, filePath } = await dialog.showSaveDialog(win, {
      title: 'Projeyi Kaydet',
      defaultPath: 'proje.mlpgui',
      filters: [{ name: 'MLP GUI Projesi', extensions: ['mlpgui'] }]
    });
    if (canceled || !filePath) return { success: false };

    // Dosya adı doğrulama
    const name = path.basename(filePath, '.mlpgui').trim();
    if (!name) return { success: false, error: 'Geçersiz dosya adı' };

    const json = JSON.stringify(projectData, null, 2);
    fs.writeFileSync(filePath, json, 'utf8');
    _lastSavePath = filePath;
    writeDiagnosticLog('info', 'project', 'save-project', {
      mode: 'save-as',
      filePath,
      widgetCount: projectData?.widgets?.length || 0
    });
    return { success: true, filePath };
  } catch (err) {
    writeDiagnosticLog('error', 'project', 'save-project-failed', { error: err.message });
    return { success: false, error: err.message };
  }
});

// ─── Proje aç ────────────────────────────────────────────────────────────────
ipcMain.handle('open-project', async () => {
  try {
    const win = BrowserWindow.getFocusedWindow();
    const { canceled, filePaths } = await dialog.showOpenDialog(win, {
      title: 'Proje Aç',
      filters: [{ name: 'MLP GUI Projesi', extensions: ['mlpgui'] }],
      properties: ['openFile']
    });
    if (canceled || filePaths.length === 0) return { success: false };

    const filePath = filePaths[0];
    const raw = fs.readFileSync(filePath, 'utf8');
    const data = JSON.parse(raw);
    _lastSavePath = filePath;
    writeDiagnosticLog('info', 'project', 'open-project', {
      filePath,
      widgetCount: data?.widgets?.length || 0
    });
    return { success: true, filePath, data };
  } catch (err) {
    writeDiagnosticLog('error', 'project', 'open-project-failed', { error: err.message });
    return { success: false, error: err.message };
  }
});

// ─── Son projelerden aç (dialog olmadan, doğrudan yoldan) ─────────────────────
ipcMain.handle('open-project-path', async (_event, filePath) => {
  try {
    const raw = fs.readFileSync(filePath, 'utf8');
    const data = JSON.parse(raw);
    _lastSavePath = filePath;
    writeDiagnosticLog('info', 'project', 'open-project-path', {
      filePath,
      widgetCount: data?.widgets?.length || 0
    });
    return { success: true, filePath, data };
  } catch (err) {
    writeDiagnosticLog('error', 'project', 'open-project-path-failed', { filePath, error: err.message });
    return { success: false, error: err.message };
  }
});

// ─── Yeni proje ──────────────────────────────────────────────────────────────
ipcMain.handle('new-project', async () => {
  const win = BrowserWindow.getFocusedWindow();
  const choice = dialog.showMessageBoxSync(win, {
    type: 'question',
    buttons: ['Yeni Proje', 'İptal'],
    defaultId: 0,
    title: 'Yeni Proje',
    message: 'Mevcut projeyi kapatıp yeni proje oluşturmak istiyor musunuz?'
  });
  if (choice === 0) { _lastSavePath = null; }
  return { confirmed: choice === 0 };
});

// ─── MLP kodu dışa aktar ──────────────────────────────────────────────────────
ipcMain.handle('export-mlp-code', async (event, mlpCode) => {
  try {
    const win = BrowserWindow.getFocusedWindow();
    const { canceled, filePath } = await dialog.showSaveDialog(win, {
      title: 'MLP Kodu Dışa Aktar',
      defaultPath: 'uygulama.mlp',
      filters: [{ name: 'MLP Kaynak Dosyası', extensions: ['mlp'] }]
    });
    if (canceled || !filePath) return { success: false };

    fs.writeFileSync(filePath, mlpCode, 'utf8');
    return { success: true, filePath };
  } catch (err) {
    return { success: false, error: err.message };
  }
});

// ─── Uygulamayı Standalone Electron App Olarak Dışa Aktar ────────
ipcMain.handle('export-app', async (_event, appData) => {
  try {
    const win = BrowserWindow.getFocusedWindow();
    const projectName = (appData.projectName || 'melp-uygulama')
      .replace(/[^a-zA-Z0-9_\-]/g, '_');

    const { canceled, filePath: outDir } = await dialog.showSaveDialog(win, {
      title: 'Uygulamayı Dışa Aktar',
      defaultPath: projectName,
      buttonLabel: 'Klasör Oluştur ve Aktar',
      properties: ['createDirectory']
    });
    if (canceled || !outDir) return { success: false };

    // Klasörü oluştur (varsa temizle)
    if (fs.existsSync(outDir)) {
      fs.rmSync(outDir, { recursive: true });
    }
    fs.mkdirSync(outDir, { recursive: true });

    // ── app-data.json: widget layout + event handlers ──────────────
    fs.writeFileSync(
      path.join(outDir, 'app-data.json'),
      JSON.stringify(appData, null, 2), 'utf8'
    );

    // ── package.json: mini electron app ───────────────────────────
    const pkgJson = {
      name: projectName,
      version: '1.0.0',
      description: appData.projectName + ' — MELP ile oluşturuldu',
      main: 'main.js',
      scripts: { start: 'electron .' },
      dependencies: { electron: require('./package.json').devDependencies.electron }
    };
    fs.writeFileSync(path.join(outDir, 'package.json'), JSON.stringify(pkgJson, null, 2), 'utf8');

    // ── Statik dosyaları kopyala ───────────────────────────────────
    // CSS (main.css ve themes.css)
    const assetsDir = path.join(outDir, 'assets', 'styles');
    fs.mkdirSync(assetsDir, { recursive: true });
    fs.copyFileSync(
      path.join(__dirname, 'assets', 'styles', 'main.css'),
      path.join(assetsDir, 'main.css')
    );
    const themesDir = path.join(outDir, 'assets', 'themes');
    fs.mkdirSync(themesDir, { recursive: true });
    fs.copyFileSync(
      path.join(__dirname, 'assets', 'themes', 'themes.css'),
      path.join(themesDir, 'themes.css')
    );

    // preview-form.js (runtime)
    fs.copyFileSync(
      path.join(__dirname, 'preview-form.js'),
      path.join(outDir, 'preview-form.js')
    );

    // ── main.js: mini electron launcher ───────────────────────────
    const cs = appData.canvasSettings || {};
    const appWidth  = cs.width  || 800;
    const appHeight = cs.height || 600;
    const appTitle  = cs.title  || appData.projectName || 'MELP Uygulaması';
    const mainJs = `const { app, BrowserWindow } = require('electron');
const path = require('path');
const fs   = require('fs');

app.whenReady().then(() => {
  const win = new BrowserWindow({
    width: ${appWidth},
    height: ${appHeight},
    title: '${appTitle.replace(/'/g, "\\'")}',
    useContentSize: true,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true
    }
  });
  win.setMenuBarVisibility(false);
  win.loadFile('index.html');
});
app.on('window-all-closed', () => app.quit());
`;
    fs.writeFileSync(path.join(outDir, 'main.js'), mainJs, 'utf8');

    // ── preload.js: app-data.json'u renderer'a aktar ──────────────
    const preloadJs = `const { contextBridge, ipcRenderer } = require('electron');
const path = require('path');
const fs   = require('fs');
contextBridge.exposeInMainWorld('previewAPI', {
  getFormData: () => {
    const dataPath = path.join(__dirname, 'app-data.json');
    return JSON.parse(fs.readFileSync(dataPath, 'utf8'));
  }
});
`;
    fs.writeFileSync(path.join(outDir, 'preload.js'), preloadJs, 'utf8');

    // ── index.html: uygulamanın penceresi ─────────────────────────
    const indexHtml = `<!DOCTYPE html>
<html lang="tr">
<head>
  <meta charset="UTF-8">
  <title>${appTitle}</title>
  <link rel="stylesheet" href="assets/styles/main.css">
  <link rel="stylesheet" href="assets/themes/themes.css">
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body { background: ${cs.backgroundColor || '#1e1e1e'}; overflow: hidden; }
    #form-canvas { position: relative; }
    /* preview-form.html CSS overrides */
    #form-canvas .canvas-widget::after,
    #form-canvas .canvas-widget::before { display: none !important; }
    #form-canvas .canvas-widget { cursor: default; }
    #form-canvas .canvas-widget.widget-button { cursor: pointer; }
    #form-canvas .canvas-widget.widget-button:hover > * { filter: brightness(1.1); }
    #form-canvas .canvas-widget.preview-active > * { transform: scale(0.97); filter: brightness(0.85); }
    #form-canvas .canvas-widget.widget-checkbox,
    #form-canvas .canvas-widget.widget-radio { cursor: pointer; }
    #form-canvas .canvas-widget.widget-checkbox.preview-checked::before {
      display: block !important; content: '\\2713';
      position: absolute; top: 2px; left: 3px;
      font-size: 11px; color: #4ec9b0; font-weight: bold;
    }
    /* Textbox: editable */
    #form-canvas .canvas-widget.widget-textbox .widget-content { display: none; }
    #form-canvas .canvas-widget.widget-textbox input.mlp-input {
      width: 100%; height: 100%; background: transparent; border: none;
      color: inherit; font-size: inherit; font-family: inherit;
      padding: 2px 6px; outline: none; box-sizing: border-box;
    }
    /* Animasyon destek */
    .melp-anim-run {
      animation-name: var(--melp-anim-name);
      animation-duration: var(--melp-anim-dur, 0.5s);
      animation-timing-function: var(--melp-anim-ease, ease-out);
      animation-iteration-count: var(--melp-anim-count, 1);
      animation-fill-mode: both;
    }
    .melp-anim-hover:hover {
      animation-name: var(--melp-anim-name);
      animation-duration: var(--melp-anim-dur, 0.5s);
      animation-timing-function: var(--melp-anim-ease, ease-out);
      animation-iteration-count: var(--melp-anim-count, 1);
    }
  </style>
</head>
<body>
  <div id="form-canvas"></div>
  <script src="app-runtime.js"></script>
</body>
</html>
`;
    fs.writeFileSync(path.join(outDir, 'index.html'), indexHtml, 'utf8');

    // ── app-runtime.js: widget runtime + mini interpreter ─────────
    const runtimeJs = `// MELP App Runtime — MELP GUI Designer tarafından üretildi
// ⚠️ NOT: Bu dosya otomatik üretilmiştir, düzenlemeyin.
// Gerçek native masaüstü uygulamaları MELP Beta sürümünde planlanmaktadır.
// Şu an Electron/HTML tabanlı çalışmaktadır.

(function () {
  const data = window.previewAPI.getFormData();
  const cs = data.canvasSettings || {};

  const canvas = document.getElementById('form-canvas');
  canvas.style.width           = (cs.width  || 800) + 'px';
  canvas.style.height          = (cs.height || 600) + 'px';
  canvas.style.backgroundColor = cs.backgroundColor || '#1e1e1e';
  canvas.innerHTML = data.canvasHtml || '';

  // Tasarım modu kalıntılarını temizle
  canvas.querySelectorAll('.resize-handle, .canvas-info, .rubber-band').forEach(el => el.remove());
  canvas.querySelectorAll('.canvas-widget').forEach(el => {
    el.classList.remove('selected');
    el.style.outline = '';
    el.style.boxShadow = '';
    el.style.cursor = 'default';
  });

  // ── Widget ID → DOM element haritası ────────────────────────────
  const widgetMap = {};
  canvas.querySelectorAll('.canvas-widget[id]').forEach(el => {
    widgetMap[el.id] = el;
  });

  // ── Textbox'ları editable yap ────────────────────────────────────
  canvas.querySelectorAll('.canvas-widget.widget-textbox').forEach(tw => {
    const content = tw.querySelector('.widget-content');
    const placeholder = content ? content.textContent.trim() : '';
    const input = document.createElement('input');
    input.className     = 'mlp-input';
    input.type          = 'text';
    input.placeholder   = placeholder;
    input.style.color   = tw.style.color || '#d4d4d4';
    input.style.fontSize = tw.style.fontSize || '13px';
    input.style.fontFamily = tw.style.fontFamily || 'Arial';
    if (content) content.style.display = 'none';
    tw.appendChild(input);
  });

  // ── GUI Runtime API (event handler'lardan çağrılır) ──────────────
  function GUI_TEXTBOX_GET_TEXT(id) {
    const el = widgetMap[id];
    if (!el) return '';
    const inp = el.querySelector('input.mlp-input');
    return inp ? inp.value : '';
  }
  function GUI_TEXTBOX_SET_TEXT(id, text) {
    const el = widgetMap[id];
    if (!el) return;
    const inp = el.querySelector('input.mlp-input');
    if (inp) inp.value = text;
  }
  function GUI_LABEL_GET_TEXT(id) {
    const el = widgetMap[id];
    if (!el) return '';
    const c = el.querySelector('.widget-content');
    return c ? c.textContent : '';
  }
  function GUI_LABEL_SET_TEXT(id, text) {
    const el = widgetMap[id];
    if (!el) return;
    const c = el.querySelector('.widget-content');
    if (c) c.textContent = text;
  }
  function GUI_WIDGET_SET_VISIBLE(id, visible) {
    const el = widgetMap[id];
    if (!el) return;
    el.style.display = (visible === 0 || visible === false || visible === '0') ? 'none' : '';
  }
  function GUI_WIDGET_SET_ENABLED(id, enabled) {
    const el = widgetMap[id];
    if (!el) return;
    el.style.opacity = (enabled === 0 || enabled === false || enabled === '0') ? '0.4' : '';
    el.style.pointerEvents = (enabled === 0 || enabled === false || enabled === '0') ? 'none' : '';
  }
  function sayiye_cevir(s) { const n = parseFloat(s); return isNaN(n) ? 0 : n; }
  function sayiden_yazi(n) { return String(n); }
  function degisken(v) { return v; } // sugar alias

  // ── Event handler'ları derle ─────────────────────────────────────
  const handlers = {};
  (data.eventHandlers || []).forEach(function(h) {
    try {
      // Handler body'sini fonksiyona wrap et
      // Değişken tanımlama: "degisken x = ..." → "let x = ..."
      let body = h.body || '';
      body = body.replace(/^\\s*degisken\\s+(\\w+)\\s*=/gm, 'let $1 =');
      body = body.replace(/^\\s*eger\\s+(.+?)\\s+ise\\s*$/gm, 'if ($1) {');
      body = body.replace(/^\\s*biteger\\s*$/gm, '}');
      body = body.replace(/^\\s*dondur\\s*$/gm, 'return;');
      // eslint-disable-next-line no-new-func
      handlers[h.name] = new Function(
        'GUI_TEXTBOX_GET_TEXT','GUI_TEXTBOX_SET_TEXT',
        'GUI_LABEL_GET_TEXT','GUI_LABEL_SET_TEXT',
        'GUI_WIDGET_SET_VISIBLE','GUI_WIDGET_SET_ENABLED',
        'sayiye_cevir','sayiden_yazi',
        body
      ).bind(null,
        GUI_TEXTBOX_GET_TEXT, GUI_TEXTBOX_SET_TEXT,
        GUI_LABEL_GET_TEXT,   GUI_LABEL_SET_TEXT,
        GUI_WIDGET_SET_VISIBLE, GUI_WIDGET_SET_ENABLED,
        sayiye_cevir, sayiden_yazi
      );
    } catch(e) {
      console.warn('Handler derleme hatası (' + h.name + '):', e);
    }
  });

  function runHandler(name) {
    if (handlers[name]) {
      try { handlers[name](); }
      catch(e) { console.error('Handler çalıştırma hatası (' + name + '):', e); }
    }
  }

  // ── Widget etkileşimleri ─────────────────────────────────────────
  canvas.querySelectorAll('.canvas-widget.widget-button').forEach(btn => {
    btn.style.cursor = 'pointer';
    const onClick = btn.dataset.onClick || btn.getAttribute('data-on-click') || '';
    btn.addEventListener('mousedown', () => btn.classList.add('preview-active'));
    btn.addEventListener('mouseup',   () => btn.classList.remove('preview-active'));
    btn.addEventListener('mouseleave',() => btn.classList.remove('preview-active'));
    if (onClick) btn.addEventListener('click', () => runHandler(onClick));
  });

  canvas.querySelectorAll('.canvas-widget.widget-checkbox').forEach(cb => {
    cb.style.cursor = 'pointer';
    cb.addEventListener('click', () => cb.classList.toggle('preview-checked'));
  });

  // ── Animasyonlar ─────────────────────────────────────────────────
  (data.animationData || []).forEach(function(item) {
    const anim = item.anim;
    if (!anim || anim.type === 'none') return;
    const el = canvas.querySelector('#' + item.id);
    if (!el) return;
    const dur   = (parseFloat(anim.duration) || 0.5) + 's';
    const durMs = Math.round((parseFloat(anim.duration) || 0.5) * 1000);
    const ease  = anim.easing || 'ease-out';
    const count = anim.repeat ? 'infinite' : '1';
    const name  = 'melp-' + anim.type;
    if (anim.trigger === 'onLoad') {
      el.style.animationName = name; el.style.animationDuration = dur;
      el.style.animationTimingFunction = ease; el.style.animationIterationCount = count;
      el.style.animationFillMode = 'both';
    } else if (anim.trigger === 'onClick') {
      el.style.setProperty('--melp-anim-name', name);
      el.style.setProperty('--melp-anim-dur',  dur);
      el.style.setProperty('--melp-anim-ease', ease);
      el.style.setProperty('--melp-anim-count', count);
      el.addEventListener('click', function() {
        el.classList.remove('melp-anim-run');
        void el.offsetWidth;
        el.classList.add('melp-anim-run');
        setTimeout(function() { el.classList.remove('melp-anim-run'); }, durMs + 150);
      });
    } else if (anim.trigger === 'onHover') {
      el.style.setProperty('--melp-anim-name', name);
      el.style.setProperty('--melp-anim-dur',  dur);
      el.style.setProperty('--melp-anim-ease', ease);
      el.style.setProperty('--melp-anim-count', count);
      el.classList.add('melp-anim-hover');
    }
  });
})();
`;
    fs.writeFileSync(path.join(outDir, 'app-runtime.js'), runtimeJs, 'utf8');

    // ── README.txt: kullanım talimatları ──────────────────────────
    const readme = `${appTitle} — MELP GUI Designer ile oluşturuldu
${'='.repeat(50)}

Bu uygulama MELP GUI Designer (Electron tabanlı) tarafından üretilmiştir.

⚠️  ÖNEMLI NOT:
Bu uygulama şu an Electron/HTML teknolojisi üzerinde çalışmaktadır.
Gerçek native Linux masaüstü uygulaması (GTK/SDL tabanlı) desteği
MELP Beta sürümünde planlanmaktadır.

ÇALIŞTIRMAK İÇİN:
  1. Node.js ve npm yüklü olmalı
  2. Bu klasörde terminal açın:
       npm install
       npm start
  VEYA electron yüklüyse:
       electron .

KLASÖR İÇERİĞİ:
  main.js          — Electron başlatıcı
  index.html       — Uygulama penceresi
  app-runtime.js   — Widget runtime (GUI_* fonksiyonları)
  app-data.json    — Widget layout ve event handler'ları
  preload.js       — Güvenli veri köprüsü
  assets/          — CSS stilleri

Oluşturulma tarihi: ${new Date().toLocaleDateString('tr-TR')}
MELP GUI Designer — https://github.com/mlp-lang/melp-ide
`;
    fs.writeFileSync(path.join(outDir, 'README.txt'), readme, 'utf8');

    return { success: true, outDir };
  } catch (err) {
    return { success: false, error: err.message };
  }
});

// ─── Ayarlar kaydet ───────────────────────────────────────────────
ipcMain.handle('save-settings', async (_event, settings) => {
  try {
    const normalizedSettings = {
      ...settings,
      diagnosticLogPath: normalizeLogPath(settings?.diagnosticLogPath)
    };
    fs.writeFileSync(settingsPath, JSON.stringify(normalizedSettings, null, 2), 'utf8');
    _settingsCache = normalizedSettings || {};
    writeDiagnosticLog('info', 'settings', 'save-settings', {
      loggingEnabled: !!normalizedSettings?.enableDiagnosticLogging,
      autosaveEnabled: !!normalizedSettings?.autosaveEnabled,
      uiLanguage: normalizedSettings?.uiLanguage || 'tr',
      diagnosticLogPath: normalizedSettings?.diagnosticLogPath
    });
    return { success: true };
  } catch (err) {
    return { success: false, error: err.message };
  }
});

ipcMain.handle('get-log-status', async () => ({
  enabled: isLoggingEnabled(),
  logPath: getLogFilePath(),
  logDir: getLogDir()
}));

ipcMain.handle('open-log-folder', async (_event, targetPath) => {
  try {
    const logDir = normalizeDirectoryPath(targetPath || getLogDir());
    fs.mkdirSync(logDir, { recursive: true });
    const openResult = await shell.openPath(logDir);
    if (openResult) {
      return { success: false, error: openResult, logDir };
    }
    return { success: true, logDir };
  } catch (err) {
    return { success: false, error: err.message };
  }
});

ipcMain.handle('log-diagnostic', async (_event, entry) => {
  writeDiagnosticLog(
    entry?.level || 'info',
    entry?.source || 'renderer',
    entry?.message || 'event',
    entry?.meta || null
  );
  return { success: true };
});

ipcMain.handle('get-memory-stats', async () => {
  try {
    const metrics = app.getAppMetrics();
    let totalKb = 0;
    let mainKb = 0;
    let rendererKb = 0;
    let gpuKb = 0;
    let utilityKb = 0;
    let rendererCount = 0;

    for (const metric of metrics) {
      const workingSetKb = metric?.memory?.workingSetSize || 0;
      totalKb += workingSetKb;
      if (metric.type === 'Browser') mainKb += workingSetKb;
      if (metric.type === 'Tab' || metric.type === 'Renderer') {
        rendererKb += workingSetKb;
        rendererCount++;
      }
      if (metric.type === 'GPU') gpuKb += workingSetKb;
      if (metric.type === 'Utility') utilityKb += workingSetKb;
    }

    return {
      success: true,
      sampleAt: new Date().toISOString(),
      totalMb: +(totalKb / 1024).toFixed(1),
      mainMb: +(mainKb / 1024).toFixed(1),
      rendererMb: +(rendererKb / 1024).toFixed(1),
      gpuMb: +(gpuKb / 1024).toFixed(1),
      utilityMb: +(utilityKb / 1024).toFixed(1),
      rendererCount
    };
  } catch (err) {
    return { success: false, error: err.message };
  }
});

// ─── Dosya/klasör seç (ayarlar için) ──────────────────────────────
ipcMain.handle('browse-file', async (_event, opts) => {
  const win = BrowserWindow.getFocusedWindow();
  const { canceled, filePaths } = await dialog.showOpenDialog(win, {
    title: opts.title || 'Dosya Seç',
    defaultPath: opts.defaultPath || '',
    properties: ['openFile'],
  });
  if (canceled || filePaths.length === 0) return { canceled: true };
  return { canceled: false, filePath: filePaths[0] };
});

ipcMain.handle('browse-directory', async (_event, opts) => {
  const win = BrowserWindow.getFocusedWindow();
  const { canceled, filePaths } = await dialog.showOpenDialog(win, {
    title: opts?.title || 'Klasör Seç',
    defaultPath: normalizeDirectoryPath(opts?.defaultPath),
    properties: ['openDirectory', 'createDirectory'],
  });
  if (canceled || filePaths.length === 0) return { canceled: true };
  return { canceled: false, filePath: filePaths[0] };
});

// ─── Şablon dosyası kaydet (.mlptemplate) ─────────────────────
ipcMain.handle('save-template', async (_event, data) => {
  const win = BrowserWindow.getFocusedWindow();
  const suggestedName = (data.name || 'sablon').replace(/[^a-z0-9_\-]/gi, '_');
  const { canceled, filePath } = await dialog.showSaveDialog(win, {
    title: 'Şablonu Kaydet',
    defaultPath: suggestedName + '.mlptemplate',
    filters: [
      { name: 'MLP Şablon', extensions: ['mlptemplate'] },
      { name: 'JSON', extensions: ['json'] }
    ]
  });
  if (canceled || !filePath) return { canceled: true };
  fs.writeFileSync(filePath, JSON.stringify(data, null, 2), 'utf8');
  return { canceled: false, filePath };
});

// ─── Şablon dosyası içe aktar ──────────────────────────────────
ipcMain.handle('import-template', async () => {
  const win = BrowserWindow.getFocusedWindow();
  const { canceled, filePaths } = await dialog.showOpenDialog(win, {
    title: 'Şablon İçe Aktar',
    filters: [{ name: 'MLP Şablon', extensions: ['mlptemplate', 'json'] }],
    properties: ['openFile']
  });
  if (canceled || filePaths.length === 0) return { canceled: true };
  try {
    const raw = fs.readFileSync(filePaths[0], 'utf8');
    return { canceled: false, data: JSON.parse(raw), filePath: filePaths[0] };
  } catch (e) {
    return { canceled: false, error: e.message };
  }
});

// ─── WASM çalıştır (gelecekte Node.js tarafına offload için stub) ─────────────
// Şu an renderer'da wasmBackend.compile() ile yapılıyor; bu handler yer tutucu.
ipcMain.handle('run-wasm', async (_event, { wasmBytes }) => {
  // TODO: gerekirse Node.js WebAssembly + WASI polyfill burada çalışacak
  return { ok: false, error: 'Renderer-side execution kullanın' };
});

// ─── Form Önizleme Penceresi ──────────────────────────────────────────────────
let _previewFormData = null;
let _previewWindow = null;

ipcMain.handle('open-form-preview', async (_event, formData) => {
  _previewFormData = formData;
  const cs = formData.canvasSettings || {};
  const w  = (cs.width  || 800);
  const h  = (cs.height || 600);

  if (_previewWindow && !_previewWindow.isDestroyed()) {
    writeDiagnosticLog('info', 'preview', 'reuse-preview-window', {
      title: cs.title || 'Önizleme',
      width: w,
      height: h
    });
    _previewWindow.setTitle(cs.title || 'Önizleme');
    if (typeof _previewWindow.setContentSize === 'function') {
      _previewWindow.setContentSize(w, h);
    } else {
      _previewWindow.setSize(w, h);
    }
    if (!_previewWindow.isVisible()) _previewWindow.show();
    if (!_previewWindow.webContents.isLoading()) {
      _previewWindow.webContents.reload();
    }
    _previewWindow.focus();
    return { ok: true, reused: true };
  }

  _previewWindow = new BrowserWindow({
    width:  w,
    height: h,
    useContentSize: true,   // w/h = içerik alanı (başlık çubuğu hariç)
    title:  cs.title || 'Önizleme',
    resizable: true,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      devTools: false,
      preload: path.join(__dirname, 'preload-preview.js')
    },
    icon: path.join(__dirname, 'assets/icon.png')
  });
  _previewWindow.on('closed', () => {
    writeDiagnosticLog('info', 'preview', 'preview-window-closed');
    _previewWindow = null;
    _previewFormData = null;
  });
  writeDiagnosticLog('info', 'preview', 'create-preview-window', {
    title: cs.title || 'Önizleme',
    width: w,
    height: h
  });
  _previewWindow.loadFile('preview-form.html');
  return { ok: true };
});

ipcMain.handle('get-preview-form-data', async () => _previewFormData);

function createWindow() {
  // İlk açılışta taskbar iş alanına göre konumlandır
  const wa = screen.getPrimaryDisplay().workArea;
  const initW = Math.min(1400, wa.width);
  const initH = Math.min(900,  wa.height);
  const initX = wa.x + Math.floor((wa.width  - initW) / 2);
  const initY = wa.y + Math.floor((wa.height - initH) / 2);

  const mainWindow = new BrowserWindow({
    width:  initW,
    height: initH,
    x: initX,
    y: initY,
    show: false,  // ready-to-show'da clamp sonrası göster
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      devTools: true,
      preload: path.join(__dirname, 'preload.js')
    },
    icon: path.join(__dirname, 'assets/icon.png')
  });

  mainWindow.loadFile('index.html');
  writeDiagnosticLog('info', 'app', 'create-main-window', {
    width: initW,
    height: initH,
    loggingEnabled: isLoggingEnabled()
  });

  // Ctrl+R / F5 → sayfayı kapat olmadan yenile (native menü kapalı olduğundan manuel eklendi)
  mainWindow.webContents.on('before-input-event', (event, input) => {
    if (input.type !== 'keyDown') return;
    const reload = (input.key === 'F5') ||
                   (input.key === 'r' && input.control && !input.shift) ||
                   (input.key === 'F5' && input.shift) /* hard reload */;
    const hardReload = input.key === 'F5' && input.shift;
    if (reload) {
      if (hardReload) {
        mainWindow.webContents.reloadIgnoringCache();
      } else {
        mainWindow.webContents.reload();
      }
      event.preventDefault();
    }
  });

  // Pencereyi OS görev çubuğu (taskbar) arkasına kaçmasın diye iş alanına kilitle
  function clampToWorkArea() {
    if (mainWindow.isDestroyed() || mainWindow.isMaximized() || mainWindow.isFullScreen()) return;
    const b  = mainWindow.getBounds();
    const wa = screen.getDisplayNearestPoint({ x: b.x + Math.floor(b.width / 2), y: b.y + Math.floor(b.height / 2) }).workArea;
    let { x, y, width, height } = b;
    if (x < wa.x) x = wa.x;
    if (y < wa.y) y = wa.y;
    if (x + width  > wa.x + wa.width)  width  = wa.x + wa.width  - x;
    if (y + height > wa.y + wa.height) height = wa.y + wa.height - y;
    if (x !== b.x || y !== b.y || width !== b.width || height !== b.height) {
      mainWindow.setBounds({ x, y, width, height });
    }
  }

  // İlk gösterimde hemen sınırla (taskbar arkasına kaçmayı önle)
  mainWindow.once('ready-to-show', () => {
    clampToWorkArea();
    mainWindow.show();
  });

  // Boyutlandırma / taşıma sırasında da sınırla (debounce ile)
  let clampTimer;
  function clampDebounced() {
    clearTimeout(clampTimer);
    clampTimer = setTimeout(clampToWorkArea, 80);
  }
  mainWindow.on('resize', clampDebounced);
  mainWindow.on('move',   clampDebounced);

  // Kapatma öncesi kaydedilmemiş değişiklik kontrolü
  let _allowClose = false;
  mainWindow.on('close', (event) => {
    if (_allowClose) return; // izin verilmişse doğrudan kapat
    event.preventDefault();
    mainWindow.webContents.send('before-close-check');
  });

  ipcMain.once('close-response', (event, { dirty, projectName }) => {
    if (!dirty) { _allowClose = true; mainWindow.close(); return; }
    const choice = dialog.showMessageBoxSync(mainWindow, {
      type: 'question',
      buttons: ['Kaydet', 'Kaydetmeden Çık', 'İptal'],
      defaultId: 0,
      cancelId: 2,
      title: 'Kaydedilmemiş Değişiklikler',
      message: `"${projectName || 'Proje'}" henüz kaydedilmedi.`,
      detail: 'Değişikliklerinizi kaydetmek istiyor musunuz?'
    });
    if (choice === 0) {
      // Kaydet — renderer’ın kaydetmesini iste, sonra kapat
      mainWindow.webContents.send('save-then-close');
      ipcMain.once('save-then-close-done', () => { _allowClose = true; mainWindow.close(); });
    } else if (choice === 1) {
      _allowClose = true; mainWindow.close();
    }
    // choice === 2 → İptal: hiçbir şey yapma
  });

  // DevTools'u otomatik aç
  // mainWindow.webContents.openDevTools({ mode: 'detach' });
}

app.whenReady().then(() => {
  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow();
    }
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

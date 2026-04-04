// Ana Uygulama

// Global designer instance
let designer;
// Kaydedilmemiş değişiklik takibi
let _isDirty = false;
let _currentProjectName = 'Proje';

// Pencere başlığını güncelle: ● + proje adı
function updateWindowTitle() {
  const prefix = _isDirty ? '\u25CF ' : '';
  document.title = `${prefix}${_currentProjectName} — MELP`;
}
// Editörün kendi klasör yolu (IPC'den alınır)
let _melpAppPath = '';
// Şu an çalıştırılan .mlp dosyasının tam yolu (kaydedilmişse gerçek yol, değilse /tmp)
let _currentMlpFile = '/tmp/_melp_ide_run.mlp';
// Ayarlar (settings.json'dan yüklenir)
let _settings = {
  compilerPath: '',
  normalizerPath: '',
  lliPath: '/usr/bin/lli',
  outputLlFile: 'output.ll',
  tmpFile: '/tmp/_melp_ide_run.mlp',
  autosaveEnabled: false,
  autosaveInterval: 60,
  openLastProject: false,
  enableDiagnosticLogging: false
};
// Otomatik kaydet timer
let _autosaveTimer = null;
let _memoryLogTimer = null;
const MEMORY_LOG_INTERVAL_MS = 5000;
const MEMORY_LOG_MAX_SAMPLES = 24;
const _memoryLogSamples = [];
let _memoryLogLastError = '';

document.addEventListener('DOMContentLoaded', async () => {
  designer = new Designer();

  // Widget değişince: KAYNAK güncelle + KOD'a eksik event stub'ları ekle
  designer.onWidgetChange = () => {
    _isDirty = true;
    updateWindowTitle();
    updateSourceView();
    appendMissingEventStubs();
    // Canlı önizleme — Görünüm sekmesi açıksa anında, değilse debounced
    if (_previewHotReload) {
      const viewPreview = document.getElementById('view-preview');
      if (viewPreview && !viewPreview.classList.contains('hidden')) {
        updatePreviewView();
      } else {
        clearTimeout(_previewHotReloadTimer);
        _previewHotReloadTimer = setTimeout(() => {
          const vp = document.getElementById('view-preview');
          if (vp && !vp.classList.contains('hidden')) updatePreviewView();
        }, PREVIEW_HOTRELOAD_DEBOUNCE);
      }
    }
    const tabCode = document.getElementById('tab-code');
    if (tabCode?.classList.contains('active')) {
      requestAnimationFrame(() => {
        const lastWidget = designer.widgets[designer.widgets.length - 1];
        if (!lastWidget) return;
        const handlerName = lastWidget.properties.onClick
          || lastWidget.properties.onChange
          || lastWidget.properties.onSelect;
        if (handlerName) highlightFunction(handlerName);
      });
    }
  };

  // App yolunu IPC'den al (Electron ortamı) — compilerPath ve normalizerPath dinamik olarak set edilir
  try {
    const sysInfo = await window.electronAPI.getSystemInfo();
    _melpAppPath = sysInfo.appPath;
    _settings.compilerPath = _melpAppPath + '/bin/melp_compiler';
    _settings.normalizerPath = _melpAppPath + '/../../ORTAK/normalizer/melp_normalizer.sh';
  } catch (e) {
    console.warn('getSystemInfo başarısız, varsayılan yollar kullanılacak:', e);
  }

  // settings.json yükle
  try {
    const settingsResp = await fetch('./settings.json');
    if (settingsResp.ok) {
      const loaded = await settingsResp.json();
      _settings = { ..._settings, ...loaded };
      _currentMlpFile = _settings.tmpFile;
    }
  } catch (e) {
    console.warn('settings.json yüklenemedi, varsayılanlar kullanılıyor:', e);
  }

  // Config dosyalarını yükle
  showStatus('Dil ve syntax yapılandırmaları yükleniyor...');
  const configLoaded = await configLoader.loadConfigs();

  if (configLoaded) {
    populateLanguageSelector();
    populateSyntaxSelector();

    // MLP keyword'lerini IntelliSense'e yükle (setTimeout ile gecikme)
    setTimeout(() => {
      try {
        if (typeof intelliSense !== 'undefined' && intelliSense.loadKeywords) {
          const mlpKeywords = [
            'numeric', 'string', 'boolean', 'pointer',
            'function', 'end', 'return',
            'if', 'else', 'then', 'elif',
            'while', 'for', 'in', 'break', 'continue', 'stop',
            'struct', 'enum', 'const',
            'lambda', 'map', 'list', 'optional',
            'import', 'export', 'module',
            'true', 'false', 'null',
            'and', 'or', 'not',
            'print', 'typeof', 'range'
          ];
          intelliSense.loadKeywords(mlpKeywords);
          console.log('IntelliSense keywords loaded:', mlpKeywords.length);
        } else {
          console.error('IntelliSense object not available!');
        }
      } catch (error) {
        console.error('Error loading IntelliSense keywords:', error);
      }
    }, 500);
  }

  initializeUI();
  initThemePicker();
  loadSavedTheme();
  renderRecentMenu();
  updateWindowTitle();

  // Escape tuşu: tüm açık overlay'leri kapat
  document.addEventListener('keydown', (e) => {
    if (e.key !== 'Escape') return;
    ['theme-picker-overlay', 'settings-overlay', 'templates-overlay', 'snippet-manager-overlay']
      .forEach(id => {
        const el = document.getElementById(id);
        if (el && !el.classList.contains('hidden')) el.classList.add('hidden');
      });
  });

  // Ayarlar dialogunu hazırla
  initSettingsDialog();
  // Şablon dialogunu hazırla
  initTemplatesDialog();
  // Özel widget dialoglarını hazırla
  initCustomWidgetDialog();
  initCustomWidgetManagerDialog();
  renderCustomWidgetsToolbox();
  // "+" kısayol butonu toolbox'ta
  const btnAddCw = document.getElementById('btn-add-custom-widget');
  if (btnAddCw) btnAddCw.addEventListener('click', () => handleCreateCustomWidget());
  // Önizleme araç çubuğu
  initPreviewToolbar();
  // Debug paneli
  initDebugPanel();
  // Animasyon paneli
  initAnimationPanel();
  // Widget seçim değişikliği → animasyon panelini güncelle
  designer.onSelect = (w) => showAnimationFor(w);

  // Otomatik kaydeti başlat
  restartAutosave();
  updateAutosaveMenu();

  // UI dilini başlat (asenkron, UI renderını engellemez)
  if (window.i18n) window.i18n.initI18n().catch(() => {});

  // Özellik arama filtresi
  const propsSearchEl = document.getElementById('props-search');
  if (propsSearchEl) {
    propsSearchEl.addEventListener('input', () => filterProperties(propsSearchEl.value));
  }

  // Snap başlangıç durumunu göster
  const snapItem = document.getElementById('menu-snap-grid');
  if (snapItem) snapItem.style.fontWeight = 'bold';

  showStatus('MLP GUI Designer hazır');

  // Son projeyi aç
  if (_settings.openLastProject) openLastProjectOnStartup();

  // Kapat öncesi kaydedilmemiş değişiklik sorgusu
  if (window.electronAPI?.onBeforeClose) {
    window.electronAPI.onBeforeClose(() => {
      window.electronAPI.respondClose(_isDirty, _currentProjectName);
    });
  }

  // Kaydet-sonra-kapat (main process'ten istek)
  if (window.electronAPI?.onSaveThenClose) {
    window.electronAPI.onSaveThenClose(async () => {
      await handleSaveProject();
      window.electronAPI.saveThenCloseDone();
    });
  }

  // Kod editörü değişince de dirty olarak işaretle
  const codeEditorEl = document.getElementById('code-editor');
  if (codeEditorEl) {
    codeEditorEl.addEventListener('input', () => { _isDirty = true; updateWindowTitle(); });
  }

  refreshMemoryLogState();

  if (window.electronAPI?.logDiagnostic) {
    window.addEventListener('error', (event) => {
      window.electronAPI.logDiagnostic({
        level: 'error',
        source: 'renderer',
        message: 'window-error',
        meta: {
          message: event.message,
          filename: event.filename,
          lineno: event.lineno,
          colno: event.colno
        }
      }).catch(() => {});
    });

    window.addEventListener('unhandledrejection', (event) => {
      window.electronAPI.logDiagnostic({
        level: 'error',
        source: 'renderer',
        message: 'unhandled-rejection',
        meta: {
          reason: String(event.reason?.stack || event.reason || 'unknown')
        }
      }).catch(() => {});
    });
  }
});

function logDiagnostic(source, message, meta = null, level = 'info') {
  if (!_settings.enableDiagnosticLogging || !window.electronAPI?.logDiagnostic) return;
  window.electronAPI.logDiagnostic({ level, source, message, meta }).catch(() => {});
}

function formatSampleTime(isoDate) {
  try {
    const date = new Date(isoDate);
    return date.toLocaleTimeString('tr-TR', { hour12: false });
  } catch (_err) {
    return '—';
  }
}

function updateMemorySummary(sample) {
  document.getElementById('log-total-mb').textContent = sample ? `${sample.totalMb} MB` : '—';
  document.getElementById('log-main-mb').textContent = sample ? `${sample.mainMb} MB` : '—';
  document.getElementById('log-renderer-mb').textContent = sample ? `${sample.rendererMb} MB` : '—';
  document.getElementById('log-gpu-mb').textContent = sample ? `${sample.gpuMb} MB` : '—';
  document.getElementById('log-renderer-count').textContent = sample ? String(sample.rendererCount) : '—';
}

function renderMemoryLogView() {
  const listEl = document.getElementById('log-list');
  const emptyEl = document.getElementById('log-empty-state');
  if (!listEl || !emptyEl) return;

  if (!_settings.enableDiagnosticLogging) {
    emptyEl.textContent = 'Tanılama loglamasını ayarlardan açın. Açıkken burada son 120 saniyelik bellek örnekleri görünür.';
    emptyEl.classList.remove('hidden');
    listEl.classList.add('hidden');
    listEl.innerHTML = '';
    updateMemorySummary(null);
    return;
  }

  if (_memoryLogSamples.length === 0) {
    emptyEl.textContent = _memoryLogLastError || 'İlk bellek örneği bekleniyor...';
    emptyEl.classList.remove('hidden');
    listEl.classList.add('hidden');
    updateMemorySummary(null);
    return;
  }

  emptyEl.classList.add('hidden');
  listEl.classList.remove('hidden');
  const latest = _memoryLogSamples[_memoryLogSamples.length - 1];
  updateMemorySummary(latest);

  listEl.innerHTML = _memoryLogSamples.slice().reverse().map((sample) => {
    const totalDelta = (sample.totalMb - latest.totalMb).toFixed(1);
    const deltaLabel = totalDelta === '0.0' ? 'Şimdi' : `${totalDelta > 0 ? '+' : ''}${totalDelta} MB`;
    const sourceLabel = sample.source === 'renderer-fallback' ? 'Renderer' : 'Electron';
    return `
      <div class="log-entry">
        <span class="log-entry-time">${formatSampleTime(sample.sampleAt)}</span>
        <span class="log-entry-total">Toplam ${sample.totalMb} MB</span>
        <span class="log-entry-muted">Main ${sample.mainMb} MB</span>
        <span class="log-entry-muted">Renderer ${sample.rendererMb} MB</span>
        <span class="log-entry-muted">GPU ${sample.gpuMb} MB</span>
        <span class="log-entry-badge">${sourceLabel} · ${deltaLabel}</span>
      </div>
    `;
  }).join('');
}

function getRendererFallbackMemoryStats() {
  const perfMem = window.performance?.memory;
  if (!perfMem || typeof perfMem.usedJSHeapSize !== 'number') {
    return null;
  }

  const rendererMb = +(perfMem.usedJSHeapSize / (1024 * 1024)).toFixed(1);
  return {
    success: true,
    source: 'renderer-fallback',
    sampleAt: new Date().toISOString(),
    totalMb: rendererMb,
    mainMb: 0,
    rendererMb,
    gpuMb: 0,
    utilityMb: 0,
    rendererCount: 1
  };
}

async function sampleMemoryUsage() {
  if (!_settings.enableDiagnosticLogging) return;

  try {
    let result = null;

    if (window.electronAPI?.getMemoryStats) {
      result = await window.electronAPI.getMemoryStats();
      if (!result?.success) {
        _memoryLogLastError = `Bellek örneği alınamadı: ${result?.error || 'Bilinmeyen hata'}`;
        showStatus(_memoryLogLastError, 5000);
        logDiagnostic('memory-log', 'memory-sample-failed', { error: result?.error || 'unknown' }, 'error');
        renderMemoryLogView();
        return;
      }
      result.source = 'electron';
    } else {
      result = getRendererFallbackMemoryStats();
      if (!result) {
        _memoryLogLastError = 'Bellek API bulunamadı. MELP penceresini yeniden başlatın.';
        showStatus(_memoryLogLastError, 5000);
        logDiagnostic('memory-log', 'memory-api-missing', null, 'error');
        renderMemoryLogView();
        return;
      }
    }

    _memoryLogLastError = '';
    _memoryLogSamples.push(result);
    if (_memoryLogSamples.length > MEMORY_LOG_MAX_SAMPLES) {
      _memoryLogSamples.splice(0, _memoryLogSamples.length - MEMORY_LOG_MAX_SAMPLES);
    }
    renderMemoryLogView();
  } catch (err) {
    _memoryLogLastError = `Bellek polling hatası: ${err.message}`;
    showStatus(_memoryLogLastError, 5000);
    logDiagnostic('memory-log', 'memory-polling-exception', { error: err.message }, 'error');
    renderMemoryLogView();
  }
}

function stopMemoryLogTimer() {
  clearInterval(_memoryLogTimer);
  _memoryLogTimer = null;
}

function refreshMemoryLogState() {
  stopMemoryLogTimer();
  if (!_settings.enableDiagnosticLogging) {
    _memoryLogSamples.length = 0;
    _memoryLogLastError = '';
    renderMemoryLogView();
    return;
  }
  renderMemoryLogView();
  sampleMemoryUsage();
  _memoryLogTimer = setInterval(sampleMemoryUsage, MEMORY_LOG_INTERVAL_MS);
}

// menus: bkz. src/ui/menus.js

// Dil seçiciyi doldur
function populateLanguageSelector() {
  const selector = document.getElementById('language-selector');
  const languages = configLoader.getLanguages();

  // Clear existing options
  selector.innerHTML = '';

  languages.forEach(lang => {
    const option = document.createElement('option');
    option.value = lang.id;
    option.textContent = lang.name;
    if (lang.id === configLoader.currentLanguage) {
      option.selected = true;
    }
    selector.appendChild(option);
  });
}

// Syntax seçiciyi doldur
function populateSyntaxSelector() {
  const selector = document.getElementById('syntax-selector');
  const syntaxes = configLoader.getSyntaxes();

  // Clear existing options
  selector.innerHTML = '';

  Object.values(syntaxes).forEach(syntax => {
    const option = document.createElement('option');
    option.value = syntax.id;
    option.textContent = syntax.name;
    if (syntax.id === configLoader.currentSyntax) {
      option.selected = true;
    }
    selector.appendChild(option);
  });
}

// Dil değişti
function handleLanguageChange(e) {
  const langId = e.target.value;
  configLoader.setLanguage(langId);
  showStatus(`Dil değiştirildi: ${configLoader.getCurrentLanguage().name}`);

  // Kod görünümü açıksa güncelle
  if (!document.getElementById('view-code').classList.contains('hidden')) {
    updateEventCode();
  }
  // Tasarım kaynağını her zaman güncelle
  updateDesignSource();
}

// Syntax değişti
function handleSyntaxChange(e) {
  const syntaxId = e.target.value;
  configLoader.setSyntax(syntaxId);
  showStatus(`Syntax değiştirildi: ${configLoader.getCurrentSyntax().name}`);

  if (!document.getElementById('view-code').classList.contains('hidden')) {
    syncEventCode();
  }
  updateSourceView();
}

// KAYNAK sekmesi: app_start bloğu — terminal görünümlü salt okunur
function updateSourceView() {
  const el = document.getElementById('source-editor');
  if (!el) return;
  el.textContent = designer.generateDesignSource();
}

// Tasarım kaynağını güncelle (eski referanslar için)
function updateDesignSource() { updateSourceView(); }

// ── GÖRÜNÜM sekmesi — Gerçek Zamanlı Önizleme ────────────────────────────────
// Önizleme boyutu { w, h } veya 'canvas' (canvasSettings'ten)
let _previewSize = { w: 800, h: 600 };
let _previewHotReload = true;
let _previewHotReloadTimer = null;
const PREVIEW_HOTRELOAD_DEBOUNCE = 400;

// Boyutu uygula + aktif butonu işaretle
function _applyPreviewSize(preset) {
  const cs = designer.canvasSettings;
  if (preset === 'canvas') {
    _previewSize = { w: cs?.width || 800, h: cs?.height || 600 };
  } else {
    _previewSize = { w: preset.w, h: preset.h };
  }
  const infoEl = document.getElementById('preview-size-info');
  if (infoEl) infoEl.textContent = `${_previewSize.w}×${_previewSize.h}`;

  // Preview pencere boyutunu güncelle
  const win = document.getElementById('preview-window');
  if (win) {
    win.style.width = _previewSize.w + 'px';
  }
  updatePreviewView();
}

// Önizlemeyi yenile (canvas klonu)
function updatePreviewView() {
  const previewBody = document.getElementById('preview-window-body');
  if (!previewBody) return;

  const widgets = designer.widgets;

  if (!widgets || widgets.length === 0) {
    previewBody.innerHTML = '<div class="preview-empty-msg">Tasarım sekmesine widget ekleyin</div>';
    return;
  }

  const cs = designer.canvasSettings;
  const canvasW = _previewSize.w || cs?.width  || 800;
  const canvasH = _previewSize.h || cs?.height || 600;
  const canvasBg = cs?.backgroundColor || 'var(--bg-secondary)';

  const canvas = document.getElementById('canvas');
  const clone  = canvas.cloneNode(true);
  clone.removeAttribute('id');
  clone.className = 'preview-canvas';
  clone.style.width     = canvasW + 'px';
  clone.style.height    = canvasH + 'px';
  clone.style.minHeight = canvasH + 'px';
  clone.style.backgroundImage = 'none';
  clone.style.backgroundColor = canvasBg;
  clone.style.outline = 'none';

  clone.querySelectorAll('.resize-handle').forEach(h => h.remove());
  clone.querySelectorAll('.canvas-info').forEach(h => h.remove());
  clone.querySelectorAll('.rubber-band').forEach(h => h.remove());
  clone.querySelectorAll('.canvas-widget').forEach(w => {
    w.classList.remove('selected');
    w.style.cursor    = 'default';
    w.style.outline   = 'none';
    w.style.boxShadow = '';

    // Responsive % konumlama: widget _usePercent ise % ile yerleştir
    const wId = w.id;
    if (wId) {
      const widgetData = designer.widgets.find(wd => wd.id === wId);
      if (widgetData && widgetData.properties._usePercent) {
        const p = widgetData.properties;
        const xP = p._xPct != null ? p._xPct : (p.x / canvasW) * 100;
        const yP = p._yPct != null ? p._yPct : (p.y / canvasH) * 100;
        const wP = p._wPct != null ? p._wPct : (p.width  / canvasW) * 100;
        const hP = p._hPct != null ? p._hPct : (p.height / canvasH) * 100;
        w.style.left   = xP + '%';
        w.style.top    = yP + '%';
        w.style.width  = wP + '%';
        w.style.height = hP + '%';
      }
    }
  });

  // Textbox widget'larını gerçek <input> elementine dönüştür (etkileşimli önizleme)
  clone.querySelectorAll('.canvas-widget.widget-textbox').forEach(w => {
    const wId = w.id;
    const widgetData = designer.widgets.find(wd => wd.id === wId);
    const props = widgetData?.properties || {};
    const content = w.querySelector('.widget-content');
    if (content) content.remove();
    const inp = document.createElement('input');
    inp.type = 'text';
    inp.value = props.text || '';
    inp.placeholder = props.placeholder || '';
    inp.style.cssText = 'width:100%;height:100%;box-sizing:border-box;background:transparent;border:none;outline:none;color:inherit;font-size:inherit;font-family:inherit;padding:2px 6px;';
    w.style.pointerEvents = 'auto';
    w.style.cursor = 'text';
    w.appendChild(inp);
  });

  const titleEl = document.getElementById('preview-title-text');
  if (titleEl && cs?.title) titleEl.textContent = cs.title;

  previewBody.innerHTML = '';
  previewBody.appendChild(clone);

  // Widget animasyonlarını önizleme klonuna uygula
  if (typeof applyAnimationsToPreview === 'function') applyAnimationsToPreview(clone);
}

// Preview toolbar butonlarını başlat (DOMContentLoaded içinde çağrılır)
function initPreviewToolbar() {
  document.querySelectorAll('.preview-preset-btn').forEach(btn => {
    btn.addEventListener('click', () => {
      document.querySelectorAll('.preview-preset-btn').forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
      const w = btn.dataset.w;
      const h = btn.dataset.h;
      _applyPreviewSize(w === 'canvas' ? 'canvas' : { w: parseInt(w), h: parseInt(h) });
    });
  });

  const refreshBtn = document.getElementById('preview-refresh-btn');
  if (refreshBtn) refreshBtn.addEventListener('click', updatePreviewView);

  const hotReloadToggle = document.getElementById('preview-hotreload-toggle');
  if (hotReloadToggle) {
    hotReloadToggle.checked = _previewHotReload;
    hotReloadToggle.addEventListener('change', () => {
      _previewHotReload = hotReloadToggle.checked;
    });
  }
}
// editor: bkz. src/ui/editor.js

// Klavye kısayolları
document.addEventListener('keydown', (e) => {
  // Ctrl+S: Kaydet
  if (e.ctrlKey && e.key === 's') {
    e.preventDefault();
    handleSaveProject();
  }

  // Ctrl+O: Aç
  if (e.ctrlKey && e.key === 'o') {
    e.preventDefault();
    handleOpenProject();
  }

  // Ctrl+N: Yeni
  if (e.ctrlKey && e.key === 'n') {
    e.preventDefault();
    handleNewProject();
  }

  // Ctrl+E: Dışa aktar
  if (e.ctrlKey && e.key === 'e') {
    e.preventDefault();
    handleExportCode();
  }

  // F5: Çalıştır
  if (e.key === 'F5') {
    e.preventDefault();
    handleRun();
  }

  // Ctrl+`: Terminal göster/gizle (isInputFocused kontrolünden önce — her zaman çalışsın)
  if (e.ctrlKey && e.key === '`') {
    e.preventDefault();
    toggleBottomTerminal();
    return;
  }

  // Kod editörüne focus varsa Ctrl+Z/Y/X/C/V'yi tarayıcıya bırak
  const codeEditorEl = document.getElementById('code-editor');
  const isCodeFocused = document.activeElement === codeEditorEl;

  // Input/textarea/select odaklanmışsa tüm tuşları yerel davranışa bırak
  const activeTag = document.activeElement?.tagName?.toUpperCase();
  const isInputFocused = activeTag === 'INPUT' || activeTag === 'TEXTAREA' || activeTag === 'SELECT';
  if (isInputFocused) return;

  // Ctrl+Z: Geri al
  if (e.ctrlKey && e.key === 'z') {
    if (isCodeFocused) return; // kod editörünün kendi undo'su çalışsın
    e.preventDefault();
    handleUndo();
  }

  // Ctrl+Y: Yinele
  if (e.ctrlKey && e.key === 'y') {
    if (isCodeFocused) return;
    e.preventDefault();
    handleRedo();
  }

  // Ctrl+X: Kes
  if (e.ctrlKey && e.key === 'x') {
    if (isCodeFocused) return; // kod editörünün kendi cut'ı çalışsın
    e.preventDefault();
    handleCut();
  }

  // Ctrl+C: Kopyala
  if (e.ctrlKey && e.key === 'c') {
    if (isCodeFocused) return; // kod editörünün kendi copy'si çalışsın
    e.preventDefault();
    handleCopy();
  }

  // Ctrl+V: Yapıştır
  if (e.ctrlKey && e.key === 'v') {
    if (isCodeFocused) return; // kod editörünün kendi paste'i çalışsın
    e.preventDefault();
    handlePaste();
  }

  // Ctrl+G: Gruplandır
  if (e.ctrlKey && !e.shiftKey && e.key === 'g') {
    if (isCodeFocused) return;
    e.preventDefault();
    handleGroup();
    return;
  }

  // Ctrl+Shift+G: Grubu çöz
  if (e.ctrlKey && e.shiftKey && e.key === 'G') {
    if (isCodeFocused) return;
    e.preventDefault();
    handleUngroup();
    return;
  }

  // Ctrl+A: Tümünü seç
  if (e.ctrlKey && e.key === 'a') {
    if (isCodeFocused) return;
    e.preventDefault();
    handleSelectAll();
  }

  // F9: Breakpoint toggle (satır numarasına göre — aktif satır imleci yoksa en sona)
  if (e.key === 'F9' && !e.ctrlKey && !e.shiftKey) {
    e.preventDefault();
    if (typeof toggleBreakpoint === 'function') {
      const codeEl = document.getElementById('code-editor');
      const sel = window.getSelection();
      if (sel && sel.rangeCount > 0 && codeEl && codeEl.contains(sel.anchorNode)) {
        const range = sel.getRangeAt(0);
        const preText = codeEl.textContent.substring(0, range.startOffset);
        const lineNum = (preText.match(/\n/g) || []).length + 1;
        toggleBreakpoint(lineNum);
      }
    }
  }

  // F5: Continue / Debug başlat
  if (e.key === 'F5' && !e.ctrlKey) {
    e.preventDefault();
    if (e.shiftKey) {
      if (typeof debugStop === 'function') debugStop();
    } else {
      if (typeof debugContinue === 'function') debugContinue();
    }
  }

  // F10: Step over
  if (e.key === 'F10' && !e.ctrlKey && !e.shiftKey) {
    e.preventDefault();
    if (typeof debugStepOver === 'function') debugStepOver();
  }

  // F11: Step into
  if (e.key === 'F11' && !e.ctrlKey && !e.shiftKey) {
    e.preventDefault();
    if (typeof debugStepInto === 'function') debugStepInto();
  }
});

// Canvas widget sağ tık menüsü
(function () {
  const canvasMenu = document.getElementById('canvas-context-menu');

  // Properties panel input'ları için de aynı menüyü tekrar kullan
  // (Electron'da native input context menüsü çalışmıyor)
  const propsMenu = document.getElementById('code-context-menu');

  // ── input / textarea sağ tık ──────────────────────────────────────────
  document.addEventListener('contextmenu', (e) => {
    const tag = e.target.tagName?.toUpperCase();
    if (tag !== 'INPUT' && tag !== 'TEXTAREA') return;
    // Renk ve checkbox input'larında native davranışı koru
    const inputType = e.target.type?.toLowerCase();
    if (inputType === 'color' || inputType === 'checkbox' || inputType === 'radio') return;
    // canvas-widget içindeyse bu handler'ı atla (aşağıda yakalanır)
    if (e.target.closest('.canvas-widget')) return;
    // kod editörü içindeyse de atla (kendi handler'ı var)
    const codeEditorEl = document.getElementById('code-editor');
    if (codeEditorEl && codeEditorEl.contains(e.target)) return;

    e.preventDefault();
    e.stopPropagation();
    window._ctxInputTarget = e.target;

    const x = Math.min(e.clientX, window.innerWidth  - 150);
    const y = Math.min(e.clientY, window.innerHeight - 120);
    propsMenu.style.left = x + 'px';
    propsMenu.style.top  = y + 'px';
    propsMenu.style.display = 'block';
  });

  // propsMenu tıklama — input üzerinde execCommand çalışır
  propsMenu.addEventListener('mousedown', (e) => {
    // Sadece input context'i için (kod editörü kendi handler'ından yönetir)
    if (!window._ctxInputTarget) return;
    const action = e.target.closest('.ctx-item')?.dataset.action;
    if (!action) return;
    e.preventDefault();
    propsMenu.style.display = 'none';
    const inp = window._ctxInputTarget;
    window._ctxInputTarget = null;
    e.stopImmediatePropagation(); // kod editörü handler'ının focus çalmasını engelle
    inp.focus();
    if      (action === 'cut')       document.execCommand('cut');
    else if (action === 'copy')      document.execCommand('copy');
    else if (action === 'paste') {
      navigator.clipboard.readText()
        .then(text => document.execCommand('insertText', false, text))
        .catch(() => {});
    }
    else if (action === 'selectall') inp.select();
  }, true);

  // propsMenu kapanınca _ctxInputTarget'ı temizle
  window.addEventListener('mousedown', (e) => {
    if (propsMenu.style.display !== 'none' && !propsMenu.contains(e.target)) {
      propsMenu.style.display = 'none';
      window._ctxInputTarget = null;
    }
  });

  let _ctxWidget = null;   // sağ tıklanan widget (null = boş alan)
  let _ctxPastePos = null; // yapıştırma konumu (canvas koordinatı)

  // Hover efekti
  canvasMenu.querySelectorAll('.cctx-item').forEach(item => {
    item.addEventListener('mouseenter', () => { item.style.background = '#3e3e3e'; });
    item.addEventListener('mouseleave', () => { item.style.background = ''; });
  });

  const _itemCut   = canvasMenu.querySelector('[data-action="cut"]');
  const _itemCopy  = canvasMenu.querySelector('[data-action="copy"]');
  const _itemPaste = canvasMenu.querySelector('[data-action="paste"]');

  function hideCanvasMenu() { canvasMenu.style.display = 'none'; }

  function showCanvasMenu(clientX, clientY) {
    const onWidget = !!_ctxWidget;
    const hasClip  = !!window._canvasClipboard;

    // Kes / Kopyala: sadece widget seçiliyse
    _itemCut.style.display  = onWidget ? '' : 'none';
    _itemCopy.style.display = onWidget ? '' : 'none';
    // Yapıştır: pano doluysa her zaman
    _itemPaste.style.display = hasClip ? '' : 'none';

    if (!onWidget && !hasClip) return; // gösterecek hiçbir şey yok

    const x = Math.min(clientX, window.innerWidth  - 150);
    const y = Math.min(clientY, window.innerHeight - 120);
    canvasMenu.style.left = x + 'px';
    canvasMenu.style.top  = y + 'px';
    canvasMenu.style.display = 'block';
  }

  // Canvas veya widget üzerinde sağ tık
  document.addEventListener('contextmenu', (e) => {
    // Canvas içinde mi?
    const canvasEl = document.getElementById('canvas');
    if (!canvasEl) return;
    const insideCanvas = e.target === canvasEl || canvasEl.contains(e.target);
    if (!insideCanvas) return;

    e.preventDefault();
    e.stopPropagation();

    // Canvas koordinatını hesapla (yapıştırma için)
    const rect = canvasEl.getBoundingClientRect();
    _ctxPastePos = {
      x: Math.round(e.clientX - rect.left),
      y: Math.round(e.clientY - rect.top)
    };

    // Widget üzerinde mi?
    const widgetEl = e.target.closest('.canvas-widget');
    if (widgetEl) {
      const widgetId = widgetEl.dataset.widgetId;
      _ctxWidget = designer.widgets.find(w => w.id === widgetId) || null;
    } else {
      _ctxWidget = null; // boş alan
    }

    showCanvasMenu(e.clientX, e.clientY);
  });

  // Menü eylem tıklamaları
  canvasMenu.addEventListener('mousedown', (e) => {
    const action = e.target.closest('.cctx-item')?.dataset.action;
    if (!action) return;
    e.preventDefault();
    hideCanvasMenu();

    if (action === 'cut' || action === 'copy') {
      if (!_ctxWidget) return;
      window._canvasClipboard = {
        type: _ctxWidget.type,
        properties: JSON.parse(JSON.stringify(_ctxWidget.properties))
      };
      if (action === 'cut') {
        designer.deleteWidget(_ctxWidget);
        _ctxWidget = null;
      }

    } else if (action === 'paste') {
      if (!window._canvasClipboard) return;
      const cb = window._canvasClipboard;
      // Sağ tıklanan konuma yapıştır; konum yoksa orijinalden +20 kaydır
      const pasteX = _ctxPastePos ? _ctxPastePos.x : (cb.properties.x || 0) + 20;
      const pasteY = _ctxPastePos ? _ctxPastePos.y : (cb.properties.y || 0) + 20;
      const newProps = {
        ...cb.properties,
        x: pasteX,
        y: pasteY
      };
      const newWidget = new Widget(cb.type, newProps);
      ['onClick', 'onChange', 'onSelect'].forEach(p => {
        if (p in newWidget.properties) newWidget.properties[p] = '';
      });
      designer.addWidget(newWidget);
    }
  });

  // Dışarı tıklayınca kapat
  window.addEventListener('mousedown', (e) => {
    if (!canvasMenu.contains(e.target)) hideCanvasMenu();
  });
  window.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') hideCanvasMenu();
  }, true);
}());

// Kod editöründe sağ tık — özel HTML context menüsü göster
(function () {
  const ctxMenu = document.getElementById('code-context-menu');

  // Hover efekti
  ctxMenu.querySelectorAll('.ctx-item').forEach(item => {
    item.addEventListener('mouseenter', () => { item.style.background = '#3e3e3e'; });
    item.addEventListener('mouseleave', () => { item.style.background = ''; });
  });

  function hideCtxMenu() { ctxMenu.style.display = 'none'; }

  window.addEventListener('contextmenu', (e) => {
    const codeEditorEl = document.getElementById('code-editor');
    if (!codeEditorEl) return;
    const insideEditor = e.target === codeEditorEl || codeEditorEl.contains(e.target);
    if (!insideEditor) { hideCtxMenu(); return; }

    e.preventDefault();
    // Menüyü ekranın dışına taşıma
    const x = Math.min(e.clientX, window.innerWidth  - 160);
    const y = Math.min(e.clientY, window.innerHeight - 140);
    ctxMenu.style.left = x + 'px';
    ctxMenu.style.top  = y + 'px';
    ctxMenu.style.display = 'block';
  });

  // Menü tıklamaları
  ctxMenu.addEventListener('mousedown', (e) => {
    const action = e.target.closest('.ctx-item')?.dataset.action;
    if (!action) return;
    e.preventDefault();
    hideCtxMenu();
    const codeEditorEl = document.getElementById('code-editor');
    codeEditorEl.focus();
    if (action === 'cut')       document.execCommand('cut');
    else if (action === 'copy') document.execCommand('copy');
    else if (action === 'paste') {
      navigator.clipboard.readText()
        .then(text => {
          // Electron'da execCommand('insertText') içerik düzenleyicilerde güvenilir çalışır
          const success = document.execCommand('insertText', false, text);
          if (!success) {
            // Fallback: Selection API ile elle ekle
            const sel = window.getSelection();
            if (sel.rangeCount > 0) {
              const range = sel.getRangeAt(0);
              range.deleteContents();
              range.insertNode(document.createTextNode(text));
              range.collapse(false);
              sel.removeAllRanges();
              sel.addRange(range);
              codeEditorEl.dispatchEvent(new Event('input', { bubbles: true }));
            }
          }
        })
        .catch(() => { showStatus('Pano erişim izni verilmedi'); });
    }
    else if (action === 'selectall') document.execCommand('selectAll');
  });

  // Menüyü kapat: dışarı tıkla veya Escape
  window.addEventListener('mousedown', (e) => {
    if (!ctxMenu.contains(e.target)) hideCtxMenu();
  });
  window.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') hideCtxMenu();
  }, true);
}());

// Menü dropdown'ları
document.querySelectorAll('.menu-item').forEach(item => {
  let timeout;

  item.addEventListener('mouseenter', () => {
    clearTimeout(timeout);
    const dropdown = item.querySelector('.menu-dropdown');
    if (dropdown) {
      dropdown.style.display = 'block';
    }
  });

  item.addEventListener('mouseleave', () => {
    const dropdown = item.querySelector('.menu-dropdown');
    if (dropdown) {
      timeout = setTimeout(() => {
        dropdown.style.display = 'none';
      }, 200);
    }
  });
});

// edit-ops: bkz. src/core/edit-ops.js

// yardim: bkz. src/ui/help.js
// ─── Alt Terminal Paneli ────────────────────────────────────────
;(function() {
  const panel   = document.getElementById('bottom-terminal');
  const output  = document.getElementById('terminal-output');
  const input   = document.getElementById('terminal-input');
  const prompt  = document.getElementById('terminal-prompt');
  const btnClear = document.getElementById('terminal-clear-btn');
  const btnKill  = document.getElementById('terminal-kill-btn');
  const btnClose = document.getElementById('terminal-close-btn');
  const resizeHandle = document.getElementById('terminal-resize-handle');

  let cwd = null;          // null → ana süreç HOME'unu kullanır
  let history = [];        // komut geçmişi
  let histIdx = -1;        // geçmiş gezginme indeksi
  let running = false;     // komut çalışıyor mu?
  let removeOutputListener = null;
  let outputChars = 0;
  const MAX_TERMINAL_NODES = 2000;
  const MAX_TERMINAL_CHARS = 250000;

  // Prompt metnini güncelle
  function updatePrompt() {
    const dir = cwd ? cwd.replace(/^\/home\/[^/]+/, '~') : '~';
    prompt.textContent = dir + ' $';
  }

  // Çıktı satırı ekle
  function appendLine(text, cls) {
    const span = document.createElement('span');
    span.className = 'term-line-' + cls;
    span.textContent = text;
    output.appendChild(span);
    outputChars += text.length;

    while (output.childNodes.length > MAX_TERMINAL_NODES || outputChars > MAX_TERMINAL_CHARS) {
      const first = output.firstChild;
      if (!first) break;
      outputChars -= first.textContent ? first.textContent.length : 0;
      output.removeChild(first);
    }

    output.scrollTop = output.scrollHeight;
  }

  // Terminale hoşgeldin mesajı
  function printWelcome() {
    appendLine('MELP IDE Terminal — bash\n', 'info');
  }

  // Temizle
  function clearOutput() {
    output.innerHTML = '';
    outputChars = 0;
  }

  // Terminali aç/kapat
  window.toggleBottomTerminal = function(forceOpen) {
    const isHidden = panel.classList.contains('hidden');
    if (forceOpen === true || isHidden) {
      panel.classList.remove('hidden');
      if (output.children.length === 0) {
        cwd = null;
        updatePrompt();
        printWelcome();
      }
      requestAnimationFrame(() => input.focus());
    } else {
      panel.classList.add('hidden');
    }
  };

  // Kapat butonu
  btnClose.addEventListener('click', () => panel.classList.add('hidden'));

  // Temizle butonu
  btnClear.addEventListener('click', clearOutput);

  // Durdur butonu
  btnKill.addEventListener('click', () => {
    if (window.electronAPI?.terminalKill) {
      window.electronAPI.terminalKill();
    }
  });

  // Terminal pozisyon butonları
  document.querySelectorAll('.term-pos-btn').forEach(btn => {
    btn.addEventListener('click', () => {
      const pos = btn.dataset.pos;
      document.querySelectorAll('.term-pos-btn').forEach(b => b.classList.remove('active'));
      btn.classList.add('active');
      panel.classList.remove('term-pos-center', 'term-pos-left-center', 'term-pos-center-right');
      if (pos !== 'full') panel.classList.add('term-pos-' + pos);
    });
  });

  // IPC çıkış dinleyicisini kur (sadece bir kez)
  function setupOutputListener() {
    if (removeOutputListener) removeOutputListener();
    if (!window.electronAPI?.onTerminalOutput) return;
    removeOutputListener = window.electronAPI.onTerminalOutput((data) => {
      if (data.t === 'stdout') {
        appendLine(data.s, 'stdout');
      } else if (data.t === 'stderr') {
        appendLine(data.s, 'stderr');
      } else if (data.t === 'exit') {
        running = false;
        input.disabled = false;
        input.focus();
        if (data.code !== 0 && data.code !== null) {
          appendLine(`\n[süreç bitti, çıkış kodu: ${data.code}]\n`, 'info');
        } else {
          appendLine('', 'stdout');
        }
      }
    });
  }
  setupOutputListener();

  // Komut çalıştır
  function runCommand(cmd) {
    cmd = cmd.trim();
    if (!cmd) return;

    if (history[history.length - 1] !== cmd) history.push(cmd);
    histIdx = -1;
    input.value = '';

    appendLine(prompt.textContent + ' ' + cmd + '\n', 'prompt');

    if (cmd.startsWith('cd')) {
      const parts = cmd.split(/\s+/);
      const target = parts[1] || (typeof process !== 'undefined' ? process.env.HOME : '~');
      if (window.electronAPI?.terminalExec) {
        running = true;
        input.disabled = true;
        window.electronAPI.terminalExec(`cd ${target} 2>&1 && pwd`, cwd);
        if (removeOutputListener) removeOutputListener();
        let pwdBuf = '';
        removeOutputListener = window.electronAPI.onTerminalOutput((data) => {
          if (data.t === 'stdout') { pwdBuf += data.s; }
          else if (data.t === 'stderr') { appendLine(data.s, 'stderr'); }
          else if (data.t === 'exit') {
            running = false;
            input.disabled = false;
            const newDir = pwdBuf.trim().split('\n').pop();
            if (newDir && newDir.startsWith('/')) {
              cwd = newDir;
              updatePrompt();
            }
            setupOutputListener();
            input.focus();
          }
        });
      }
      return;
    }

    if (cmd === 'clear' || cmd === 'cls') {
      clearOutput();
      return;
    }

    if (window.electronAPI?.terminalExec) {
      running = true;
      input.disabled = true;
      window.electronAPI.terminalExec(cmd, cwd);
    } else {
      appendLine('[Hata: electronAPI bulunamadı — Electron dışında çalışıyor]\n', 'stderr');
    }
  }

  // Enter / geçmiş / Ctrl+C / Ctrl+L
  input.addEventListener('keydown', (e) => {
    if (e.key === 'Enter') {
      e.preventDefault();
      runCommand(input.value);
    } else if (e.key === 'ArrowUp') {
      e.preventDefault();
      if (history.length === 0) return;
      if (histIdx === -1) histIdx = history.length - 1;
      else if (histIdx > 0) histIdx--;
      input.value = history[histIdx];
      setTimeout(() => input.setSelectionRange(input.value.length, input.value.length), 0);
    } else if (e.key === 'ArrowDown') {
      e.preventDefault();
      if (histIdx === -1) return;
      histIdx++;
      if (histIdx >= history.length) { histIdx = -1; input.value = ''; }
      else input.value = history[histIdx];
    } else if (e.ctrlKey && e.key === 'c') {
      e.preventDefault();
      if (running && window.electronAPI?.terminalKill) {
        window.electronAPI.terminalKill();
        appendLine('^C\n', 'info');
      } else {
        appendLine('\n', 'stdout');
        input.value = '';
      }
    } else if (e.ctrlKey && e.key === 'l') {
      e.preventDefault();
      clearOutput();
    }
  });

  // ── Terminal çıktısı sağ tık menüsü ──────────────────────
  const termMenu = document.getElementById('code-context-menu');

  output.addEventListener('contextmenu', (e) => {
    e.preventDefault();
    e.stopPropagation();

    const sel = window.getSelection()?.toString() ?? '';
    window._termCtxSel = sel;

    const x = Math.min(e.clientX, window.innerWidth  - 160);
    const y = Math.min(e.clientY, window.innerHeight - 130);
    termMenu.style.left    = x + 'px';
    termMenu.style.top     = y + 'px';
    termMenu.style.display = 'block';

    termMenu.querySelectorAll('.ctx-item').forEach(item => {
      const a = item.dataset.action;
      if (a === 'copy') {
        item.style.opacity       = sel.length > 0 ? '1' : '0.4';
        item.style.pointerEvents = sel.length > 0 ? '' : 'none';
        item.style.display = '';
      } else if (a === 'selectall') {
        item.style.display = '';
        item.style.opacity = '1';
        item.style.pointerEvents = '';
      } else {
        item.style.display = 'none';
      }
    });

    const closeTermMenu = (ev) => {
      if (!termMenu.contains(ev.target)) {
        termMenu.style.display = 'none';
        termMenu.querySelectorAll('.ctx-item').forEach(i => {
          i.style.display = '';
          i.style.opacity = '';
          i.style.pointerEvents = '';
        });
        document.removeEventListener('mousedown', closeTermMenu);
      }
    };
    setTimeout(() => document.addEventListener('mousedown', closeTermMenu), 0);
  });

  // Terminal menüsü tıklama
  termMenu.addEventListener('mousedown', (e) => {
    if (!output.contains(document.activeElement) && !window._termCtxSel) return;
    if (typeof window._termCtxSel === 'undefined') return;
    const action = e.target.closest('.ctx-item')?.dataset.action;
    if (!action) return;
    e.preventDefault();
    termMenu.style.display = 'none';
    termMenu.querySelectorAll('.ctx-item').forEach(i => {
      i.style.display = '';
      i.style.opacity = '';
      i.style.pointerEvents = '';
    });
    if (action === 'copy') {
      if (window._termCtxSel) {
        navigator.clipboard.writeText(window._termCtxSel).catch(() => {
          document.execCommand('copy');
        });
      }
    } else if (action === 'selectall') {
      const range = document.createRange();
      range.selectNodeContents(output);
      const sel = window.getSelection();
      sel.removeAllRanges();
      sel.addRange(range);
    }
    window._termCtxSel = undefined;
  });

  // ── Dikey boyutlandırma (sürükle) ──────────────────────────
  let resizing = false, startY = 0, startH = 0;

  resizeHandle.addEventListener('mousedown', (e) => {
    resizing = true;
    startY = e.clientY;
    startH = panel.offsetHeight;
    document.body.style.cursor = 'ns-resize';
    document.body.style.userSelect = 'none';
    e.preventDefault();
  });

  document.addEventListener('mousemove', (e) => {
    if (!resizing) return;
    const delta = startY - e.clientY; // yukarı sürük → büyüt
    const newH = Math.max(80, Math.min(window.innerHeight - 160, startH + delta)); // 160px: menubar+toolbar+statusbar icin yer birak
    panel.style.height = newH + 'px';
  });

  document.addEventListener('mouseup', () => {
    if (!resizing) return;
    resizing = false;
    document.body.style.cursor = '';
    document.body.style.userSelect = '';
  });

  // WASM çıktısını terminal paneline yazan global yardımcı
  window._appendTerminal = function(type, data) {
    if (type === 'stdout') appendLine(data, 'stdout');
    else if (type === 'stderr') appendLine(data, 'stderr');
    else if (type === 'exit') {
      if (data !== 0) {
        appendLine('\n[süreç bitti, çıkış kodu: ' + data + ']\n', 'info');
      } else {
        appendLine('', 'stdout');
      }
    }
  };

  updatePrompt();
})();

// dialogs: bkz. src/ui/dialogs.js

// ═══════════════════════════════════════════════════════════════
// Kod editörü — Autocomplete (IntelliSense)
// ═══════════════════════════════════════════════════════════════
(function() {
  const editor   = document.getElementById('code-editor');
  const dropdown = document.getElementById('ac-dropdown');
  if (!editor || !dropdown) return;

  // MLP anahtar kelimeleri + yerleşik fonksiyonlar
  const AC_ITEMS = [
    // Yerleşik fonksiyonlar
    { label: 'print',        snippet: 'print()',        icon: 'ƒ', type: 'fonksiyon' },
    { label: 'println',      snippet: 'println()',      icon: 'ƒ', type: 'fonksiyon' },
    { label: 'len',          snippet: 'len()',          icon: 'ƒ', type: 'fonksiyon' },
    { label: 'type_of',      snippet: 'type_of()',      icon: 'ƒ', type: 'fonksiyon' },
    { label: 'to_number',    snippet: 'to_number()',    icon: 'ƒ', type: 'fonksiyon' },
    { label: 'to_string',    snippet: 'to_string()',    icon: 'ƒ', type: 'fonksiyon' },
    { label: 'to_logical',   snippet: 'to_logical()',   icon: 'ƒ', type: 'fonksiyon' },
    { label: 'input',        snippet: 'input()',        icon: 'ƒ', type: 'fonksiyon' },
    { label: 'sqrt',         snippet: 'sqrt()',         icon: 'ƒ', type: 'fonksiyon' },
    { label: 'abs',          snippet: 'abs()',          icon: 'ƒ', type: 'fonksiyon' },
    { label: 'floor',        snippet: 'floor()',        icon: 'ƒ', type: 'fonksiyon' },
    { label: 'ceil',         snippet: 'ceil()',         icon: 'ƒ', type: 'fonksiyon' },
    { label: 'round',        snippet: 'round()',        icon: 'ƒ', type: 'fonksiyon' },
    { label: 'push',         snippet: 'push()',         icon: 'ƒ', type: 'fonksiyon' },
    { label: 'pop',          snippet: 'pop()',          icon: 'ƒ', type: 'fonksiyon' },
    // Kontrol akışı
    { label: 'if',           snippet: 'if ',            icon: '⌥', type: 'anahtar kelime' },
    { label: 'else',         snippet: 'else\n    ',     icon: '⌥', type: 'anahtar kelime' },
    { label: 'elif',         snippet: 'elif ',          icon: '⌥', type: 'anahtar kelime' },
    { label: 'end_if',       snippet: 'end_if',         icon: '⌥', type: 'anahtar kelime' },
    { label: 'while',        snippet: 'while ',         icon: '⌥', type: 'anahtar kelime' },
    { label: 'end_while',    snippet: 'end_while',      icon: '⌥', type: 'anahtar kelime' },
    { label: 'for',          snippet: 'for ',           icon: '⌥', type: 'anahtar kelime' },
    { label: 'in',           snippet: 'in ',            icon: '⌥', type: 'anahtar kelime' },
    { label: 'end_for',      snippet: 'end_for',        icon: '⌥', type: 'anahtar kelime' },
    { label: 'return',       snippet: 'return ',        icon: '⌥', type: 'anahtar kelime' },
    { label: 'break',        snippet: 'break',          icon: '⌥', type: 'anahtar kelime' },
    { label: 'continue',     snippet: 'continue',       icon: '⌥', type: 'anahtar kelime' },
    // Tanım blokları
    { label: 'function',     snippet: 'function ',      icon: '◈', type: 'blok' },
    { label: 'end_function', snippet: 'end_function',   icon: '◈', type: 'blok' },
    { label: 'event',        snippet: 'event ',         icon: '◈', type: 'blok' },
    { label: 'end_event',    snippet: 'end_event',      icon: '◈', type: 'blok' },
    { label: 'struct',       snippet: 'struct ',        icon: '◈', type: 'blok' },
    { label: 'end_struct',   snippet: 'end_struct',     icon: '◈', type: 'blok' },
    { label: 'interface',    snippet: 'interface ',     icon: '◈', type: 'blok' },
    { label: 'end_interface',snippet: 'end_interface',  icon: '◈', type: 'blok' },
    { label: 'program',      snippet: 'program ',       icon: '◈', type: 'blok' },
    { label: 'app_start',    snippet: 'app_start()',    icon: '◈', type: 'blok' },
    { label: 'end_app',      snippet: 'end_app',        icon: '◈', type: 'blok' },
    { label: 'import',       snippet: 'import ',        icon: '◈', type: 'blok' },
    { label: 'implements',   snippet: 'implements ',    icon: '◈', type: 'blok' },
    // Tipler
    { label: 'numeric',      snippet: 'numeric ',       icon: 'T', type: 'tip' },
    { label: 'character',    snippet: 'character ',     icon: 'T', type: 'tip' },
    { label: 'logical',      snippet: 'logical ',       icon: 'T', type: 'tip' },
    { label: 'void',         snippet: 'void',           icon: 'T', type: 'tip' },
    // Değerler
    { label: 'true',         snippet: 'true',           icon: '•', type: 'değer' },
    { label: 'false',        snippet: 'false',          icon: '•', type: 'değer' },
    { label: 'null',         snippet: 'null',           icon: '•', type: 'değer' },
    // Snippet'lar (! prefix ile tetiklenir)
    { label: '!btn-click',  icon: '✂', type: 'snippet',
      snippet: `event buton_tikla()\n    -- buton click handler\nend_event` },
    { label: '!if-else',    icon: '✂', type: 'snippet',
      snippet: `if  then\n    \nelse\n    \nend_if` },
    { label: '!loop',       icon: '✂', type: 'snippet',
      snippet: `for i in range(0, 10)\n    \nend_for` },
    { label: '!gui-init',   icon: '✂', type: 'snippet',
      snippet: `app_start()\n    GUI_PENCERE_AC(800, 600, "Uygulama")\nend_app` },
    { label: '!func',       icon: '✂', type: 'snippet',
      snippet: `function isim() -> void\n    \nend_function` },
    { label: '!struct',     icon: '✂', type: 'snippet',
      snippet: `struct Isim\n    -- alanlar\nend_struct` },
    { label: '!while',      icon: '✂', type: 'snippet',
      snippet: `while  do\n    \nend_while` },
    { label: '!program',    icon: '✂', type: 'snippet',
      snippet: `program Uygulamam\n\napp_start()\n    GUI_PENCERE_AC(800, 600, "Uygulamam")\nend_app` },
  ];

  let acActive = -1;   // seçili öğe indeksi
  let acItems  = [];   // mevcut filtrelenmiş liste
  let acPrefix = '';   // mevcut prefix

  // Seçimi görsel olarak güncelle
  function updateActive() {
    dropdown.querySelectorAll('.ac-item').forEach((el, i) => {
      el.classList.toggle('ac-active', i === acActive);
      if (i === acActive) el.scrollIntoView({ block: 'nearest' });
    });
  }

  // Dropdown'u kapat
  function closeAC() {
    dropdown.classList.add('hidden');
    dropdown.innerHTML = '';
    acActive = -1;
    acItems  = [];
    acPrefix = '';
  }

  // İmleç öncesindeki mevcut kelimeyi al
  function getPrefix() {
    const sel = window.getSelection();
    if (!sel || sel.rangeCount === 0) return '';
    const range = sel.getRangeAt(0).cloneRange();
    range.collapse(true);
    // Satır başından imlece kadar olan metni al
    const tmpRange = document.createRange();
    tmpRange.setStart(range.startContainer, 0);
    tmpRange.setEnd(range.startContainer, range.startOffset);
    const lineText = tmpRange.toString();
    // ! snippet tetikleyicisi: !harf-harf
    const snippetM = lineText.match(/![a-z\-]*$/);
    if (snippetM) return snippetM[0];
    // Normal kelime tamamlama (harf, rakam, _)
    const m = lineText.match(/[\w]+$/);
    return m ? m[0] : '';
  }

  // İmleç position'unu hesapla (fixed koordinat)
  function getCursorRect() {
    const sel = window.getSelection();
    if (!sel || sel.rangeCount === 0) return null;
    const range = sel.getRangeAt(0).cloneRange();
    range.collapse(true);
    const rects = range.getClientRects();
    if (rects.length > 0) return rects[0];
    // Fallback: editör rectangle
    return editor.getBoundingClientRect();
  }

  // Prefix'i silip snippet'i ekle
  function insertSnippet(snippet) {
    const sel = window.getSelection();
    if (!sel || sel.rangeCount === 0) return;
    const range = sel.getRangeAt(0);
    // Prefix kadar geri git
    const node = range.startContainer;
    const off  = range.startOffset;
    const prefLen = acPrefix.length;
    if (node.nodeType === Node.TEXT_NODE && off >= prefLen) {
      const delRange = document.createRange();
      delRange.setStart(node, off - prefLen);
      delRange.setEnd(node, off);
      delRange.deleteContents();
      // Snippet ekle
      const textNode = document.createTextNode(snippet);
      delRange.insertNode(textNode);
      // İmleci konumlandır:
      // "()" ile bitiyorsa parantezin içine (son karakterden 1 geri), yoksa sona
      const newRange = document.createRange();
      const cursorBack = snippet.endsWith('()') ? 1 : 0;
      newRange.setStart(textNode, textNode.length - cursorBack);
      newRange.collapse(true);
      sel.removeAllRanges();
      sel.addRange(newRange);
    }
    closeAC();
    editor.dispatchEvent(new Event('input'));
  }

  // Dropdown'u göster / güncelle
  function showAC(prefix) {
    if (!prefix || prefix.length < 1) { closeAC(); return; }
    const lower = prefix.toLowerCase();
    let filtered;
    if (prefix.startsWith('!')) {
      // Snippet modu: yerleşik + kullanıcı snippet'leri
      const userSnippets = loadUserSnippets()
        .map(s => ({ label: s.trigger, snippet: s.body, icon: '✂', type: 'snippet', _desc: s.desc }));
      const allSnippets = [...AC_ITEMS, ...userSnippets];
      filtered = allSnippets.filter(it => it.type === 'snippet' && it.label.startsWith(lower));
    } else {
      // Normal tamamlama: snippet olmayanlar
      filtered = AC_ITEMS.filter(it => it.type !== 'snippet' && it.label.toLowerCase().startsWith(lower) && it.label !== lower);
    }
    if (filtered.length === 0) { closeAC(); return; }

    acItems  = filtered;
    acPrefix = prefix;
    acActive = 0;

    dropdown.innerHTML = '';
    filtered.forEach((item, i) => {
      const div = document.createElement('div');
      div.className = 'ac-item' + (i === 0 ? ' ac-active' : '');
      div.innerHTML = `<span class="ac-item-icon">${item.icon}</span>` +
                      `<span class="ac-item-label">${item.label}</span>` +
                      `<span class="ac-item-type">${item._desc || item.type}</span>`;
      if (item._desc) div.title = item._desc;
      div.addEventListener('mousedown', (e) => {
        e.preventDefault();
        insertSnippet(item.snippet);
      });
      dropdown.appendChild(div);
    });

    // Konumlandır: imleç altına
    const rect = getCursorRect();
    if (!rect) return;
    const ddH = Math.min(220, filtered.length * 28 + 8);
    const below = window.innerHeight - rect.bottom;
    let top = rect.bottom + 2;
    if (below < ddH + 4) top = rect.top - ddH - 2; // üste aç
    let left = rect.left;
    if (left + 220 > window.innerWidth) left = window.innerWidth - 225;

    dropdown.style.top  = top  + 'px';
    dropdown.style.left = left + 'px';
    dropdown.classList.remove('hidden');
  }

  // input → prefix hesapla → göster (min 2 karakter)
  editor.addEventListener('input', () => {
    const prefix = getPrefix();
    if (!prefix || prefix.length < 2) { closeAC(); return; }
    showAC(prefix);
  });

  // Klavye navigasyonu — editörde
  editor.addEventListener('keydown', (e) => {
    if (dropdown.classList.contains('hidden')) return;
    if (e.key === 'ArrowDown') {
      e.preventDefault();
      acActive = Math.min(acActive + 1, acItems.length - 1);
      updateActive();
    } else if (e.key === 'ArrowUp') {
      e.preventDefault();
      acActive = Math.max(acActive - 1, 0);
      updateActive();
    } else if (e.key === 'Tab' || e.key === 'Enter') {
      if (acActive >= 0 && acItems[acActive]) {
        e.preventDefault();
        insertSnippet(acItems[acActive].snippet);
      }
    } else if (e.key === 'Escape') {
      e.preventDefault();
      closeAC();
    }
  }, true); // capture: Tab'dan önce yakala

  // ──── Otomatik kapanan karakterler ─────────────────────────────
  const closingChars = new Map([['(', ')'], ['[', ']'], ['"', '"'], ["'", "'"]]);

  editor.addEventListener('keydown', (e) => {
    const close = closingChars.get(e.key);
    const sel   = window.getSelection();
    if (!sel || !sel.rangeCount) return;

    const range = sel.getRangeAt(0);
    const node  = range.startContainer;
    const off   = range.startOffset;

    // Kapanan karakterin üstüne aynı karakter → sadece ilerle (çift ekleme)
    if (closingChars.has(e.key) && !close && range.collapsed) {
      if (node.nodeType === Node.TEXT_NODE && node.textContent[off] === e.key) {
        e.preventDefault();
        const nr = document.createRange();
        nr.setStart(node, off + 1);
        nr.collapse(true);
        sel.removeAllRanges();
        sel.addRange(nr);
        return;
      }
    }

    // Backspace: "" veya '' imleci ortadayken ikisini birden sil
    if (e.key === 'Backspace' && range.collapsed) {
      if (node.nodeType === Node.TEXT_NODE && off > 0) {
        const prev = node.textContent[off - 1];
        const next = node.textContent[off];
        if ((prev === '"' && next === '"') || (prev === "'" && next === "'") ||
            (prev === '(' && next === ')') || (prev === '[' && next === ']')) {
          e.preventDefault();
          const dr = document.createRange();
          dr.setStart(node, off - 1);
          dr.setEnd(node, off + 1);
          dr.deleteContents();
          return;
        }
      }
    }

    if (!close) return;

    // Seçili metin varsa: etrafını sar ( "seçili" → "seçili" )
    if (!range.collapsed) {
      e.preventDefault();
      const selected = range.toString();
      range.deleteContents();
      const textNode = document.createTextNode(e.key + selected + close);
      range.insertNode(textNode);
      const newRange = document.createRange();
      newRange.setStart(textNode, 1 + selected.length);
      newRange.collapse(true);
      sel.removeAllRanges();
      sel.addRange(newRange);
      return;
    }

    // Seçili metin yok: çift ekle, ortaya konumlan
    e.preventDefault();
    const textNode = document.createTextNode(e.key + close);
    range.insertNode(textNode);
    const newRange = document.createRange();
    newRange.setStart(textNode, 1);
    newRange.collapse(true);
    sel.removeAllRanges();
    sel.addRange(newRange);
  }, true);
}());

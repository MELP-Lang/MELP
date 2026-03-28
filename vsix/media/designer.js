/** MELP VS Code WebView - Tasarımcı (media/designer.js) */
(function () {
  'use strict';

  const vscode = acquireVsCodeApi();

  function reportWebviewError(text) {
    vscode.postMessage({ type: 'showError', text: text });
  }

  if (Array.isArray(window.__melpQueuedErrors)) {
    window.__melpQueuedErrors.forEach(message => {
      if (message && message.type === 'showError') {
        reportWebviewError(message.text);
      }
    });
    window.__melpQueuedErrors = [];
  }

  window.addEventListener('error', event => {
    reportWebviewError('Tasarimci webview hatasi: ' + event.message + ' @ ' + (event.filename || 'inline') + ':' + (event.lineno || 0));
  });

  window.addEventListener('unhandledrejection', event => {
    const reason = event.reason instanceof Error ? event.reason.message : String(event.reason);
    reportWebviewError('Tasarimci promise hatasi: ' + reason);
  });

  // --- Durum ---
  let state = {
    widgets: [],
    code: '',
    canvasSettings: { title: 'Pencere', width: 800, height: 600, backgroundColor: '#1e1e1e', gridSize: 20, snapToGrid: true },
    selectedId: null,
    nextId: 1,
  };

  // --- Undo Stack ---
  let undoStack = [];
  let redoStack = [];
  const MAX_UNDO = 50;

  function pushUndo() {
    undoStack.push(JSON.stringify({ widgets: state.widgets, code: state.code }));
    if (undoStack.length > MAX_UNDO) { undoStack.shift(); }
    redoStack = [];
    updateUndoButtons();
  }

  function undo() {
    if (undoStack.length === 0) { return; }
    redoStack.push(JSON.stringify({ widgets: state.widgets, code: state.code }));
    const snap = JSON.parse(undoStack.pop());
    state.widgets = snap.widgets;
    state.code = snap.code;
    state.selectedId = null;
    renderAll();
    updateUndoButtons();
  }

  function redo() {
    if (redoStack.length === 0) { return; }
    undoStack.push(JSON.stringify({ widgets: state.widgets, code: state.code }));
    const snap = JSON.parse(redoStack.pop());
    state.widgets = snap.widgets;
    state.code = snap.code;
    state.selectedId = null;
    renderAll();
    updateUndoButtons();
  }

  function updateUndoButtons() {
    const btnUndo = document.getElementById('btn-undo');
    const btnRedo = document.getElementById('btn-redo');
    if (btnUndo) { btnUndo.disabled = undoStack.length === 0; btnUndo.title = `Geri Al (Ctrl+Z) — ${undoStack.length} adım`; }
    if (btnRedo) { btnRedo.disabled = redoStack.length === 0; btnRedo.title = `Yinele (Ctrl+Y) — ${redoStack.length} adım`; }
  }

  let dragState = null;
  let resizeState = null;

  // --- DOM ---
  const canvas       = document.getElementById('canvas');
  const propContent  = document.getElementById('properties-content');
  const layersList   = document.getElementById('layers-list');
  const codeTabs     = document.getElementById('code-tabs');

  // --- Mesaj dinleyici ---
  window.addEventListener('message', ({ data }) => {
    if (data.type === 'load') {
      loadState(data.content);
    }
  });

  vscode.postMessage({ type: 'requestData' });

  // --- Yükleme ---
  function loadState(data) {
    if (!data) { return; }
    state.widgets = data.widgets || [];
    state.code    = data.code    || '';
    state.canvasSettings = Object.assign(state.canvasSettings, data.canvasSettings || {});
    state.nextId  = state.widgets.reduce((m, w) => Math.max(m, parseInt(w.id?.replace(/\D/g, '') || '0', 10)), 0) + 1;
    undoStack = []; redoStack = [];
    updateUndoButtons();
    applyCanvas();
    renderAll();
    buildCodeTabs();
  }

  // --- Kaydetme ---
  function saveState() {
    vscode.postMessage({
      type: 'save',
      content: {
        version: '1.0',
        canvasSettings: state.canvasSettings,
        widgets: state.widgets,
        code: state.code,
      }
    });
  }

  // --- Tuval ayarları ---
  function applyCanvas() {
    const s = state.canvasSettings;
    canvas.style.width           = (s.width  || 800) + 'px';
    canvas.style.minHeight       = (s.height || 600) + 'px';
    canvas.style.backgroundColor = s.backgroundColor || '#1e1e1e';
    const gs = s.gridSize || 20;
    canvas.style.backgroundSize  = `${gs}px ${gs}px`;
  }

  // --- Render ---
  function renderAll() {
    canvas.innerHTML = '';
    state.widgets.forEach(renderWidget);
    renderLayers();
    renderProperties();
  }

  function renderWidget(w) {
    const el = document.createElement('div');
    el.className = 'design-widget' + (w.id === state.selectedId ? ' selected' : '');
    el.dataset.id = w.id;
    el.style.left   = w.x + 'px';
    el.style.top    = w.y + 'px';
    el.style.width  = (w.width  || 100) + 'px';
    el.style.height = (w.height || 30)  + 'px';

    el.innerHTML = buildWidgetHTML(w);

    const rh = document.createElement('div');
    rh.className = 'resize-handle';
    el.appendChild(rh);
    rh.addEventListener('mousedown', e => startResize(e, w));

    el.addEventListener('mousedown', e => {
      if (e.target === rh) { return; }
      selectWidget(w.id);
      startDrag(e, w, el);
    });

    canvas.appendChild(el);
    return el;
  }

  function buildWidgetHTML(w) {
    const fs = w.fontSize || 13;
    const fc = w.foregroundColor || 'inherit';
    const bg = w.backgroundColor || 'transparent';
    switch (w.type) {
      case 'button':   return `<button style="width:100%;height:100%;font-size:${fs}px;background:${bg};color:${fc}">${w.text||'Düğme'}</button>`;
      case 'label':    return `<span style="font-size:${fs}px;color:${fc};background:${bg};display:block;padding:2px">${w.text||'Etiket'}</span>`;
      case 'textbox':  return `<input type="text" placeholder="${w.placeholder||''}" value="${w.text||''}" style="width:100%;height:100%;font-size:${fs}px;box-sizing:border-box">`;
      case 'checkbox': return `<label style="font-size:${fs}px"><input type="checkbox" ${w.checked?'checked':''}> ${w.text||'Onay'}</label>`;
      case 'radiobutton': return `<label style="font-size:${fs}px"><input type="radio" name="${w.group||'group1'}" ${w.checked?'checked':''}> ${w.text||'Seçenek'}</label>`;
      case 'combobox': return `<select style="width:100%;height:100%;font-size:${fs}px"><option>${w.text||'Seçenek'}</option></select>`;
      case 'listbox':  return `<select multiple style="width:100%;height:100%;font-size:${fs}px"><option>${w.text||'Öğe'}</option></select>`;
      case 'slider':   return `<input type="range" min="${w.min||0}" max="${w.max||100}" value="${w.value||50}" style="width:100%;height:100%">`;
      case 'progressbar': return `<progress value="${w.value||50}" max="${w.max||100}" style="width:100%;height:100%"></progress>`;
      case 'image':    return `<div style="width:100%;height:100%;border:1px dashed #888;display:flex;align-items:center;justify-content:center;font-size:11px">🖼️ ${w.src||'resim'}</div>`;
      case 'panel':    return `<div style="width:100%;height:100%;border:1px solid #555;background:${bg === 'transparent' ? 'rgba(255,255,255,0.05)' : bg}"></div>`;
      case 'groupbox': return `<fieldset style="width:100%;height:100%;border:1px solid #666;background:${bg};color:${fc}"><legend style="font-size:${fs}px">${w.text||'Grup'}</legend></fieldset>`;
      default:         return `<div style="width:100%;height:100%;border:1px solid #888;display:flex;align-items:center;justify-content:center;font-size:11px">${w.type}</div>`;
    }
  }

  function renderLayers() {
    layersList.innerHTML = '';
    state.widgets.slice().reverse().forEach(w => {
      const item = document.createElement('div');
      item.className = 'layer-item' + (w.id === state.selectedId ? ' selected' : '');
      item.textContent = `${w.type} — ${w.id}`;
      item.title = `${w.type} (${w.x}, ${w.y})`;
      item.addEventListener('click', () => selectWidget(w.id));
      layersList.appendChild(item);
    });
  }

  // --- Properties Paneli (Genişletilmiş) ---
  function renderProperties() {
    propContent.innerHTML = '';
    const w = state.widgets.find(x => x.id === state.selectedId);
    if (!w) {
      propContent.innerHTML = '<p class="hint">Bir widget seçin</p>';
      renderCanvasProperties();
      return;
    }

    // Widget türüne göre ek özellikler
    const extraProps = getExtraProps(w.type);

    const allProps = [
      { key: 'id',    label: 'ID',       readonly: true  },
      { key: 'type',  label: 'Tip',      readonly: true  },
      { key: 'text',  label: 'Metin',    type: 'text' },
      { key: 'x',     label: 'X',        type: 'number' },
      { key: 'y',     label: 'Y',        type: 'number' },
      { key: 'width', label: 'Genişlik', type: 'number' },
      { key: 'height',label: 'Yükseklik',type: 'number' },
      { key: 'fontSize', label: 'Font Boyutu', type: 'number', default: 13 },
      { key: 'foregroundColor', label: 'Yazı Rengi', type: 'color', default: '#ffffff' },
      { key: 'backgroundColor', label: 'Arkaplan', type: 'color', default: '#transparent' },
      { key: 'visible', label: 'Görünür', type: 'checkbox', default: true },
      { key: 'enabled', label: 'Etkin',   type: 'checkbox', default: true },
      ...extraProps,
    ];

    for (const p of allProps) {
      const val = w[p.key] ?? p.default ?? '';
      const row = document.createElement('div');
      row.className = 'prop-row';
      const lbl = document.createElement('div');
      lbl.className = 'prop-label';
      lbl.textContent = p.label;

      let inp;
      if (p.type === 'checkbox') {
        inp = document.createElement('input');
        inp.type = 'checkbox';
        inp.checked = !!val;
        inp.readOnly = p.readonly || false;
        if (!p.readonly) {
          inp.addEventListener('change', () => {
            pushUndo();
            w[p.key] = inp.checked;
            refreshWidget(w);
          });
        }
      } else if (p.type === 'color') {
        inp = document.createElement('input');
        inp.type = 'color';
        inp.value = (typeof val === 'string' && val.startsWith('#')) ? val : '#ffffff';
        if (!p.readonly) {
          inp.addEventListener('input', () => {
            w[p.key] = inp.value;
            refreshWidget(w);
          });
          inp.addEventListener('change', () => { pushUndo(); });
        }
      } else {
        inp = document.createElement('input');
        inp.className = 'prop-input';
        inp.type = p.type || 'text';
        inp.value = val;
        inp.readOnly = p.readonly || false;
        if (!p.readonly) {
          inp.addEventListener('change', () => {
            pushUndo();
            w[p.key] = p.type === 'number' ? (parseInt(inp.value, 10) || 0) : inp.value;
            refreshWidget(w);
          });
        }
      }

      row.appendChild(lbl);
      row.appendChild(inp);
      propContent.appendChild(row);
    }
  }

  function getExtraProps(type) {
    switch (type) {
      case 'textbox':  return [{ key: 'placeholder', label: 'Yer Tutucu', type: 'text', default: '' }];
      case 'checkbox': return [{ key: 'checked', label: 'İşaretli', type: 'checkbox', default: false }];
      case 'radiobutton': return [
        { key: 'checked', label: 'İşaretli', type: 'checkbox', default: false },
        { key: 'group', label: 'Grup Adı', type: 'text', default: 'group1' }
      ];
      case 'slider': return [
        { key: 'min', label: 'Minimum', type: 'number', default: 0 },
        { key: 'max', label: 'Maksimum', type: 'number', default: 100 },
        { key: 'value', label: 'Değer', type: 'number', default: 50 },
        { key: 'step', label: 'Adım', type: 'number', default: 1 }
      ];
      case 'progressbar': return [
        { key: 'value', label: 'Değer', type: 'number', default: 50 },
        { key: 'max', label: 'Maksimum', type: 'number', default: 100 }
      ];
      case 'image':    return [{ key: 'src', label: 'Kaynak URL', type: 'text', default: '' }];
      default:         return [];
    }
  }

  function renderCanvasProperties() {
    const s = state.canvasSettings;
    const canvasProps = [
      { key: 'title',           label: 'Pencere Adı',  type: 'text' },
      { key: 'width',           label: 'Genişlik',     type: 'number' },
      { key: 'height',          label: 'Yükseklik',    type: 'number' },
      { key: 'backgroundColor', label: 'Arkaplan',     type: 'color' },
      { key: 'gridSize',        label: 'Grid Boyutu',  type: 'number' },
      { key: 'snapToGrid',      label: 'Grid\'e Yapış', type: 'checkbox' },
    ];

    const header = document.createElement('div');
    header.className = 'prop-label';
    header.style.fontWeight = 'bold';
    header.style.marginBottom = '4px';
    header.textContent = 'Tuval Ayarları';
    propContent.appendChild(header);

    for (const p of canvasProps) {
      const val = s[p.key];
      const row = document.createElement('div');
      row.className = 'prop-row';
      const lbl = document.createElement('div');
      lbl.className = 'prop-label';
      lbl.textContent = p.label;

      let inp;
      if (p.type === 'checkbox') {
        inp = document.createElement('input');
        inp.type = 'checkbox';
        inp.checked = !!val;
        inp.addEventListener('change', () => { s[p.key] = inp.checked; applyCanvas(); });
      } else if (p.type === 'color') {
        inp = document.createElement('input');
        inp.type = 'color';
        inp.value = (typeof val === 'string' && val.startsWith('#')) ? val : '#1e1e1e';
        inp.addEventListener('input', () => { s[p.key] = inp.value; applyCanvas(); });
      } else {
        inp = document.createElement('input');
        inp.className = 'prop-input';
        inp.type = p.type || 'text';
        inp.value = val ?? '';
        inp.addEventListener('change', () => {
          s[p.key] = p.type === 'number' ? (parseInt(inp.value, 10) || 0) : inp.value;
          applyCanvas();
        });
      }

      row.appendChild(lbl);
      row.appendChild(inp);
      propContent.appendChild(row);
    }
  }

  function refreshWidget(w) {
    const el = canvas.querySelector(`[data-id="${w.id}"]`);
    if (!el) { return; }
    el.style.left   = w.x + 'px';
    el.style.top    = w.y + 'px';
    el.style.width  = (w.width  || 100) + 'px';
    el.style.height = (w.height || 30)  + 'px';
    el.innerHTML = buildWidgetHTML(w);
    const rh = document.createElement('div');
    rh.className = 'resize-handle';
    el.appendChild(rh);
    rh.addEventListener('mousedown', e => startResize(e, w));
  }

  // --- Seçim ---
  function selectWidget(id) {
    state.selectedId = id;
    canvas.querySelectorAll('.design-widget').forEach(el => {
      el.classList.toggle('selected', el.dataset.id === id);
    });
    renderLayers();
    renderProperties();
    buildCodeTabs();
  }

  // --- Sürükleme ---
  function startDrag(e, w, el) {
    e.preventDefault();
    pushUndo();
    dragState = { widgetId: w.id, startX: e.clientX, startY: e.clientY, origX: w.x, origY: w.y };
    document.addEventListener('mousemove', onDragMove);
    document.addEventListener('mouseup', onDragEnd, { once: true });
  }

  function snap(v) {
    if (!state.canvasSettings.snapToGrid) { return v; }
    const gs = state.canvasSettings.gridSize || 20;
    return Math.round(v / gs) * gs;
  }

  function onDragMove(e) {
    if (!dragState) { return; }
    const w = state.widgets.find(x => x.id === dragState.widgetId);
    if (!w) { return; }
    w.x = snap(Math.max(0, dragState.origX + e.clientX - dragState.startX));
    w.y = snap(Math.max(0, dragState.origY + e.clientY - dragState.startY));
    const el = canvas.querySelector(`[data-id="${w.id}"]`);
    if (el) { el.style.left = w.x + 'px'; el.style.top = w.y + 'px'; }
  }

  function onDragEnd() {
    dragState = null;
    document.removeEventListener('mousemove', onDragMove);
    renderProperties();
  }

  // --- Resize ---
  function startResize(e, w) {
    e.preventDefault();
    e.stopPropagation();
    pushUndo();
    resizeState = { widgetId: w.id, startX: e.clientX, startY: e.clientY, origW: w.width, origH: w.height };
    document.addEventListener('mousemove', onResizeMove);
    document.addEventListener('mouseup', onResizeEnd, { once: true });
  }

  function onResizeMove(e) {
    if (!resizeState) { return; }
    const w = state.widgets.find(x => x.id === resizeState.widgetId);
    if (!w) { return; }
    w.width  = Math.max(20, snap(resizeState.origW + e.clientX - resizeState.startX));
    w.height = Math.max(16, snap(resizeState.origH + e.clientY - resizeState.startY));
    const el = canvas.querySelector(`[data-id="${w.id}"]`);
    if (el) { el.style.width = w.width + 'px'; el.style.height = w.height + 'px'; }
  }

  function onResizeEnd() {
    resizeState = null;
    document.removeEventListener('mousemove', onResizeMove);
    renderProperties();
  }

  // --- Toolbar: widget ekle ---
  document.querySelectorAll('.tool-btn').forEach(btn => {
    btn.addEventListener('dblclick', () => {
      addWidget(btn.dataset.type);
    });
    btn.addEventListener('click', () => {
      btn.style.outline = '2px solid var(--highlight)';
      setTimeout(() => { btn.style.outline = ''; }, 500);
    });
  });

  function addWidget(type) {
    pushUndo();
    const id = `${type}_${state.nextId++}`;
    const defaults = { 
      button: { width:100, height:30 }, 
      label: { width:80, height:20 }, 
      textbox: { width:120, height:24 }, 
      checkbox: { width:100, height:20 },
      radiobutton: { width:100, height:20 },
      combobox: { width:120, height:24 }, 
      listbox: { width:120, height:80 },
      slider: { width:150, height:24 },
      progressbar: { width:200, height:24 },
      image: { width:100, height:80 }, 
      panel: { width:200, height:150 },
      groupbox: { width:200, height:150 }
    };
    const def = defaults[type] || { width:100, height:30 };
    const w = {
      id, type,
      x: snap(40), y: snap(40),
      width: def.width, height: def.height,
      text: type === 'button' ? 'Düğme' : type === 'label' ? 'Etiket' : type === 'radiobutton' ? 'Seçenek' : type === 'groupbox' ? 'Grup' : '',
      visible: true, enabled: true,
      fontSize: 13,
    };
    // Widget tipine özgü varsayılanlar
    if (type === 'radiobutton') { w.group = 'group1'; w.checked = false; }
    if (type === 'slider') { w.min = 0; w.max = 100; w.value = 50; w.step = 1; }
    if (type === 'progressbar') { w.value = 50; w.max = 100; }
    
    state.widgets.push(w);
    renderWidget(w);
    selectWidget(id);
    ensureEventStub(id, 'onClick');
    renderLayers();
  }

  // --- Canvas tıklama (boş alana) ---
  canvas.addEventListener('mousedown', e => {
    if (e.target === canvas) {
      state.selectedId = null;
      canvas.querySelectorAll('.design-widget').forEach(el => el.classList.remove('selected'));
      renderLayers();
      renderProperties();
    }
  });

  // --- Delete ---
  document.getElementById('btn-delete').addEventListener('click', deleteSelected);
  document.addEventListener('keydown', e => {
    if ((e.key === 'Delete' || e.key === 'Backspace') && state.selectedId && document.activeElement.tagName !== 'INPUT' && document.activeElement.tagName !== 'TEXTAREA') {
      deleteSelected();
    }
    if ((e.ctrlKey || e.metaKey) && e.key === 's') { e.preventDefault(); saveState(); }
    if ((e.ctrlKey || e.metaKey) && e.key === 'z') { e.preventDefault(); undo(); }
    if ((e.ctrlKey || e.metaKey) && e.key === 'y') { e.preventDefault(); redo(); }
  });

  function deleteSelected() {
    if (!state.selectedId) { return; }
    pushUndo();
    state.widgets = state.widgets.filter(w => w.id !== state.selectedId);
    state.selectedId = null;
    renderAll();
  }

  // --- Kaydet düğmesi ---
  document.getElementById('btn-save').addEventListener('click', saveState);

  // --- Undo/Redo düğmeleri ---
  const btnUndo = document.getElementById('btn-undo');
  const btnRedo = document.getElementById('btn-redo');
  if (btnUndo) { btnUndo.addEventListener('click', undo); }
  if (btnRedo) { btnRedo.addEventListener('click', redo); }

  // --- Önizleme ---
  document.getElementById('btn-preview').addEventListener('click', showPreview);

  const btnOpenCodeEditor = document.getElementById('btn-open-code-editor');
  if (btnOpenCodeEditor) {
    btnOpenCodeEditor.addEventListener('click', () => {
      saveState();
      vscode.postMessage({ type: 'openCodeEditor' });
    });
  }

  // --- Çalıştır ---
  const btnRun = document.getElementById('btn-run');
  if (btnRun) {
    btnRun.addEventListener('click', () => {
      saveState();
      vscode.postMessage({ type: 'run' });
    });
  }

  function closePreviewOverlay() {
    const existingOverlay = document.getElementById('gui-preview-overlay');
    if (existingOverlay) {
      existingOverlay.remove();
    }
    window.removeEventListener('keydown', onPreviewKeyDown);
  }

  function onPreviewKeyDown(event) {
    if (event.key === 'Escape') {
      event.preventDefault();
      closePreviewOverlay();
    }
  }

  function showPreview() {
    closePreviewOverlay();

    const overlay = document.createElement('div');
    overlay.id = 'gui-preview-overlay';
    overlay.style.cssText = 'position:fixed;top:0;left:0;width:100%;height:100%;background:rgba(0,0,0,0.7);z-index:1000;display:flex;align-items:center;justify-content:center';

    const closeButton = document.createElement('button');
    closeButton.id = 'gui-preview-close-floating';
    closeButton.textContent = 'Kapat';
    closeButton.style.cssText = 'position:fixed;top:16px;right:16px;z-index:1002;background:#c42b1c;color:#fff;border:1px solid #ffb7b0;border-radius:4px;padding:8px 12px;font-size:13px;font-weight:600;cursor:pointer;box-shadow:0 4px 18px rgba(0,0,0,0.35)';

    const win = document.createElement('div');
    const s = state.canvasSettings;
    win.style.cssText = `position:relative;width:${s.width||800}px;height:${s.height||600}px;background:${s.backgroundColor||'#1e1e1e'};border:1px solid #555;border-radius:4px;overflow:hidden`;

    const titleBar = document.createElement('div');
    titleBar.style.cssText = 'background:#333;padding:4px 8px;font-size:12px;display:flex;justify-content:space-between;align-items:center';
    titleBar.innerHTML = `<span>${s.title||'Pencere'}</span><button id="gui-preview-close" style="background:#555;border:1px solid #777;color:#fff;cursor:pointer;font-size:12px;padding:2px 8px;border-radius:3px">Kapat</button>`;
    win.appendChild(titleBar);

    const body = document.createElement('div');
    body.style.cssText = `position:relative;width:100%;height:${(s.height||600)-26}px;background:${s.backgroundColor||'#1e1e1e'}`;

    state.widgets.forEach(w => {
      if (!w.visible) { return; }
      const el = document.createElement('div');
      el.style.cssText = `position:absolute;left:${w.x}px;top:${w.y}px;width:${w.width||100}px;height:${w.height||30}px`;
      el.innerHTML = buildWidgetHTML(w);
      // Event görsel geri bildirim
      if (w.type === 'button') {
        el.querySelector('button')?.addEventListener('click', () => {
          const info = document.createElement('div');
          info.style.cssText = 'position:absolute;bottom:8px;right:8px;background:#333;padding:4px 8px;border-radius:4px;font-size:12px;color:#ccc';
          info.textContent = `${w.id}_onClick tetiklendi`;
          body.appendChild(info);
          setTimeout(() => info.remove(), 1500);
        });
      }
      body.appendChild(el);
    });

    win.appendChild(body);
    overlay.appendChild(closeButton);
    overlay.appendChild(win);
    overlay.addEventListener('click', e => { if (e.target === overlay) { closePreviewOverlay(); } });
    document.body.appendChild(overlay);
    document.getElementById('gui-preview-close')?.addEventListener('click', closePreviewOverlay);
    closeButton.addEventListener('click', closePreviewOverlay);
    window.addEventListener('keydown', onPreviewKeyDown);
  }

  // --- Kod sekmeleri ---
  const eventsByType = {
    button:      ['onClick'],
    label:       [],
    textbox:     ['onChange', 'onFocus', 'onBlur'],
    checkbox:    ['onChange'],
    radiobutton: ['onChange'],
    combobox:    ['onChange'],
    listbox:     ['onChange', 'onSelect'],
    slider:      ['onChange'],
    progressbar: [],
    image:       ['onClick'],
    panel:       [],
    groupbox:    [],
  };

  function buildCodeTabs() {
    codeTabs.innerHTML = '';
    const selected = state.widgets.find(x => x.id === state.selectedId);
    const widgetsToShow = selected ? [selected] : state.widgets;

    widgetsToShow.forEach(w => {
      const events = eventsByType[w.type] ?? ['onClick'];
      if (events.length === 0) { return; }
      events.forEach(ev => {
        const tab = document.createElement('div');
        tab.className = 'code-tab';
        tab.textContent = selected ? `${w.id}.${ev}` : `${w.id}.${ev}`;
        tab.title = w.id + ' → ' + ev;
        tab.addEventListener('click', () => {
          codeTabs.querySelectorAll('.code-tab').forEach(t => t.classList.remove('active'));
          tab.classList.add('active');
          openEventInEditor(w.id, ev);
        });
        codeTabs.appendChild(tab);
      });
    });
    if (codeTabs.firstChild) { codeTabs.firstChild.classList.add('active'); }
  }

  function ensureEventStub(widgetId, eventName) {
    const handlerName = `${widgetId}_${eventName}`;
    if (!state.code.includes(`event ${handlerName}`)) {
      state.code += `\nevent ${handlerName}()\n    \nend_event\n`;
    }
  }

  function openEventInEditor(widgetId, eventName) {
    ensureEventStub(widgetId, eventName);
    saveState();
    vscode.postMessage({ type: 'openCodeEditor', handlerName: `${widgetId}_${eventName}` });
  }

  // İlk render
  renderAll();
  updateUndoButtons();
})();

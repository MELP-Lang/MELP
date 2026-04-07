// custom-widgets.js — Özel Widget Oluşturma ve Yönetimi

const CUSTOM_WIDGETS_KEY = 'melp_custom_widgets';
let _cwEditId = null;

// localStorage'dan özel widget listesini yükle
function loadCustomWidgets() {
  try {
    return JSON.parse(localStorage.getItem(CUSTOM_WIDGETS_KEY) || '[]');
  } catch { return []; }
}

// Özel widget listesini localStorage'a kaydet
function saveCustomWidgets(list) {
  localStorage.setItem(CUSTOM_WIDGETS_KEY, JSON.stringify(list));
}

// Toolbox "Özel Bileşenler" kategorisini yenile
function renderCustomWidgetsToolbox() {
  const list = loadCustomWidgets();
  const container = document.getElementById('custom-widgets-list');
  const emptyMsg  = document.getElementById('custom-widgets-empty');
  if (!container) return;
  while (container.firstChild) container.removeChild(container.firstChild);

  list.forEach(cw => {
    const item = document.createElement('div');
    item.className = 'widget-item';
    item.draggable = true;
    item.dataset.widgetType = cw.baseType;
    item.dataset.customId = cw.id;
    item.innerHTML =
      `<span class="widget-icon">${cw.icon || '🧩'}</span>` +
      `<span class="widget-name">${cw.name}</span>`;
    container.appendChild(item);
  });

  // Boş durum mesajı
  if (emptyMsg) emptyMsg.style.display = list.length === 0 ? '' : 'none';

  // Dinamik öğelere event listener ekle (designer hazır olduğunda)
  if (typeof designer !== 'undefined') {
    container.querySelectorAll('.widget-item').forEach(item => {
      item.addEventListener('dragstart', e => designer.onToolboxDragStart(e));
      item.addEventListener('dblclick',  e => designer.onToolboxDblClick(e));
    });
  }
}

// Props alanlarını seçili base type'a göre göster/gizle ve varsayılanları doldur
function _updateCwPropsFields(keepValues) {
  const baseType    = document.getElementById('cw-base-type').value;
  const hasText     = ['button','label','textbox','checkbox','radio','groupbox'].includes(baseType);
  const hasBgColor  = ['button','label','textbox','panel','listbox','combobox','groupbox'].includes(baseType);
  const hasTextClr  = ['button','label','textbox','checkbox','radio','groupbox','listbox','combobox'].includes(baseType);
  const hasFontSize = ['button','label','textbox','checkbox','radio','groupbox','listbox','combobox'].includes(baseType);

  document.getElementById('cw-row-text').style.display      = hasText      ? '' : 'none';
  document.getElementById('cw-row-bgcolor').style.display   = hasBgColor   ? '' : 'none';
  document.getElementById('cw-row-textcolor').style.display = hasTextClr   ? '' : 'none';
  document.getElementById('cw-row-fontsize').style.display  = hasFontSize  ? '' : 'none';

  if (!keepValues) {
    const defs = typeof getDefaultProperties === 'function'
      ? getDefaultProperties(baseType) : {};
    document.getElementById('cw-width').value  = defs.width  || 120;
    document.getElementById('cw-height').value = defs.height || 30;
    if (hasText) document.getElementById('cw-text').value = defs.text || '';
    if (hasBgColor) {
      const bg = defs.backgroundColor || '#3c3c3c';
      document.getElementById('cw-bgcolor-hex').value    = bg;
      document.getElementById('cw-bgcolor-picker').value =
        /^#[0-9a-fA-F]{6}$/.test(bg) ? bg : '#3c3c3c';
    }
    if (hasTextClr) {
      const tc = defs.textColor || '#d4d4d4';
      document.getElementById('cw-textcolor-hex').value    = tc;
      document.getElementById('cw-textcolor-picker').value =
        /^#[0-9a-fA-F]{6}$/.test(tc) ? tc : '#d4d4d4';
    }
    if (hasFontSize) document.getElementById('cw-fontsize').value = defs.fontSize || 14;
  }
}

// Oluşturma/düzenleme dialogunu aç
function handleCreateCustomWidget(editId) {
  _cwEditId = editId || null;
  const overlay = document.getElementById('custom-widget-overlay');
  if (!overlay) return;

  if (_cwEditId) {
    document.getElementById('custom-widget-dialog-title').textContent = '🧩 Özel Widget Düzenle';
    const cw = loadCustomWidgets().find(w => w.id === _cwEditId);
    if (!cw) return;
    document.getElementById('cw-name').value      = cw.name;
    document.getElementById('cw-icon').value      = cw.icon || '🧩';
    document.getElementById('cw-base-type').value = cw.baseType;
    document.getElementById('cw-width').value     = cw.defaults.width  || 120;
    document.getElementById('cw-height').value    = cw.defaults.height || 30;
    _updateCwPropsFields(true);
    if (cw.defaults.text !== undefined)
      document.getElementById('cw-text').value = cw.defaults.text;
    if (cw.defaults.backgroundColor) {
      document.getElementById('cw-bgcolor-hex').value = cw.defaults.backgroundColor;
      if (/^#[0-9a-fA-F]{6}$/.test(cw.defaults.backgroundColor))
        document.getElementById('cw-bgcolor-picker').value = cw.defaults.backgroundColor;
    }
    if (cw.defaults.textColor) {
      document.getElementById('cw-textcolor-hex').value = cw.defaults.textColor;
      if (/^#[0-9a-fA-F]{6}$/.test(cw.defaults.textColor))
        document.getElementById('cw-textcolor-picker').value = cw.defaults.textColor;
    }
    if (cw.defaults.fontSize)
      document.getElementById('cw-fontsize').value = cw.defaults.fontSize;
  } else {
    document.getElementById('custom-widget-dialog-title').textContent = '🧩 Özel Widget Oluştur';
    document.getElementById('cw-name').value      = '';
    document.getElementById('cw-icon').value      = '🧩';
    document.getElementById('cw-base-type').value = 'button';
    _updateCwPropsFields(false);
  }

  overlay.classList.remove('hidden');
}

// Dialog event listener'larını başlat (bir kez çağrılır)
function initCustomWidgetDialog() {
  const overlay = document.getElementById('custom-widget-overlay');
  if (!overlay) return;

  document.getElementById('custom-widget-close').addEventListener('click',
    () => overlay.classList.add('hidden'));
  document.getElementById('custom-widget-cancel').addEventListener('click',
    () => overlay.classList.add('hidden'));
  overlay.addEventListener('click',
    e => { if (e.target === overlay) overlay.classList.add('hidden'); });

  // Base type değişince alanları güncelle
  document.getElementById('cw-base-type').addEventListener('change',
    () => { _cwEditId = null; _updateCwPropsFields(false); });

  // Renk picker ↔ hex input senkronizasyonu
  const bgPicker = document.getElementById('cw-bgcolor-picker');
  const bgHex    = document.getElementById('cw-bgcolor-hex');
  bgPicker.addEventListener('input', () => { bgHex.value = bgPicker.value; });
  bgHex.addEventListener('input', () => {
    if (/^#[0-9a-fA-F]{6}$/.test(bgHex.value)) bgPicker.value = bgHex.value;
  });

  const tcPicker = document.getElementById('cw-textcolor-picker');
  const tcHex    = document.getElementById('cw-textcolor-hex');
  tcPicker.addEventListener('input', () => { tcHex.value = tcPicker.value; });
  tcHex.addEventListener('input', () => {
    if (/^#[0-9a-fA-F]{6}$/.test(tcHex.value)) tcPicker.value = tcHex.value;
  });

  document.getElementById('custom-widget-save').addEventListener('click', () => {
    const name = document.getElementById('cw-name').value.trim();
    if (!name) { alert('Widget adı boş bırakılamaz.'); return; }

    const baseType = document.getElementById('cw-base-type').value;
    const icon     = document.getElementById('cw-icon').value.trim() || '🧩';
    const defaults = {};

    const w = parseInt(document.getElementById('cw-width').value);
    const h = parseInt(document.getElementById('cw-height').value);
    if (w > 0) defaults.width  = w;
    if (h > 0) defaults.height = h;

    const rowText    = document.getElementById('cw-row-text');
    const rowBg      = document.getElementById('cw-row-bgcolor');
    const rowTc      = document.getElementById('cw-row-textcolor');
    const rowFs      = document.getElementById('cw-row-fontsize');

    if (rowText.style.display !== 'none') {
      const v = document.getElementById('cw-text').value;
      if (v !== '') defaults.text = v;
    }
    if (rowBg.style.display !== 'none') {
      const v = document.getElementById('cw-bgcolor-hex').value.trim();
      if (v) defaults.backgroundColor = v;
    }
    if (rowTc.style.display !== 'none') {
      const v = document.getElementById('cw-textcolor-hex').value.trim();
      if (v) defaults.textColor = v;
    }
    if (rowFs.style.display !== 'none') {
      const v = parseInt(document.getElementById('cw-fontsize').value);
      if (v > 0) defaults.fontSize = v;
    }

    const list = loadCustomWidgets();
    if (_cwEditId) {
      const idx = list.findIndex(w2 => w2.id === _cwEditId);
      if (idx !== -1) list[idx] = { ...list[idx], name, icon, baseType, defaults };
    } else {
      list.push({ id: 'cw_' + Date.now(), name, icon, baseType, defaults });
    }

    saveCustomWidgets(list);
    renderCustomWidgetsToolbox();
    overlay.classList.add('hidden');
    showStatus(`"${name}" özel widget'ı ${_cwEditId ? 'güncellendi' : 'oluşturuldu'}`);
    _cwEditId = null;

    // Manager açıksa yenile
    const mgr = document.getElementById('custom-widget-manager-overlay');
    if (mgr && !mgr.classList.contains('hidden')) _renderCwmList();
  });
}

// ── Yönetici ──────────────────────────────────────────────────────────────────

function _renderCwmList() {
  const container = document.getElementById('cwm-list');
  if (!container) return;
  const list = loadCustomWidgets();

  if (list.length === 0) {
    container.innerHTML =
      '<div style="text-align:center;color:#888;padding:40px 0;">' +
      'Henüz özel widget oluşturulmadı.<br>' +
      '<small>Yukarıdaki "+ Yeni Widget" butonunu kullanın.</small></div>';
    return;
  }

  container.innerHTML = '';
  list.forEach(cw => {
    const row = document.createElement('div');
    row.style.cssText =
      'display:flex;align-items:center;padding:10px 8px;border-bottom:1px solid #333;gap:8px;';
    row.innerHTML =
      `<span style="font-size:22px;min-width:30px;text-align:center;">${cw.icon || '🧩'}</span>` +
      `<div style="flex:1;min-width:0;">` +
        `<div style="font-size:13px;color:#ccc;white-space:nowrap;overflow:hidden;text-overflow:ellipsis;">${cw.name}</div>` +
        `<div style="font-size:11px;color:#888;">Temel: ${cw.baseType}</div>` +
      `</div>` +
      `<button class="cwm-edit-btn settings-browse" data-id="${cw.id}" style="font-size:12px;">Düzenle</button>` +
      `<button class="cwm-export-btn settings-browse" data-id="${cw.id}" style="font-size:12px;margin:0 4px;">Dışa Aktar</button>` +
      `<button class="cwm-delete-btn" data-id="${cw.id}" ` +
        `style="background:#c42b1c;border:none;color:#fff;padding:4px 8px;border-radius:3px;cursor:pointer;font-size:12px;">Sil</button>`;
    container.appendChild(row);
  });

  container.querySelectorAll('.cwm-edit-btn').forEach(btn => {
    btn.addEventListener('click', () => {
      document.getElementById('custom-widget-manager-overlay').classList.add('hidden');
      handleCreateCustomWidget(btn.dataset.id);
    });
  });

  container.querySelectorAll('.cwm-export-btn').forEach(btn => {
    btn.addEventListener('click', () => exportCustomWidget(btn.dataset.id));
  });

  container.querySelectorAll('.cwm-delete-btn').forEach(btn => {
    btn.addEventListener('click', () => {
      const list2 = loadCustomWidgets();
      const cw2   = list2.find(w => w.id === btn.dataset.id);
      if (cw2 && confirm(`"${cw2.name}" özel widget'ı silmek istediğinizden emin misiniz?`)) {
        saveCustomWidgets(list2.filter(w => w.id !== btn.dataset.id));
        renderCustomWidgetsToolbox();
        _renderCwmList();
        showStatus(`"${cw2.name}" silindi`);
      }
    });
  });
}

function handleCustomWidgetManager() {
  const overlay = document.getElementById('custom-widget-manager-overlay');
  if (!overlay) return;
  _renderCwmList();
  overlay.classList.remove('hidden');
}

function initCustomWidgetManagerDialog() {
  const overlay = document.getElementById('custom-widget-manager-overlay');
  if (!overlay) return;

  document.getElementById('cwm-close').addEventListener('click',
    () => overlay.classList.add('hidden'));
  overlay.addEventListener('click',
    e => { if (e.target === overlay) overlay.classList.add('hidden'); });

  document.getElementById('cwm-new').addEventListener('click', () => {
    overlay.classList.add('hidden');
    handleCreateCustomWidget();
  });

  document.getElementById('cwm-import').addEventListener('click', importCustomWidget);
}

// ── Dışa / İçe Aktar ──────────────────────────────────────────────────────────

function exportCustomWidget(id) {
  const cw = loadCustomWidgets().find(w => w.id === id);
  if (!cw) return;
  const blob = new Blob([JSON.stringify(cw, null, 2)], { type: 'application/json' });
  const url  = URL.createObjectURL(blob);
  const a    = document.createElement('a');
  a.href     = url;
  a.download = `${cw.name.replace(/[^\w\s\-çğıöşüÇĞİÖŞÜ]/g, '_')}.mlpwidget`;
  a.click();
  URL.revokeObjectURL(url);
  showStatus(`"${cw.name}" dışa aktarıldı`);
}

function importCustomWidget() {
  const input  = document.createElement('input');
  input.type   = 'file';
  input.accept = '.mlpwidget,application/json';
  input.addEventListener('change', e => {
    const file = e.target.files[0];
    if (!file) return;
    const reader = new FileReader();
    reader.onload = evt => {
      try {
        const cw = JSON.parse(evt.target.result);
        if (!cw.name || !cw.baseType || typeof cw.defaults !== 'object') {
          alert('Geçersiz .mlpwidget dosyası.');
          return;
        }
        cw.id = 'cw_' + Date.now();
        const list = loadCustomWidgets();
        list.push(cw);
        saveCustomWidgets(list);
        renderCustomWidgetsToolbox();
        const mgr = document.getElementById('custom-widget-manager-overlay');
        if (mgr && !mgr.classList.contains('hidden')) _renderCwmList();
        showStatus(`"${cw.name}" içe aktarıldı`);
      } catch {
        alert('Dosya okunamadı veya geçersiz JSON.');
      }
    };
    reader.readAsText(file);
  });
  input.click();
}

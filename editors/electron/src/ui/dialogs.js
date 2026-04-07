// ==================== ARAÇLAR MENÜSÜ ====================
/** Ayarlar dialogunu aç ve mevcut değerleri doldur */
function handleSettings() {
  document.getElementById('s-compiler-path').value       = _settings.compilerPath      || '';
  document.getElementById('s-lli-path').value            = _settings.lliPath            || '';
  document.getElementById('s-tmp-file').value            = _settings.tmpFile            || '/tmp/_melp_ide_run.mlp';
  document.getElementById('s-autosave-enabled').checked  = !!_settings.autosaveEnabled;
  document.getElementById('s-autosave-interval').value   = _settings.autosaveInterval   || 60;
  document.getElementById('s-open-last').checked         = !!_settings.openLastProject;
  document.getElementById('s-enable-diagnostic-logging').checked = !!_settings.enableDiagnosticLogging;
  const langEl = document.getElementById('s-ui-language');
  if (langEl) langEl.value = (window.i18n?.getCurrentLang?.() || _settings.uiLanguage || 'tr');
  const logPathEl = document.getElementById('s-log-path');
  if (logPathEl) logPathEl.value = 'Yükleniyor...';
  if (window.electronAPI?.getLogStatus) {
    window.electronAPI.getLogStatus().then((status) => {
      if (logPathEl) logPathEl.value = status?.logPath || '';
    }).catch(() => {
      if (logPathEl) logPathEl.value = '';
    });
  }
  document.getElementById('settings-overlay').classList.remove('hidden');
}

function normalizeLogPathInput(rawPath) {
  const raw = String(rawPath || '').trim();
  if (!raw) return '';
  if (/\.(log|txt|jsonl)$/i.test(raw)) return raw;
  return raw.replace(/\/+$/, '') + '/melp-diagnostics.log';
}

/** Ayarlar dialog event listener'larını kur (bir kez çağrılır) */
function initSettingsDialog() {
  const overlay = document.getElementById('settings-overlay');
  if (!overlay) return;

  // Kapat butonları
  ['settings-close', 'settings-cancel'].forEach(id => {
    const el = document.getElementById(id);
    if (el) el.addEventListener('click', () => overlay.classList.add('hidden'));
  });
  overlay.addEventListener('click', e => {
    if (e.target === overlay) overlay.classList.add('hidden');
  });

  // Gözat butonları
  const compilerBrowse = document.getElementById('s-compiler-browse');
  if (compilerBrowse) {
    compilerBrowse.addEventListener('click', async () => {
      const r = await window.electronAPI.browseFile({ title: 'Derleyici Seç' });
      if (r && !r.canceled) document.getElementById('s-compiler-path').value = r.filePath;
    });
  }
  const lliBrowse = document.getElementById('s-lli-browse');
  if (lliBrowse) {
    lliBrowse.addEventListener('click', async () => {
      const r = await window.electronAPI.browseFile({ title: 'lli Yolu Seç' });
      if (r && !r.canceled) document.getElementById('s-lli-path').value = r.filePath;
    });
  }

  const openLogFolderBtn = document.getElementById('s-open-log-folder');
  if (openLogFolderBtn) {
    openLogFolderBtn.addEventListener('click', async () => {
      const current = document.getElementById('s-log-path')?.value || '';
      const r = await window.electronAPI.openLogFolder(current);
      if (!r?.success) {
        alert('Log klasörü açılamadı: ' + (r?.error || 'Bilinmeyen hata'));
      }
    });
  }

  const browseLogFolderBtn = document.getElementById('s-browse-log-folder');
  if (browseLogFolderBtn) {
    browseLogFolderBtn.addEventListener('click', async () => {
      const current = document.getElementById('s-log-path')?.value || '';
      const r = await window.electronAPI.browseDirectory({
        title: 'Log klasörü seç',
        defaultPath: current
      });
      if (r && !r.canceled) {
        document.getElementById('s-log-path').value = normalizeLogPathInput(r.filePath);
      }
    });
  }

  const logPathInput = document.getElementById('s-log-path');
  if (logPathInput) {
    logPathInput.addEventListener('blur', () => {
      logPathInput.value = normalizeLogPathInput(logPathInput.value);
    });
  }

  // Kaydet butonu
  const saveBtn = document.getElementById('settings-save');
  if (saveBtn) {
    saveBtn.addEventListener('click', async () => {
      const newSettings = {
        ..._settings,
        compilerPath:     document.getElementById('s-compiler-path').value.trim(),
        lliPath:          document.getElementById('s-lli-path').value.trim(),
        tmpFile:          document.getElementById('s-tmp-file').value.trim() || '/tmp/_melp_ide_run.mlp',
        autosaveEnabled:  document.getElementById('s-autosave-enabled').checked,
        autosaveInterval: parseInt(document.getElementById('s-autosave-interval').value, 10) || 60,
        openLastProject:  document.getElementById('s-open-last').checked,
        enableDiagnosticLogging: document.getElementById('s-enable-diagnostic-logging').checked,
        diagnosticLogPath: normalizeLogPathInput(document.getElementById('s-log-path').value),
        uiLanguage:       document.getElementById('s-ui-language')?.value || 'tr',
      };
      try {
        const r = await window.electronAPI.saveSettings(newSettings);
        if (r && r.success) {
          _settings = newSettings;
          _currentMlpFile = _settings.tmpFile;
          restartAutosave();
          updateAutosaveMenu();
          if (typeof refreshMemoryLogState === 'function') refreshMemoryLogState();
          // Dil değişmişse UI'yı güncelle
          if (window.i18n && newSettings.uiLanguage) {
            window.i18n.setLanguage(newSettings.uiLanguage).catch(() => {});
          }
          overlay.classList.add('hidden');
          showStatus('Ayarlar kaydedildi');
          if (typeof logDiagnostic === 'function') {
            logDiagnostic('settings', 'settings-updated', {
              loggingEnabled: !!newSettings.enableDiagnosticLogging,
              autosaveEnabled: !!newSettings.autosaveEnabled,
              openLastProject: !!newSettings.openLastProject
            });
          }
        } else {
          alert('Ayarlar kaydedilemedi: ' + (r && r.error ? r.error : 'Bilinmeyen hata'));
        }
      } catch (err) {
        alert('Ayarlar kaydedilemedi: ' + err.message);
      }
    });
  }
}

// ─── Yerleşik Şablonlar ──────────────────────────────────────
const BUILTIN_TEMPLATES = [
  {
    name: 'Giriş Formu',
    icon: '🔐',
    description: 'Kullanıcı adı + şifre + giriş butonu',
    widgets: [
      { type: 'label',   properties: { x:50, y:60,  width:200, height:25, text:'Kullanıcı Adı',    textColor:'#d4d4d4', fontSize:13, fontFamily:'Arial', visible:true } },
      { type: 'textbox', properties: { x:50, y:90,  width:200, height:30, placeholder:'kullanıcı adı', backgroundColor:'#3c3c3c', textColor:'#d4d4d4', fontSize:13, fontFamily:'Arial', borderWidth:1, borderColor:'#555', borderStyle:'solid', enabled:true, visible:true, tabIndex:1, onChange:'' } },
      { type: 'label',   properties: { x:50, y:135, width:200, height:25, text:'Şifre',             textColor:'#d4d4d4', fontSize:13, fontFamily:'Arial', visible:true } },
      { type: 'textbox', properties: { x:50, y:165, width:200, height:30, placeholder:'••••••••',   backgroundColor:'#3c3c3c', textColor:'#d4d4d4', fontSize:13, fontFamily:'Arial', borderWidth:1, borderColor:'#555', borderStyle:'solid', enabled:true, visible:true, tabIndex:2, onChange:'' } },
      { type: 'button',  properties: { x:50, y:215, width:120, height:35, text:'Giriş Yap',  backgroundColor:'#0e639c', textColor:'#fff', fontSize:13, fontFamily:'Arial', borderWidth:0, borderColor:'#888', borderStyle:'solid', enabled:true, visible:true, tabIndex:3, onClick:'giris_yap' } },
      { type: 'button',  properties: { x:180,y:215, width:70,  height:35, text:'İptal',      backgroundColor:'#555',    textColor:'#fff', fontSize:13, fontFamily:'Arial', borderWidth:0, borderColor:'#888', borderStyle:'solid', enabled:true, visible:true, tabIndex:4, onClick:'' } },
    ]
  },
  {
    name: 'Ayarlar Paneli',
    icon: '⚙️',
    description: 'Grup kutuları + combo + checkbox',
    widgets: [
      { type: 'groupbox', properties: { x:20, y:20,  width:360, height:140, text:'Genel',    borderWidth:1, borderColor:'#555', borderStyle:'solid', textColor:'#d4d4d4', fontSize:12, fontFamily:'Arial', visible:true } },
      { type: 'label',    properties: { x:45, y:52,  width:80,  height:25,  text:'Dil',      textColor:'#d4d4d4', fontSize:12, fontFamily:'Arial', visible:true } },
      { type: 'combobox', properties: { x:45, y:78,  width:160, height:30,  items:['Türkçe','English','Deutsch'], selectedIndex:0, backgroundColor:'#3c3c3c', textColor:'#d4d4d4', fontSize:12, fontFamily:'Arial', borderWidth:1, borderColor:'#555', borderStyle:'solid', enabled:true, visible:true, tabIndex:1, onChange:'' } },
      { type: 'checkbox', properties: { x:45, y:118, width:200, height:25,  text:'Başlangıçta aç', checked:false, textColor:'#d4d4d4', fontSize:12, fontFamily:'Arial', borderWidth:0, borderColor:'#888', borderStyle:'solid', enabled:true, visible:true, tabIndex:2, onChange:'' } },
      { type: 'groupbox', properties: { x:20, y:175, width:360, height:100, text:'Görünüm', borderWidth:1, borderColor:'#555', borderStyle:'solid', textColor:'#d4d4d4', fontSize:12, fontFamily:'Arial', visible:true } },
      { type: 'checkbox', properties: { x:45, y:205, width:150, height:25,  text:'Koyu tema', checked:true, textColor:'#d4d4d4', fontSize:12, fontFamily:'Arial', borderWidth:0, borderColor:'#888', borderStyle:'solid', enabled:true, visible:true, tabIndex:3, onChange:'' } },
      { type: 'button',   properties: { x:265,y:295, width:80,  height:32,  text:'Kaydet', backgroundColor:'#0e639c', textColor:'#fff', fontSize:12, fontFamily:'Arial', borderWidth:0, borderColor:'#888', borderStyle:'solid', enabled:true, visible:true, tabIndex:4, onClick:'ayarlari_kaydet' } },
      { type: 'button',   properties: { x:175,y:295, width:80,  height:32,  text:'İptal',  backgroundColor:'#555',    textColor:'#fff', fontSize:12, fontFamily:'Arial', borderWidth:0, borderColor:'#888', borderStyle:'solid', enabled:true, visible:true, tabIndex:5, onClick:'' } },
    ]
  },
  {
    name: 'Araç Çubuğu',
    icon: '🔧',
    description: 'Üst panel + işlem butonları',
    widgets: [
      { type: 'panel',  properties: { x:0,   y:0, width:580, height:44, backgroundColor:'#2d2d30', borderWidth:1, borderColor:'#3e3e42', borderStyle:'solid', visible:true } },
      { type: 'button', properties: { x:8,   y:6, width:60,  height:32, text:'Yeni',    backgroundColor:'#3e3e42', textColor:'#ccc', fontSize:12, fontFamily:'Arial', borderWidth:1, borderColor:'#555', borderStyle:'solid', enabled:true, visible:true, tabIndex:1, onClick:'yeni_dosya' } },
      { type: 'button', properties: { x:74,  y:6, width:55,  height:32, text:'Aç',     backgroundColor:'#3e3e42', textColor:'#ccc', fontSize:12, fontFamily:'Arial', borderWidth:1, borderColor:'#555', borderStyle:'solid', enabled:true, visible:true, tabIndex:2, onClick:'dosya_ac' } },
      { type: 'button', properties: { x:135, y:6, width:65,  height:32, text:'Kaydet', backgroundColor:'#3e3e42', textColor:'#ccc', fontSize:12, fontFamily:'Arial', borderWidth:1, borderColor:'#555', borderStyle:'solid', enabled:true, visible:true, tabIndex:3, onClick:'kaydet' } },
      { type: 'panel',  properties: { x:207, y:4, width:2,   height:36, backgroundColor:'#555555', borderWidth:0, borderColor:'#888', borderStyle:'solid', visible:true } },
      { type: 'button', properties: { x:217, y:6, width:55,  height:32, text:'Kes',    backgroundColor:'#3e3e42', textColor:'#ccc', fontSize:12, fontFamily:'Arial', borderWidth:1, borderColor:'#555', borderStyle:'solid', enabled:true, visible:true, tabIndex:4, onClick:'kes' } },
      { type: 'button', properties: { x:278, y:6, width:70,  height:32, text:'Kopyala',backgroundColor:'#3e3e42', textColor:'#ccc', fontSize:12, fontFamily:'Arial', borderWidth:1, borderColor:'#555', borderStyle:'solid', enabled:true, visible:true, tabIndex:5, onClick:'kopyala' } },
      { type: 'button', properties: { x:354, y:6, width:70,  height:32, text:'Yapıştır',backgroundColor:'#3e3e42',textColor:'#ccc', fontSize:12, fontFamily:'Arial', borderWidth:1, borderColor:'#555', borderStyle:'solid', enabled:true, visible:true, tabIndex:6, onClick:'yapistir' } },
    ]
  }
];

const TEMPLATES_STORAGE_KEY = 'mlp-user-templates';

/** Şablon seçici dialogunu aç */
function handleTemplates() {
  const overlay = document.getElementById('templates-overlay');
  if (!overlay) return;
  renderTemplatesGrid();
  overlay.classList.remove('hidden');
}

/** templates-overlay kapanma vs listener'larını kur */
function initTemplatesDialog() {
  const overlay = document.getElementById('templates-overlay');
  if (!overlay) return;
  document.getElementById('templates-close').addEventListener('click', () => overlay.classList.add('hidden'));
  overlay.addEventListener('click', e => { if (e.target === overlay) overlay.classList.add('hidden'); });
}

/** Şablon grid'ini render et */
function renderTemplatesGrid() {
  const builtinGrid = document.getElementById('builtin-templates-grid');
  if (!builtinGrid) return;
  builtinGrid.innerHTML = '';

  BUILTIN_TEMPLATES.forEach(tmpl => {
    const card = _makeTemplateCard(tmpl, () => loadTemplate(tmpl));
    builtinGrid.appendChild(card);
  });

  // Kullanıcı şablonları
  const userSection = document.getElementById('user-templates-section');
  const userGrid    = document.getElementById('user-templates-grid');
  try {
    const saved = JSON.parse(localStorage.getItem(TEMPLATES_STORAGE_KEY) || '[]');
    if (saved.length > 0 && userSection && userGrid) {
      userSection.style.display = '';
      userGrid.innerHTML = '';
      saved.forEach((tmpl, idx) => {
        const card = _makeTemplateCard(tmpl, () => loadTemplate(tmpl), () => {
          saved.splice(idx, 1);
          localStorage.setItem(TEMPLATES_STORAGE_KEY, JSON.stringify(saved));
          renderTemplatesGrid();
        });
        userGrid.appendChild(card);
      });
    } else if (userSection) {
      userSection.style.display = 'none';
    }
  } catch(e) {}
}

function _makeTemplateCard(tmpl, onLoad, onDelete) {
  const card = document.createElement('div');
  card.style.cssText = 'background:var(--bg-secondary,#2d2d30);border:1px solid #444;border-radius:6px;padding:12px;cursor:pointer;text-align:center;transition:border-color .15s;';
  card.innerHTML = `
    <div style="font-size:28px;margin-bottom:6px;">${tmpl.icon || '🧩'}</div>
    <div style="font-weight:bold;font-size:13px;color:#ccc;margin-bottom:4px;">${tmpl.name}</div>
    <div style="font-size:11px;color:#888;">${tmpl.description || ''} <span style="opacity:.6">(${(tmpl.widgets || []).length} widget)</span></div>
  `;
  card.addEventListener('mouseenter', () => { card.style.borderColor = '#0e639c'; });
  card.addEventListener('mouseleave', () => { card.style.borderColor = '#444'; });
  card.addEventListener('click', () => {
    onLoad();
    document.getElementById('templates-overlay').classList.add('hidden');
  });
  if (onDelete) {
    const del = document.createElement('button');
    del.textContent = '🗑';
    del.title = 'Şablonu sil';
    del.style.cssText = 'margin-top:8px;background:none;border:1px solid #555;color:#888;border-radius:3px;cursor:pointer;font-size:12px;padding:2px 6px;';
    del.addEventListener('click', e => { e.stopPropagation(); onDelete(); });
    card.appendChild(del);
  }
  return card;
}

/** Şablonu canvas'a yükle */
function loadTemplate(tmpl) {
  if (!tmpl || !tmpl.widgets) return;
  // Mevcut canvas'ta alan varsa ortaya gelsin
  tmpl.widgets.forEach(data => {
    const w = new Widget(data.type, { ...data.properties });
    designer.addWidget(w);
  });
  saveToUndoStack();
  showStatus(`Şablon yüklendi: ${tmpl.name} (${tmpl.widgets.length} widget)`);
}

/** Seçili widget'ları şablon olarak kaydet */
function handleSaveTemplate() {
  const selected = designer.selectedWidgets ? [...designer.selectedWidgets] : [];
  if (selected.length === 0) {
    showStatus('Şablon olarak kaydetmek için widget seçin (Ctrl+Click)');
    return;
  }
  const name = prompt('Şablon adı girin:', `Şablon ${Date.now() % 10000}`);
  if (!name || !name.trim()) return;
  const tmpl = {
    name: name.trim(),
    icon: '🧩',
    description: `${selected.length} widget`,
    widgets: selected.map(w => ({ type: w.type, properties: JSON.parse(JSON.stringify(w.properties)) }))
  };
  try {
    const saved = JSON.parse(localStorage.getItem(TEMPLATES_STORAGE_KEY) || '[]');
    saved.unshift(tmpl);
    localStorage.setItem(TEMPLATES_STORAGE_KEY, JSON.stringify(saved));
    showStatus(`Şablon kaydedildi: "${tmpl.name}"`);
  } catch(e) {
    alert('Şablon kaydedilemedi: ' + e.message);
  }
  // Dosyaya da kaydet
  if (window.electronAPI?.saveTemplate) {
    window.electronAPI.saveTemplate(tmpl).then(r => {
      if (!r.canceled) showStatus(`Şablon dosyaya kaydedildi: ${r.filePath}`);
    }).catch(() => {});
  }
}

async function handleImportTemplate() {
  if (!window.electronAPI?.importTemplate) {
    showStatus('Şablon içe aktarma sadece masaüstü uygulamada desteklenir');
    return;
  }
  const r = await window.electronAPI.importTemplate();
  if (r.canceled) return;
  if (r.error) { alert('Şablon dosyası okunamadı: ' + r.error); return; }
  const tmpl = r.data;
  if (!tmpl || !tmpl.widgets) { alert('Geçersiz şablon dosyası'); return; }
  try {
    const saved = JSON.parse(localStorage.getItem(TEMPLATES_STORAGE_KEY) || '[]');
    saved.unshift({ ...tmpl, name: tmpl.name || 'İçe Aktarılan Şablon' });
    localStorage.setItem(TEMPLATES_STORAGE_KEY, JSON.stringify(saved));
    showStatus(`Şablon içe aktarıldı: "${tmpl.name}"`);
  } catch(e) {
    alert('Şablon içe aktarılamadı: ' + e.message);
  }
}

// ═══════════════════════════════════════════════════════════════
// Snippet Yöneticisi
// ═══════════════════════════════════════════════════════════════
const USER_SNIPPETS_KEY = 'mlp-user-snippets';

function loadUserSnippets() {
  try { return JSON.parse(localStorage.getItem(USER_SNIPPETS_KEY) || '[]'); } catch { return []; }
}
function saveUserSnippets(list) {
  localStorage.setItem(USER_SNIPPETS_KEY, JSON.stringify(list));
}

function handleSnippetManager() {
  const overlay = document.getElementById('snippet-manager-overlay');
  overlay.classList.remove('hidden');

  let snippets = loadUserSnippets();
  let selectedIdx = -1;

  const list      = document.getElementById('snippet-list');
  const triggerEl = document.getElementById('snip-trigger');
  const descEl    = document.getElementById('snip-desc');
  const bodyEl    = document.getElementById('snip-body');

  function renderList() {
    list.innerHTML = '';
    if (snippets.length === 0) {
      list.innerHTML = '<div style="padding:12px;font-size:12px;color:#666;text-align:center;">Henüz snippet yok.<br>"+ Yeni" ile ekleyin.</div>';
      return;
    }
    snippets.forEach((s, i) => {
      const div = document.createElement('div');
      div.style.cssText = `padding:7px 10px;cursor:pointer;font-size:12px;font-family:monospace;border-bottom:1px solid #333;${i === selectedIdx ? 'background:#094771;color:#fff;' : 'color:#ccc;'}`;
      div.textContent = s.trigger || '(isimsiz)';
      div.title = s.desc || '';
      div.addEventListener('click', () => {
        selectedIdx = i;
        triggerEl.value = s.trigger || '';
        descEl.value    = s.desc    || '';
        bodyEl.value    = s.body    || '';
        renderList();
      });
      list.appendChild(div);
    });
  }

  function clearForm() {
    triggerEl.value = '!';
    descEl.value = '';
    bodyEl.value = '';
    selectedIdx = -1;
    renderList();
    triggerEl.focus();
  }

  renderList();

  const addBtn    = document.getElementById('snippet-add-btn');
  const deleteBtn = document.getElementById('snippet-delete-btn');
  const saveBtn   = document.getElementById('snippet-save-btn');
  const closeBtn  = document.getElementById('snippet-manager-close');

  // Klonlanmış elemanları temizlemek için yeni listener'lar ekle
  const newAddBtn = addBtn.cloneNode(true);
  const newDelBtn = deleteBtn.cloneNode(true);
  const newSaveBtn = saveBtn.cloneNode(true);
  const newCloseBtn = closeBtn.cloneNode(true);
  addBtn.replaceWith(newAddBtn);
  deleteBtn.replaceWith(newDelBtn);
  saveBtn.replaceWith(newSaveBtn);
  closeBtn.replaceWith(newCloseBtn);

  newAddBtn.addEventListener('click', clearForm);

  newDelBtn.addEventListener('click', () => {
    if (selectedIdx < 0 || selectedIdx >= snippets.length) return;
    snippets.splice(selectedIdx, 1);
    saveUserSnippets(snippets);
    clearForm();
  });

  newSaveBtn.addEventListener('click', () => {
    let trigger = triggerEl.value.trim();
    const desc  = descEl.value.trim();
    const body  = bodyEl.value;
    if (!trigger || !body.trim()) {
      alert('Tetikleyici ve kod boş olamaz.');
      return;
    }
    if (!trigger.startsWith('!')) trigger = '!' + trigger;
    const entry = { trigger, desc, body };
    if (selectedIdx >= 0 && selectedIdx < snippets.length) {
      snippets[selectedIdx] = entry;
    } else {
      snippets.push(entry);
      selectedIdx = snippets.length - 1;
    }
    saveUserSnippets(snippets);
    renderList();
    showStatus(`Snippet kaydedildi: ${trigger}`);
  });

  newCloseBtn.addEventListener('click', () => overlay.classList.add('hidden'));
  // overlay tıklama: önceki listener'ı temizlemek için _overlayClose referansını kullan
  if (overlay._overlayClose) overlay.removeEventListener('click', overlay._overlayClose);
  overlay._overlayClose = (e) => { if (e.target === overlay) overlay.classList.add('hidden'); };
  overlay.addEventListener('click', overlay._overlayClose);
}

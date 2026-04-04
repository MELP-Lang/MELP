// project-manager.js -- proje ac/kapat/kaydet, recent, autosave
// Yeni proje
async function handleNewProject() {
  if (designer.widgets.length > 0) {
    const result = await window.electronAPI.newProject();
    if (!result.confirmed) return;
  }

  designer.clear();
  _isDirty = false;
  _currentProjectName = 'Proje';
  updateWindowTitle();
  showStatus('Yeni proje oluşturuldu');
  if (typeof logDiagnostic === 'function') logDiagnostic('project', 'new-project');
}

// Proje aç
async function handleOpenProject() {
  try {
    const result = await window.electronAPI.openProject();

    if (result.success && result.data) {
      designer.loadFromJSON(result.data);
      _isDirty = false;
      if (result.filePath) {
        _currentProjectName = result.filePath.split('/').pop().replace('.mlpgui', '');
        addRecentProject(result.filePath);
      }
      updateWindowTitle();
      showStatus('Proje açıldı: ' + result.filePath);
      if (typeof logDiagnostic === 'function') {
        logDiagnostic('project', 'project-opened', {
          filePath: result.filePath,
          widgetCount: result.data?.widgets?.length || 0
        });
      }
    } else if (result.error) {
      alert('Proje açılırken hata: ' + result.error);
    }
  } catch (error) {
    console.error('Proje açma hatası:', error);
    if (typeof logDiagnostic === 'function') logDiagnostic('project', 'project-open-failed', { error: error.message }, 'error');
    alert('Proje açılırken bir hata oluştu:\n' + error.message + '\n\n(Detay için F12 konsola bakın)');
  }
}

// Proje kaydet
async function handleSaveProject() {
  try {
    const projectData = designer.toJSON();
    const result = await window.electronAPI.saveProject(projectData);

    if (result.success) {
      _isDirty = false;
      const fileName = result.filePath ? result.filePath.split('/').pop().replace('.mlpgui', '') : 'Proje';
      _currentProjectName = fileName;
      if (result.filePath) addRecentProject(result.filePath);
      updateWindowTitle();
      showStatus('Proje kaydedildi: ' + result.filePath);
      if (typeof logDiagnostic === 'function') {
        logDiagnostic('project', 'project-saved', {
          filePath: result.filePath,
          widgetCount: projectData?.widgets?.length || 0
        });
      }
    } else if (result.error) {
      alert('Proje kaydedilirken hata: ' + result.error);
    }
  } catch (error) {
    console.error('Proje kaydetme hatası:', error);
    if (typeof logDiagnostic === 'function') logDiagnostic('project', 'project-save-failed', { error: error.message }, 'error');
    alert('Proje kaydedilirken bir hata oluştu');
  }
}

// MLP kodunu dışa aktar
async function handleExportCode() {
  try {
    const mlpCode = designer.generateCode();
    const result = await window.electronAPI.exportMlpCode(mlpCode);

    if (result.success) {
      showStatus('MLP kodu dışa aktarıldı: ' + result.filePath);
    } else if (result.error) {
      alert('Kod dışa aktarılırken hata: ' + result.error);
    }
  } catch (error) {
    console.error('Kod dışa aktarma hatası:', error);
    alert('Kod dışa aktarılırken bir hata oluştu');
  }
}

// Uygulamayı standalone Electron app olarak dışa aktar
async function handleExportApp() {
  if (!window.electronAPI?.exportApp) {
    alert('Bu özellik yalnızca Electron ortamında kullanılabilir.');
    return;
  }
  if (!designer || designer.widgets.length === 0) {
    showStatus('Dışa aktarmak için önce Tasarım sekmesine widget ekleyin.');
    return;
  }

  const canvas = document.getElementById('canvas');
  const animationData = (designer.widgets || [])
    .filter(w => w.properties._anim && w.properties._anim.type !== 'none')
    .map(w => ({ id: w.id, anim: w.properties._anim }));

  // Event handler'ları designer'dan al
  const eventHandlers = [];
  if (designer.eventHandlers) {
    for (const [name, body] of Object.entries(designer.eventHandlers)) {
      eventHandlers.push({ name, body });
    }
  } else if (designer.currentProject?.eventHandlers) {
    designer.currentProject.eventHandlers.forEach(h => eventHandlers.push(h));
  }

  const appData = {
    projectName:    designer.canvasSettings?.title || 'melp-uygulama',
    canvasHtml:     canvas ? canvas.innerHTML : '',
    canvasSettings: designer.canvasSettings || {},
    animationData,
    eventHandlers,
    exportedAt:     new Date().toISOString(),
    exportNote:     'Electron/HTML tabanlı. Native masaüstü desteği MELP Beta\'da planlanmaktadır.'
  };

  try {
    showStatus('Dışa aktarılıyor...');
    const result = await window.electronAPI.exportApp(appData);
    if (result.success) {
      showStatus('✅ Uygulama dışa aktarıldı: ' + result.outDir);
    } else if (result.error) {
      showStatus('❌ Dışa aktarma hatası: ' + result.error);
    }
  } catch (err) {
    console.error('handleExportApp hatası:', err);
    showStatus('❌ Dışa aktarma başarısız');
  }
}

// Çalıştır (önizleme)

const RECENT_MAX = 5;
const RECENT_KEY = 'mlp-recent-projects';

function addRecentProject(filePath) {
  if (!filePath) return;
  try {
    let recent = JSON.parse(localStorage.getItem(RECENT_KEY) || '[]');
    recent = recent.filter(r => r.path !== filePath); // duplicate'i çıkar
    recent.unshift({ name: filePath.split('/').pop(), path: filePath });
    recent = recent.slice(0, RECENT_MAX);
    localStorage.setItem(RECENT_KEY, JSON.stringify(recent));
    renderRecentMenu();
  } catch(e) {}
}

function renderRecentMenu() {
  const container = document.getElementById('recent-projects-list');
  if (!container) return;
  try {
    const recent = JSON.parse(localStorage.getItem(RECENT_KEY) || '[]');
    container.innerHTML = '';
    if (recent.length === 0) {
      const empty = document.createElement('div');
      empty.className = 'menu-option';
      empty.style.cssText = 'opacity:0.5;cursor:default;font-style:italic;';
      empty.textContent = '  (son proje yok)';
      container.appendChild(empty);
      return;
    }
    recent.forEach(r => {
      const div = document.createElement('div');
      div.className = 'menu-option';
      div.textContent = '  ' + r.name;
      div.title = r.path;
      div.style.fontSize = '12px';
      div.addEventListener('click', () => openRecentProject(r.path));
      container.appendChild(div);
    });
  } catch(e) {}
}

async function openRecentProject(filePath) {
  try {
    const result = await window.electronAPI.openProjectFromPath(filePath);
    if (result.success && result.data) {
      designer.loadFromJSON(result.data);
      _isDirty = false;
      _currentProjectName = filePath.split('/').pop().replace('.mlpgui', '');
      addRecentProject(filePath); // öne taşı
      updateWindowTitle();
      showStatus('Proje açıldı: ' + filePath);
    } else {
      alert('Proje açılamadı: ' + (result.error || filePath + ' bulunamadı'));
    }
  } catch(e) {
    alert('Proje açılırken hata: ' + e.message);
  }
}


function restartAutosave() {
  clearInterval(_autosaveTimer);
  _autosaveTimer = null;
  if (_settings.autosaveEnabled) {
    const ms = (_settings.autosaveInterval || 60) * 1000;
    _autosaveTimer = setInterval(async () => {
      if (!_isDirty) return;
      await handleSaveProject();
      const badge = document.getElementById('autosave-badge');
      if (badge) {
        badge.textContent = '💾 Otomatik kaydedildi';
        badge.style.display = 'block';
        setTimeout(() => { badge.style.display = 'none'; }, 3000);
      }
    }, ms);
  }
}

/** Araçlar menüsündeki otomatik kaydet satırını toggle et */
function handleAutosaveToggle() {
  _settings.autosaveEnabled = !_settings.autosaveEnabled;
  restartAutosave();
  updateAutosaveMenu();
  showStatus(_settings.autosaveEnabled
    ? `Otomatik kaydetme aktif (${_settings.autosaveInterval || 60}s)`
    : 'Otomatik kaydetme devre dışı');
}

/** Araçlar menüsündeki autosave kaydının görünümünü güncelle */
function updateAutosaveMenu() {
  const item = document.getElementById('menu-autosave-toggle');
  if (!item) return;
  if (_settings.autosaveEnabled) {
    item.style.fontWeight = 'bold';
    item.textContent = `⏱️ Otomatik Kaydet (${_settings.autosaveInterval || 60}s) ✓`;
  } else {
    item.style.fontWeight = 'normal';
    item.textContent = '⏱️ Otomatik Kaydet';
  }
}

/** Başlangıçta son projeyi aç */
async function openLastProjectOnStartup() {
  try {
    const recent = JSON.parse(localStorage.getItem('mlp-recent-projects') || '[]');
    if (recent.length > 0 && recent[0].path) {
      await openRecentProject(recent[0].path);
    }
  } catch (e) {
    console.warn('Son proje açılamadı:', e);
  }
}

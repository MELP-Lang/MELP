const { contextBridge, ipcRenderer } = require('electron');

// API'yi renderer process'e güvenli şekilde expose et
contextBridge.exposeInMainWorld('electronAPI', {
  // Proje yönetimi
  saveProject: (projectData) => ipcRenderer.invoke('save-project', projectData),
  openProject: () => ipcRenderer.invoke('open-project'),
  openProjectFromPath: (filePath) => ipcRenderer.invoke('open-project-path', filePath),
  newProject: () => ipcRenderer.invoke('new-project'),

  // MLP kod dışa aktarma
  exportMlpCode: (mlpCode) => ipcRenderer.invoke('export-mlp-code', mlpCode),

  // Terminal
  terminalExec: (command, cwd) => ipcRenderer.send('terminal-exec', { command, cwd }),
  terminalKill: () => ipcRenderer.send('terminal-kill'),
  onTerminalOutput: (cb) => {
    const listener = (_, data) => cb(data);
    ipcRenderer.on('terminal-out', listener);
    return () => ipcRenderer.removeListener('terminal-out', listener);
  },

  // Geçici dosya yaz
  writeTempFile: (filePath, content) => ipcRenderer.invoke('write-temp-file', { filePath, content }),

  // Sistem bilgisi (user@host)
  getSystemInfo: () => ipcRenderer.invoke('get-system-info'),

  // Tanılama logları
  logDiagnostic: (entry) => ipcRenderer.invoke('log-diagnostic', entry),
  getLogStatus: () => ipcRenderer.invoke('get-log-status'),
  openLogFolder: (targetPath) => ipcRenderer.invoke('open-log-folder', targetPath),
  getMemoryStats: () => ipcRenderer.invoke('get-memory-stats'),

  // Kapat öncesi kaydedilmemiş değişiklik sorgusu
  onBeforeClose: (cb) => {
    const listener = () => cb();
    ipcRenderer.on('before-close-check', listener);
    return () => ipcRenderer.removeListener('before-close-check', listener);
  },
  respondClose: (dirty, projectName) => ipcRenderer.send('close-response', { dirty, projectName }),

  // Kaydet-sonra-kapat
  onSaveThenClose: (cb) => {
    const listener = () => cb();
    ipcRenderer.on('save-then-close', listener);
    return () => ipcRenderer.removeListener('save-then-close', listener);
  },
  saveThenCloseDone: () => ipcRenderer.send('save-then-close-done'),

  // Ayarlar
  saveSettings: (settings) => ipcRenderer.invoke('save-settings', settings),
  browseFile:   (opts)     => ipcRenderer.invoke('browse-file', opts),
  browseDirectory: (opts)  => ipcRenderer.invoke('browse-directory', opts),

  // Şablon dosyaları
  saveTemplate:   (data) => ipcRenderer.invoke('save-template', data),
  importTemplate: ()     => ipcRenderer.invoke('import-template'),

  // WASM çalıştır (gelecek kullanım için)
  runWasm: (wasmBytes) => ipcRenderer.invoke('run-wasm', { wasmBytes }),

  // Form önizleme penceresi
  openFormPreview: (formData) => ipcRenderer.invoke('open-form-preview', formData),

  // Uygulamayı standalone Electron app olarak dışa aktar
  exportApp: (appData) => ipcRenderer.invoke('export-app', appData)
});

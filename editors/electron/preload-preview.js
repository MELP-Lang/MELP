const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('previewAPI', {
  getFormData: () => ipcRenderer.invoke('get-preview-form-data')
});

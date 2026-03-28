# ELECTRON_IDE - Özellik Kartı

**Özellik:** Electron-based IDE  
**Durum:** 🔴 TODO (Phase 4.5)  
**Sözdizimi Sürümü:** N/A (Desktop app)

---

## 🎯 ÖZELLIKLER

### Core Features
- **Editor**: Monaco-based code editor
- **File Explorer**: Project tree view
- **Terminal**: Integrated terminal
- **Debugger**: Visual debugging interface

### UI Components
```typescript
// Main Window
ElectronWindow {
  Editor: MonacoEditor,
  Sidebar: FileExplorer,
  Panel: {
    Terminal: IntegratedTerminal,
    Debug: DebugPanel,
    Output: OutputPanel
  },
  StatusBar: LanguageStatus
}
```

### Shortcuts
- `Ctrl+B`: Toggle sidebar
- `Ctrl+J`: Toggle panel
- `Ctrl+P`: Quick file open
- `F5`: Start debugging
- `Ctrl+Shift+P`: Command palette

---

**Phase:** 4.5 (Month 7)

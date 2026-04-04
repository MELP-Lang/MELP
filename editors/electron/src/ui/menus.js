// UI event listener'larını başlat
function initializeUI() {
  const codeEditor = document.getElementById('code-editor');

  const tabDesign  = document.getElementById('tab-design');
  const tabCode    = document.getElementById('tab-code');
  const tabSource  = document.getElementById('tab-source');
  const tabPreview = document.getElementById('tab-preview');
  const tabLogs    = document.getElementById('tab-logs');
  const viewDesign  = document.getElementById('view-design');
  const viewCode    = document.getElementById('view-code');
  const viewSource  = document.getElementById('view-source');
  const viewPreview = document.getElementById('view-preview');
  const viewLogs    = document.getElementById('view-logs');

  function activateTab(active, views) {
    [tabDesign, tabCode, tabSource, tabPreview, tabLogs].forEach(t => t?.classList.remove('active'));
    [viewDesign, viewCode, viewSource, viewPreview, viewLogs].forEach(v => v?.classList.add('hidden'));
    active.classList.add('active');
    views.forEach(v => v?.classList.remove('hidden'));
  }

  tabDesign.addEventListener('click', () => {
    activateTab(tabDesign, [viewDesign]);
    showStatus('Tasarım');
  });

  tabCode.addEventListener('click', () => {
    activateTab(tabCode, [viewCode]);
    syncEventCode();
    showStatus('Kod');
    // İmleçi ilk event gövdesinin içine (boş satıra) konumlandır
    requestAnimationFrame(() => {
      const el = document.getElementById('code-editor');
      if (!el) return;
      const text = el.textContent;
      const evtIdx = text.indexOf('\nevent ');
      if (evtIdx === -1) { el.focus(); return; }
      const eol = text.indexOf('\n', evtIdx + 1); // event satırının sonu
      const cursorPos = eol !== -1 ? eol + 1 : text.length; // bir sonraki satır başı (boş satır)
      el.focus();
      const textNode = el.firstChild;
      if (textNode && textNode.nodeType === Node.TEXT_NODE && cursorPos <= textNode.length) {
        const range = document.createRange();
        range.setStart(textNode, cursorPos);
        range.collapse(true);
        const sel = window.getSelection();
        sel.removeAllRanges();
        sel.addRange(range);
      }
    });
  });

  tabSource.addEventListener('click', () => {
    activateTab(tabSource, [viewSource]);
    updateSourceView();
    showStatus('Kaynak');
  });

  tabPreview.addEventListener('click', () => {
    activateTab(tabPreview, [viewPreview]);
    // Aktif preset'in boyutunu uygula (canvasSettings ile senkron)
    if (typeof _applyPreviewSize === 'function') {
      const activeBtn = document.querySelector('.preview-preset-btn.active');
      if (activeBtn) {
        const w = activeBtn.dataset.w;
        const h = activeBtn.dataset.h;
        _applyPreviewSize(w === 'canvas' ? 'canvas' : { w: parseInt(w), h: parseInt(h) });
      } else {
        updatePreviewView();
      }
    } else {
      updatePreviewView();
    }
    showStatus('Görünüm');
  });

  tabLogs.addEventListener('click', () => {
    activateTab(tabLogs, [viewLogs]);
    if (typeof renderMemoryLogView === 'function') renderMemoryLogView();
    showStatus('Log');
  });

  // Dosya menüsü
  document.getElementById('menu-new').addEventListener('click', handleNewProject);
  document.getElementById('menu-open').addEventListener('click', handleOpenProject);
  document.getElementById('menu-save').addEventListener('click', handleSaveProject);
  document.getElementById('menu-export').addEventListener('click', handleExportCode);
  document.getElementById('menu-export-app').addEventListener('click', handleExportApp);

  // Düzenle menüsü
  document.getElementById('menu-undo').addEventListener('click', handleUndo);
  document.getElementById('menu-redo').addEventListener('click', handleRedo);
  document.getElementById('menu-cut').addEventListener('click', handleCut);
  document.getElementById('menu-copy').addEventListener('click', handleCopy);
  document.getElementById('menu-paste').addEventListener('click', handlePaste);
  document.getElementById('menu-delete').addEventListener('click', handleDelete);
  document.getElementById('menu-select-all').addEventListener('click', handleSelectAll);
  document.getElementById('menu-group').addEventListener('click', handleGroup);
  document.getElementById('menu-ungroup').addEventListener('click', handleUngroup);

  // Görünüm menüsü
  document.getElementById('menu-theme-select').addEventListener('click', handleThemeSelect);
  document.getElementById('menu-toggle-toolbox').addEventListener('click', handleToggleToolbox);
  document.getElementById('menu-toggle-properties').addEventListener('click', handleToggleProperties);
  document.getElementById('menu-zoom-in').addEventListener('click', handleZoomIn);
  document.getElementById('menu-zoom-out').addEventListener('click', handleZoomOut);
  document.getElementById('menu-zoom-reset').addEventListener('click', handleZoomReset);
  document.getElementById('menu-toggle-grid').addEventListener('click', handleToggleGrid);
  document.getElementById('menu-snap-grid').addEventListener('click', handleSnapGrid);
  document.getElementById('menu-align-left').addEventListener('click', () => handleAlign('left'));
  document.getElementById('menu-align-right').addEventListener('click', () => handleAlign('right'));
  document.getElementById('menu-align-top').addEventListener('click', () => handleAlign('top'));
  document.getElementById('menu-align-bottom').addEventListener('click', () => handleAlign('bottom'));
  document.getElementById('menu-align-center-h').addEventListener('click', () => handleAlign('center-h'));
  document.getElementById('menu-align-center-v').addEventListener('click', () => handleAlign('center-v'));
  document.getElementById('menu-distribute-h').addEventListener('click', () => handleAlign('distribute-h'));
  document.getElementById('menu-distribute-v').addEventListener('click', () => handleAlign('distribute-v'));
  document.getElementById('menu-arrange-flow').addEventListener('click', () => handleSmartArrange('flow'));
  document.getElementById('menu-arrange-grid').addEventListener('click', () => handleSmartArrange('grid'));
  document.getElementById('menu-arrange-stack-v').addEventListener('click', () => handleSmartArrange('stack-v'));
  document.getElementById('menu-arrange-stack-h').addEventListener('click', () => handleSmartArrange('stack-h'));
  document.getElementById('menu-toggle-terminal').addEventListener('click', () => toggleBottomTerminal());

  // Araçlar menüsü
  document.getElementById('menu-settings').addEventListener('click', handleSettings);
  document.getElementById('menu-autosave-toggle').addEventListener('click', handleAutosaveToggle);
  document.getElementById('menu-templates').addEventListener('click', handleTemplates);
  document.getElementById('menu-save-template').addEventListener('click', handleSaveTemplate);
  document.getElementById('menu-import-template').addEventListener('click', handleImportTemplate);
  document.getElementById('menu-snippet-manager').addEventListener('click', handleSnippetManager);
  document.getElementById('menu-create-custom-widget').addEventListener('click', () => handleCreateCustomWidget());
  document.getElementById('menu-custom-widget-manager').addEventListener('click', handleCustomWidgetManager);

  // Yardım menüsü
  document.getElementById('menu-quickstart').addEventListener('click', handleQuickstart);
  document.getElementById('menu-keyboard-shortcuts').addEventListener('click', handleKeyboardShortcuts);
  document.getElementById('menu-mlp-functions').addEventListener('click', handleMlpFunctions);
  document.getElementById('menu-documentation').addEventListener('click', handleDocumentation);
  document.getElementById('menu-mlp-docs').addEventListener('click', handleDocumentation);
  document.getElementById('menu-about').addEventListener('click', handleAbout);

  // Toolbar butonları
  document.getElementById('btn-new').addEventListener('click', handleNewProject);
  document.getElementById('btn-open').addEventListener('click', handleOpenProject);
  document.getElementById('btn-save').addEventListener('click', handleSaveProject);
  document.getElementById('btn-export').addEventListener('click', handleExportCode);
  document.getElementById('btn-run').addEventListener('click', handleRun);

  // Hizalama toolbar butonları
  document.getElementById('btn-align-left').addEventListener('click', () => handleAlign('left'));
  document.getElementById('btn-align-center-h').addEventListener('click', () => handleAlign('center-h'));
  document.getElementById('btn-align-right').addEventListener('click', () => handleAlign('right'));
  document.getElementById('btn-align-top').addEventListener('click', () => handleAlign('top'));
  document.getElementById('btn-align-center-v').addEventListener('click', () => handleAlign('center-v'));
  document.getElementById('btn-align-bottom').addEventListener('click', () => handleAlign('bottom'));
  document.getElementById('btn-distribute-h').addEventListener('click', () => handleAlign('distribute-h'));
  document.getElementById('btn-distribute-v').addEventListener('click', () => handleAlign('distribute-v'));

  // Dil ve Syntax seçiciler
  document.getElementById('language-selector').addEventListener('change', handleLanguageChange);
  document.getElementById('syntax-selector').addEventListener('change', handleSyntaxChange);

  // Code editor'da yazı yazılınca validation yap
  codeEditor.addEventListener('input', () => {
    // Satır numaralarını hemen güncelle
    updateLineNumbers();

    // Kısa bir gecikme ile validate et (her tuş vuruşunda değil)
    clearTimeout(window.validationTimeout);
    window.validationTimeout = setTimeout(() => {
      validateAndHighlightCode();
    }, 500); // 500ms bekle

    // FAZ 2.1: WASM as-you-type lint (800ms debounce)
    _scheduleWasmLint(codeEditor.value);
  });

  // IntelliSense editor'ı ayarla
  intelliSense.setEditor(codeEditor);
  // Code editor'da IntelliSense için klavye event'leri
  codeEditor.addEventListener('keydown', (e) => {
    // Ctrl+Space - IntelliSense'i manuel olarak aç
    if (e.ctrlKey && e.code === 'Space') {
      e.preventDefault();
      const pos = intelliSense.getCursorPosition(codeEditor);
      intelliSense.show(codeEditor, pos);
      return;
    }

    // IntelliSense açıksa
    if (intelliSense.isVisible) {
      switch (e.key) {
        case 'ArrowDown':
          e.preventDefault();
          intelliSense.selectNext();
          break;
        case 'ArrowUp':
          e.preventDefault();
          intelliSense.selectPrevious();
          break;
        case 'Enter':
        case 'Tab':
          e.preventDefault();
          intelliSense.insertSelected();
          break;
        case 'Escape':
          e.preventDefault();
          intelliSense.cancel();
          break;
      }
    }
  });

  // Code editor'da yazarken otomatik IntelliSense
  codeEditor.addEventListener('keyup', (e) => {
    // Özel tuşları atla
    if (['ArrowDown', 'ArrowUp', 'Enter', 'Tab', 'Escape', 'Shift', 'Control', 'Alt'].includes(e.key)) {
      return;
    }



    if (e.key === '.') {
      // Nokta tetikleyicisi — widget_X. yazınca property önerileri
      const pos = intelliSense.getCursorPosition(codeEditor);
      const line = intelliSense.getLineText(pos.line);
      const textBefore = line.substring(0, pos.column);
      const dotMatch = textBefore.match(/(\w+)\.(\w*)$/);
      if (dotMatch) {
        const widgetId = dotMatch[1];
        const widget = designer && designer.widgets
          ? designer.widgets.find(w => w.id === widgetId)
          : null;
        if (widget) {
          intelliSense.showForDot(codeEditor, pos, widgetId, widget.type);
          return;
        }
      }
      intelliSense.hide();

    } else if (e.key.match(/^[a-zA-Z_0-9]$/)) {
      clearTimeout(window.intellisenseTimeout);
      window.intellisenseTimeout = setTimeout(() => {
        const pos = intelliSense.getCursorPosition(codeEditor);
        const line = intelliSense.getLineText(pos.line);
        const textBefore = line.substring(0, pos.column);

        // Nokta sonrası yazıyorsa widget property önerileri
        const dotMatch = textBefore.match(/(\w+)\.(\w*)$/);
        if (dotMatch) {
          const widgetId = dotMatch[1];
          const widget = designer && designer.widgets
            ? designer.widgets.find(w => w.id === widgetId)
            : null;
          if (widget) {
            intelliSense.showForDot(codeEditor, pos, widgetId, widget.type);
            return;
          }
        }

        // Normal kelime tamamlama
        const wordBefore = intelliSense.getWordBeforeCursor(line, pos.column);
        if (wordBefore.length >= 2) {
          intelliSense.show(codeEditor, pos);
        }
      }, 300);

    } else {
      // Boşluk, noktalama vs - IntelliSense'i kapat
      intelliSense.hide();
    }
  });

  // Editör dışına tıklanınca IntelliSense'i kapat
  // Editör dışına tıklanınca IntelliSense'i kapat
  document.addEventListener('click', (e) => {
    if (!intelliSense.isVisible) return;
    
    // Popup içindeki item'a tıklandıysa - zaten selectItem çağrılacak
    if (intelliSense.popup.contains(e.target)) {
      return;
    }
    
    // Editor'e tıklandıysa - açık kalsın
    if (codeEditor.contains(e.target) || e.target === codeEditor) {
      return;
    }
    
    // Dışarı tıklandı - kapat
    intelliSense.hide();
  });
}

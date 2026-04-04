// edit-ops.js — Undo/Redo, Pano işlemleri, Gruplama

// ==================== DÜZENLE MENÜSÜ ====================

// Undo/Redo için basit history sistemi
let undoStack = [];
let redoStack = [];
let _undoDebounceTimer = null;

// Debounceli undo kaydı — hızlı ard arda değişiklikte (drag, resize) gereksiz snapshot üretmesini önler
function saveToUndoStack() {
  clearTimeout(_undoDebounceTimer);
  _undoDebounceTimer = setTimeout(() => {
    if (designer && designer.widgets) {
      undoStack.push(JSON.stringify(designer.toJSON()));
      redoStack = []; // Yeni değişiklik → redo stack temizle
      if (undoStack.length > 30) { // Max 30 undo (bellek tasarrufu)
        undoStack.splice(0, undoStack.length - 30);
      }
    }
  }, 250); // 250 ms sonra kaydet — hızlı sürüklemede tek snapshot al
}

function handleUndo() {
  if (undoStack.length === 0) {
    showStatus('Geri alınacak işlem yok');
    return;
  }

  // Mevcut durumu redo stack'e kaydet
  redoStack.push(JSON.stringify(designer.toJSON()));

  // Son durumu geri yükle
  const prevState = undoStack.pop();
  designer.loadFromJSON(JSON.parse(prevState));
  showStatus('Geri alındı');
}

function handleRedo() {
  if (redoStack.length === 0) {
    showStatus('Yinelenecek işlem yok');
    return;
  }

  // Mevcut durumu undo stack'e kaydet
  undoStack.push(JSON.stringify(designer.toJSON()));

  // Redo durumunu geri yükle
  const nextState = redoStack.pop();
  designer.loadFromJSON(JSON.parse(nextState));
  showStatus('Yinelendi');
}

let clipboard = null;

function handleCut() {
  const selected = [...(designer.selectedWidgets || new Set())];
  if (selected.length === 0) { showStatus('Kesilecek widget seçilmedi'); return; }
  clipboard = JSON.stringify(selected.map(w => w.toJSON()));
  selected.forEach(w => designer.deleteWidget(w));
  saveToUndoStack();
  showStatus(selected.length > 1 ? `${selected.length} widget kesildi` : 'Widget kesildi');
}

function handleCopy() {
  const selected = [...(designer.selectedWidgets || new Set())];
  if (selected.length === 0) { showStatus('Kopyalanacak widget seçilmedi'); return; }
  clipboard = JSON.stringify(selected.map(w => w.toJSON()));
  showStatus(selected.length > 1 ? `${selected.length} widget kopyalandı` : 'Widget kopyalandı');
}

function handlePaste() {
  if (!clipboard) { showStatus('Yapıştırılacak widget yok'); return; }
  try {
    const raw  = JSON.parse(clipboard);
    const items = Array.isArray(raw) ? raw : [raw]; // eski tek-widget formatı uyumu
    designer.clearSelection();
    items.forEach(data => {
      // Yeni ID için Widget.fromJSON değil new Widget() kullan
      const newProps = { ...data.properties, x: (data.properties.x || 0) + 20, y: (data.properties.y || 0) + 20 };
      // Event handler'ları temizle (kopyalanan widget'ın handler'ları yeni isme taşınmasın)
      ['onClick','onChange','onSelect'].forEach(p => { if (p in newProps) newProps[p] = ''; });
      const widget = new Widget(data.type, newProps);
      designer.addWidget(widget);
      designer.addToSelection(widget);
    });
    saveToUndoStack();
    showStatus(items.length > 1 ? `${items.length} widget yapıştırıldı` : 'Widget yapıştırıldı');
  } catch (e) {
    showStatus('Yapıştırma hatası: ' + e.message);
  }
}

function handleDelete() {
  if (!designer.selectedWidgets || designer.selectedWidgets.size === 0) {
    showStatus('Silinecek widget seçilmedi');
    return;
  }
  [...designer.selectedWidgets].forEach(w => designer.deleteWidget(w));
}

function handleSelectAll() {
  designer.clearSelection();
  designer.widgets.forEach(w => designer.addToSelection(w));
  showStatus(`${designer.selectedWidgets.size} widget seçildi`);
}

// ==================== GRUPLAMA ====================

function handleGroup() {
  const result = designer.groupSelected();
  if (!result) {
    showStatus('Gruplamak için en az 2 widget seçin (Ctrl+Click)');
    return;
  }
  saveToUndoStack();
  showStatus(`${result.count} widget gruplandı (ID: ${result.id})`);
}

function handleUngroup() {
  const result = designer.ungroupSelected();
  if (!result) {
    showStatus('Seçili widget\'lar herhangi bir grubun içinde değil');
    return;
  }
  saveToUndoStack();
  showStatus(`Grup çözüldü`);
}

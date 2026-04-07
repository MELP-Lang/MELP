// Ana Designer Sınıfı - Drag & Drop ve Widget Yönetimi

class Designer {
  constructor() {
    this.widgets = [];
    this.selectedWidget = null;
    this.canvas = document.getElementById('canvas');
    this.propertiesPanel = new PropertiesPanel(
      document.getElementById('properties'),
      this.onPropertyChange.bind(this)
    );
    this.codeGenerator = new CodeGenerator();
    this.isDragging = false;
    this.isResizing = false;
    this.onWidgetChange = null; // widget eklenince/silinince tetiklenir
    this.onSelect = null;         // widget seçilince / seçim kaldırılınca tetiklenir
    this.dragStartX = 0;
    this.dragStartY = 0;
    this.resizeHandle = null;
    this.rafId = null; // RequestAnimationFrame ID

    // GPU-accelerated drag için geçici değerler
    this.currentMouseX = 0;
    this.currentMouseY = 0;
    this.initialWidgetX = 0;
    this.initialWidgetY = 0;

    // Çoklu seçim
    this.selectedWidgets = new Set();
    this.multiDragOffsets = new Map();

    // Snap-to-grid
    this.snapSize = 8; // app.js'den güncellenir

    // Canvas (pencere) ayarları
    this.canvasSettings = {
      title: 'Pencere',
      width: 800, height: 600,
      backgroundColor: '#1e1e1e',
      gridColor: '',   // boş = temaya göre otomatik
      gridSize: 20,    // px
      snapToGrid: true
    };
    this.applyCanvasSettings();

    // Widget Gruplama
    this.groups = []; // [ { id: 'group_123', widgetIds: [...] }, ... ]

    // Rubber band (kauçuk bant) seçimi
    this.isRubberBanding = false;
    this.rubberBandEl = null;
    this.rubberBandStartX = 0;
    this.rubberBandStartY = 0;
    this._loadToken = 0;

    this.initializeEventListeners();
  }

  // Event listener'ları başlat
  initializeEventListeners() {
    // Toolbox'tan sürükleme
    const toolboxItems = document.querySelectorAll('.widget-item');
    toolboxItems.forEach(item => {
      item.addEventListener('dragstart', this.onToolboxDragStart.bind(this));
      item.addEventListener('dblclick', this.onToolboxDblClick.bind(this));
    });

    // Canvas'a bırakma
    this.canvas.addEventListener('dragover', this.onCanvasDragOver.bind(this));
    this.canvas.addEventListener('drop', this.onCanvasDrop.bind(this));

    // Canvas'ta widget seçme ve sürükleme
    this.canvas.addEventListener('mousedown', this.onCanvasMouseDown.bind(this));
    document.addEventListener('mousemove', this.onDocumentMouseMove.bind(this));
    document.addEventListener('mouseup', this.onDocumentMouseUp.bind(this));

    // Rubber band (lasso) seçimi için overlay div
    this.rubberBandEl = document.createElement('div');
    this.rubberBandEl.className = 'rubber-band';
    this.rubberBandEl.style.display = 'none';
    this.canvas.appendChild(this.rubberBandEl);

    // Widget'a çift tıklama (kod görünümüne geç)
    this.canvas.addEventListener('dblclick', this.onCanvasDoubleClick.bind(this));

    // Delete tuşu ile silme
    document.addEventListener('keydown', this.onKeyDown.bind(this));
  }

  // Toolbox'tan sürükleme başladı
  onToolboxDragStart(e) {
    const itemEl = e.target.closest ? e.target.closest('.widget-item') : e.target;
    const widgetType = itemEl ? itemEl.dataset.widgetType : '';
    const customId   = itemEl ? (itemEl.dataset.customId || '') : '';
    e.dataTransfer.setData('widgetType', widgetType || '');
    if (customId) e.dataTransfer.setData('customWidgetId', customId);
    e.dataTransfer.effectAllowed = 'copy';
  }

  // Toolbox item'a çift tıklandı → canvas ortasına ekle
  onToolboxDblClick(e) {
    const itemEl = e.target.closest ? e.target.closest('.widget-item') : e.currentTarget;
    const widgetType = itemEl ? itemEl.dataset.widgetType : e.currentTarget.dataset.widgetType;
    if (!widgetType) return;

    // Özel widget varsayılanlarını yükle
    let customDefaults = {};
    const customId = itemEl ? (itemEl.dataset.customId || '') : '';
    if (customId && typeof loadCustomWidgets === 'function') {
      const cw = loadCustomWidgets().find(w => w.id === customId);
      if (cw) customDefaults = { ...cw.defaults };
    }

    const defaults = { ...getDefaultProperties(widgetType), ...customDefaults };

    // Canvas hidden olduğunda offsetWidth/Height = 0; parent veya fallback kullan
    let cw = this.canvas.offsetWidth;
    let ch = this.canvas.offsetHeight;
    if (!cw || !ch) {
      const parent = this.canvas.parentElement;
      cw = parent?.offsetWidth  || 800;
      ch = parent?.offsetHeight || 600;
    }

    const cx = Math.round(cw / 2) - Math.round((defaults.width  || 120) / 2);
    const cy = Math.round(ch / 2) - Math.round((defaults.height || 30)  / 2);
    const widget = new Widget(widgetType, { x: cx, y: cy, ...customDefaults });
    this.addWidget(widget);
    showStatus(`${getWidgetTypeName(widgetType)} merkeze eklendi`);
  }

  // Canvas üzerinde sürükleme
  onCanvasDragOver(e) {
    e.preventDefault();
    e.dataTransfer.dropEffect = 'copy';
  }

  // Canvas'a widget bırakıldı
  onCanvasDrop(e) {
    e.preventDefault();
    const widgetType = e.dataTransfer.getData('widgetType');
    if (!widgetType) return;

    // Canvas'a göre pozisyon hesapla
    const rect = this.canvas.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;

    // Özel widget varsayılanlarını yükle
    let customDefaults = {};
    const customId = e.dataTransfer.getData('customWidgetId');
    if (customId && typeof loadCustomWidgets === 'function') {
      const cw = loadCustomWidgets().find(w => w.id === customId);
      if (cw) customDefaults = { ...cw.defaults };
    }

    // Yeni widget oluştur
    const widget = new Widget(widgetType, { x, y, ...customDefaults });
    this.addWidget(widget);

    showStatus(`${getWidgetTypeName(widgetType)} eklendi`);
  }

  // Widget ekle
  addWidget(widget, options = {}) {
    const {
      select = true,
      saveHistory = true,
      notify = true
    } = options;

    // Drop anında event tipine göre otomatik handler adı ata
    const autoHandlers = {
      button:   { prop: 'onClick',   name: `on_${widget.id}_click`  },
      checkbox: { prop: 'onClick',   name: `on_${widget.id}_change` },
      radio:    { prop: 'onClick',   name: `on_${widget.id}_change` },
      textbox:  { prop: 'onChange',  name: `on_${widget.id}_change` },
      listbox:  { prop: 'onSelect',  name: `on_${widget.id}_select` },
      combobox: { prop: 'onSelect',  name: `on_${widget.id}_select` },
    };
    const auto = autoHandlers[widget.type];
    if (auto && !widget.properties[auto.prop]) {
      widget.properties[auto.prop] = auto.name;
    }

    this.widgets.push(widget);
    const element = widget.render();
    this.canvas.appendChild(element);

    // Canvas info'yu gizle
    const canvasInfo = this.canvas.querySelector('.canvas-info');
    if (canvasInfo && this.widgets.length > 0) {
      canvasInfo.style.display = 'none';
    }

    if (select) {
      this.selectWidget(widget);
    }
    updateWidgetCount(this.widgets.length);

    // Undo stack'e kaydet
    if (saveHistory && typeof saveToUndoStack === 'function') {
      saveToUndoStack();
    }

    // Tasarım kaynağını güncelle
    if (notify) {
      this.onWidgetChange?.();
    }
  }

  // Canvas'ta mouse basıldı
  onCanvasMouseDown(e) {
    // Resize handle'a mı tıklandı?
    if (e.target.classList.contains('resize-handle')) {
      this.isResizing = true;
      this.resizeHandle = e.target.dataset.position;
      this.dragStartX = e.clientX;
      this.dragStartY = e.clientY;
      e.preventDefault();
      return;
    }

    // Widget'a mı tıklandı?
    const widgetElement = e.target.closest('.canvas-widget');
    if (widgetElement) {
      const widgetId = widgetElement.dataset.widgetId;
      const widget = this.widgets.find(w => w.id === widgetId);
      if (widget) {
        if (e.ctrlKey || e.shiftKey) {
          // Ctrl/Shift+tıkla: bireysel widget ekle/çıkar (grup üyesi olsa bile)
          if (this.selectedWidgets.has(widget)) {
            this.removeFromSelection(widget);
          } else {
            this.addToSelection(widget);
          }
        } else {
          // Normal tıkla: widget seçili değilse tek seçim yap
          if (!this.selectedWidgets.has(widget)) {
            this.clearSelection();
            // Gruba ait widget'a normal tıklama → tüm grup üyelerini seç
            const group = this.getGroupForWidget(widget);
            if (group) {
              const groupWidgets = group.widgetIds
                .map(id => this.widgets.find(w => w.id === id))
                .filter(Boolean);
              groupWidgets.forEach(w => this.addToSelection(w));
            } else {
              this.addToSelection(widget);
            }
          }
          // Zaten seçiliyse ya da çoklu seçimdeyse — seçimi değiştirme, sadece sürükle
        }

        // Sürükleme hazırlığı (tüm seçili widget'lar için)
        this.isDragging = true;
        this.multiDragOffsets.clear();
        for (const w of this.selectedWidgets) {
          this.multiDragOffsets.set(w.id, { x: w.properties.x, y: w.properties.y });
          w.element.style.willChange = 'transform';
        }
        this.dragStartX = e.clientX;
        this.dragStartY = e.clientY;
        e.preventDefault();
      }
    } else {
      // Boş alana tıklandı
      if (!e.ctrlKey && !e.shiftKey) {
        this.clearSelection(true); // canvas özelliklerini göster
      }
      // Rubber band (lasso) seçimini başlat
      const rect = this.canvas.getBoundingClientRect();
      this.rubberBandStartX = e.clientX - rect.left;
      this.rubberBandStartY = e.clientY - rect.top;
      this.isRubberBanding = true;
      this.rubberBandEl.style.left   = this.rubberBandStartX + 'px';
      this.rubberBandEl.style.top    = this.rubberBandStartY + 'px';
      this.rubberBandEl.style.width  = '0';
      this.rubberBandEl.style.height = '0';
      this.rubberBandEl.style.display = 'block';
      e.preventDefault();
    }
  }

  // Mouse hareket etti
  onDocumentMouseMove(e) {
    if (!this.isDragging && !this.isResizing && !this.isRubberBanding) return;

    this.currentMouseX = e.clientX;
    this.currentMouseY = e.clientY;

    // Rubber band: throttle gerekmez, sadece div pozisyonu güncelle
    if (this.isRubberBanding) {
      const rect = this.canvas.getBoundingClientRect();
      const curX = e.clientX - rect.left;
      const curY = e.clientY - rect.top;
      const x = Math.min(curX, this.rubberBandStartX);
      const y = Math.min(curY, this.rubberBandStartY);
      const w = Math.abs(curX - this.rubberBandStartX);
      const h = Math.abs(curY - this.rubberBandStartY);
      this.rubberBandEl.style.left   = x + 'px';
      this.rubberBandEl.style.top    = y + 'px';
      this.rubberBandEl.style.width  = w + 'px';
      this.rubberBandEl.style.height = h + 'px';
      return;
    }

    // requestAnimationFrame ile throttle (60 FPS'e sınırla)
    if (this.rafId) return;

    this.rafId = requestAnimationFrame(() => {
      this.rafId = null;

      if (this.isDragging && this.selectedWidgets.size > 0) {
        const rect = this.canvas.getBoundingClientRect();
        const cursorDX = this.currentMouseX - this.dragStartX;
        const cursorDY = this.currentMouseY - this.dragStartY;

        for (const w of this.selectedWidgets) {
          const initial = this.multiDragOffsets.get(w.id);
          if (!initial) continue;

          let newX = initial.x + cursorDX;
          let newY = initial.y + cursorDY;
          // Snap-to-grid uygula
          if (this.snapSize > 1) {
            newX = Math.round(newX / this.snapSize) * this.snapSize;
            newY = Math.round(newY / this.snapSize) * this.snapSize;
          }
          newX = Math.max(0, Math.min(newX, rect.width  - w.properties.width));
          newY = Math.max(0, Math.min(newY, rect.height - w.properties.height));

          const dX = newX - initial.x;
          const dY = newY - initial.y;
          w.element.style.transform = `translate3d(${dX}px, ${dY}px, 0)`;
          w.properties.x = Math.round(newX);
          w.properties.y = Math.round(newY);
        }

      } else if (this.isResizing && this.selectedWidget) {
        // Widget'ı yeniden boyutlandır
        const deltaX = this.currentMouseX - this.dragStartX;
        const deltaY = this.currentMouseY - this.dragStartY;

        const props = this.selectedWidget.properties;
        let newWidth = props.width;
        let newHeight = props.height;
        let newX = props.x;
        let newY = props.y;

        switch (this.resizeHandle) {
          case 'se': // Güneydoğu (sağ alt)
            newWidth = Math.max(50, props.width + deltaX);
            newHeight = Math.max(30, props.height + deltaY);
            break;
          case 'sw': // Güneybatı (sol alt)
            newWidth = Math.max(50, props.width - deltaX);
            newHeight = Math.max(30, props.height + deltaY);
            newX = props.x + (props.width - newWidth);
            break;
          case 'ne': // Kuzeydoğu (sağ üst)
            newWidth = Math.max(50, props.width + deltaX);
            newHeight = Math.max(30, props.height - deltaY);
            newY = props.y + (props.height - newHeight);
            break;
          case 'nw': // Kuzeybatı (sol üst)
            newWidth = Math.max(50, props.width - deltaX);
            newHeight = Math.max(30, props.height - deltaY);
            newX = props.x + (props.width - newWidth);
            newY = props.y + (props.height - newHeight);
            break;
        }

        this.selectedWidget.update({ x: newX, y: newY, width: newWidth, height: newHeight });
        this.dragStartX = this.currentMouseX;
        this.dragStartY = this.currentMouseY;
        this.propertiesPanel.showProperties(this.selectedWidget);
      }
    });
  }

  // Mouse bırakıldı
  onDocumentMouseUp(e) {
    // Rubber band bitti: hangi widget'lar içinde?
    if (this.isRubberBanding) {
      this.isRubberBanding = false;
      this.rubberBandEl.style.display = 'none';

      const rbX = parseFloat(this.rubberBandEl.style.left);
      const rbY = parseFloat(this.rubberBandEl.style.top);
      const rbW = parseFloat(this.rubberBandEl.style.width);
      const rbH = parseFloat(this.rubberBandEl.style.height);

      if (rbW > 3 && rbH > 3) {
        for (const w of this.widgets) {
          const wx = w.properties.x, wy = w.properties.y;
          const ww = w.properties.width, wh = w.properties.height;
          // Rubber band ile kesişiyor mu?
          if (wx < rbX + rbW && wx + ww > rbX && wy < rbY + rbH && wy + wh > rbY) {
            this.addToSelection(w);
          }
        }
        // Grup farkındalığı: seçime giren herhangi bir grup üyesi varsa tüm grubu ekle
        const groupsToExpand = new Set();
        for (const w of this.selectedWidgets) {
          const g = this.getGroupForWidget(w);
          if (g) groupsToExpand.add(g);
        }
        for (const group of groupsToExpand) {
          group.widgetIds
            .map(id => this.widgets.find(w => w.id === id))
            .filter(Boolean)
            .forEach(w => this.addToSelection(w));
        }
      }
      return;
    }

    // Drag bitti: tüm seçili widget'ların transform'unu temizle ve pozisyonu kaydet
    if (this.isDragging && this.selectedWidgets.size > 0) {
      // Bekleyen rAF varsa iptal et — son mouse konumundan final pozisyonu burada hesapla
      if (this.rafId) {
        cancelAnimationFrame(this.rafId);
        this.rafId = null;
        const rect = this.canvas.getBoundingClientRect();
        const cursorDX = this.currentMouseX - this.dragStartX;
        const cursorDY = this.currentMouseY - this.dragStartY;
        for (const w of this.selectedWidgets) {
          const initial = this.multiDragOffsets.get(w.id);
          if (!initial) continue;
          let newX = Math.round(Math.max(0, Math.min(initial.x + cursorDX, rect.width  - w.properties.width)));
          let newY = Math.round(Math.max(0, Math.min(initial.y + cursorDY, rect.height - w.properties.height)));
          w.properties.x = newX;
          w.properties.y = newY;
        }
      }
      for (const w of this.selectedWidgets) {
        w.element.style.transform  = '';
        w.element.style.willChange = 'auto';
        w.element.style.left       = w.properties.x + 'px';
        w.element.style.top        = w.properties.y + 'px';
      }
      if (this.selectedWidget) {
        this.propertiesPanel.showProperties(this.selectedWidget);
      }
    }

    const wasDraggingOrResizing = this.isDragging || this.isResizing;
    this.isDragging = false;
    this.isResizing = false;
    this.resizeHandle = null;
    // Sürükleme/yeniden boyutlandırma bittiğinde KAYNAK güncelle + undo stack'e kaydet
    if (wasDraggingOrResizing) {
      if (typeof saveToUndoStack === 'function') saveToUndoStack();
      this.onWidgetChange?.();
    }
  }

  // Çift tıklama (kod görünümüne geç)
  onCanvasDoubleClick(e) {
    const widgetElement = e.target.closest('.canvas-widget');
    if (widgetElement) {
      const codeTab = document.getElementById('tab-code');
      if (codeTab) {
        codeTab.click();
        showStatus('Kod görünümüne geçildi');
      }
    }
  }

  // Klavye olayları
  onKeyDown(e) {
    const activeTag = document.activeElement?.tagName?.toUpperCase();
    const isInput = activeTag === 'INPUT' || activeTag === 'TEXTAREA' || activeTag === 'SELECT';
    if (isInput) return;

    // Delete: tüm seçili widget'ları sil
    if (e.key === 'Delete' && this.selectedWidgets.size > 0) {
      [...this.selectedWidgets].forEach(w => this.deleteWidget(w));
      e.preventDefault();
    }

    // Ctrl+A: tümünü seç
    if (e.ctrlKey && e.key === 'a') {
      e.preventDefault();
      this.clearSelection();
      this.widgets.forEach(w => this.addToSelection(w));
    }
  }

  // ── Seçim yönetimi ──────────────────────────────────────────

  // Tek widget seç (önceki seçimi temizler) — geriye uyumluluk için korunur
  selectWidget(widget) {
    this.clearSelection();
    if (widget) this.addToSelection(widget);
  }

  // Tüm seçimi temizle
  clearSelection(showCanvasProps = false) {
    for (const w of this.selectedWidgets) {
      if (w.element) w.element.classList.remove('selected');
    }
    this.selectedWidgets.clear();
    this.selectedWidget = null;
    this.onSelect?.(null);
    if (showCanvasProps) {
      this.propertiesPanel.showCanvasProperties(
        this.canvasSettings,
        (key, value) => this.onCanvasSettingsChange(key, value)
      );
    } else {
      this.propertiesPanel.clear();
    }
  }

  // Canvas/pencere ayarı değişti
  onCanvasSettingsChange(key, value) {
    this.canvasSettings[key] = value;
    this.applyCanvasSettings();
    // Snap değişince designer'a bildir
    if (key === 'snapToGrid' || key === 'gridSize') {
      const snap = this.canvasSettings.snapToGrid;
      const size = this.canvasSettings.gridSize || 8;
      this.snapSize = snap ? size : 1;
      // app.js menü öğesini de güncelle
      const menuSnap = document.getElementById('menu-snap-grid');
      if (menuSnap) menuSnap.style.fontWeight = snap ? 'bold' : 'normal';
    }
    this.onWidgetChange?.();
  }

  // Canvas ayarlarını DOM'a uygula
  applyCanvasSettings() {
    const s = this.canvasSettings;
    this.canvas.style.width           = s.width  + 'px';
    this.canvas.style.minHeight       = s.height + 'px';
    this.canvas.style.backgroundColor = s.backgroundColor || '';
    // Izgara
    const size = s.gridSize || 20;
    this.canvas.style.backgroundSize = `${size}px ${size}px`;
    if (s.gridColor) {
      this.canvas.style.backgroundImage =
        `linear-gradient(${s.gridColor} 1px, transparent 1px), ` +
        `linear-gradient(90deg, ${s.gridColor} 1px, transparent 1px)`;
    }
    // Snap
    this.snapSize = s.snapToGrid ? (size || 8) : 1;
  }

  // Seçime widget ekle
  addToSelection(widget) {
    this.selectedWidgets.add(widget);
    this.selectedWidget = widget; // özellikler paneli için son seçilen
    if (widget.element) widget.element.classList.add('selected');
    if (this.selectedWidgets.size === 1) {
      this.propertiesPanel.showProperties(widget);
      this.onSelect?.(widget);
    } else {
      // Çoklu seçimde özellikler panelini temizle
      this.propertiesPanel.clear();
      this.onSelect?.(null);
    }
  }

  // Seçimden widget çıkar
  removeFromSelection(widget) {
    this.selectedWidgets.delete(widget);
    if (widget.element) widget.element.classList.remove('selected');
    if (this.selectedWidget === widget) {
      const remaining = [...this.selectedWidgets];
      this.selectedWidget = remaining.length > 0 ? remaining[remaining.length - 1] : null;
      if (this.selectedWidget) this.propertiesPanel.showProperties(this.selectedWidget);
      else this.propertiesPanel.clear();
    }
  }

  // Widget özelliği değişti
  onPropertyChange(widget, property, value) {
    this.updateWidgetProperty(widget, property, value);
    // Her özellik değişimi KAYNAK'ı etkiler; event prop → KOD'a stub ekle
    this.onWidgetChange?.();
  }

  // Widget özelliğini güncelle
  updateWidgetProperty(widget, property, value) {
    const updates = { [property]: value };
    widget.update(updates);

    // Properties panel'i güncelle (pozisyon değişirse sayılar güncellensin)
    if (this.selectedWidget === widget) {
      this.propertiesPanel.showProperties(widget);
    }
  }
  // ── Gruplama ────────────────────────────────────────────────────────────────────

  // Seçili widget'ları grupla
  groupSelected() {
    if (this.selectedWidgets.size < 2) return null;
    const widgetIds = [...this.selectedWidgets].map(w => w.id);
    const groupId   = 'g' + Date.now();
    // İçindeki widget'ları mevcut gruplardan çıkar
    this.groups = this.groups.filter(g => !g.widgetIds.some(id => widgetIds.includes(id)));
    this.groups.push({ id: groupId, widgetIds });
    return { id: groupId, count: widgetIds.length };
  }

  // Seçili widget'ların bulunduğu grupları çöz
  ungroupSelected() {
    if (this.selectedWidgets.size === 0) return null;
    const selIds = [...this.selectedWidgets].map(w => w.id);
    let dissolved = 0;
    this.groups = this.groups.filter(g => {
      if (g.widgetIds.some(id => selIds.includes(id))) { dissolved++; return false; }
      return true;
    });
    return dissolved > 0 ? { dissolved } : null;
  }

  // Widget hangi gruba ait?
  getGroupForWidget(widget) {
    return this.groups.find(g => g.widgetIds.includes(widget.id)) || null;
  }
  // Widget sil
  deleteWidget(widget) {
    const index = this.widgets.indexOf(widget);
    if (index > -1) {
      this.widgets.splice(index, 1);
      if (widget.element && widget.element.parentNode) {
        widget.element.parentNode.removeChild(widget.element);
      }
      this.selectedWidgets.delete(widget);
      if (this.selectedWidget === widget) {
        const remaining = [...this.selectedWidgets];
        this.selectedWidget = remaining.length > 0 ? remaining[remaining.length - 1] : null;
        if (this.selectedWidget) this.propertiesPanel.showProperties(this.selectedWidget);
        else this.propertiesPanel.clear();
      }
      updateWidgetCount(this.widgets.length);
      showStatus(`${getWidgetTypeName(widget.type)} silindi`);

      // Tasarım kaynağını güncelle
      this.onWidgetChange?.();

      // Eğer widget kalmadıysa info'yu göster
      if (this.widgets.length === 0) {
        const canvasInfo = this.canvas.querySelector('.canvas-info');
        if (canvasInfo) {
          canvasInfo.style.display = 'block';
        }
      }
    }
  }

  // MLP kodu üret (tam — geriye uyumluluk)
  generateCode() {
    return this.codeGenerator.generate(this.widgets);
  }

  // Sadece tasarım kaynağı (app_start bloğu)
  generateDesignSource() {
    return this.codeGenerator.generateDesignSource(this.widgets);
  }

  // Sadece event handler'lar
  generateEventCode() {
    return this.codeGenerator.generateEventCode(this.widgets);
  }

  // Projeyi JSON'a dönüştür
  toJSON() {
    return {
      version: '1.0',
      canvasSettings: { ...this.canvasSettings },
      widgets: this.widgets.map(w => w.toJSON()),
      groups: this.groups.length > 0 ? this.groups : undefined
    };
  }

  // JSON'dan proje yükle — geniş projeler UI'yi dondurmaz (batch loading)
  loadFromJSON(data) {
    const loadToken = ++this._loadToken;
    this.clear(false);
    // Canvas ayarlarını yükle
    if (data.canvasSettings) {
      // Yeni format: { canvasSettings: { title, width, height, ... } }
      this.canvasSettings = { ...this.canvasSettings, ...data.canvasSettings };
      this.applyCanvasSettings();
    } else if (data.windowTitle || data.windowWidth || data.windowHeight) {
      // Eski format: windowTitle/Width/Height doğrudan üst düzeyde
      this.canvasSettings = {
        ...this.canvasSettings,
        title:  data.windowTitle  || this.canvasSettings.title,
        width:  data.windowWidth  || this.canvasSettings.width,
        height: data.windowHeight || this.canvasSettings.height
      };
      this.applyCanvasSettings();
    }
    // Grupları yükle
    this.groups = data.groups || [];
    if (!data.widgets || data.widgets.length === 0) {
      this.onWidgetChange?.();
      showStatus('Proje yüklendi');
      return;
    }

    const BATCH = 30; // Her adımda en fazla 30 widget
    const all = data.widgets.slice();
    const total = all.length;

    // Az sayıda widget için senkron yükle (undo/redo performansı)
    if (total <= BATCH) {
      all.forEach((widgetData, i) => {
        try {
          const widget = Widget.fromJSON(widgetData);
          this.addWidget(widget, { select: false, saveHistory: false, notify: false });
        } catch (err) {
          console.error(`Widget[${i}] yüklenemedi (id=${widgetData?.id}, type=${widgetData?.type}):`, err);
          throw err; // yukarıya ilet
        }
      });
      this.onWidgetChange?.();
      showStatus('Proje yüklendi');
      return;
    }

    // Çok sayıda widget için pencereyi dondurmadan async yükle
    const loadBatch = () => {
      if (loadToken !== this._loadToken) return;

      const chunk = all.splice(0, BATCH);
      chunk.forEach(widgetData => {
        const widget = Widget.fromJSON(widgetData);
        this.addWidget(widget, { select: false, saveHistory: false, notify: false });
      });

      if (all.length > 0) {
        showStatus(`Yükleniyor… ${total - all.length} / ${total}`);
        setTimeout(loadBatch, 0); // Sonraki batch için event loop'a bırak
      } else {
        this.onWidgetChange?.();
        showStatus('Proje yüklendi');
      }
    };
    loadBatch();
  }

  // Tümünü temizle
  clear(cancelPendingLoad = true) {
    if (cancelPendingLoad) {
      this._loadToken++;
    }

    this.widgets = [];
    this.groups  = [];
    this.clearSelection();
    clearElement(this.canvas);

    if (!this.rubberBandEl) {
      this.rubberBandEl = document.createElement('div');
      this.rubberBandEl.className = 'rubber-band';
    }
    this.rubberBandEl.style.display = 'none';
    this.canvas.appendChild(this.rubberBandEl);

    // Canvas info'yu ekle
    const canvasInfo = document.createElement('div');
    canvasInfo.className = 'canvas-info';
    canvasInfo.textContent = "Widget'ları buraya sürükleyin";
    this.canvas.appendChild(canvasInfo);

    this.propertiesPanel.clear();
    updateWidgetCount(0);
    widgetIdCounter = 1;
  }
}

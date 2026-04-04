// Özellikler Paneli Yönetimi

class PropertiesPanel {
  constructor(container, onPropertyChange) {
    this.container = container;
    this.onPropertyChange = onPropertyChange;
    this.currentWidget = null;
  }

  // Widget seçildiğinde özellikleri göster
  showProperties(widget) {
    this.currentWidget = widget;
    clearElement(this.container);

    if (!widget) {
      const noSelection = document.createElement('div');
      noSelection.className = 'no-selection';
      noSelection.textContent = 'Bir widget seçin';
      this.container.appendChild(noSelection);
      return;
    }

    // Widget bilgi başlığı
    const header = document.createElement('div');
    header.className = 'prop-widget-header';
    header.style.cssText = 'padding: 8px; background-color: #2d2d30; margin-bottom: 12px; border-radius: 3px;';
    header.innerHTML = `
      <strong style="color: #cccccc;">${getWidgetTypeName(widget.type)}</strong>
      <div style="margin-top: 6px; display: flex; align-items: center; gap: 4px;">
        <span class="prop-widget-id-label" style="font-size: 11px; color: #888;">ID:</span>
        <input type="text" readonly value="${widget.id}" class="prop-widget-id-input"
          style="font-size: 11px; color: #aaa; background: transparent; border: none; outline: none; cursor: text; width: 100%; user-select: text;" />
      </div>
    `;
    this.container.appendChild(header);

    // Özellikleri grupla
    this.renderPropertyGroup('Pozisyon ve Boyut', [
      { key: 'x', label: 'X', type: 'number' },
      { key: 'y', label: 'Y', type: 'number' },
      { key: 'width', label: 'Genişlik', type: 'number' },
      { key: 'height', label: 'Yükseklik', type: 'number' }
    ], widget);

    // Görünüm özellikleri
    const appearanceProps = [];
    if (widget.properties.hasOwnProperty('text')) {
      appearanceProps.push({ key: 'text', label: 'Metin', type: 'text' });
    }
    if (widget.properties.hasOwnProperty('placeholder')) {
      appearanceProps.push({ key: 'placeholder', label: 'Placeholder', type: 'text' });
    }
    if (widget.properties.hasOwnProperty('backgroundColor')) {
      appearanceProps.push({ key: 'backgroundColor', label: 'Arka Plan Rengi', type: 'color' });
    }
    if (widget.properties.hasOwnProperty('textColor')) {
      appearanceProps.push({ key: 'textColor', label: 'Metin Rengi', type: 'color' });
    }
    if (widget.properties.hasOwnProperty('fontSize')) {
      appearanceProps.push({ key: 'fontSize', label: 'Font Boyutu', type: 'number' });
    }
    if (widget.properties.hasOwnProperty('fontFamily')) {
      appearanceProps.push({ key: 'fontFamily', label: 'Font Ailesi', type: 'font-select' });
    }

    if (appearanceProps.length > 0) {
      this.renderPropertyGroup('Görünüm', appearanceProps, widget);
    }

    // Kenarık özellikleri
    if (widget.properties.hasOwnProperty('borderWidth')) {
      this.renderPropertyGroup('Kenarık', [
        { key: 'borderWidth', label: 'Kalınlık', type: 'number' },
        { key: 'borderColor', label: 'Renk',     type: 'color'  },
        { key: 'borderStyle', label: 'Stil',     type: 'select',
          options: [
            { value: 'solid',  label: 'Düz'     },
            { value: 'dashed', label: 'Kesik'    },
            { value: 'dotted', label: 'Noktalı'  },
            { value: 'double', label: 'Çift'     },
          ]
        },
      ], widget);
    }

    // Davranış özellikleri
    const behaviorProps = [];
    if (widget.properties.hasOwnProperty('enabled')) {
      behaviorProps.push({ key: 'enabled', label: 'Etkin', type: 'checkbox' });
    }
    if (widget.properties.hasOwnProperty('visible')) {
      behaviorProps.push({ key: 'visible', label: 'Görünür', type: 'checkbox' });
    }
    if (widget.properties.hasOwnProperty('checked')) {
      behaviorProps.push({ key: 'checked', label: 'İşaretli', type: 'checkbox' });
    }
    if (widget.properties.hasOwnProperty('tabIndex')) {
      behaviorProps.push({ key: 'tabIndex', label: 'Sekme Sırası', type: 'number' });
    }

    if (behaviorProps.length > 0) {
      this.renderPropertyGroup('Davranış', behaviorProps, widget);
    }

    // Özel özellikler
    if (widget.type === 'radio' && widget.properties.hasOwnProperty('group')) {
      this.renderPropertyGroup('Özel', [
        { key: 'group', label: 'Grup', type: 'text' }
      ], widget);
    }

    if ((widget.type === 'listbox' || widget.type === 'combobox') && widget.properties.hasOwnProperty('items')) {
      this.renderItemsEditor(widget);
    }

    // Event handler'lar
    this.renderEventHandlers(widget);

    // Responsive (% tabanlı konum) bölümü
    this.renderResponsiveSection(widget);
  }

  // Özellik grubunu render et
  renderPropertyGroup(title, properties, widget) {
    const group = document.createElement('div');
    group.className = 'property-group';

    const groupTitle = document.createElement('div');
    groupTitle.className = 'property-group-title';
    groupTitle.textContent = title;
    group.appendChild(groupTitle);

    properties.forEach(prop => {
      const container = document.createElement('div');
      container.className = 'property-row-full';
      container.dataset.filterLabel = (title + ' ' + prop.label).toLowerCase();

      const item = document.createElement('div');
      item.className = 'property-item';

      const label = document.createElement('label');
      label.textContent = prop.label;
      item.appendChild(label);

      const input = this.createPropertyInput(prop, widget);
      item.appendChild(input);
      container.appendChild(item);
      group.appendChild(container);
    });

    this.container.appendChild(group);
  }

  // Özellik input'u oluştur
  createPropertyInput(prop, widget) {
    let input;

    if (prop.type === 'checkbox') {
      const row = document.createElement('div');
      row.className = 'checkbox-row';
      input = document.createElement('input');
      input.type = 'checkbox';
      input.checked = widget.properties[prop.key];
      input.addEventListener('change', (e) => {
        if (this.onPropertyChange) this.onPropertyChange(widget, prop.key, e.target.checked);
      });
      row.appendChild(input);
      return row;
    } else if (prop.type === 'color') {
      const colorRow = document.createElement('div');
      colorRow.className = 'color-row';

      const isTransparent = widget.properties[prop.key] === 'transparent';
      const prevKey = '_prev_' + prop.key;
      if (!widget.properties[prevKey] && !isTransparent) {
        widget.properties[prevKey] = widget.properties[prop.key];
      }
      let lastColor = widget.properties[prevKey] || '#888888';

      const swatch = document.createElement('input');
      swatch.type = 'color';
      swatch.className = 'color-swatch';
      swatch.value = lastColor;
      swatch.style.opacity = isTransparent ? '0.3' : '1';
      swatch.disabled = isTransparent;

      const hexInput = document.createElement('input');
      hexInput.type = 'text';
      hexInput.className = 'color-hex';
      hexInput.value = isTransparent ? 'transparent' : lastColor;
      hexInput.title = 'Hex renk kodu (#RRGGBB)';

      const transpCheck = document.createElement('input');
      transpCheck.type = 'checkbox';
      transpCheck.className = 'color-transparent-check';
      transpCheck.title = 'Saydam';
      transpCheck.checked = isTransparent;

      const fireChange = (value) => {
        if (this.onPropertyChange) this.onPropertyChange(widget, prop.key, value);
      };

      transpCheck.addEventListener('change', () => {
        if (transpCheck.checked) {
          widget.properties[prevKey] = lastColor;
          swatch.disabled = true;
          swatch.style.opacity = '0.3';
          hexInput.value = 'transparent';
          fireChange('transparent');
        } else {
          swatch.disabled = false;
          swatch.style.opacity = '1';
          swatch.value = lastColor;
          hexInput.value = lastColor;
          fireChange(lastColor);
        }
      });

      swatch.addEventListener('change', (e) => {
        if (!transpCheck.checked) {
          lastColor = e.target.value;
          widget.properties[prevKey] = lastColor;
          hexInput.value = lastColor;
          fireChange(lastColor);
        }
      });

      hexInput.addEventListener('change', (e) => {
        const v = e.target.value.trim();
        if (v === 'transparent') {
          transpCheck.checked = true;
          transpCheck.dispatchEvent(new Event('change'));
        } else if (/^#[0-9a-f]{6}$/i.test(v) || /^#[0-9a-f]{3}$/i.test(v)) {
          const full = v.length === 4
            ? '#' + v[1]+v[1]+v[2]+v[2]+v[3]+v[3]
            : v.toLowerCase();
          lastColor = full;
          widget.properties[prevKey] = full;
          swatch.value = full;
          if (transpCheck.checked) { transpCheck.checked = false; swatch.disabled = false; swatch.style.opacity = '1'; }
          fireChange(full);
        }
      });

      colorRow.appendChild(swatch);
      colorRow.appendChild(hexInput);
      colorRow.appendChild(transpCheck);
      return colorRow;
    } else if (prop.type === 'number') {
      input = document.createElement('input');
      input.type = 'number';
      input.value = widget.properties[prop.key];
    } else if (prop.type === 'select') {
      input = document.createElement('select');
      prop.options.forEach(opt => {
        const option = document.createElement('option');
        option.value = opt.value;
        option.textContent = opt.label;
        if (widget.properties[prop.key] === opt.value) option.selected = true;
        input.appendChild(option);
      });
    } else if (prop.type === 'font-select') {
      const FONTS = [
        'Arial', 'Verdana', 'Helvetica', 'Tahoma', 'Trebuchet MS',
        'Segoe UI', 'Georgia', 'Times New Roman', 'Courier New',
        'Consolas', 'Impact', 'Comic Sans MS'
      ];
      input = document.createElement('select');
      const currentFont = widget.properties[prop.key] || 'Arial';
      FONTS.forEach(f => {
        const opt = document.createElement('option');
        opt.value = f;
        opt.textContent = f;
        opt.style.fontFamily = f;
        if (f === currentFont) opt.selected = true;
        input.appendChild(opt);
      });
      input.style.fontFamily = currentFont;
      input.addEventListener('change', () => { input.style.fontFamily = input.value; });
    } else {
      input = document.createElement('input');
      input.type = 'text';
      input.value = widget.properties[prop.key] || '';
    }

    input.addEventListener('change', (e) => {
      const value = prop.type === 'number' ? parseFloat(e.target.value) || 0 : e.target.value;
      if (this.onPropertyChange) this.onPropertyChange(widget, prop.key, value);
    });

    return input;
  }

  // Liste öğelerini düzenle
  renderItemsEditor(widget) {
    const group = document.createElement('div');
    group.className = 'property-group';

    const groupTitle = document.createElement('div');
    groupTitle.className = 'property-group-title';
    groupTitle.textContent = 'Liste Öğeleri';
    group.appendChild(groupTitle);

    const itemsText = document.createElement('textarea');
    itemsText.className = 'property-input';
    itemsText.style.cssText = 'width: 100%; min-height: 100px; resize: vertical; font-family: monospace;';
    itemsText.placeholder = 'Her satıra bir öğe yazın...';
    itemsText.value = (widget.properties.items || []).join('\n');

    itemsText.addEventListener('change', () => {
      const items = itemsText.value.split('\n').filter(line => line.trim());
      if (this.onPropertyChange) {
        this.onPropertyChange(widget, 'items', items);
      }
    });

    group.appendChild(itemsText);
    this.container.appendChild(group);
  }

  // Event handler'ları render et
  renderEventHandlers(widget) {
    const events = [];

    if (widget.properties.hasOwnProperty('onClick')) {
      events.push({ key: 'onClick', label: 'onClick' });
    }
    if (widget.properties.hasOwnProperty('onChange')) {
      events.push({ key: 'onChange', label: 'onChange' });
    }
    if (widget.properties.hasOwnProperty('onSelect')) {
      events.push({ key: 'onSelect', label: 'onSelect' });
    }

    if (events.length === 0) return;

    const group = document.createElement('div');
    group.className = 'property-group';

    const groupTitle = document.createElement('div');
    groupTitle.className = 'property-group-title';
    groupTitle.textContent = 'Olaylar';
    group.appendChild(groupTitle);

    events.forEach(event => {
      const container = document.createElement('div');
      container.className = 'property-row-full';
      container.dataset.filterLabel = ('olaylar ' + event.label).toLowerCase();

      const item = document.createElement('div');
      item.className = 'property-item';

      const label = document.createElement('label');
      label.textContent = event.label;
      item.appendChild(label);

      const eventHandler = document.createElement('div');
      eventHandler.className = 'event-handler';

      const input = document.createElement('input');
      input.type = 'text';
      input.placeholder = 'Fonksiyon adı...';
      input.value = widget.properties[event.key] || '';
      input.addEventListener('change', (e) => {
        if (this.onPropertyChange) this.onPropertyChange(widget, event.key, e.target.value);
      });

      const editBtn = document.createElement('button');
      editBtn.textContent = '...';
      editBtn.title = 'Kod düzenle';
      editBtn.addEventListener('click', () => {
        const codeTab = document.getElementById('tab-code');
        if (codeTab) {
          codeTab.click();
          const functionName = widget.properties[event.key];
          if (functionName) {
            setTimeout(() => { highlightFunction(functionName); }, 100);
          }
        }
      });

      eventHandler.appendChild(input);
      eventHandler.appendChild(editBtn);
      item.appendChild(eventHandler);
      container.appendChild(item);
      group.appendChild(container);
    });

    this.container.appendChild(group);
  }

  // Canvas (pencere) özelliklerini göster
  showCanvasProperties(settings, onChange) {
    this.currentWidget = null;
    clearElement(this.container);

    const header = document.createElement('div');
    header.className = 'prop-widget-header';
    header.style.cssText = 'padding:8px;background-color:#2d2d30;margin-bottom:12px;border-radius:3px;';
    header.innerHTML = '<strong style="color:#cccccc;">&#x1F5BC; Pencere Özellikleri</strong>';
    this.container.appendChild(header);

    const fields = [
      { key: 'title',           label: 'Başlık',                  type: 'text'     },
      { key: 'width',           label: 'Genişlik',                type: 'number'   },
      { key: 'height',          label: 'Yükseklik',               type: 'number'   },
      { key: 'backgroundColor', label: 'Arka Plan Rengi',         type: 'color'    },
      { key: 'gridColor',       label: 'Izgara Rengi',            type: 'color'    },
      { key: 'gridSize',        label: 'Izgara Aralığı (px)',     type: 'number'   },
      { key: 'snapToGrid',      label: 'Izgara’ya Yapıştır',      type: 'checkbox' },
    ];

    const group = document.createElement('div');
    group.className = 'property-group';
    const title = document.createElement('div');
    title.className = 'property-group-title';
    title.textContent = 'Pencere';
    group.appendChild(title);

    fields.forEach(field => {
      const container = document.createElement('div');
      container.className = 'property-row-full';

      const item = document.createElement('div');
      item.className = 'property-item';

      const label = document.createElement('label');
      label.textContent = field.label;
      item.appendChild(label);

      if (field.type === 'color') {
        const colorRow = document.createElement('div');
        colorRow.className = 'color-row';
        const allowEmpty = field.key === 'gridColor';
        const currentVal = settings[field.key] || '';

        const swatch = document.createElement('input');
        swatch.type = 'color';
        swatch.className = 'color-swatch';
        swatch.value = currentVal || '#888888';
        if (allowEmpty && !currentVal) swatch.style.opacity = '0.4';

        const hexInput = document.createElement('input');
        hexInput.type = 'text';
        hexInput.className = 'color-hex';
        hexInput.value = currentVal;
        if (allowEmpty) hexInput.placeholder = 'Otomatik';

        swatch.addEventListener('input', () => {
          hexInput.value = swatch.value;
          swatch.style.opacity = '1';
          onChange(field.key, swatch.value);
        });
        hexInput.addEventListener('change', () => {
          const v = hexInput.value.trim();
          if (allowEmpty && v === '') {
            swatch.style.opacity = '0.4';
            onChange(field.key, '');
          } else if (/^#[0-9a-f]{6}$/i.test(v)) {
            swatch.value = v;
            swatch.style.opacity = '1';
            onChange(field.key, v);
          }
        });

        colorRow.appendChild(swatch);
        colorRow.appendChild(hexInput);
        item.appendChild(colorRow);
      } else if (field.type === 'checkbox') {
        const chkRow = document.createElement('div');
        chkRow.className = 'checkbox-row';
        const chk = document.createElement('input');
        chk.type = 'checkbox';
        chk.checked = !!settings[field.key];
        chk.addEventListener('change', () => onChange(field.key, chk.checked));
        chkRow.appendChild(chk);
        item.appendChild(chkRow);
      } else {
        const input = document.createElement('input');
        input.type = field.type;
        input.value = settings[field.key] ?? '';
        if (field.type === 'number') { input.min = '1'; input.max = '3840'; }
        input.addEventListener('change', () => {
          const v = field.type === 'number' ? parseInt(input.value, 10) : input.value;
          if (field.type === 'number' && isNaN(v)) return;
          onChange(field.key, v);
        });
        item.appendChild(input);
      }

      container.appendChild(item);
      group.appendChild(container);
    });

    this.container.appendChild(group);
  }

  // Responsive % konumlama bölümü
  renderResponsiveSection(widget) {
    const grp = document.createElement('div');
    grp.className = 'property-group';

    const title = document.createElement('div');
    title.className = 'property-group-title';
    title.textContent = '📐 Responsive';
    grp.appendChild(title);

    // Checkbox: % Tabanlı Konum
    const chkContainer = document.createElement('div');
    chkContainer.className = 'property-row-full';
    chkContainer.dataset.filterLabel = 'responsive % tabanlı konum';
    const chkItem = document.createElement('div');
    chkItem.className = 'property-item';
    const chkLabel = document.createElement('label');
    chkLabel.textContent = '% Konum';
    const chkRow2 = document.createElement('div');
    chkRow2.className = 'checkbox-row';
    const chk = document.createElement('input');
    chk.type = 'checkbox';
    chk.title = 'Etkinleştirildiğinde widget, önizlemede kanvas boyutuna göre ölçeklenir';
    chk.checked = !!widget.properties._usePercent;
    chkRow2.appendChild(chk);
    chkItem.appendChild(chkLabel);
    chkItem.appendChild(chkRow2);
    chkContainer.appendChild(chkItem);
    grp.appendChild(chkContainer);

    // % değerleri bilgi alanı
    const infoDiv = document.createElement('div');
    infoDiv.className = 'responsive-info';

    const renderInfo = () => {
      infoDiv.innerHTML = '';
      if (!widget.properties._usePercent) return;

      const cs = typeof designer !== 'undefined' ? designer.canvasSettings : { width: 800, height: 600 };
      const cw = cs.width  || 800;
      const ch = cs.height || 600;
      // Güncel değerleri göster
      const xP = (widget.properties._xPct != null ? widget.properties._xPct : (widget.properties.x / cw) * 100).toFixed(1);
      const yP = (widget.properties._yPct != null ? widget.properties._yPct : (widget.properties.y / ch) * 100).toFixed(1);
      const wP = (widget.properties._wPct != null ? widget.properties._wPct : (widget.properties.width  / cw) * 100).toFixed(1);
      const hP = (widget.properties._hPct != null ? widget.properties._hPct : (widget.properties.height / ch) * 100).toFixed(1);

      const pctTable = document.createElement('div');
      pctTable.className = 'responsive-pct-table';
      pctTable.innerHTML = `
        <div class="responsive-pct-row"><span>X:</span><span>${xP}%</span><span>Y:</span><span>${yP}%</span></div>
        <div class="responsive-pct-row"><span>G:</span><span>${wP}%</span><span>Y:</span><span>${hP}%</span></div>
      `;
      infoDiv.appendChild(pctTable);

      const recomputeBtn = document.createElement('button');
      recomputeBtn.className = 'responsive-recompute-btn';
      recomputeBtn.textContent = '🔄 Pikselden Yenile';
      recomputeBtn.title = 'Mevcut piksel değerlerinden % değerlerini yeniden hesapla';
      recomputeBtn.addEventListener('click', () => {
        widget.properties._xPct = (widget.properties.x / cw) * 100;
        widget.properties._yPct = (widget.properties.y / ch) * 100;
        widget.properties._wPct = (widget.properties.width  / cw) * 100;
        widget.properties._hPct = (widget.properties.height / ch) * 100;
        if (typeof designer !== 'undefined') designer.onWidgetChange?.();
        renderInfo();
      });
      infoDiv.appendChild(recomputeBtn);
    };

    chk.addEventListener('change', () => {
      const enabled = chk.checked;
      widget.properties._usePercent = enabled;
      if (enabled) {
        const cs = typeof designer !== 'undefined' ? designer.canvasSettings : { width: 800, height: 600 };
        const cw = cs.width  || 800;
        const ch = cs.height || 600;
        widget.properties._xPct = (widget.properties.x / cw) * 100;
        widget.properties._yPct = (widget.properties.y / ch) * 100;
        widget.properties._wPct = (widget.properties.width  / cw) * 100;
        widget.properties._hPct = (widget.properties.height / ch) * 100;
      }
      if (widget.element) widget.element.classList.toggle('is-responsive', enabled);
      if (typeof designer !== 'undefined') designer.onWidgetChange?.();
      renderInfo();
    });

    // Başlangıçta elemanın class'ını senkronize et
    if (widget.element) widget.element.classList.toggle('is-responsive', !!widget.properties._usePercent);

    grp.appendChild(infoDiv);
    renderInfo();
    this.container.appendChild(grp);
  }

  // Özellikleri temizle
  clear() {
    this.currentWidget = null;
    clearElement(this.container);
    const noSelection = document.createElement('div');
    noSelection.className = 'no-selection';
    noSelection.textContent = 'Bir widget seçin';
    this.container.appendChild(noSelection);
  }
}

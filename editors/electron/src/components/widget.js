// Widget Sınıfı ve Yönetimi

// Yüklenen widget ID'lerinin en büyük sayısını takip eder (çakışma önleme)
// designer.js clear() tarafından sıfırlanır
let widgetIdCounter = 0;

class Widget {
  constructor(type, properties = {}) {
    this.id = generateId(type);
    this.type = type;
    this.properties = { ...getDefaultProperties(type), ...properties };
    this.element = null;
  }

  // Widget'ı DOM'a render et
  render() {
    const div = document.createElement('div');
    div.id = this.id;
    div.className = `canvas-widget widget-${this.type}`;
    div.dataset.widgetId = this.id;
    div.dataset.widgetType = this.type;

    // Pozisyon ve boyut
    div.style.left = this.properties.x + 'px';
    div.style.top = this.properties.y + 'px';
    div.style.width = this.properties.width + 'px';
    div.style.height = this.properties.height + 'px';

    // Tip'e göre özel render
    this.renderWidgetContent(div);

    // Kenarık uygula
    this.applyBorderStyle(div, this.properties);

    // Resize handle'ları ekle
    this.addResizeHandles(div);

    this.element = div;
    return div;
  }

  // Kenarık stilini uygula
  applyBorderStyle(element, props) {
    const w = props.borderWidth ?? 0;
    if (w > 0) {
      element.style.borderWidth = w + 'px';
      element.style.borderStyle = props.borderStyle || 'solid';
      element.style.borderColor = props.borderColor || '#888888';
    } else {
      element.style.borderWidth = '0';
      element.style.borderStyle = '';
      element.style.borderColor = '';
    }
  }

  // Widget içeriğini render et
  renderWidgetContent(element) {
    // İçerik için wrapper div oluştur (pointer events için)
    const contentWrapper = document.createElement('div');
    contentWrapper.className = 'widget-content';
    contentWrapper.style.pointerEvents = 'none';
    contentWrapper.style.width = '100%';
    contentWrapper.style.height = '100%';
    contentWrapper.style.display = 'flex';
    contentWrapper.style.alignItems = 'center';
    contentWrapper.style.justifyContent = 'center';

    switch (this.type) {
      case 'button':
        contentWrapper.textContent = this.properties.text;
        element.style.backgroundColor = this.properties.backgroundColor;
        element.style.color = this.properties.textColor;
        element.style.fontSize = this.properties.fontSize + 'px';
        break;

      case 'label':
        contentWrapper.textContent = this.properties.text;
        element.style.color = this.properties.textColor;
        element.style.fontSize = this.properties.fontSize + 'px';
        element.style.backgroundColor = this.properties.backgroundColor;
        break;

      case 'textbox':
        element.style.backgroundColor = this.properties.backgroundColor;
        element.style.color = this.properties.textColor;
        element.style.fontSize = this.properties.fontSize + 'px';
        contentWrapper.textContent = this.properties.text || this.properties.placeholder;
        break;

      case 'checkbox':
        contentWrapper.textContent = this.properties.text;
        element.style.color = this.properties.textColor;
        element.style.fontSize = this.properties.fontSize + 'px';
        if (this.properties.checked) {
          element.classList.add('checked');
        }
        break;

      case 'radio':
        contentWrapper.textContent = this.properties.text;
        element.style.color = this.properties.textColor;
        element.style.fontSize = this.properties.fontSize + 'px';
        if (this.properties.checked) {
          element.classList.add('checked');
        }
        break;

      case 'panel':
        element.style.backgroundColor = this.properties.backgroundColor;
        contentWrapper.textContent = ''; // Panel boş
        break;

      case 'groupbox':
        element.style.color = this.properties.textColor;
        element.style.fontSize = this.properties.fontSize + 'px';
        element.dataset.title = this.properties.text;
        contentWrapper.textContent = ''; // GroupBox başlığı CSS ile gösterilir
        break;

      case 'listbox':
        element.style.backgroundColor = this.properties.backgroundColor;
        element.style.color = this.properties.textColor;
        element.style.fontSize = this.properties.fontSize + 'px';
        if (this.properties.items && this.properties.items.length > 0) {
          contentWrapper.innerHTML = this.properties.items.map(item =>
            `<div style="padding: 4px;">${item}</div>`
          ).join('');
        } else {
          contentWrapper.innerHTML = '<div style="padding: 4px; color: #666;">(Liste boş)</div>';
        }
        contentWrapper.style.flexDirection = 'column';
        contentWrapper.style.alignItems = 'flex-start';
        contentWrapper.style.justifyContent = 'flex-start';
        contentWrapper.style.overflow = 'auto';
        break;

      case 'combobox':
        element.style.backgroundColor = this.properties.backgroundColor;
        element.style.color = this.properties.textColor;
        element.style.fontSize = this.properties.fontSize + 'px';
        const selectedItem = this.properties.items && this.properties.selectedIndex >= 0
          ? this.properties.items[this.properties.selectedIndex]
          : '(Seçiniz)';
        contentWrapper.textContent = selectedItem;
        break;
    }

    // Görünürlük
    if (!this.properties.visible) {
      element.style.opacity = '0.3';
    }

    // Font ailesi (tüm metin tabanlı widget'lar için)
    if (this.properties.fontFamily) {
      element.style.fontFamily = this.properties.fontFamily;
    }

    // Content wrapper'ı element'e ekle
    element.appendChild(contentWrapper);
  }

  // Resize handle'ları ekle
  addResizeHandles(element) {
    const positions = ['nw', 'ne', 'sw', 'se'];
    positions.forEach(pos => {
      const handle = document.createElement('div');
      handle.className = `resize-handle ${pos}`;
      handle.dataset.position = pos;
      element.appendChild(handle);
    });
  }

  // Widget'ı güncelle
  update(newProperties) {
    this.properties = { ...this.properties, ...newProperties };
    if (!this.element) return;

    // Performans optimizasyonu: Sadece pozisyon/boyut değişiyorsa sadece style değiştir
    const positionOnlyUpdate = Object.keys(newProperties).every(key =>
      ['x', 'y', 'width', 'height'].includes(key)
    );

    if (positionOnlyUpdate) {
      // Doğrudan style değişikliği (reflow/repaint minimum)
      if ('x' in newProperties) {
        this.element.style.left = newProperties.x + 'px';
      }
      if ('y' in newProperties) {
        this.element.style.top = newProperties.y + 'px';
      }
      if ('width' in newProperties) {
        this.element.style.width = newProperties.width + 'px';
      }
      if ('height' in newProperties) {
        this.element.style.height = newProperties.height + 'px';
      }
    } else {
      // İçerik güncellemesi için daha akıllı yaklaşım
      this.updateElementInPlace(newProperties);
    }
  }

  // Element'i yerinde güncelle (replaceChild kullanmadan)
  updateElementInPlace(changedProps) {
    if (!this.element) return;

    // Content wrapper'ı bul
    let contentWrapper = this.element.querySelector('.widget-content');

    // Yoksa oluştur
    if (!contentWrapper) {
      contentWrapper = document.createElement('div');
      contentWrapper.className = 'widget-content';
      contentWrapper.style.pointerEvents = 'none';
      contentWrapper.style.width = '100%';
      contentWrapper.style.height = '100%';
      contentWrapper.style.display = 'flex';
      contentWrapper.style.alignItems = 'center';
      contentWrapper.style.justifyContent = 'center';
      this.element.appendChild(contentWrapper);
    }

    // Text değişiklikleri
    if ('text' in changedProps) {
      switch (this.type) {
        case 'button':
        case 'label':
        case 'checkbox':
        case 'radio':
          contentWrapper.textContent = this.properties.text;
          break;
        case 'textbox':
          contentWrapper.textContent = this.properties.text || this.properties.placeholder;
          break;
        case 'groupbox':
          this.element.dataset.title = this.properties.text;
          break;
      }
    }

    // Placeholder değişiklikleri
    if ('placeholder' in changedProps && this.type === 'textbox') {
      if (!this.properties.text) {
        contentWrapper.textContent = this.properties.placeholder;
      }
    }

    // Renk değişiklikleri
    if ('backgroundColor' in changedProps) {
      this.element.style.backgroundColor = this.properties.backgroundColor;
    }
    if ('textColor' in changedProps) {
      this.element.style.color = this.properties.textColor;
    }
    if ('fontSize' in changedProps) {
      this.element.style.fontSize = this.properties.fontSize + 'px';
    }
    if ('fontFamily' in changedProps) {
      this.element.style.fontFamily = this.properties.fontFamily || '';
    }

    // Görünürlük
    if ('visible' in changedProps) {
      this.element.style.opacity = this.properties.visible ? '1' : '0.3';
    }

    // Checkbox/Radio işaretleme
    if ('checked' in changedProps) {
      if (this.properties.checked) {
        this.element.classList.add('checked');
      } else {
        this.element.classList.remove('checked');
      }
    }

    // Liste öğeleri güncelleme
    if ('items' in changedProps) {
      if (this.type === 'listbox') {
        contentWrapper.style.flexDirection = 'column';
        contentWrapper.style.alignItems = 'flex-start';
        contentWrapper.style.justifyContent = 'flex-start';
        contentWrapper.style.overflow = 'auto';

        if (this.properties.items && this.properties.items.length > 0) {
          contentWrapper.innerHTML = this.properties.items.map(item =>
            `<div style="padding: 4px;">${item}</div>`
          ).join('');
        } else {
          contentWrapper.innerHTML = '<div style="padding: 4px; color: #666;">(Liste boş)</div>';
        }
      } else if (this.type === 'combobox') {
        const selectedItem = this.properties.items && this.properties.selectedIndex >= 0
          ? this.properties.items[this.properties.selectedIndex]
          : '(Seçiniz)';
        contentWrapper.textContent = selectedItem;
      }
    }

    // Kenarık değişiklikleri
    if ('borderWidth' in changedProps || 'borderColor' in changedProps || 'borderStyle' in changedProps) {
      this.applyBorderStyle(this.element, this.properties);
    }

    // ComboBox seçim değişikliği
    if ('selectedIndex' in changedProps && this.type === 'combobox') {
      const selectedItem = this.properties.items && this.properties.selectedIndex >= 0
        ? this.properties.items[this.properties.selectedIndex]
        : '(Seçiniz)';
      contentWrapper.textContent = selectedItem;
    }
  }

  // Widget verilerini JSON'a dönüştür
  toJSON() {
    return {
      id: this.id,
      type: this.type,
      properties: this.properties
    };
  }

  // JSON'dan widget oluştur
  static fromJSON(data) {
    // Eski format desteği: özellikler doğrudan üst düzeyde (id, type, x, y, text, ...)
    // Yeni format: özellikler data.properties nesnesi içinde
    let properties;
    if (data.properties && typeof data.properties === 'object') {
      properties = data.properties;
    } else {
      // Eski format: id ve type dışındaki tüm alanları properties olarak al
      const { id, type, ...rest } = data;
      properties = rest;
    }
    const widget = new Widget(data.type, properties);
    widget.id = data.id;
    // ID counter'ı güncelle (yeni format: widget_N; eski format: lbl1, btn1 vb.)
    const idNum = parseInt(data.id.split('_')[1]);
    if (!isNaN(idNum) && idNum >= widgetIdCounter) {
      widgetIdCounter = idNum + 1;
    }
    return widget;
  }
}

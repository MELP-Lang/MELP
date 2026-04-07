// IntelliSense Autocomplete System

// Widget tiplerine göre erişilebilir property'ler
const WIDGET_PROPERTIES = {
  button: [
    { name: 'text',      type: 'string',  desc: 'Buton üzerindeki yazı' },
    { name: 'visible',   type: 'numeric', desc: '1 = görünür, 0 = gizli' },
    { name: 'enabled',   type: 'numeric', desc: '1 = etkin, 0 = devre dışı' },
    { name: 'x',         type: 'numeric', desc: 'Yatay konum (piksel)' },
    { name: 'y',         type: 'numeric', desc: 'Dikey konum (piksel)' },
    { name: 'width',     type: 'numeric', desc: 'Genişlik (piksel)' },
    { name: 'height',    type: 'numeric', desc: 'Yükseklik (piksel)' },
    { name: 'bgColor',   type: 'string',  desc: 'Arka plan rengi (#RRGGBB)' },
    { name: 'textColor', type: 'string',  desc: 'Yazı rengi (#RRGGBB)' },
    { name: 'fontSize',  type: 'numeric', desc: 'Yazı boyutu (pt)' },
  ],
  label: [
    { name: 'text',      type: 'string',  desc: 'Etiket yazısı' },
    { name: 'visible',   type: 'numeric', desc: '1 = görünür, 0 = gizli' },
    { name: 'x',         type: 'numeric', desc: 'Yatay konum' },
    { name: 'y',         type: 'numeric', desc: 'Dikey konum' },
    { name: 'textColor', type: 'string',  desc: 'Yazı rengi (#RRGGBB)' },
    { name: 'fontSize',  type: 'numeric', desc: 'Yazı boyutu (pt)' },
  ],
  textbox: [
    { name: 'text',        type: 'string',  desc: 'İçerikteki metin' },
    { name: 'placeholder', type: 'string',  desc: 'Yer tutucu metin' },
    { name: 'visible',     type: 'numeric', desc: '1 = görünür, 0 = gizli' },
    { name: 'enabled',     type: 'numeric', desc: '1 = etkin, 0 = devre dışı' },
    { name: 'x',           type: 'numeric', desc: 'Yatay konum' },
    { name: 'y',           type: 'numeric', desc: 'Dikey konum' },
    { name: 'width',       type: 'numeric', desc: 'Genişlik' },
    { name: 'height',      type: 'numeric', desc: 'Yükseklik' },
  ],
  checkbox: [
    { name: 'text',    type: 'string',  desc: 'Onay kutusu yazısı' },
    { name: 'checked', type: 'numeric', desc: '1 = işaretli, 0 = işaretsiz' },
    { name: 'visible', type: 'numeric', desc: '1 = görünür, 0 = gizli' },
    { name: 'enabled', type: 'numeric', desc: '1 = etkin, 0 = devre dışı' },
    { name: 'x',       type: 'numeric', desc: 'Yatay konum' },
    { name: 'y',       type: 'numeric', desc: 'Dikey konum' },
  ],
  radio: [
    { name: 'text',    type: 'string',  desc: 'Seçenek yazısı' },
    { name: 'checked', type: 'numeric', desc: '1 = seçili, 0 = seçisiz' },
    { name: 'visible', type: 'numeric', desc: '1 = görünür, 0 = gizli' },
    { name: 'enabled', type: 'numeric', desc: '1 = etkin, 0 = devre dışı' },
    { name: 'x',       type: 'numeric', desc: 'Yatay konum' },
    { name: 'y',       type: 'numeric', desc: 'Dikey konum' },
  ],
  listbox: [
    { name: 'selectedIndex', type: 'numeric', desc: 'Seçili satır indeksi (0 tabanlı)' },
    { name: 'selectedText',  type: 'string',  desc: 'Seçili satırın metni' },
    { name: 'visible',       type: 'numeric', desc: '1 = görünür, 0 = gizli' },
    { name: 'enabled',       type: 'numeric', desc: '1 = etkin, 0 = devre dışı' },
    { name: 'x',             type: 'numeric', desc: 'Yatay konum' },
    { name: 'y',             type: 'numeric', desc: 'Dikey konum' },
  ],
  combobox: [
    { name: 'selectedIndex', type: 'numeric', desc: 'Seçili indeks (0 tabanlı)' },
    { name: 'selectedText',  type: 'string',  desc: 'Seçili metnin yazısı' },
    { name: 'visible',       type: 'numeric', desc: '1 = görünür, 0 = gizli' },
    { name: 'enabled',       type: 'numeric', desc: '1 = etkin, 0 = devre dışı' },
    { name: 'x',             type: 'numeric', desc: 'Yatay konum' },
    { name: 'y',             type: 'numeric', desc: 'Dikey konum' },
  ],
  panel: [
    { name: 'visible', type: 'numeric', desc: '1 = görünür, 0 = gizli' },
    { name: 'x',       type: 'numeric', desc: 'Yatay konum' },
    { name: 'y',       type: 'numeric', desc: 'Dikey konum' },
    { name: 'width',   type: 'numeric', desc: 'Genişlik' },
    { name: 'height',  type: 'numeric', desc: 'Yükseklik' },
  ],
};

class IntelliSense {
  constructor() {
    this.popup = null;
    this.items = [];
    this.selectedIndex = 0;
    this.isVisible = false;
    this.currentEditor = null;
    this.triggerPosition = { line: 0, column: 0 };
    this.filterText = '';

    // MLP functions database
    this.functions = [];
    this.keywords = [];

    this.createPopup();
    this.loadFunctions();
  }

  // Popup HTML oluştur
  createPopup() {
    this.popup = document.createElement('div');
    this.popup.className = 'intellisense-popup';
    this.popup.style.display = 'none';
    document.body.appendChild(this.popup);
  }

  // MLP fonksiyonlarını yükle
  async loadFunctions() {
    try {
      const response = await fetch('./mlp_functions.json');
      const data = await response.json();

      // Fonksiyonları düzleştir
      this.functions = [];
      for (const category of Object.values(data.categories)) {
        this.functions.push(...category.functions);
      }

      // Sabitleri ekle
      if (data.constants) {
        this.functions.push(...data.constants.map(c => ({
          name: c.name,
          signature: `${c.name} = ${c.value}`,
          description: c.description,
          category: 'constant'
        })));
      }

      // fonksiyon yüklendi
    } catch (error) {
      console.error('IntelliSense functions loading error:', error);
    }
  }

  // MLP keyword'lerini yükle
  loadKeywords(keywords) {
    this.keywords = keywords.map(kw => ({
      name: kw,
      signature: kw,
      description: 'MLP anahtar kelimesi',
      category: 'keyword'
    }));
  }

  // Editor'ı ayarla
  setEditor(editor) {
    this.currentEditor = editor;

  }

  // IntelliSense'i göster
  show(editor, position) {

    this.currentEditor = editor;
    this.triggerPosition = position;

    // Filtreleme için metin al
    const line = this.getLineText(position.line);
    this.filterText = this.getWordBeforeCursor(line, position.column);

    // Önerileri filtrele
    this.updateSuggestions();

    if (this.items.length === 0) {
      this.hide();
      return;
    }

    // Popup'ı konumlandır
    this.positionPopup(position);

    // Göster

    this.popup.style.display = 'block';
    this.isVisible = true;
    this.selectedIndex = 0;
    this.renderItems();
  }

  // Önerileri güncelle
  updateSuggestions() {
    const filter = this.filterText.toLowerCase();

    // Tüm öğeleri birleştir
    const allItems = [...this.functions, ...this.keywords];

    // Filtrele
    if (filter.length === 0) {
      this.items = allItems.slice(0, 20); // İlk 20'yi göster
    } else {
      this.items = allItems.filter(item =>
        item.name.toLowerCase().startsWith(filter)
      ).slice(0, 20);
    }

    // İsme göre sırala
    this.items.sort((a, b) => a.name.localeCompare(b.name));
  }

  // Popup konumlandır
  positionPopup(position) {
    const selection = window.getSelection();
    if (!selection.rangeCount) {
      return;
    }

    const range = selection.getRangeAt(0);
    const rect = range.getBoundingClientRect();

    // Eğer rect boşsa (cursor görünmüyorsa), editor'ün koordinatlarını kullan
    if (rect.width === 0 && rect.height === 0) {
      const editorRect = this.currentEditor.getBoundingClientRect();
      this.popup.style.left = `${editorRect.left + 10}px`;
      this.popup.style.top = `${editorRect.top + 30}px`;

      return;
    }

    // Cursor'un gerçek pozisyonunu kullan
    const left = rect.left;
    const top = rect.bottom + 2;

    this.popup.style.left = `${left}px`;
    this.popup.style.top = `${top}px`;
    

  }
  // Öğeleri render et
  renderItems() {
    this.popup.innerHTML = '';

    this.items.forEach((item, index) => {
      const itemDiv = document.createElement('div');
      itemDiv.className = 'intellisense-item';
      if (index === this.selectedIndex) {
        itemDiv.classList.add('selected');
      }

      // Icon
      const icon = this.getIconForCategory(item.category);
      const iconSpan = document.createElement('span');
      iconSpan.className = 'intellisense-icon';
      iconSpan.textContent = icon;

      // Name
      const nameSpan = document.createElement('span');
      nameSpan.className = 'intellisense-name';
      nameSpan.textContent = item.name;

      // Type/Category
      const typeSpan = document.createElement('span');
      typeSpan.className = 'intellisense-type';
      typeSpan.textContent = item.category || 'function';

      itemDiv.appendChild(iconSpan);
      itemDiv.appendChild(nameSpan);
      itemDiv.appendChild(typeSpan);

      // Click event
      itemDiv.addEventListener('click', () => {
        this.selectItem(index);
      });

      // Hover event
      itemDiv.addEventListener('mouseenter', () => {
        this.selectedIndex = index;
        this.renderItems();
      });

      this.popup.appendChild(itemDiv);
    });
  }

  // Kategori icon'u
  getIconForCategory(category) {
    const icons = {
      'function': '𝑓',
      'keyword': '🔑',
      'constant': '📌',
      'variable': '📦',
      'reduce': '∑',
      'map': '→',
      'utility': '🔧',
      'property': '●'
    };
    return icons[category] || '📄';
  }

  // Öğe seç ve ekle
  selectItem(index) {
    if (index < 0 || index >= this.items.length) return;

    const item = this.items[index];
    this.insertCompletion(item);
    this.hide();
  }

  // Tamamlamayı ekle
  insertCompletion(item) {
    if (!this.currentEditor) return;

    const completion = item.name;
    
    // Selection API kullanarak cursor pozisyonunda ekle
    const selection = window.getSelection();
    if (!selection.rangeCount) return;
    
    const range = selection.getRangeAt(0);
    
    // Kelime başlangıcını bul ve sil
    if (this.filterText.length > 0) {
      // Geriye doğru filterText uzunluğu kadar sil
      const textNode = range.startContainer;
      if (textNode.nodeType === Node.TEXT_NODE) {
        const offset = range.startOffset;
        const deleteRange = document.createRange();
        deleteRange.setStart(textNode, Math.max(0, offset - this.filterText.length));
        deleteRange.setEnd(textNode, offset);
        deleteRange.deleteContents();
      }
    }
    
    // Yeni metni ekle
    const textNode = document.createTextNode(completion);
    range.insertNode(textNode);
    
    // Cursor'u metnin sonuna taşı
    range.setStartAfter(textNode);
    range.setEndAfter(textNode);
    selection.removeAllRanges();
    selection.addRange(range);
    
    // Event tetikle (kod güncelleme için)
    this.currentEditor.dispatchEvent(new Event('input', { bubbles: true }));
  }

  // Widget property tamamlama (nokta sonrası)
  showForDot(editor, position, widgetId, widgetType) {
    this.currentEditor = editor;
    this.triggerPosition = position;
    this.isDotMode = true;

    // Nokta sonrası kısmi metni filtre olarak al
    const line = this.getLineText(position.line);
    const textBefore = line.substring(0, position.column);
    const dotIdx = textBefore.lastIndexOf('.');
    this.filterText = dotIdx >= 0 ? textBefore.substring(dotIdx + 1) : '';

    // Property listesini oluştur
    const props = WIDGET_PROPERTIES[widgetType] || [];
    const filter = this.filterText.toLowerCase();
    this.items = props
      .filter(p => p.name.toLowerCase().startsWith(filter))
      .map(p => ({
        name: p.name,
        signature: `${widgetId}.${p.name}: ${p.type}`,
        description: p.desc,
        category: 'property'
      }));



    if (this.items.length === 0) {
      this.hide();
      return;
    }

    this.positionPopup(position);
    this.popup.style.display = 'block';
    this.isVisible = true;
    this.selectedIndex = 0;
    this.renderItems();
  }

  // Gizle
  hide() {
    this.popup.style.display = 'none';
    this.isVisible = false;
    this.items = [];
    this.selectedIndex = 0;
    this.isDotMode = false;
  }

  // Yukarı ok
  selectPrevious() {
    if (this.selectedIndex > 0) {
      this.selectedIndex--;
      this.renderItems();
      this.scrollToSelected();
    }
  }

  // Aşağı ok
  selectNext() {
    if (this.selectedIndex < this.items.length - 1) {
      this.selectedIndex++;
      this.renderItems();
      this.scrollToSelected();
    }
  }

  // Seçili öğeye scroll
  scrollToSelected() {
    const items = this.popup.querySelectorAll('.intellisense-item');
    if (items[this.selectedIndex]) {
      items[this.selectedIndex].scrollIntoView({ block: 'nearest' });
    }
  }

  // Enter - seçili öğeyi ekle
  insertSelected() {
    if (this.items.length > 0) {
      this.selectItem(this.selectedIndex);
    }
  }

  // Escape - gizle
  cancel() {
    this.hide();
  }

  // Yardımcı: Satır metnini al
  getLineText(lineNum) {
    if (!this.currentEditor) return '';
    
    // textContent kullan (innerText satır sayısını bozuyor)
    const text = this.currentEditor.textContent;  // innerText satır sayısını bozuyor!
    
    const lines = text.split('\n');

    
    return lines[lineNum] || '';
  }

  // Yardımcı: Cursor'dan önceki kelimeyi al
  getWordBeforeCursor(line, column) {
    const textBefore = line.substring(0, column);
    const match = textBefore.match(/[a-zA-Z_][a-zA-Z0-9_]*$/);
    return match ? match[0] : '';
  }

  // Yardımcı: Cursor pozisyonunu al (editor içinde)
  getCursorPosition(editor) {
    const selection = window.getSelection();
    if (!selection.rangeCount) {
      return { line: 0, column: 0 };
    }

    const range = selection.getRangeAt(0);
    const preCaretRange = range.cloneRange();
    preCaretRange.selectNodeContents(editor);
    preCaretRange.setEnd(range.endContainer, range.endOffset);
    const textBefore = preCaretRange.toString();
    const lines = textBefore.split('\n');

    const pos = {
      line: lines.length - 1,
      column: lines[lines.length - 1].length
    };
    

    return pos;
  }
}

// Global instance
const intelliSense = new IntelliSense();

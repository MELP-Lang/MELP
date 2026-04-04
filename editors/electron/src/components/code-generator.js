// MLP Kod Üreteci

class CodeGenerator {
  constructor() {
    this.language = 'tr-TR'; // Varsayılan dil
    this.syntax = 'mlp-default'; // Varsayılan syntax
  }

  // Geriye uyumluluk — tam kodu üretir
  generate(widgets, windowTitle = 'MLP GUI Uygulaması', windowWidth = 800, windowHeight = 600) {
    return this.generateEventCode(widgets) + '\n' +
           this.generateDesignSource(widgets, windowTitle, windowWidth, windowHeight);
  }

  // TASARIM KAYNAĞI: app_start() bloğu — widget mizanpajı
  generateDesignSource(widgets, windowTitle = 'MLP GUI Uygulaması', windowWidth = 800, windowHeight = 600) {
    const currentLang = configLoader.getCurrentLanguage();
    if (currentLang) this.language = currentLang.id;

    // program adı: sadece ASCII harf/rakam/alt çizgi (lexer non-ASCII kabul etmez)
    const programName = windowTitle
      .replace(/\s+/g, '')
      .replace(/[ğĞ]/g, 'g').replace(/[üÜ]/g, 'u').replace(/[şŞ]/g, 's')
      .replace(/[ıİ]/g, 'i').replace(/[öÖ]/g, 'o').replace(/[çÇ]/g, 'c')
      .replace(/[^a-zA-Z0-9_]/g, '_') || 'MLPProgram';

    let code = `-- lang: ${this.language}\n`;
    code += `-- Tasarim kaynagi -- otomatik olusturulur, elle duzenlenmeyin\n\n`;
    code += `program ${programName}\n\n`;

    code += `app_start()\n`;
    code += `    GUI_PENCERE_AC(${windowWidth}, ${windowHeight}, "${windowTitle}")\n`;

    if (widgets.length === 0) {
      code += `    -- Henuz widget eklenmedi\n`;
    } else {
      code += `\n`;
      widgets.forEach(widget => {
        code += this.generateWidgetCode(widget);
      });
    }

    code += `end_app\n`;
    return code;
  }

  // OLAY KOD\u0130: sadece event handler'lar
  generateEventCode(widgets) {
    const currentLang = configLoader.getCurrentLanguage();
    if (currentLang) this.language = currentLang.id;

    const eventHandlers = this.collectEventHandlers(widgets);

    if (eventHandlers.length === 0) {
      return `-- lang: ${this.language}\n-- Olay isleyicisi yok\n-- Bir widget'a onClick/onChange ekleyin\n`;
    }

    let code = `-- lang: ${this.language}\n\n`;
    eventHandlers.forEach(handler => {
      code += this.generateEventHandler(handler);
    });
    return code;
  }

  // Widget'lardan MLP kodu üret (eski imza — geriye uyumluluk)
  _generate_legacy(widgets, windowTitle = 'MLP GUI Uygulaması', windowWidth = 800, windowHeight = 600) {
    const currentLang = configLoader.getCurrentLanguage();
    const currentSyntax = configLoader.getCurrentSyntax();

    if (currentLang) {
      this.language = currentLang.id;
    }
    if (currentSyntax) {
      this.syntax = currentSyntax.id;
    }

    let code = `-- lang: ${this.language}\n`;
    code += `-- ${windowTitle}\n`;
    code += `-- Bu kod MLP GUI Designer ile otomatik oluşturulmuştur\n\n`;

    // Event handler'lar önce gelir (forward declaration gibi)
    const eventHandlers = this.collectEventHandlers(widgets);
    if (eventHandlers.length > 0) {
      eventHandlers.forEach(handler => {
        code += this.generateEventHandler(handler);
      });
    }

    // app_start bloğu — pencere + widget kurulumu, event loop Electron'a teslim
    code += `app_start()\n`;
    code += `    GUI_PENCERE_AC(${windowWidth}, ${windowHeight}, "${windowTitle}")\n`;
    code += `\n`;

    // Widget'ları oluştur ve event bağlantıları kur
    widgets.forEach(widget => {
      code += this.generateWidgetCode(widget);
    });

    code += `end_app\n`;

    return code;
  }

  // Event handler'ları topla
  collectEventHandlers(widgets) {
    const handlers = new Set();
    widgets.forEach(widget => {
      if (widget.properties.onClick) handlers.add(widget.properties.onClick);
      if (widget.properties.onChange) handlers.add(widget.properties.onChange);
      if (widget.properties.onSelect) handlers.add(widget.properties.onSelect);
    });
    return Array.from(handlers);
  }

  // Event handler üret — dönüş değeri yok, event fire → çalışır → biter
  generateEventHandler(handlerName) {
    if (!handlerName) return '';

    const syntax = configLoader.getCurrentSyntax();
    const useBraces = syntax?.block_style === 'braces';

    let code = '';
    if (useBraces) {
      code += `event ${handlerName}() {\n`;
      code += `    \n`;
      code += `}\n\n`;
    } else {
      code += `event ${handlerName}()\n`;
      code += `    \n`;
      code += `end_event\n\n`;
    }

    return code;
  }

  // Tek bir widget için kod üret
  generateWidgetCode(widget) {
    const props = widget.properties;
    let code = `-- ${widget.type}: ${widget.id}\n`;

    switch (widget.type) {
      case 'button':
        code += `    ${widget.id} = GUI_BUTTON_CREATE("${props.text}", ${props.x}, ${props.y}, ${props.width}, ${props.height})\n`;
        if (props.onClick) {
          code += `    GUI_BUTTON_ON_CLICK(${widget.id}, "${props.onClick}")\n`;
        }
        break;

      case 'label':
        code += `    ${widget.id} = GUI_LABEL_CREATE("${props.text}", ${props.x}, ${props.y})\n`;
        break;

      case 'textbox':
        code += `    ${widget.id} = GUI_TEXTBOX_CREATE("${props.placeholder || ''}", ${props.x}, ${props.y}, ${props.width}, ${props.height})\n`;
        if (props.text) {
          code += `    GUI_TEXTBOX_SET_TEXT(${widget.id}, "${props.text}")\n`;
        }
        break;

      case 'checkbox':
        code += `    ${widget.id} = GUI_CHECKBOX_CREATE("${props.text}", ${props.x}, ${props.y})\n`;
        if (props.checked) {
          code += `    GUI_CHECKBOX_SET_CHECKED(${widget.id}, 1)\n`;
        }
        break;

      case 'radio':
        code += `    ${widget.id} = GUI_RADIO_CREATE("${props.text}", "${props.group}", ${props.x}, ${props.y})\n`;
        if (props.checked) {
          code += `    GUI_RADIO_SET_CHECKED(${widget.id}, 1)\n`;
        }
        break;

      case 'panel':
        code += `    ${widget.id} = GUI_PANEL_CREATE(${props.x}, ${props.y}, ${props.width}, ${props.height})\n`;
        break;

      case 'groupbox':
        code += `    ${widget.id} = GUI_GROUPBOX_CREATE("${props.text}", ${props.x}, ${props.y}, ${props.width}, ${props.height})\n`;
        break;

      case 'listbox':
        code += `    ${widget.id} = GUI_LISTBOX_CREATE(${props.x}, ${props.y}, ${props.width}, ${props.height})\n`;
        if (props.items && props.items.length > 0) {
          props.items.forEach(item => {
            code += `    GUI_LISTBOX_ADD_ITEM(${widget.id}, "${item}")\n`;
          });
        }
        break;

      case 'combobox':
        code += `    ${widget.id} = GUI_COMBOBOX_CREATE(${props.x}, ${props.y}, ${props.width}, ${props.height})\n`;
        if (props.items && props.items.length > 0) {
          props.items.forEach(item => {
            code += `    GUI_COMBOBOX_ADD_ITEM(${widget.id}, "${item}")\n`;
          });
        }
        if (props.selectedIndex >= 0) {
          code += `    GUI_COMBOBOX_SET_SELECTED(${widget.id}, ${props.selectedIndex})\n`;
        }
        break;
    }

    // Görünürlük
    if (!props.visible) {
      code += `    GUI_WIDGET_SET_VISIBLE(${widget.id}, 0)\n`;
    }

    // Etkinlik
    if (props.hasOwnProperty('enabled') && !props.enabled) {
      code += `    GUI_WIDGET_SET_ENABLED(${widget.id}, 0)\n`;
    }

    return code;
  }

  // Dil ayarla
  setLanguage(lang) {
    this.language = lang;
  }

  // Syntax'a göre block markers'ı al
  getBlockMarkers() {
    const syntax = configLoader.getCurrentSyntax();
    if (!syntax) {
      return {
        if_start: 'if',
        if_end: 'end if',
        while_start: 'while',
        while_end: 'end while',
        func_start: 'func',
        func_end: 'end func'
      };
    }

    const markers = syntax.markers || {};
    const blockStyle = syntax.block_style || 'keyword';

    // C-style için
    if (blockStyle === 'braces') {
      return {
        if_start: '{',
        if_end: '}',
        while_start: '{',
        while_end: '}',
        func_start: '{',
        func_end: '}'
      };
    }

    // Python-style için
    if (blockStyle === 'indentation') {
      return {
        if_start: ':',
        if_end: '',  // Dedent ile hallolur
        while_start: ':',
        while_end: '',
        func_start: ':',
        func_end: ''
      };
    }

    // MLP-default
    return {
      if_start: '',
      if_end: 'end_if',
      while_start: '',
      while_end: 'end_while',
      func_start: '',
      func_end: 'end_function'
    };
  }

  // Dile göre keyword translate et
  translateKeyword(keyword) {
    return configLoader.translateKeyword(keyword) || keyword;
  }
}

// Node.js uyumluluk (testler için)
if (typeof module !== 'undefined') {
  if (typeof configLoader === 'undefined') {
    global.configLoader = {
      getCurrentLanguage: () => null,
      getCurrentSyntax:   () => null,
      translateKeyword:   (k) => k
    };
  }
  module.exports = { CodeGenerator };
}

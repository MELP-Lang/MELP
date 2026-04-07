/**
 * CodeGenerator birim testleri
 * Not: configLoader global stub'ı code-generator.js yüklendiğinde otomatik kurulur.
 */
const { CodeGenerator } = require('../src/components/code-generator.js');

// Sahte widget nesnesi üretici
function makeWidget(type, id, props = {}) {
  const defaults = {
    button:   { text: 'Buton', x: 10, y: 10, width: 80, height: 30, visible: true, enabled: true },
    label:    { text: 'Etiket', x: 10, y: 10, width: 80, height: 25, visible: true },
    textbox:  { text: '', placeholder: 'Metin girin', x: 10, y: 50, width: 200, height: 30, visible: true, enabled: true },
    checkbox: { text: 'Onay', x: 10, y: 90, width: 150, height: 25, checked: false, visible: true, enabled: true },
    listbox:  { x: 10, y: 10, width: 150, height: 100, items: ['Öğe1', 'Öğe2'], visible: true },
    combobox: { x: 10, y: 10, width: 150, height: 30, items: ['A', 'B'], selectedIndex: 0, visible: true },
  };
  return { type, id, properties: { ...defaults[type], ...props } };
}

module.exports = function ({ test, assert, assertEqual }) {
  // ─── generateWidgetCode ───────────────────────────────────────
  test('button kodu doğru GUI_BUTTON_CREATE çağrısı içermeli', () => {
    const gen = new CodeGenerator();
    const w = makeWidget('button', 'button_1', { text: 'Tamam', x: 20, y: 30, width: 90, height: 32 });
    const code = gen.generateWidgetCode(w);
    assert(code.includes('GUI_BUTTON_CREATE'), 'GUI_BUTTON_CREATE çağrısı olmalı');
    assert(code.includes('"Tamam"'), 'Buton metni olmalı');
    assert(code.includes('20'), 'X koordinatı olmalı');
    assert(code.includes('button_1'), 'Widget ID olmalı');
  });

  test('button onClick handler GUI_BUTTON_ON_CLICK üretmeli', () => {
    const gen = new CodeGenerator();
    const w = makeWidget('button', 'btn_1', { onClick: 'butona_tikla' });
    const code = gen.generateWidgetCode(w);
    assert(code.includes('GUI_BUTTON_ON_CLICK'), 'GUI_BUTTON_ON_CLICK çağrısı olmalı');
    assert(code.includes('"butona_tikla"'), 'Handler adı olmalı');
  });

  test('label kodu GUI_LABEL_CREATE içermeli', () => {
    const gen = new CodeGenerator();
    const w = makeWidget('label', 'label_1', { text: 'Ad:' });
    const code = gen.generateWidgetCode(w);
    assert(code.includes('GUI_LABEL_CREATE'), 'GUI_LABEL_CREATE çağrısı olmalı');
    assert(code.includes('"Ad:"'), 'Label metni olmalı');
  });

  test('textbox kodu GUI_TEXTBOX_CREATE içermeli', () => {
    const gen = new CodeGenerator();
    const w = makeWidget('textbox', 'tbx_1');
    const code = gen.generateWidgetCode(w);
    assert(code.includes('GUI_TEXTBOX_CREATE'), 'GUI_TEXTBOX_CREATE çağrısı olmalı');
  });

  test('checkbox kodu GUI_CHECKBOX_CREATE içermeli', () => {
    const gen = new CodeGenerator();
    const w = makeWidget('checkbox', 'chk_1', { text: 'Etkin' });
    const code = gen.generateWidgetCode(w);
    assert(code.includes('GUI_CHECKBOX_CREATE'), 'GUI_CHECKBOX_CREATE çağrısı olmalı');
  });

  test('checkbox checked=true GUI_CHECKBOX_SET_CHECKED üretmeli', () => {
    const gen = new CodeGenerator();
    const w = makeWidget('checkbox', 'chk_1', { checked: true });
    const code = gen.generateWidgetCode(w);
    assert(code.includes('GUI_CHECKBOX_SET_CHECKED'), 'Checked için set çağrısı olmalı');
  });

  test('listbox öğeleri GUI_LISTBOX_ADD_ITEM ile eklenmeli', () => {
    const gen = new CodeGenerator();
    const w = makeWidget('listbox', 'lb_1', { items: ['Elma', 'Armut'] });
    const code = gen.generateWidgetCode(w);
    assert(code.includes('GUI_LISTBOX_ADD_ITEM'), 'GUI_LISTBOX_ADD_ITEM çağrısı olmalı');
    assert(code.includes('"Elma"'), '"Elma" öğesi olmalı');
    assert(code.includes('"Armut"'), '"Armut" öğesi olmalı');
  });

  test('görünmez widget GUI_WIDGET_SET_VISIBLE(id, 0) üretmeli', () => {
    const gen = new CodeGenerator();
    const w = makeWidget('button', 'btn_hidden', { visible: false });
    const code = gen.generateWidgetCode(w);
    assert(code.includes('GUI_WIDGET_SET_VISIBLE'), 'Görünmezlik kodu olmalı');
    assert(code.includes(', 0)'), 'Değer 0 olmalı');
  });

  test('devre dışı widget GUI_WIDGET_SET_ENABLED(id, 0) üretmeli', () => {
    const gen = new CodeGenerator();
    const w = makeWidget('button', 'btn_dis', { enabled: false });
    const code = gen.generateWidgetCode(w);
    assert(code.includes('GUI_WIDGET_SET_ENABLED'), 'Devre dışı kodu olmalı');
  });

  // ─── generateDesignSource ─────────────────────────────────────
  test('generateDesignSource app_start / end_app içermeli', () => {
    const gen = new CodeGenerator();
    const code = gen.generateDesignSource([], 'Test Uygulaması', 800, 600);
    assert(code.includes('app_start()'), 'app_start() olmalı');
    assert(code.includes('end_app'), 'end_app olmalı');
    assert(code.includes('GUI_PENCERE_AC(800, 600'), 'Pencere boyutu olmalı');
  });

  test('generateDesignSource program adında boşluk olmamalı', () => {
    const gen = new CodeGenerator();
    const code = gen.generateDesignSource([], 'Benim Uygulamam');
    const programLine = code.split('\n').find(l => l.startsWith('program '));
    assert(programLine, 'program satırı olmalı');
    assert(!programLine.includes(' ', 8), `Program adında boşluk olmamalı: "${programLine}"`);
  });

  test('widget listesiyle tam kod üretilmeli', () => {
    const gen = new CodeGenerator();
    const widgets = [
      makeWidget('button', 'btn_1', { text: 'Gönder', onClick: 'gonder_tikla' }),
      makeWidget('label', 'lbl_1', { text: 'Ad Soyad:' }),
    ];
    const code = gen.generate(widgets, 'Form Uygulaması', 640, 480);
    assert(code.includes('GUI_BUTTON_CREATE'), 'Buton kodu olmalı');
    assert(code.includes('GUI_LABEL_CREATE'), 'Etiket kodu olmalı');
    assert(code.includes('gonder_tikla'), 'Event handler olmalı');
  });

  // ─── collectEventHandlers ─────────────────────────────────────
  test('collectEventHandlers benzersiz handler listesi döndürmeli', () => {
    const gen = new CodeGenerator();
    const widgets = [
      makeWidget('button', 'btn_1', { onClick: 'tikla' }),
      makeWidget('button', 'btn_2', { onClick: 'tikla' }),   // duplikasyon
      makeWidget('textbox', 'tbx_1', { onChange: 'degisti' }),
    ];
    const handlers = gen.collectEventHandlers(widgets);
    assertEqual(handlers.length, 2, 'Benzersiz 2 handler olmalı (tikla, degisti)');
  });

  // ─── JSON round-trip (proje serialize/deserialize) ────────────
  test('widget özellikleri JSON round-trip sonrası korunmalı', () => {
    const original = {
      id: 'button_42',
      type: 'button',
      properties: {
        x: 100, y: 200, width: 120, height: 35,
        text: 'Uygula', backgroundColor: '#0e639c',
        enabled: true, visible: true, onClick: 'apply_clicked'
      }
    };
    const serialized   = JSON.stringify(original);
    const deserialized = JSON.parse(serialized);
    assertEqual(deserialized.id,                     original.id,                     'id korunmalı');
    assertEqual(deserialized.properties.x,           original.properties.x,           'x korunmalı');
    assertEqual(deserialized.properties.text,        original.properties.text,        'text korunmalı');
    assertEqual(deserialized.properties.onClick,     original.properties.onClick,     'onClick korunmalı');
    assertEqual(deserialized.properties.backgroundColor,
                original.properties.backgroundColor,           'backgroundColor korunmalı');
  });

  test('çoklu widget projesi JSON round-trip yapabilmeli', () => {
    const project = {
      version: '1.0.4',
      title: 'Test Projesi',
      width: 800,
      height: 600,
      widgets: [
        { id: 'btn_1', type: 'button',  properties: { x:10, y:10, width:80, height:30, text:'OK', visible:true, enabled:true } },
        { id: 'lbl_1', type: 'label',   properties: { x:10, y:50, width:80, height:25, text:'Ad:', visible:true } },
        { id: 'tbx_1', type: 'textbox', properties: { x:10, y:80, width:200, height:30, text:'', visible:true, enabled:true } },
      ]
    };
    const parsed = JSON.parse(JSON.stringify(project));
    assertEqual(parsed.widgets.length, 3, '3 widget olmalı');
    assertEqual(parsed.widgets[0].id, 'btn_1', 'İlk widget ID korunmalı');
    assertEqual(parsed.version, '1.0.4', 'Sürüm korunmalı');
    assert(parsed.widgets.every(w => w.properties.visible), 'Tüm widgetlar görünür olmalı');
  });
};

/**
 * helpers.js için birim testler
 */
const { getDefaultProperties, getWidgetTypeName, generateId, rgbToHex, hexToRgb } = require('../src/utils/helpers.js');

module.exports = function ({ test, assert, assertEqual }) {
  // ─── getDefaultProperties ─────────────────────────────────────
  test('button varsayılanları doğru alanları içermeli', () => {
    const d = getDefaultProperties('button');
    assert(d.width > 0, 'width > 0');
    assert(d.height > 0, 'height > 0');
    assertEqual(d.text, 'Button', 'text === "Button"');
    assertEqual(d.enabled, true, 'enabled === true');
    assertEqual(d.visible, true, 'visible === true');
  });

  test('label varsayılanları doğru alanları içermeli', () => {
    const d = getDefaultProperties('label');
    assert(d.width > 0, 'width > 0');
    assertEqual(d.text, 'Label', 'text === "Label"');
    assertEqual(d.visible, true, 'visible === true');
  });

  test('textbox varsayılanları doğru alanları içermeli', () => {
    const d = getDefaultProperties('textbox');
    assert(typeof d.placeholder === 'string', 'placeholder bir string olmalı');
    assertEqual(d.enabled, true, 'enabled === true');
  });

  test('bilinmeyen tür için boş obje döndürmeli', () => {
    const d = getDefaultProperties('unknown_type_xyz');
    assert(d !== null && d !== undefined, 'null döndürmemeli');
    assert(typeof d === 'object', 'obje döndürmeli');
  });

  // ─── getWidgetTypeName ────────────────────────────────────────
  test('button için Türkçe isim döndürmeli', () => {
    const name = getWidgetTypeName('button');
    assert(typeof name === 'string' && name.length > 0, 'boş olmayan string döndürmeli');
  });

  test('bilinmeyen tür için string döndürmeli', () => {
    const name = getWidgetTypeName('xyz_unknown');
    assert(typeof name === 'string', 'string döndürmeli');
  });

  // ─── generateId ───────────────────────────────────────────────
  test('generateId prefix ile id oluşturmalı', () => {
    const id = generateId('button');
    assert(id.startsWith('button_'), `"button_" ile başlamalı, alındı: "${id}"`);
  });

  test('generateId ardışık id üretmeli', () => {
    const id1 = generateId('label');
    const id2 = generateId('label');
    assert(id1 !== id2, 'ardışık id\'ler farklı olmalı');
  });

  test('generateId prefix olmadan çalışmalı', () => {
    const id = generateId();
    assert(typeof id === 'string' && id.length > 0, 'boş olmayan string döndürmeli');
  });

  // ─── rgbToHex ─────────────────────────────────────────────────
  test('rgbToHex kırmızı dönüştürmeli', () => {
    const hex = rgbToHex('rgb(255, 0, 0)');
    assertEqual(hex.toLowerCase(), '#ff0000', 'rgb(255,0,0) → #ff0000');
  });

  test('rgbToHex siyah dönüştürmeli', () => {
    const hex = rgbToHex('rgb(0, 0, 0)');
    assertEqual(hex.toLowerCase(), '#000000', 'rgb(0,0,0) → #000000');
  });

  // ─── hexToRgb ─────────────────────────────────────────────────
  test('hexToRgb kırmızı için doğru değer döndürmeli', () => {
    const rgb = hexToRgb('#ff0000');
    assert(rgb !== null, 'null döndürmemeli');
    assertEqual(rgb.r, 255, 'r === 255');
    assertEqual(rgb.g, 0, 'g === 0');
    assertEqual(rgb.b, 0, 'b === 0');
  });

  test('hexToRgb beyaz için doğru değer döndürmeli', () => {
    const rgb = hexToRgb('#ffffff');
    assertEqual(rgb.r, 255, 'r === 255');
    assertEqual(rgb.g, 255, 'g === 255');
    assertEqual(rgb.b, 255, 'b === 255');
  });

  test('geçersiz hex için null döndürmeli', () => {
    const rgb = hexToRgb('not-a-color');
    assert(rgb === null, 'geçersiz hex için null döndürmeli');
  });
};

/**
 * SyntaxValidator birim testleri
 */
const { SyntaxValidator } = require('../src/utils/syntax-validator.js');

module.exports = function ({ test, assert, assertEqual }) {
  function makeValidator() { return new SyntaxValidator(); }

  // ─── Temel doğrulama ──────────────────────────────────────────
  test('geçerli kod hata döndürmemeli', () => {
    const v = makeValidator();
    const errors = v.validate('x = GUI_BUTTON_CREATE("Tamam", 10, 10, 80, 30)');
    assert(errors.length === 0, `Hata olmamalı; alındı: ${errors.map(e => e.message).join(', ')}`);
  });

  test('yorum satırları atlanmalı', () => {
    const v = makeValidator();
    const errors = v.validate('-- bu bir yorum\n// bu da yorum');
    assert(errors.length === 0, 'Yorumlar hata üretmemeli');
  });

  test('boş kod hata döndürmemeli', () => {
    const v = makeValidator();
    const errors = v.validate('');
    assertEqual(errors.length, 0, 'Boş kod için hata olmamalı');
  });

  // ─── Parantez dengesi ─────────────────────────────────────────
  test('eksik kapatma parantezi hata üretmeli', () => {
    const v = makeValidator();
    const errors = v.validate('x = func(a, b');
    assert(errors.length > 0, 'Eksik parantez için hata bekleniyor');
    assert(errors.some(e => e.severity === 'error'), 'Hata seviyesi "error" olmalı');
  });

  test('fazla kapatma parantezi hata üretmeli', () => {
    const v = makeValidator();
    const errors = v.validate('x = func(a))');
    assert(errors.length > 0, 'Fazla parantez için hata bekleniyor');
  });

  test('dengeli parantezler hata üretmemeli', () => {
    const v = makeValidator();
    const errors = v.validate('result = add(mul(2, 3), sub(5, 1))');
    assert(errors.length === 0, `Dengelı parantezler hata üretmemeli; alındı: ${errors.map(e=>e.message).join(',')}`);
  });

  // ─── Tırnak dengesi ───────────────────────────────────────────
  test('eşleşmeyen çift tırnak hata üretmeli', () => {
    const v = makeValidator();
    const errors = v.validate('x = "eksik kapanış');
    assert(errors.length > 0, 'Eşleşmeyen tırnak için hata bekleniyor');
  });

  test('dengeli tırnaklar hata üretmemeli', () => {
    const v = makeValidator();
    const errors = v.validate('x = "merhaba dünya"');
    assert(errors.length === 0, 'Dengeli tırnaklar hata üretmemeli');
  });

  // ─── API metodları ────────────────────────────────────────────
  test('hasErrors() hatasız kodda false döndürmeli', () => {
    const v = makeValidator();
    v.validate('x = 5');
    assertEqual(v.hasErrors(), false, 'Hata yoksa false döndürmeli');
  });

  test('hasErrors() hatalı kodda true döndürmeli', () => {
    const v = makeValidator();
    v.validate('x = func(a');
    assertEqual(v.hasErrors(), true, 'Hata varsa true döndürmeli');
  });

  test('getAllErrors() dizi döndürmeli', () => {
    const v = makeValidator();
    v.validate('');
    assert(Array.isArray(v.getAllErrors()), 'getAllErrors() dizi döndürmeli');
  });

  test('getErrorsForLine() doğru satır hatalarını döndürmeli', () => {
    const v = makeValidator();
    v.validate('iyi satır\nx = func(a');
    const line2Errors = v.getErrorsForLine(2);
    assert(line2Errors.length > 0, 'Satır 2\'de hata olmalı');
    assert(line2Errors.every(e => e.line === 2), 'Tüm hatalar satır 2\'de olmalı');
  });

  // ─── Kod üreteci entegrasyon ──────────────────────────────────
  test('validate() her çağrıda önceki hataları temizlemeli', () => {
    const v = makeValidator();
    v.validate('x = func(a'); // hatalı
    v.validate('x = 5');       // geçerli — temizlenmeli
    assertEqual(v.hasErrors(), false, 'İkinci validate çağrısı hataları temizlemeli');
  });
};

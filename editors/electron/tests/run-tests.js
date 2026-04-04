#!/usr/bin/env node
/**
 * MELP Test Runner
 * Kullanım: node tests/run-tests.js
 */

let passed = 0;
let failed = 0;

function test(name, fn) {
  try {
    fn();
    passed++;
    console.log(`  ✅ ${name}`);
  } catch (e) {
    failed++;
    console.error(`  ❌ ${name}: ${e.message}`);
  }
}

function assert(condition, message) {
  if (!condition) throw new Error(message || 'Assertion failed');
}

function assertEqual(a, b, message) {
  if (a !== b) throw new Error(message || `Expected ${JSON.stringify(b)}, got ${JSON.stringify(a)}`);
}

const ctx = { test, assert, assertEqual };

// ─── Test dosyalarını çalıştır ────────────────────────────────────
console.log('\n=== MELP Unit Tests ===\n');

console.log('helpers.js:');
require('./test-helpers.js')(ctx);

console.log('\nsyntax-validator.js:');
require('./test-syntax-validator.js')(ctx);

console.log('\ncode-generator.js + JSON round-trip:');
require('./test-code-generator.js')(ctx);

console.log(`\n${'─'.repeat(40)}`);
console.log(`Toplam: ${passed + failed} test | ✅ ${passed} geçti | ❌ ${failed} başarısız\n`);

if (failed > 0) process.exit(1);

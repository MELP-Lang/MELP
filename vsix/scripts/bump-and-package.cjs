#!/usr/bin/env node
/**
 * Patch sürümünü artır (0.7.0 → 0.7.1) ve VSIX paketle.
 * Kullanım: node scripts/bump-and-package.cjs [patch|minor|major]
 */

const fs   = require('fs');
const path = require('path');
const { execSync } = require('child_process');

const pkgPath = path.join(__dirname, '..', 'package.json');
const pkg     = JSON.parse(fs.readFileSync(pkgPath, 'utf-8'));

const level = process.argv[2] ?? 'patch';
let [major, minor, patch] = pkg.version.split('.').map(Number);

if (level === 'major') { major++; minor = 0; patch = 0; }
else if (level === 'minor') { minor++; patch = 0; }
else { patch++; }

const newVersion = `${major}.${minor}.${patch}`;
pkg.version = newVersion;
fs.writeFileSync(pkgPath, JSON.stringify(pkg, null, 2) + '\n');
console.log(`\n📦 Sürüm: ${newVersion}`);

// Önce webpack bundle
execSync('npm run compile', { stdio: 'inherit' });

// vsce package
if (typeof globalThis.File === 'undefined') {
  globalThis.File = class File {};
}
require('../node_modules/@vscode/vsce/out/main')(
  ['', '', 'package', '--no-dependencies', '--out', `mlp-ide-${newVersion}.vsix`]
);

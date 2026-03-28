#!/usr/bin/env node

if (typeof globalThis.File === 'undefined') {
  globalThis.File = class File {};
}

require('../node_modules/@vscode/vsce/out/main')(process.argv);
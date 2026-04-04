// WASM Backend — Emscripten tabanlı MLP derleyici (renderer process)
// Web editörün app-web.js::wasmBackend ile aynı pattern.
'use strict';

let _melpModule = null;

async function _loadMelpModule() {
  if (_melpModule) return _melpModule;
  if (typeof MelpCompiler === 'undefined') {
    throw new Error('melp_compiler.js yüklenmemiş');
  }
  _melpModule = await MelpCompiler({
    locateFile(path) {
      // Electron renderer: assets/ klasörüne relative yol
      if (path.endsWith('.wasm')) return 'assets/melp_compiler.wasm';
      return path;
    }
  });
  return _melpModule;
}

// Derlenmiş WASM binary'sini WASI polyfill ile çalıştır
async function _execWasm(wasmBytes) {
  let stdout = '';
  let instance;
  const importObject = {
    wasi_snapshot_preview1: {
      fd_write(fd, iovPtr, iovCnt, nwrittenPtr) {
        const mem = new DataView(instance.exports.memory.buffer);
        let written = 0;
        for (let i = 0; i < iovCnt; i++) {
          const base = mem.getUint32(iovPtr + i * 8,     true);
          const len  = mem.getUint32(iovPtr + i * 8 + 4, true);
          const bytes = new Uint8Array(instance.exports.memory.buffer, base, len);
          stdout += new TextDecoder().decode(bytes);
          written += len;
        }
        mem.setUint32(nwrittenPtr, written, true);
        return 0;
      },
      proc_exit(code)         { throw { exitCode: code }; },
      environ_get()           { return 0; },
      environ_sizes_get()     { return 0; },
      args_get()              { return 0; },
      args_sizes_get()        { return 0; },
      clock_time_get()        { return 0; },
      clock_res_get()         { return 0; },
    }
  };
  try {
    ({ instance } = await WebAssembly.instantiate(wasmBytes, importObject));
    // _start varsa onu çağır (clang/WASI convention); yoksa main'e dön
    if (instance.exports._start) {
      instance.exports._start();
    } else if (instance.exports.main) {
      instance.exports.main();
    }
  } catch (e) {
    if (e && typeof e.exitCode !== 'undefined' && e.exitCode !== 0) {
      return { stdout, stderr: `exit code ${e.exitCode}`, exitCode: e.exitCode };
    }
  }
  return { stdout, stderr: '', exitCode: 0 };
}

const wasmBackend = {
  // WASM modülü yüklenebilir mi?
  isAvailable() {
    return typeof MelpCompiler !== 'undefined';
  },

  // MLP kaynağını derle; run=true ise derlenmiş binary'yi de çalıştır.
  // Döner: { stdout, stderr, exitCode }
  async compile(code, run = true) {
    try {
      const mod = await _loadMelpModule();
      const rc = mod.ccall('melp_compile', 'number', ['string'], [code]);
      if (rc !== 0) {
        const errStr = mod.ccall('melp_get_error', 'string', [], []);
        return { stdout: '', stderr: errStr || 'Derleme hatası', exitCode: 1 };
      }
      const size = mod.ccall('melp_get_wasm_size', 'number', [], []);
      const ptr  = mod.ccall('melp_get_wasm_ptr',  'number', [], []);
      const wasmBytes = new Uint8Array(mod.HEAPU8.buffer, ptr, size).slice();

      if (!run) {
        return { stdout: `✅ Derleme başarılı (${size} byte WASM)\n`, stderr: '', exitCode: 0 };
      }
      return await _execWasm(wasmBytes);
    } catch (err) {
      return { stdout: '', stderr: err.message || String(err), exitCode: 1 };
    }
  }
};

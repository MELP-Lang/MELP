// Diagnostics — WASM/derleyici çıktısından hata/uyarı parse'lama
'use strict';

// Örnek satırlar:
//   error: 5:12: Tanımsız değişken: x
//   warning: 3:1: Kullanılmayan değişken: y
//   /tmp/_melp_ide_run.mlp:5:12: error: Tanımsız değişken: x
const DIAG_RE = /(?:error|warning|note)[\s:]+(\d+):(\d+)[:\s]+(.+)/i;
const DIAG_WITH_FILE_RE = /[^:\s]+:(\d+):(\d+):\s*(error|warning|note):\s*(.+)/i;

/**
 * WASM/derleyici stderr çıktısını parse ederek diagnostic listesi üretir.
 * @param {string} stderr
 * @returns {{ severity: 'error'|'warning'|'note', line: number, col: number, message: string }[]}
 */
function parseCompilerDiagnostics(stderr) {
  if (!stderr) return [];
  const diagnostics = [];
  for (const raw of stderr.split('\n')) {
    const line = raw.trim();
    if (!line) continue;

    let m = DIAG_WITH_FILE_RE.exec(line);
    if (m) {
      diagnostics.push({
        severity: m[3].toLowerCase(),
        line:     parseInt(m[1], 10),
        col:      parseInt(m[2], 10),
        message:  m[4].trim()
      });
      continue;
    }

    m = DIAG_RE.exec(line);
    if (m) {
      // severity satırın başındadır
      const sev = /^warning/i.test(line) ? 'warning' : /^note/i.test(line) ? 'note' : 'error';
      diagnostics.push({
        severity: sev,
        line:     parseInt(m[1], 10),
        col:      parseInt(m[2], 10),
        message:  m[3].trim()
      });
    }
  }
  return diagnostics;
}

/**
 * Hata ve uyarı sayısını döndürür.
 */
function diagnosticSummary(diagnostics) {
  const errors   = diagnostics.filter(d => d.severity === 'error').length;
  const warnings = diagnostics.filter(d => d.severity === 'warning').length;
  const parts = [];
  if (errors)   parts.push(`${errors} hata`);
  if (warnings) parts.push(`${warnings} uyarı`);
  return parts.join(', ') || '';
}

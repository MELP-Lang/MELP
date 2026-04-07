// debug.js — Breakpoint & Debug panel yönetimi

// ── Durum ─────────────────────────────────────────────────────────────────────
const _breakpoints = new Set();  // satır numaraları (1-based)
const _watchList   = [];         // [{ id, name, value }]
let   _debugActive = false;      // debug session açık mı?
let   _dbgCurrentLine = null;    // şu anda durulan satır

// ── Gutter (Satır Numarası) Breakpoint Toggle ─────────────────────────────────

function initBreakpointGutter() {
  const gutter = document.getElementById('line-numbers');
  if (!gutter) return;
  gutter.addEventListener('click', e => {
    const el = e.target.closest('.line-number');
    if (!el) return;
    const lineNum = parseInt(el.dataset.line || el.textContent.trim());
    if (!lineNum) return;
    toggleBreakpoint(lineNum);
  });
}

function toggleBreakpoint(line) {
  if (_breakpoints.has(line)) {
    _breakpoints.delete(line);
  } else {
    _breakpoints.add(line);
  }
  _refreshGutterBreakpoints();
  _refreshBreakpointList();
  showStatus(`Satır ${line}: breakpoint ${_breakpoints.has(line) ? 'eklendi' : 'kaldırıldı'}`);
}

function _refreshGutterBreakpoints() {
  const gutter = document.getElementById('line-numbers');
  if (!gutter) return;
  gutter.querySelectorAll('.line-number').forEach(el => {
    const lineNum = parseInt(el.dataset.line || el.textContent.replace('⬤', '').trim());
    el.classList.toggle('has-breakpoint', _breakpoints.has(lineNum));
  });
}

// ── Breakpoint Listesi ────────────────────────────────────────────────────────

function _refreshBreakpointList() {
  const container = document.getElementById('debug-bp-list');
  if (!container) return;
  container.innerHTML = '';

  if (_breakpoints.size === 0) {
    container.innerHTML =
      '<div class="debug-empty">Breakpoint yok.<br><small>Satır numarasına tıklayın.</small></div>';
    return;
  }

  const sorted = [..._breakpoints].sort((a, b) => a - b);
  sorted.forEach(line => {
    const row = document.createElement('div');
    row.className = 'debug-bp-row';
    row.innerHTML =
      `<span class="debug-bp-dot">⬤</span>` +
      `<span class="debug-bp-line" data-line="${line}">Satır ${line}</span>` +
      `<button class="debug-bp-remove" data-line="${line}" title="Kaldır">✕</button>`;
    row.querySelector('.debug-bp-line').addEventListener('click', () => {
      if (typeof scrollToLine === 'function') scrollToLine(line);
    });
    row.querySelector('.debug-bp-remove').addEventListener('click', e => {
      e.stopPropagation();
      _breakpoints.delete(line);
      _refreshGutterBreakpoints();
      _refreshBreakpointList();
    });
    container.appendChild(row);
  });
}

function clearAllBreakpoints() {
  _breakpoints.clear();
  _refreshGutterBreakpoints();
  _refreshBreakpointList();
  showStatus('Tüm breakpoint\'ler kaldırıldı');
}

// ── Watch Panel ───────────────────────────────────────────────────────────────

function addWatch(name) {
  if (!name || _watchList.find(w => w.name === name)) return;
  _watchList.push({ id: Date.now(), name, value: '—' });
  _refreshWatchList();
}

function removeWatch(id) {
  const idx = _watchList.findIndex(w => w.id === id);
  if (idx !== -1) _watchList.splice(idx, 1);
  _refreshWatchList();
}

// Runtime'dan değer güncellemesi geldiğinde (ileriki compiler entegrasyonu)
function updateWatchValue(name, value) {
  const entry = _watchList.find(w => w.name === name);
  if (entry) {
    entry.value = value;
    _refreshWatchList();
  }
}

function _refreshWatchList() {
  const container = document.getElementById('debug-watch-list');
  if (!container) return;
  container.innerHTML = '';

  if (_watchList.length === 0) {
    container.innerHTML =
      '<div class="debug-empty">İzleme listesi boş.<br><small>Aşağıdaki alana değişken adı girin.</small></div>';
    return;
  }

  _watchList.forEach(w => {
    const row = document.createElement('div');
    row.className = 'debug-watch-row';
    row.innerHTML =
      `<span class="debug-watch-name">${w.name}</span>` +
      `<span class="debug-watch-value">${w.value}</span>` +
      `<button class="debug-watch-remove" data-id="${w.id}" title="Kaldır">✕</button>`;
    row.querySelector('.debug-watch-remove').addEventListener('click', () => removeWatch(w.id));
    container.appendChild(row);
  });
}

// ── Debug Session Durumu ──────────────────────────────────────────────────────
// (Gerçek çalıştırma ileriki commit'te compiler entegrasyonu ile)

function debugRun() {
  _debugActive = true;
  _updateDebugToolbar();
  showStatus('Debug: Çalışıyor... (compiler entegrasyonu için bkz. compile-runner.js)');
  if (typeof logDiagnostic === 'function') logDiagnostic('debug', 'debug-run');
}

function debugContinue() {
  if (!_debugActive) { debugRun(); return; }
  _dbgCurrentLine = null;
  _updateDebugToolbar();
  _highlightCurrentLine(null);
  showStatus('Debug: Devam etti');
  if (typeof logDiagnostic === 'function') logDiagnostic('debug', 'debug-continue');
}

function debugStepOver() {
  showStatus('Debug: Adım (F10) — compiler entegrasyonu gerektirir');
  if (typeof logDiagnostic === 'function') logDiagnostic('debug', 'debug-step-over');
}

function debugStepInto() {
  showStatus('Debug: İçeri Gir (F11) — compiler entegrasyonu gerektirir');
  if (typeof logDiagnostic === 'function') logDiagnostic('debug', 'debug-step-into');
}

function debugStop() {
  _debugActive = false;
  _dbgCurrentLine = null;
  _updateDebugToolbar();
  _highlightCurrentLine(null);
  showStatus('Debug: Durduruldu');
  if (typeof logDiagnostic === 'function') logDiagnostic('debug', 'debug-stop');
}

function _updateDebugToolbar() {
  const btnContinue = document.getElementById('dbg-btn-continue');
  const btnStep     = document.getElementById('dbg-btn-step');
  const btnInto     = document.getElementById('dbg-btn-into');
  const btnStop     = document.getElementById('dbg-btn-stop');
  const btnRun      = document.getElementById('dbg-btn-run');
  const badge       = document.getElementById('debug-active-badge');

  if (btnRun)      btnRun.disabled      = _debugActive;
  if (btnContinue) btnContinue.disabled = !_debugActive;
  if (btnStep)     btnStep.disabled     = !_debugActive;
  if (btnInto)     btnInto.disabled     = !_debugActive;
  if (btnStop)     btnStop.disabled     = !_debugActive;
  if (badge)       badge.style.display  = _debugActive ? '' : 'none';
}

function _highlightCurrentLine(lineNum) {
  const gutter = document.getElementById('line-numbers');
  if (!gutter) return;
  gutter.querySelectorAll('.line-number.dbg-current').forEach(el =>
    el.classList.remove('dbg-current'));
  if (lineNum) {
    const el = gutter.children[lineNum - 1];
    if (el) el.classList.add('dbg-current');
  }
}

// ── Panel Sekmeleri ───────────────────────────────────────────────────────────

function initDebugPanel() {
  // Panel sekmesi tıklaması
  const termTab  = document.getElementById('debug-tab-terminal');
  const debugTab = document.getElementById('debug-tab-debug');
  const termArea = document.getElementById('terminal-content-area');
  const debugArea= document.getElementById('debug-content-area');

  function switchTab(active, inactive, show, hide) {
    active.classList.add('active');
    inactive.classList.remove('active');
    show.classList.remove('hidden');
    hide.classList.add('hidden');
  }

  if (termTab && debugTab) {
    termTab.addEventListener('click',  () => switchTab(termTab,  debugTab, termArea,  debugArea));
    debugTab.addEventListener('click', () => switchTab(debugTab, termTab,  debugArea, termArea));
  }

  // Watch ekle
  const watchInput = document.getElementById('debug-watch-input');
  const watchAdd   = document.getElementById('debug-watch-add');
  if (watchAdd && watchInput) {
    watchAdd.addEventListener('click', () => {
      const name = watchInput.value.trim();
      if (name) { addWatch(name); watchInput.value = ''; }
    });
    watchInput.addEventListener('keydown', e => {
      if (e.key === 'Enter') {
        const name = watchInput.value.trim();
        if (name) { addWatch(name); watchInput.value = ''; }
      }
    });
  }

  // Tüm breakpoint'leri temizle
  const clearBpBtn = document.getElementById('debug-clear-bp');
  if (clearBpBtn) clearBpBtn.addEventListener('click', clearAllBreakpoints);

  // Debug toolbar butonları
  document.getElementById('dbg-btn-run')?.addEventListener('click', debugRun);
  document.getElementById('dbg-btn-continue')?.addEventListener('click', debugContinue);
  document.getElementById('dbg-btn-step')?.addEventListener('click', debugStepOver);
  document.getElementById('dbg-btn-into')?.addEventListener('click', debugStepInto);
  document.getElementById('dbg-btn-stop')?.addEventListener('click', debugStop);

  // Gutter breakpoint tıklaması
  initBreakpointGutter();

  _updateDebugToolbar();
  _refreshBreakpointList();
  _refreshWatchList();
}

// getBreakpoints: compile-runner.js tarafından çağrılabilir
function getBreakpoints() { return [..._breakpoints]; }

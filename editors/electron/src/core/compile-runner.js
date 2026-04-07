// compile-runner.js -- WASM diagnostics + handleRun
// ── WASM diagnostics yardımcıları ───────────────────────────────────────────
// _updateDiagnosticsPanel: diagnostics listesini mevcut error panel'e ilet
function _updateDiagnosticsPanel(diagnostics) {
  if (typeof displayErrors === 'function') {
    displayErrors(diagnostics);
  }
  // Durum çubuğunda hata/uyarı sayısını göster
  if (typeof diagnosticSummary === 'function') {
    const summary = diagnosticSummary(diagnostics);
    if (summary) showStatus(summary);
  }
}

// ── FAZ 2.1: As-you-type debounce derleme ───────────────────────────────────
let _wasmDebounceTimer = null;
const WASM_DEBOUNCE_MS = 800;

function _scheduleWasmLint(code) {
  if (typeof wasmBackend === 'undefined' || !wasmBackend.isAvailable()) return;
  clearTimeout(_wasmDebounceTimer);
  _wasmDebounceTimer = setTimeout(async () => {
    const result = await wasmBackend.compile(code, false); // sadece derle, çalıştırma
    const diagnostics = (typeof parseCompilerDiagnostics === 'function')
      ? parseCompilerDiagnostics(result.stderr)
      : [];
    _updateDiagnosticsPanel(diagnostics);
    // Durum ibaresi
    const cntEl = document.getElementById('status-message');
    if (result.exitCode === 0) {
      if (cntEl) cntEl.textContent = '✅ Hazır';
    } else {
      const summary = (typeof diagnosticSummary === 'function')
        ? diagnosticSummary(diagnostics) : 'Hata';
      if (cntEl) cntEl.textContent = '🔴 ' + summary;
    }
  }, WASM_DEBOUNCE_MS);
}

async function handleRun() {
  if (typeof logDiagnostic === 'function') {
    logDiagnostic('run', 'handle-run', {
      hasWidgets: !!(designer && designer.widgets && designer.widgets.length > 0),
      widgetCount: designer?.widgets?.length || 0,
      wasmAvailable: typeof wasmBackend !== 'undefined' && wasmBackend.isAvailable()
    });
  }

  // ── Masaüstü form önizleme (her zaman ilk önce) ────────────────────────────
  if (window.electronAPI?.openFormPreview) {
    const canvas = document.getElementById('canvas');
    if (canvas && designer && designer.widgets.length > 0) {
      // Animasyon verisini gönder: { id, anim } listesi
      const animationData = designer.widgets
        .filter(w => w.properties._anim && w.properties._anim.type !== 'none')
        .map(w => ({ id: w.id, anim: w.properties._anim }));
      await window.electronAPI.openFormPreview({
        canvasHtml:     canvas.innerHTML,
        canvasSettings: designer.canvasSettings,
        animationData
      });
      showStatus('Form penceresi açıldı');
      if (typeof logDiagnostic === 'function') {
        logDiagnostic('preview', 'open-form-preview', {
          widgetCount: designer.widgets.length,
          width: designer.canvasSettings?.width,
          height: designer.canvasSettings?.height
        });
      }
      return;
    } else {
      showStatus('Önce Tasarım sekmesine widget ekleyin');
      return;
    }
  }

  // ── Electron yoksa (tarayıcı modu) — kod sekmesine geç ──────────────────────
  if (!window.electronAPI?.writeTempFile) {
    const tabCode = document.getElementById('tab-code');
    tabCode.click();
    showStatus('Kod oluşturuldu');
    return;
  }

  // KOD sekmesindeki kullanıcı kodunu oku — syncEventCode() ÇAĞIRMA, o kodu siler!
  const codeEditor = document.getElementById('code-editor');
  // innerText satır sonlarını (<br>/<div>) \n olarak döndürür; textContent döndürmez
  const rawCode = codeEditor ? (codeEditor.innerText || codeEditor.textContent || '') : '';

  // event NAME() ... end_event → function NAME() -> void ... end_function
  // (stage1 derleyici "event" anahtar kelimesini tanımıyor; top-level fonksiyon çalıştırılabilir)
  const handlerNames = [];
  const mlpCode = rawCode.replace(
    /^event\s+(\w+)\s*\(([^)]*)\)\s*\n([\s\S]*?)^end_event/gm,
    (match, name, params, body) => {
      handlerNames.push(name);
      return `function ${name}(${params}) -> void\n${body}end_function`;
    }
  ).trimEnd() + (handlerNames.length > 0
    ? '\n\n-- Test calisma\n' + handlerNames.map(n => `${n}()`).join('\n')
    : '');

  if (!mlpCode.trim()) {
    showStatus('Çalıştırılacak kod yok — KOD sekmesine MLP kodu yazın');
    return;
  }

  // ── WASM yolu (in-process, hızlı) ──────────────────────────────────────────
  if (typeof wasmBackend !== 'undefined' && wasmBackend.isAvailable()) {
    showStatus('Derleniyor… (WASM)');
    if (typeof toggleBottomTerminal === 'function') toggleBottomTerminal(true);

    const result = await wasmBackend.compile(mlpCode, true);

    const diagnostics = (typeof parseCompilerDiagnostics === 'function')
      ? parseCompilerDiagnostics(result.stderr)
      : [];
    const summary = (typeof diagnosticSummary === 'function')
      ? diagnosticSummary(diagnostics)
      : '';

    if (result.exitCode !== 0) {
      if (typeof logDiagnostic === 'function') {
        logDiagnostic('wasm', 'compile-failed', {
          exitCode: result.exitCode,
          stderrPreview: String(result.stderr || '').slice(0, 500)
        }, 'error');
      }
      showStatus(summary || 'Derleme hatası');
      _appendTerminal('stderr', result.stderr || 'Bilinmeyen hata\n');
      _appendTerminal('exit', result.exitCode);
      _updateDiagnosticsPanel(diagnostics);
    } else {
      if (typeof logDiagnostic === 'function') {
        logDiagnostic('wasm', 'compile-succeeded', {
          stdoutPreview: String(result.stdout || '').slice(0, 500)
        });
      }
      showStatus(summary ? `✅ Derlendi — ${summary}` : '✅ Çalıştırıldı');
      _appendTerminal('stdout', result.stdout || '');
      _appendTerminal('exit', 0);
      _updateDiagnosticsPanel([]);
    }
    return;
  }

  // ── Fallback: native binary (WASM yoksa) ────────────────────────────────────
  // Terminale geç
  if (typeof toggleBottomTerminal === 'function') toggleBottomTerminal(true);

  // Dil tespiti — TR ise .tr.mlp yaz ve normalizer'dan geçir
  const currentLang = (typeof configLoader !== 'undefined' && configLoader.currentLanguage) || 'en-US';
  const isTurkish = currentLang === 'tr-TR';

  const tmpFile = isTurkish ? '/tmp/_melp_ide_run.tr.mlp' : '/tmp/_melp_ide_run.mlp';
  const normalizedFile = '/tmp/_melp_ide_run.mlp';
  _currentMlpFile = isTurkish ? normalizedFile : tmpFile;
  try {
    const writeResult = await window.electronAPI.writeTempFile(tmpFile, mlpCode);
    if (!writeResult || !writeResult.ok) throw new Error('Dosya yazılamadı');
  } catch (err) {
    showStatus('Geçici dosya yazılamadı!');
    alert(
      `Çalıştırma hatası: ${tmpFile} dosyasına yazılamadı.\n\nHata: ${err.message}\n\nÇözüm: /tmp klasörünün yazılabilir olduğundan emin olun ya da settings.json dosyasından tmpFile yolunu güncelleyin.`
    );
    return;
  }

  // Derleyici yolu (settings.json'dan veya dinamik default'tan)
  const compilerPath = _settings.compilerPath;
  const lliPath = _settings.lliPath;
  const normalizerPath = _settings.normalizerPath;
  // Derleyici verbose satırlarını (📖🔤✅🌳⚙️📊) filtrele; hata varsa göster; sonra lli çalıştır
  let cmd;
  if (isTurkish && normalizerPath) {
    cmd = `bash "${normalizerPath}" "${tmpFile}" "${normalizedFile}" && _o=$("${compilerPath}" "${normalizedFile}" 2>&1); _e=$?; printf '%s\n' "$_o" | grep -vE '^(📖|🔤|✅|🌳|⚙️|📊)'; [ $_e -eq 0 ] && echo '--- Çıktı ---' && "${lliPath}" output.ll 2>&1`;
  } else {
    cmd = `_o=$("${compilerPath}" "${tmpFile}" 2>&1); _e=$?; printf '%s\n' "$_o" | grep -vE '^(📖|🔤|✅|🌳|⚙️|📊)'; [ $_e -eq 0 ] && echo '--- Çıktı ---' && "${lliPath}" output.ll 2>&1`;
  }

  showStatus('Derleniyor ve çalıştırılıyor…');
  if (typeof logDiagnostic === 'function') {
    logDiagnostic('run', 'native-compile-exec', {
      compilerPath,
      lliPath,
      tmpFile,
      isTurkish,
      normalizerPath
    });
  }
  window.electronAPI.terminalExec(cmd, '/tmp');
}

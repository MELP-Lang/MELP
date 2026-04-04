
// Satır numaralarını güncelle
function updateLineNumbers() {
  const codeEditor = document.getElementById('code-editor');
  const lineNumbersDiv = document.getElementById('line-numbers');

  if (!lineNumbersDiv) return;

  const code = codeEditor.textContent;
  const lines = code.split('\n');

  // Satır numaralarını oluştur
  let lineNumbersHTML = '';
  for (let i = 1; i <= lines.length; i++) {
    lineNumbersHTML += `<div class="line-number" data-line="${i}">${i}</div>`;
  }

  lineNumbersDiv.innerHTML = lineNumbersHTML;

  // Hatalı satırları işaretle
  const errors = syntaxValidator.getAllErrors();
  errors.forEach(err => {
    const lineNum = err.line;
    const lineElement = lineNumbersDiv.children[lineNum - 1];
    if (lineElement && err.severity === 'error') {
      lineElement.classList.add('error-line');
    }
  });

  // Breakpoint işaretlerini geri uygula
  if (typeof _refreshGutterBreakpoints === 'function') {
    _refreshGutterBreakpoints();
  }
}

// Kodu validate et ve hataları göster
function validateAndHighlightCode() {
  const codeEditor = document.getElementById('code-editor');
  const code = codeEditor.textContent;

  // Validate
  const errors = syntaxValidator.validate(code);

  // Hataları göster
  displayErrors(errors);
  highlightErrors(code, errors);
}

function displayErrors(errors) {
  const errorPanel = document.getElementById('error-panel');
  if (!errorPanel) return;

  errorPanel.innerHTML = '';
  if (errors.length === 0) {
    errorPanel.innerHTML = '<div class="no-errors">✅ Sözdizimi hatası yok</div>';
    return;
  }

  errors.forEach((error, index) => {
    const errorItem = document.createElement('div');
    errorItem.className = 'error-item ' + (error.severity || 'error');
    errorItem.dataset.line = error.line;
    errorItem.innerHTML = `
      <span class="error-icon">${error.severity === 'warning' ? '⚠️' : '🔴'}</span>
      <span class="error-location">Satır ${error.line}${error.column ? ':' + error.column : ''}</span>
      <span class="error-message">${error.message}</span>
    `;
    errorItem.addEventListener('click', () => {
      scrollToLine(error.line);
    });

    errorPanel.appendChild(errorItem);
  });
}

// Hataları kod içinde highlight et
// NOT: innerHTML yazmak contenteditable'ı bozuyor.
// Hata çizgileri satır numarası panelinde gösterilir (updateLineNumbers → error-line class).
function highlightErrors(code, errors) {
  // Sadece satır numarası panelini güncelle — code-editor'a dokunma.
  updateLineNumbers();
}

// Belirli satıra scroll et
function scrollToLine(lineNum) {
  const codeEditor = document.getElementById('code-editor');
  const lines = codeEditor.querySelectorAll('.code-line');

  if (lines[lineNum - 1]) {
    lines[lineNum - 1].scrollIntoView({ behavior: 'smooth', block: 'center' });
    lines[lineNum - 1].classList.add('highlight-flash');

    setTimeout(() => {
      lines[lineNum - 1].classList.remove('highlight-flash');
    }, 2000);
  }
}

// HTML escape
function escapeHtml(text) {
  const div = document.createElement('div');
  div.textContent = text;
  return div.innerHTML;
}

// Fonksiyonu highlight et ve scroll et
function highlightFunction(functionName) {
  if (!functionName) return;
  const codeEditor = document.getElementById('code-editor');
  const code = codeEditor.textContent;

  // event veya func keyword ile ara (multiline)
  const escaped = functionName.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
  const pattern = new RegExp(`^(?:event|func)\\s+${escaped}\\s*\\(`, 'm');
  const match = code.match(pattern);

  if (!match) {
    showStatus(`"${functionName}" kodu bulunamadı`);
    return;
  }

  // event/func satırının satır numarasını bul
  const lineNumber = code.substring(0, match.index).split('\n').length;

  // Scroll
  const viewCode = document.getElementById('view-code');
  const codeContainer = viewCode.querySelector('.code-container') || viewCode;
  codeContainer.scrollTop = Math.max(0, (lineNumber - 2) * 20);

  // Gövde başı: event satırından sonraki \n'den sonra
  const sigLineEnd = code.indexOf('\n', match.index + match[0].length);
  const bodyStart = sigLineEnd !== -1 ? sigLineEnd + 1 : code.length;

  // Gövde sonu: end_event veya } satırı
  const endEvtIdx = code.indexOf('\nend_event', bodyStart);
  const endBraceIdx = code.indexOf('\n}', bodyStart);
  let blockEnd = code.length;
  if (endEvtIdx !== -1 && (endBraceIdx === -1 || endEvtIdx < endBraceIdx)) blockEnd = endEvtIdx;
  else if (endBraceIdx !== -1) blockEnd = endBraceIdx;

  // Gövde içeriğinin sonuna git (varsa print("...") sonrası, yoksa boş satır)
  const bodyText = code.substring(bodyStart, blockEnd);
  const trimmedLen = bodyText.trimEnd().length;
  const cursorPos = bodyStart + trimmedLen;

  // İmleç konumlandır
  codeEditor.focus();
  const textNode = codeEditor.firstChild;
  if (textNode && textNode.nodeType === Node.TEXT_NODE && cursorPos <= textNode.length) {
    const range = document.createRange();
    range.setStart(textNode, cursorPos);
    range.collapse(true);
    const sel = window.getSelection();
    sel.removeAllRanges();
    sel.addRange(range);
  }

  showStatus(`${functionName} → satır ${lineNumber}`);
}

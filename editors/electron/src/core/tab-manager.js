// tab-manager.js -- syncEventCode, appendMissingEventStubs
// KOD sekmesi: tüm event'leri sıfırdan üret (ilk yükleme veya language değişimi)
function syncEventCode() {
  const el = document.getElementById('code-editor');
  if (!el) return;
  el.textContent = designer.generateEventCode();
  updateLineNumbers();
  validateAndHighlightCode();
}

// KOD sekmesi: SADECE eksik event stub'ları ekle (kullanıcı değişikliklerini koru)
function appendMissingEventStubs() {
  const el = document.getElementById('code-editor');
  if (!el) return;
  const handlers = designer.codeGenerator.collectEventHandlers(designer.widgets);
  if (handlers.length === 0) return;

  let current = el.textContent;
  // İlk açılışta sadece önceçeki 'event yok' placeholder varsa temizle
  if (current.includes('-- Olay işleyicisi yok') || current.trim() === '') {
    current = '';
  }

  let added = false;
  handlers.forEach(handler => {
    if (!current.includes(`event ${handler}(`)) {
      const stub = designer.codeGenerator.generateEventHandler(handler);
      current += (current.length > 0 && !current.endsWith('\n\n') ? '\n' : '') + stub;
      added = true;
    }
  });

  if (added) {
    el.textContent = current;
    updateLineNumbers();
    validateAndHighlightCode();
    // Cursor konumlandırmayı highlightFunction veya tab-click handler'a bırak
  }
}

// Olay işleyicilerini güncelle (önceki referanslar için)
function updateEventCode() { syncEventCode(); }

// Kod görünümünü güncelle (geriye uyumluluk)
function updateCodeView() { syncEventCode(); }

// Splitter kaldırıldı — boş stub
function initSplitter() {}


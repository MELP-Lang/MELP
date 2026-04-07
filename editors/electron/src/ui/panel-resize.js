/**
 * Panel Collapse + Resize
 * Sol (toolbox) ve sağ (özellikler) panellerin açılıp kapanmasını
 * ve sürüklenerek genişletilmesini yönetir.
 * Boyut ve durum localStorage'da saklanır.
 */
(function () {
  const leftPanel   = document.getElementById('left-panel');
  const rightPanel  = document.getElementById('right-panel');
  const leftResizer = document.getElementById('left-resizer');
  const rightResizer= document.getElementById('right-resizer');
  const leftBtn     = document.getElementById('left-collapse-btn');
  const rightBtn    = document.getElementById('right-collapse-btn');

  if (!leftPanel || !rightPanel) return; // guard

  const MIN_W = 150, MAX_W = 600;

  // ── Boyut geri yükle ────────────────────────────────────────
  const lw = parseInt(localStorage.getItem('melp-left-w'), 10);
  const rw = parseInt(localStorage.getItem('melp-right-w'), 10);
  if (lw >= MIN_W && lw <= MAX_W) leftPanel.style.width  = lw + 'px';
  if (rw >= MIN_W && rw <= MAX_W) rightPanel.style.width = rw + 'px';

  // ── Collapse yardımcıları ────────────────────────────────────
  function setCollapsed(panel, btn, side, collapsed) {
    panel.classList.toggle('panel-collapsed', collapsed);
    btn.textContent = collapsed
      ? (side === 'left' ? '›' : '‹')
      : (side === 'left' ? '‹' : '›');
    btn.title = collapsed ? 'Paneli Göster' : 'Paneli Gizle';
    const key = side === 'left' ? 'melp-left-collapsed' : 'melp-right-collapsed';
    localStorage.setItem(key, collapsed ? '1' : '0');
  }

  // Kaydedilmiş durum
  if (localStorage.getItem('melp-left-collapsed')  === '1') setCollapsed(leftPanel,  leftBtn,  'left',  true);
  if (localStorage.getItem('melp-right-collapsed') === '1') setCollapsed(rightPanel, rightBtn, 'right', true);

  leftBtn.addEventListener('click', () =>
    setCollapsed(leftPanel, leftBtn, 'left', !leftPanel.classList.contains('panel-collapsed')));

  rightBtn.addEventListener('click', () =>
    setCollapsed(rightPanel, rightBtn, 'right', !rightPanel.classList.contains('panel-collapsed')));

  // ── Sürükle-bırak yeniden boyutlandırma ─────────────────────
  function makeResizable(resizer, panel, side) {
    resizer.addEventListener('mousedown', (e) => {
      if (panel.classList.contains('panel-collapsed')) return;
      const startX = e.clientX;
      const startW = panel.offsetWidth;
      resizer.classList.add('is-dragging');
      document.body.style.cursor    = 'col-resize';
      document.body.style.userSelect = 'none';

      function onMove(e) {
        const dx   = side === 'left' ? (e.clientX - startX) : (startX - e.clientX);
        const newW = Math.max(MIN_W, Math.min(MAX_W, startW + dx));
        panel.style.width = newW + 'px';
        localStorage.setItem(side === 'left' ? 'melp-left-w' : 'melp-right-w', newW);
      }

      function onUp() {
        resizer.classList.remove('is-dragging');
        document.body.style.cursor     = '';
        document.body.style.userSelect = '';
        document.removeEventListener('mousemove', onMove);
        document.removeEventListener('mouseup',   onUp);
      }

      document.addEventListener('mousemove', onMove);
      document.addEventListener('mouseup',   onUp);
      e.preventDefault();
    });
  }

  makeResizable(leftResizer,  leftPanel,  'left');
  makeResizable(rightResizer, rightPanel, 'right');
})();

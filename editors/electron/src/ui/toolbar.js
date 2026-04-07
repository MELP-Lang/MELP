// toolbar.js -- filterProperties

function filterProperties(q) {
  const term = q.trim().toLowerCase();
  const container = document.getElementById('properties');
  if (!container) return;
  container.querySelectorAll('.property-group').forEach(group => {
    let visibleRows = 0;
    group.querySelectorAll('.property-row').forEach(row => {
      const match = !term || (row.dataset.filterLabel || '').includes(term);
      row.classList.toggle('props-filter-hidden', !match);
      if (match) visibleRows++;
    });
    // Grup başlığını da gizle/göster
    group.classList.toggle('props-filter-hidden', term !== '' && visibleRows === 0);
  });
}

/** Ayarlar dialogunu aç ve mevcut değerleri doldur */

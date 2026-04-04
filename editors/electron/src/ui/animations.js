// animations.js -- Widget Animasyon Sekmesi (v1.3)

const ANIM_TYPES = [
  { value: 'none',       label: '— Yok —' },
  { value: 'fadeIn',     label: '✨ Fade In' },
  { value: 'fadeOut',    label: '🌫 Fade Out' },
  { value: 'slideLeft',  label: '⬅ Slide Soldan' },
  { value: 'slideRight', label: '➡ Slide Sağdan' },
  { value: 'slideUp',    label: '⬆ Slide Yukarıdan' },
  { value: 'slideDown',  label: '⬇ Slide Aşağıdan' },
  { value: 'pulse',      label: '💗 Pulse' },
  { value: 'bounce',     label: '🏀 Bounce' },
  { value: 'shake',      label: '〰 Shake' },
  { value: 'zoom',       label: '🔍 Zoom In' },
];

const ANIM_TRIGGERS = [
  { value: 'onLoad',  label: '⚡ Yüklenince (onLoad)' },
  { value: 'onClick', label: '🖱 Tıklayınca (onClick)' },
  { value: 'onHover', label: '👆 Üzerine Gelince (onHover)' },
];

const ANIM_DURATIONS = [
  { value: '0.2', label: '0.2s (Çok Hızlı)' },
  { value: '0.3', label: '0.3s (Hızlı)' },
  { value: '0.5', label: '0.5s' },
  { value: '0.8', label: '0.8s' },
  { value: '1.0', label: '1.0s' },
  { value: '1.5', label: '1.5s (Yavaş)' },
];

const ANIM_EASES = [
  { value: 'ease',        label: 'ease' },
  { value: 'ease-in',     label: 'ease-in' },
  { value: 'ease-out',    label: 'ease-out (önerilen)' },
  { value: 'ease-in-out', label: 'ease-in-out' },
  { value: 'linear',      label: 'linear' },
];

let _animWidget = null;

function _getAnim(widget) {
  if (!widget) return { type: 'none', trigger: 'onLoad', duration: '0.5', easing: 'ease-out', repeat: false };
  return widget.properties._anim
    ? Object.assign({ type: 'none', trigger: 'onLoad', duration: '0.5', easing: 'ease-out', repeat: false }, widget.properties._anim)
    : { type: 'none', trigger: 'onLoad', duration: '0.5', easing: 'ease-out', repeat: false };
}

function _saveAnim(widget, anim) {
  widget.properties._anim = { ...anim };
  _updateAnimBadge(widget);
  if (typeof designer !== 'undefined') designer.onWidgetChange?.();
}

function _updateAnimBadge(widget) {
  if (!widget || !widget.element) return;
  const anim = widget.properties._anim;
  const hasAnim = !!(anim && anim.type && anim.type !== 'none');
  widget.element.classList.toggle('has-animation', hasAnim);
}

function _makeSelect(options, currentValue, onChangeCb) {
  const sel = document.createElement('select');
  sel.className = 'property-select';
  options.forEach(opt => {
    const o = document.createElement('option');
    o.value = opt.value;
    o.textContent = opt.label;
    if (opt.value === currentValue) o.selected = true;
    sel.appendChild(o);
  });
  sel.addEventListener('change', () => onChangeCb(sel.value));
  return sel;
}

function _makeRow(labelText, inputEl) {
  const row = document.createElement('div');
  row.className = 'property-row';
  const label = document.createElement('label');
  label.className = 'property-label';
  label.textContent = labelText;
  row.appendChild(label);
  row.appendChild(inputEl);
  return row;
}

function _setRowsDisabled(rows, disabled) {
  rows.forEach(r => {
    r.style.opacity = disabled ? '0.4' : '1';
    r.querySelectorAll('select,input,button').forEach(el => { el.disabled = disabled; });
  });
}

function _renderPreviewSection(panel, widgetId, getAnim) {
  const existing = panel.querySelector('.anim-preview-section');
  if (existing) existing.remove();

  const anim = getAnim();
  if (anim.type === 'none') return;

  const sec = document.createElement('div');
  sec.className = 'property-group anim-preview-section';

  const title = document.createElement('div');
  title.className = 'property-group-title';
  title.textContent = 'Önizleme';
  sec.appendChild(title);

  const btn = document.createElement('button');
  btn.className = 'anim-preview-btn';
  btn.textContent = '▶ Tasarım görünümünde oynat';
  btn.title = 'Animasyonu tasarım kanvası üzerinde önizle';
  btn.addEventListener('click', () => {
    const el = document.getElementById(widgetId);
    if (!el) return;
    const a = getAnim();
    el.classList.remove('anim-playing');
    void el.offsetWidth; // reflow — animasyonu sıfırla
    el.style.setProperty('--anim-type-name', `melp-${a.type}`);
    el.style.setProperty('--anim-type-dur',  (a.duration || '0.5') + 's');
    el.style.setProperty('--anim-type-ease', a.easing || 'ease-out');
    el.classList.add('anim-playing');
    const durMs = Math.round(parseFloat(a.duration || 0.5) * 1000);
    setTimeout(() => el.classList.remove('anim-playing'), durMs + 150);
  });
  sec.appendChild(btn);
  panel.appendChild(sec);
}

// Animasyon panelini seçili widget için göster
function showAnimationFor(widget) {
  _animWidget = widget;
  const panel = document.getElementById('anim-panel');
  if (!panel) return;
  panel.innerHTML = '';

  if (!widget) {
    const msg = document.createElement('div');
    msg.className = 'no-selection';
    msg.textContent = 'Bir widget seçin';
    panel.appendChild(msg);
    return;
  }

  const anim = _getAnim(widget);

  // Widget bilgi başlığı
  const hdr = document.createElement('div');
  hdr.className = 'prop-widget-header';
  hdr.style.cssText = 'padding:8px;background-color:#2d2d30;margin-bottom:12px;border-radius:3px;';
  hdr.innerHTML = `<strong style="color:#cccccc;">🎬 Animasyon</strong>
    <div style="margin-top:4px;font-size:11px;color:#888;">${widget.id}</div>`;
  panel.appendChild(hdr);

  // Grup: Animasyon Ayarları
  const grp = document.createElement('div');
  grp.className = 'property-group';

  const grpTitle = document.createElement('div');
  grpTitle.className = 'property-group-title';
  grpTitle.textContent = 'Animasyon Ayarları';
  grp.appendChild(grpTitle);

  // Animasyon Tipi
  const typeSel = _makeSelect(ANIM_TYPES, anim.type, (v) => {
    anim.type = v;
    _saveAnim(widget, anim);
    const disableRest = v === 'none';
    _setRowsDisabled([triggerRow, durRow, easeRow, repeatRow], disableRest);
    _renderPreviewSection(panel, widget.id, () => _getAnim(widget));
  });
  const typeRow = _makeRow('Tip', typeSel);
  grp.appendChild(typeRow);

  // Tetikleyici
  const triggerSel = _makeSelect(ANIM_TRIGGERS, anim.trigger, (v) => {
    anim.trigger = v;
    _saveAnim(widget, anim);
  });
  const triggerRow = _makeRow('Tetikleyici', triggerSel);
  grp.appendChild(triggerRow);

  // Süre
  const durSel = _makeSelect(ANIM_DURATIONS, anim.duration || '0.5', (v) => {
    anim.duration = v;
    _saveAnim(widget, anim);
  });
  const durRow = _makeRow('Süre', durSel);
  grp.appendChild(durRow);

  // Geçiş (easing)
  const easeSel = _makeSelect(ANIM_EASES, anim.easing || 'ease-out', (v) => {
    anim.easing = v;
    _saveAnim(widget, anim);
  });
  const easeRow = _makeRow('Geçiş', easeSel);
  grp.appendChild(easeRow);

  // Tekrar
  const repeatChk = document.createElement('input');
  repeatChk.type = 'checkbox';
  repeatChk.className = 'property-input';
  repeatChk.checked = !!anim.repeat;
  repeatChk.addEventListener('change', () => {
    anim.repeat = repeatChk.checked;
    _saveAnim(widget, anim);
  });
  const repeatRow = _makeRow('Sonsuz Tekrar', repeatChk);
  grp.appendChild(repeatRow);

  panel.appendChild(grp);

  // Eğer tipi "none" ise diğer satırları pasif göster
  if (anim.type === 'none') {
    _setRowsDisabled([triggerRow, durRow, easeRow, repeatRow], true);
  }

  // Önizleme butonu
  _renderPreviewSection(panel, widget.id, () => _getAnim(widget));
}

// Panel temizle (widget seçimi kaldırıldığında)
function clearAnimationPanel() {
  _animWidget = null;
  showAnimationFor(null);
}

// Animasyon paneli sekmelerini başlat
function initAnimationPanel() {
  const tabProps = document.getElementById('right-tab-props');
  const tabAnim  = document.getElementById('right-tab-anim');
  const propsContainer = document.getElementById('properties');
  const animPanel      = document.getElementById('anim-panel');
  const searchWrap     = document.getElementById('props-search-wrap');

  if (!tabProps || !tabAnim || !propsContainer || !animPanel) return;

  tabProps.addEventListener('click', () => {
    tabProps.classList.add('active');
    tabAnim.classList.remove('active');
    propsContainer.classList.remove('hidden');
    animPanel.classList.add('hidden');
    if (searchWrap) searchWrap.classList.remove('hidden');
  });

  tabAnim.addEventListener('click', () => {
    tabAnim.classList.add('active');
    tabProps.classList.remove('active');
    propsContainer.classList.add('hidden');
    animPanel.classList.remove('hidden');
    if (searchWrap) searchWrap.classList.add('hidden');
    // Seçili widget için animasyon UI'ını render et
    showAnimationFor(_animWidget);
  });

  // Başlangıç durumu
  clearAnimationPanel();

  // Mevcut widgetlerin badge'larını güncelle
  if (typeof designer !== 'undefined') {
    designer.widgets.forEach(w => _updateAnimBadge(w));
  }
}

// Preview klonuna animasyonları uygula — updatePreviewView() içinden çağrılır
function applyAnimationsToPreview(cloneRoot) {
  if (typeof designer === 'undefined') return;
  designer.widgets.forEach(w => {
    const anim = w.properties._anim;
    if (!anim || anim.type === 'none') return;

    const el = cloneRoot.querySelector('#' + w.id);
    if (!el) return;

    const dur   = (parseFloat(anim.duration) || 0.5) + 's';
    const ease  = anim.easing || 'ease-out';
    const count = anim.repeat ? 'infinite' : '1';

    if (anim.trigger === 'onLoad') {
      el.style.animationName            = `melp-${anim.type}`;
      el.style.animationDuration        = dur;
      el.style.animationTimingFunction  = ease;
      el.style.animationIterationCount  = count;
      el.style.animationFillMode        = 'both';

    } else if (anim.trigger === 'onClick') {
      el.style.setProperty('--melp-anim-name',  `melp-${anim.type}`);
      el.style.setProperty('--melp-anim-dur',   dur);
      el.style.setProperty('--melp-anim-ease',  ease);
      el.style.setProperty('--melp-anim-count', count);
      el.addEventListener('click', () => {
        el.classList.remove('melp-anim-run');
        void el.offsetWidth;
        el.classList.add('melp-anim-run');
        const durMs = Math.round(parseFloat(anim.duration || 0.5) * 1000);
        setTimeout(() => el.classList.remove('melp-anim-run'), durMs + 150);
      });

    } else if (anim.trigger === 'onHover') {
      el.style.setProperty('--melp-anim-name',  `melp-${anim.type}`);
      el.style.setProperty('--melp-anim-dur',   dur);
      el.style.setProperty('--melp-anim-ease',  ease);
      el.style.setProperty('--melp-anim-count', count);
      el.classList.add('melp-anim-hover');
    }
  });
}

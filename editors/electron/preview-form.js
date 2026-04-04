// preview-form.js — Masaüstü form önizleme penceresi renderer'ı
window.addEventListener('DOMContentLoaded', async () => {
  if (!window.previewAPI) return;

  const data = await window.previewAPI.getFormData();
  if (!data) {
    document.body.innerHTML = '<p style="color:#aaa;padding:24px;font-family:sans-serif;">Veri yüklenemedi.</p>';
    return;
  }

  const { canvasHtml, canvasSettings } = data;
  const cs = canvasSettings || {};
  const w  = cs.width  || 800;
  const h  = cs.height || 600;
  const bg = cs.backgroundColor || '#1e1e1e';

  if (cs.title) document.title = cs.title;

  const canvas = document.getElementById('form-canvas');
  canvas.style.width           = w + 'px';
  canvas.style.height          = h + 'px';
  canvas.style.backgroundColor = bg;

  // HTML'i güvenli şekilde yerleştir
  canvas.innerHTML = canvasHtml || '';

  // Tasarım modu öğelerini temizle
  canvas.querySelectorAll('.resize-handle, .canvas-info, .rubber-band').forEach(el => el.remove());
  canvas.querySelectorAll('.canvas-widget').forEach(el => {
    el.classList.remove('selected');
    el.style.outline   = '';
    el.style.boxShadow = '';
    el.style.cursor    = 'default';
  });

  // ── Etkileşimli widget davranışları ───────────────────────────────────────

  // Butonlar: basma/bırakma animasyonu
  canvas.querySelectorAll('.canvas-widget.widget-button').forEach(btn => {
    btn.addEventListener('mousedown', () => btn.classList.add('preview-active'));
    btn.addEventListener('mouseup',   () => btn.classList.remove('preview-active'));
    btn.addEventListener('mouseleave',() => btn.classList.remove('preview-active'));
    btn.style.cursor = 'pointer';
  });

  // Checkbox: toggle efekti
  canvas.querySelectorAll('.canvas-widget.widget-checkbox').forEach(cb => {
    cb.style.cursor = 'pointer';
    cb.addEventListener('click', () => cb.classList.toggle('preview-checked'));
  });

  // Textbox: gerçek <input> elementine dönüştür
  canvas.querySelectorAll('.canvas-widget.widget-textbox').forEach(w => {
    const content = w.querySelector('.widget-content');
    const initialText = content ? content.textContent : '';
    if (content) content.remove();
    const inp = document.createElement('input');
    inp.type = 'text';
    inp.value = '';
    inp.placeholder = initialText || '';
    inp.style.cssText = 'width:100%;height:100%;box-sizing:border-box;background:transparent;border:none;outline:none;color:inherit;font-size:inherit;font-family:inherit;padding:2px 6px;';
    w.style.pointerEvents = 'auto';
    w.style.cursor = 'text';
    w.appendChild(inp);
  });

  // ── Animasyonları uygula ───────────────────────────────────────────────
  const animationData = data.animationData || [];
  animationData.forEach(({ id, anim }) => {
    if (!anim || anim.type === 'none') return;
    const el = canvas.querySelector('#' + id);
    if (!el) return;

    const dur    = (parseFloat(anim.duration) || 0.5) + 's';
    const durMs  = Math.round((parseFloat(anim.duration) || 0.5) * 1000);
    const ease   = anim.easing  || 'ease-out';
    const count  = anim.repeat  ? 'infinite' : '1';
    const name   = 'melp-' + anim.type;

    if (anim.trigger === 'onLoad') {
      el.style.animationName           = name;
      el.style.animationDuration       = dur;
      el.style.animationTimingFunction = ease;
      el.style.animationIterationCount = count;
      el.style.animationFillMode       = 'both';

    } else if (anim.trigger === 'onClick') {
      el.style.setProperty('--melp-anim-name',  name);
      el.style.setProperty('--melp-anim-dur',   dur);
      el.style.setProperty('--melp-anim-ease',  ease);
      el.style.setProperty('--melp-anim-count', count);
      el.addEventListener('click', () => {
        el.classList.remove('melp-anim-run');
        void el.offsetWidth; // reflow
        el.classList.add('melp-anim-run');
        setTimeout(() => el.classList.remove('melp-anim-run'), durMs + 150);
      });

    } else if (anim.trigger === 'onHover') {
      el.style.setProperty('--melp-anim-name',  name);
      el.style.setProperty('--melp-anim-dur',   dur);
      el.style.setProperty('--melp-anim-ease',  ease);
      el.style.setProperty('--melp-anim-count', count);
      el.classList.add('melp-anim-hover');
    }
  });
});

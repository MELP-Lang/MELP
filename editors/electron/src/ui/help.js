// ==================== YARDIM MENÜSÜ ====================
// (son projeler: bkz. src/core/project-manager.js)

function handleQuickstart() {
  const message = `
🚀 MLP GUI Designer - Hızlı Başlangıç

1️⃣ Widget Ekleme:
   - Sol panelden widget seçin
   - Çalışma alanına sürükleyin

2️⃣ Widget Düzenleme:
   - Taşımak için sürükleyin
   - Köşelerden boyutlandırın
   - Sağ panelde özellikleri düzenleyin

3️⃣ Event Handler:
   - Widget seçin
   - Sağ panel > Olaylar
   - Fonksiyon adı girin

4️⃣ Kod Üretme:
   - Ctrl+E veya Dosya > Dışa Aktar
   - .mlp dosyası kaydedin
   - mlpc ile derleyin

5️⃣ Klavye Kısayolları:
   - Ctrl+S: Kaydet
   - Ctrl+N: Yeni
   - Ctrl+O: Aç
   - Delete: Sil

Detaylı bilgi için Yardım > Dokümantasyon
  `;
  alert(message);
}

function handleKeyboardShortcuts() {
  const message = `
⌨️ Klavye Kısayolları

📁 Dosya:
  Ctrl+N  - Yeni proje
  Ctrl+O  - Proje aç
  Ctrl+S  - Kaydet
  Ctrl+E  - MLP kodu dışa aktar
  F5      - Kodu göster/çalıştır

✂️ Düzenle:
  Ctrl+Z  - Geri al
  Ctrl+Y  - Yinele
  Ctrl+X  - Kes
  Ctrl+C  - Kopyala
  Ctrl+V  - Yapıştır
  Delete  - Sil

👁️ Görünüm:
  +       - Yakınlaştır
  -       - Uzaklaştır

🖱️ Widget İşlemleri:
  Sürükle      - Taşı
  Köşe Sürükle - Boyutlandır
  Çift Tık     - Kod görünümüne geç
  Delete       - Sil
  `;
  alert(message);
}

async function handleMlpFunctions() {
  try {
    // mlp_functions.json'dan fonksiyonları yükle
    const response = await fetch('./mlp_functions.json');
    const data = await response.json();

    // Modal dialog oluştur
    const modal = document.createElement('div');
    modal.className = 'modal-overlay';
    modal.innerHTML = `
      <div class="modal-dialog" style="max-width: 900px; max-height: 80vh;">
        <div class="modal-header">
          <h2>🔧 MLP Fonksiyonları</h2>
          <button class="modal-close" onclick="this.closest('.modal-overlay').remove()">✖</button>
        </div>
        <div class="modal-body" style="overflow-y: auto; max-height: 60vh;">
          <div class="functions-search">
            <input type="text" id="function-search" placeholder="🔍 Fonksiyon ara..." style="width: 100%; padding: 8px; margin-bottom: 12px; font-size: 14px;">
          </div>
          <div id="functions-list"></div>
        </div>
      </div>
    `;
    document.body.appendChild(modal);

    // Fonksiyonları listele
    const listContainer = modal.querySelector('#functions-list');
    renderFunctions(data, listContainer);

    // Arama fonksiyonu
    const searchInput = modal.querySelector('#function-search');
    searchInput.addEventListener('input', (e) => {
      const query = e.target.value.toLowerCase();
      renderFunctions(data, listContainer, query);
    });

    // Modal dışına tıklanınca kapat
    modal.addEventListener('click', (e) => {
      if (e.target === modal) {
        modal.remove();
      }
    });

  } catch (error) {
    console.error('MLP Functions loading error:', error);
    alert('MLP fonksiyonları yüklenemedi. Dosya: mlp_functions.json');
  }
}

function renderFunctions(data, container, searchQuery = '') {
  container.innerHTML = '';

  // Kategorilere göre grupla
  for (const [categoryName, categoryData] of Object.entries(data.categories)) {
    const functions = categoryData.functions.filter(fn => {
      if (!searchQuery) return true;
      return fn.name.toLowerCase().includes(searchQuery) ||
             fn.description.toLowerCase().includes(searchQuery);
    });

    if (functions.length === 0) continue;

    // Kategori başlığı
    const categoryDiv = document.createElement('div');
    categoryDiv.style.marginBottom = '24px';
    categoryDiv.innerHTML = `
      <h3 style="color: var(--accent-color); border-bottom: 2px solid var(--accent-color); padding-bottom: 6px; margin-bottom: 12px;">
        ${categoryData.icon} ${categoryData.name}
      </h3>
    `;

    // Fonksiyonlar
    functions.forEach(fn => {
      const fnDiv = document.createElement('div');
      fnDiv.style.marginBottom = '16px';
      fnDiv.style.padding = '12px';
      fnDiv.style.backgroundColor = 'var(--bg-secondary)';
      fnDiv.style.borderRadius = '6px';
      fnDiv.style.borderLeft = '4px solid var(--accent-color)';

      fnDiv.innerHTML = `
        <div style="font-family: 'Courier New', monospace; font-weight: bold; color: var(--accent-color); margin-bottom: 6px;">
          ${fn.signature}
        </div>
        <div style="color: var(--text-secondary); margin-bottom: 8px; font-size: 13px;">
          ${fn.description}
        </div>
        ${fn.params && fn.params.length > 0 ? `
          <div style="font-size: 12px; margin-top: 8px;">
            <strong>Parametreler:</strong>
            <ul style="margin: 4px 0 0 20px; padding: 0;">
              ${fn.params.map(p => `
                <li><code>${p.name}</code> (${p.type}): ${p.description}</li>
              `).join('')}
            </ul>
          </div>
        ` : ''}
        ${fn.returns ? `
          <div style="font-size: 12px; margin-top: 6px;">
            <strong>Döndürür:</strong> (${fn.returns.type}) ${fn.returns.description}
          </div>
        ` : ''}
        ${fn.example ? `
          <details style="margin-top: 8px; font-size: 12px;">
            <summary style="cursor: pointer; color: var(--accent-color);">📝 Örnek Kod</summary>
            <pre style="background: var(--bg-tertiary); padding: 8px; border-radius: 4px; margin-top: 6px; overflow-x: auto;"><code>${fn.example}</code></pre>
          </details>
        ` : ''}
      `;

      categoryDiv.appendChild(fnDiv);
    });

    container.appendChild(categoryDiv);
  }

  // Sabitler varsa ekle
  if (data.constants && data.constants.length > 0) {
    const constantsDiv = document.createElement('div');
    constantsDiv.style.marginBottom = '24px';
    constantsDiv.innerHTML = `
      <h3 style="color: var(--accent-color); border-bottom: 2px solid var(--accent-color); padding-bottom: 6px; margin-bottom: 12px;">
        📌 Sabitler
      </h3>
    `;

    data.constants.forEach(c => {
      const constDiv = document.createElement('div');
      constDiv.style.marginBottom = '8px';
      constDiv.style.padding = '8px';
      constDiv.style.backgroundColor = 'var(--bg-secondary)';
      constDiv.style.borderRadius = '4px';

      constDiv.innerHTML = `
        <code style="color: var(--accent-color);">${c.name} = ${c.value}</code>
        <span style="color: var(--text-secondary); margin-left: 12px; font-size: 12px;">- ${c.description}</span>
      `;

      constantsDiv.appendChild(constDiv);
    });

    container.appendChild(constantsDiv);
  }

  if (container.children.length === 0) {
    container.innerHTML = '<p style="text-align: center; color: var(--text-tertiary); padding: 20px;">Sonuç bulunamadı.</p>';
  }
}

function handleAbout() {
  alert(`🎨 MELP — MLP GUI Designer\n\nElektron tabanlı görsel GUI tasarım aracı.\nMLP programlama dili için Figma seviyesi\narayüz tasarım ve kod üretim ortamı.\n\nGeliştirici: MLP Team\nLisans: MIT`);
}

function handleDocumentation() {
  const message = `
📚 MLP GUI Designer Dokümantasyonu

Dosya Konumu:
  mlp_gui_designer/README.md
  mlp_gui_designer/QUICKSTART.md
  mlp_gui_designer/ARCHITECTURE.md

Online Dokümantasyon:
  https://github.com/your-repo/MLP

İçerik:
  • Widget türleri ve özellikleri
  • Kod üretimi
  • Proje yönetimi
  • İleri seviye özellikler

Dokümantasyon dosyalarını bir metin editörü
veya markdown viewer ile açabilirsiniz.
  `;
  alert(message);
}


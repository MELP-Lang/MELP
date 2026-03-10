# Güvenlik Politikası

## Desteklenen Sürümler

| Sürüm | Güvenlik Güncellemesi |
|---|---|
| 0.1.x (Alfa) | ✅ |

## Güvenlik Açığı Bildirimi

**Lütfen güvenlik açıklarını GitHub Issues üzerinden kamuya açık olarak bildirmeyin.**

### Bildirme Yöntemi

Güvenlik açığını doğrudan e-posta ile bildirin:

- **E-posta:** [proje iletişim e-postası — yakında eklenecek]
- **Konu:** `[SECURITY] Kısa açıklama`

### Beklentiler

- Bildirimlere **72 saat** içinde yanıt verilmeye çalışılır
- Geçerli bulgular için **şeffaf bir düzeltme süreci** izlenir
- Açıklayan kişi, düzeltme yayına alınana kadar bulguyu gizli tutar (coordinated disclosure)
- Dilersen CHANGELOG'da teşekkür edilir

### Kapsamdaki Bileşenler

| Bileşen | Kapsam |
|---|---|
| `melp_compiler` — derleyici binary | ✅ Kapsamda |
| `mlp_runtime.bc` — runtime | ✅ Kapsamda |
| Örnek programlar | ⚠️ Alfa'da düşük öncelik |
| Belgeler | ❌ Kapsam dışı |

### Alfa Notu

Bu proje Alfa aşamasındadır. Güvenlik garantisi yoktur ve **üretim ortamında kullanılması önerilmez**. Bununla birlikte güvenlik bildirimlerine ciddiyetle yaklaşırız.

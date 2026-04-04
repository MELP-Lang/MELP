# MELP — Geçiş Belge Dizini

Bu ağaç yalnızca uyumluluk ve yönlendirme amacıyla tutulur.
Aktif ve tercih edilen Stage1 referans ağacı [../belgeler_v2/README.md](../belgeler_v2/README.md) dosyasındadır.

İngilizce sürüm → [README_EN.md](README_EN.md)

---

## Kanonik Başlangıç Noktaları

- [../belgeler_v2/README.md](../belgeler_v2/README.md) — Stage1 için birincil belge girişi.
- [../belgeler_v2/dil/referans.md](../belgeler_v2/dil/referans.md) — Kanonik sözdizimi referansı.
- [rehberler/ffi.md](rehberler/ffi.md) — Kısa FFI rehberi.
- [rehberler/bellek.md](rehberler/bellek.md) — Kısa manuel bellek rehberi.
- [rehberler/hata_yonetimi.md](rehberler/hata_yonetimi.md) — Kısa hata yönetimi rehberi.
- [KAPSAM.md](KAPSAM.md) — Eski ağaçtan yeni ağaca taşınan konuların haritası.

---

## Geçiş Notu

Bu dizindeki belgeler yeni içerik için otorite değildir.
Yeni Stage1 referansları mümkün olduğunda `belgeler_v2/` altında tutulmalıdır.

---

## Örnek (Türkçe MLP sözdizimi)

```
sayısal fonksiyon topla(a; b)
  a + b döndür
fonksiyon sonu

fonksiyon giriş()
  sayısal sonuç = topla(3; 5)
  yaz(sonuç)
fonksiyon sonu
```

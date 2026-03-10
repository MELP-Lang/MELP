# Katkıda Bulunma Kılavuzu

MELP'e katkıda bulunmak istediğiniz için teşekkür ederiz! Bu belge katkı sürecini açıklamaktadır.

---

## Katkı Türleri

| Tür | Nereye |
|---|---|
| **Hata bildirimi** | [Issues → Bug Report](../../issues/new?template=bug_report.md) |
| **Özellik isteği** | [Issues → Feature Request](../../issues/new?template=feature_request.md) |
| **Belge düzeltmesi** | PR açın |
| **Örnek program** | `examples/` altına ekleyip PR açın |
| **Derleyici geliştirme** | `stage0/v2` deposuna bakın (ayrı repo) |

---

## Örnek Program Katkısı (En Kolay Başlangıç)

`examples/` dizinine yeni bir program ekleyebilirsiniz:

```
examples/
├── base/         ← temel örnekler (değişken, döngü, fonksiyon...)
├── cli/          ← komut satırı araçları
├── stdlib/       ← standart kütüphane kullanımı
└── games/        ← küçük oyunlar
```

Kuralllar:
1. Program derlenmeli ve doğru çıktı vermeli
2. Dosya adı açıklayıcı olmalı: `02_fibonacci.mlp`
3. İlk satırda kısa açıklama yorum olarak: `-- Fibonacci dizisi`

---

## PR Süreci

1. Bu repoyu fork edin
2. Dal açın: `git checkout -b ornek-fibonacci`
3. Değişikliği yapın ve test edin
4. Commit: `git commit -m "examples: fibonacci dizisi eklendi"`
5. Fork'unuza push: `git push origin ornek-fibonacci`
6. PR açın — PR template otomatik yüklenecek

---

## Commit Mesajı Kuralları

Ön ek kullanın:

```
examples: fibonacci dizisi eklendi
docs: INSTALL.md türkçe açıklama düzeltildi
fix: print() null string hatası giderildi
feat: string interpolasyon eklendi
```

---

## Derleyici Katkısı

Derleyici kaynak kodu bu repoda değildir. Derleyici geliştirme:
- Repo: `stage0/v2` (şu an özel/internal)
- Alfa döneminde derleyici çekirdeğine dış katkı açılmamıştır
- Beta'dan itibaren derleyici kaynak kodu da açılacaktır

---

## İletişim

- **GitHub Issues** — hata ve özellik talepleri için
- **GitHub Discussions** — genel sorular ve tartışmalar için

---

## Davranış Kuralları

Tüm katkıcılar [Davranış Kurallarımıza](CODE_OF_CONDUCT.md) uymayı kabul eder.

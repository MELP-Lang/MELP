# YZ_Bunu_Oku.md

**Dikkat: Bu dizindeki tüm testler güncellenmiştir ve STAGE0'da derlenip çalışmaktadır.**

- `examples/en/golden/` altındaki tüm `.mlp` dosyaları, Stage0 golden test suite'inin güncel ve çalışan kopyasıdır.
- Her test, Stage0 derleyicisi ile derlenip başarıyla çalıştırılmıştır.
- Stage1 normalizer/derleyici/VSIX/araçları bu dosyalar üzerinde test edilirken, **herhangi bir testin başarısız olması Stage1 tarafında bir hata veya eksiklik olduğunu gösterir**.
- Testlerin tamamı İngilizce MLP (space-separated) formundadır.
- Herhangi bir otomatik dönüştürme, script veya normalizer güncellemesi sonrası bu dizindeki testlerin tamamı tekrar çalıştırılmalı ve %100 başarı beklenmelidir.

## Kural
- **Bu dizindeki testler STAGE0 ile %100 geçiyorsa, Stage1 pipeline da %100 geçmelidir.**
- Aksi durumda, Stage1 normalizer/araçlarında bug veya uyumsuzluk vardır.

---

> **YZ'ye Not:**
> - Bu dizindeki testler, MELP dilinin minimum garanti edilen semantik kapsamını temsil eder.
> - Stage1, Stage2, VSIX, normalizer, çeviri, Türkçe/diğer diller, yeni sözdizimi denemeleri vb. her şeyin temel referansı burasıdır.
> - Testler güncellendiğinde, eskiye dönük uyumluluk korunmalıdır.

---
description: "Stage1 dokümantasyonunu sistematik olarak temizlemek, eski MELP sözdizimi kaymalarını gidermek, belgeler_old'u aktif kapsamdan çıkarmak ve depoyu tek kanonik referans ağacına yaklaştırmak için kullanın."
---

# Stage1 Dokümantasyonunu Kanonikleştirme Görevi

`/home/pardus/PROJELER/MELP/LLVM/STAGE1` içinde çalışıyorsunuz.

## Görev

Stage1 dokümantasyonunu ve kullanıcıya açık örnek belgeleri sistematik olarak tarayın, aktif dokümantasyondaki eski veya tümevarımla üretilmiş MELP sözdizimlerini temizleyin ve depoyu tek aktif referans kaynağına doğru konsolide edin.

Bu iş, dar kapsamlı tek dosya düzeltmesi değil; depo genelinde bir dokümantasyon hijyeni görevidir.

## Ana Hedefler

1. Kullanıcıya açık Stage1 belgelerinden eski Stage0/PMLP sözdizimini temizlemek.
2. Gelecek YZ'lerin bayat dosyalardan sözdizimi türetmesini engellemek.
3. `belgeler_old/` dizinini aktif Stage1 dokümantasyon yüzeyinden çıkarmak.
4. Aktif referans kaynağını `belgeler_v2/` etrafında toplamak.
5. Kullanıcıya dönük belgeleri son kullanıcı odağında tutmak; AI devir notlarını ve iç süreç gürültüsünü dışarıda bırakmak.

## Tek Doğruluk Kaynağı

Anahtar kelime seçiminde aşağıdaki dosyaları tek doğruluk kaynağı kabul edin:

- `/home/pardus/PROJELER/MELP/ORTAK/dil/en/keywords.json`
- `/home/pardus/PROJELER/MELP/ORTAK/dil/tr/keywords.json`

Depo kuralları için aşağıdaki dosyaları bağlayıcı kabul edin:

- `/home/pardus/PROJELER/MELP/LLVM/STAGE1/.github/copilot-instructions.md`
- `/home/pardus/PROJELER/MELP/LLVM/STAGE1/.github/instructions/melp-canonical-language.instructions.md`

Depodaki mevcut dosyalar bu kaynaklarla çelişiyorsa, depo dosyalarını bayat kabul edin.

## Aktif Kapsam

En az şu kullanıcıya açık Stage1 belge alanlarını tarayın ve düzeltin:

- kök README ve kurulum belgeleri
- `belgeler/`
- `belgeler_v2/`
- `docs_EN/`
- `docs_TR/`
- `YARDIM/`
- `editors/` altındaki kullanıcıya açık belge dosyaları
- `examples/` altındaki README ve açıklama belgeleri

## Kapsam Dışı

- `belgeler_old/` aktif belge hedefi olarak kapsam dışıdır
- geçici AI devir notları
- kullanıcıya açık yüzeye sızmayan iç tarihsel talimat dökümleri
- dokümantasyon dışındaki ilgisiz kod değişiklikleri

`belgeler_old/` tarihsel bağlam için okunabilir; ancak aktif Stage1 referans yüzeyinin parçası olarak korunmamalıdır.

Not: Kullanıcı `belgeler_old/` içeriğini repo dışına taşıyabilir. Bu yüzden görev planınızı, bu dizin hiç yokmuş gibi de tamamlayabilecek şekilde kurun. `belgeler_old/` varlığına bağımlı bir aktif dokümantasyon stratejisi kurmayın.

## Sözdizimi Kuralları

### İngilizce kanonik örnekler

Boşluklu blok kapanışları kullanın:

- `end function`
- `end if`
- `end for`
- `end while`
- `end match`
- `end enum`
- `end struct`
- `else if`

### Türkçe kanonik örnekler

`tr/keywords.json` içindeki kanonik `keywords` eşleşmelerini kullanın. Örnek:

- `function -> fonksiyon`
- `end function -> fonksiyon sonu`
- `main -> giriş`
- `print -> yaz`
- `if -> koşul`
- `else -> yoksa`
- `end if -> koşul sonu`

Kullanıcı özellikle istemedikçe `ana`, `yazdır` gibi `aliases` değerlerini varsayılan belge dili olarak kullanmayın.

## Zorunlu Çıktılar

1. Aktif dokümantasyon dosyalarından hangilerinin hâlâ eski alt çizgili sözdizimi veya bayat anahtar kelime biçimleri içerdiğini envanterleyin.
2. Bu dosyaları kanonik sözdizimine güncelleyin.
3. `belgeler_old/` dizinini aktif Stage1 belge yüzeyinin parçası gibi gösteren anlatımları kaldırın.
4. Tek aktif referans ağacı bırakın. Varsayılan tercih `belgeler_v2/` olsun.
5. Arşiv referanslarını yalnızca açıkça arşiv/tarih etiketiyle koruyun.
6. Kullanıcıya açık belgelerden AI yönelimli talimatları, handoff dilini ve iç süreç gürültüsünü çıkarın.

## Çalışma Yöntemi

1. Depoda şu eski biçimleri arayın:
   - `end_function`
   - `end_if`
   - `end_while`
   - `end_for`
   - `else_if`
2. `tr/keywords.json` ile çelişen bayat Türkçe varsayılanları arayın.
3. Dosyaları şu kümelere ayırın:
   - hemen düzeltilecek kullanıcı belgeleri
   - kalabilir ama arşiv olarak etiketlenmesi gereken tarihsel belgeler
   - gelecekteki kamuya açık belgeleri yönlendirmemesi gereken iç dosyalar
4. Minimal ama sistematik düzenlemeler yapın.
5. Taramaları yeniden çalıştırıp aktif belge yüzeyinin temizlendiğini doğrulayın.

## Karar Kuralları

- `belgeler/` ve `belgeler_v2/` çelişirse, kullanıcı aksini istemedikçe aktif yüzeyi `belgeler_v2/` yönüne taşıyın.
- Bir dosya kullanıcıya açıksa ve içinde hem kanonik hem eski sözdizimi varsa, dosyayı tamamen normalize edin.
- Bir dosya iç tarihsel malzemeyse, yalnızca açıkça kanonik olmayan tarihsel içerik olarak çerçeveleniyorsa bırakın.
- Hangi ağacın otorite olduğu konusunda ikili mesaj bırakmayın.

## Teslimatlar

- temizlenmiş aktif Stage1 dokümantasyonu
- `belgeler_old/` için net arşiv statüsü
- gelecek YZ'lerin bayat belgelerden sözdizimi türetmesini zorlaştıran bir depo durumu
- nelerin değiştirildiğini, nelerin kaldığını ve tek aktif referans kaynağının neresi olduğunu özetleyen kısa bir sonuç notu

## Kabul Kriterleri

- kullanıcıya açık Stage1 belgeleri artık eski alt çizgili sözdizimini aktif sözdizim olarak öğretmiyor
- Türkçe örnekler `tr/keywords.json` ile uyumlu
- İngilizce örnekler kanonik Stage1 politikası ile uyumlu
- `belgeler_old/` aktif Stage1 referans kaynağı olarak sunulmuyor
- kullanıcı aksini istemedikçe `belgeler_v2/` tercih edilen kanonik belge ağacı olarak ele alınıyor
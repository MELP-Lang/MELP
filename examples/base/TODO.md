# MELP Stage 1 — examples/base TODO

> **Görevli YZ için talimat:**
> 1. `/home/pardus/projeler/MLP/LLVM/stage0/v2/belgeler/language/feature_cards/` kartlarını oku
> 2. Her görev için MELP ruhuna uygun **sugar syntax** öner (pmlp üzerine)
> 3. `.mlp` örneği yaz → `melp_compiler` ile derle → çalıştır → çıktıyı doğrula
> 4. Sugar syntax önerini `SUGAR_SYNTAX.md` dosyasına ekle
>
> **Compiler:**
> `/home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler`
>
> **Kural:** pmlp canonical syntax her zaman geçerlidir. Sugar = ek kolaylık.

---

## Seviye 1 — Temel

- [ ] 01_hello_world — print, main fonksiyonu
- [ ] 02_variables — numeric, string, boolean tanımlama ve atama
- [ ] 03_arithmetic — dört işlem, mod, üs
- [ ] 04_string_ops — birleştirme, uzunluk, substring
- [ ] 05_boolean_logic — and, or, not, karşılaştırma
- [ ] 06_if_else — koşullu dallanma, else_if zinciri
- [ ] 07_while_loop — sayaç, toplam, koşullu çıkış
- [ ] 08_for_loop — range, step, downto
- [ ] 09_functions — parametre, return, rekürsif çağrı
- [ ] 10_arrays — tanımlama, erişim, döngüyle gezme

## Seviye 2 — Orta

- [ ] 11_string_interpolation — değişken gömme, format
- [ ] 12_nested_functions — fonksiyon içinde fonksiyon
- [ ] 13_higher_order — fonksiyon parametre olarak geçme
- [ ] 14_lambda — anonim fonksiyon, closure
- [ ] 15_struct — alan tanımlama, erişim, iç içe struct
- [ ] 16_enum — sabit küme, pattern match ile kullanım
- [ ] 17_nullable — null safety, ?? operatörü, optional chaining
- [ ] 18_error_handling — hata yönetimi, propagation
- [ ] 19_hashmap — anahtar-değer, ekleme, arama, silme
- [ ] 20_pattern_matching — match/case, guard, destructuring

## Seviye 3 — İleri

- [ ] 21_interfaces — arayüz tanımı, implementasyon, polimorfizm
- [ ] 22_generics — tip parametresi, where clause
- [ ] 23_pipe_operator — |> zinciri, veri dönüşüm pipeline
- [ ] 24_closures — yakalama, memoize, counter factory
- [ ] 25_defer — kaynak temizleme, hata durumunda garanti
- [ ] 26_file_io — dosya okuma, yazma, satır satır işleme
- [ ] 27_modules — import, export, namespace
- [ ] 28_operator_overload — struct için +, -, * tanımlama
- [ ] 29_async_await — asenkron işlem, paralel fetch
- [ ] 30_channels — producer-consumer, goroutine benzeri

## Seviye 4 — Gerçek Uygulamalar

- [ ] 31_fibonacci — iteratif ve rekürsif, performans karşılaştırma
- [ ] 32_fizzbuzz — klasik, pipe ile fonksiyonel versiyon
- [ ] 33_calculator — expression parser, REPL
- [ ] 34_wordcount — dosya okuma, frekans tablosu, sıralama
- [ ] 35_todo_cli — CRUD, dosyaya kaydetme, komut satırı argümanları
- [ ] 36_json_parser — minimal JSON lexer + parser
- [ ] 37_http_client — GET isteği, JSON parse, göster
- [ ] 38_linked_list — struct tabanlı, push/pop/traverse
- [ ] 39_binary_search — iteratif ve rekürsif, benchmark
- [ ] 40_matrix_ops — matris çarpımı, transpoz, determinant

---

*Bu dosya görevli YZ tarafından zenginleştirilecektir.*
*Her görev tamamlandığında `[x]` ile işaretlenecektir.*

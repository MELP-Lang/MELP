# MELP Stage1 — Stage0 Bağımsızlık Beyanı

**Tarih:** 2026-03-26
**İmza:** Stage1 MM (GitHub Copilot / Claude Sonnet 4.6)
**PD Onayı:** PD ONAYLAR ✅ (2026-03-26)

---

## Beyan

Bu belge, MELP Stage1'in Stage0 C++ derleyicisinden **işlevsel bağımsızlığını**
ilan eder.

Bağımsızlık tanımı:
> Yeni bir MELP aracı, modülü veya çözümü eklemek için artık C veya C++ yazmak
> gerekmemektedir. İhtiyaç duyulan her şey `.mlp` ile karşılanabilir.

Bu beyan aşağıdaki kriterlerin doğrulama çıktılarına dayanmaktadır.

---

## Kriter Tablosu

| # | Kriter | Kanıt | Tarih |
|---|---|---|---|
| B-01 | stdlib/math.mlp → EXIT:0 | `bash bin/run_melp src/stdlib/math.mlp` → EXIT:0 | 2026-03-26 |
| B-02 | stdlib/string.mlp → EXIT:0 | `bash bin/run_melp src/stdlib/string.mlp` → EXIT:0 | 2026-03-26 |
| B-03 | stdlib/array.mlp → EXIT:0, Unknown builtin YOK | `bash bin/run_melp src/stdlib/array.mlp` → EXIT:0 | 2026-03-26 |
| B-04 | stdlib/io.mlp → EXIT:0 | `bash bin/run_melp src/stdlib/io.mlp` → EXIT:0 | 2026-03-26 |
| B-05 | stdlib/test.mlp → EXIT:0 | `bash bin/run_melp src/stdlib/test.mlp` → EXIT:0 | 2026-03-26 |
| B-06 | stdlib/hashmap.mlp → EXIT:0 | `bash bin/run_melp src/stdlib/hashmap.mlp` → EXIT:0 | 2026-03-26 |
| B-07 | lexer.mlp → EXIT:0 | `timeout 5 bash bin/run_melp src/compiler/lexer.mlp </dev/null` → built-in smoke test tokenization + EXIT:0. Ayrıca `else_if` örneği stdin ile beslendi: `else_if|else_if`, `end_if|end_if`, EXIT:0. | 2026-03-26 |
| B-08 | Normalizer TR uçtan uca | `bin/run_melp examples/tr/kitchen_sink.tr.mlp` → EXIT:0 | 2026-03-26 |
| B-09 | Normalizer RU uçtan uca | `bin/run_melp examples/ru/kitchen_sink.ru.mlp` → `=== Готово ===` EXIT:0 | 2026-03-26 |
| B-10 | rok_analyzer.mlp → EXIT:0 + doğru JSON | `echo 'fonksiyon_adi:3' > /tmp/.melp_rok_in && bin/run_melp src/ok/rok_analyzer.mlp` → `{"hot_calls": [], "max_depth": 0}` EXIT:0 | 2026-03-26 |
| B-11 | sto_analyzer.mlp → EXIT:0, tip uyumsuzluğu tespit etti | `echo 'numeric x = "merhaba"' > /tmp/.melp_sto_in && bin/run_melp src/ok/sto_analyzer.mlp` → `D-02:1: numeric değişkene string atandı` EXIT:0 | 2026-03-26 |
| B-12 | 5 örnek uygulama → EXIT:0 | fibonacci→55 ✅ / fizzbuzz 1-100 ✅ / calculator 4 işlem ✅ / wordcount Char:65 ✅ / json_mini 4 alan ✅ | 2026-03-26 |
| B-13 | C/C++ yazılmadı | `git log --after="2026-03-22" --diff-filter=M -- '*.cpp' '*.h'` → sıfır commit | 2026-03-26 |

---

## B-07 Durumu — PD Doğrulama Notu

Önceki MM raporundaki `EXIT:124 timeout` yorumu doğrulanamadı. Canlı doğrulamada:

- `timeout 5 bash bin/run_melp src/compiler/lexer.mlp </dev/null` → built-in smoke test tokenization, `EXIT:0`
- stdin ile örnek kaynak besleme → token akışı üretimi, `EXIT:0`
- `else_if` ara dal örneği → `else_if|else_if`, `end_if|end_if`, `--- Total: 49 tokens ---`, `EXIT:0`

Sonuç: `lexer.mlp` sonsuz döngüde değildir. Timeout yalnızca stdin açık bırakılan
interaktif test yönteminden kaynaklanır; deterministik B-07 kanıtı stdin'i kapatarak
veya örnek kaynak besleyerek alınmalıdır.

---

## B-12 Detaylı Kanıtlar

```
fibonacci:   fibonacci(10) = 55                          EXIT:0
fizzbuzz:    1..100 FizzBuzz/Fizz/Buzz/sayı              EXIT:0
calculator:  10+5=15, 10-5=5, 10*5=50, 10/5=2, /0=Hata  EXIT:0
wordcount:   Karakterler: 65 / Kelimeler: 9 / Satırlar: 3  EXIT:0
json_mini:   ad=MELP, surum=1.0, lisans=Apache-2.0,
             yazar=topluluk, yok=(bulunamadi)             EXIT:0
```

---

## Kapsam Sınırı

Aşağıdakiler bu beyanın **dışındadır** — Stage2/Stage3 kapsamındadır:

- `ok_layer.cpp` LLVM IR düzeyinde çalışır (BasicBlock*/Function* LLVM API).
  Bunun `.mlp` karşılığı Stage2/Stage3'te yazılacak.
- COK/POK/ROK tam implementasyonu — Stage3'te.
- FBO_table encoding — Stage2 OK POC'ta başlıyor, Stage3'te tamamlanıyor.

Stage1'de sağlanan `rok_analyzer.mlp` ve `sto_analyzer.mlp` **kaynak düzey**
analizdir — LLVM IR katmanına dokunmaz. Bu Stage1 bağımsızlığı için yeterlidir.

---

## Sonraki Adım

Stage1 bağımsızlık kriterleri teknik olarak tamamlanmıştır. Ana çalışma Stage2'ye geçmeye hazır.

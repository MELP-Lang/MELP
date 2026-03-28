# PD TALİMAT — EDITORS/VSIX/001
**Tarih:** 2026-03-18 (Güncellendi: 2026-03-19 — Görev 1 mimarisi revize edildi)
**Yazan:** PD (Proje Direktörü)
**Muhatap:** EDITORS/VSIX MM
**Konu:** .mlp İngilizce pass + binary senkronizasyon + sürüm güncelleme
**Öncelik:** KRİTİK

---

## Bağlam

2026-03-18 PD kararı: İngilizce `.mlp` dosyaları TR/RU keyword çevirisinden bağımsız olmalı.

**2026-03-19 MİMARİ REVİZYONU — Görev 1:**

VSIX MM'in tespiti doğrudur. Normalizer iki farklı iş yapıyor:

1. **MLP→PMLP syntax dönüşümü** (dil bağımsız, her zaman gerekli):
   - `end function` → `end_function`
   - `end if` → `end_if`
   - `loop` → `while`
   - `end loop` → `end_while`
   - `else if` → `else_if`

2. **TR/RU keyword çevirisi** (dile özgü, sadece .tr.mlp ve .ru.mlp için):
   - `fonksiyon` → `function`
   - `eğer` → `if`
   - `yazdır` → `print`
   - vb.

STAGE0 yalnızca PMLP syntax'ını anlıyor. Dolayısıyla İngilizce `.mlp` dosyaları da 1. adımdan **geçmek zorunda**. Tam bypass yapılırsa İngilizce MLP derlenmez.

**PD Kararı: Seçenek B — İki pass ayrımı**

```
.tr.mlp → [TR keyword çevirisi + MLP→PMLP syntax dönüşümü]  →  STAGE0
.ru.mlp → [RU keyword çevirisi + MLP→PMLP syntax dönüşümü]  →  STAGE0
.mlp    → [SADECE MLP→PMLP syntax dönüşümü]                 →  STAGE0
```

Bu ayrım normalizer içinde `lang` parametresiyle zaten kontrol ediliyor. `english` modda normalizer TR/RU keyword çevirisi yapmıyor — yalnızca syntax normalizasyonu yapıyor. Bu **mevcut normalizer davranışı** — değiştirmek gerekmez.

**Sonuç:** Görev 1'deki "bypass" ifadesi yanlıştı. Doğrusu: `.mlp` dosyaları normalizer'dan geçecek, ancak `lang=english` modda (TR/RU çevirisi olmadan, yalnızca syntax normalizasyonu ile).

Referans: `MELP/PROJE_MİMARİSİ.md` güncellenecek — PD yapacak.

---

## GÖREV 1 — `.mlp` İçin `lang=english` Zorunlu Yap (mlpCompiler.ts)

**Dosya:** `EDITORS/VSIX/src/mlpCompiler.ts`

**Mevcut kod (satır 160-179):**
```typescript
const lang = options.sourcePath.endsWith('.tr.mlp')
    ? 'turkish'
    : options.sourcePath.endsWith('.ru.mlp')
        ? 'russian'
        : 'english';
// normalizer her zaman çalışıyor — BU DOĞRU DAVRANIŞDIR
```

**Yapılacak değişiklik:** Mevcut kod zaten doğru — `.mlp` için `lang='english'` atıyor ve normalizer çalışıyor. **Değişiklik gerekmez.**

Ancak doğrulama gerekiyor: normalizer `english` modda çalışırken gerçekten yalnızca syntax normalizasyonu mu yapıyor, TR/RU keyword çevirisi var mı?

**Test et:**
```bash
echo 'numeric x = 42
if x > 10 then
  print("big")
end if' > /tmp/en_test.mlp

# normalizer english modda çalıştır
cp /tmp/en_test.mlp /tmp/.melp_norm_src
echo "english" > /tmp/.melp_norm_lang
/home/pardus/PROJELER/MELP/ORTAK/normalizer/melp_normalizer_bin > /tmp/en_test_out.pmlp

cat /tmp/en_test_out.pmlp
```

**Beklenen çıktı:**
```
numeric x = 42
if x > 10 then
  print("big")
end_if
```

(`end if` → `end_if` dönüşümü olmalı. TR/RU keyword'leri girmemeli.)

Bu testi raporla. Çıktı beklenenden farklıysa PD'ye eskalasyon yap.

---

## GÖREV 2 — debugAdapter.ts Doğrulama

**Dosya:** `EDITORS/VSIX/src/debugAdapter.ts`

Normalizer çağrısının `lang` parametresini doğrula. `.mlp` için `english` kullanılıyor mu?

```bash
grep -n "normalizer\|\.tr\.\|\.ru\.\|lang\b" src/debugAdapter.ts
```

ile kontrol et. Görev 1 ile aynı mantık var mı? Yoksa düzelt.

---

## GÖREV 3 — sync-binary.sh'a Normalizer Ekle

**Dosya:** `EDITORS/VSIX/scripts/sync-binary.sh`

Mevcut script sadece `melp_compiler_linux` ve `mlp_runtime.bc` kopyalıyor. Normalizer binary eksik.

**Eklenecek kaynak:**
```
ORTAK/normalizer/melp_normalizer_bin
```

**Hedef:**
```
EDITORS/VSIX/bin/melp_normalizer_bin
```

Mevcut `for ITEM in "melp_compiler_linux:$STAGE0_BINARY" "mlp_runtime.bc:$STAGE0_RUNTIME"` döngüsüne normalizer'ı da ekle. Kaynak yolu:
```bash
NORMALIZER_SRC="/home/pardus/PROJELER/MELP/ORTAK/normalizer/melp_normalizer_bin"
```

Script'in dry-run / --apply mantığı korunacak.

---

## GÖREV 4 — sync-binary.sh --apply Çalıştır

Görev 3 tamamlandıktan sonra:

```bash
cd /home/pardus/PROJELER/MELP/EDITORS/VSIX
bash scripts/sync-binary.sh --apply
```

Çıktıda şunları doğrula:
- `melp_compiler_linux` → güncel
- `mlp_runtime.bc` → güncel
- `melp_normalizer_bin` → güncel (yeni eklendi)

---

## GÖREV 5 — .stage0-dep Güncelle

**Dosya:** `EDITORS/VSIX/.stage0-dep`

Mevcut: `"stage0_version": "1.0.5"`
Güncel STAGE0: `1.0.11`

```json
{
  "stage0_version": "1.0.11",
  ...
  "last_synced": "2026-03-18"
}
```

---

## GÖREV 6 — VSIX Rebuild

TypeScript değişiklikleri (Görev 1-2) sonrası:

```bash
cd /home/pardus/PROJELER/MELP/EDITORS/VSIX
npm run build
```

Hata yoksa devam et.

---

## GÖREV 7 — Doğrulama Testleri

```bash
S0C=/home/pardus/PROJELER/MELP/LLVM/STAGE0/build/compiler/stage1/modules/orchestrator/melp_compiler
NORM=/home/pardus/PROJELER/MELP/ORTAK/normalizer/melp_normalizer_bin

# Test 1 — İngilizce .mlp direkt STAGE0'a gidiyor mu?
echo 'numeric x = 42
print("Hello " + str(x))' > /tmp/vsix_t1.mlp
$S0C /tmp/vsix_t1.mlp -o /tmp/vsix_t1.ll 2>&1 && \
clang-19 /tmp/vsix_t1.ll -lm -o /tmp/vsix_t1_bin && \
/tmp/vsix_t1_bin
# Beklenen: Hello 42

# Test 2 — Türkçe .tr.mlp normalizer'dan geçiyor mu?
echo 'sayısal x = 42
yaz("Merhaba " + str(x))' > /tmp/vsix_t2.tr.mlp
/home/pardus/PROJELER/MELP/ORTAK/normalizer/melp_normalizer.sh \
    /tmp/vsix_t2.tr.mlp /tmp/vsix_t2_pmlp.mlp && \
$S0C /tmp/vsix_t2_pmlp.mlp -o /tmp/vsix_t2.ll 2>&1 && \
clang-19 /tmp/vsix_t2.ll -lm -o /tmp/vsix_t2_bin && \
/tmp/vsix_t2_bin
# Beklenen: Merhaba 42

# Test 3 — bin/melp_normalizer_bin güncel mi?
md5sum /home/pardus/PROJELER/MELP/ORTAK/normalizer/melp_normalizer_bin \
       /home/pardus/PROJELER/MELP/EDITORS/VSIX/bin/melp_normalizer_bin
# Beklenen: iki hash aynı
```

---

## TESLİM

PD'ye şunları raporla:

1. Görev 1-2: mlpCompiler.ts ve debugAdapter.ts diff (hangi satırlar değişti)
2. Görev 3-4: sync-binary.sh çıktısı (3 binary güncel mi?)
3. Görev 5: .stage0-dep güncel versiyon
4. Görev 6: npm run build hata çıktısı (yoksa "✅ temiz")
5. Görev 7: 3 testin çıktısı

**PD doğrulama yapacak, eskalasyon dosyaları PD onayından sonra güncellenecek.**

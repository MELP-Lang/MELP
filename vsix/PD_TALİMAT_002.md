# PD TALİMAT — EDITORS/VSIX/002
**Tarih:** 2026-03-20
**Yazan:** PD (Proje Direktörü)
**Öncelik:** KRİTİK

---

## Durum Özeti (PD doğrulaması)

| Bileşen | Durum | Not |
|---|---|---|
| STAGE0 binary | ✅ v1.0.11 mevcut | 717/803 test geçiyor |
| VSIX bin/ sync | ✅ 3 binary güncel | normalizer hash eşleşiyor |
| `.stage0-dep` | ✅ 1.0.11 | güncellendi |
| `dist/` (TypeScript) | ✅ Mar 18 rebuild | güncel |
| VSIX paketi | ⚠️ `0.7.5` — Mar 17 | dist/ Mar 18 yenilendi ama yeni .vsix paketlenmedi |
| `kitchen_sink.mlp` | ⚠️ 412. satırda hata | `scope/exit_compute` Stage 1 özelliği — beklenebilir |
| 86 test başarısızlığı | ✅ Kabul edilebilir | ağırlıkla Stage 1 özellikleri |

**Karar:** İngilizce MLP komut satırında çalışıyor (%89.3). VS Code tarafında yeni VSIX paketi üretilmeli. `kitchen_sink.mlp` Stage 1 vitrin dosyası olarak değerlendirilmeli.

---

## TALİMAT A — EDITORS/VSIX MM
**Konu:** Yeni VSIX paketi + İngilizce uçtan uca test

### Görev 1 — VSIX yeniden paketleme
```bash
cd /home/pardus/PROJELER/MELP/EDITORS/VSIX
npm run package
```

**Beklenen:** `mlp-ide-0.7.6.vsix` (veya mevcut versiyonun bir üstü)

**PD doğrulama kriteri:**
- Yeni `.vsix` dosyası oluşmalı
- Dosya tarihi 2026-03-20 olmalı

### Görev 2 — İngilizce uçtan uca komut satırı testleri
```bash
S0C=/home/pardus/PROJELER/MELP/LLVM/STAGE0/build/compiler/stage1/modules/orchestrator/melp_compiler

# Test A — Değişken + print
echo 'numeric x = 42
print("Sonuc: " + str(x))' > /tmp/en_t1.mlp
$S0C /tmp/en_t1.mlp -o /tmp/en_t1.ll && clang-19 /tmp/en_t1.ll -lm -o /tmp/en_t1_bin && /tmp/en_t1_bin
# Beklenen: Sonuc: 42

# Test B — Fonksiyon + döngü
echo 'function toplam(numeric a; numeric b) returns numeric
    return a + b
end_function
print(str(toplam(10; 32)))' > /tmp/en_t2.mlp
$S0C /tmp/en_t2.mlp -o /tmp/en_t2.ll && clang-19 /tmp/en_t2.ll -lm -o /tmp/en_t2_bin && /tmp/en_t2_bin
# Beklenen: 42

# Test C — if/else + while
echo 'numeric i = 0
while i < 3
    if i == 1 then
        print("bir")
    else
        print(str(i))
    end_if
    i = i + 1
end_while' > /tmp/en_t3.mlp
$S0C /tmp/en_t3.mlp -o /tmp/en_t3.ll && clang-19 /tmp/en_t3.ll -lm -o /tmp/en_t3_bin && /tmp/en_t3_bin
# Beklenen: 0 / bir / 2 (satır satır)
```

**Teslim:**
- 3 testin gerçek çıktısı
- yeni `.vsix` dosya adı ve boyutu

---

## TALİMAT B — LLVM/STAGE1 MM (İletim)
**Konu:** Stage 0 kapsamını belgele + `kitchen_sink` ayrıştırması

### Görev 1 — kitchen_sink dosyalarını ayır
- `kitchen_sink_stage0.mlp`: yalnızca Stage 0 destekli özellikler
- `kitchen_sink.mlp`: Stage 1 vitrin olarak mevcut hali korunacak

**Doğrulama:**
```bash
$S0C kitchen_sink_stage0.mlp -o /tmp/ks0.ll && clang-19 /tmp/ks0.ll -lm -o /tmp/ks0_bin && /tmp/ks0_bin
```

### Görev 2 — 86 başarısız testi sınıflandır
`BEKLEYEN_SORUNLAR.md` içindeki 86 başarısız test şu iki başlıkta etiketlenecek:
- Stage 1 ertelenmiş
- Stage 0 regression

**Kural:** Herhangi bir Stage 0 regression varsa PD’ye anında eskalasyon.

**Teslim:**
- `kitchen_sink_stage0.mlp` derlenip çalışır kanıtı
- 86 test sınıflandırma özeti: kaç adet Stage 1 ertelenmiş, kaç adet Stage 0 regression

---

## Yürütme Notu
Talimatlar bağımsızdır; paralel yürütülebilir.

---

## EK ONAY — PD Yanıtı (VSIX MM Teklifi)
**Tarih:** 2026-03-20

PD_TALİMAT_002 onaylandı. İçerik PD beklentisiyle uyumlu.

**Karar:** VSIX MM'in teklifi kabul edilmiştir. Talimat A (VSIX paketi + 3 test) MM tarafından doğrudan uygulanabilir.

### PD doğrulama kriterleri (teslimde zorunlu)
1. `npm run package` çıktısının son 10 satırı (hata var/yok net görünmeli)
2. Yeni `.vsix` dosyasının adı, boyutu ve tarihi:
    - `ls -lh *.vsix | tail -1`
3. Test A, Test B, Test C gerçek terminal çıktıları:
    - Beklenen değerlerle karşılaştırmalı rapor

### Kapsam sınırı
Talimat B (STAGE1 MM) VSIX MM kapsamı dışındadır. Ayrı iletim beklenmektedir.

### Teslim formatı
Doğrulama çıktıları bu kanala veya tek bir teslim raporu olarak iletilebilir. PD nihai kontrolü bizzat yapacaktır.

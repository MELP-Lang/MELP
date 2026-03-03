# COMPTIME - Özellik Kartı

**Özellik:** Derleme zamanı hesaplama — `comptime`  
**Durum:** 🔮 NOT PLANNED (AOT mimarisiyle uyum değerlendirmesi gerekiyor)  
**Hafta:** Belirsiz  
**Sözdizimi Sürümü:** Belirlenmedi

---

## 🎯 ÖZELLİK

Derleme zamanında çalışan fonksiyon ve ifadeler. Zig'in `comptime` modeline benzer. MELP'in **AOT (Ahead-of-Time) mimarisiyle uyumu henüz değerlendirilmedi** — bu nedenle planlanmış bir özellik DEĞİLDİR.

---

## 📖 SÖZDİZİMİ

```pmlp
-- ⚠️ COMPTIME PLANLI DEĞİL — taslak not

-- Zig'in comptime modeli (referans):
-- fn factorial(comptime n: usize) usize { ... }
-- const result = factorial(10);  -- derleme zamanında hesaplanır

-- MELP eşdeğeri hipotezi (kesin sözdizimi YOK):
-- comptime function factorial(numeric n) as numeric
--     if n <= 1 then return 1 end_if
--     return n * factorial(n - 1)
-- end_function
--
-- comptime numeric FACT_10 = factorial(10)
```

**Neden Planlanmamış:**
```
1. MELP AOT derleyici → Runtime yok → comptime/runtime ayrımı farklı
2. LLVM IR constant folding zaten bazı derleme zamanı optimizasyonları yapıyor  
3. Generic + where clause yeterli mi? → Önce bu soruyu cevapla
4. Macro sistemi (MACRO.md) ile overlap var → Önce macro tasarlanmalı
```

**Şu Anki Alternatifler:**
```pmlp
-- 1. LLVM constant folding (örtük — kod yazma):
numeric MAX = 1000    -- derleyici sabit olarak optimize eder

-- 2. Generics ile tip bazlı seçim:
function process{T: Numeric}(T val) as T
    -- tip bazlı özelleşme

-- 3. Makro (Stage 2+ planında)
```

---

## 🧪 GOLDEN TEST

```pmlp
-- Henüz test yazilamaz — özellik planlanmamış
```

---

**Bağımlılıklar:** AOT mimari kararı, Macro sistemi, Generic sistemi  
**Hafta:** Belirsiz — önce AOT/comptime uyum analizi gerekiyor  
**Referans Diller:** Zig `comptime`, D `static if`, C++ `constexpr`, Nim `when`  
**Kritik Not:** Bu kart "neden yok" sorusunun cevabını belgeliyor — özellik geldiğinde tamamen yeniden yazılacak

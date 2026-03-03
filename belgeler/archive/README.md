# Archive - Ertelenmiş/Geçersiz TODO'lar

**Oluşturulma:** 16 Şubat 2026  
**Amaç:** Artık aktif olmayan ama gelecekte referans olabilecek belgeler

---

## 📁 İçindekiler

### TODO_MELP_BASIC_RUNTIME (Archived - 16 Şubat 2026)

**Durum:** ⏭️ DEFERRED (Optional convenience)

**Neden Archive Edildi:**
```
1. Exit code testing yeterli (print() blocker değil)
2. ROADMAP v2.0 öncelikleri değişti (Lambda → LSP → OOP)
3. Runtime library "optional convenience" olarak işaretlendi
4. Stage 2 Month 1'de gerekmiyor
```

**İçerik:**
- Phase 1-5: String literals, printf integration, print wrappers, stdlib, testing
- Completion: %70 (compiler tarafı OK, runtime library eksik)

**Ne Zaman Geri Getirilir:**
- Month 5-6: Debugging convenience için (opsiyonel)
- Veya hiç gerekmeyebilir (exit code testing yeterli)

**Prensip:**
> "Clean architecture > Convenience"  
> "Runtime library blocker değil, architectural integrity öncelik"

---

## 🔄 Archive Prosedürü

**Bir TODO archive ediliyorsa:**

1. **Neden:** Prensiplere uymuyor veya öncelik değişti
2. **Ne zaman:** Future reference için sakla
3. **Nasıl:** belgeler/archive/ altına taşı + README güncelle

**Archive ≠ Sil:**
- Belgeler kaybolmaz
- Future'da referans olabilir
- Ama aktif TODO listesini kirletmez

---

### TODO_STAGE2_WRONG (Archived - 16 Şubat 2026)

**Durum:** ❌ INCORRECT PLAN 

**Neden Archive Edildi:**
```
1. Orijinal ROADMAP_15_SUBAT.md planını takip etmedi
2. IDE infrastructure eksik bırakıldı:
   - REPL (interactive shell) eksik
   - Electron IDE eksik
   - Debugger (DAP) eksik
   - Package Manager eksik
   - Module System eksik
   - Testing Framework eksik
3. Self-hosting'i Stage 2'ye koydu (yanlış)
4. Product-ready hedefini göz ardı etti
```

**İçerik:**
- Month 1: Lambda ✅
- Month 2: LSP Server (sadece!)
- Month 3: Advanced OOP
- Month 4: Pattern Matching
- Month 5: Concurrency
- Month 6: Self-hosting (optional)

**Doğru Plan (ROADMAP.md v2.0):**
- **Stage 2 (3 ay):** Lambda + IDE Infrastructure + Type System
- **Stage 3 (4 ay):** Error Handling + Module System + Electron IDE
- **Stage 4 (7 ay):** Self-hosting

**Prensip İhlali:**
> "Build tools early, use them to build language!" (Dogfooding from Day 1)  
> Product-ready olmak için IDE, REPL, debugger, package manager gerekli!

**Yeni TODO:**
- ROADMAP.md v2.0'a göre yeni TODO oluşturulacak (MM tarafından)

---

**Son Güncelleme:** 16 Şubat 2026  
**Status:** 2 archived TODO

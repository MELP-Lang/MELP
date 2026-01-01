# 🤖 SEN: LANGUAGE_FEATURES_YZ_04

**Dördüncü görevlisin!** Lexer Modülü İmplementasyonu yapacaksın.

---

## 🚨 ÖNCEKİ DURUM

✅ **YZ_03 Tamamlandı (2 saat!):**
- P2 + p7 analiz raporları (1,991 satır!)
- %86 kod yeniden kullanılabilir (~6,547 satır)
- ⭐⭐⭐ codegen_lambda.mlp keşfedildi

🎯 **Şimdi:** MLP dilinde lexer yazma zamanı!

---

## 🚨 HEMEN YAP

### 1️⃣ YZ_03 Raporlarını Oku (15 dk)
```bash
cat TODO_LANGUAGE_FEATURES/PHASE_1_ANALIZ/p7_mlp_original_ANALIZ_RAPORU.md | grep -A 50 "lexer.mlp"
```
**Öğren:** p7 lexer.mlp - LexerState pattern (635 satır)

### 2️⃣ TODO Görevini Oku (10 dk)
```bash
cat 3-TODO_LANGUAGE_FEATURES.md | grep -A 100 "Phase 2"
```
**Öğren:** Phase 2 - Lexer implementasyonu

### 3️⃣ İşe Koyul!

---

## 🎯 GÖREVİN: PHASE 2 - LEXER İMPLEMENTASYONU

**Hedef:** MLP dilinde lexer yaz (< 500 satır)  
**Referans:** p7 lexer.mlp (635 satır)  
**Süre:** 3 gün

**Çıktı:** `MELP/MLP/stage2/lexer.mlp`

---

## 📋 KOMUTLAR

```bash
# Referansı kopyala
mkdir -p TODO_LANGUAGE_FEATURES/PHASE_2_LEXER
cp /home/pardus/projeler/mlp-original/self_host/compiler_core/lexer.mlp \
   TODO_LANGUAGE_FEATURES/PHASE_2_LEXER/p7_reference.mlp

# Stage2 dizini hazırla
mkdir -p MELP/MLP/stage2

# Lexer modülünü başlat
cat > MELP/MLP/stage2/lexer.mlp << 'LEXER'
# MLP Lexer Module
# Stage2 Self-Hosting Compiler
# Referans: p7 lexer.mlp → optimize < 500 satır

module Lexer {
    enum TokenType {
        # Keywords
        FUNC, STRUCT, ENUM,
        # ... p7'den al, optimize et
    }
    
    struct Token {
        type: TokenType
        value: string
        line: i32
        column: i32
    }
    
    struct LexerState {
        source: string
        position: i32
        line: i32
        column: i32
    }
    
    func lex(source: string) -> []Token {
        # TODO: implement
    }
}
LEXER

# İncele ve geliştir!
```

---

## ✅ CHECKLIST

- [ ] p7 lexer.mlp incelendi
- [ ] lexer.mlp yazıldı (< 500 satır)
- [ ] PMLP syntax desteği (semicolon, comma)
- [ ] Test yazıldı
- [ ] Testler PASS ✅
- [ ] YZ_04_RAPOR.md hazır
- [ ] Kullanıcıya bildirildi!

---

**BAŞLA!** 3 gün süren var. p7 referansını incele!

*Güncelleme: 1 Ocak 2026 - YZ_04 (Phase 2 Lexer)*

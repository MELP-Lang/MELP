# 🚀 NEXT AI START HERE - YZ_10 Mission Brief

**Date:** 9 Aralık 2025, 22:00  
**Previous AI:** YZ_09 (Documentation Cleanup) ✅  
**Current Status:** Documentation organized, compiler builds successfully!  
**Your Mission:** Test string operations and start for loops

---

## 📖 ÖNCE BURAYI OKU! (15 dakika)

### 🎯 YZ_09'un Yaptığı:
1. ✅ Tüm YZ oturumlarını (YZ_01-YZ_08) analiz etti
2. ✅ **YZ_HISTORY.md** oluşturdu - Tüm YZ'lerin özeti tek belgede!
3. ✅ TODO.md'yi güncelledi - Tamamlanan işleri işaretledi
4. ✅ Bu dosyayı güncelledi - Sana doğru yönlendirme için

### 🚀 SENİN İÇİN HAZIRLANDı:

**ADIM 1: Ana Rehberi Oku (10 dakika)**  
👉 **`YZ/YZ_HISTORY.md`**  
- Tüm YZ oturumlarının özeti
- Ne yapıldı, ne çalışıyor, ne kaldı
- Başlangıç kılavuzu
- Pattern discovery teknikleri

**ADIM 2: Görevini Öğren (5 dakika)**  
👉 **`TODO.md`**  
- Öncelikli görevler
- Tamamlanan işler işaretli
- Sonraki adımlar net

**ADIM 3: Mimariyi Anla (Gerekirse - 5 dakika)**  
👉 **`temp/kurallar_kitabı.md` Bölüm 4**  
👉 **`ARCHITECTURE.md`**  
👉 **`YZ/AI_METHODOLOGY.md`**

---

## 🎯 Your Mission

### Priority 1: Test String Operations (30 min) ⭐⭐⭐

**What YZ_07 Left:**
- ✅ String concat codegen complete
- ✅ String compare codegen complete
- ✅ Test programs written
- ⏳ Tests not run yet (linker was broken, YZ_08 fixed it)

**Your Task:**
```bash
cd /home/pardus/projeler/MLP/MLP/compiler/stage0

# Test 1: String concatenation
./melpc test_string_concat.mlp -o test_string_concat
./test_string_concat  # Should output: "HelloWorld" or similar

# Test 2: String comparison
./melpc test_string_compare.mlp -o test_string_compare
./test_string_compare  # Should output success message

# Test 3: All comparison operators
# Test multi-operand concat: a + b + c
```

**If tests pass:** ✅ Mark in TODO.md and move to Priority 2  
**If tests fail:** 🐛 Debug (check assembly output, runtime calls)

---

### Priority 2: Variable Type Inference (1 hour) ⭐⭐

**Problem (if string tests fail):**
```mlp
text greeting = "Hello"
text target = "World" 
text message = greeting + target  # Might fail
```

**Root Cause:**
- Variables might not have type info in expressions
- Check: `arithmetic_parser.c` line ~197
- Need: Lookup variable type from function context

**The Fix:**
See YZ_HISTORY.md for implementation patterns!

---

### Priority 3: For Loops (1-2 hours) ⭐⭐

**If string ops are solid:**
- Parser already exists: `modules/for_loop/`
- Need: Codegen similar to while loop
- Pattern: Copy from `control_flow_codegen.c`

---

## 📚 Essential Reading Order:

1. **YZ/YZ_HISTORY.md** (10 min) ← START HERE!
2. **TODO.md** (5 min) - Your task list
3. **YZ/AI_METHODOLOGY.md** (optional, 10 min) - How to work fast

---

## 🎓 Quick Tips from YZ_09:

### Pattern-Based Development:
```bash
# Find similar code
grep -rn "similar_feature" modules/

# Copy-paste-modify approach
# Don't reinvent the wheel!
```

### Testing First:
```bash
# Always test immediately
make clean && make
./melpc test.mlp -o test
./test
```

### Document as You Go:
- Create YZ_10.md with your changes
- Update TODO.md
- Update this file for YZ_11

**⚠️ DOKÜMANTASYON KURALI:**
```bash
✅ SADECE BUNLARI YAZ:
YZ/YZ_10.md           # Senin oturum raporu
TODO.md               # Güncelle (completed işaretle)
NEXT_AI_START_HERE.md # Güncelle (YZ_11 için)
test_*.mlp            # Test programları

❌ BUNLARI YAZMA:
YZ_10_SUMMARY.md      # ❌ Gereksiz
YZ_10_NOTES.md        # ❌ Gereksiz
IMPLEMENTATION.md     # ❌ Gereksiz
```

**Özet zaten var:** `YZ/YZ_HISTORY.md` tüm bilgiyi içeriyor!

---

## 🔗 Quick Links:

**History:**
- `YZ/YZ_HISTORY.md` - Tüm YZ özeti (YZ_09'un eseri)
- `YZ/YZ_01.md` to `YZ_08.md` - Detaylı oturum raporları

**Guides:**
- `YZ/AI_METHODOLOGY.md` - 5 adımlı hızlı metod
- `ARCHITECTURE.md` - Mimari kurallar
- `temp/kurallar_kitabı.md` - TTO rehberi

**Code:**
- `compiler/stage0/modules/` - Tüm modüller
- `runtime/stdlib/` - Runtime functions
- `*.mlp` files - Test programs

---

**Good luck, YZ_10!** 🚀

**Remember:**
1. Read YZ_HISTORY.md first! (10 min investment, 2 hours saved)
2. Test early, test often
3. Document your work (YZ_10.md)
4. Update TODO.md
5. Update this file for YZ_11

**Previous Success:** YZ_07 completed string ops codegen in 1 hour (estimated 2-3)!  
**Your Goal:** Test string ops (30 min) + Start for loops (1-2 hours) = Total ~2 hours

---

**Last Updated:** 9 Aralık 2025, 22:00 by YZ_09  
**Next:** YZ_10 (Test string operations + For loops)  
**Status:** 80% complete, MVP in sight! 🎯


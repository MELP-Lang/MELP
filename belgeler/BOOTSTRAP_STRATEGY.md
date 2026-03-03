# BOOTSTRAP STRATEGY - MELP Hedefi

**Tarih:** 24 Ocak 2026  
**Durum:** ✅ APPROVED (Üst Akıl)  
**Amaç:** Self-hosting vs Bootstrap farkını netleştirmek

---

## 🚨 KRITIK: BU BIR SOURCE PROJECT!

**En Önemli İçgörü:**
```
Bu proje KENDİNİ DERLEMEYECEK!
├─ TC, BASE, IDEAL, MODERN → Source code repositories
├─ Başka projeler bunları KULLANACAK
├─ Her modül tamamen BAĞIMSIZ olmalı
└─ Code duplication = SORUN DEĞİL, AVANTAJ!
```

**Neden Code Duplication OK?**
- ✅ Her modül standalone (copy-paste edilebilir)
- ✅ Başka projeler istediği seviyeyi alır
- ✅ Zero dependency (her modül kendi lexer/parser/codegen)
- ✅ Binary olarak dağıtılmayacak (kaynak kod dağıtılacak)

**Mimari:**
```
compiler/TC/     → Minimal bootstrap compiler (standalone)
compiler/base/   → Essential features compiler (standalone)
compiler/ideal/  → Advanced features compiler (standalone)
compiler/modern/ → Cutting-edge features compiler (standalone)

→ Hiçbiri birbirine bağımlı değil!
→ Her biri kendi lexer, parser, codegen içerir!
```

**Strateji: COPY & EXTEND**
```
1. TC tamamlandı
   ↓
2. BASE: TC'den KOPYALA + extend (string, I/O) → Standalone ✅
   ↓
3. IDEAL: BASE'den KOPYALA + extend (arrays, structs, enums, else_if) → Standalone
   ↓
4. MODERN: IDEAL'den KOPYALA + extend (lambdas, generics) → Standalone
```

**Pratik Kullanım:**
```bash
# Bir proje IDEAL seviyesi isterse:
cp -r compiler/ideal/ my_project/compiler/

# Hemen kullanabilir! (TC veya BASE'e bağımlı değil)
cd my_project/compiler/ideal/
mkdir build && cd build
cmake ..
make
```

---

## 🎯 HEDEF: Bootstrap (Library Development), DEĞIL Self-hosting!

### ❌ Self-hosting (HEDEF DEĞİL!)

```
Stage0 (C++) → Stage1 compiler (PMPL ile yazılmış)
            → Stage2 compiler (Stage1 ile derlenmiş)
            → Self-hosting proof
```

**Neden istemiyoruz?**
- ⚠️ Self-hosting = Nice-to-have (must-have değil!)
- ⚠️ Akademik hedef (pratik değil)
- ⚠️ Riskli ve karmaşık
- ⚠️ Timeline uzuyor (18+ hafta)
- ⚠️ Compiler'ı MELP ile yazmak zorunda değiliz

### ✅ Bootstrap (BİZİM HEDEF!)

```
Stage0 (C++) [PERMANENT BACKEND]
     ↓
stdlib/*.mlp (MELP ile yazılmış modüller)
     ↓
Production-ready applications (MELP ile)
```

**Neden istiyoruz?**
- ✅ Pratik hedef
- ✅ C++ compiler güvenilir ve hızlı kalır
- ✅ MELP ile modüller yazabiliriz (ASIL AMAÇ!)
- ✅ Timeline kısa (6 hafta compiler, sonra library development)
- ✅ Industry standard (Go, Rust, Python hep böyle başladı)

---

## 📊 Kritik Fark

| Özellik | Self-hosting | Bootstrap (Bizim) |
|---------|--------------|-------------------|
| **Compiler backend** | MELP ile yaz | C++ (KALICI) |
| **Amaç** | Compiler kendini derler | Compiler kütüphaneleri derler |
| **Risk** | Yüksek | Düşük |
| **Timeline** | 18+ hafta | 6 hafta + stdlib |
| **Pratik değer** | Akademik | Production-ready |
| **Öncelik** | Nice-to-have | Must-have |

---

## 🎯 Ne İstiyoruz?

**ASIL HEDEF:** MELP diliyle istediğimiz her modülü yazabilmek!

### Örnek: Graph Modülü

```mlp
-- stdlib/graph.mlp (MELP ile yazılmış!)

struct Graph
    numeric node_count
    list edges
end_struct

function create_graph(numeric nodes) as Graph
    return Graph{nodes; []}
end_function

function add_edge(Graph g; numeric from; numeric to) as none
    g.edges = g.edges + [(from; to)]
end_function

function shortest_path(Graph g; numeric start; numeric end) as numeric[]
    -- Dijkstra implementation (MELP ile!)
    -- ...
end_function
```

```bash
# Stage0 compiler (C++) bunu derler
./melp_compiler stdlib/graph.mlp -o libgraph.so

# Artık graph modülünü kullanabiliriz
```

**Hiç olmayan bir modülü sıfırdan MELP ile tasarlayabiliriz!**

---

## 🚀 Strateji

### Phase 0: TC Bootstrap ✅ TAMAMLANDI
- Minimal Turing-complete compiler
- 2-3 gün

### Phase 1: BASE Features ✅ TAMAMLANDI
- String operations, I/O, operators, type system
- 3-4 gün, 40,572 tests pass

### Phase 2: IDEAL Features ⏳ ŞUANKİ HEDEF
- else_if fix, arrays, structs, enums, string concat
- 4-5 gün
- **Bu bitince MELP ile basit modüller yazabiliriz!**

### Phase 3: MODERN Features
- Lambdas, generics, pattern matching, async/await
- 5-7 gün
- **Bu bitince MELP ile modern modüller yazabiliriz!**

### Phase 4: STDLIB Bootstrap (YENİ!)
- **Artık MELP ile modül yazabiliriz!**
- stdlib/collections.mlp
- stdlib/graph.mlp
- stdlib/network.mlp
- stdlib/database.mlp
- stdlib/crypto.mlp
- stdlib/...

---

## 📋 Pratik Faydalar

**Stage0 (C++ LLVM) tamamlandığında yapabileceklerimiz:**

1. ✅ Web servers (MELP ile)
2. ✅ Database drivers (MELP ile)
3. ✅ Graph algorithms (MELP ile)
4. ✅ Network protocols (MELP ile)
5. ✅ Crypto libraries (MELP ile)
6. ✅ Game engines (MELP ile)
7. ✅ ML frameworks (MELP ile)
8. ✅ **Her türlü modül/uygulama** (MELP ile!)

**Tüm bunlar için compiler C++ olabilir** - önemli değil!  
**Önemli olan MELP ile yazmak!**

---

## 🎓 Self-hosting Nerede?

**Nice-to-have özellik** → İleride topluluk yapabilir

**Neden acele etmiyoruz?**
- Önce dil rüştünü ispatlamalı
- Production-ready applications yazabilmeliyiz
- Self-hosting akademik bir hedef (pratik değer sınırlı)
- Community contribution olabilir (Go, Rust böyle yaptı)

**Timeline:**
- Phase 0-3: 12-15 gün (Stage0 complete)
- Phase 4: Ongoing (stdlib development)
- Self-hosting: Future (optional, community-driven)

---

## ✅ Onay

**Üst Akıl:** ✅ APPROVED (24 Ocak 2026)

**Karar:** 
- Self-hosting nice-to-have (must-have değil)
- Bootstrap must-have (MELP ile modül yazma)
- Stage0 (C++) kalıcı backend olacak
- Self-hosting ileride topluluk yapabilir

**Sonraki adım:** IDEAL modülü için UA organizasyonu başlatılacak

---

## 🔗 İlgili Belgeler

- [STAGE0_LIMITATIONS.md](STAGE0_LIMITATIONS.md) - Stage0 kısıtlamaları
- [MELP_PHILOSOPHY.md](MELP_PHILOSOPHY.md) - MELP felsefesi
- [pmlp0_sozdizimi.md](pmlp0_sozdizimi.md) - PMPL0 syntax
- [pmlp1_sozdizimi.md](pmlp1_sozdizimi.md) - PMPL1 syntax (Stage1 için)
- [3-TODO_IDEAL_FEATURES.md](../3-TODO_IDEAL_FEATURES.md) - IDEAL TODO

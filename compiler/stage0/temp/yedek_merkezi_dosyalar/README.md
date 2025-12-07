# Yedek Merkezi Dosyalar

**Tarih:** 7 Aralık 2025  
**Sebep:** Radikal modüler mimari geçişi

## Neden Yedeklendi?

Bu dosyalar MELP'in merkezi mimari döneminden kalma. AI agentları sürekli bu merkezi dosyalara kod eklemeye çalışıyordu ve modüler yapıyı bozuyordu.

**Çözüm:** Tüm merkezi dosyaları kaldırıp AI'ları **modül içinde çözüm aramaya zorlamak**.

## Yedeklenen Dosyalar

### Ana Dosyalar
- `main.c` (801 bytes) - Pipeline çağıran entry point
- `main_full.c` (6178 bytes) - Eski monolitik main
- `main_simple.c` (4008 bytes) - Basitleştirilmiş versiyon
- `main.c.backup` (30604 bytes) - Orijinal 736-satırlık monolitik kod

### Orchestrator (Kaldırıldı)
- `orchestrator.c` (6102 bytes) - Token routing
- `orchestrator.h` (619 bytes)

**Sebep:** Orchestrator modüler yapıya aykırı. Her modül standalone olmalı.

### Helpers (Kaldırıldı)
- `helpers.c` (2494 bytes) - Assembly helper fonksiyonları
- `helpers.h` (152 bytes)

**Sebep:** Helper fonksiyonları modüllere dağıtılmalı. Merkezi helper AI'ları cezbediyor.

### Lexer (Modüle Taşınacak)
- `lexer.c` (10876 bytes) - Lexer logic
- `lexer.h` (1904 bytes)

**Yeni Konum:** `modules/lexer/` olarak standalone modül

### Parser/Codegen Headers
- `parser.h` (442 bytes) - Parser interface
- `codegen.h` (431 bytes) - Codegen interface

**Sebep:** Gereksiz abstraction. Modüller kendi header'larını yönetsin.

### Dizinler

#### cli/ (Modüle Taşınacak)
- `cli_parser.c` - CLI argüman parsing
- `cli_parser.h`
- `cli_parser.o`

**Yeni Konum:** `modules/cli/` veya `melp` wrapper script ile

#### pipeline/ (SİLİNECEK!)
- `pipeline.c` - Pipeline orchestration
- `pipeline.h`

**Sebep:** Pipeline orchestration AI'ları merkezi düşünmeye itiyor. Unix pipe kullanılmalı.

#### error/ (Modüle Taşınacak)
- `error_handler.c` - Error handling
- `error_handler.h`
- `error_handler.o`

**Yeni Konum:** `modules/error/` veya her modül kendi error handling'ini yapsın

## Yeni Mimari

```
compiler/stage0/
├── Makefile                 (sadece build)
└── modules/
    ├── lexer/
    │   ├── lexer.c
    │   ├── lexer.h
    │   └── main.c          # standalone: ./lexer < input.mlp > tokens.json
    ├── parser/
    │   ├── parser.c
    │   └── main.c          # standalone: ./parser < tokens.json > ast.json
    ├── codegen/
    │   ├── codegen.c
    │   └── main.c          # standalone: ./codegen < ast.json > output.s
    └── print/
        ├── print_parser.c
        ├── print_codegen.c
        └── main.c          # standalone test
```

## Unix Philosophy Pipeline

```bash
# Eski (monolitik):
./melpc program.mlp -o output.s

# Yeni (pipeline):
./modules/lexer/lexer < program.mlp | \
./modules/parser/parser | \
./modules/codegen/codegen > output.s
```

## Geri Dönüş Planı

Eğer bu radikal değişiklik başarısız olursa:

```bash
cd /home/pardus/projeler/MLP/MLP/compiler/stage0
cp -r temp/yedek_merkezi_dosyalar/* .
git checkout HEAD -- Makefile
make clean && make
```

## AI Agent Notları

**Gelecek AI Agentlar için:**

❌ **YAPMA:**
- Bu dosyaları geri getirme
- Yeni merkezi dosya oluşturma
- "Orchestrator gerekli" deme

✅ **YAP:**
- Her sorunu modül içinde çöz
- Standalone binary yaz
- JSON/stdin/stdout kullan
- Unix pipe philosophy

**Unutma:** Merkezi dosya yok = Merkezi düşünemezsin = Modüler düşünmeye zorlanırsın! 🎯

---

**Yedekleme Tarihi:** 7 Aralık 2025, 17:52  
**Son Commit:** feat: Add file creation lock enforcement (c7f4673)

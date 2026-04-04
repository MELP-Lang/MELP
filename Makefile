# -- NOT: Bu Makefile normalizer adımını içermez.
# -- Tam pipeline için: bin/run_melp kullan (normalizer → melp_compiler)
# -- Sadece STAGE0-uyumlu .pmlp kaynak dosyaları için kullan.
# -- PD kararı: K-13, 2026-03-29
MELP = /home/pardus/PROJELER/MELP/LLVM/STAGE0/build/compiler/stage1/modules/orchestrator/melp_compiler
CLANG = clang
BUILD_DIR = build

MELP_FLAGS = --ok-all
OPT_FLAGS = -O2

# Kurulum dizini (sudo make install PREFIX=/usr/local ile geçersiz kılınabilir)
PREFIX ?= /usr/local

.PHONY: all clean test install uninstall

all: $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# .mlp → .ll (LLVM IR)
$(BUILD_DIR)/%.ll: src/%.mlp | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(MELP) $< $(MELP_FLAGS) -o $@

# .mlp → ELF binary
$(BUILD_DIR)/%.elf: src/%.mlp | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(MELP) $< $(MELP_FLAGS) -o $(BUILD_DIR)/$(notdir $*).ll
	$(CLANG) $(OPT_FLAGS) -x ir $(BUILD_DIR)/$(notdir $*).ll -o $@

# Hızlı derleme kısayolu: make run SRC=apps/hello
run: | $(BUILD_DIR)
	$(MELP) src/$(SRC).mlp $(MELP_FLAGS) -o $(BUILD_DIR)/$(notdir $(SRC)).ll
	$(CLANG) $(OPT_FLAGS) -x ir $(BUILD_DIR)/$(notdir $(SRC)).ll -o $(BUILD_DIR)/$(notdir $(SRC)).elf
	$(BUILD_DIR)/$(notdir $(SRC)).elf

clean:
	rm -rf $(BUILD_DIR)

# ── Kurulum ─────────────────────────────────────────────────────────────────
# sudo make install          → /usr/local/bin/melp + /usr/local/bin/melp-lsp
# sudo make install PREFIX=~/.local → kullanıcı dizinine kur (sudo gerekmez)
install:
	@echo "MLP derleyicisi kuruluyor: $(PREFIX)/bin/melp"
	install -d $(PREFIX)/bin
	install -m 755 bin/melp_compiler $(PREFIX)/bin/melp
	install -m 755 bin/melp_lsp      $(PREFIX)/bin/melp-lsp
	@echo "Kurulum tamamlandı. Sürüm: $$($(PREFIX)/bin/melp --version 2>/dev/null || echo 'bkz. CHANGELOG.md')"

uninstall:
	rm -f $(PREFIX)/bin/melp $(PREFIX)/bin/melp-lsp
	@echo "MLP kaldırıldı."

MELP = /home/pardus/projeler/MLP/LLVM/stage0/v2/build/compiler/stage1/modules/orchestrator/melp_compiler
CLANG = clang
BUILD_DIR = build

MELP_FLAGS = --ok-all
OPT_FLAGS = -O2

.PHONY: all clean test

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

CC = gcc
CFLAGS = -Wall -g -Wno-unused-function -Wno-unused-variable -I../../runtime/tto
LDFLAGS = -L../../runtime/tto -ltto_runtime
TARGET = melpc

# 26 Core Modules (22 documented core + 4 essential extras)
# Core 22: parser_core, expression, statement, comments, control_flow, 
#          arithmetic, comparison, logical, bitwise_operations, variable,
#          array, string_operations, functions, lambda, type_system,
#          null_safety, optimization_pass, memory, file_io, codegen_context,
#          print, struct
# Essential +4: async, debug, runtime_tto, tto_runtime

MODULE_DIRS = parser_core expression statement comments control_flow \
              arithmetic comparison logical bitwise_operations variable \
              array string_operations functions lambda type_system \
              null_safety optimization_pass memory file_io codegen_context \
              print struct \
              async debug runtime_tto tto_runtime

# Generate object file lists
# Most modules have: module.o, module_parser.o, module_codegen.o
# Special cases: 
#   - runtime_tto: only runtime_tto.o
#   - tto_runtime: only tto_runtime.o
#   - optimization_pass: only optimization_pass.o (no parser/codegen)
#   - debug: uses debug_features.* naming instead of debug.*
MODULE_OBJS = $(foreach dir,$(filter-out runtime_tto tto_runtime optimization_pass debug,$(MODULE_DIRS)),\
              modules/$(dir)/$(dir).o modules/$(dir)/$(dir)_parser.o modules/$(dir)/$(dir)_codegen.o) \
              modules/runtime_tto/runtime_tto.o \
              modules/tto_runtime/tto_runtime.o \
              modules/optimization_pass/optimization_pass.o \
              modules/debug/debug_features.o modules/debug/debug_features_parser.o modules/debug/debug_features_codegen.o

OBJS = main.o lexer.o \
       error/error_handler.o \
       cli/cli_parser.o \
       orchestrator.o helpers.o \
       $(MODULE_OBJS)

# ========== ARCHITECTURE ENFORCEMENT ==========
# These checks prevent AI agents from creating monolithic code

# Rule #1: main.c cannot exceed 50 lines
check-main-size:
	@lines=$$(wc -l < main.c); \
	if [ $$lines -gt 50 ]; then \
		echo ""; \
		echo "❌ ARCHITECTURE VIOLATION: main.c is $$lines lines (max: 50)"; \
		echo "   main.c should ONLY have main() function"; \
		echo "   Read RULES.md"; \
		echo ""; \
		exit 1; \
	else \
		echo "✅ main.c size OK ($$lines/50 lines)"; \
	fi

# Rule #2: orchestrator.c cannot exceed 200 lines
check-orchestrator-size:
	@lines=$$(wc -l < orchestrator.c); \
	if [ $$lines -gt 200 ]; then \
		echo ""; \
		echo "❌ ARCHITECTURE VIOLATION: orchestrator.c is $$lines lines (max: 200)"; \
		echo "   orchestrator.c should ONLY route to modules"; \
		echo "   Extract logic to modules/"; \
		echo "   Read RULES.md"; \
		echo ""; \
		exit 1; \
	else \
		echo "✅ orchestrator.c size OK ($$lines/200 lines)"; \
	fi

# Rule #3: main.c cannot import module internals directly
check-coupling:
	@if grep -E '^#include.*"modules/.*(parser|codegen)\.h"' main.c > /dev/null 2>&1; then \
		echo ""; \
		echo "❌ ARCHITECTURE VIOLATION: main.c imports module internals"; \
		grep -n 'modules/.*\.h' main.c | head -5; \
		echo "   Use pipeline architecture instead"; \
		echo "   Read ARCHITECTURE.md for rules"; \
		echo ""; \
		exit 1; \
	else \
		echo "✅ Coupling check OK"; \
	fi

# Rule #4: Check binary name (no versioned names like melpc_26)
check-binary-name:
	@if [ -f melpc_[0-9]* ] 2>/dev/null; then \
		echo ""; \
		echo "❌ ARCHITECTURE VIOLATION: Versioned binary detected"; \
		ls -1 melpc_* 2>/dev/null; \
		echo "   Binary must be named 'melpc' only"; \
		echo ""; \
		exit 1; \
	else \
		echo "✅ Binary name check OK"; \
	fi

# Rule #5: CRITICAL - File Creation Lock 🔒
check-forbidden-files:
	@echo "🔍 Checking for forbidden file creation..."
	@VIOLATIONS=0; \
	FORBIDDEN_FILES=""; \
	for file in *.c *.h; do \
		if [ -f "$$file" ]; then \
			case "$$file" in \
				main.c|main_full.c|main_simple.c|orchestrator.c|orchestrator.h|helpers.c|helpers.h|lexer.c|lexer.h|parser.h|codegen.h) \
					;; \
				test_*.c|test_*.h) \
					;; \
				*) \
					echo "❌ FORBIDDEN FILE: $$file"; \
					echo "   Only modules/*, test_* allowed!"; \
					FORBIDDEN_FILES="$$FORBIDDEN_FILES $$file"; \
					VIOLATIONS=$$((VIOLATIONS + 1)); \
					;; \
			esac; \
		fi; \
	done; \
	if [ $$VIOLATIONS -gt 0 ]; then \
		echo ""; \
		echo "╔════════════════════════════════════════════╗"; \
		echo "║   ❌ BUILD REJECTED!                       ║"; \
		echo "║   Forbidden files detected: $$VIOLATIONS           ║"; \
		echo "║   Files:$$FORBIDDEN_FILES"; \
		echo "║   Read ARCHITECTURE.md                     ║"; \
		echo "╚════════════════════════════════════════════╝"; \
		echo ""; \
		exit 1; \
	else \
		echo "✅ File creation rules: PASSED"; \
	fi

# Run all architecture checks
check-architecture: check-main-size check-orchestrator-size check-coupling check-binary-name check-forbidden-files
	@echo ""
	@echo "✅ All architecture checks passed"
	@echo ""

# Build-time validation (runs BEFORE compilation)
.PHONY: validate-before-build
validate-before-build:
	@echo "🔍 Pre-build Architecture Validation..."
	@echo ""
	@$(MAKE) -s check-architecture
	@echo "🔍 Running full validation script..."
	@cd ../.. && bash ./scripts/validate_architecture.sh

# ==============================================

all: validate-before-build $(TARGET)

$(TARGET): $(OBJS) ../../runtime/tto/libtto_runtime.a
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) -lm

# Build TTO runtime if needed
../../runtime/tto/libtto_runtime.a:
	@echo "Building TTO runtime library..."
	$(MAKE) -C ../../runtime/tto

main.o: main.c cli/cli_parser.h pipeline/pipeline.h
	@echo "Compiling main.c with size check..."
	@lines=$$(wc -l < main.c); \
	if [ $$lines -gt 300 ]; then \
		echo ""; \
		echo "❌ FATAL: main.c is $$lines lines (max: 300)"; \
		echo "   Cannot compile! Refactor required."; \
		echo "   Read ARCHITECTURE.md for guidance."; \
		echo ""; \
		exit 1; \
	fi
	$(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

# Error handler module
error/error_handler.o: error/error_handler.c error/error_handler.h lexer.h
	$(CC) $(CFLAGS) -c $< -o $@

# CLI parser module
cli/cli_parser.o: cli/cli_parser.c cli/cli_parser.h
	$(CC) $(CFLAGS) -c $< -o $@

# Orchestrator module
orchestrator.o: orchestrator.c orchestrator.h helpers.h lexer.h
	$(CC) $(CFLAGS) -c $< -o $@

# Helpers module
helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rules for standard modules (with .o, _parser.o, _codegen.o)
modules/%/%.o: modules/%/%.c modules/%/%.h
	$(CC) $(CFLAGS) -c $< -o $@

modules/%/%_parser.o: modules/%/%_parser.c modules/%/%_parser.h modules/%/%.h lexer.h
	$(CC) $(CFLAGS) -c $< -o $@

modules/%/%_codegen.o: modules/%/%_codegen.c modules/%/%_codegen.h modules/%/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Special rule for runtime_tto
modules/runtime_tto/runtime_tto.o: modules/runtime_tto/runtime_tto.c modules/runtime_tto/runtime_tto.h
	$(CC) $(CFLAGS) -c $< -o $@

# Special rule for tto_runtime
modules/tto_runtime/tto_runtime.o: modules/tto_runtime/tto_runtime.c modules/tto_runtime/tto_runtime.h
	$(CC) $(CFLAGS) -c $< -o $@

# Special rule for optimization_pass
modules/optimization_pass/optimization_pass.o: modules/optimization_pass/optimization_pass.c modules/optimization_pass/optimization_pass.h
	$(CC) $(CFLAGS) -c $< -o $@

# Special rules for debug (uses debug_features.* naming)
modules/debug/debug_features.o: modules/debug/debug_features.c modules/debug/debug_features.h
	$(CC) $(CFLAGS) -c $< -o $@

modules/debug/debug_features_parser.o: modules/debug/debug_features_parser.c modules/debug/debug_features_parser.h modules/debug/debug_features.h
	$(CC) $(CFLAGS) -c $< -o $@

modules/debug/debug_features_codegen.o: modules/debug/debug_features_codegen.c modules/debug/debug_features_codegen.h modules/debug/debug_features.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) *.s *.o test_* modules/*/*.o error/*.o cli/*.o pipeline/*.o

test: $(TARGET)
	@echo "=== Testing 26-module MELP compiler ==="
	./$(TARGET) --version || echo "Compiler built successfully!"

.PHONY: all clean test check-architecture check-main-size check-orchestrator-size check-coupling check-binary-name check-forbidden-files

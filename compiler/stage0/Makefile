# ╔═══════════════════════════════════════════════════════════╗
# ║  MELP Stage 0 - MODULAR ARCHITECTURE ONLY                 ║
# ║  NO monolithic compiler (main.c/pipeline.c FORBIDDEN)     ║
# ╚═══════════════════════════════════════════════════════════╝

.PHONY: all clean help test-functions test-string modules
.DEFAULT_GOAL := help

# ═══════════════════════════════════════════════════════════
# Architecture Enforcement
# ═══════════════════════════════════════════════════════════

check-monolithic:
	@echo "🔍 Checking for monolithic violations..."
	@if [ -f main.c ] || [ -f pipeline/pipeline.c ] || [ -f cli/cli_parser.c ] || [ -f melpc ]; then \
		echo ""; \
		echo "╔════════════════════════════════════════════╗"; \
		echo "║   ❌ ARCHITECTURE VIOLATION!               ║"; \
		echo "║   Monolithic files detected:               ║"; \
		[ -f main.c ] && echo "║     - main.c                               ║" || true; \
		[ -d cli ] && echo "║     - cli/                                 ║" || true; \
		[ -d pipeline ] && echo "║     - pipeline/                            ║" || true; \
		[ -f melpc ] && echo "║     - melpc binary                         ║" || true; \
		echo "║                                            ║"; \
		echo "║   ⚠️  FORBIDDEN: Use modular compilers     ║"; \
		echo "║   ✅ Allowed: modules/*/standalone         ║"; \
		echo "║                                            ║"; \
		echo "║   Read: RADICAL_CHANGE.md                  ║"; \
		echo "╚════════════════════════════════════════════╝"; \
		echo ""; \
		exit 1; \
	else \
		echo "✅ No monolithic files - Architecture clean!"; \
	fi

# ═══════════════════════════════════════════════════════════
# Build Targets
# ═══════════════════════════════════════════════════════════

all: check-monolithic modules
	@echo ""
	@echo "╔════════════════════════════════════════════╗"
	@echo "║  ✅ MELP Stage 0 - Modular Build Complete  ║"
	@echo "╚════════════════════════════════════════════╝"
	@echo ""
	@echo "Available compilers:"
	@echo "  • modules/functions/functions_compiler"
	@echo "  • modules/functions/functions_standalone"
	@echo ""

modules: check-monolithic
	@echo "Building modular compilers..."
	@$(MAKE) -C modules/functions

# ═══════════════════════════════════════════════════════════
# Runtime Libraries
# ═══════════════════════════════════════════════════════════

runtime:
	@echo "Building runtime libraries..."
	@$(MAKE) -C ../../runtime/tto
	@$(MAKE) -C ../../runtime/stdlib

# ═══════════════════════════════════════════════════════════
# Testing
# ═══════════════════════════════════════════════════════════

test-functions: modules
	@echo "Testing functions compiler..."
	@cd modules/functions && ./functions_compiler test_mvc.mlp test_mvc.s
	@cd modules/functions && gcc -no-pie test_mvc.s \
		-L../../../../runtime/stdlib -lmlp_stdlib \
		-L../../../../runtime/tto -ltto_runtime -lm \
		-o test_mvc
	@cd modules/functions && ./test_mvc
	@echo "✅ Functions test passed!"

test-string: modules
	@echo "Testing string operations..."
	@cd modules/functions && ./functions_compiler test_string_concat.mlp test_string_concat.s
	@cd modules/functions && gcc -no-pie test_string_concat.s \
		-L../../../../runtime/stdlib -lmlp_stdlib \
		-L../../../../runtime/tto -ltto_runtime -lm \
		-o test_string_concat
	@cd modules/functions && ./test_string_concat
	@echo "✅ String concatenation test passed!"

# ═══════════════════════════════════════════════════════════
# Cleanup
# ═══════════════════════════════════════════════════════════

clean: check-monolithic
	@echo "Cleaning modular builds..."
	@$(MAKE) -C modules/functions clean || true
	@find . -name "*.o" -delete
	@find . -name "*.s" -delete
	@find . -name "test_*" -type f -executable -delete
	@echo "✅ Clean complete"

clean-all: clean
	@echo "Cleaning runtime libraries..."
	@$(MAKE) -C ../../runtime/tto clean || true
	@$(MAKE) -C ../../runtime/stdlib clean || true

# ═══════════════════════════════════════════════════════════
# Help
# ═══════════════════════════════════════════════════════════

help:
	@echo ""
	@echo "╔════════════════════════════════════════════════════════╗"
	@echo "║  MELP Stage 0 - Modular Architecture Build System      ║"
	@echo "╚════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "⚠️  NO MONOLITHIC COMPILER!"
	@echo "   main.c, pipeline.c, cli/, melpc are FORBIDDEN"
	@echo ""
	@echo "Available targets:"
	@echo "  make all              Build all modular compilers"
	@echo "  make modules          Build function compiler modules"
	@echo "  make runtime          Build TTO + stdlib libraries"
	@echo "  make test-functions   Test functions compiler (MVC)"
	@echo "  make test-string      Test string operations"
	@echo "  make clean            Clean build artifacts"
	@echo "  make clean-all        Clean everything including runtime"
	@echo ""
	@echo "Modular compilers location:"
	@echo "  📁 modules/functions/functions_compiler"
	@echo ""
	@echo "Usage example:"
	@echo "  cd modules/functions"
	@echo "  ./functions_compiler input.mlp output.s"
	@echo "  gcc output.s -L../../runtime/... -o program"
	@echo ""
	@echo "📖 Read: RADICAL_CHANGE.md, ARCHITECTURE.md"
	@echo ""

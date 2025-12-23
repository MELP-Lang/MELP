# Functions Module - Standalone Test Compiler Makefile
# Modül #7 (P0 Critical)
#
# Bağımlılıklar:
#   - variable (modül #1)
#   - comments (modül #2)
#   - arithmetic (modül #3)
#   - comparison (modül #4)
#   - logical (modül #5)
#   - control_flow (modül #6)

CC = gcc
CFLAGS = -Wall -Wextra -g -I../.. -I../../../../runtime/sto -I../../../../runtime/stdlib
LDFLAGS = -L../../../../runtime/stdlib -lmlp_stdlib -L../../../../runtime/sto -lsto_runtime -lm
NASM = nasm
NASMFLAGS = -f elf64
LD = ld

# Ana hedef
TARGET = functions_compiler

# Ana kaynak dosyalar
MAIN_SRC = functions_standalone.c

# Functions modülü kaynakları
FUNCTIONS_SOURCES = \
	functions.c \
	functions_parser.c \
	functions_codegen.c \
	functions_codegen_llvm.c \
	functions_generic.c \
	find_function_by_name.c

# Variable modülü (Modül #1)
VARIABLE_DIR = ../variable
VARIABLE_SOURCES = \
	$(VARIABLE_DIR)/variable.c \
	$(VARIABLE_DIR)/variable_parser.c \
	$(VARIABLE_DIR)/variable_codegen.c

# Comments modülü (Modül #2)
COMMENTS_DIR = ../comments
COMMENTS_SOURCES = \
	$(COMMENTS_DIR)/comments.c \
	$(COMMENTS_DIR)/comments_parser.c

# Arithmetic modülü (Modül #3)
ARITHMETIC_DIR = ../arithmetic
ARITHMETIC_SOURCES = \
	$(ARITHMETIC_DIR)/arithmetic.c \
	$(ARITHMETIC_DIR)/arithmetic_parser.c \
	$(ARITHMETIC_DIR)/arithmetic_codegen.c \
	$(ARITHMETIC_DIR)/arithmetic_optimize.c \
	$(ARITHMETIC_DIR)/string_interpolation.c

# Comparison modülü (Modül #4)
COMPARISON_DIR = ../comparison
COMPARISON_SOURCES = \
	$(COMPARISON_DIR)/comparison.c \
	$(COMPARISON_DIR)/comparison_parser.c \
	$(COMPARISON_DIR)/comparison_codegen.c

# Logical modülü (Modül #5)
LOGICAL_DIR = ../logical
LOGICAL_SOURCES = \
	$(LOGICAL_DIR)/logical.c \
	$(LOGICAL_DIR)/logical_parser.c \
	$(LOGICAL_DIR)/logical_codegen.c

# Control Flow modülü (Modül #6)
CONTROL_FLOW_DIR = ../control_flow
CONTROL_FLOW_SOURCES = \
	$(CONTROL_FLOW_DIR)/control_flow.c \
	$(CONTROL_FLOW_DIR)/control_flow_parser.c \
	$(CONTROL_FLOW_DIR)/control_flow_codegen.c

# For Loop modülü (Modül #7 - Phase 3.3)
FOR_LOOP_DIR = ../for_loop
FOR_LOOP_SOURCES = \
	$(FOR_LOOP_DIR)/for_loop.c \
	$(FOR_LOOP_DIR)/for_loop_parser.c \
	$(FOR_LOOP_DIR)/for_loop_codegen.c

# Switch modülü (YZ_89 - Phase 21)
SWITCH_DIR = ../switch
SWITCH_SOURCES = \
	$(SWITCH_DIR)/switch.c \
	$(SWITCH_DIR)/switch_parser.c \
	$(SWITCH_DIR)/switch_codegen.c

# Print modülü (YZ_61 - Phase 15)
PRINT_DIR = ../print
PRINT_SOURCES = \
	$(PRINT_DIR)/print.c \
	$(PRINT_DIR)/print_parser.c \
	$(PRINT_DIR)/print_codegen.c

# Array modülü (YZ_14 - Array indexing)
ARRAY_DIR = ../array
ARRAY_SOURCES = \
	$(ARRAY_DIR)/array_parser.c \
	$(ARRAY_DIR)/array_codegen.c

# Statement modülü (Body parsing için)
STATEMENT_DIR = ../statement
STATEMENT_SOURCES = \
	$(STATEMENT_DIR)/statement.c \
	$(STATEMENT_DIR)/statement_parser.c \
	$(STATEMENT_DIR)/statement_codegen.c \
	$(STATEMENT_DIR)/statement_optimize.c

# Import modülü (YZ_35 - Module system)
IMPORT_DIR = ../import
IMPORT_SOURCES = \
	$(IMPORT_DIR)/import.c \
	$(IMPORT_DIR)/import_parser.c \
	$(IMPORT_DIR)/import_cache.c \
	$(IMPORT_DIR)/import_cache_persist.c \
	$(IMPORT_DIR)/module_declaration.c \
	$(IMPORT_DIR)/export_tracker.c \
	$(IMPORT_DIR)/namespace_resolver.c

# Struct modülü (YZ_81 - Struct definitions)
STRUCT_DIR = ../struct
STRUCT_SOURCES = \
	$(STRUCT_DIR)/struct.c \
	$(STRUCT_DIR)/struct_parser.c \
	$(STRUCT_DIR)/struct_codegen.c

# Enum modülü (YZ_96 - Enum types)
ENUM_DIR = ../enum
ENUM_SOURCES = \
	$(ENUM_DIR)/enum.c \
	$(ENUM_DIR)/enum_parser.c \
	$(ENUM_DIR)/enum_codegen.c

# Type System modülü (YZ_203.5 - Generic type inference)
TYPE_SYSTEM_DIR = ../type_system
TYPE_SYSTEM_SOURCES = \
	$(TYPE_SYSTEM_DIR)/type_inference.c

# Parser Core modülü (Parser structure)
PARSER_CORE_DIR = ../parser_core
PARSER_CORE_SOURCES = \
	$(PARSER_CORE_DIR)/parser_core.c

# Codegen Context (TTO desteği için)
CODEGEN_CONTEXT_DIR = ../codegen_context
CODEGEN_CONTEXT_SOURCES = $(CODEGEN_CONTEXT_DIR)/codegen_context.c

# Error Handling modülü
ERROR_DIR = ../error
ERROR_SOURCES = $(ERROR_DIR)/error.c

# LLVM Backend modülü (YZ_58)
LLVM_BACKEND_DIR = ../llvm_backend
LLVM_BACKEND_SOURCES = $(LLVM_BACKEND_DIR)/llvm_backend.c

# Lexer modülü
LEXER_DIR = ../lexer
LEXER_SOURCES = $(LEXER_DIR)/lexer.c

# ⭐ RF_YZ_1: Normalize modülü (user syntax -> PMPL transformation)
NORMALIZE_DIR = ../../normalize
NORMALIZE_SOURCES = $(NORMALIZE_DIR)/normalize.c

# Tüm kaynak dosyalar
SOURCES = \
	$(MAIN_SRC) \
	$(FUNCTIONS_SOURCES) \
	$(VARIABLE_SOURCES) \
	$(COMMENTS_SOURCES) \
	$(ARITHMETIC_SOURCES) \
	$(COMPARISON_SOURCES) \
	$(LOGICAL_SOURCES) \
	$(CONTROL_FLOW_SOURCES) \
	$(FOR_LOOP_SOURCES) \
	$(SWITCH_SOURCES) \
	$(PRINT_SOURCES) \
	$(ARRAY_SOURCES) \
	$(STATEMENT_SOURCES) \
	$(IMPORT_SOURCES) \
	$(STRUCT_SOURCES) \
	$(ENUM_SOURCES) \
	$(TYPE_SYSTEM_SOURCES) \
	$(PARSER_CORE_SOURCES) \
	$(CODEGEN_CONTEXT_SOURCES) \
	$(ERROR_SOURCES) \
	$(LLVM_BACKEND_SOURCES) \
	$(NORMALIZE_SOURCES) \
	$(LEXER_SOURCES)

# Object dosyaları
OBJECTS = $(SOURCES:.c=.o)

# Test dosyaları
TEST_SOURCE = test_func.mlp
TEST_ASM = test_func.s
TEST_OBJ = test_func.o
TEST_EXEC = test_func

# Varsayılan hedef
all: info $(TARGET)
	@echo ""
	@echo "✅ Functions Standalone Compiler hazır!"
	@echo "   Binary: $(TARGET)"
	@echo "   Boyut: $$(du -h $(TARGET) | cut -f1)"
	@echo ""
	@echo "Test için: make test"

# Bilgilendirme
info:
	@echo "=========================================="
	@echo "  FUNCTIONS MODULE BUILD"
	@echo "  Modül #7 (P0 Critical)"
	@echo "=========================================="
	@echo "Bağımlılıklar:"
	@echo "  • variable (Modül #1)"
	@echo "  • comments (Modül #2)"
	@echo "  • arithmetic (Modül #3)"
	@echo "  • comparison (Modül #4)"
	@echo "  • logical (Modül #5)"
	@echo "  • control_flow (Modül #6)"
	@echo ""
	@echo "Toplam kaynak dosya sayısı: $$(echo $(SOURCES) | wc -w)"
	@echo ""

# Ana derleyici
$(TARGET): $(OBJECTS) ../../../../runtime/sto/libsto_runtime.a ../../../../runtime/stdlib/libmlp_stdlib.a
	@echo "🔗 Linking: $(TARGET)"
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Standalone executable (simplified build)
functions_standalone: functions_standalone.o $(filter-out functions_standalone.o,$(OBJECTS))
	@echo "🔗 Linking: functions_standalone"
	$(CC) $(CFLAGS) -o $@ $^ -lm

# C dosyalarını derle
%.o: %.c
	@echo "  📦 Compiling: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Test hedefi
test: $(TARGET)
	@echo ""
	@echo "=========================================="
	@echo "  FUNCTIONS MODULE TEST"
	@echo "=========================================="
	@echo ""
	@echo "📝 Test dosyası oluşturuluyor..."
	@echo 'sayisal x = 10' > $(TEST_SOURCE)
	@echo 'sayisal y = 20' >> $(TEST_SOURCE)
	@echo '' >> $(TEST_SOURCE)
	@echo 'def topla(sayisal a, sayisal b) {' >> $(TEST_SOURCE)
	@echo '    sayisal sonuc = a + b' >> $(TEST_SOURCE)
	@echo '    return sonuc' >> $(TEST_SOURCE)
	@echo '}' >> $(TEST_SOURCE)
	@echo '' >> $(TEST_SOURCE)
	@echo 'def carpma(sayisal a, sayisal b) {' >> $(TEST_SOURCE)
	@echo '    return a * b' >> $(TEST_SOURCE)
	@echo '}' >> $(TEST_SOURCE)
	@echo '' >> $(TEST_SOURCE)
	@echo 'sayisal z = topla(x, y)' >> $(TEST_SOURCE)
	@echo 'sayisal w = carpma(x, 5)' >> $(TEST_SOURCE)
	@echo ""
	@echo "Test dosyası içeriği ($$(wc -c < $(TEST_SOURCE)) bytes):"
	@cat $(TEST_SOURCE)
	@echo ""
	@echo "---"
	@echo ""
	@echo "🔨 Derleme başlıyor..."
	./$(TARGET) $(TEST_SOURCE) $(TEST_ASM)
	@echo ""
	@echo "📜 Üretilen Assembly (ilk 50 satır):"
	@head -50 $(TEST_ASM)
	@echo "..."
	@echo ""
	@echo "🔧 NASM ile assembly..."
	$(NASM) $(NASMFLAGS) $(TEST_ASM) -o $(TEST_OBJ)
	@echo "  ✅ NASM: OK"
	@echo ""
	@echo "🔗 Linking..."
	$(LD) $(TEST_OBJ) -o $(TEST_EXEC)
	@echo "  ✅ Linking: OK"
	@echo ""
	@echo "🚀 Çalıştırılıyor..."
	./$(TEST_EXEC) && echo "  ✅ Execution: OK (exit code $$?)" || echo "  ❌ Execution failed (exit code $$?)"
	@echo ""
	@echo "=========================================="
	@echo "  ✅ TEST TAMAMLANDI"
	@echo "=========================================="

# Temizlik
clean:
	@echo "🧹 Temizlik yapılıyor..."
	rm -f $(OBJECTS) $(TARGET) $(TEST_SOURCE) $(TEST_ASM) $(TEST_OBJ) $(TEST_EXEC)
	rm -f *.ll *.o test_* *.mlp
	@echo "✅ Temizlendi!"

# LLVM test (YZ_61 - Phase 15)
test_llvm: $(TARGET)
	@echo "Testing LLVM backend with stdlib integration..."
	./$(TARGET) -c --backend=llvm $(file) $(file:.mlp=.ll)
	clang $(file:.mlp=.ll) \
		-L../../../../runtime/stdlib -lmlp_stdlib \
		-L../../../../runtime/sto -lsto_runtime -lm \
		-o $(file:.mlp=)
	./$(file:.mlp=)

.PHONY: all info test clean test_llvm

# Arithmetic Module - Standalone Makefile
# =========================================

CC = gcc
CFLAGS = -Wall -g -I../.. -I../../../../runtime/sto
LDFLAGS = -lm

# TTO Runtime library path
TTO_LIB = ../../../../runtime/sto/libsto_runtime.a

# Source files
SOURCES = arithmetic_standalone.c \
          arithmetic.c \
          arithmetic_parser.c \
          arithmetic_codegen.c \
          arithmetic_optimize.c \
          ../variable/variable.c \
          ../variable/variable_parser.c \
          ../variable/variable_codegen.c \
          ../codegen_context/codegen_context.c \
          ../lexer/lexer.c \
          ../comments/comments.c

OBJECTS = $(SOURCES:.c=.o)
TARGET = arithmetic_compiler

# Build standalone compiler
all: $(TARGET)
	@echo "✅ Arithmetic module standalone compiler built!"
	@echo "   Usage: ./$(TARGET) input.mlp output.s"

$(TARGET): $(OBJECTS) $(TTO_LIB)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(TTO_LIB) $(LDFLAGS)
	@echo "   📦 Linked with TTO Runtime (BigDecimal + SSO)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(TARGET) test*.mlp test*.s test*.o test_program
	@echo "🧹 Cleaned arithmetic module"

# Test
test: $(TARGET)
	@echo "🧪 Testing arithmetic module..."
	@echo "numeric x = 10 + 20" > test_arith.mlp
	@echo "numeric y = 5 * 3" >> test_arith.mlp
	@echo "numeric z = 100 - 25" >> test_arith.mlp
	@./$(TARGET) test_arith.mlp test_arith.s
	@echo ""
	@echo "📄 Generated assembly:"
	@head -30 test_arith.s
	@echo ""
	@nasm -f elf64 test_arith.s -o test_arith.o && echo "✅ Assembly OK"
	@ld test_arith.o -o test_arith_prog && echo "✅ Link OK"
	@./test_arith_prog && echo "✅ Execution OK"

.PHONY: all clean test

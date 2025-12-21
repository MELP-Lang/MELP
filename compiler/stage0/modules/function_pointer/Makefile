# Function Pointer Module - Standalone Makefile
# YZ_209: Function Pointers

CC = gcc
CFLAGS = -Wall -Wextra -g -I../.. -I../../../../runtime/sto -I../../../../runtime/stdlib
LDFLAGS = -L../../../../runtime/stdlib -lmlp_stdlib -L../../../../runtime/sto -lsto_runtime -lm

# Target
TARGET = function_pointer_standalone

# Source files
STANDALONE_SRC = function_pointer_standalone.c

# Module objects
MODULE_OBJS = function_pointer.o function_pointer_parser.o function_pointer_codegen.o

# Core dependencies
LEXER_DIR = ../lexer
LEXER_OBJ = $(LEXER_DIR)/lexer.o

PARSER_CORE_DIR = ../parser_core
PARSER_CORE_OBJ = $(PARSER_CORE_DIR)/parser_core.o

ERROR_DIR = ../error
ERROR_OBJ = $(ERROR_DIR)/error.o

COMMENTS_DIR = ../comments
COMMENTS_OBJ = $(COMMENTS_DIR)/comments.o

FUNCTIONS_DIR = ../functions
FUNCTIONS_OBJ = $(FUNCTIONS_DIR)/functions.o

LLVM_BACKEND_DIR = ../llvm_backend
LLVM_BACKEND_OBJ = $(LLVM_BACKEND_DIR)/llvm_backend.o

# All objects
ALL_OBJS = $(MODULE_OBJS) $(LEXER_OBJ) $(PARSER_CORE_OBJ) $(ERROR_OBJ) $(COMMENTS_OBJ) $(FUNCTIONS_OBJ) $(LLVM_BACKEND_OBJ)

.PHONY: all clean test

all: $(TARGET)
	@echo "✅ Function Pointer Module built: $(TARGET)"
	@ls -lh $(TARGET)

$(TARGET): $(STANDALONE_SRC) $(ALL_OBJS)
	$(CC) $(CFLAGS) $(STANDALONE_SRC) $(ALL_OBJS) -o $(TARGET) $(LDFLAGS)

# Module objects
function_pointer.o: function_pointer.c function_pointer.h
	$(CC) $(CFLAGS) -c $< -o $@

function_pointer_parser.o: function_pointer_parser.c function_pointer_parser.h function_pointer.h
	$(CC) $(CFLAGS) -c $< -o $@

function_pointer_codegen.o: function_pointer_codegen.c function_pointer_codegen.h function_pointer.h
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencies (build if needed)
$(LEXER_OBJ):
	$(MAKE) -C $(LEXER_DIR)

$(PARSER_CORE_OBJ):
	$(MAKE) -C $(PARSER_CORE_DIR)

$(ERROR_OBJ):
	$(MAKE) -C $(ERROR_DIR)

$(COMMENTS_OBJ):
	$(MAKE) -C $(COMMENTS_DIR)

$(FUNCTIONS_OBJ):
	$(MAKE) -C $(FUNCTIONS_DIR)

$(LLVM_BACKEND_OBJ):
	$(MAKE) -C $(LLVM_BACKEND_DIR)

clean:
	rm -f *.o $(TARGET) test_*.s test_*.o test_*_bin *.ll

test: $(TARGET)
	@echo "🧪 Testing function pointer module..."
	@if [ -f test_function_pointer.mlp ]; then \
		./$(TARGET) test_function_pointer.mlp test_function_pointer.ll && \
		echo "✅ Test passed"; \
	else \
		echo "⚠️  No test file found"; \
	fi

help:
	@echo "Function Pointer Module Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make all     - Build module"
	@echo "  make clean   - Clean build artifacts"
	@echo "  make test    - Run tests"
	@echo "  make help    - Show this help"

# Package Module - Makefile
# YZ_205: Package Structure System

CC = gcc
CFLAGS = -Wall -Wextra -g -I../.. -I..
LDFLAGS = -lm

# Target
TARGET = test_package

# Package module sources
PACKAGE_SOURCES = \
	package_parser.c

# Lexer module (required for tokenization)
LEXER_DIR = ../lexer
LEXER_SOURCES = \
	$(LEXER_DIR)/lexer.c

# Comments module (required by lexer)
COMMENTS_DIR = ../comments
COMMENTS_SOURCES = \
	$(COMMENTS_DIR)/comments.c \
	$(COMMENTS_DIR)/comments_parser.c

# All sources
SOURCES = \
	$(PACKAGE_SOURCES) \
	$(LEXER_SOURCES) \
	$(COMMENTS_SOURCES) \
	../../../../tests/package/test_package.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build test executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o ../../../../tests/package/$(TARGET) $(OBJECTS) $(LDFLAGS)
	@echo "✅ Package test driver built: tests/package/$(TARGET)"

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS)
	rm -f ../../../../tests/package/$(TARGET)
	@echo "🧹 Cleaned package module"

# Test targets
test: $(TARGET)
	@echo "\n=== Running Package Parser Tests ==="
	@echo "\n--- Test 1: Basic Package ---"
	-../../../../tests/package/$(TARGET) ../../../../tests/package/test_basic_package.mlp
	@echo "\n--- Test 2: Dependencies ---"
	-../../../../tests/package/$(TARGET) ../../../../tests/package/test_dependencies.mlp
	@echo "\n--- Test 3: Minimal ---"
	-../../../../tests/package/$(TARGET) ../../../../tests/package/test_minimal.mlp
	@echo "\n--- Test 4: Complete ---"
	-../../../../tests/package/$(TARGET) ../../../../tests/package/test_complete.mlp
	@echo "\n--- Test 5: Invalid (should fail) ---"
	-../../../../tests/package/$(TARGET) ../../../../tests/package/test_invalid.mlp
	@echo "\n=== Tests Complete ==="

# Install target (optional)
install:
	@echo "Package parser module ready"

.PHONY: all clean test install

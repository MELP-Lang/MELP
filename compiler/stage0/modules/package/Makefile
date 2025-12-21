# Package Module - Makefile
# YZ_205: Package Structure System

CC = gcc
CFLAGS = -Wall -Wextra -g -I../.. -I..
LDFLAGS = -lm

# Targets
TARGET_PARSER = test_package
TARGET_RESOLVER = test_resolver
TARGET_BUILDER = test_builder

# Package module sources
PACKAGE_SOURCES = \
	package_parser.c \
	dependency_graph.c \
	dependency_resolver.c \
	package_builder.c \
	package_validator.c

# Lexer module (required for tokenization)
LEXER_DIR = ../lexer
LEXER_SOURCES = \
	$(LEXER_DIR)/lexer.c

# Comments module (required by lexer)
COMMENTS_DIR = ../comments
COMMENTS_SOURCES = \
	$(COMMENTS_DIR)/comments.c \
	$(COMMENTS_DIR)/comments_parser.c

# Common object files
COMMON_OBJS = package_parser.o dependency_graph.o dependency_resolver.o \
	package_builder.o package_validator.o \
	$(LEXER_DIR)/lexer.o $(COMMENTS_DIR)/comments.o $(COMMENTS_DIR)/comments_parser.o

# Default target
all: $(TARGET_PARSER) $(TARGET_RESOLVER) $(TARGET_BUILDER)

# Build parser test executable
$(TARGET_PARSER): $(COMMON_OBJS) ../../../../tests/package/test_package.o
	$(CC) $(CFLAGS) -o ../../../../tests/package/$(TARGET_PARSER) $^ $(LDFLAGS)
	@echo "✅ Package parser test built: tests/package/$(TARGET_PARSER)"
# Build resolver test executable
$(TARGET_RESOLVER): $(COMMON_OBJS) ../../../../tests/package/test_resolver.o
	$(CC) $(CFLAGS) -o ../../../../tests/package/$(TARGET_RESOLVER) $^ $(LDFLAGS)
	@echo "✅ Dependency resolver test built: tests/package/$(TARGET_RESOLVER)"

# Build builder/validator test executable
$(TARGET_BUILDER): $(COMMON_OBJS) ../../../../tests/package/test_builder.o
	$(CC) $(CFLAGS) -o ../../../../tests/package/$(TARGET_BUILDER) $^ $(LDFLAGS)
	@echo "✅ Package builder test built: tests/package/$(TARGET_BUILDER)"

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o
	rm -f $(LEXER_DIR)/*.o
	rm -f $(COMMENTS_DIR)/*.o
	rm -f ../../../../tests/package/*.o
	rm -f ../../../../tests/package/$(TARGET_PARSER)
	rm -f ../../../../tests/package/$(TARGET_RESOLVER)
	rm -f ../../../../tests/package/$(TARGET_BUILDER)
	@echo "🧹 Cleaned package module"
	rm -f $(LEXER_DIR)/*.o
	rm -f $(COMMENTS_DIR)/*.o
	rm -f ../../../../tests/package/*.o
	rm -f ../../../../tests/package/$(TARGET_PARSER)
	rm -f ../../../../tests/package/$(TARGET_RESOLVER)
	@echo "🧹 Cleaned package module"

# Test targets
test-parser: $(TARGET_PARSER)
	@echo "\n=== Running Package Parser Tests ==="
	@echo "\n--- Test 1: Basic Package ---"
	-../../../../tests/package/$(TARGET_PARSER) ../../../../tests/package/test_basic_package.mlp
	@echo "\n--- Test 2: Dependencies ---"
	-../../../../tests/package/$(TARGET_PARSER) ../../../../tests/package/test_dependencies.mlp
	@echo "\n--- Test 3: Minimal ---"
test-resolver: $(TARGET_RESOLVER)
	@echo "\n=== Running Dependency Resolver Tests ==="
	cd ../../../../ && ./tests/package/$(TARGET_RESOLVER)
	@echo "\n=== Resolver Tests Complete ==="

test-builder: $(TARGET_BUILDER)
	@echo "\n=== Running Builder & Validator Tests ==="
	cd ../../../../ && ./tests/package/$(TARGET_BUILDER)
	@echo "\n=== Builder Tests Complete ==="

test: test-parser test-resolver test-builder
	@echo "\n✅ All tests complete!"
test-resolver: $(TARGET_RESOLVER)
	@echo "\n=== Running Dependency Resolver Tests ==="
	cd ../../../../ && ./tests/package/$(TARGET_RESOLVER)
	@echo "\n=== Resolver Tests Complete ==="

test: test-parser test-resolver
	@echo "\n✅ All tests complete!"

# Install target (optional)
install:
	@echo "Package module ready (parser + resolver)"

.PHONY: all clean test test-parser test-resolver install


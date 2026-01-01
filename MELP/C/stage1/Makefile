CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = 

SRCS = main.c lexer.c parser.c codegen.c mlp_runtime.c
OBJS = $(SRCS:.c=.o)
TARGET = melpc_stage1

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✅ Stage1 compiler built: ./$(TARGET)"

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned"

test: $(TARGET)
	@echo "=== Testing Stage1 Compiler ==="
	@echo "Test 1: Simple function"
	./$(TARGET) ../../MLP/stage2/test_simple.mlp test_simple.c
	gcc -o test_simple test_simple.c mlp_runtime.c
	./test_simple
	@echo "Exit code: $$?"
	@echo ""

.PHONY: help
help:
	@echo "MLP Stage1 Bridge Compiler - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build compiler"
	@echo "  make clean    - Remove build files"
	@echo "  make test     - Run test suite"

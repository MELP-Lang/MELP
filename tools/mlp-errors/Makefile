# Makefile for mlp-errors

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -D_GNU_SOURCE
TARGET = mlp-errors
INSTALL_DIR = ../../build

SRCS = main.c error_loader.c error_formatter.c cJSON.c
OBJS = $(SRCS:.c=.o)
HEADERS = error_types.h cJSON.h

.PHONY: all clean install test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	mkdir -p $(INSTALL_DIR)
	cp $(TARGET) $(INSTALL_DIR)/

test: $(TARGET)
	@echo "Testing mlp-errors..."
	@echo "\n=== Test 1: E0001 (Undefined Variable) ==="
	./$(TARGET) E0001 ../../test_minimal.mlp 5 10 x
	@echo "\n=== Test 2: E0002 (Type Mismatch) ==="
	./$(TARGET) E0002 ../../test_minimal.mlp 12 5 int string
	@echo "\n=== Test 3: W0001 (Unused Variable) ==="
	./$(TARGET) W0001 ../../test_minimal.mlp 8 5 unused
	@echo "\n=== Test 4: English language ==="
	MLP_LANG=en ./$(TARGET) E0001 ../../test_minimal.mlp 5 10 myVar
	@echo "\nAll tests passed!"

help:
	@echo "mlp-errors - MELP Error Message System"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build mlp-errors binary (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install to build directory"
	@echo "  test     - Run test suite"
	@echo "  help     - Show this help"
	@echo ""
	@echo "Usage:"
	@echo "  mlp-errors <code> <file> <line> <col> [placeholders...]"
	@echo ""
	@echo "Examples:"
	@echo "  mlp-errors E0001 test.mlp 5 10 myVar"
	@echo "  MLP_LANG=en mlp-errors E0002 main.mlp 12 5 int string"

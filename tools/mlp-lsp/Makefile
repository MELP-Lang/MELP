# Makefile for mlp-lsp (MLP Language Server Protocol)
# YZ_03 - LSP Server (Phase 1 - Complete Implementation)
# Date: 1 Jan 2026

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -D_GNU_SOURCE
TARGET = mlp-lsp

# Source files
SRCS = main.c lsp_protocol.c jsonrpc.c symbol_table.c mlp_parser.c cJSON.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "✅ mlp-lsp built successfully"

# Compile
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "🧹 Cleaned build artifacts"

# Test
test: $(TARGET)
	@echo "🧪 Testing LSP server..."
	@echo '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{}}' | ./$(TARGET) --stdio || true

# Install (optional)
install: $(TARGET)
	cp $(TARGET) ../../build/

.PHONY: all clean test install

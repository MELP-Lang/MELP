# Codegen Emit Module
# Purpose: Generate C code instead of assembly
# Part of: MLP-GCC Self-Hosting Phase 0

CC = gcc
CFLAGS = -Wall -Wextra -g -I../..

# Source files
SRCS = codegen_emit.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(OBJS)
	@echo "✅ codegen_emit module compiled"

# Compile object files
%.o: %.c codegen_emit.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS)
	@echo "🧹 codegen_emit module cleaned"

# Test
test: all
	@echo "✅ codegen_emit module ready"

.PHONY: all clean test

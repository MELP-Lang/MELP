# LLVM Backend Module Makefile
# YZ_58 - Phase 13.5 Part 2

CC = gcc
CFLAGS = -Wall -Wextra -g -I../error -I../lexer
AR = ar
ARFLAGS = rcs

# Target library
TARGET = libllvm_backend.a

# Source files
SRCS = llvm_backend.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET) test

# Build library
$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "✅ Built LLVM backend library: $(TARGET)"

# Compile object files
%.o: %.c llvm_backend.h
	$(CC) $(CFLAGS) -c $< -o $@

# Test program
test: test_llvm_backend
	@echo "Running LLVM backend tests..."
	@./test_llvm_backend > test_output.ll
	@echo "✅ Generated test_output.ll"
	@clang test_output.ll -o test_output && ./test_output
	@echo "✅ Test passed!"

test_llvm_backend: test_llvm_backend.c $(TARGET)
	$(CC) $(CFLAGS) test_llvm_backend.c -L. -lllvm_backend -o test_llvm_backend

# Clean
clean:
	rm -f $(OBJS) $(TARGET) test_llvm_backend test_output.ll test_output

.PHONY: all test clean

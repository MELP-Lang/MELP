# Makefile for mlp-fmt

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = mlp-fmt
OBJS = main.o fmt_lexer.o fmt_formatter.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c fmt_types.h fmt_lexer.h fmt_formatter.h
	$(CC) $(CFLAGS) -c main.c

fmt_lexer.o: fmt_lexer.c fmt_lexer.h fmt_types.h
	$(CC) $(CFLAGS) -c fmt_lexer.c

fmt_formatter.o: fmt_formatter.c fmt_formatter.h fmt_types.h
	$(CC) $(CFLAGS) -c fmt_formatter.c

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	@echo "Running formatter tests..."
	@./test_fmt.sh

.PHONY: all clean test

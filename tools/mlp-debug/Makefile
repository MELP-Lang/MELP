# Makefile for mlp-debug (MLP Debugger)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = mlp-debug
OBJS = main.o source_mapper.o gdb_wrapper.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c source_mapper.h gdb_wrapper.h
	$(CC) $(CFLAGS) -c main.c

source_mapper.o: source_mapper.c source_mapper.h
	$(CC) $(CFLAGS) -c source_mapper.c

gdb_wrapper.o: gdb_wrapper.c gdb_wrapper.h source_mapper.h
	$(CC) $(CFLAGS) -c gdb_wrapper.c

clean:
	rm -f $(OBJS) $(TARGET)

install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

test: $(TARGET)
	@echo "Testing mlp-debug..."
	@./$(TARGET) --help

.PHONY: all clean install test

# Makefile for MLP Runtime Library (8 modules)
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -lm

OBJS = runtime_error.o runtime_memory.o runtime_string.o runtime_types.o \
       runtime_array.o runtime_print.o runtime_io.o runtime_system.o

all: libruntime.a

libruntime.a: $(OBJS)
ar rcs $@ $^

runtime_error.o: runtime_error.c runtime_error.h
$(CC) $(CFLAGS) -c $<

runtime_memory.o: runtime_memory.c runtime_memory.h runtime_error.h
$(CC) $(CFLAGS) -c $<

runtime_string.o: runtime_string.c runtime_string.h runtime_memory.h
$(CC) $(CFLAGS) -c $<

runtime_types.o: runtime_types.c runtime_types.h runtime_memory.h
$(CC) $(CFLAGS) -c $<

runtime_array.o: runtime_array.c runtime_array.h runtime_memory.h
$(CC) $(CFLAGS) -c $<

runtime_print.o: runtime_print.c runtime_print.h runtime_memory.h
$(CC) $(CFLAGS) -c $<

runtime_io.o: runtime_io.c runtime_io.h runtime_memory.h
$(CC) $(CFLAGS) -c $<

runtime_system.o: runtime_system.c runtime_system.h runtime_memory.h
$(CC) $(CFLAGS) -c $<

clean:
rm -f *.o libruntime.a

.PHONY: all clean

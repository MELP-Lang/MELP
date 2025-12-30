# Makefile for MLP Runtime Library (8 runtime + 7 stdlib modules)
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -I.
LDFLAGS = -lm

# Runtime modules (8)
RUNTIME_OBJS = runtime_error.o runtime_memory.o runtime_string.o runtime_types.o \
               runtime_array.o runtime_print.o runtime_io.o runtime_system.o

# Stdlib modules (7) - from P2
# Note: file_io.c excluded (duplicate functions with mlp_io.c)
STDLIB_OBJS = mlp_list.o mlp_optional.o mlp_state.o mlp_panic.o \
              mlp_io.o mlp_math.o

# STO Stubs (temporary for bootstrap)
STO_STUBS = sto_stubs.o

OBJS = $(RUNTIME_OBJS) $(STDLIB_OBJS) $(STO_STUBS)

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

# Stdlib modules (from P2)
mlp_list.o: mlp_list.c mlp_list.h
	$(CC) $(CFLAGS) -c $<

mlp_optional.o: mlp_optional.c mlp_optional.h mlp_panic.h runtime_error.h
	$(CC) $(CFLAGS) -c $<

mlp_state.o: mlp_state.c mlp_state.h mlp_io.h
	$(CC) $(CFLAGS) -c $<

mlp_panic.o: mlp_panic.c mlp_panic.h
	$(CC) $(CFLAGS) -c $<

mlp_io.o: mlp_io.c mlp_io.h sto/sto_types.h
	$(CC) $(CFLAGS) -c $<

file_io.o: file_io.c file_io.h
	$(CC) $(CFLAGS) -c $<

mlp_math.o: mlp_math.c mlp_math.h
	$(CC) $(CFLAGS) -c $<

sto_stubs.o: sto_stubs.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o libruntime.a

.PHONY: all clean

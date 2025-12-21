# MLP Standard Library Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC -D_GNU_SOURCE
AR = ar
ARFLAGS = rcs

# Library
LIB = libmlp_stdlib.a

# Sources - YZ_94: Added mlp_math.c for min/max functions
# YZ_200: Added mlp_list.c for dynamic list operations
# YZ_201: Added mlp_map.c for hash table operations
# YZ_202: Added mlp_optional.c for nullable type support
# YZ_210: Added mlp_memory.c for reference counting GC
SOURCES = mlp_io.c mlp_string.c mlp_panic.c mlp_state.c mlp_math.c mlp_list.c mlp_map.c mlp_optional.c mlp_memory.c
OBJECTS = $(SOURCES:.c=.o)

all: $(LIB)

$(LIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "✅ MLP stdlib created: $(LIB)"

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(LIB)

.PHONY: all clean

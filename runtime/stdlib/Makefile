# MLP Standard Library Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC
AR = ar
ARFLAGS = rcs

# Library
LIB = libmlp_stdlib.a

# Sources
SOURCES = mlp_io.c mlp_string.c mlp_panic.c mlp_state.c
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

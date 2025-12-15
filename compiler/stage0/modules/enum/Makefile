# YZ_96: Enum Module Makefile

CC = gcc
CFLAGS = -Wall -Wextra -g -I../..

SOURCES = enum.c enum_parser.c enum_codegen.c
OBJECTS = $(SOURCES:.c=.o)

all: $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

.PHONY: all clean

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I.
TARGET = test_runtime_sto
SOURCES = runtime_sto.c test_runtime_sto.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all test clean

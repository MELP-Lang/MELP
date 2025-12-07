CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I.
AR = ar
ARFLAGS = rcs

# Targets
TARGET = test_runtime_tto
LIB = libtto_runtime.a
SOURCES = runtime_tto.c test_runtime_tto.c
LIB_OBJECTS = runtime_tto.o
TEST_OBJECTS = runtime_tto.o test_runtime_tto.o

all: $(LIB) $(TARGET)

# Static library for linking with compiler
$(LIB): $(LIB_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "✅ TTO runtime library created: $(LIB)"

$(TARGET): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(LIB_OBJECTS) $(TEST_OBJECTS) $(TARGET) $(LIB)

.PHONY: all test clean

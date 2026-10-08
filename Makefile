CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
TARGET := shell
SOURCES := src/main.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) src/shell.c
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(TARGET)

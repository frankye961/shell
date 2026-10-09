CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
TARGET := shell
SOURCES := src/main.c

.PHONY: all clean

all: $(TARGET) src/shell

$(TARGET): $(SOURCES) src/shell.c
	$(CC) $(CFLAGS) $(SOURCES) -o $@

src/shell: $(TARGET)
	cp $(TARGET) $@

clean:
	rm -f $(TARGET) src/shell

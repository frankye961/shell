CC ?= cc
CPPFLAGS ?= -D_POSIX_C_SOURCE=200809L -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic
TARGET := shell
SOURCES := src/main.c src/shell.c src/prompt.c src/history.c src/parser.c src/builtins.c src/execute.c src/signals.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) include/shell.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(TARGET) src/shell

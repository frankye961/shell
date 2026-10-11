#include "shell.h"

#include <stdio.h>
#include <string.h>

#define HISTORY_SIZE 1000
#define HISTORY_COMMAND_LENGTH 1024

static char history_array[HISTORY_SIZE][HISTORY_COMMAND_LENGTH];
static size_t history_count;

void history_add(const char *command)
{
    if (strspn(command, " \t\r\n") == strlen(command)) {
        return;
    }

    size_t index = history_count % HISTORY_SIZE;
    size_t length = strcspn(command, "\r\n");
    if (length >= HISTORY_COMMAND_LENGTH) {
        length = HISTORY_COMMAND_LENGTH - 1;
    }
    memcpy(history_array[index], command, length);
    history_array[index][length] = '\0';
    history_count++;
}

void history_print(void)
{
    size_t first = history_count > HISTORY_SIZE ? history_count - HISTORY_SIZE : 0;
    for (size_t i = first; i < history_count; i++) {
        printf("%zu: %s\n", i + 1, history_array[i % HISTORY_SIZE]);
    }
}

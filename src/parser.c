#include "shell.h"

#include <string.h>

int parse_command(char *line, char *args[MAX_ARGS])
{
    int count = 0;
    char *saveptr;

    for (char *word = strtok_r(line, " \t\r\n", &saveptr);
         word != NULL;
         word = strtok_r(NULL, " \t\r\n", &saveptr)) {
        if (count == MAX_ARGS - 1) {
            return -1;
        }
        args[count++] = word;
    }

    args[count] = NULL;
    return count;
}

#include "shell.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int change_directory(char *args[MAX_ARGS], int count)
{
    if (count > 2) {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    const char *directory = count == 2 ? args[1] : getenv("HOME");
    if (directory == NULL || *directory == '\0') {
        fprintf(stderr, "cd: HOME is not set\n");
        return 1;
    }
    if (chdir(directory) == -1) {
        fprintf(stderr, "cd: %s: %s\n", directory, strerror(errno));
        return 1;
    }
    return 0;
}

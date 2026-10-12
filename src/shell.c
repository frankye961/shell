#include "shell.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shell_run(void)
{
    if (install_shell_sigint_handler() == -1) {
        perror("sigaction");
        return 1;
    }

    char *line = NULL;
    size_t capacity = 0;
    int status = 0;

    for (;;) {
        char *args[MAX_ARGS];
        print_prompt();
        if (getline(&line, &capacity, stdin) == -1) {
            if (ferror(stdin)) {
                if (errno == EINTR) {
                    clearerr(stdin);
                    putchar('\n');
                    status = 130;
                    continue;
                }
                perror("getline");
                status = 1;
            }
            break;
        }

        history_add(line);
        int count = parse_command(line, args);
        if (count == -1) {
            fprintf(stderr, "shell: too many arguments (maximum %d)\n", MAX_ARGS - 1);
            status = 1;
        } else if (count == 0) {
            continue;
        } else if (strcmp(args[0], "exit") == 0) {
            break;
        } else if (strcmp(args[0], "cd") == 0) {
            status = change_directory(args, count);
        } else if (strcmp(args[0], "history") == 0) {
            history_print();
        } else {
            status = execute_command(args);
        }
    }

    free(line);
    return status;
}

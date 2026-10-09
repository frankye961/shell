#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_ARGS 64
#define CWD_SIZE 4096
#define HISTORY_SIZE 1000
#define HISTORY_COMMAND_LENGTH 1024

static char history_array[HISTORY_SIZE][HISTORY_COMMAND_LENGTH];
static size_t history_count;

static const char *get_current_user(void)
{
    struct passwd *user = getpwuid(getuid());
    return user != NULL && user->pw_name != NULL ? user->pw_name : "unknown";
}

static void print_prompt(const char *user)
{
    char cwd[CWD_SIZE];
    const char *directory = getcwd(cwd, sizeof(cwd)) != NULL ? cwd : "?";
    printf("%s$%s> ", user, directory);
    fflush(stdout);
}

static void history_add(const char *command)
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

static int parse_command(char *line, char *args[MAX_ARGS])
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


static int execute_command(char *args[MAX_ARGS])
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        execvp(args[0], args);
        perror(args[0]);
        _exit(127);
    }

    int status;
    while (waitpid(pid, &status, 0) == -1) {
        if (errno != EINTR) {
            perror("waitpid");
            return 1;
        }
    }
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }
    return 1;
}

static void history(void)
{
    size_t first = history_count > HISTORY_SIZE ? history_count - HISTORY_SIZE : 0;
    for (size_t i = first; i < history_count; i++) {
        printf("%zu: %s\n", i + 1, history_array[i % HISTORY_SIZE]);
    }
}

int shell_run(void)
{
    const char *user = get_current_user();
    char *line = NULL;
    size_t capacity = 0;
    int status = 0;

    for (;;) {
        char *args[MAX_ARGS];
        print_prompt(user);
        if (getline(&line, &capacity, stdin) == -1) {
            if (ferror(stdin)) {
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
        } else if (strcmp(args[0], "history") == 0) {
            history();
        } else {
            status = execute_command(args);
        }
    }

    free(line);
    return status;
}

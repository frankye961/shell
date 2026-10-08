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

static int change_directory(char *args[MAX_ARGS])
{
    if (args[2] != NULL) {
        fprintf(stderr, "cd: too many arguments\n");
        return 1;
    }

    const char *path = args[1] != NULL ? args[1] : getenv("HOME");
    if (path == NULL) {
        fprintf(stderr, "cd: HOME is not set\n");
        return 1;
    }
    if (chdir(path) == -1) {
        perror("cd");
        return 1;
    }
    return 0;
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

        int count = parse_command(line, args);
        if (count == -1) {
            fprintf(stderr, "shell: too many arguments (maximum %d)\n", MAX_ARGS - 1);
            status = 1;
        } else if (count == 0) {
            continue;
        } else if (strcmp(args[0], "exit") == 0) {
            break;
        } else if (strcmp(args[0], "cd") == 0) {
            status = change_directory(args);
        } else {
            status = execute_command(args);
        }
    }

    free(line);
    return status;
}

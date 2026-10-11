#include "shell.h"

#include <pwd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define CWD_SIZE 4096

void print_prompt(void)
{
    struct passwd *user = getpwuid(getuid());
    const char *name = user != NULL && user->pw_name != NULL ? user->pw_name : "unknown";
    char cwd[CWD_SIZE];
    const char *directory = getcwd(cwd, sizeof(cwd)) != NULL ? cwd : "?";
    printf("%s$%s> ", name, directory);
    fflush(stdout);
}

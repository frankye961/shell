#include "shell.h"

#include <signal.h>
#include <stddef.h>

static void handle_sigint(int signum)
{
    /* Leave output and prompt handling to the main loop. */
    (void)signum;
}

int install_shell_sigint_handler(void)
{
    struct sigaction action = {0};
    action.sa_handler = handle_sigint;
    sigemptyset(&action.sa_mask);
    return sigaction(SIGINT, &action, NULL);
}

int restore_child_sigint(void)
{
    struct sigaction action = {0};
    action.sa_handler = SIG_DFL;
    sigemptyset(&action.sa_mask);
    return sigaction(SIGINT, &action, NULL);
}

#ifndef SHELL_H
#define SHELL_H

#define MAX_ARGS 64

int shell_run(void);
void print_prompt(void);
void history_add(const char *command);
void history_print(void);
int parse_command(char *line, char *args[MAX_ARGS]);
int change_directory(char *args[MAX_ARGS], int count);
int execute_command(char *args[MAX_ARGS]);

#endif

# Simple C shell

A small interactive shell that runs commands found through `PATH`. It accepts
arguments separated by spaces or tabs, waits for each command to finish, and
shows the current directory in the prompt. The built-in commands are `cd`
(with no argument, it uses `HOME`) and `exit`. Ctrl-D also exits.

Input is split on whitespace. Quoting, pipes, redirection, and background jobs
are not supported.

## Build and run

```sh
make
./shell
```

Use `make clean` to remove the executable.

`src/main.c` is the only entry point. `src/shell.c` runs the command loop;
the other source files handle the prompt, history, parsing, built-ins, and
external commands. Shared declarations are in `include/shell.h`.

#include <stdio.h>
#include <string.h>

#include "interpreter.h"

int repl() {
    char buffer[4096] = {0};

    so_interpreter interp;
    so_interpreter_init(&interp);

    printf("> ");
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        if (strcmp(buffer, ".quit\n") == 0)
            break;

        so_interpreter_run(&interp, buffer);

        memset(buffer, 0, sizeof(buffer));
        printf("> ");
    }

    so_interpreter_deinit(&interp);

    return 0;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        return repl();
    }
}

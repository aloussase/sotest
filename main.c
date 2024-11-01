#include <stdio.h>
#include <stdlib.h>
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

int file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "error: could not read file: %s\n", filename);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *contents = malloc(size + 1);
    int nread = fread(contents, sizeof(char), size, fp);

    if (nread < size) {
        fprintf(stderr, "error: read less than %ld bytes\n", size);
        return -1;
    }

    contents[nread] = '\0';

    so_interpreter interp;
    so_interpreter_init(&interp);
    so_interpreter_run(&interp, contents);
    so_interpreter_deinit(&interp);

    free(contents);
    fclose(fp);

    return 0;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        return repl();
    }

    if (argc == 2) {
        char *filename = argv[1];
        return file(filename);
    }

    fprintf(stderr, "usage: sotest [filename]\n");
    return -1;
}

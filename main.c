#include <stdio.h>
#include <string.h>
#include "parser.h"

int repl()
{
    char buffer[4096] = {0};

    printf("> ");

    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        if (strcmp(buffer, ".quit\n") == 0)
            return 0;

        so_parser parser;
        so_parser_init(&parser, buffer);
        so_parser_parse(&parser);

        for (int i = 0; i < parser.ncommands; i++)
        {
            print_ast(parser.commands[i]);
            free_ast(parser.commands[i]);
        }

        so_parser_deinit(&parser);

        memset(buffer, 0, sizeof(buffer));
        printf("> ");
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        return repl();
    }
}

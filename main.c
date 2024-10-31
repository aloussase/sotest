#include <stdio.h>
#include <string.h>
#include "lexer.h"

int repl()
{
    char buffer[4096] = {0};

    printf("> ");

    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        if (strcmp(buffer, ".quit\n") == 0)
            return 0;

        so_lexer lexer;
        so_lexer_init(&lexer, buffer);

        so_token token;
        while ((token = so_lexer_next_token(&lexer)).type != SO_TT_EOF)
        {
            printf("%s\n", token.lexeme);
            so_token_deinit(&token);
        }

        so_token_deinit(&token);

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

#ifndef parser_h__
#define parser_h__

#include "ast.h"
#include "lexer.h"

#define MAX_COMMANDS 100

typedef struct
{
    const char *source;
    so_lexer lexer;
    so_token current;
    int ncommands;
    so_expr *commands[MAX_COMMANDS];
} so_parser;

void so_parser_init(so_parser *, const char *source);

void so_parser_deinit(so_parser *);

void so_parser_parse(so_parser *);

#endif

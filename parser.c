#include "ast.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void so_parse_call(so_parser *);
static void so_parse_load(so_parser *);

static so_token so_parser_advance(so_parser *p)
{
    p->current = so_lexer_next_token(&p->lexer);
    return p->current;
}

static int so_parser_expect(so_parser *p, so_token_type tt)
{
    if (p->current.type != tt)
    {
        fprintf(stderr, "warning: expected a different type of token\n");
        return 0;
    }
    return 1;
}

static void add_command(so_parser *p, so_expr *e)
{
    if (p->ncommands >= MAX_COMMANDS)
    {
        fprintf(stderr, "warning: maximum number of commands reached\n");
        return;
    }
    p->commands[p->ncommands++] = e;
}

void so_parser_init(so_parser *p, const char *source)
{
    p->source = source;
    p->ncommands = 0;
    so_lexer_init(&p->lexer, source);
}

void so_parser_deinit(so_parser *p)
{
    p->source = NULL;
    p->ncommands = 0;
    memset(p->commands, 0, MAX_COMMANDS * sizeof(so_expr *));
    so_lexer_deinit(&p->lexer);
}

void so_parser_parse(so_parser *p)
{
    while (so_parser_advance(p).type != SO_TT_EOF)
    {
        switch (p->current.type)
        {
        case SO_TT_USE:
            so_parse_load(p);
            break;
        case SO_TT_CALL:
            so_parse_call(p);
            break;
        case SO_TT_EOF:
            so_token_deinit(&p->current);
            return;
        default:
            fprintf(stderr, "warning: invalid start of simple expression: %s\n", p->current.lexeme);
            so_token_deinit(&p->current);
        }
    }

    so_token_deinit(&p->current);
}

void so_parse_call(so_parser *p)
{
    so_token_deinit(&p->current);
}

void so_parse_load(so_parser *p)
{
    if (!so_parser_expect(p, SO_TT_USE))
    {
        so_token_deinit(&p->current);
        return;
    }

    so_token use = p->current;
    so_parser_advance(p);

    if (!so_parser_expect(p, SO_TT_STRING))
    {
        so_token_deinit(&use);
        so_token_deinit(&p->current);
        return;
    }

    so_expr *e = create_load_node(p->current.lexeme);
    add_command(p, e);

    so_token_deinit(&use);
    so_token_deinit(&p->current);
}

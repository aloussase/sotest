#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static so_token so_lexer_string(so_lexer *);
static so_token so_lexer_integer(so_lexer *);
static so_token so_lexer_bareword(so_lexer *);

void so_token_deinit(so_token *t)
{
    free(t->lexeme);
    t->lexeme = NULL;
    t->type = SO_TT_INVALID;
}

static char so_lexer_advance(so_lexer *l)
{
    if (l->current >= l->source_length)
        return '\0';
    return l->source[l->current++];
}

static char so_lexer_peek(so_lexer *l)
{
    if (l->current >= l->source_length)
        return '\0';
    return l->source[l->current];
}

static int so_lexer_eof(so_lexer *l)
{
    return so_lexer_peek(l) == '\0';
}

void so_lexer_init(so_lexer *l, const char *source)
{
    l->current = 0;
    l->start = 0;
    l->source_length = strlen(source);
    l->source = source;
}

void so_lexer_deinit(so_lexer *l)
{
    l->current = 0;
    l->start = 0;
    l->source_length = 0;
    l->source = NULL;
}

so_token so_lexer_next_token(so_lexer *l)
{
    char c = so_lexer_advance(l);

    if (c == '\0')
        return (so_token){.lexeme = strdup("eof"), .type = SO_TT_EOF};

    l->start = l->current;

    switch (c)
    {
    case '"':
        return so_lexer_string(l);
    case '\n':
    case ' ':
        return so_lexer_next_token(l);
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return so_lexer_integer(l);
    default:
        return so_lexer_bareword(l);
    }
}

so_token so_lexer_string(so_lexer *l)
{
    while (!so_lexer_eof(l) && so_lexer_peek(l) != '"')
        so_lexer_advance(l);

    if (so_lexer_peek(l) != '"')
    {
        fprintf(stderr, "Unterminated string literal at %d\n", l->current);
        return (so_token){.lexeme = strdup("invalid"), .type = SO_TT_INVALID};
    }

    char *lexeme = strndup(
        &l->source[l->start],
        l->current - l->start);

    so_lexer_advance(l);

    return (so_token){.lexeme = lexeme, .type = SO_TT_STRING};
}

so_token so_lexer_integer(so_lexer *l)
{
}

so_token so_lexer_bareword(so_lexer *l)
{
}

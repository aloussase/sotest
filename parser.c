#include "ast.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void so_parse_call(so_parser *);

static void so_parse_load(so_parser *);

static so_expr *so_parser_parse_simple_expression(so_parser *);

static so_token so_parser_advance(so_parser *p) {
    p->current = so_lexer_next_token(&p->lexer);
    return p->current;
}

static int so_parser_expect(so_parser *p, so_token_type tt) {
    if (p->current.type != tt) {
        char buffer[20] = {0};
        so_token_type_to_string(tt, buffer, sizeof(buffer));
        fprintf(stderr, "warning: expected a token of type: %s\n", buffer);
        return 0;
    }
    return 1;
}

static void add_command(so_parser *p, so_expr *e) {
    if (p->ncommands >= MAX_COMMANDS) {
        fprintf(stderr, "warning: maximum number of commands reached\n");
        return;
    }
    p->commands[p->ncommands++] = e;
}

void so_parser_init(so_parser *p, const char *source) {
    p->source = source;
    p->ncommands = 0;
    so_lexer_init(&p->lexer, source);
}

void so_parser_deinit(so_parser *p) {
    p->source = NULL;
    p->ncommands = 0;
    memset(p->commands, 0, MAX_COMMANDS * sizeof(so_expr *));
    so_lexer_deinit(&p->lexer);
}

void so_parser_parse(so_parser *p) {
    while (so_parser_advance(p).type != SO_TT_EOF) {
        switch (p->current.type) {
            case SO_TT_USE:
                so_parse_load(p);
                break;
            case SO_TT_CALL:
                so_parse_call(p);
                break;
            case SO_TT_EOL: // Happens on an empty line
                so_token_deinit(&p->current);
                break;
            case SO_TT_EOF: // Happens at end of input
                so_token_deinit(&p->current);
                return;
            default:
                fprintf(stderr, "warning: invalid start of expression, expected use or call: %s\n", p->current.lexeme);
                so_token_deinit(&p->current);
        }
    }

    so_token_deinit(&p->current);
}

void so_parse_call(so_parser *p) {
    if (!so_parser_expect(p, SO_TT_CALL)) {
        so_token_deinit(&p->current);
        return;
    }

    so_token_deinit(&p->current);
    so_parser_advance(p);

    if (!so_parser_expect(p, SO_TT_BARE)) {
        so_token_deinit(&p->current);
        return;
    }

    so_token name = p->current;
    so_expr *args[MAX_CALL_ARGS];
    int nargs = 0;

    while (so_parser_advance(p).type != SO_TT_EOF && p->current.type != SO_TT_EOL && nargs < MAX_CALL_ARGS) {
        so_expr *e = so_parser_parse_simple_expression(p);
        if (e == NULL) {
            char type[10] = {0};
            so_token_type_to_string(p->current.type, type, sizeof(type));
            fprintf(
                stderr,
                "warning: expected either a string or a number, but got %s\n",
                type);
            so_token_deinit(&p->current);
            continue;
        }

        args[nargs++] = e;
    }

    if (!so_parser_expect(p, SO_TT_EOL)) {
        fprintf(stderr, "warning: expected an end of line\n");
        so_token_deinit(&p->current);
        return;
    }

    so_expr *e = create_call_node(name.lexeme, nargs, args);
    add_command(p, e);

    so_token_deinit(&p->current);
    so_token_deinit(&name);
}

void so_parse_load(so_parser *p) {
    if (!so_parser_expect(p, SO_TT_USE)) {
        so_token_deinit(&p->current);
        return;
    }

    so_token use = p->current;
    so_parser_advance(p);

    if (!so_parser_expect(p, SO_TT_STRING)) {
        so_token_deinit(&use);
        so_token_deinit(&p->current);
        return;
    }

    so_token name = p->current;
    so_parser_advance(p);

    if (!so_parser_expect(p, SO_TT_EOL)) {
        so_token_deinit(&use);
        so_token_deinit(&name);
        so_token_deinit(&p->current);
        return;
    }

    so_expr *e = create_load_node(name.lexeme);
    add_command(p, e);

    so_token_deinit(&use);
    so_token_deinit(&name);
    so_token_deinit(&p->current);
}

so_expr *so_parser_parse_simple_expression(so_parser *p) {
    so_expr *e = NULL;

    if (p->current.type == SO_TT_STRING) {
        e = create_string_node(p->current.lexeme);
        so_token_deinit(&p->current);
    }

    if (p->current.type == SO_TT_INTEGER) {
        int n = atoi(p->current.lexeme);
        e = create_number_node(n);
        so_token_deinit(&p->current);
    }

    return e;
}

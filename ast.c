#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

so_expr *create_string_node(const char *s)
{
    so_expr *e = malloc(sizeof(so_expr));
    e->tag = SO_EXPR_STRING;
    e->as.string = strdup(s);
    return e;
}

so_expr *create_number_node(int n)
{
    so_expr *e = malloc(sizeof(so_expr));
    e->tag = SO_EXPR_INT;
    e->as.number = n;
    return e;
}

so_expr *create_call_node(const char *name, int nargs, so_expr *args[])
{
    so_expr *e = malloc(sizeof(so_expr));
    e->tag = SO_EXPR_CALL;
    e->as.call = (so_expr_call){.name = strdup(name), .nargs = nargs};
    for (int i = 0; i < nargs; i++)
        e->as.call.args[i] = args[i];
    return e;
}

so_expr *create_load_node(const char *name)
{
    so_expr *e = malloc(sizeof(so_expr));
    e->tag = SO_EXPR_LOAD;
    e->as.load = (so_expr_load){.name = strdup(name)};
    return e;
}

void print_ast(so_expr *e)
{
    switch (e->tag)
    {
    case SO_EXPR_INT:
        printf("%d\n", e->as.number);
        break;
    case SO_EXPR_STRING:
        printf("%s\n", e->as.string);
        break;
    case SO_EXPR_LOAD:
        printf("USE %s\n", e->as.load.name);
        break;
    case SO_EXPR_CALL:
        printf("CALL %s (%d)\n", e->as.call.name, e->as.call.nargs);
        break;
    }
}

void free_ast(so_expr *e)
{
    switch (e->tag)
    {
    case SO_EXPR_INT:
        break;
    case SO_EXPR_STRING:
        free(e->as.string);
        break;
    case SO_EXPR_LOAD:
        free(e->as.load.name);
        break;
    }
    free(e);
}

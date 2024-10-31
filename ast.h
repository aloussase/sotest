#ifndef ast_h__
#define ast_h__

#define MAX_CALL_ARGS 100

typedef struct so_expr so_expr;

typedef struct
{
    char *name;
    int nargs;
    so_expr *args[MAX_CALL_ARGS];
} so_expr_call;

typedef struct
{
    char *name;
} so_expr_load;

struct so_expr
{
    enum
    {
        SO_EXPR_CALL,
        SO_EXPR_LOAD,
        SO_EXPR_INT,
        SO_EXPR_STRING,
    } tag;
    union
    {
        so_expr_call call;
        so_expr_load load;
        int number;
        char *string;
    } as;
};

so_expr *create_string_node(const char *s);
so_expr *create_number_node(int n);
so_expr *create_call_node(const char *name, int nargs, so_expr *args[]);
so_expr *create_load_node(const char *name);

void print_ast(so_expr *);
void free_ast(so_expr *);

#endif

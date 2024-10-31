#ifndef lexer_h__
#define lexer_h__

typedef enum
{
    SO_TT_USE,
    SO_TT_CALL,
    SO_TT_BARE,
    SO_TT_STRING,
    SO_TT_INTEGER,
    SO_TT_EOF,
    SO_TT_INVALID,
} so_token_type;

void so_token_type_to_string(so_token_type tt, char buffer[], int size);

typedef struct
{
    so_token_type type;
    char *lexeme;
} so_token;

/// @brief Deinitialize resources for the provided token.
void so_token_deinit(so_token *);

typedef struct
{
    int start;
    int current;
    int source_length;
    const char *source;
} so_lexer;

/// @brief Initialize the lexer.
/// @param
void so_lexer_init(so_lexer *, const char *source);

/// @brief Deinitialize the lexer, after which it will be unusable.
/// @param
void so_lexer_deinit(so_lexer *);

/// @brief  Lex the next token in the input.
/// @param
/// @return
so_token so_lexer_next_token(so_lexer *);

#endif

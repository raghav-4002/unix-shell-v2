#ifndef TOKENS_H_
#define TOKENS_H_


#define MAX_LEXEME_SIZE 128
#define MAX_TOK_COUNT   128


typedef enum Token_type
{
    NAME,                   /* Generic type for commands,
                               arguments and files */

    PIPE, BG_OPERATOR,       /* `|` and `&` */
    LOGIC_AND, LOGIC_OR,     /* `||` and `&&` */
    SEMICOLON,               /* `;` */

    LEFT_REDIR,              /* `<` */
    RIGHT_REDIR,             /* `>` */
    DOUBLE_RIGHT_REDIR,      /* `>>` */

    LEFT_PAREN, RIGHT_PAREN, /* `(` and `)` */

    NIL,
} Token_type;


typedef struct Token
{
    Token_type type;
    char *lexeme;
} Token;


#endif

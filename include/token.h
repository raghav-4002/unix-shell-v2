#ifndef TOKENS_H_
#define TOKENS_H_


typedef enum Token_type
{
    CMD, ARG,                /* Command and its arguments */

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
    char *arg;
} Token;


#endif

#ifndef LEXER_HELPER_
#define LEXER_HELPER_


#include <stdlib.h>
#include <stdbool.h>

#include "token.h"


struct Lexer_obj
{
    Token *tokens;    /* Pointer to the current generated tokens */
    size_t tok_count; /* Size of the `tokens` array */

    char  *source;    /* string to be tokenized */
    size_t start;     /* 1st char of the lexeme */
    size_t current;   /* current char of string being considered */
};


void lex_init_obj(struct Lexer_obj *lexer_obj, char *input);
bool lex_current_at_end(struct Lexer_obj *lexer_obj);
char lex_advance_current(struct Lexer_obj *lexer_obj);
bool lex_match(struct Lexer_obj *lexer_obj, char expected);
void lex_init_token(Token *token, Token_type type);
char *create_substring(char *string, size_t start, size_t end);
void destroy_lex_data(struct Lexer_obj *lexer_obj);


#endif // LEXER_HELPER_

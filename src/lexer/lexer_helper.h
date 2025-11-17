#ifndef LEXER_HELPER_
#define LEXER_HELPER_

#include <stdlib.h>
#include <stdbool.h>

#include "token.h"


struct Lexer_obj
{
    Token *tokens;
    size_t arr_size; /* size of `tokens` array */

    char  *source;   /* string to be tokenized */
    size_t start;    /* 1st char of the lexeme */
    size_t current;  /* current char of string being considered */
};


void init_parameters(struct Lexer_obj *lexer_obj, char *input);
bool current_is_at_end(struct Lexer_obj *lexer_obj);
char advance_current(struct Lexer_obj *lexer_obj);
bool match(struct Lexer_obj *lexer_obj, char expected);
void init_token(Token *token, Token_type type);
char *create_substring(char *string, size_t start, size_t end);
void free_tokens_on_error(struct Lexer_obj *lexer_obj);


#endif // LEXER_HELPER_

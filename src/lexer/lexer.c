#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lexer.h"
#include "token.h"
#include "lexer_helper.h"


static int add_arg(struct Lexer_obj *lexer_obj);
static int add_token(struct Lexer_obj *lexer_obj, Token_type type);
static int command(struct Lexer_obj *lexer_obj);
static int scan_token(struct Lexer_obj *lexer_obj);


/*
 * @brief : Adds literal value of argument into token
            of type `COMMAND`.
 * @param : A pointer to `struct Parameters`.
 * @return: `0` on success; `-1` on failure.
 */
static int
add_arg(struct Lexer_obj *lexer_obj)
{
    char  *string    = lexer_obj->source;
    size_t start     = lexer_obj->start;
    size_t end       = lexer_obj->current;
    size_t cur_index = lexer_obj->arr_size - 1;

    lexer_obj->tokens[cur_index].arg = create_substring(string, start, end);

    if (!lexer_obj->tokens[cur_index].arg) return -1;

    return 0;
}


/*
 * @brief : Adds a token into `tokens` array.
 * @param : A pointer to `struct Parameters`.
            `Token_type` enum Member.
 * @return: `0` on success; `-1` on failure.
 */
static int
add_token(struct Lexer_obj *lexer_obj, Token_type type)
{
    Token *tokens = lexer_obj->tokens;

    /* Add space for one more token */
    lexer_obj->arr_size += 1;
    size_t arr_size       = lexer_obj->arr_size;
    size_t cur_index      = arr_size - 1;

    /* Resize array */
    tokens = realloc(tokens, arr_size * sizeof(*tokens));

    if (!tokens) {
        lexer_obj->arr_size -= 1;  /* reset array size */
        return -1;
    }

    Token *cur_token = &tokens[cur_index];
    init_token(cur_token, type);

    lexer_obj->tokens = tokens;

    if (type == COMMAND) {
        if (add_arg(lexer_obj) == -1) return -1;
    }

    return 0;
}


/*
 * @brief : Tokenizes a lexeme of type `COMMAND`.
 * @param : A pointer to `struct Parameters`.
 * @return: `0` on success; `-1` on failure.
 */
static int
command(struct Lexer_obj *lexer_obj)
{
    /* Move current ahead, until any of the recognised lexeme is not found */
    while (!match(lexer_obj, ' ') && !match(lexer_obj, '\n')
           && !match(lexer_obj, '\t') && !match(lexer_obj, '\0')
           && !match(lexer_obj, ';') && !match(lexer_obj, '&')
           && !match(lexer_obj, '|')) {

        advance_current(lexer_obj);
    }

    int err_return = add_token(lexer_obj, COMMAND);

    return err_return;
}


/*
 * @brief : Recognises the current lexeme.
 * @param : A pointer to `struct Parameters`.
 * @return: `0` on success; `-1` on failure
 */
static int
scan_token(struct Lexer_obj *lexer_obj)
{
    char c = advance_current(lexer_obj);

    int err_return = 0;

    switch (c) {
        /* Single character tokens */
        case ';':
            err_return = add_token(lexer_obj, SEMICOLON);
            break;

        case ' ': case '\n': case '\t':
            break;

        /* Double/single character tokens */
        case '|':
            if (match(lexer_obj, '|')) {
                advance_current(lexer_obj);
                err_return = add_token(lexer_obj, LOGIC_OR);
            }
            else {
                err_return = add_token(lexer_obj, PIPE);
            }
            break;

        case '&':
            if (match(lexer_obj, '&')) {
                advance_current(lexer_obj);
                err_return = add_token(lexer_obj, LOGIC_AND);
            }
            else {
                err_return = add_token(lexer_obj, BG_OPERATOR);
            }
            break;

        /* Command tokens */
        default:
            err_return = command(lexer_obj);
            break;

    }

    if (err_return == -1) return -1;

    return 0;
}


/*
 * @brief : Tokenizes an input string; stops when encounters `\0`
 * @param : An input string
 * @return: An array of type `Token`; last element is type `NIL`
 *          `NULL` on failure
 */
Token *
tokenize(char *input)
{
    /*
     * A pointer to structure `Parameters`
     *
     * Members of `struct Parameters`

     * `Token *tokens`  : a pointer to an array of tokens.
     * `size_t arr_size`: represents size of the tokens array.

     * `char *source`   : source string to be tokenized.
     * `size_t start`   : an index pointing to the head of the 
                          lexeme being considered.
     * `size_t current` : an index pointing to the current character
                          being considered.
     */
    struct Lexer_obj *lexer_obj = malloc(sizeof(*lexer_obj));

    if (lexer_obj == NULL) return NULL;

    init_parameters(lexer_obj, input);

    int err_return = 0;

    while (!current_is_at_end(lexer_obj)) {
        /* Move to the next lexeme */
        lexer_obj->start = lexer_obj->current;
        err_return        = scan_token(lexer_obj);

        if (err_return == -1) {
            free_tokens_on_error(lexer_obj);
            return NULL;
        }
    }

    /* Add `NIL` as last token */
    err_return = add_token(lexer_obj, NIL);

    if (err_return == -1) {
        free_tokens_on_error(lexer_obj);
        return NULL;
    }

    return lexer_obj->tokens;
}

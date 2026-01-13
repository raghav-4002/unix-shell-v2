#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"
#include "token.h"
#include "lexer_helper.h"


static int add_word(Lexer_obj *lexer_obj);
static int add_token(Lexer_obj *lexer_obj, Token_type type);
static int handle_name(Lexer_obj *lexer_obj);
static int scan_token(Lexer_obj *lexer_obj);


/* Adds the literal lexeme into the token of type `NAME` */
static int
add_word(Lexer_obj *lexer_obj)
{
    const char *string = lexer_obj->source;
    int start       = lexer_obj->start;
    int end         = lexer_obj->current;
    int cur_index   = lexer_obj->tok_count - 1;

    char *substring = create_substring(string, start, end);
    if (substring == NULL) {
        return -1;
    }

    /* Add substring as the lexeme for WORD */
    lexer_obj->tokens[cur_index].lexeme = substring;
    return 0;
}


/* Add token to `tokens` array of `Lexer_obj` */
static int
add_token(Lexer_obj *lexer_obj, Token_type type)
{
    if (lexer_obj->tok_count >= MAX_TOK_COUNT) {
        fprintf(stderr, "Max token count exceeded\n");
        return -1;
    }

    /* Expand the tokens array to add space for new token */
    if (expand_tok_array(lexer_obj) == -1) {
        return -1;
    }

    Token *tokens     = lexer_obj->tokens;
    int curr_index = lexer_obj->tok_count - 1;

    init_token(&tokens[curr_index], type);

    if (type == NAME) {
        /* Add lexeme value for token of type `NAME` */
        return add_word(lexer_obj);
    }

    return 0;
}


/* For tokens of type `NAME`, identify the lexeme and add token to array  */
static int
handle_name(Lexer_obj *lexer_obj)
{
    char curr_ch = GET_CURR_CHAR(lexer_obj);

    /* Move current ahead, until any of the recognised lexeme is not found */
    while (curr_ch != ' ' && curr_ch != '\t' && curr_ch != '\0'
        && curr_ch != ';' && curr_ch != '&'  && curr_ch != '|'
        && curr_ch != '\n') {

        advance_current(lexer_obj);
        curr_ch = GET_CURR_CHAR(lexer_obj);

        if (GET_CURR_LEXEME_SIZE(lexer_obj) > MAX_LEXEME_SIZE) {
            fprintf(stderr, "Maximum allowed word size exceeded\n");
            return -1;
        }
    }

    return add_token(lexer_obj, NAME);
}


/* Identify lexeme starting from `start` to `current` in the string */
static int
scan_token(Lexer_obj *lexer_obj)
{
    /* Get the first character of current lexeme */
    const char c   = advance_current(lexer_obj);

    switch (c) {
        case ' ': case '\t':
            /* Skip whitespaces */
            return 0;

        case ';':
            return add_token(lexer_obj, SEMICLN);

        case '\\':  /* Match `\` */
            return add_token(lexer_obj, BACKSLSH);

        case '|':   /* `|` and `||` */
            if ('|' == GET_CURR_CHAR(lexer_obj)) {
                advance_current(lexer_obj);
                return add_token(lexer_obj, DOUBLE_PIPE);
            }
            else {
                return add_token(lexer_obj, PIPE);
            }

        case '&':   /* `&` and `&&` */
            if ('&' == GET_CURR_CHAR(lexer_obj)) {
                advance_current(lexer_obj);
                return add_token(lexer_obj, DOUBLE_AMPRSND);
            }
            else {
                return add_token(lexer_obj, AMPRSND);
            }

        /* ==== Word tokens ==== */
        default:
            return handle_name(lexer_obj);
    }
}


Token *
tokenize(const char *input)
{
    Lexer_obj *lexer_obj = get_lexer_obj(input);
    if (lexer_obj == NULL) {
        return NULL;
    }

    /* Main tokenizer loop */
    while (!is_current_at_end(lexer_obj)) {
        /* Move to the next lexeme */
        lexer_obj->start = lexer_obj->current;

        if (scan_token(lexer_obj) == -1) {
            destroy_lexer_obj(lexer_obj);
            return NULL;
        }
    }

    /* Add `NIL` as last token */
    if (add_token(lexer_obj, NIL) == -1) {
        destroy_lexer_obj(lexer_obj);
        return NULL;
    }

    /* Only need to return `tokens` array */
    Token *tokens = lexer_obj->tokens;
    free(lexer_obj);
    return tokens;
}

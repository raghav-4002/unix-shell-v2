#include "utils.h"
#include "token.h"


void
free_tokens(Token *tokens)
{
    for (int i = 0; tokens[i].type != NIL; i++) {
        if (tokens[i].type == NAME) {
            free(tokens[i].lexeme);
        }
    }

    free(tokens);
}

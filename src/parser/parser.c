#include <stddef.h>

#include "parser.h"
#include "ast.h"
#include "instance.h"
#include "token.h"


static Ast_node *parse_condition(Token *tokens, int *current);
static Instance *parse_instance(Token *tokens, int *current);


static Ast_node *
parse_condition(Token *tokens, int *current)
{
    
}


static Instance *
parse_instance(Token *tokens, int *current)
{
    /* `instance` is the head of the `Instance` linked list */
    Instance *instance = get_instance_obj();
    if (instance == NULL) {
        return NULL;
    }

    Instance *ptr      = instance;
    Ast_node *ast_root = parse_condition(tokens, current);
    if (ast_root == NULL) {
        destroy_instance(instance);
        return NULL;
    }

    ptr->ast_root = ast_root;

    while (tokens[*current].type == AMPRSND
        || tokens[*current].type == SEMICLN) {

        if (tokens[*current].type == AMPRSND) {
            ptr->context = BACKGROUND;
        }

        *current += 1;

        if (tokens[*current].type != NIL) {
            Instance *temp = get_instance_obj();
            if (temp == NULL) {
                destroy_instance(instance);
                return NULL;
            }

            ptr->next = temp;
            ptr       = temp;
            ast_root  = parse_condition(tokens, current);
            if (ast_root == NULL) {
                destroy_instance(instance);
                return NULL;
            }
            ptr->ast_root = ast_root;
        }
    }

    return instance;
}


Instance *
parse_tokens(Token *tokens)
{
    int current = 0;
    Instance *instance = parse_instance(tokens, &current);
    if (instance == NULL) {
        return NULL;
    }

    /* If not all tokens are parsed */
    if (tokens[current].type != NIL) {
        destroy_instance(instance);
        return NULL;
    }

    return instance;
}
#include "list.h"
#include "ast.h"

#include <stdbool.h>
#include <stdlib.h>


void
destroy_list(List_node *head)
{
    while (head != NULL) {
        if (head->ast_root != NULL) {
            destroy_ast(head->ast_root);
        }
        List_node *temp = head;
        head = head->next;
        free(temp);
    }
}


List_node *
get_list_node(void)
{
    List_node *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->ast_root       = NULL;
    node->is_foreground  = true; /* Default context */
    node->next           = NULL;
    return node;
}
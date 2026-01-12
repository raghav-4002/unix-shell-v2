#include "exec_unit.h"
#include "ast.h"

#include <stddef.h>
#include <stdlib.h>


void
destroy_exec_unit_list(Execution_unit *head)
{
    while (head != NULL) {
        if (head->ast_root != NULL) {
            destroy_ast(head->ast_root);
        }
        Execution_unit *temp = head;
        head = head->next;
        free(temp);
    }
}


Execution_unit *
get_exec_unit_node(void)
{
    Execution_unit *node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }

    node->ast_root = NULL;
    node->context  = FOREGROUND; /* Default context */
    node->next     = NULL;
    return node;
}
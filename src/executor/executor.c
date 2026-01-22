#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "executor.h"
#include "ast.h"
#include "list.h"
#include "pipeline.h"
#include "stack.h"


static void traverse_ast(Ast_node *ast_root, bool in_subshell, bool in_foreground);
static bool can_execute_right_pipeline(Ast_node *node);


static void
update_node_status(Ast_node *node)
{
    assert(node->type == AND || node->type == OR);

    if (node->type == AND) {
        if (node->left->return_status != 0) {
            node->return_status = node->left->return_status;
        }
        else if (node->right->return_status != 0) {
            node->return_status = node->right->return_status;
        }
        else {
            node->return_status = 0;
        }
    }

    else if (node->type == OR) {
        if (node->left->return_status == 0) {
            node->return_status = 0;
        }
        else if (node->right->return_status == 0) {
            node->return_status = 0;
        }
        else {
            node->return_status = node->right->return_status;
        }
    }
}


static bool
can_execute_right_pipeline(Ast_node *node)
{
    /* failed and (anything) = failed */
    if (node->type == AND && node->left->return_status != 0) {
        node->return_status = node->left->return_status;
        return false;
    }

    /* success or (anything) = success */
    if (node->type == OR  && node->left->return_status == 0) {
        node->return_status = node->left->return_status;
        return false;
    }

    return true;
}


static void
traverse_ast(Ast_node *ast_root, bool in_subshell, bool in_foreground)
{
    Stack *stack = NULL;
    Ast_node *node = ast_root;

    while (node->type != PIPELINE) {
        if (push_node_into_stack(node, &stack) == -1) {
            //TODO: error
        }
        node = node->left;
    }

    assert(node->type == PIPELINE);
    int return_status = launch_pipeline(node->pipeline, false, true);
    node->return_status = return_status;

    while (stack != NULL) {
        node = pop_node_from_stack(&stack);
        assert(node->type == AND || node->type == OR);

        if (can_execute_right_pipeline(node)) {
            return_status = launch_pipeline(node->right->pipeline, false, true);
            node->right->return_status = return_status;
            update_node_status(node);
        }
    }
}


void
execute(List_node *head)
{
    for (List_node *node = head; node != NULL; node = node->next) {
        /* For now, all list nodes are considered as foreground */
        traverse_ast(node->ast_root, false, true);
    }
}
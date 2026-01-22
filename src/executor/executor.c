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
    // TODO: launch_pipeline(node->pipeline, in_subshell, in_foreground)
    // update node return status

    while (stack != NULL) {
        node = pop_node_from_stack(&stack);
        assert(node->type == AND || node->type == OR);
        if (can_execute_right_pipeline(node)) {
            // TODO: launch_pipeline(node->right->pipeline, in_subshell, in_foreground)
            // update node->right->return_status
            // update node->return_status
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
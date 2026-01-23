#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "executor.h"
#include "ast.h"
#include "list.h"
#include "pipeline.h"
#include "stack.h"


static void update_node_status(Ast_node *node);
static void traverse_ast(Ast_node *ast_root, bool in_subshell, bool in_foreground);
static bool can_execute_right_pipeline(Ast_node *node);


/* For node of type `AND` and `OR`, update its return
   status by determining the return status of both its
   children. Both children must have a valid return status. */
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


/* For node of type `AND` or `OR`, determine if right child
   can be executed. If not, update the return status of the node */
static bool
can_execute_right_pipeline(Ast_node *node)
{
    assert(node->type == AND || node->type == OR);

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
    /*
        An AST will always have the structure like following:

                                (condition)
                                /         \
                               /           \
                            (condition)   [Pipeline]
                            /         \
                           /           \
                    (condition)   [Pipeline]
                    /         \
                   /           \
                [Pipeline]    [Pipeline]
    */

    Stack *stack   = NULL;
    Ast_node *node = ast_root;

    while (node->type != PIPELINE) {
        /* Push nodes into stack and go to left child until 
           node of type `PIPELINE` is not found */

        if (push_node_into_stack(node, &stack) == -1) {
            destroy_stack(&stack);
            return;
        }
        node = node->left;
    }

    assert(node->type == PIPELINE);
    int return_status   = launch_pipeline(node->pipeline, false, true);
    node->return_status = return_status;

    while (stack != NULL) {
        /* Start emptying the stack and execute the right
           child depending on the type of node */

        node = pop_node_from_stack(&stack);
        assert(node->type == AND || node->type == OR);

        /* Current node type will always be `AND` or `OR` and its
           right child will always be of type `PIPELINE` */
        if (can_execute_right_pipeline(node)) {
            node->right->return_status = 
                launch_pipeline(node->right->pipeline, false, true);
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
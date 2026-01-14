#include <stdio.h>

#include "executor.h"
#include "ast.h"
#include "exec_unit.h"


int
find_return_stat_of_root(Ast_node *root)
{

}


void
traverse_and_eval_ast(Ast_node *root)
{
    if (root->type == JOB) {
        int return_status = launch_job(root->job);
        root->return_status = return_status;
        return;
    }

    traverse_and_eval_ast(root->left);

    if ((root->type == AND && root->left->return_status == 0)
     || (root->type == OR  && root->left->return_status != 0)) {
    
        traverse_and_eval_ast(root->right);
        root->return_status = find_return_stat_of_root(root);
    }
}


void
execute_unit(Execution_unit *unit)
{
    //TODO: Check for foreground and background goes here
    traverse_and_eval_ast(unit->ast_root);
}


void
execute(Execution_unit *head)
{
    Execution_unit *ptr = head;
    while (ptr != NULL) {
        execute_unit(ptr);
        ptr = ptr->next;
    }
}
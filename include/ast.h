/*
    AST - Abstrct Syntax Tree is a representation of
    command execution flow in tree form. The ast takes
    conditional (`&&` and `||`).

    Each node of the AST is of type: AND, OR or JOB.
    Each node also holds a pointer to its left and right
    child. Also, an extra field, `return_status` is
    present to dictate execution of next jobs.

    For example, the tree for

        `job1 || job2 && job3`
    
    will look like:

            (&&)
            /  \
           /    \
        (||)    (job3)
        /  \
       /    \
    (job1)  (job2)

    Execution will start from `job1`. Based on its
    success, `job2` will be launched. And similarly
    `job3` will be executed.
*/


#ifndef AST_H_
#define AST_H_


#include "job.h"


typedef enum Node_type
{
    AND, OR,  /* `&&` and `||` */
    JOB,
} Node_type;


typedef struct Ast_node
{
    Node_type type;
    int       return_status;
    
    /* Left and right children */
    struct Ast_node *left;
    struct Ast_node *right;
    
    /* Only for node of type `JOB`.
       `NULL` for rest */
    Job_obj *job;
} Ast_node;


Ast_node *create_ast_node(Node_type type, Job_obj *job);
void destroy_ast(Ast_node *ast_root);


#endif // AST_H_

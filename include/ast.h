#ifndef AST_H_
#define AST_H_


typedef enum Node_type
{
    /* Only pipeline is defined as for now */
    PIPELINE,
} Node_type;

typedef struct Ast_node
{
    Node_type type;

    /* 
        Index for the pipeline in the pipeline
        table. Only defined for node of type
        `Pipeline`
    */
    int pipeline_index;
    
    /* Left and right children */
    struct Ast_node *right;
    struct Ast_node *left;
} Ast_node;


#endif // AST_H_

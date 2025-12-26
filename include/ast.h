#ifndef AST_H_
#define AST_H_


typedef enum Node_type
{
    PIPELINE,
    AND, OR,
} Node_type;

typedef struct Ast_node
{
    Node_type type;

    /* 
        Index for the pipeline in the pipeline
        table. Only defined for node of type
        `PIPELINE`. For other types its `-1`
    */
    int pipeline_index;
    
    /* Left and right children */
    struct Ast_node *right;
    struct Ast_node *left;
} Ast_node;


Ast_node *create_ast_node(Node_type type, int pipeline_index);
void destroy_ast(Ast_node *ast_root);


#endif // AST_H_

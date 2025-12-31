#ifndef AST_H_
#define AST_H_


typedef enum Node_type
{
    AND, OR,         /* `&&` and `||` */

    NEXT, BG_NEXT,  /* `;` and `&` */

    PIPELINE,       /* for pipelines */
} Node_type;

typedef struct Ast_node
{
    Node_type type;
    int return_val;
    
    /* Left and right children */
    struct Ast_node *left;
    struct Ast_node *right;
    
    /* Index in the pipeline table
       will only be used by nodes of
       type `PIPELINE`. For rest, its `-1` */
    int pipeline_index;
} Ast_node;


Ast_node *create_ast_node(Node_type type, int pipeline_index);
void destroy_ast(Ast_node *ast_root);


#endif // AST_H_

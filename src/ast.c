#include <stdlib.h>
#include <stdio.h>

#include "ast.h"


Ast_node *
create_ast_node(Node_type type, int pipeline_index)
{
    Ast_node *ast_node = malloc(sizeof(*ast_node));
    if (ast_node == NULL) {
        perror("Ast node creation");
        return NULL;
    }

    ast_node->type           = type;
    ast_node->pipeline_index = pipeline_index;
    ast_node->left           = NULL;
    ast_node->right          = NULL;

    return ast_node;
}


void
destroy_ast(Ast_node *ast_root)
{
    /* Free each node of ast in postorder manner */
    if (ast_root == NULL) {
        return;
    }

    /* We won't free the pipeline associated with a node */
    free(ast_root->left);
    free(ast_root->right);
    free(ast_root);
}

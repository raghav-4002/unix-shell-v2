#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "ast.h"
#include "token.h"
#include "pipeline.h"
#include "command.h"
#include "parser_helper.h"


static Ast_node *
parse_condition(Token *tokens, int *current, Pipeline_table *pipeline_table)
{
    int pipeline_index = parse_pipeline(tokens, current, pipeline_table);
    if (pipeline_index == -1) {
        return NULL;
    }
    Ast_node *left = create_ast_node(PIPELINE, pipeline_index);
    if (left == NULL) {
        return NULL;
    }

    while (tokens[*current].type == LOGIC_AND
        || tokens[*current].type == LOGIC_OR) {

        Node_type root_type = tokens[*current].type == LOGIC_AND
                            ? AND : OR;

        *current += 1;
        pipeline_index = parse_pipeline(tokens, current, pipeline_table);
        if (pipeline_index == -1) {
            destroy_ast(left);
            return NULL;
        }
        Ast_node *right = create_ast_node(PIPELINE, pipeline_index);
        if (right == NULL) {
            destroy_ast(left);
            return NULL;
        }

        Ast_node *root = create_ast_node(root_type, -1);
        if (root == NULL) {
            destroy_ast(right);
            destroy_ast(left);
            return NULL;
        }
        root->left  = left;
        root->right = right;
        left = root;
    }

    return left;
}


static Parser_obj *
parse_sequence(Token *tokens, int *current)
{
    Pipeline_table *pipeline_table = get_pipeline_table();
    if (pipeline_table == NULL) {
        return NULL;
    }

    Ast_node *left = parse_condition(tokens, current, pipeline_table);
    if (left == NULL) {
        destroy_pipeline_table(pipeline_table);
        return NULL;
    }

    while (tokens[*current].type == SEMICOLON) {
        *current += 1;

        Ast_node *right = parse_condition(tokens, current, pipeline_table);
        if (right == NULL) {
            destroy_ast(left);
            destroy_pipeline_table(pipeline_table);
            return NULL;
        }

        Ast_node *root = create_ast_node(NEXT, -1);
        if (root == NULL) {
            destroy_ast(right);
            destroy_ast(left);
            destroy_pipeline_table(pipeline_table);
            return NULL;
        }

        root->left  = left;
        root->right = right;
        left = root;
    }    

    Parser_obj *parser_obj = create_parser_obj(left, pipeline_table);
    if (parser_obj == NULL) {
        destroy_ast(left);
        destroy_pipeline_table(pipeline_table);
        return NULL;
    }

    return parser_obj;
}


Parser_obj *
parse_tokens(Token *tokens)
{
    int current = 0;
    Parser_obj *parser_obj = parse_sequence(tokens, &current);
    if (parser_obj == NULL) {
        return NULL;
    }

    /* Current token type is not `NIL` means that
       we haven't parsed all the tokens */
    if (tokens[current].type != NIL) {
        fprintf(stderr, "Syntax Error\n");
        destroy_parser_obj(parser_obj);
        return NULL;
    }

    return parser_obj;
}

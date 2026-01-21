/*

*/

#ifndef LIST_H_
#define LIST_H_


#include "ast.h"
#include <stdbool.h>


typedef struct List_node
{
    bool        is_foreground;
    Ast_node   *ast_root;
    struct Job *next;
} List_node;


List_node *get_list_node(void);
void destroy_list(List_node *head);


#endif // LIST_H_
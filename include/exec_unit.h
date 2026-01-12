/*
    The parser returns a linked list, where each node of the
    list represents an instance/execution unit. An execution 
    unit is something that is separated by `;` or `&` in the
    command line. For ex:

    `ls | whoami && cat file ; whatis man & ping site.com | grep abc &`

    represents 3 execution units:
    1. `ls | whoami && cat file`
    2. `whatis man`
    3. `ping site.com | grep abc`

    Each unit will either be executed in foreground or background.
    For ex:
    `ls | whoami && cat file` will be executed in foreground, as its
    terminated by a `;`

    `whatis man` will be executed in the background as its terminated
    by a `&`

    `ping site.com | grep abc` will also be executed in background as
    its also terminated by a `&`

    When the shell sees that a unit should be executed in background, it
    creates a child shell and hands it the unit to be executed. The child
    shell will execute the unit asynchronously in the background, while the
    parent shell executes the foreground units.

    Each execution unit will have its own AST, that the shell/subshell needs
    to traverse and execute. Each unit also points to the next unit, thus
    creating a linked list.

    Note that there can only be one unit being executed in the foreground.
*/

#ifndef EXEC_UNIT_H_
#define EXEC_UNIT_H_


#include "ast.h"


typedef enum Execution_context
{
    FOREGROUND, BACKGROUND,
} Execution_context;


typedef struct Execution_unit
{
    Execution_context      context;
    Ast_node              *ast_root;
    struct Execution_unit *next;
} Execution_unit;


Execution_unit *get_exec_unit_node(void);
void destroy_exec_unit_list(Execution_unit *head);


#endif // EXEC_UNIT_H_
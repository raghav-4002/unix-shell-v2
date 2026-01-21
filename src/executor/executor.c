#include <stdio.h>

#include "executor.h"
#include "ast.h"
#include "list.h"
#include "pipeline.h"


void
execute(List_node *head)
{
    for (List_node *job = head; job != NULL; job = job->next) {
        
    }
}
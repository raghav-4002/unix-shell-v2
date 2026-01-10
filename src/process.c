#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "process.h"


Process_obj *
get_process_obj(void)
{
    Process_obj *process_obj = malloc(sizeof(*process_obj));
    if (process_obj == NULL) {
        perror("get_process_obj");
        return NULL;
    }

    process_obj->argv     = NULL;
    process_obj->argc     = 0;
    process_obj->capacity = 0;
    return process_obj;
}


void
destroy_process_obj(Process_obj *process_obj)
{
    /* Free every `char *` in argv array */
    for (int i = 0; i < process_obj->argc; i++) {
        free(process_obj->argv[i]);
    }

    /* Free the argv array */
    free(process_obj->argv);

    /* Free the structure */
    free(process_obj);
}


int
add_arg_to_process(Process_obj *process_obj, const char *arg)
{
    #define INCR_SIZE 2

    if (process_obj->capacity <= process_obj->argc) {
        /* Increase capacity for insufficient size */
        process_obj->capacity += INCR_SIZE;
        
        char **temp = realloc(process_obj->argv, process_obj->capacity);
        if (temp == NULL) {
            process_obj->capacity -= 1; /* reset capacity */
            perror("add_arg_to_cmd");
            return -1;
            /* process_obj->argv still points to prev memory */
        }
    
        process_obj->argv = temp;
    }

    process_obj->argv[process_obj->argc] = arg;
    process_obj->argc += 1;

    /* Return index where arg is stored */
    return process_obj->argc - 1;

    #undef INCR_SIZE
}
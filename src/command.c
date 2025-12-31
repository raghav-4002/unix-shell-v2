#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "command.h"


Command_obj *
get_cmd_obj(void)
{
    Command_obj *cmd_obj = malloc(sizeof(*cmd_obj));
    if (cmd_obj == NULL) {
        perror("get_cmd_obj");
        return NULL;
    }

    cmd_obj->argv     = NULL;
    cmd_obj->argc     = 0;
    cmd_obj->capacity = 0;
    cmd_obj->out_fd   = STDOUT_FILENO; /* default OP desc. */
    cmd_obj->in_fd    = STDIN_FILENO;  /* default OP desc. */

    return cmd_obj;
}


void
destroy_cmd_obj(Command_obj *cmd_obj)
{
    /* Free every `char *` in argv array */
    for (int i = 0; i < cmd_obj->argc; i++) {
        free(cmd_obj->argv[i]);
    }

    /* Free the argv array */
    free(cmd_obj->argv);

    /* Free the structure */
    free(cmd_obj);
}


int
add_arg_to_cmd(Command_obj *cmd_obj, const char *arg)
{
    #define INCR_SIZE 2

    if (cmd_obj->capacity <= cmd_obj->argc) {
        /* Increase capacity for insufficient size */
        cmd_obj->capacity += INCR_SIZE;
        
        char **temp = realloc(cmd_obj->argv, cmd_obj->capacity);
        if (temp == NULL) {
            cmd_obj->capacity -= 1; /* reset capacity */
            perror("add_arg_to_cmd");
            return -1;
            /* cmd_obj->argv still points to prev memory */
        }
    
        cmd_obj->argv = temp;
    }

    cmd_obj->argv[cmd_obj->argc] = arg;
    cmd_obj->argc += 1;

    /* Return index where arg is stored */
    return cmd_obj->argc - 1;

    #undef INCR_SIZE
}
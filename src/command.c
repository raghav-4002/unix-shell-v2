#include <stdlib.h>
#include <stdio.h>

#include "command.h"


Command *
init_cmd_obj(void)
{
    Command *command = malloc(sizeof(*command));
    if (command == NULL) {
        perror("Command object initialization");
        return NULL;
    }

    command->argv     = NULL;
    command->argc     = 0;
    command->capacity = 0;
    return command;
}

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "command.h"


Cmd_table *
init_cmd_table(void)
{
    Cmd_table *cmd_table = malloc(sizeof(*cmd_table));
    if (cmd_table == NULL) {
        perror("Init Command Table");
        return NULL;
    }

    cmd_table->capacity = 0;
    cmd_table->count    = 0;
    cmd_table->command  = NULL;
    return cmd_table;
}


void
destroy_command(Command command)
{
    for (int i = 0; command[i] != NULL; i++) {
        free(command[i]);        
    }
    free(command);
}


void
destroy_cmd_table(Cmd_table *cmd_table)
{
    for (int i = 0; i < cmd_table->count; i++) {
        destroy_command(cmd_table->command[i]);
    }

    free(cmd_table->command);
    cmd_table->capacity = 0;
    cmd_table->count    = 0;
    cmd_table->command  = NULL;
}


/* Inserts a command into the command table. Returns the 
   index of the added command. */
int
write_command(Cmd_table *cmd_table, const Command command)
{
    if (cmd_table->capacity <= cmd_table->count) {
        cmd_table->capacity = cmd_table->capacity == 0 
                            ? 4 : cmd_table->capacity + 4;

        Command *temp = realloc(cmd_table->command, cmd_table->capacity);
        if (temp == NULL) {
            perror("write_command");
            destroy_cmd_table(cmd_table);
            return -1;
        }

        cmd_table->command = temp;
    }

    cmd_table->command[cmd_table->count] = command;
    cmd_table->count += 1;
    return cmd_table->count - 1;
}

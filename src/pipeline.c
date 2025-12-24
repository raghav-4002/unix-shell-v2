#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "pipeline.h"


Pipeline *
get_pipeline_obj(void)
{
    Pipeline *pipeline_obj = malloc(sizeof(*pipeline_obj));
    if (pipeline_obj == NULL) {
        perror("Initializing pipeline object");
        return NULL;
    }

    pipeline_obj->command  = NULL;
    pipeline_obj->count    = 0;
    pipeline_obj->capacity = 0;
    return pipeline_obj;
}


void
destroy_pipeline_obj(Pipeline *pipeline_obj)
{
    for (int i = 0; i < pipeline_obj->count; i++) {
        /* Destroy each command in pipeline */
        destroy_cmd_obj(pipeline_obj->command[i]);
    }

    free(pipeline_obj);
}


int
add_cmd_to_pipeline(Pipeline *pipeline_obj, Command *command_obj)
{
    #define INCR_SIZE 1

    if (pipeline_obj->capacity <= pipeline_obj->count) {
        pipeline_obj->capacity += INCR_SIZE;

        Command **command = realloc(pipeline_obj->command, pipeline_obj->capacity);
        if (command == NULL) {
            perror("Add command to pipeline");
            pipeline_obj->capacity -= INCR_SIZE; /* reset capacity */
            return -1;
        }

        pipeline_obj->command = command;
    }

    pipeline_obj->command[pipeline_obj->count] = command_obj;
    pipeline_obj->count += 1;
    return 0;

    #undef INCR_SIZE
}

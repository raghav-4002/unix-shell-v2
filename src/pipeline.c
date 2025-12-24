#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "pipeline.h"


Pipeline *
get_pipeline_obj(void)
{
    Pipeline *pipeline = malloc(sizeof(*pipeline));
    if (pipeline == NULL) {
        perror("Initializing pipeline object");
        return NULL;
    }

    pipeline->command  = NULL;
    pipeline->count    = 0;
    pipeline->capacity = 0;
    return pipeline;
}


void
destroy_pipeline_obj(Pipeline *pipeline)
{
    for (int i = 0; i < pipeline->count; i++) {
        /* Destroy each command in pipeline */
        destroy_cmd_obj(pipeline->command[i]);
    }

    free(pipeline);
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

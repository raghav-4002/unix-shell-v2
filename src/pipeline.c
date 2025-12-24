#include <stdio.h>
#include <stdlib.h>

#include "command.h"
#include "pipeline.h"


Pipeline *
get_pipeline_obj(void)
{
    Pipeline *pipeline_obj = malloc(sizeof(*pipeline_obj));
    if (pipeline_obj == NULL) {
        perror("Pipeline object initialization");
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
    return (pipeline_obj->count - 1);

    #undef INCR_SIZE
}


Pipeline_table *
get_pipeline_table(void)
{
    Pipeline_table *pipeline_table = malloc(sizeof(*pipeline_table));
    if (pipeline_table == NULL) {
        perror("Pipeline table initialization");
        return NULL;
    }

    pipeline_table->pipeline = NULL;
    pipeline_table->count    = 0;
    pipeline_table->capacity = 0;
    return pipeline_table;
}


void
destroy_pipeline_table(Pipeline_table *pipeline_table)
{
    for (int i = 0; i < pipeline_table->count; i++) {
        destroy_pipeline_obj(pipeline_table->pipeline[i]);
    }

    free(pipeline_table);
}


int
add_pipeline_to_table(Pipeline_table *pipeline_table, Pipeline *pipeline_obj)
{
    #define INCR_SIZE 2

    if (pipeline_table->capacity <= pipeline_table->count) {
        pipeline_table->capacity += INCR_SIZE;

        Pipeline **pipeline = realloc(pipeline_table->pipeline, pipeline_table->capacity);
        if (pipeline == NULL) {
            perror("Add pipeline to table");
            pipeline_table->capacity -= INCR_SIZE; /* reset size */
            return -1;
        }

        pipeline_table->pipeline = pipeline;
    }

    pipeline_table->pipeline[pipeline_table->count] = pipeline_obj;
    pipeline_table->count += 1;
    return (pipeline_table->count - 1);

    #undef INCR_SIZE
}

#include <stdio.h>
#include <stdlib.h>

#include "pipeline.h"
#include "command.h"


/* ========== Pipeline object ========== */

Pipeline_obj *
get_pipeline_obj(void)
{
    Pipeline_obj *pipe_obj = malloc(sizeof(*pipe_obj));
    if (pipe_obj == NULL) {
        perror("get_pipeline_obj");
        return NULL;
    }

    pipe_obj->command  = NULL;
    pipe_obj->count    = 0;
    pipe_obj->capacity = 0;

    return pipe_obj;
}


void
destroy_pipeline_obj(Pipeline_obj *pipe_obj)
{
    for (int i = 0; i < pipe_obj->count; i++) {
        /* Destroy each command in the pipeline */
        destroy_cmd_obj(pipe_obj->command[i]);
    }

    free(pipe_obj->command);
    free(pipe_obj);
}


int
add_cmd_to_pipeline(Pipeline_obj *pipe_obj, Command_obj *cmd_obj)
{
    #define INCR_SIZE 1

    if (pipe_obj->capacity <= pipe_obj->count) {
        pipe_obj->capacity += INCR_SIZE;

        Command_obj **temp = realloc(pipe_obj->command, pipe_obj->capacity);
        if (temp == NULL) {
            pipe_obj->capacity -= 1; /* reset capacity */
            perror("add_cmd_to_pipeline");
            return -1;
        }

        pipe_obj->command = temp;
    }

    /* Store the command obj in array */
    pipe_obj->command[pipe_obj->count] = cmd_obj;
    pipe_obj->count += 1;

    /* Return index where the command was stored */
    return pipe_obj->count - 1;

    #undef INCR_SIZE
}


/* ========== Pipeline table ========== */

Pipeline_table *
get_pipeline_table(void)
{
    Pipeline_table *pipe_table = malloc(sizeof(*pipe_table));
    if (pipe_table == NULL) {
        perror("get_pipeline_table");
        return NULL;
    }

    pipe_table->pipeline = NULL;
    pipe_table->count    = 0;
    pipe_table->capacity = 0;

    return pipe_table;
}


void
destroy_pipeline_table(Pipeline_table *pipe_table)
{
    for (int i = 0; i < pipe_table->count; i++) {
        /* Destroy each pipeline in the table */
        destroy_pipeline_obj(pipe_table->pipeline[i]);
    }

    free(pipe_table->pipeline);
    free(pipe_table);
}


int
add_pipeline_to_table(Pipeline_table *pipe_table, Pipeline_obj *pipe_obj)
{
    #define INCR_SIZE 2

    if (pipe_table->capacity <= pipe_table->count) {
        pipe_table->capacity += INCR_SIZE;

        Pipeline_obj **temp = realloc(pipe_table->pipeline, pipe_table->capacity);
        if (temp == NULL) {
            pipe_table->capacity -= 1; /* reset capacity */
            perror("add_pipeline_to_table");
            return -1;
        }

        pipe_table->pipeline = temp;
    }

    /* Add pipeline to table */
    pipe_table->pipeline[pipe_table->count] = pipe_obj;
    pipe_table->count += 1;

    /* Return index where the pipeline was added */
    return pipe_table->count - 1;

    #undef INCR_SIZE
}

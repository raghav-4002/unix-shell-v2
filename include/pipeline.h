#ifndef PIPELINE_H_
#define PIPELINE_H_


#include "command.h"

/*
   A pipeline is an array of `Command` where
   each `Command` is a member of the pipeline.
   `ls -al | grep` => `{{"ls", "-al", NULL}, {"grep", NULL}}`
*/
typedef struct Pipeline
{
    Command **command;
    int       count;
    int       capacity;
} Pipeline;


/* Pipeline table is an array of `Pipeline` */
typedef struct Pipeline_table
{
    Pipeline *pipeline;
    int       count;
    int       capacity;
} Pipeline_table;


Pipeline *get_pipeline_obj(void);
void destroy_pipeline_obj(Pipeline *pipeline_obj);
int add_cmd_to_pipeline(Pipeline *pipeline_obj, Command *command_obj);


#endif // PIPELINE_H_

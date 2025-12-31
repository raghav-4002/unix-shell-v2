#ifndef PIPELINE_H_
#define PIPELINE_H_


#include "command.h"


/* 
   Struct representing a pipeline. 
   Is just an array of `Command_obj`
   where each command obj is a member
   of the pipeline
*/
typedef struct Pipeline_obj
{
    Command_obj **command;
    int           count;
    int           capacity;
} Pipeline_obj;


/*
    Struct representing all pipelines.
    Is just an array of `Pipeline_obj`
    where each pipeline obj is a pipeline
    in the line.
*/
typedef struct Pipeline_table
{
    Pipeline_obj **pipeline;
    int            count;
    int            capacity;
} Pipeline_table;


Pipeline_obj *get_pipeline_obj(void);
void destroy_pipeline_obj(Pipeline_obj *pipeline_obj);
int add_cmd_to_pipeline(Pipeline_obj *pipeline_obj, Command_obj *command_obj);

Pipeline_table *get_pipeline_table(void);
void destroy_pipeline_table(Pipeline_table *pipeline_table);
int add_pipeline_to_table(Pipeline_table *pipeline_table, Pipeline_obj *pipeline_obj);


#endif // PIPELINE_H_

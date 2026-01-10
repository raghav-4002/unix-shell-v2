#include <stdio.h>
#include <stdlib.h>

#include "job.h"
#include "process.h"


Job_obj *
get_job_obj(void)
{
    Job_obj *job_obj = malloc(sizeof(*job_obj));
    if (job_obj == NULL) {
        perror("get_job_obj");
        return NULL;
    }

    job_obj->process       = NULL;
    job_obj->process_count = 0;
    job_obj->capacity      = 0;
    return job_obj;
}


void
destroy_job_obj(Job_obj *job_obj)
{
    for (int i = 0; i < job_obj->process_count; i++) {
        /* Destroy each command in the pipeline */
        destroy_process_obj(job_obj->process[i]);
    }

    free(job_obj->process);
    free(job_obj);
}


int
add_process_to_job(Job_obj *job_obj, Process_obj *process_obj)
{
    #define INCR_SIZE 1

    if (job_obj->capacity <= job_obj->process_count) {
        job_obj->capacity += INCR_SIZE;

        Process_obj **temp = realloc(job_obj->process, job_obj->capacity);
        if (temp == NULL) {
            job_obj->capacity -= 1; /* reset capacity */
            perror("add_process_to_job");
            return -1;
        }

        job_obj->process = temp;
    }

    /* Store the process obj in array */
    job_obj->process[job_obj->process_count] = process_obj;
    job_obj->process_count += 1;

    /* Return index where the process was stored */
    return job_obj->process_count - 1;

    #undef INCR_SIZE
}


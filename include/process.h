#ifndef PROCESS_H_
#define PROCESS_H_


typedef struct Process_obj
{
    char **argv;     /* something like `{"ls", "-al", NULL}` */
    int    argc;     /* argument count including `NULL` */
    int    capacity; /* represents the actual size of argv array. 
                        capacity >= argc */
} Process_obj;


/* =========================================== */


/* Returns a pointer to default-initialized
   `Process_obj`. Returns `NULL` on error. */
Process_obj *get_process_obj(void);


/* Free up memory pointed by `process_obj` */
void destroy_process_obj(Process_obj *process_obj);


/* Add another argument to command array. 
   On success, returns index where the 
   arg was added. On failure, returns -1. */
int add_arg_to_process(Process_obj *process_obj, const char *arg);


#endif // PROCESS_H_
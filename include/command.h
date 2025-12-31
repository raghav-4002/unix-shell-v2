#ifndef COMMAND_H_
#define COMMAND_H_


/*
   Struct representing a command.
   Is just an array of `char *`, 
   where is `char *` is an argument
   of a command. The last member of
   the array must be `NULL`. 
*/
typedef struct Command_obj
{
    char **argv;     /* something like `{"ls", "-al", NULL}` */
    int    argc;     /* argument count including `NULL` */
    int    capacity; /* represents the actual size of argv array. 
                        capacity >= argc */

    int out_fd; /* Write to this file desc. (STDOUT_FILENO default) */
    int in_fd;  /* Read from this file desc. (STDIN_FILENO default) */
} Command_obj;


Command_obj *get_cmd_obj(void);
void destroy_cmd_obj(Command_obj *command_obj);
int add_arg_to_cmd(Command_obj *command_obj, const char *arg);


#endif // COMMAND_H_

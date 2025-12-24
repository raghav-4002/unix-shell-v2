#ifndef COMMAND_H_
#define COMMAND_H_


/*
    Structure representing a command.
    Members:
    - `char **argv`: An array of strings. Should be terminated
       by `NULL`. Ex - `{"ls", "-al", NULL}`
    
    - `int argc`: Number of arguments in `argv` including `NULL`.
       Ex - `{"ls", "-al", NULL}` has `argc = 3`

    - `int capacity`: Capacity of `argv`. It includes both the
       occupied and unoccupied space in `argv`
*/
typedef struct Command
{
    char **argv;     /* something like `{"ls", "-al", NULL}` */
    int    argc;     /* argument count including `NULL` */
    int    capacity; /* Capacity of argv array */
} Command;


Command *get_cmd_obj(void);
void destroy_cmd_obj(Command *command_obj);
int add_arg_to_cmd(Command *command_obj, const char *arg);


#endif // COMMAND_H_

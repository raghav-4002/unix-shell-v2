#ifndef COMMAND_H_
#define COMMAND_H_


typedef struct Command
{
    char **argv;     /* something like {"ls", "-al", NULL} */
    int    argc;     /* argument count including `NULL` */
    int    capacity; /* Capacity of argv array */
} Command;


Command *init_cmd_obj(void);
void destroy_cmd_obj(Command *command);
void add_arg_to_cmd_obj(Command *command, char *arg);


#endif // COMMAND_H_

#include <stdio.h>
#include <stdlib.h>

#include "io-handling/input.h"
#include "lexer/lexer.h"
#include "lexer/token.h"


void
run(char *line)
{
    Token *tokens = tokenize(line);

    /* For now, just print the tokens */
    for (size_t i = 0; tokens[i].type != NIL; i++) {
        switch (tokens[i].type) {
            case LOGIC_AND:
                printf("LOGIC_AND\n");
                break;

            case LOGIC_OR:
                printf("LOGIC_OR\n");
                break;

            case SEMICOLON:
                printf("SEMICOLON\n");
                break;

            case BG_OPERATOR:
                printf("BG_OPERATOR\n");
                break;

            case PIPE:
                printf("PIP\n");
                break;

            default:
                printf("%s\n", tokens[i].arg);
                break;
        }
    }
}


void
run_prompt(void)
{
    while (1) {
        // display_prompt();
        char *line = read_from_stdin();

        /* Line reading fails, just retry */
        if (line == NULL) continue;                

        run(line);
        free(line);
    }
}


int
run_script(char *filepath)
{
    fprintf(stderr, "Scripting is not supported as of yet.\n");
    return -1;
}


#define EXCESS_ARGS_ERR "Usage: ./shell [script]\n"

int
init_shell(int argc, char *argv[])
{
    int return_val = 0;

    if (argc == 2) {
        return_val = run_script(argv[1]);
    }
    else if (argc > 2) {
        fprintf(stderr, EXCESS_ARGS_ERR);
        return EXIT_FAILURE;
    }
    else if (argc == 1) {
        run_prompt();
    }

    if (return_val == -1) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "token.h"


#define TESTS_COUNT 1


typedef struct Test_case
{
    const char *input_string;
    Token output_tokens[MAX_TOK_COUNT];
} Test_case;


Test_case tests[TESTS_COUNT] = {
    {"ls", {{CMD, "ls"}, {NIL, NULL}}}
};


bool
check_lexer_output(Token *expected_output, Token *actual_output)
{
    for (size_t i = 0; actual_output[i].type != NIL; i++) {
        if (actual_output[i].type != expected_output[i].type) {
            return false;
        }

        if (actual_output[i].lexeme != NULL && expected_output[i].lexeme != NULL) {
            if (!strcmp(actual_output[i].lexeme, expected_output[i].lexeme)) {
                return true;
            }
            return false;
        }

        return false;
    }
}


int
main(void)
{
    for (size_t i = 0; i < TESTS_COUNT; i++) {
        const char *input_string = tests[i].input_string;
        Token *expected_output = tests[i].output_tokens;
        Token *actual_output = tokenize(input_string);

        if (check_lexer_output(expected_output, actual_output) == true) {
            printf("Pass: Testcase %zu: %s\n", i, input_string);
        }
        else {
            printf("Fail: Testcase %zu: %s\n", i, input_string);
        }

        free(actual_output);
    }
}

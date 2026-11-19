/**
 * A sample program for parsing a command line. If you find it useful,
 * feel free to adapt this code for Assignment 4.
 * Do fix memory leaks and any additional issues you find.
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define INPUT_LENGTH 2048
#define MAX_ARGS 512
struct command_line
{
    char *argv[MAX_ARGS + 1];
    int argc;
    char *input_file;
    char *output_file;
    bool is_bg;
};
struct command_line *parse_input()
{
    char input[INPUT_LENGTH];
    struct command_line *curr_command = (struct command_line *)calloc(1,
                                                                      sizeof(struct command_line));
    // Get input
    printf(": ");
    fflush(stdout);
    fgets(input, INPUT_LENGTH, stdin);
    // Tokenize the input
    char *token = strtok(input, " \n");
    while (token)
    {
        if (!strcmp(token, "<"))
        {
            curr_command->input_file = strdup(strtok(NULL, " \n"));
        }
        else if (!strcmp(token, ">"))
        {
            curr_command->output_file = strdup(strtok(NULL, " \n"));
        }
        else if (!strcmp(token, "&"))
        {
            curr_command->is_bg = true;
        }
        else
        {
            curr_command->argv[curr_command->argc++] = strdup(token);
        }
        token = strtok(NULL, " \n");
    }
    return curr_command;
}
int main()
{
    struct command_line *curr_command;
    while (true)
    {
        curr_command = parse_input();

        if (strcmp(curr_command->argv[0], "exit")==0)
        {
            printf("exit\n");
            break;
        } else if (strcmp(curr_command->argv[0], "ls")==0)
        {
            printf("list Directory\n");
        } else if (strcmp(curr_command->argv[0], "cd")==0)
        {
            printf("change directory\n");
        } else {
            printf("other\n");
        }
        
        
    }
    return EXIT_SUCCESS;
}
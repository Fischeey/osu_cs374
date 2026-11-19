/**
 * A sample program for parsing a command line. If you find it useful,
 * feel free to adapt this code for Assignment 4.
 * Do fix memory leaks and any additional issues you find.
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#define INPUT_LENGTH 2048
#define MAX_ARGS 512
struct command_line
{
    char *argv[MAX_ARGS + 1];
    int argc;
    char *input_file;
    char *output_file;
    bool is_comment;
    bool is_bg;
};
struct command_line *parse_input()
{
    char input[INPUT_LENGTH];
    struct command_line *curr_command = (struct command_line *)calloc(1,
                                                                      sizeof(struct command_line));
    // Get input
    
    fflush(stdout);
    fgets(input, INPUT_LENGTH, stdin);
    // Tokenize the input
    char *token = strtok(input, " \n");
    curr_command->is_comment = 0;
    while (token)
    {
        if (token[0] == '#')
        {
            curr_command->is_comment = true;
            break; // ignore the rest of the comment line
        }  else if (!strcmp(token, "<"))
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
    char file_path[512];
    char exit_status[48];
    struct command_line *curr_command = NULL;
    char *home = getenv("HOME");
    strcpy(home, file_path);
    strcpy(exit_status, "exit status 0");
    while (true)
    {
        printf(": ");
        fflush(stdout);
        curr_command = parse_input();
        //printf("%s\n", curr_command->is_comment ? "true" : "false");
        if (curr_command->is_comment == true)
        {
            break;
        } else if (strcmp(curr_command->argv[0], "exit")==0)
        {
            printf("exit\n");
            break;
        } else if (strcmp(curr_command->argv[0], "cd")==0)
        {
            char *home = getenv("HOME");
    if (curr_command->argc == 1) {
        char *home = getenv("HOME");
        if (home == NULL || home[0] == '\0') {
            fprintf(stderr, "cd: HOME not set\n");
        } else if (chdir(home) != 0) {
            perror("cd");
        }
    } else {
                
                printf("%s\n", curr_command->argv[1]);
                chdir(curr_command->argv[1]);
            }
            
        } else if (strcmp(curr_command->argv[0], "status")==0)
        {
            printf("%s\n", exit_status);
        } else {
            pid_t spawnpid = -5;
            int intVal = 10;
            // If fork is successful, the value of spawnpid will be 0 in the child
            // and will be the child's pid in the parent
            //printf("run function");
            if (curr_command->input_file != NULL) {
                printf("input_file: %s\n", curr_command->input_file);
            }
            if (curr_command->output_file != NULL) {
                printf("output_file: %s\n", curr_command->output_file);
            }
            fflush(stdout);
            spawnpid = fork();
            switch (spawnpid){
                case -1:
                    perror("fork() failed!");
                    exit(1);
                    break;
                case 0:


                if (curr_command->input_file != NULL) {
                    int in_fd = open(curr_command->input_file, O_RDONLY);
                    if (in_fd == -1) {
                        perror("cannot open input file");
                        exit(1);
                    }
                    dup2(in_fd, STDIN_FILENO);
                    close(in_fd);
                }

                if (curr_command->output_file != NULL) {
                    int out_fd = open(curr_command->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (out_fd == -1) {
                        perror("cannot open output file");
                        exit(1);
                    }
                    dup2(out_fd, STDOUT_FILENO);
                    close(out_fd);
                }
                    // spawnpid is 0 in the child
                    execvp(curr_command->argv[0], curr_command->argv);
                    // Only reached if execvp fails:
                    perror(curr_command->argv[0]);
                    strcpy(exit_status, "exit status 0");
                    exit(1);
                default:
                    // spawnpid is the pid of the child
                    int childStatus;
                    waitpid(spawnpid, &childStatus, 0);
                    break;
            }
            
        }
        
        
        
    }
    return EXIT_SUCCESS;
}
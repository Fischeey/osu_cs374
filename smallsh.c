/**
 * 
 * 
 * 
 */

//included libraries
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>



// defining constants
#define INPUT_LENGTH 2048
#define MAX_ARGS 512
#define MAX_BG_PIDS 512
//define command line struct
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
    struct command_line *curr_command = (struct command_line *)calloc(1, sizeof(struct command_line));
    // Get input
    
    fflush(stdout);
    fgets(input, INPUT_LENGTH, stdin);
    // Tokenize the input

    char *token = strtok(input, " \n");
    curr_command->is_comment = false;
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
    int PID_arr[64];
    int PID_arr_length = 0;
    struct command_line *curr_command = NULL;

    char *home = getenv("HOME");
    if (home != NULL) {
        snprintf(file_path, sizeof(file_path), "%s", home);
    }
    strcpy(exit_status, "exit value 0");
    

void handle_SIGINT(int signo){
  char* message = "Caught SIGINT\n";
  // We are using write rather than printf
  write(STDOUT_FILENO, message, 14);
}

volatile bool foreground_only = 0;

void handle_SIGTSTP(int signo) {
    (void)signo;
    if (foreground_only == 0) {
        foreground_only = 1;
        const char *msg = "\nEntering foreground-only mode (& is now ignored)\n";
        write(STDOUT_FILENO, msg, 50);
    } else {
        foreground_only = 0;
        const char *msg = "\nExiting foreground-only mode\n";
        write(STDOUT_FILENO, msg, 30);
    }
}



      struct sigaction SIGINT_action = {0};
      struct sigaction SIGTSTP_action = {0};
  // Fill out the SIGINT_action struct
  // Register handle_SIGINT as the signal handler
  SIGINT_action.sa_handler = handle_SIGINT;
  SIGTSTP_action.sa_handler = handle_SIGTSTP;
  // Block all catchable signals while handle_SIGINT is running
  sigfillset(&SIGINT_action.sa_mask);
    sigfillset(&SIGTSTP_action.sa_mask);
  // No flags set
  SIGINT_action.sa_flags = 0;
  SIGTSTP_action.sa_flags = 0;

  // Install our signal handler
  sigaction(SIGINT, &SIGINT_action, NULL);
  sigaction(SIGTSTP, &SIGTSTP_action, NULL);





    while (true)
    {
        int pid;
        int status =-1;
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0){
            fprintf(stderr, "[DBG] waitpid returned pid=%d, status=%d\n", pid, status);
            if (WIFEXITED(status)) {
                printf("background pid %d is done: exit value %d\n", pid, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("background pid %d is done: terminated by signal %d\n", pid, WTERMSIG(status));
            }
        }

        printf(": ");
        fflush(stdout);
        curr_command = parse_input();
        //printf("%s\n", curr_command->is_comment ? "true" : "false");
        if (curr_command->is_comment == true)
        {
            continue;
        } else if (strcmp(curr_command->argv[0], "exit")==0)
        {
            printf("exit\n");
            break;
        } else if (strcmp(curr_command->argv[0], "cd") == 0) {
    if (curr_command->argc == 1) {
        char *home = getenv("HOME");
        if (home != NULL && home[0] != '\0') {
            if (chdir(home) != 0) {
                perror("cd");
            }
        }

        if (home != NULL && chdir(home) != 0) {
            perror("cd");
        }
    } else {
        if (chdir(curr_command->argv[1]) != 0) {
            perror("cd");
        }
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
                    strcpy(exit_status, "exit value 0");
                    exit(1);
                default:
                    // spawnpid is the pid of the child
                    int childStatus = 0;
                        if (curr_command->is_bg && !foreground_only) {
                            printf("background pid is %d\n", spawnpid);
                            fflush(stdout);
                            if (PID_arr_length < MAX_BG_PIDS) {
                                PID_arr[PID_arr_length++] = spawnpid;
                            }
                        } else {
                            waitpid(spawnpid, &childStatus, 0);
                            // Update foreground exit / signal status here
                        }
                    





                    if (WIFEXITED(childStatus)) {
    // Normal exit (e.g., exit(0), exit(1), or returning from main)
    int code = WEXITSTATUS(childStatus);
    snprintf(exit_status, sizeof(exit_status), "exit value %d", code);
} else if (WIFSIGNALED(childStatus)) {
    // Terminated by an unhandled signal (e.g., SIGTERM, SIGKILL)x
    int sig = WTERMSIG(childStatus);
    snprintf(exit_status, sizeof(exit_status), "terminated by signal %d", sig);
}




                    break;
            }
            
        }
        
        
        
    }
    return EXIT_SUCCESS;
}
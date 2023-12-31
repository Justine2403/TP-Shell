#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#define PROMPT "\nenseash % "
#define WELCOME "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'."
#define MAX_SIZE 128

char command[MAX_SIZE];
int bytesRead;
struct timespec start, end; // Variables to store the start and end times
int status = -1;
char *inputFile = NULL;
char *outputFile = NULL;

int print_message(char *output) {
    write(1, output, strlen(output));
}

void exiting() {
    // check if command is "exit" or Ctrl+D
    if ((strcmp(command, "exit") == 0) || (bytesRead == 0)) {
        print_message("\nBye bye...\n"); // print bye-bye message
        exit(EXIT_SUCCESS);                 // break the loop
    }
}

void print_prompt_message(int status, long time_diff) {
    char msg[50];

    // Check if the child process exited normally.
    if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        int len = sprintf(msg, "\nenseash [exit:%d|%ldms] %% ", exit_status, time_diff);
        write(1, msg, len);
    }
    // Check if the child process was terminated by a signal.
    else if (WIFSIGNALED(status)) {
        int term_signal = WTERMSIG(status);
        int len = sprintf(msg, "\nenseash [sign:%d|%ldms] %% ", term_signal, time_diff);
        write(1, msg, len);
    }
}

void tokenize_execute() {
    // Tokenize the command and arguments
    char *token = strtok(command, " ");
    char *args[MAX_SIZE];
    int arg_count = 0;

    while (token != NULL) {
        if (strcmp(token, ">") == 0 || strcmp(token, "<") == 0 || strcmp(token, "|") == 0) {
            break;
        } else {
            args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    execvp(args[0], args); // execute the command with arguments
    perror("error");
    exit(EXIT_FAILURE);
}

void handle_input_redirection(char *command) {
    char *input_redirect = strchr(command, '<');
    if (input_redirect != NULL) {
        *input_redirect = '\0'; // Null terminate the command at '<'
        input_redirect++;

        while (*input_redirect == ' ') {
            input_redirect++; // Skip any spaces after '<'
        }
        int fd = open(input_redirect, O_RDONLY); // Open the file for reading
        if (fd < 0) {
            perror("Error opening input file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, 0); // Redirect stdin to the file
        close(fd);   // Close the file descriptor
    }
}

void handle_output_redirection(char *command) {
    char *output_redirect = strchr(command, '>');
    if (output_redirect != NULL) {
        *output_redirect = '\0';
        output_redirect++; // Move to the filename after '<'

        while (*output_redirect == ' ') {
            output_redirect++; // Skip any spaces after '>'
        }
        int fd = open(output_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open the file for writing
        if (fd < 0) {
            perror("Error opening output file");
            exit(EXIT_FAILURE);
        }
        dup2(fd, 1); // Redirect stdout to the file
        close(fd);   // Close the file descriptor
    }
}

void execute() {
    while (1) {
        bytesRead = read(0, command, MAX_SIZE - 1);

        // replace newline character with null terminator
        char *pos;
        if ((pos = strchr(command, '\n')) != NULL)
            *pos = '\0';

        exiting();

        // Make a copy of the command string
        char command_copy[MAX_SIZE];
        strncpy(command_copy, command, MAX_SIZE);

        // Tokenize the command to check for pipes
        char *token = strtok(command_copy, " ");
        while (token != NULL) {
            if (strcmp(token, "|") == 0) {
                // If a pipe is found, set up pipes and execute commands on each side
                int pipe_fd[2];
                if (pipe(pipe_fd) == -1) {
                    perror("Pipe creation error");
                    exit(EXIT_FAILURE);
                }

                pid_t child_pid = fork();

                if (child_pid == 0) {
                    // Child process - write to the pipe
                    close(pipe_fd[0]);            // Close read end of the pipe
                    dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
                    close(pipe_fd[1]);            // Close the write end of the pipe
                    tokenize_execute();          // Execute the command on the left side of the pipe
                    exit(EXIT_SUCCESS);
                } else {
                    // Parent process - read from the pipe
                    close(pipe_fd[1]);            // Close the write end of the pipe
                    dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
                    close(pipe_fd[0]);            // Close the read end of the pipe

                    // Move to the next token after "|"
                    token = strtok(NULL, " ");

                    // Set up output redirection for the right side of the pipe
                    handle_output_redirection(token);

                    // Execute the command on the right side of the pipe
                    tokenize_execute();

                    // Wait for both child processes to finish
                    wait(NULL);
                    wait(NULL);

                    // Reset stdin to the terminal
                    dup2(0, STDIN_FILENO);
                }
            } else {
                token = strtok(NULL, " ");
            }
        }

        // If no pipe is found, execute the command as usual
        pid_t pid = fork();

        if (pid == 0) {
            handle_input_redirection(command_copy);
            handle_output_redirection(command_copy);
            tokenize_execute();
        } else {
            clock_gettime(CLOCK_REALTIME, &start); // Get the current time before
            waitpid(pid, &status, 0);
            clock_gettime(CLOCK_REALTIME, &end); // Get the current time after

            // Calculate the time difference
            long time_diff = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
            print_prompt_message(status, time_diff);
        }
    }
}

int main(int argc, char **argv[]) {
    // show welcome message
    print_message(WELCOME);
    print_message(PROMPT);
    execute();  
    return 0;
}

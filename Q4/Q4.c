#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define PROMPT "\nenseash % "
#define MAX_SIZE 128

char command[MAX_SIZE]; 
int bytesRead;

int print_message(char *output) {
	write(1, output, strlen(output)); 
}

void exiting(){
// check if command is "exit" or Ctrl+D
    if ((strcmp(command, "exit") == 0) || (bytesRead == 0)) {
    print_message("\nBye bye...\n");  // print bye bye message
    exit(EXIT_SUCCESS); // break the loop
    }
}

void print_prompt_message(int status) {
    char msg[50];     

    // Check if the child process exited normally.
    if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        int len = sprintf(msg, "\nenseash [exit:%d] %% ", exit_status);
        print_message(msg);
    } 
    // Check if the child process was terminated by a signal.
    else if (WIFSIGNALED(status)) {
        int term_signal = WTERMSIG(status);
        int len = sprintf(msg, "\nenseash [sign:%d] %% ", term_signal);
        print_message(msg);
    } 
    else {
        print_message(PROMPT);
    }
}

void execute(){
    int status = -1;
        while(1){
        print_prompt_message(status);
        bytesRead = read(0, command, MAX_SIZE);
        
        // replace newline character with null terminator
        char *pos;
        if ((pos=strchr(command, '\n')) != NULL)
            *pos = '\0';

        exiting();
        
        pid_t pid = fork();

        if (pid == 0) {
            execlp(command, command, NULL);
            perror("error");
            exit(EXIT_FAILURE); 
        }
        else{
             waitpid(pid, &status, 0);
        }
    }
}

int main (int argc, char **argv[]){
    print_message("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.");
    execute(); 
    return 0;
}

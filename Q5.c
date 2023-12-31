#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define PROMPT "\nenseash % "
#define WELCOME "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'."
#define MAX_SIZE 128

char command[MAX_SIZE]; 
int bytesRead;
struct timespec start, end; // Variables to store the start and end times
int status = -1;


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

void print_prompt_message(int status, long time_diff) {
    char msg[50];   

    // Check if the child process exited normally.
    if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        int len = sprintf(msg, "\nenseash [exit:%d|%ldms] %% ", exit_status,time_diff);
        write(1, msg, len);
    } 
    // Check if the child process was terminated by a signal.
    else if (WIFSIGNALED(status)) {
        int term_signal = WTERMSIG(status);
        int len = sprintf(msg, "\nenseash [sign:%d|%ldms] %% ", term_signal,time_diff);
        write(1, msg, len);
    } 
}

//adding time difference in execute
void execute(){       
        while(1){
        bytesRead = read(0, command, MAX_SIZE-1);

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
        else {
            
            clock_gettime(CLOCK_REALTIME, &start); // Get the current time before 
            waitpid(pid, &status, 0);
            clock_gettime(CLOCK_REALTIME, &end); // Get the current time after 

            // Calculate the time difference 
            long time_diff = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
            print_prompt_message(status, time_diff);
        }
    }
}



int main (int argc, char **argv[]){
    // show welcome message
	print_message(WELCOME);
    print_message(PROMPT);
    execute(); // execute the shell 
    
    return 0;
}

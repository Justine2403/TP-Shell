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

void execute(){
    while (1) {
        bytesRead = read(0, command, MAX_SIZE-1);
        // replace newline character with null terminator
        char *pos;
        if ((pos=strchr(command, '\n')) != NULL)
            *pos = '\0';

        exiting();

        pid_t pid = fork();

        if (pid == 0) {
            execlp(command, command, NULL); // execute the input command
            perror("error");
            exit(EXIT_FAILURE);
        }

        else{
            wait(NULL);
        }

        print_message(PROMPT);
        }
}

int main (int argc, char **argv[]){
    print_message("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.");
	print_message(PROMPT);
    execute(); 

    return 0;
}

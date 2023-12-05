#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PROMPT "\nenseash % "
#define WELCOME "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'."
#define MAX_SIZE 128


int print_message(char *output) {
	write(1, output, strlen(output)); 
}

//create a function to execute the shell
void execute(){
    char command[MAX_SIZE]; 
    while (1) {
        read(0, command, MAX_SIZE-1);   

        // replace newline character with null terminator
        char *pos;
        if ((pos= strchr(command, '\n')) != NULL)
            *pos = '\0';

        // create a child process
        pid_t pid = fork();

        if (pid == 0) {
            execlp(command, command, NULL); // execute the input command
            perror("error");
            exit(EXIT_FAILURE);
        }

        else{
            wait(NULL);
        }
        // output of the prompt again 
        print_message(PROMPT);
        }
}

int main (int argc, char **argv[]){

    print_message(WELCOME);
	print_message(PROMPT);
    execute(); // execute the shell 
    
    return 0;
}

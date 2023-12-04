#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int print_welcome_message() {
	// define welcome message
    char *msgWelcome = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.";
    //length of the string with strlen
	write(1, msgWelcome, strlen(msgWelcome)); //fd = 1 is for Standard Output which is our terminal	
}

int print_prompt_message() {
	//show of simple prompt
    char *msgPrompt = "\nenseash % ";
    //length of the string with strlen
	write(1, msgPrompt, strlen(msgPrompt));
}

void execute(){
    // define a array to store the command
    char command[50]; 
    while (1) {
        read(0, command, 49);   

        // replace newline character with null terminator
        char *pos;
        if ((pos= strchr(command, '\n')) != NULL)
            *pos = '\0';

        // create a child process
        pid_t pid = fork();

        if (pid == 0) {
            // this is the child process
            execlp(command, command, NULL); // execute the input command
        }
        else{
            wait(NULL);
        }
        // output of the prompt again 
        print_prompt_message();
        }
}

int main (int argc, char **argv[]){
    // show welcome message
	print_welcome_message();
    print_prompt_message();
    execute(); // execute the shell 
    
    return 0;
}

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

int print_bye_message(){
	//show bye bye message for when exiting
	char *msg_bye = "\nBye bye\n";
	write(1, msg_bye, strlen(msg_bye));
}

void execute_exit(){
    // define a array to store the command
    char command[50]; 
    while (1) {
        int bytesRead = read(0, command, 49);

        // Check for Ctrl+D command
        if (bytesRead == 0) {
        print_bye_message();  // print bye bye message
        break; // break the loop
        }

        // replace newline character with null terminator
        char *pos;
        if ((pos=strchr(command, '\n')) != NULL)
            *pos = '\0';

        // check if command is "exit"
        if (strcmp(command, "exit") == 0) {
        print_bye_message();  // print bye bye message
        break; // break the loop
        }
        
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
    execute_exit(); // execute the shell 
    
    return 0;
}

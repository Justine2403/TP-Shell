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

// Prints the prompt message.
void print_prompt_message(int status) {
    char msg[50];     // Declare a character array to hold message to be printed.
    // Check if the child process exited normally.
    if (WIFEXITED(status)) {
        // If it did, get the exit status of the child.
        int exit_status = WEXITSTATUS(status);
        // Format the message to include the exit status.
        int len = sprintf(msg, "\nenseash [exit:%d] %% ", exit_status);
        // Write the message to the standard output.
        write(1, msg, len);
    } 
    // Check if the child process was terminated by a signal.
    else if (WIFSIGNALED(status)) {
        // If it was, get the signal number that terminated the process.
        int term_signal = WTERMSIG(status);
        // Format the message to include the signal number.
        int len = sprintf(msg, "\nenseash [sign:%d] %% ", term_signal);
        // Write the message to the standard output.
        write(1, msg, len);
    } 
    // If the child process neither exited normally nor was terminated by a signal,
    // just print "enseash % ".
    else {
        write(1, "\nenseash % ", 10);
    }
}

int print_bye_message(){
	//show bye bye message for when exiting
	char *msg_bye = "\nBye bye\n";
	write(1, msg_bye, strlen(msg_bye));
}

void execute_exit(){
    // define a array to store the command
    char command[50]; 
    int status = -1;
        while(1){
        print_prompt_message(status);
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
            exit(0);
        }
        else{
             waitpid(pid, &status, 0);
        }
        }

}


int main (int argc, char **argv[]){
    // show welcome message
	print_welcome_message();
    execute_exit(); // execute the shell 
    
    return 0;
}

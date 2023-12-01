#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BUFSIZE 128

int main (int argc, char **argv[]){
    char buffer[BUFSIZE];

    // define fortune message
    char *msg_fortune = "Today is what happened to yesterday.";
    
	// define the date
	char *msg_date = "Sun Dec 13 13:19:40 CET 2020";

	// define welcome message
    char *msg_welcome = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.";
    

	// length of the string with strlen
    write(1, msg_welcome, strlen(msg_welcome)); // fd = 1 is for Standard Output which is our terminal
    
	// show of simple prompt
    char *msg_prompt = "\nenseash %";
    write(1, msg_prompt, strlen(msg_prompt));

    while(1){ //define an infinite loop
        // waiting for an user to write a command 
        read(0, buffer, BUFSIZE); //storing message input in buffer 

        // check if the input is fortune if yes write fortune message, 7 is the size of "fortune"
        if (strncmp(buffer, "fortune", 7) == 0){
            write(1, msg_fortune, strlen(msg_fortune));
        }

        // show the date if input is "date"
        if (strncmp(buffer, "date", 4) == 0){
			write(1, msg_date, strlen(msg_date));
		}
		// output of the prompt again 
        write(1, msg_prompt, strlen(msg_prompt));
		}		
	}


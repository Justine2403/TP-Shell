#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

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



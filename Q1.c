#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define PROMPT "\nenseash % \n"

int print_message(char *output) {
	write(1, output, strlen(output)); //fd = 1 is for Standard Output which is our terminal	
}

int main (int argc, char **argv[]){

    //show welcome message
	print_message("Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.");
	//show of simple prompt
	print_message(PROMPT);

	return 0;
}



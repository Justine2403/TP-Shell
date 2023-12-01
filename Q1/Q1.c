#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char **argv[]){
    
    // define welcome message
	char *msg_welcome = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.";
	//length of the string with strlen
    write(1,msg_welcome,strlen(msg_welcome)); //fd = 1 is for Standard Output which is our terminal
	
	//show of simple prompt
	char *msg_prompt = "\nenseash %\n";
	write(1,msg_prompt, strlen(msg_prompt));
	
	return 0;
}
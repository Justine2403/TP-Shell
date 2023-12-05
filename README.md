# TP-Shell
Create a micro shell that displays the exit codes and execution times of launched programs.

## 1. Display a welcome message, followed by a simple prompt.: 

To display the welcome message and the prompt messages, we use the **write** function.
Let's explain what this function does in detail:

- **Write** takes 3 arguments: an integer 0 or 1 if the message/string is an input 0 or output 1 (in our case: 1), a string containing the message to be displayed, and the size of the message, which can be obtained by calling the strlen function.

When we compile the C file containing this program, we name its executable "enseash" using the command gcc -o enseash Q1.c. We have successfully created a microshell that displays a prompt message and a welcome message.

## 2. Execution of the entered command and return to the prompt (REPL : read–eval–print loop):
### a. Read the command entered by user: 
We want to import into our microshell the commands already defined for our terminal, such as **fortune** and **date**.
For this, we create a child process inside an infinite while loop using the **fork** function, which returns 0 if we are in the child process and 1 if we are back in the **parent process**. This method is used to create our own program in our new microshell "enseash".

Let's explain how we do this:

- In a while(1) loop, we first store the command written by the user after launching "enseash" (cf. Q1) and read it using the **read** function (same arguments as write).
- We then call the fork function and store its result in a variable of type pid_t called pid.
- If we are in the child process (pid==0), we use the execlp function, which reads the command entered by the user and identifies it with an existing command in the terminal, and applies it.
- If we are in the parent process, we wait for the user to enter a command.
    
### b. Execute this command (simple command for the moment, without argument) :
So far, we have managed to execute commands in our microshell such as fortune and date. These commands take no arguments.

### c. Print the prompt enseash % and waits for a new command :
In the same loop, we call a function defined in the functions.h document (See folder Q2). This function is intended to write the prompt message using write; it is called before storing the child process and once we have exited the child process and are waiting for a new command.
 
## 3. Management of the shell output with the command ”exit” or with <ctrl>+d : 
To exit the microshell, you need to break the loop with the **exit(EXIT_SUCCESS)** function in our program.
As we did for the prompt message, we create a function that displays a bye-bye message, which we will call later.

- **Exiting the microshell with the exit command :**
If the command entered by the user is exit, we exit the microshell. This is done using the execlp function, which compares the entered command with exit, which already exists in the terminal. If we are in this case, the bye-bye message function is called, and we break the loop with exit(EXIT_SUCCESS).

- **Exiting the microshell avec ctrl+d :**
To know if the user used these two keys, we store in a variable "bytesRead" the output of the read(0, command, 49) function call (command is the string that stores the command entered by the user). If bytesRead is equal to 0, then the user did click on <ctrl>+D. In this case, we call the function that displays the bye-bye message, and we exit the while loop.

## 4. Display the return code (or signal) of the previous command in the prompt :
We want to display the state of the compiled program before each prompt message.

- If the child process of the last compiled program terminates normally, we display in the prompt message **enseash [exit:0]**.
- If the child process of the last program is stopped by a signal, we display in the prompt message **enseash [sign:n]** with n being the number of the signal used to stop the child process of the last program.
- We use the **WIFEXITED** function to find out how the child process terminated.

## 5. Measurement of the command execution time using the call clock_gettime :

In addition to the exit or signal return on the last compiled program, we want to add the execution time.
For this, we use the **clock_gettime** function and call it in the function that displays the prompt message.

Let's explain in detail what the different clock_gettime functions do:

- clock_gettime(CLOCK_REALTIME, &start) gets the current system time and stores it in the start structure.
- clock_gettime(CLOCK_REALTIME, &end) uses clock_gettime again to get the current time and stores it in the end structure after the child process has finished execution.
- waitpid(pid, &status, 0) waits for the child process identified by PID (pid) to terminate. During this wait, the parent process is blocked. The waitpid function stores the exit status of the child process in the status variable.
- We then calculate the time difference; it is calculated in milliseconds using the tv_sec (seconds) and tv_nsec (nanoseconds) fields of the start and end structures. This difference is stored in the time_diff variable.
- We then implement this execution time in the prompt_message to display it in our shell.

## 6. Execution of a complex command (with arguments) :
In this part, we aim to execute a command with arguments in the shell.

- For this, we will create a token function that extracts the first token from the command string. **strtok** is used to split a string into tokens based on a specified delimiter. In this case, the delimiter is a space (" ").
- We create an array of character pointers (args) to store the tokens. MAX_SIZE is the maximum number of tokens we can handle.
- The arg_count variable is used to track the number of tokens.
- We then run a loop that continues until there are no more tokens. The condition checks if the current token is not NULL.

This function will be implemented in the if statement.

- We replace the execlp(command, command, NULL) function with execvp(args[0], args), which executes the command with arguments.

## 7. Management of redirections to stdin and stdout with ’<’ and ’>’ :

In this part, the goal is to handle redirection of inputs (<) and outputs (>) for commands entered in the microshell.
We first include <fcntl.h>.
We define two functions for input and output.
For input, we perform the following steps:

- Find the < symbol:
	strchr searches for the first occurrence of the < character in the command string. If found, it returns a pointer to that position.

- Adjust the command string:
	*input_redirect = '\0';: Terminates the command string at the position of <.
	input_redirect++;: Moves the pointer to the filename after <.

- Ignore spaces after <:	
	while (*input_redirect == ' ') { input_redirect++; }: Ignores any spaces after <. This ensures that there are no additional spaces between < and the filename.

- Open the file for reading:
	int fd = open(input_redirect, O_RDONLY); :Open the file specified after < in read-only mode (O_RDONLY).
Check if the file opening is successful. If not, display an error message and exit.

- Redirect stdin to the file:
	Redirect the file descriptor fd (representing the opened file) to standard input (file descriptor 0).
	Close the file descriptor as it is no longer needed.

The same procedure is applied for output redirection, but with stdout.

## 8. Management of pipe redirection with ‘|’ :

We want to enable the shell to handle pipe redirection, allowing commands like ls | wc -l to work as expected. 
To make it work, we need  perform the following steps:

- Tokenizing for Pipes:
This part tokenizes the command to identify the presence of the pipe (|). When a pipe is found, it sets up pipes and executes commands on each side.

- Creating Pipes:
It creates a pipe using the pipe system call. The array pipe_fd holds the file descriptors for the read and write ends of the pipe.

- Forking for Left Side of the Pipe:
In the child process, it sets up redirection to write to the pipe "STDOUT_FILENO" is redirected to the write end of the pipe), and then it executes the command on the left side of the pipe using tokenize_execute().

- Forking for Right Side of the Pipe:
In the parent process, it sets up redirection to read from the pipe "STDIN_FILENO" is redirected to the read end of the pipe), and then it executes the command on the right side of the pipe using tokenize_execute(). Finally, it waits for both child processes to finish, and it resets stdin to the terminal.



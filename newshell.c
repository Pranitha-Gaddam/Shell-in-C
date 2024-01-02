#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "path.h" 

#define MAX_INPUT_SIZE 512

pid_t shell_pgid;
pid_t subprocess_pid;

// Function prototypes
void processInput(FILE *input);
void parseCommand(char *command);
void executeCommand(char **userCmd);
void errorCase(const char *message);
void handle_signal(int signo);
void set_signal_handlers();

int main(int argc, char *argv[]) {
    while (1) {
        set_signal_handlers(); // Set up signal handlers for Ctrl-C and Ctrl-Z
        if (argc == 1) {
            printf("myshell> ");
            processInput(stdin); // Process user input in interactive mode
        } else if (argc == 2) {
            FILE *file = fopen(argv[1], "r");
            if (file == NULL) {
                perror("Error opening file");
                return 1;
            }
            processInput(file); // Process commands from the file
            fclose(file);
            resetPath(); //reset path to original path
	    exit(EXIT_SUCCESS);
        } else {
            errorCase("Usage: myshell [optional_filename]\n");
        }
    }
    resetPath(); // Reset the path before exiting
    return 0;
}

//signal handlers to implement signal control 
void set_signal_handlers() {
    signal(SIGINT, handle_signal);
    signal(SIGTSTP, handle_signal);
}

void handle_signal(int signo) {
    if (signo == SIGINT) {
        // Handle Ctrl-C in the shell
        if (subprocess_pid != 0) {
            kill(subprocess_pid, SIGTERM); // Terminate subprocess if running
            waitpid(subprocess_pid, NULL, 0);
        }
        printf("\nCtrl-C received in the shell.\n");
        fflush(stdout);
    } else if (signo == SIGTSTP) {
        // Handle Ctrl-Z in the shell
        if (subprocess_pid != 0) {
            kill(subprocess_pid, SIGTERM); // Terminate subprocess if running
            waitpid(subprocess_pid, NULL, 0);
        }
        printf("\nCtrl-Z received in the shell.\n");
    }
}
//divides input line into different commands according to ";"
void processInput(FILE *input) {
    char inputLine[MAX_INPUT_SIZE];
    while (fgets(inputLine, sizeof(inputLine), input) != NULL) {
        // Remove newline character if present
        inputLine[strcspn(inputLine, "\n")] = '\0';

        if (inputLine == NULL) {
            if (feof(input)) {
                printf("Exiting shell\n");
                exit(EXIT_SUCCESS);
            } else if (strcmp(inputLine, "") == 0) {
                return; // Skip empty lines
            } else {
                errorCase("Error reading input");
            }
        }

        char *command = strtok(inputLine, ";");
        if (command == NULL) {
            return;
        }

        char *cmdList[512];
        int i = 0;
        while (command != NULL && i < 512) {
            cmdList[i] = command;
            command = strtok(NULL, ";");
            i++;
        }

        for (int j = 0; j < i; j++) {
            parseCommand(cmdList[j]);
        }
    }
}
//parse function that divides user command and places the arguments into an array
void parseCommand(char *command) {
    char *arg = strtok(command, " \t\n");
    if (arg == NULL) {
        return; // Skip if no command is found
    }

    char *userCmd[512];
    int i = 0;
    while (arg != NULL && i < 512) {
        userCmd[i] = arg;
        arg = strtok(NULL, " \t\n");
        i++;
    }

    userCmd[i] = NULL;
    executeCommand(userCmd);
}

//function to execute built it as well as system commands
void executeCommand(char **userCmd) {
    if (strcmp(userCmd[0], "cd") == 0) {
        // cd command;
        printf("cd function called\n");
    } else if (strcmp(userCmd[0], "exit") == 0) {
        // exit command
	printf("exit function called\n");
	    
	int argNum = 0;
	    while(userCmd[argNum] != NULL) {
                argNum++;
        }
	if (argNum > 1) {
            printf("invalid command.\n");
	}
	else {
	printf("exiting..\n");
	exit(0);
	}	
}
else if (strcmp(userCmd[0], "path") == 0) {
        // path command
        execPath(userCmd); // Execute path command
    } else {
        pid_t pid = fork();

        if (pid == -1) {
            errorCase("Failed to fork");
        } else if (pid == 0) {
            // Child process
            if (execvp(userCmd[0], userCmd) < 0) {
                perror("\n Could not execute system command");
                resetPath();
                exit(EXIT_FAILURE);
            }
            exit(0);
        } else {
            // Parent process
            subprocess_pid = pid; // Store the subprocess PID
            wait(NULL); // Wait for the child process to complete
            subprocess_pid = 0; // Reset subprocess PID
            return;
        }
    }
}

//handles error cases and exits gracefully
void errorCase(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    resetPath(); //reset path to original path before exiting
    exit(EXIT_FAILURE);
}


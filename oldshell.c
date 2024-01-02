#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "path.c"

#define MAX_INPUT_SIZE 512

pid_t shell_pgid;
pid_t subprocess_pid;

void processInput(FILE *input);
void parseCommand(char *command);
void executeCommand(char **userCmd);

void errorCase(const char *message) {
        fprintf(stderr, "Error: %s\n", message);
        resetPath();
	exit(EXIT_FAILURE);
}

void handle_signal(int signo) {
    if (signo == SIGINT) {
        // Handle Ctrl-C in the shell
         if (subprocess_pid != 0) {
                kill(subprocess_pid, SIGTERM);
                waitpid(subprocess_pid, NULL, 0);
        }
	    printf("\nCtrl-C received in the shell.\n");
        fflush(stdout);
    } else if (signo == SIGTSTP) {
        // Handle Ctrl-Z in the shell
         if (subprocess_pid != 0) {
                kill(subprocess_pid, SIGTERM);
                waitpid(subprocess_pid, NULL, 0);
        }
	    printf("\nCtrl-Z ignored in the shell.\n");
    }
}

void set_signal_handlers() {
    signal(SIGINT, handle_signal);
    signal(SIGTSTP, handle_signal);
}
void processInput(FILE *input) {
        char inputLine[MAX_INPUT_SIZE];
        while(fgets(inputLine, sizeof(inputLine), input)!=NULL){
        if (inputLine == NULL) {
                if (feof(input)) {
                        printf("Exiting shell\n");
                exit(EXIT_SUCCESS);
                }
		else if (strcmp(inputLine, "") == 0) {
                        //free(inputLine);
                        return;
                }

                else {
                        errorCase("Error reading input");
                }
        }

        char *command = strtok(inputLine, ";");
        if (command == NULL) {
                return;
        }

        char *cmdList[512];
        int i=0;
        while (command != NULL && i<512) {
                cmdList[i] = command;
                command = strtok(NULL, ";");
                i++;
        }
        for (int j=0; j<i; j++) {
                parseCommand(cmdList[j]);
        }
	}
}
void parseCommand(char *command) {
        char *arg = strtok(command, " \t\n");
        if (arg == NULL) {
                return;
        }

        char *userCmd[512];
        int i = 0;
        while(arg != NULL && i<512) {
                userCmd[i] = arg;
                arg = strtok(NULL, " \t\n");
                i++;
        }

        userCmd[i] = NULL;
        executeCommand(userCmd);
}
void executeCommand(char **userCmd) {

        if(strcmp(userCmd[0], "cd") == 0) {
                //cd command;
                printf("cd worked\n");
                /*if (userCmd[1] == NULL) {
                        errorCase("Missing argument");
                }
                if (chdir(userCmd[1]) != 0) {
                        perror("cd");
                }*/
        }
        else if(strcmp(userCmd[0], "exit") == 0) {
                //exit command
                printf("exit worked");
        }
        else if(strcmp(userCmd[0], "path") == 0) {
                //path command
		execPath(userCmd);
                printf("path worked");
        }
        else {
                pid_t pid = fork();

                if (pid == -1) {
                        errorCase("Failed to fork");
                        //return;
                }
                else if (pid == 0) {
                        if (execvp(userCmd[0], userCmd) < 0) {
                                perror("\n Could not execute system command");
                                resetPath();
				exit(EXIT_FAILURE);
                        }
                        exit(0);
                }
                else {
                        subprocess_pid = pid;
			wait(NULL);
			subprocess_pid = 0;
                        return;
                }
        }
}
int main(int argc, char *argv[]) {
        while (1) {
        set_signal_handlers();
        if (strcmp(argv[0], "stop") == 0) {
                exit(0);
        }
        if (argc == 1) {
                printf("myshell> ");
                processInput(stdin);
        }
        else if (argc == 2) {
                FILE *file = fopen(argv[1], "r");
                if (file == NULL) {
                        perror("Error opening file");
                        return 1;
                }
                processInput(file);
                fclose(file);
		resetPath();
		exit(EXIT_SUCCESS);
		
        }
        else {
                errorCase("Usage: myshell [optional_filename]\n");
        }
        }
	resetPath();
        return 0;
}


//WILL SEPARATE SIGNAL HANDLING FROM MAIN FILE LATER


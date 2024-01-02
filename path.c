#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "path.h"

#define MAX_PATH 1024

char shellPath[MAX_PATH];
char origPath[MAX_PATH];
int i=0;

void resetPath() {
	setenv("PATH", origPath, 1);
}

void execPath(char **userCmd) {
	if (i==0) {
	strcpy(origPath, getenv("PATH"));
	i++;
	}
	int argNum = 0;
        while(userCmd[argNum] != NULL) {
                argNum++;
        }
        if (getenv("PATH") == NULL) {
                printf("PATH environment variable not set.\n");
                return;
        }
        strcpy(shellPath, getenv("PATH"));
        if (argNum == 1) {
                printf("%s\n", shellPath);
                return;
        }
        else if (argNum == 3 && (strcmp(userCmd[1], "+") == 0)) {
                strcat(shellPath, ":");
                strcat(shellPath, userCmd[2]);
		//putenv(shellPath);
		setenv("PATH", shellPath, 1);
        }
        else if (argNum == 3 && (strcmp(userCmd[1], "-") == 0)) {
                char *remove =  strstr(shellPath, userCmd[2]);
                if (remove != NULL) {
                //memmove(remove, remove + strlen(userCmd[2])+1, strlen(remove) + strlen(userCmd[2])+1)+1;
		remove = memmove(remove, remove + strlen(userCmd[2]) + 1, strlen(remove) + strlen(userCmd[2]) + 1) + 1;

		setenv("PATH", shellPath, 1);
        }
                else {
                        printf("%s noT found in path.\n", userCmd[2]);
                }
        }
                else {
                printf("Invalid command, try again.\n");
        }
}


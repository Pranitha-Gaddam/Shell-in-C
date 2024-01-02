# Shell-in-C

The shell implements two different functionalities: 
1. `path` as a built in command
2. Signal handling

The shell project is divided into two compoments: `newshell.c` and `path.c`. The signal handling functionality was implemented within `newshell.c`.

newshell.c: This file contains the main logic for the shell, including reading input, parsing commands, and executing them. It also handles signal handling and the overall flow of the shell.
->Signal handling: Ctrl C and Ctrl Z were handled in the shell such that the subprocesses are terminated, but the shell continues to run

`path.c`: Manages the PATH environment variable. It includes functions for resetting the PATH to its original state and executing the 'path' command to display or modify the PATH.

<h3>Complete Specification</h3>
The shell functions under both interactive and batch file modes. In terms of defensive programming, it handles lines with no commands between semi-colons by skipping over them. Empty lines are also ignored. However, Ctrl D was not implemented. To exit the shell, type "exit" into the shell.

USAGE:
<li>To compile with the makefile, type: make
<li>To run in interactive mode, type: ./newshell 
<li>To run in batch mode, type: ./newshell [filename]
<li>To exit program, type: exit
<li>To clean, type: make clean

IMPORTANT: In shell, after every command line argument is executed, user will need to press enter on keyboard to receive prompt for next argument.

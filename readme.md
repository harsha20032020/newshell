## Newshell

#### This is a primitive shell written in C.
To run it 
```c
make
./a.out
```
The 
[Shell.c](./shell.c)  :  Is the main file which has the main function.
The utilities folder contains all the utility files and functions.

[Execute.c](./utilities/execute.c) : Contains logic for the categorization of inputs and various other commands like cd , echo , repeat and also a helper functions to other commands.

[ls.c](./utilities/ls.c) : contains the functions for different ls commands and even has a custom function to detect different flags and work on them.
[name.c](./utilities/name.c) : contains the logic for printing the prompt and the path accounting for tilda.

[parser.c](./utilities/parser.c) : contains the logic for parsing the line inputted into the shell and also contains the necessary steps to implement piping and helper functions to pipe redirection.

[processes.c](./utilities/processes.c): contains code for the foreground and background processes as well as detecting killing of a child process and other functions regarding the addition of jobs and manipulataion of the jobs linked list.

[signals.c](./utilities/signals.c): contains the appropriate code for signal handling.

[file.c](./utilities/file.c): contains function for I/O file redirections.

[list.c](./utilities/list.c): contains function for manipulation of linked lists used for jobs.

This shell in entirety can run various bash commands and even custom made commands like replay(runs a command after a specific interval usage format : "replay -command echo "hi" -interval 3 -period 6"), pinfo which returns the deatils of the process and many more.This shell also supports file redirection and Piping. 

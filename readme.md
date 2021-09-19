## Newshell

#### This is a primitive shell written in C.
To run it 
```c
make
./a.out
```
The 
```
Shell.c  :  Is the main file which has the main function.
The utilities folder contains all the utility files and functions.
Execute.c : Contains logic for the categorization of inputs and various other commands like cd , echo , repeat and also a helper functions to other commands.
ls.c : contains the functions for different ls commands and even has a custom function to detect different flags and work on them.
name.c : contains the logic for printing the prompt and the path accounting for tilda.
parser.c : contains the logic for parsing the line inputted into the shell
processes.c: contains code for the foreground and background processes as well as detecting killing of a child process.
```

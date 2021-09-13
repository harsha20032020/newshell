#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#define LINE_LENGTH 1024
#include "./utilities/name.h"
#include "./utilities/parser.h"
#include "./utilities/execute.h"
int main()
{
    //printdir();
    char *initial_dir = getcwd(NULL, 0);
    int initiallen = strlen(initial_dir);
    while(1)
    {
        //printf("%s\n", initial_dir);
        username(initiallen);                           //prints the default prompt message
        readcmd();
        // size_t characters = getline(string_pointer, &size, stdin);
        // printf("You typed: %s", string);
        //splitcommands(string);
        //printf("%s\n", path);
        //printpath();
    }
}
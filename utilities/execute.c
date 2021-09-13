#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/utsname.h>
#include "execute.h"
#include <limits.h>

void executioner(char *command[100], int len)
{
    char currentdir[PATH_MAX];
    getcwd(currentdir, sizeof(currentdir));
    if (strcmp(command[0], "echo") == 0)
    {
        echocommand(command, len);
    }
    else if (strcmp(command[0], "pwd") == 0)
    {
        printf("\033[0;33m"); 
        printf("%s\n", currentdir);
        printf("\033[0m");
    }
    else if (strcmp(command[0], "cd") == 0)
    {
        cdcommand(command, len);
    }
}
void echocommand(char *command[100], int len)
{
    for (int i = 1; i < len; i++)
    {
        printf("%s ", command[i]);
    }
    printf("\n");
}
void cdcommand(char *command[100], int len)
{
    char currentdir[PATH_MAX];
    getcwd(currentdir, sizeof(currentdir));
    strcat(currentdir, "/");
    strcat(currentdir,command[1]);
    if (chdir(currentdir) == 0)
    {
        //printf("cd successful\n");
        //printf("%s\n", currentdir);
        //printf("%s\n",basename(currentdir));
    }
    else
    {
        printf("cd failed\n");
    }
}
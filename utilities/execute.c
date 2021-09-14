#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/utsname.h>
#include <time.h>
#include "execute.h"
#include "name.h"
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "process.h"
void temp(char *str, int i, int j)
{
    printf("\033[1;34m");
    printf("~");
    if (i > j)
    {
        printf("%s", str);
    }
    else
    {
        for (int k = i; k < j; k++)
        {
            printf("%c", str[k]);
        }
    }
    printf("\033[0m");
    //printf("$ ");
}
void executioner(char *command[100], int len)
{
    char currentdir[PATH_MAX];
    getcwd(currentdir, sizeof(currentdir));
    child();
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
    else if (strcmp(command[0], "repeat") == 0)
    {
        int n = atoi(command[1]);
        repeat(command, n, len);
    }
    else if (strcmp(command[0], "pinfo") == 0 && len == 1)
    {
        pinfo();
    }
    else if (strcmp(command[0], "pinfo") == 0 && len == 2)
    {
        pinfo2(atoi(command[1]));
    }
    else
    {
        processes(command, len);
    }
    //free(command);
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
    char prev[PATH_MAX];
    /*fail if prev is NULL, do something*/
    //printf("%s\n",prev);
    if (command[1][0] == '~')
    {
        char *home = getenv("HOME");
        strcpy(currentdir, home);
        if (chdir(home) == 0)
        {
        }
        else
        {
            printf("cd: %s: No such file or directory\n", command[1]);
        }
    }
    else if (command[1][0] == '-')
    {
        if (chdir(prev) == 0)
        {
            printf("1:%s\n2:%s\n", prev, currentdir);
        }
        else
        {
            printf("cd: %s: No such file or directory\n", command[1]);
        }
    }
    else
    {
        strcat(currentdir, "/");
        strcat(currentdir, command[1]);
        if (chdir(currentdir) == 0)
        {
            //printf("cd successful\n");
            //printf("%s\n", currentdir);
            //printf("%s\n",basename(currentdir));
        }
        else
        {
            printf("cd: %s: No such file or directory\n", command[1]);
        }
    }
    strcpy(prev, currentdir);
}
void processes(char *command[100], int len)
{
    if (command[len - 1][0] == '&')
    {
        background(command, len);
    }
    else
    {
        foreground(command, len);
    }
}
void repeat(char *commands[100], int n, int len)
{
    char *newstring[100];
    for (int i = 2; i < len; i++)
    {
        newstring[i - 2] = commands[i];
    }
    for (int i = 0; i < n; i++)
    {
        executioner(newstring, len - 2);
    }
}
void pinfo()
{
    pid_t pid;
    pid = getpid();
    char path[50];
    sprintf(path, "/proc/%d/stat", pid);
    printf("pid--   %d\n", pid);
    int fd = open(path, O_RDONLY);
    int i = 1;
    char buffer[1000];
    read(fd, buffer, 1000);
    char *token = strtok(buffer, " ");
    char *state;
    int vsize;
    while (token != NULL)
    {
        if (i == 3)
        {
            state = token;
        }
        if (i == 23)
        {
            vsize = atoi(token);
        }
        //printf("%d %s\n",i,token);
        token = strtok(NULL, " ");
        i++;
    }
    printf("Process State--   %s\n", state);
    printf("Virtual Memory Size(in bytes)--   %d\n", vsize);
    close(fd);
    char exec[200];
    sprintf(path, "/proc/%d/exe", pid);
    readlink(path, exec, sizeof(exec));
    char wd[200];
    getcwd(wd, sizeof(wd));
    printf("Executable Path--   ");
    temp(exec,strlen(wd),strlen(exec));
    printf("\n");
}
void pinfo2(int pid)
{
    // pid_t pid;
    // pid = getpid();
    char path[50];
    sprintf(path, "/proc/%d/stat", pid);
    printf("pid--   %d\n", pid);
    int fd = open(path, O_RDONLY);
    int i = 1;
    char buffer[1000];
    read(fd, buffer, 1000);
    char *token = strtok(buffer, " ");
    char *state;
    int vsize;
    while (token != NULL)
    {
        if (i == 3)
        {
            state = token;
        }
        if (i == 23)
        {
            vsize = atoi(token);
        }
        //printf("%d %s\n",i,token);
        token = strtok(NULL, " ");
        i++;
    }
    printf("Process State--   %s\n", state);
    printf("Virtual Memory Size(in bytes)--   %d\n", vsize);
    close(fd);
    char exec[200];
    sprintf(path, "/proc/%d/exe", pid);
    readlink(path, exec, sizeof(exec));
    char wd[200];
    getcwd(wd, sizeof(wd));
    printf("Executable Path--   ");
    temp(exec,strlen(wd),strlen(exec));
    printf("\n");
}
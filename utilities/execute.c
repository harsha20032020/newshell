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
    else
    {
        processes(command, len);
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
        command[len - 1] = NULL;
        pid_t pid, newpid;
        pid = fork();
        if (pid != 0)
        {
            printf("%d\n", pid);
        }
        if (pid == 0)
        {
            //newpid = getpid();
            //printf("%d\n", newpid);
            if (execvp(command[0], command) == -1) //runs the command
            {
                perror("Error At Background Pocesses");
            }
            exit(1); 
        }
    }
    else
    {
        int status;   //status of the process
        pid_t pid, newpid; //pid of the process
        pid = fork();   //forking the process
        // if (pid != 0)
        // {
        //     printf("%d\n", pid);
        // }
        if (pid == 0)
        {
            //newpid = getpid();
            //printf("%d\n", newpid);
            if (execvp(command[0], command) == -1) //command[0] is the name of the program
            {
                perror("Error At Background Pocesses");
            }
            exit(1);
        }
        else
        {
            do
            {
                waitpid(pid, &status, WUNTRACED); //wait for child process to finish
            } while (!WIFEXITED(status) && !WIFSIGNALED(status)); //wait until child process is done
        }
    }
}
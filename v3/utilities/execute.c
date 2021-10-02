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
#include "parser.h"
#include "dirent.h"
#include "ls.h"
char backg[PATH_MAX];
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
void executioner(char *command[100], int len,char initial_dir[1024])
{
    char currentdir[PATH_MAX];
    getcwd(currentdir, sizeof(currentdir));
    child(backg);
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
        cdcommand(command, len, initial_dir);
    }
    else if (strcmp(command[0], "repeat") == 0)
    {
        int n = atoi(command[1]);
        repeat(command, n, len, initial_dir);
    }
    else if (strcmp(command[0], "pinfo") == 0 && len == 1)
    {
        pinfo();
    }
    else if (strcmp(command[0], "pinfo") == 0 && len == 2)
    {
        pinfo2(atoi(command[1]));
    }
    else if (strcmp(command[0], "history") == 0 && len == 1)
    {
        history();
    }
    else if (strcmp(command[0], "history") == 0 && len == 2)
    {
        complexhistory(atoi(command[1]));
    }
    else if (strcmp(command[0], "ls") == 0)
    {
        lscommand(command, len, initial_dir);
    }
    else if (strcmp(command[0],"jobs")==0)
    {
        //printf("%d\n",len);
        if(len==2)
        {
            if(strcmp(command[1],"-r")==0)
            {
                running();
            }
            else if(strcmp(command[1],"-s")==0)
            {
                terminated();
            }
            else
            {
                printf("Enter A Valid Flag\n");
            }
        }
        else
        {
            display();
        }
    }
    else if(strcmp(command[0],"sig")==0)
    {
        kill_process(atoi(command[1]),atoi(command[2]));
    }
    else if(strcmp(command[0],"fg")==0)
    {
        foreground_process(atoi(command[1]));
    }
    else if(strcmp(command[0],"bg")==0)
    {
        background_process(atoi(command[1]));
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
void cdcommand(char *command[100], int len,char initial_dir[1024])
{
    char currentdir[PATH_MAX];
    getcwd(currentdir, sizeof(currentdir));
    char prev[PATH_MAX];
    char temp[PATH_MAX];
    strcpy(temp, currentdir);
    /*fail if prev is NULL, do something*/
    //printf("%s\n",prev);
    // if (len == 1)
    // {
    //     printf("this came\n");
    //     char currentdir[PATH_MAX];
    //     getcwd(currentdir, sizeof(currentdir));
    //     chdir(currentdir);
    // }
    if (len == 1)
    {
        //char *home ;
        //= getenv("HOME");
        strcpy(currentdir, initial_dir);
        if (chdir(currentdir) == 0)
        {
        }
        else
        {
            printf("cd: %s: No such file or directory\n", command[1]);
        }
    }
    else if (command[1][0] == '~')
    {
        strcpy(currentdir, initial_dir);
        if (chdir(currentdir) == 0)
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
            //printf("1:%s\n2:%s\n", prev, currentdir);
        }
        else
        {
            printf("cd: %s: No such file or directory\n", command[1]);
        }
    }
    else if (len > 2)
    {
        printf("cd cannot have more than 2 arguments\n");
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
    strcpy(prev, temp);
}
void processes(char *command[100], int len)
{
    if (command[len - 1][0] == '&')
    {
        strcpy(backg,command[0]);
        background(command, len);
    }
    else
    {
        foreground(command, len);
    }
}
void repeat(char *commands[100], int n, int len,char initial_dir[1024])
{
    char *newstring[100];
    for (int i = 2; i < len; i++)
    {
        newstring[i - 2] = commands[i];
    }
    for (int i = 0; i < n; i++)
    {
        executioner(newstring, len - 2,initial_dir);
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
    if (fd != -1)
    {
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
        if(getpgid(pid) == pid)
        {
            printf("Process State--   +%s\n", state);
        }
        else
        {
            printf("Process State--   %s\n", state);
        }
        printf("Virtual Memory Size(in bytes)--   %d\n", vsize);
        close(fd);
        char exec[200];
        sprintf(path, "/proc/%d/exe", pid);
        int l=readlink(path, exec, sizeof(exec));
        exec[l] = '\0';
        char wd[200];
        getcwd(wd, sizeof(wd));
        printf("Executable Path--   ");
        temp(exec, strlen(wd), strlen(exec));
        printf("\n");
    }
    else
    {
        perror("Process Unknown");
    }
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
    if (fd != -1)
    {
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
        int l=readlink(path, exec, sizeof(exec));
        exec[l] = '\0';
        char wd[200];
        getcwd(wd, sizeof(wd));
        printf("Executable Path--   ");
        temp(exec, strlen(wd), strlen(exec));
        printf("\n");
    }
    else
    {
        perror("Please check process pid");
    }
}

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
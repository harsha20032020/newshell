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
void background(char *command[100], int len)
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
            perror("Error At Background Processes");
        }
        exit(1);
    }
}
void foreground(char *command[100], int len)
{
    int status;        //status of the process
    pid_t pid, newpid; //pid of the process
    pid = fork();      //forking the process
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
            perror("Error At Background Processes");
        }
        exit(1);
    }
    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);                 //wait for child process to finish
        } while (!WIFEXITED(status) && !WIFSIGNALED(status)); //wait until child process is done
    }
}
void child()
{
    //https://stackoverflow.com/questions/4200373/just-check-status-process-in-c
	int status;
	pid_t pid;
	pid = waitpid(-1, &status, WNOHANG);
	if (pid > 0)
	{
		fprintf(stderr, "pid %d exited Normally\n", pid);
        fflush(stderr);
	}
}
// idk why but looks and runs perfect ig
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
#include "list.h"
int i = 1;
//int vartemp;
void background(char *command[100], int len)
{
    //int vartemp;
    //printf("from proc %d\n",vartemp);
    //struct node *listglobal;
    command[len - 1] = NULL;
    pid_t pid, newpid;
    pid = fork();
    setpgid(0, 0);
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
    insert_node(listglobal, command[0], "Running", pid, i, command, len);
    i++;
    //printf("Ran successfully\n");
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
            perror("Error At Foreground Processes");
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
void child(char back[PATH_MAX])
{
    //https://stackoverflow.com/questions/4200373/just-check-status-process-in-c
    int status;
    pid_t pid;
    pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0)
    {
        if (WIFEXITED(status))
        {
            printf("Child %s with pid %d exited normally with exit code=%d\n", back, pid, WEXITSTATUS(status));
        }
        if (WIFSIGNALED(status))
        {
            printf("Child %s with pid %d exited abnormally via signal %d\n", back, pid, WTERMSIG(status));
        }
    }
    // else if (pid > 0 && WIFSIGNALED(status))
    // {
    //     fprintf(stderr, "pid %d exited on signal %d\n", pid, WTERMSIG(status));
    // }
    // else if (pid > 0 && WIFSTOPPED(status))
    // {
    //     fprintf(stderr, "pid %d stopped by signal %d\n", pid, WSTOPSIG(status));
    // }
    // else if (pid > 0 && WIFCONTINUED(status))
    // {
    //     fprintf(stderr, "pid %d continued\n", pid);
    // }
}
void display()
{
    print_list(listglobal);
}
void running()
{
    runningprintlist(listglobal);
}
void terminated()
{
    terminatedprintlist(listglobal);
}
void kill_process(int a, int b)
{
    int pid = find_process_by_index(listglobal, a);
    if (pid != -1)
    {
        kill(pid, b);
    }
    else
    {
        printf("No such process\n");
    }
}
void foreground_process(int a)
{
    int pid = find_process_by_index(listglobal, a);
    if (pid != -1)
    {
        printf("The process is successfully converted\n");
        pid_t wpid;
        int status;
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    else
    {
        printf("No such process\n");
    }
}
void background_process(int a)
{
    int pid = find_process_by_index(listglobal, a);
    if (pid != -1)
    {
        struct node* temp=find_process_struc(listglobal,a);
        //printf("Here %s %d %d\n",temp->status,temp->pid,temp->index);
        if(strcmp(temp->status,"Running")==0)
        {
            printf("The process is already running\n");
        }
        else
        {
            // temp->status="Running";
            // printf("This Bloc ran %s\n",temp->status);
            kill(pid, SIGTTIN);
			kill(pid, SIGCONT);
        }
    }
    else
    {
        printf("No such process\n");
    }
}
// idk why but looks and runs perfect ig
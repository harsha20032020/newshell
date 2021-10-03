#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include "list.h"
// Handler for SIGINT, caused by
// Ctrl-C at keyboard
void handle_sigint(int sig)
{
    printf("Caught signal %d\n", sig);
}
void ctrlC()
{

	if (getpid() != getppid())
		return;

	if (getpid() != 0)
	{
		fprintf(stdout, "\nSending SIGINT to %d.\n", getpid());
		kill(getpid(), SIGINT);
	}
	signal(SIGINT, ctrlC);
}
void ctrlZ()
{
    printf("ctrlz triggered\n");
    printf("%d %d\n",getpid(),getppid());
	if (getpid() != getppid())
		return;
	printf("%d,%d\n", getppid(), getpid());

	if (getpid() != getppid() && getpid() != 0)
	{
		struct node *temp = listglobal;
        while (temp != NULL)
        {
            if (temp->pid == getpid())
            {
                fprintf(stdout, "Sending SIGSTOP to %d.\n", temp->pid);
                //kill(temp->pid, SIGTSTP);
                temp->status="Sleeping";
                break;
            }
            temp = temp->next;
        }

		kill(getpid(), SIGSTOP);
		kill(getpid(), SIGCONT);
	}
	//signal(SIGTSTP, ctrlZ);
}
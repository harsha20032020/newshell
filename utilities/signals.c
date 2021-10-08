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
    // printf("%d %d\n",getpid(),getppid());
	// if (getpid() != getppid())
	// 	return;
	// printf("%d,%d\n", getppid(), getpid());

	// if (getpid() != getppid() && getpid() != 0)
	// {
	// 	char *command[100]={"Fore","to","back"};
	// 	insert_node(listglobal,"Foretoback","Running",getpid(),0,command,1);
	// 	kill(getpid(), SIGTTIN);
	// 	kill(getpid(), SIGTSTP);
	// }
	// signal(SIGTSTP, ctrlZ);
}
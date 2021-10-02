#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
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
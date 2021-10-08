#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include "parser.h"
#include "execute.h"
#include <fcntl.h>
int fileio(char *command[100], int limit,char initial_dir[1024])
{
	int in = 0, out = 0, dir = 0, total = 0;
	int i = 0;
	while (i < limit)
	{
		if (strcmp(command[i], "<") == 0)
		{
			total++;
			in = 1;
			dir = 1;
			printf("Input File %s redirection\n", command[i + 1]);
			inputio(command, limit, i,initial_dir);
		}
		else if (strcmp(command[i], ">") == 0)
		{
			total++;
			out = 1;
			dir = 1;
			printf("Output File %s redirection\n", command[i + 1]);
			outputio(command,limit,i,0,initial_dir);
		}
		else if (strcmp(command[i], ">>") == 0)
		{
			total++;
			out = 1;
			dir = 1;
			printf("Append Output File %s redirection\n", command[i + 1]);
			outputio(command,limit,i,1,initial_dir);
		}
		i++;
	}
	if(dir==0)
	{
		executioner(command,limit,initial_dir);
	}
	executioner(command,limit,initial_dir);
	return total;
}
void inputio(char *command[100], int limit, int n,char initial_dir[1024])
{
	//printf("n=%d s[n]=%s\n",n,command[n]);
	command[n] = NULL;
	char *file = command[n + 1];
	//printf("file=%s\n",file);
	int fd1 = open(file, O_RDONLY, 0644);
	//printf("%d\n", fd1);
	if (fd1 == -1)
	{
		perror("File opening failed");
	}
	close(0);
	if (dup2(fd1, 0) == -1)
	{
		perror("File Descriptor Duplication failed");
	}
	close(fd1);
	// for(int i = 0; i < limit; i++)
	// {
	// 	printf("%d %s\n",i,command[i]);
	// }
	//executioner(command,limit,initial_dir);
}
void outputio(char *command[100],int limit,int n,int flg,char initial_dir[1024])
{
	int fd1;
	if(flg==1)
	{
		fd1=open(command[n+1],O_WRONLY | O_RDONLY |O_APPEND | O_CREAT , 0644 );
	}
	else
	{
		fd1=open(command[n+1],O_WRONLY | O_RDONLY | O_CREAT , 0644 );
	}
	if(fd1<0)
	{
		perror("File Opening failed");
	}
	if(dup2(fd1,1)!=1)
	{
		perror("Dup2 error");
	}
	executioner(command,limit-2,initial_dir);
	close(fd1);
	freopen("/dev/tty","w",stdout);
}
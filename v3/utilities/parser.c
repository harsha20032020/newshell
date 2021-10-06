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
//int i = 1;
char prev[LINE_LENGTH];
char *readcmd(char initial_dir[1024])
{
	char *string;						  // character array pointer
	size_t size = 10;					  // initial size of char array
	string = malloc(size * sizeof(char)); // allocate memory for char array
	char **string_pointer = &string;	  // double pointer to char array
	size_t characters = getline(string_pointer, &size, stdin);
	if (characters == -1)
	{
		printf("\n");
		exit(1);
		//printf("Error. Please give input correctly.");
		//return readcmd(initial_dir);
	}
	writetofile(string);
	stringsplitter(string, initial_dir);
	return string;
}
void writetofile(char str[LINE_LENGTH])
{
	FILE *fptr;

	// use appropriate location if you are using MacOS or Linux
	fptr = fopen("history.txt", "a");
	//printf("pr=%sstr=%s",prev,str);
	if (strcmp(prev, str) != 0)
	{

		fprintf(fptr, "%s", str);
		strcpy(prev, str);
	}
	//fprintf(fptr, "%s", str);
	fclose(fptr);
}
int countlines()
{
	int count = 0;
	FILE *fptr;
	fptr = fopen("history.txt", "r");
	char c;
	while ((c = fgetc(fptr)) != EOF)
	{
		if (c == '\n')
		{
			count++;
		}
	}
	fclose(fptr);
	return count;
}
void history()
{
	FILE *fptr;
	char line[1000];
	fptr = fopen("history.txt", "r");
	int n = countlines();
	char comm[100];
	// while (fgets(line, sizeof(line), fptr) != 0)
	// {
	// 	printf("%s",line);
	// }
	printf("\033[0;95m");
	if (n > 10)
	{
		for (int i = 0; i < n; i++)
		{
			fgets(line, sizeof(line), fptr);
			if (i > n - 10)
			{
				printf("%s", line);
			}
		}
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			fgets(line, sizeof(line), fptr);
			printf("%s", line);
		}
	}
	printf("\033[0m");
}
void complexhistory(int x)
{
	FILE *fptr;
	char line[1000];
	fptr = fopen("history.txt", "r");
	int n = countlines();
	printf("\033[0;95m");
	for (int i = 0; i < n; i++)
	{
		fgets(line, sizeof(line), fptr);
		if (i >= n - x)
		{
			printf("%s", line);
		}
	}
	printf("\033[0m");
}
void stringsplitter(char str[LINE_LENGTH], char initial_dir[1024])
{
	char *delimiter = " \n"; // the first delimiter
	char *delimiter1 = ";";	 // the second delimiter
	char *ptr, *ptr1;		 //
	int limit1 = 0;
	char *word, *word1;
	char *commands[100];
	//commands=malloc(sizeof(char)*100*100);
	word1 = strtok_r(str, delimiter1, &ptr1);
	int flg = 0;
	while (word1 != NULL)
	{

		int limit = 0;
		word = strtok_r(word1, delimiter, &ptr);
		// int fdh = open("history.txt", O_RDWR | O_CREAT);
		while (word != NULL)
		{
			if (strcmp(word, ">") == 0 || strcmp(word, "<") == 0 || strcmp(word, ">>") == 0)
			{
				flg = 1;
			}
			commands[limit++] = word;
			// write(fdh,word,strlen(word));
			word = strtok_r(NULL, delimiter, &ptr);
		}

		word1 = strtok_r(NULL, delimiter1, &ptr1);
		// for (int i = 0; i < limit; i++)
		// {
		// 	write(fdh, commands, strlen(commands[i]));
		// }
		for (int i = limit; i < 100; i++)
		{
			commands[i] = NULL;
		}
		if (flg == 1)
		{
			int temp = fileio(commands, limit,initial_dir);
			//printf("temp value = %d\n", temp);
		}
		if (flg == 0)
		{
			executioner(commands, limit, initial_dir);
		}
		//printf("%s\n%s\n%d\n", commands[0], commands[1], limit);
	}
}
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
	if (dup2(fd1, 0) == -1)
	{
		perror("File Descriptor Duplication failed");
	}
	close(fd1);
	// for(int i = 0; i < limit; i++)
	// {
	// 	printf("%d %s\n",i,command[i]);
	// }
	executioner(command,limit,initial_dir);
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
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
#include "file.h"
#include <sys/types.h>
#include <sys/wait.h>

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
	if (strrchr(str, '|') != NULL)
	{
		pipes(str, initial_dir);
	}
	else
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
				int temp = fileio(commands, limit, initial_dir);
				//printf("temp value = %d\n", temp);
			}
			if (flg == 0)
			{
				executioner(commands, limit, initial_dir);
			}
			//printf("%s\n%s\n%d\n", commands[0], commands[1], limit);
		}
	}
}
void pipes(char *str, char initial_dir[1024])
{
	struct commandlists *comms;
	comms = malloc(sizeof(struct commandlists));
	for (int i = 0; i < 25; i++)
	{
		comms->commands[i] = malloc(sizeof(struct temp));
		for (int j = 0; j < 25; j++)
		{
			comms->commands[i]->comms[j] = NULL;
		}
	}
	parserpiper(str, comms);
	pipexecutioner(comms, initial_dir);
}
void parserpiper(char *str, struct commandlists *comms)
{
	int call_index = 0;
	int arg_index = 0;
	int calls = 1;
	char *word = strtok(str, " \t\n\0");
	comms->flg = 0;
	while (word != NULL)
	{
		if (strcmp(word, "|") == 0)
		{
			comms->commands[call_index]->len = arg_index;
			calls++;
			call_index++;
			arg_index = 0;
		}
		else if (strcmp(word, ">") == 0)
		{
			comms->flg = 1;
			comms->target = strtok(NULL, " \t\n\0");
		}
		else if (strcmp(word, "<") == 0)
		{
			comms->flg = 2;
			comms->target = strtok(NULL, " \t\n\0");
		}
		else if (strcmp(word, ">>") == 0)
		{
			comms->flg = 3;
			comms->target = strtok(NULL, " \t\n\0");
		}
		else if (strcmp(word, "&") == 0)
		{
			comms->blg = 1;
		}
		else
		{
			comms->commands[call_index]->comms[arg_index] = word;
			arg_index++;
		}
		word = strtok(NULL, " \t\n\0");
	}
	comms->commands[call_index]->len = arg_index;
	comms->len = calls;
}
void pipexecutioner(struct commandlists* comms, char initial_dir[1024])
{
	int fd[2];
	pid_t pid;
	int fd_alt = 0;

	for (int i = 0; i < comms->len; i++)
	{
		pipe(fd);

		if ((pid = fork()) == -1)
		{ // ERROR
			perror("Error at fork \n");
			exit(1);
		}
		else if (pid == 0)
		{ // CHILD

			// redirect flag checks
			if (comms->flg == 1)
			{
				int fd_out = creat(comms->target, 0666);
				dup2(fd_out, STDOUT_FILENO);
				close(fd_out);
			}
			if (comms->flg == 2)
			{
				int fd_in = open(comms->target, O_RDONLY);
				dup2(fd_in, STDIN_FILENO);
				close(fd_in);
			}
			if (comms->flg == 3)
			{
				int fd_app = open(comms->target, O_WRONLY | O_APPEND, 0666);
				dup2(fd_app, STDOUT_FILENO);
				close(fd_app);
			}

			dup2(fd_alt, 0);
			if (comms->commands[i + 1]->comms[0] != NULL)
			{
				dup2(fd[1], 1);
			}

			close(fd[0]);
			execvp(comms->commands[i]->comms[0], comms->commands[i]->comms);
			perror("Error at execvp \n");
			exit(1);
		}
		else
		{ 

			if (1)
			{
				wait(NULL);
			}
			close(fd[1]);
			fd_alt = fd[0];
		}
	}
}
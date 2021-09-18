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
		printf("Error. Please give input correctly.");
		return readcmd(initial_dir);
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
	if(strcmp(prev, str) != 0)
	{

		fprintf(fptr, "%s", str);
		strcpy(prev,str);
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
	if (n > 20)
	{
		for (int i = 0; i < n; i++)
		{
			fgets(line, sizeof(line), fptr);
			if (i > n - 20)
			{
        		printf("%d %s", i,line);
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
	while (word1 != NULL)
	{

		int limit = 0;
		word = strtok_r(word1, delimiter, &ptr);
		// int fdh = open("history.txt", O_RDWR | O_CREAT);
		while (word != NULL)
		{
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
		executioner(commands, limit, initial_dir);
		//printf("%s\n%s\n%d\n", commands[0], commands[1], limit);
	}
}
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
char *readcmd()
{
	char *string;						  // character array pointer
	size_t size = 10;					  // initial size of char array
	string = malloc(size * sizeof(char)); // allocate memory for char array
	char **string_pointer = &string;	  // double pointer to char array
	size_t characters = getline(string_pointer, &size, stdin);
	if (characters == -1)
	{
		printf("Error. Please give input correctly.");
		return readcmd();
	}
	//printf("You typed: %s", string);
	stringsplitter(string);
	return string;
}
void stringsplitter(char str[LINE_LENGTH])
{
	char *delimiter = " \n";  // the first delimiter
	char *delimiter1 = ";";   // the second delimiter
	char *ptr, *ptr1;         // 
	int limit1 = 0;
	char *word,*word1;
	char *commands[100];
	//commands=malloc(sizeof(char)*100*100);
	word1 = strtok_r(str, delimiter1, &ptr1);
	while (word1 != NULL)
	{
		
		int limit = 0;
		word = strtok_r(word1, delimiter, &ptr);
		while (word != NULL)
		{
			commands[limit++] = word;
			word = strtok_r(NULL, delimiter, &ptr);
		}
		word1 = strtok_r(NULL, delimiter1, &ptr1);
		for(int i=limit;i<100;i++)
		{
			commands[i]=NULL;
		}
		executioner(commands, limit);
		//printf("%s\n%s\n%d\n", commands[0], commands[1], limit);
	}
}
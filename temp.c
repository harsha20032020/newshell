#include <stdio.h>
#include <stdlib.h>

int main()
{
    int num;
    FILE *fptr;

    // use appropriate location if you are using MacOS or Linux
    fptr = fopen("program.txt", "w");
    char *string;						  // character array pointer
	size_t size = 10;					  // initial size of char array
	string = malloc(size * sizeof(char)); // allocate memory for char array
	char **string_pointer = &string;	  // double pointer to char array
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    for (int i = 0; i < 3; i++)
    {
        getline(string_pointer, &size, stdin);
        fprintf(fptr, "%s", string);
    }

    fclose(fptr);

    return 0;
}
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/utsname.h>
#include "name.h"
#define MAX_LINE_LENGTH 1024

void printdir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Dir: %s\n", cwd);
}
// this function prints the command prompt or harsh@harsh-VivoBook-ASUSLaptop-X532FLC-S532FL:
void printstr(char *str, int i, int j, char *base)
{
    //printf("%s=base %s=str", base, str);
    int flg = 1;
    if (i > j)
    {
        printf("%s", str);
    }
    else
    {
        for (int k = 0; k < i; k++)
        {
            if (str[k] != base[k])
            {
                flg = 0;
            }
        }
        if (flg == 1)
        {
            for (int k = i; k < j; k++)
            {
                printf("%c", str[k]);
            }
        }
        else
        {
            printf("%s", str);
        }
    }
    printf("$ ");
}
int username(int n, char *base)
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    struct utsname uts;
    if (uname(&uts) != 0)
    {
        return EXIT_FAILURE;
    }
    printf("\033[1;31m");
    printf("%s@%s:~", getenv("USER"), uts.nodename);
    printf("\033[0m");
    printf("\033[1;34m");
    printstr(cwd, n, strlen(cwd),base);
    printf("\033[0m");
    return EXIT_SUCCESS;
}
int printpath()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\033[1;31m");
    printf("%s\n", cwd);
    printf("%s\n", basename(cwd));
    printf("\033[0m");
    return EXIT_SUCCESS;
}
void echo(char *str)
{
    printf("%s\n", str);
}
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/utsname.h>
void printdir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Dir: %s\n", cwd);
}
int username()
{
    char *username = getenv("USER");
    //printf("User: %s\n", username);
    struct utsname uts;
    if (uname(&uts) != 0)
    {
        return EXIT_FAILURE;
    }
    //printf("%s\n", uts.nodename);
    strcat(username, "@");
    strcat(username, uts.nodename);
    printf("%s: ", username);
    return EXIT_SUCCESS;
}
void echo(char *str)
{
    printf("%s\n", str);
}
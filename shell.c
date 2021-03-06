#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#define LINE_LENGTH 1024
#include "./utilities/name.h"
#include "./utilities/parser.h"
#include "./utilities/execute.h"
#include "./utilities/ls.h"
#include "./utilities/list.h"
#include "./utilities/signals.h"
struct node *listglobal;
//int vartemp;
int main()
{
    //printdir();
    char initial_dir[1024];
    getcwd(initial_dir, sizeof(initial_dir));
    int initiallen = strlen(initial_dir);
    listglobal=initialize_list();
    // vartemp=100;
    // printf("%d\n",vartemp);
    while (1)
    {
        //printf("%s\n", initial_dir);
        signal(SIGINT, ctrlC);
        signal(SIGTSTP, ctrlZ);
        //signal(SIGINT, handle_sigint);
        int x=dup(0);
        int y=dup(1);
        usleep(1e5);
        username(initiallen,initial_dir); //prints the default prompt message
        readcmd(initial_dir);
        // size_t characters = getline(string_pointer, &size, stdin);
        // printf("You typed: %s", string);
        //splitcommands(string);
        //printf("%s\n", path);
        //printpath();
        dup2(x,0);
        dup2(y,1);
        close(x);
        close(y);
    }
}
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/utsname.h>
#include <time.h>
#include "execute.h"
#include "name.h"
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "process.h"
#include "parser.h"
#include "dirent.h"
#include "ls.h"
#include <pwd.h>
#include <grp.h>
void lscommand(char *commands[100], int len)
{
    struct dirent *dir;
    DIR *dp = NULL;
    int flg = 0;
    int flags = 0;
    // this is for opening the directory which has no - and &
    // basically exculding words starting with - and &
    char *directories[100];
    char *tokens[100];
    int p = 0, q = 0;
    for (int i = 1; i < len; i++)
    {
        if (commands[i][0] == '-')
        {
            tokens[p++] = commands[i];
        }
        else
        {
            directories[q++] = commands[i];
        }
    }
    //printf("%d %d\n", p, q);
    //tokens contain token strings and p is number of tokens and q is number of directories
    flg = flaggerfunc(tokens, p);
    //printf("flag=%d\n", flg);
    // for(int i=0;i<p;i++)
    // {
    //     printf("%s\n",tokens[i]);
    // }
    // for(int i=0;i<q;i++)
    // {
    //     printf("%s\n",directories[i]);
    // }
    if (q == 0)
    {
        if (flg == 0)
        {
            classicls(".");
        }
        if (flg == 1)
        {
            hiddenfilesls(".");
        }
        if(flg==2 || flg==3)
        {
            detailedls(".",flg);
        }
    }
    else
    {
        for (int i = 0; i < q; i++)
        {
            if (flg == 0)
            {
                classicls(directories[i]);
            }
            if (flg == 1)
            {
                hiddenfilesls(directories[i]);
            }
            if(flg==2 || flg==3)
            {
                detailedls(directories[i],flg);
            }
        }
    }
}
int flaggerfunc(char *tokens[100], int len)
{
    //printf("started %d\n",len);
    if (len == 0)
    {
        //printf("started1\n");
        return 0;
    }
    if (len == 1)
    {
        //printf("%s\n",tokens[0]);
        if (strcmp(tokens[0], "-a") == 0)
        {
            //printf("-a triggered\n");
            return 1;
        }
        else if (strcmp(tokens[0], "-l") == 0)
        {
            //printf("-l triggered\n");
            return 2;
        }
        else if (strcmp(tokens[0], "-la") == 0 || strcmp(tokens[0], "-al") == 0)
        {
            //printf("-la triggered\n");
            return 3;
        }
    }
    else if (len == 2)
    {
        if (strcmp(tokens[0], "-a") == 0 && strcmp(tokens[1], "-l") == 0)
        {
            //printf("-a -l triggered\n");
            return 3;
        }
        else if(strcmp(tokens[0], "-l") == 0 && strcmp(tokens[1], "-a") == 0)
        {
            return 3;
        }
        else if (strcmp(tokens[0], "-a") == 0 && strcmp(tokens[1], "-a") == 0)
        {
            //printf("-a -a triggered\n");
            return 1;
        }
        else if (strcmp(tokens[0], "-l") == 0 && strcmp(tokens[1], "-l") == 0)
        {
            //printf("-l -l triggered\n");
            return 2;
        }
    }
    return -1;
}
void classicls(char *command)
{
    struct dirent **list;
    int n = scandir(command, &list, NULL, alphasort);
    if (n < 0)
    {
        perror("scandir");
        exit(1);
    }
    else
    {
        // while (n--)
        // {
        //     printf("%s\n", list[n]->d_name);
        //     free(list[n]);
        // }
        for (int i = 0; i < n; i++)
        {
            if (list[i]->d_name[0] != '.')
                printf("%s\n", list[i]->d_name);
            free(list[i]);
        }
        free(list);
    }
}
void hiddenfilesls(char *command)
{
    struct dirent **list;
    int n = scandir(command, &list, NULL, alphasort);
    if (n < 0)
    {
        perror("scandir");
        exit(1);
    }
    else
    {
        // while (n--)
        // {
        //     printf("%s\n", list[n]->d_name);
        //     free(list[n]);
        // }
        for (int i = 0; i < n; i++)
        {
            printf("%s\n", list[i]->d_name);
            free(list[i]);
        }
        free(list);
    }
}
void detailedls(char *command, int flag)
{
    struct dirent **list;
    int n = scandir(command, &list, NULL, alphasort);
    if (n < 0)
    {
        perror("scandir");
        exit(1);
    }
    else
    {
        // while (n--)
        // {
        //     printf("%s\n", list[n]->d_name);
        //     free(list[n]);
        // }
        for (int i = 0; i < n; i++)
        {
            if (flag == 3)
            {
                char buffer[10000];
                struct stat file;
                stat(list[i]->d_name, &file);
                printf("%s", (S_ISDIR(file.st_mode)) ? "d" : "-");
                printf("%s", (file.st_mode & S_IRUSR) ? "r" : "-");
                printf("%s", (file.st_mode & S_IWUSR) ? "w" : "-");
                printf("%s", (file.st_mode & S_IXUSR) ? "x" : "-");
                printf("%s", (file.st_mode & S_IRGRP) ? "r" : "-");
                printf("%s", (file.st_mode & S_IWGRP) ? "w" : "-");
                printf("%s", (file.st_mode & S_IXGRP) ? "x" : "-");
                printf("%s", (file.st_mode & S_IROTH) ? "r" : "-");
                printf("%s", (file.st_mode & S_IWOTH) ? "w" : "-");
                printf("%s", (file.st_mode & S_IXOTH) ? "x" : "-");

                printf(" ");
                printf("%ld ", file.st_nlink);
                char date[12];
                struct passwd *pwd;
                struct group *grp;

                if ((pwd = getpwuid(file.st_uid)) != NULL)
                {
                    printf("%s ", pwd->pw_name);
                }
                printf(" ");

                if ((grp = getgrgid(file.st_gid)) != NULL)
                {
                    printf("%s ", grp->gr_name);
                }

                printf(" ");
                printf("%ld ", file.st_size);

                strftime(date, 20, "%b %d %H:%M ", localtime(&(file.st_mtime)));
                printf(" %s ", date);

                printf(" ");
                printf("%s ", list[i]->d_name);
                printf("\n");
            }
            else if (flag == 2)
            {
                char buffer[10000];
                struct stat file;
                if (list[i]->d_name[0] != '.')
                {
                    stat(list[i]->d_name, &file);
                    printf("%s", (S_ISDIR(file.st_mode)) ? "d" : "-");
                    printf("%s", (file.st_mode & S_IRUSR) ? "r" : "-");
                    printf("%s", (file.st_mode & S_IWUSR) ? "w" : "-");
                    printf("%s", (file.st_mode & S_IXUSR) ? "x" : "-");
                    printf("%s", (file.st_mode & S_IRGRP) ? "r" : "-");
                    printf("%s", (file.st_mode & S_IWGRP) ? "w" : "-");
                    printf("%s", (file.st_mode & S_IXGRP) ? "x" : "-");
                    printf("%s", (file.st_mode & S_IROTH) ? "r" : "-");
                    printf("%s", (file.st_mode & S_IWOTH) ? "w" : "-");
                    printf("%s", (file.st_mode & S_IXOTH) ? "x" : "-");

                    printf(" ");
                    printf("%ld ", file.st_nlink);
                    char date[12];
                    struct passwd *pwd;
                    struct group *grp;

                    if ((pwd = getpwuid(file.st_uid)) != NULL)
                    {
                        printf("%s ", pwd->pw_name);
                    }
                    printf(" ");

                    if ((grp = getgrgid(file.st_gid)) != NULL)
                    {
                        printf("%s ", grp->gr_name);
                    }

                    printf(" ");
                    printf("%ld ", file.st_size);

                    strftime(date, 20, "%b %d %H:%M ", localtime(&(file.st_mtime)));
                    printf(" %s ", date);

                    printf(" ");
                    printf("%s ", list[i]->d_name);
                    printf("\n");
                }
            }
        }
        free(list);
    }
}

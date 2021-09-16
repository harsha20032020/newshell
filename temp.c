/*
 * C Program to List Files in Directory
 */
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
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
            char buffer[10000];
            struct stat fileStat;
            stat(list[i]->d_name, &fileStat);
            printf("%s", (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
            printf("%s", (fileStat.st_mode & S_IRUSR) ? "r" : "-");
            printf("%s", (fileStat.st_mode & S_IWUSR) ? "w" : "-");
            printf("%s", (fileStat.st_mode & S_IXUSR) ? "x" : "-");
            printf("%s", (fileStat.st_mode & S_IRGRP) ? "r" : "-");
            printf("%s", (fileStat.st_mode & S_IWGRP) ? "w" : "-");
            printf("%s", (fileStat.st_mode & S_IXGRP) ? "x" : "-");
            printf("%s", (fileStat.st_mode & S_IROTH) ? "r" : "-");
            printf("%s", (fileStat.st_mode & S_IWOTH) ? "w" : "-");
            printf("%s", (fileStat.st_mode & S_IXOTH) ? "x" : "-");

            printf(" ");
            printf("%3ld ", fileStat.st_nlink);
            char date[12];
            struct passwd *pwd;
            struct group *grp;

            if ((pwd = getpwuid(fileStat.st_uid)) != NULL)
            {
                printf("%s ", pwd->pw_name);
            }
            printf(" ");

            if ((grp = getgrgid(fileStat.st_gid)) != NULL)
            {
                printf("%s ", grp->gr_name);
            }

            printf(" ");
            printf("%10ld ", fileStat.st_size);

            strftime(date, 20, "%b %d %H:%M ", localtime(&(fileStat.st_mtime)));
            printf(" %s ", date);

            printf(" ");
            printf("%s ", list[i]->d_name);
            printf("\n");
        }
        free(list);
    }
}
int main(void)
{
    char *command;
    command = ".";
    classicls(command);
    return (0);
}
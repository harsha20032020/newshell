#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include "list.h"
struct node *initialize_list()
{
    struct node *list = (struct node *)malloc(sizeof(struct node *));
    list->next = NULL;
    return list;
}
// void stringprinter(char *command[100])
// {
//     int p=0;
//     while(command[p][0]=="&")
//     {
//         printf("%s ",command[p]);
//         p++;
//     }
// }
// void getStatus(int pid,char *status)
// {

// }
void statusverifier(struct node *list)
{
    int status;
    if (list == NULL)
    {
        printf("List is empty\n");
    }
    else
    {
        struct node *temp = list;
        temp = temp->next;
        //getStatus(temp->pid, temp->status);
        while (temp != NULL)
        {
            int pid = temp->pid;
            char path[50];
            int proc;
            char stat;
            char *process_status;
            sprintf(path, "/proc/%d/stat", pid);
            FILE *filepointer = fopen(path, "r");

            if (filepointer == NULL)
            {
                //printf("No status File, no process with pid %d.\n", pid);
                process_status = "Terminated";
                //return;
            }
            else
            {
                sprintf(path, "/proc/%d/stat", pid);
                fscanf(filepointer, "%d %*s %c", &proc, &stat);

                if (stat == 'R')
                {
                    process_status = "Running";
                }
                else if (stat == 'S')
                {
                    process_status = "Sleeping";
                }
                else if (stat == 'Z')
                {
                    process_status = "Zombie";
                }
                else if (stat == 'T')
                {
                    process_status = "Stopped";
                }
                else if (stat == 'D')
                {
                    process_status = "Uninterruptible";
                }
            }
            temp->status = process_status;
            temp = temp->next;
        }
    }
}
void insert_node(struct node *list, char *process_name, char *status, int pid, int index, char *command[100], int command_size)
{
    //printf("Came here\n");
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->process_name = process_name;
    new_node->status = status;
    new_node->pid = pid;
    new_node->index = index;
    new_node->next = NULL;
    //printf("1:%s %s %d %d %d\n", new_node->process_name, new_node->status, new_node->pid, new_node->index, command_size);
    //printf("len=%d\n",command_size);
    for (int i = 0; i < command_size - 1; i++)
    {
        //new_node->command[i] = (char *)malloc(sizeof(char)*100);
        //printf("%s\n",command[i]);
        new_node->command[i] = command[i];
        if (i == command_size - 2)
        {
            new_node->command[i + 1] = "&";
        }
    }
    struct node *temp = list;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = new_node;
    //print_list(list);
    // for(int i=0;i<command_size;i++)
    // {
    //     printf("%s ",new_node->command[i]);
    // }
}
void delete_node(struct node *list, int pid)
{
    if (list->next == NULL)
    {
        printf("List is empty\n");
    }
    else
    {
        struct node *temp = list;
        struct node *prev = NULL;
        while (temp != NULL && temp->pid != pid)
        {
            prev = temp;
            temp = temp->next;
        }
        printf("%d\n", temp->pid);
        if (temp == NULL)
        {
            printf("No such process\n");
        }
        else if (temp == list)
        {
            list = temp->next;
        }
        else
        {
            prev->next = temp->next;
        }
    }
}
int find_process_by_index(struct node *list, int index)
{
    if (list == NULL)
    {
        printf("List is empty\n");
    }
    else
    {
        struct node *temp = list;
        temp = temp->next;
        while (temp != NULL)
        {
            if (temp->index == index)
            {
                return temp->pid;
            }
            temp = temp->next;
        }
    }
    return -1;
}
void print_list(struct node *list)
{
    bubbleSort(list);
    statusverifier(list);
    if (list == NULL)
    {
        printf("List is empty\n");
    }
    else
    {
        struct node *temp = list;
        temp = temp->next;
        while (temp != NULL)
        {
            printf("[%d] %s ", temp->index, temp->status);
            for (int i = 0; i < 100; i++)
            {
                if (temp->command[i] != NULL && temp->command[i][0] != '&')
                {
                    printf("%s ", temp->command[i]);
                }
                else
                {
                    break;
                }
            }
            printf("[%d]\n", temp->pid);
            temp = temp->next;
        }
    }
}
void runningprintlist(struct node *list)
{
    statusverifier(list);
    if (list == NULL)
    {
        printf("List is empty\n");
    }
    else
    {
        struct node *temp = list;
        temp = temp->next;
        while (temp != NULL)
        {
            if (strcmp(temp->status, "Running") == 0)
            {
                printf("[%d] %s ", temp->index, temp->status);
                for (int i = 0; i < 100; i++)
                {
                    if (temp->command[i] != NULL && temp->command[i][0] != '&')
                    {
                        printf("%s ", temp->command[i]);
                    }
                    else
                    {
                        break;
                    }
                }
                printf("[%d]\n", temp->pid);
            }
            temp = temp->next;
        }
    }
}
void terminatedprintlist(struct node *list)
{
    statusverifier(list);
    if (list == NULL)
    {
        printf("List is empty\n");
    }
    else
    {
        struct node *temp = list;
        temp = temp->next;
        while (temp != NULL)
        {
            if (strcmp(temp->status, "Terminated") == 0)
            {
                printf("[%d] %s ", temp->index, temp->status);
                for (int i = 0; i < 100; i++)
                {
                    if (temp->command[i] != NULL && temp->command[i][0] != '&')
                    {
                        printf("%s ", temp->command[i]);
                    }
                    else
                    {
                        break;
                    }
                }
                printf("[%d]\n", temp->pid);
            }
            temp = temp->next;
        }
    }
}
struct node *find_process_struc(struct node *list, int index)
{
    if (list == NULL)
    {
        printf("List is empty\n");
    }
    else
    {
        struct node *temp = list;
        temp = temp->next;
        while (temp != NULL)
        {
            if (temp->index == index)
            {
                return temp;
            }
            temp = temp->next;
        }
    }
    return list;
}

void bubbleSort(struct node *start)
{
    int swapped, i;
    struct node *ptr1;
    struct node *lptr = NULL;
  
    /* Checking for empty list */
    if (start == NULL)
        return;
  
    do
    {
        swapped = 0;
        ptr1 = start;
  
        while (ptr1->next != lptr)
        {
            if (ptr1->process_name < ptr1->next->process_name)
            {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            { 
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);
}
  
/* function to swap data of two nodes a and b*/
void swap(struct node *a, struct node *b)
{
    // int temp = a->data;
    // a->data = b->data;
    // b->data = temp;
    struct node *temp;   
    temp->index = a->index;
    temp->process_name = a->process_name;
    temp->status = a->status;
    temp->pid = a->pid;
    //temp->command = a->command;
    // for(int i=0;i<100;i++)
    // {
    //     temp->command[i] = a->command[i];
    // }
    //temp->next = a->next;
    a->index = b->index;
    a->process_name = b->process_name;
    a->status = b->status;
    a->pid = b->pid;
    //a->command = b->command;
    // for(int i=0;i<100;i++)
    // {
    //     a->command[i] = b->command[i];
    // }
    //a->next = b->next;
    b->index = temp->index;
    b->process_name = temp->process_name;
    b->status = temp->status;
    b->pid = temp->pid;
    //b->command = temp->command;
    // for(int i=0;i<100;i++)
    // {
    //     b->command[i] = temp->command[i];
    // }
    //b->next = temp->next;
}
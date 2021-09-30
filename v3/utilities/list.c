#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
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
void statusverifier(struct node* list)
{
    int status;
    if (list == NULL)
    {
        printf("List is empty\n");
    }
    else
    {
        struct node *temp = list;
        temp=temp->next;
        while (temp != NULL)
        {
            if(waitpid(temp->pid,&status,WNOHANG)==0)
            {
                temp->status="Running";
            }
            else
            {
                temp->status="Terminated";
            }
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
    for(int i=0;i<command_size-1;i++)
    {
        //new_node->command[i] = (char *)malloc(sizeof(char)*100);
        //printf("%s\n",command[i]);
        new_node->command[i]=command[i];
        if(i==command_size-2)
        {
            new_node->command[i+1] = "&";
        }
    }
    struct node *temp=list;  
    while(temp->next!=NULL) 
    {
        temp=temp->next;
    }
    temp->next=new_node;
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
void find_process_by_pid(struct node *list, int pid)
{
    if (list == NULL)
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
    }
}
void print_list(struct node *list)
{
    statusverifier(list);
    if (list == NULL)
    {
        printf("List is empty\n");
    }
    else
    {
        struct node *temp = list;
        temp=temp->next;
        while (temp != NULL)
        {
            printf("[%d] %s ",temp->index,temp->status);
            for(int i=0;i<100;i++)
            {
                if(temp->command[i]!=NULL && temp->command[i][0]!='&')
                {
                    printf("%s ",temp->command[i]);
                }
                else
                {
                    break;
                }
            }
            printf("[%d]\n",temp->pid);
            temp = temp->next;
        }
    }
}
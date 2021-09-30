#ifndef LIST_H
#define LIST_H
struct node
{
    char *process_name;
    char *status;
    char *command[100];
    int pid;
    int index;
    struct node *next;
};
extern struct node *listglobal;
struct node  *initialize_list();
void insert_node(struct node *list, char *process_name, char *status, int pid, int index, char *command[100], int command_size);
void delete_node(struct node *list, int pid);
void find_process_by_pid(struct node *list, int pid);
void print_list(struct node *list);
//extern int vartemp;
#endif 
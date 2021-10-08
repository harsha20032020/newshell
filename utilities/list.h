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
int find_process_by_index(struct node *list, int index);
void print_list(struct node *list);
void statusverifier(struct node* list);
void terminatedprintlist(struct node *list);
void runningprintlist(struct node *list);
struct node *find_process_struc(struct node *list, int index);
void swap(struct node *a, struct node *b);
void bubbleSort(struct node *start);
void alphasort(struct node *list);
//extern int vartemp;
#endif 
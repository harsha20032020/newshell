#ifndef LIST_H
#define LIST_H

void insert_node(struct node **list, char process_name[100], char status[100], int pid, int index, char *command[100], int command_size);
void delete_node(struct node **list, int pid);
void find_process_by_pid(struct node **list, int pid);
void print_list(struct node **list);
#endif 
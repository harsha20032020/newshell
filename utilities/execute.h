#ifndef NAME_H
#define NAME_H

void executioner(char *list[100], int size,char initial_dir[1024]);
void echocommand(char *command[100], int len);
void cdcommand(char *command[100], int len, char *initial_dir);
void processes(char *command[100], int len);
void repeat(char *commands[100],int n,int len,char *initial_dir);
void temp(char *str, int i, int j);
void pinfo();
void pinfo2(int pid);
void lscommand(char *commands[100], int len, char *initial_dir);
int flaggerfunc(char *tokens[100],int len);
#endif // !NAME#def

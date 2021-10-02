#ifndef PROCESS_H
#define PROCESS_H

void background(char *command[100], int len);
void foreground(char *command[100], int len);
void child();
void display();
void running();
void terminated();
void kill_process(int a,int b);
void foreground_process(int a);
void background_process(int a);
#endif // !NAME#def

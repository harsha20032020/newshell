#ifndef PARSER_H
#define PARSER_H
#define LINE_LENGTH 1024
char *readcmd();
void writetofile(char str[LINE_LENGTH]);
void stringsplitter(char str[LINE_LENGTH]);
void history();
void complexhistory(int x);
int countlines();
#endif // !NAME#def

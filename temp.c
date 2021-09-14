#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>

int main(int argc, char **argv) {
  char *home=getenv("HOME");
  printf("%s\n",home);
  return EXIT_SUCCESS;
}
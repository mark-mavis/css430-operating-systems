#ifndef _shell_h
#define _shell_h

#include <string.h>

#define SIMPLE  0
#define PIPE    1
#define REDIRECT_OUT    2
#define REDIRECT_IN     3

char *getCmd(char *buff, char *args[]);


void printHistory(){};
void addToHistory(){};


#endif
#include <pthread.h>    //Threading Package - Part of the POSIX standard. Supported
                        // on most platforms
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>  

//Test sync

int value = 0;
void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
    pid_t pid;      //Process ID Variable
    pthread_t tid;  //Process Thread ID (Unsigned Integer)
    pthread_attr_t attr;    //Process Thread Attributes Variable

    pid = fork();
    if (pid == 0)
    { /* child process */
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, runner, NULL);
        pthread_join(tid, NULL);
        printf("CHILD: value = %d\n", value); /* LINE C */
    }
    else if (pid > 0)
    { /* parent process */
        wait(NULL);
        printf("PARENT: value = %d\n", value); /* LINE P */
    }

    return 0;
}

void *runner(void *param)
{
    value = 5;
    pthread_exit(0);
}

void* fun(void* arg){
    char* str = (char*)arg;                     //Cast the argument as a Character pointer and assign to str
    double* db = malloc(sizeof(double));        //Request memory with Memory Allocation function to store a double float number
    *db = atof(str);                            //Convert a string to a floating-point number and assign to the double pointer db
    return (void*)db;                           //return the double pointer and cast as a void pointer
}

//USE gcc -pthread Threads.c to compile
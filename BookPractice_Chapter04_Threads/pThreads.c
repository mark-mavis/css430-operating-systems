#include <pthread.h>    //Threading Package - Part of the POSIX standard. Supported
                        // on most platforms
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t pid;      //Create the Process ID Variable
    pthread_t tid;  //Create the Process Thread ID (Unsigned Integer)
    pthread_attr_t attr;    //Create the process Thread Attributes Variable

    pthread_create(&tid, NULL, fun, "1.234" );

    double* pd;
    pthread_join(tid, &pd);
    printf("PD Value = %f\n", pd); 
    printf("PD Dereferenced Value = %f", *pd);    

    return 0;
}

void* fun(void* arg){
    char* str = (char*)arg;     //Cast the argument as a Character pointer and assign to str
    double* db = malloc(sizeof(double));     //Request memory with Memory Allocation function to store a double float number
    *db = atof(str);        //Convert a string to a floating-point number and assign to the double pointer db
    return (void*)db;   //return the double pointer and cast as a void pointer
}

//USE gcc -pthread Threads.c to compile
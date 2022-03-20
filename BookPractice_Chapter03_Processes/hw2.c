#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
    pid_t pid, pid1;

    /* fork a child process */
    pid = fork();
    //pid = 0 since the child returned that number

    if(pid < 0) { /*error occured */
        fprintf(stderr, "Fork Failed\n");
        return 1;
        //Parent = 2600
        //Child = 2603
    }else if (pid == 0){
        pid1 = getpid();
        printf("child1.1: pid = %d\n", pid);
        printf("child1.2: pid1 = %d\n", pid1);
    }else{
        pid1 = getpid();
        printf("parent2.1: pid = %d\n", pid);
        printf("parent2.2: pid1 = %d\n", pid1);
        wait(NULL);
    }
    return 0;
}
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    //pid_t pid;
    //printf("Hello World1\n");

    fork();   //1. Spin off child process that maintains same data as parent
                    //2. returns the childs process ID
                    //3. After creation of the process, both parent and child process starts execution from the new instruction
                    //  if return is <0 = Error (E.g. out of memory)
                    //  if return is >0 = Successful creation of process
                    //  if return is 0  = Child Process
    fork();     
    printf("PID: %d\n", getpid());
    
    return 0;
}
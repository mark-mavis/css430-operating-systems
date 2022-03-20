#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char* argv[]){

    pid_t pid;

    //Spin off child process from parent which is this program itself
    //returns a Process ID (pid)
    
    //Spin off a child process and
    pid = fork();
    if(pid != 0){
        //Tell Parent to wait for child and return the process ID of the child
        // process that finished
        int res = wait(NULL);
        
        if(res == -1){  
            printf("No children to wait for\n");
        }else{
            printf("%d finished execution\n", res);
        }   

        printf("Then parent process finishes last\n");
        printf("Parent ID: %d, Parent's Parent ID: %d\n", getpid(), getppid());
    }else{
        printf("Child process finishes first\n");
        printf("Child ID: %d, Child's Parent ID: %d\n", getpid(), getppid());
    }
    
    



    /* Delegating both a parent and child process to each print out there own
        set of values
    int n;
    if(pid == 0){
        n = 1;
    }else{
        n = 6;
    }

    if(pid != 0){
        wait(0);    //Telling the parent to wait until the child has finished and returned
    }

    int i;
    for(i = n; i < n + 5; i++){
        printf("%d\n", i);
        fflush(stdout);
    }
    */

    

    return 0;
}

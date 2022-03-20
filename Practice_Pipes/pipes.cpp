#include <unistd.h> //Fork(), Pipe()
#include <stdlib.h> //Exit() command
#include <stdio.h>  
#include <sys/wait.h>  //Wait() system call
#include <iostream>
using namespace std;


int main(int argc, char* argv[]) {
    
    int pipeFD[2];  //Creating a File Descriptors pipe that is an Integer Array of Size 2
                    // [0] - Input (Read into the Process)
                    // [1] - Output (Read Out from the Process)
    
    
    //Creating Pipe and passing the file desriptors in
    //Upon creation of the pipe, the pipe system call returns either 0 or -1
    //Error check what the pipe returns.
    //Because this pipe is created before the Fork(), means that the parent and the child have access to this pipe
    if(pipe(pipeFD) == -1);{
            printf("An error occurred with opening the pipe\n");
            return 1;
    }               

    //Created a child process from the parent
    pid_t pid = fork();         //The fork returns an integer when it is done executing
                                // The pipeFileDescriptors get copied into both processes

    if(pid == 0){   //If in the child Process
        //When in the context of the Child process, This code runs

        close(pipeFD[1]);       //Closing the Read From side of the pipe from the childs point of view
        char buffer[256];       //Creating a buffer of 256 bytes
        
        // N = number of bytes read
        int n = read(pipe[0]), buffer, 256);    //Read system call takes 3 parameters and returns the number of bytes read
                                                //  1. What to read from
                                                //  2. What to write too
                                                //  3. Limit of bytes to take in (up to 256)

        buffer[n] = '\0'; 
        cout << "Child Process Printing" << endl;
        cout << buffer << endl;

    }else{      //In the Parent Process
        ////When in the context of the Parent process, This code runs
        
        close(pipeFD[0]);       //Closing the Read into the parent process side of the pipe
        dup2(pipeFD[1], 1)      //
        cout << pid << endl;
    }
    return 0;
}

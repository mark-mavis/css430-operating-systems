#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "shell.h"

#define MAX_LINE    80

int main(void)
{
    char *args1[MAX_LINE/2 +1];
    char *args2[MAX_LINE/2 +1];
    
    //Command Line Buffer
    char *buff;
    
    //History Buffer
    char last_buff[MAX_LINE+1];
    
    //
    char *pt;

    //Program Control. 
    //Flag to determine when to exit program.
    int should_run = 1;
    
    //Unsigned Integer Type Length = 0
    size_t length = 0;
    
    //Array that stores both commands
    char *cmd[2];

    //Pid variables that store the processes IDs
    int pid1, pid2;


    int wstatus1, wstatus2;


    int cmdType = SIMPLE;

    
    int pipefd[2];


    int background = 0;


    int redfd = -1;


    while(should_run) {
        printf("osh>");
        int flushSuccess = fflush(stdout);

        //Get cmd 
        //getline(char *lineptr, size_t *n, FILE *Stream)
        getline(&buff, &length, stdin);
        
        // Do a trim of the line
        // strlen(const char *s)
        // while (string length of the buffer is greater than 0) and (There is a blank spot at the end of the Buffer Array)
        
        while ((strlen(buff) > 0) && (buff[strlen(buff)-1] <= ' ')){   
            buff[strlen(buff)-1] = 0;
        }

        //If there is something in the buffer, execute
        if (strlen(buff)) {
            
            // If the buffer and the user input "!!" do not equal each other...
            if (strcmp(buff,"!!") == 0) {
                strcpy(buff, last_buff);     //Then copy the History buffer into the buffer
            }
            
            // Save for history
            // Copy the buffer into the Last history buffer
            strcpy(last_buff, buff);
            

            // Check for background Symbol in the last position of the buffer
            if ( buff[strlen(buff)-1] == '&' ) {
                
                //Set the background variable to true
                background = 1;

                // Remove the background character from the buffer
                buff[strlen(buff)-1] = 0;
            }

            // Check for the PIPE symbol in the buffer
            // strchr --> Looks for the pipe character in the given buffer and returns a pointer to the first 
            //      occurance of the charater in the C string. If no character is found, return NULL
        
            if ( (pt = strchr(buff,'|')) != NULL ) {    //If string character does not return NULL, Assign Pt to the location of the
                                                        // first instance of that character
                
                cmdType = PIPE;     //Define the command as a PIPE command
                *pt = 0;            //Assign 0 to that position in the buffer
                cmd[1] = getCmd(++pt, args2);    //
                
            // Check for redirect In
            } else if ( ( pt = strchr(buff,'<')) != NULL ) {
                cmdType = REDIRECT_IN;
                *pt = 0;
                cmd[1] = getCmd(++pt,args2);
            
            // Check for redirect Out
            } else if ( ( pt = strchr(buff,'>')) != NULL ) {
                cmdType = REDIRECT_OUT;
                *pt = 0;
                cmd[1] = getCmd(++pt,args2);
            } else {
                cmdType = SIMPLE;
            }

            //Store Cmd
            cmd[0] = getCmd(buff, args1);

            //Check for Pipe
            if ( cmdType == PIPE ) {   
                if (pipe(pipefd) == -1 ) {                      //check if a pipe was created
                    printf("Failed to open the pipe\n");        //report failed pipe status
                }
            }

            //Check for REDIRECT IN Command
            if ( cmdType == REDIRECT_IN ) {  
                if ((redfd = open(cmd[1],O_RDONLY)) == -1 ) {   //check to see if file didnt opened
                    perror("Fail to open <");                   //Report failed open file status
                }
            }

            //Check for REDIRECT OUT command
            if ( cmdType == REDIRECT_OUT ) {                    
                if ((redfd = open(cmd[1],O_WRONLY | O_CREAT, 0644)) == -1 ) {       //check to check to see if file didnt open
                    perror("Fail to open >");                                       //Report failed opening
                }
            }

            if ( ( pid1 = fork() ) < 0 ) {      //check if fork was successful
                printf("Fork failed!\n");
            } else if (pid1 == 0 ) {            // Child Process
                switch(cmdType) {               
                    //If cmdType is a pipe command, execute this block
                    case PIPE:
                        // This child doesn't need the write end of the pipe
                        close(pipefd[1]);
                        // Use the read end as standard in
                        dup2(pipefd[0],STDIN_FILENO);
                        // Now that the read end has been duplicated we can close it.
                        close(pipefd[0]);
                        // Now we can execute with standard in connected to the pipe
                        execvp(cmd[1], args2);
                        break;
                    
                    //If cmdType is a REDIRECT IN command, execute this block
                    case REDIRECT_IN:
                        if ( (pid1 = fork() ) < 0 ) {
                            printf("Fork failed!\n");
                        } else if (pid1 == 0 ) {    // Child
                            dup2(redfd,STDIN_FILENO);
                            execvp(cmd[0],args1);
                        }
                        break;
                    
                    //If cmdType is a REDIRECT OUT command, execute this block
                    case REDIRECT_OUT:
                        if ( (pid1 = fork() ) < 0 ) {
                            printf("Fork failed!\n");
                        } else if (pid1 == 0 ) {    // Child
                            close(STDOUT_FILENO);
                            dup(redfd);
                            execvp(cmd[0],args1);
                        }
                        break;

                    //If cmdType is none of the previous cases, execute this block
                    default:
                        if ( background ) {
                            daemon(1,1);
                        }
                        execvp(cmd[0],args1);
                }
            } else {                            // Parent
                switch(cmdType) {
                    case PIPE:
                        if ( (pid2 = fork() ) < 0 ) {
                            printf("Fork failed!\n");
                        } else if (pid2 == 0 ) {    // Child
                            // This child doesn't need the read end of the pipe
                            close(pipefd[0]);
                            // Use the write end as standard in
                            dup2(pipefd[1],STDOUT_FILENO);
                            // Now that the write end has been duplicated we can close it.
                            close(pipefd[1]);
                            // Now we can execute with standard out connected to the pipe
                            if ( background ) {
                                daemon(1,1);
                            }
                            execvp(cmd[0],args1);
                        }
                        break;
                    case REDIRECT_IN:            // <
                        break;
                    case REDIRECT_OUT:          // >
                        break;
                }
                if ( cmdType == PIPE ) {    // If a pipe was open let's close it
                    close(pipefd[0]);
                    close(pipefd[1]);
                }
                if ( cmdType == REDIRECT_IN || cmdType == REDIRECT_OUT ) {
                    close(redfd);
                }

                background = 0;
                
                if ( background != 1 ) {
                waitpid(pid1,&wstatus1,0);
                }

                if ( cmdType == PIPE ) {
                    waitpid(pid2,&wstatus1,0);
                }
            }
        }
    }
    return 0;

} // End of main

///////////////////////////////////////////////////////////////
//

//Function that breaks up what the user is typing
char *getCmd(char *buff, char *args[])
{

    int ct = 1;
    char *cmd = NULL;

    cmd = strtok(buff, " \t\n");
    args[0] = cmd;
    do {
        args[ct] = strtok(NULL, " \t\n");

    } while(args[ct] != NULL && strlen(args[ct++]));

    printf("cmd %s\n",cmd);
    for(int i=0; i < ct; i++) {
        printf("args[%d] %s\n",i, args[i]);
    }

    return cmd;

} // End of getCmd()
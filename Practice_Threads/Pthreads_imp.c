#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10


//Calculating the summation of all of the numbers from 1 to the number provided 
// on the command line but creating a second thread to calculate and then return 
// to the parent thread

int sum;       //This data is shared by the thread(s)
void *runner(void *param);  //Threads call this function

int main(int argc, char *argv[]){
    pthread_t tid;                  //Declare the thread Identifier for the thread we create
    pthread_attr_t attr;            //Set of default thread attributes


    //Takes two thread IDs and returns an nonzero value if both the thread IS's 
    // are equal or else it returns zero
    int pthread_equal(pthread_t tid1, pthread_t tid2);
    
    //Setting the default attributes of the thread using above thread attributes
    pthread_attr_init(&attr);

    //Creating the seperate thread from the existing one by passing
    //&tid = Reference to the Thread Identified
    //&attr = Reference to the thread attributes
    //runner = Name of the function where the new thread will begin execution
    //argv[1] = The parameter that was provided on the command line
    pthread_create(&tid, &attr, runner, argv[1]);   

    //Wait for the thread to exit
    //tid = Thread Identifier
    //NULL = return value
    pthread_join(tid, NULL);   

    /* 
        Simple method for waiting on several threads

        Array of threads to be joined upon
            pthread_t workers[NUM_THREADS];
    
            for (int i = 0; i < NUM_THREADS; i++){
                pthread_join(workers[i]. NULL);
            } 
    */
   
    printf("Sum = %d\n", sum);      //Print the Sum

    return 0;
}

//The summation thread
//The thread will execut in this function
void *runner(void *parameter){
    
    int i;
    int upper_bounds = atoi(parameter);
    sum = 0;

    for(i = 0; i <= upper_bounds; i++){
        sum += i;
    }

    pthread_exit(0);
}

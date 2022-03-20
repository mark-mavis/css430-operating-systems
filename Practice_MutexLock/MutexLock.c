#include <stdio.h>        // printf
#include <stdlib.h>       // exit, atoi
#include <unistd.h>
#include <pthread.h>      // pthread*

#define BUFSIZE 1000
#define gMax    20

pthread_mutex_t lock;
pthread_mutex_t lock1;
pthread_mutex_t lock2;

/*----------------------------------------------------------------------------------------------------
  GLOBAL VARIABLES that are kept in the Data segment of the process control block
-----------------------------------------------------------------------------------------------------*/
int NUMPROD = 1;    // number of producers
int NUMCONS = 1;    // number of consumers

int globalBuffer[BUFSIZE];  // global buffer
int nextNumberToBeProduced = 0;       // global counter

int bufferInputIndex  = 0;       // input index in gBuf
int bufferOutputIndex = 0;       // output index in globalBuffer

//----------------------------------------------------------------------------------------------------


void say(int me, char* msg, int x) {
    printf("%d ", me);    //Print out the Tid of the Current Process
    printf("%s", msg);    //Print out the thread type (Producer, Consumer)
    printf("%d \n", x);   //Print out the number
}

/*----------------------------------------------------------------------------------------------------
    Function that inserts a number at the bufferInputIndex position and increments the bufferInputIndex Index
-----------------------------------------------------------------------------------------------------*/
void insert(int x) { 
  //pthread_mutex_lock(&lock1);
  globalBuffer[bufferInputIndex] = x;    
  ++bufferInputIndex;
  //pthread_mutex_unlock(&lock1);
}

/*----------------------------------------------------------------------------------------------------
    Function that returns whats in the bufferOutputIndex index position and increments the bufferOutputIndex index
-----------------------------------------------------------------------------------------------------*/
int extract() {
  //pthread_mutex_lock(&lock1);
  int x = globalBuffer[bufferOutputIndex];
  ++bufferOutputIndex;
  //pthread_mutex_unlock(&lock1);
  return x;
}

/*-----------------------------------------------------------------------------------------------------
    Function that increments the nextNumberToBeProduced variable and then returns the number that it incremented too
------------------------------------------------------------------------------------------------------*/
int incrementNextNumberToBeProduced() {
  //pthread_mutex_lock(&lock2);
  int num = ++nextNumberToBeProduced;
  //pthread_mutex_unlock(&lock2);
  return num;
}

/*------------------------------------------------------------------------------------------------------
    Function that just returns the nextNumberToBeProduced number
------------------------------------------------------------------------------------------------------*/

int getNextNumberToBeProduced() {
  //pthread_mutex_lock(&lock2);
  int num = nextNumberToBeProduced;
  //pthread_mutex_unlock(&lock2);
  return num;
}

/*------------------------------------------------------------------------------------------------------
    Function that takes in a pointer to the argument 
-------------------------------------------------------------------------------------------------------*/

void* producer(void* arg) {
  pthread_mutex_lock(&lock);              //  Prevent other producer from accessing during execusion
  int me = pthread_self();                //  Return the Thread ID of the current producer
  int num = getNextNumberToBeProduced();  //  Return Next number to be produced
  while (num < gMax) {
    num = incrementNextNumberToBeProduced();
    say(me, "Produced: ", num);
    insert(nextNumberToBeProduced);
  }
  say(me, "Exiting at ", num);
  pthread_mutex_unlock(&lock);
  return NULL;
}

/*----------------------------------------------------------------------------------------------------
    Function that takes a pointer to the argument string
-----------------------------------------------------------------------------------------------------*/
void* consumer(void* arg) {
  pthread_mutex_lock(&lock);
  int me = pthread_self();
  while (1) {
    int num = extract();
    say(me, "Consumed: ", num);
    if (num >= gMax || num == 0) { 
      say(me, "Exiting at ", num);
      break;
    }
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}

/*===================================================================================================
=====================================================================================================
    Function that checks the number of arguments passed into the command line arguments
    
    Cases:
      1. Nothing is entered - Assign 1 to both the Number producer and Number Consumer
      2. To many arguments are entered - Tell the user the correct input format
      3. User put corrent input in - Convert Argument [1] and Argument [2] to Integer

=====================================================================================================
=====================================================================================================*/

void checkInput(int argc, char* argv[]) {
  if (argc == 1) {
    NUMPROD = 1;
    NUMCONS = 1;
    printf("Checkpoint\n");
    return;
  }
/*-----------------------------------------------------------------------------------------------------
    If the user has put to little or to many arguments in the command line, 
    notify the user to input the correct format
------------------------------------------------------------------------------------------------------*/
  if (argc != 3) {
    printf("Specify <producers>  <consumer> \n");
    printf("Eg:  2  3 \n");
    exit(0);
  }
/*-----------------------------------------------------------------------------------------------------
    *Convert the first argument to an integer and assign to the Numbers to global producer variable
    *If the number is less than 1 or greater then 10, tell the user the acceptable range
    *then exit
-------------------------------------------------------------------------------------------------------*/
  NUMPROD = atoi(argv[1]);
  if (NUMPROD < 1 || NUMPROD > 10) {
    printf("Number of producers must lie in the range 1..10 \n");
    exit(0);
  }
/*-----------------------------------------------------------------------------------------------------
    *Convert the second argument to an integer and assign to the Numbers to consume
    *If the number is less than 1 or greater then 10, tell the user acceptable range
    *then exit
------------------------------------------------------------------------------------------------------*/
  NUMCONS = atoi(argv[2]);
  if (NUMCONS < 1 || NUMCONS > 10) {
    printf("Number of consumers must lie in the range 1..10 \n");
    exit(0);
  }
}


int main(int argc, char* argv[]) {

  checkInput(argc, argv);     //Check the command line arguments input by the user and update global variables

  pthread_t prod[NUMPROD];    // Array of Threads (Producers)
  pthread_t cons[NUMCONS];    // Array of Threads (Consumers)

    if (pthread_mutex_init(&lock, NULL) !=0) {
      printf("Error in mutex init. \n");
  }

  for (int i = 0; i < NUMPROD; ++i) {
      pthread_create(&prod[i], 0, producer, NULL);  //Create the number of producers (threads) specified by the user
  }

  for (int i = 0; i < NUMCONS; ++i) {
      pthread_create(&cons[i], 0, consumer, NULL);  //Create the number of Consumers (threads) specified by the user
  }

  for (int i = 0; i < NUMPROD; ++i) {
      pthread_join(prod[i], NULL);
      pthread_join(cons[i], NULL);
  }

  pthread_mutex_destroy(&lock);

  printf("All done! Hit any key to finish \n");
  getchar();
  
  return 0;
}
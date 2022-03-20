#include <stdio.h>
#include <cstdio>
#include <vector>
#include <iostream>
#include <thread>

#define BUFFER_SIZE 100

int globalCount = 0;
int inputIndexPosition;
int outputIndexPosition;
int globalBuffer[BUFFER_SIZE];

void SumNumbers (const std::vector<int>& toBeSummed){

}

void main()
{

    std::vector<int> toBeSummed;
    for(int i=0; i < 30000; ++i){
        toBeSummed.push_back(rand()); 
    }

    long sum = 0;
    
    for(int i=0; i < toBeSummed.size(); ++i){
        sum += toBeSummed[i];                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    }
    printf("the sum was %d\n", sum);



    
   /*--------------------------------------------------------------------------------
        Producer
    ---------------------------------------------------------------------------------
    while true
        produce an item in next_produced

        while the count equals the buffer size
            do nothing

        buffer[inputIndexPosition] = next_produced;
        inputIndexPosition = (inputIndexPosition + 1) % BUFFER_SIZE;
        count++;

    ---------------------------------------------------------------------------------*/
    while(true){
        while(globalCount == BUFFER_SIZE){
            //Do nothiing
            return;
        }

        globalBuffer[inputIndexPosition] = 

        

    }

    /*-------------------------------------------------------------------------------
        Consumer
    ---------------------------------------------------------------------------------
    while true
        while count equals 0
            do nothing
        next_consumed = buffer[out];
        outputIndexPosition = (outputIndexPosition + 1) % BUFFER_SIZE;
        count--;

        Consume the item in next_consumed
   ----------------------------------------------------------------------------------*/

    
    std::thread t1;
    std::thread t2;
    std::thread t3;

    t1.join();
    t2.join();
    t3.join();
}
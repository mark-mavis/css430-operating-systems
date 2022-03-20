#include <thread>
#include <semaphore.h>
using namespace std;

// TOPIC: SEMAPHORES || semaphore good when using shared memory || Working together to solve a problem

/*
    Semaphores fall into what are called SYNCHRONIZATION PRIMATIVES. Help us 
    cordinate activity between multiple concurrently threads or processes

    ---->> Synchronization Primitives <<----
    1. Mutex
    2. Condition var
    3. monitors barriers
*/

typedef int semaphore;
semaphore mutex, full, empty;

int count = 0;


int main(){



    return 0;
}
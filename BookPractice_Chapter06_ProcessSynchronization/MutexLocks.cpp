#include <iostream>
#include <thread>
#include <mutex>
#include <iostream>
#include <iterator>
using namespace std;


/*
    MUTEX LOCKS
    Mutex locks use mutual exclusion between threads, meaning that threads share date back
            and forth but only allow one thread to work on those data at a time and lock out
            the other threads
*/

int myAccount = 0;
std::mutex m; //Mutex Lock

void AddMoney()
{
    //Blocks the
    m.lock();    //  <----- Turns the mutex value of 0 to 1
    ++myAccount; //  <----- This is the critical section
    m.unlock();  //  <-----  Turn the mutex value of 1 to 0
}

int main()
{

    //Creating two threads that both are going to run the add money
    std::thread t1(AddMoney);
    std::thread t2(AddMoney);
    //Both threads start concurrently and beginning executing at the same time, even thought there is
    //      a extremely minor difference when reaching for the "AddMoney" function. Becuase of this
    //      minor difference, one thread reaches the AddMoney function first and then attains the lock
    //      before the other thread can and locks the thread out until it is done
    t1.join();
    t2.join();

    cout << myAccount << endl;
    return 0;
}

#include <setjmp.h> // setjmp( )
#include <signal.h> // signal( )
#include <unistd.h> // sleep( ), alarm( )
#include <stdio.h>  // perror( )
#include <stdlib.h> // exit( )
#include <iostream> // cout
#include <string.h> // memcpy
#include <queue>    // queue

//Saves the current environment into main_env
#define scheduler_init()                     \
{			                                    \
 cerr << "scheduler_init() called" << endl;  \
   if (setjmp(main_env) == 0)	               \
   {                                         \
      cerr << "Main Environment saved" << endl;      \
      scheduler();				               \
   }                                         \
   cerr << "Skipping out on scheduler_init" << endl; \
}

#define scheduler_start()                    \
{			                                    \
 cerr << "scheduler_start() called" << endl; \
   if (setjmp(main_env) == 0)                \
   {                                         \
      cerr << "main_env saved" << endl;      \
      longjmp( scheduler_env, 1 );           \
   }                                         \
   cerr << "scheduler_start setjmp return value non zero" << endl; \
}

#define capture()                                                    \
{                                                                    \
   register void *sp asm ("sp");                                     \
   register void *bp asm ("bp");                                     \
   cur_tcb->size_ = (int)((long long int)bp - (long long int)sp);    \
   cur_tcb->sp_ = sp;                                                \
   cur_tcb->stack_ = malloc(cur_tcb->size_*sizeof(long long int));   \
   memcpy( cur_tcb->stack_, sp, cur_tcb->size_ );              \
   thr_queue.push(cur_tcb);                                    \
}
    
#define sthread_yield()                                        \
{                                                              \
   if( setjmp(cur_tcb->env_) == 0){                            \
      if(alarmed == true){                                     \
         alarmed = false;                                      \
         capture();                                            \
         longjmp(scheduler_env, 1);                            \
      }                                                        \
   }else{                                                      \
      memcpy( cur_tcb->sp_, cur_tcb->stack_, cur_tcb->size_ ); \
   }                                                           \
                                                               \
}

#define sthread_init()                       \
{                                            \
   cerr << "sthread_init() called" << endl;  \
   if (setjmp(cur_tcb->env_) == 0 ) \
   {                                \
      capture();                    \
      longjmp(main_env, 1);	      \
   }                                \
   memcpy(cur_tcb->sp_, cur_tcb->stack_, cur_tcb->size_);	\
}

#define sthread_create(function, arguments)     \
{                                               \
   if (setjmp(main_env) == 0)                   \
   {                                            \
      cerr << "inside sthread_create" << endl;  \
      func = &function;				               \
      args = arguments;				               \
      cur_tcb = new TCB();			            \
      thread_created = true;			         \
      longjmp(scheduler_env, 1);             \
    }                                        \
}

#define sthread_exit()              \
{			                           \
   if (cur_tcb->stack_ != NULL)		\
   {                                \
      free(cur_tcb->stack_);			\
   }                                \
   longjmp(scheduler_env, 1);		   \
}


using namespace std;

static jmp_buf main_env;      //the main environment bookmark  
                              // It saves...
                              //    - Our Position in the cod (line of code)
                              //       PC = Program Counter
                              //    - Our Position in the stack
                              //       SP = Stack Pointer
static jmp_buf scheduler_env; //The Scheduler Environment bookmark

// Thread control block
class TCB 
{
public:
   TCB() : sp_(NULL), stack_(NULL), size_(0) {}
   jmp_buf env_;  // the execution environment captured by set_jmp()
   void* sp_;     // the stack pointer 
   void* stack_;  // the temporary space to maintain the latest stack contents
   int size_;     // the size of the stack contents
};

static TCB* cur_tcb;   // the TCB (Thread Control Block) of the current thread in execution

// The queue of active threads
static queue<TCB*> thr_queue;

const int kTimeQuantum = 5;      //The Amount of time to wait.

// Alarm caught to switch to the next thread
static bool alarmed = false;
static void sig_alarm(int signo) 
{
   alarmed = true;
}

// A function to be executed by a thread
void (*func)(void *);
void *args = NULL;
static bool thread_created = false;

static void scheduler() 
{
   // invoke scheduler
   if (setjmp(scheduler_env) == 0)     //Save the scheduler environment
   {
      cerr << "scheduler: initialized" << endl;

      if (signal(SIGALRM, sig_alarm) == SIG_ERR) 
      {
         perror("signal function");
         exit(-1);
      }
      cerr << "Longjmp to Main Environment about to happen" << endl;
      longjmp(main_env, 1);   //Because it returns the value 1, it skips the setjmp function in scheduler_init()
   }

   // check if it was called from sthread_create()
   if ( thread_created == true ) 
   {
      cerr << "Thread created is true" << endl;
      thread_created = false;
      (*func)(args);
   }

   // restore the next thread's environment
   if ((cur_tcb = thr_queue.front()) != NULL) 
   {
      thr_queue.pop();

      // allocate a time quantum
      alarm(kTimeQuantum);

      // return to the next thread's execution
      longjmp(cur_tcb->env_, 1);
   }

   // no threads to schedule, simply return
   cerr << "scheduler: no more threads to schedule" << endl;
   longjmp(main_env, 2);
}

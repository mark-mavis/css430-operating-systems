#include "sthread.cpp"

jmp_buf env;        //The jump environment
jmp_buf envA;       //Thenvironment of  Efunction A
jmp_buf envB;
jmp_buf envC;


static void functionA(){
    int val = 0;
    if( (val = setjmp(envA)) == 0) {
        printf("Hi I am function A\n");
    }else{
        printf("I am back from a long jump in function A\n");
    }
    longjmp (env, 1); 
}

static void functionB(){
    int val = 0;
    if( (val = setjmp(envB)) == 0) {
        printf("Hi I am function B\n");
    }else{
        printf("I am back from a long jump in function B\n");
    }
    longjmp (env, 2); 
}

static void functionC(){
    int val = 0;
    if( (val = setjmp(envC)) == 0) {
        printf("Hi I am function C\n");
    }else{
        printf("I am back from a long jump in function C\n");
    }
    longjmp (env, 3); 
}

int main (){
    int next = 0;
    int val = 0;
    
    //Creating a signal
    signal(SIGALRM, sig_alarm);
    if ( (val=setjmp ( env )) == 0){      //If evnironment returns 0
        printf("Set the environment buffer\n");
        functionA();
        functionB();
        functionC();
    }else if (val == 1){
        printf("Back from longjump %d\n", val);
        functionB();
    }else if (val == 2){
        printf("Back from longjump %d\n", val);
        functionC();
    }else if (val == 3){
        printf("Back from longjump %d\n", val);
        return 0;
    }
    return 0;
}


#include <windows.h>
#include <stdio.h>

//Data is shared by the thread(s)
DWORD Sum;

//The thread will execut in this function
DWORD WINAPI Summation(LPVOID Param){

    DWORD Upper = *(DWORD*) Param;
    for (DWORD i = 1; i <= Upper; i++){
        Sum += i;
    }

    return 0;
}

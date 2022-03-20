#include <stdio.h>
#include <stdbool.h>

/*
    Petersons solution requires the two processes to share two data items
*/

int main(){

    int turn, i, j;
    bool flag[2];

    while(true){
        int turn = j;
        while(flag[j] && turn == j){

            /* critical section */

            flag[i] = false;

            /* remainder section */

        }
    }



    return 0;
}
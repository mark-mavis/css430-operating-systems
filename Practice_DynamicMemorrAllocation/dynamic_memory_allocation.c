
/*
Dynamically Allocate Memory for an array of 5 integers
*/

#include <stdio.h>
#include <stdlib.h>

int main(){

    //Array Pointer
    int* arr;


    // malloc() allocate the memory for 5 integers
    // containing garbage values
    arr = (int*)malloc(5 * sizeof(int));    //5 * 4bytes = 20 bytes

    // Deallocated memory previously allocated by malloc() function
    free(arr);

    // calloc() allocate the memory for 5 integers and 
    // set 0 to all of the values
    arr = (int*)calloc(5, sizeof(int));

    // Deallocate memory previously allocated by calloc() function
    free(arr);

    return 0;

}

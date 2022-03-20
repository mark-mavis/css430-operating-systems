#include <stdio.h>
#include <stdlib.h>
#define COUNT 20

int main(){

    /*-----------------------------------------------------------------------------------------------
            
            The function returns a FILE pointer. Otherwise, NULL is returned and the global variable
            errno is set to indicate the error. 

            fopen()
            fopen("<filename>", "<mode>");
                - "r"   - Opens a file for reading. The file must exist
                - "w"   - Creates an empty file for writing. If a file with the same name already
                            exists, its content is erased and the new file is considered as a new
                            empty file
                - "a"   - Appends to a file. Writing operations, append data at the end of the file.
                            the file is created if it does not exist
                - "r+"  - Opens a file to update both reading and writing. The file must exist
                - "w+"  - Creates an empty file for both reading and writing
                - "a+"  - Opens a file for reading and appending

    -----------------------------------------------------------------------------------------------*/
    int intCounter;
    int date = 2000;

    FILE *fp;
    fp = fopen("openingfile.txt", "w+");
    //<actor> <studio> <Site> <date> <title>
    for(intCounter = 0; intCounter < COUNT; ++intCounter){

        fprintf(fp, "%s - %s - %s - %d\n", "Denzel Washington", "Sony", "The Equilizer", date + intCounter);
    }
}
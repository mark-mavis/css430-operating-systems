#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* structure for passing data to threads */
typedef struct
{
    int row;
    int column;
    int id;
} parameters;


int * result;
int sudoku[9][9] = 
{   {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3,	7, 1, 9, 5, 6, 8, 4, 2},
    {4,	9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};


pthread_t boss;
pthread_t workers[27];
//pthread_mutex_t lock;


void *validate(void *pdata)
{
    parameters *data = (parameters *) pdata;
    int num[9];
    if (data->row==0)
    {
        for (int i=0; i<9; i++)
            num[i] = sudoku[i][data->column-1];
    } 
    else if (data->column == 0)
    {
        for (int i=0; i<9; i++)
            num[i] = sudoku[data->row-1][i];
    }
    else
    {
        int k=0;
        for (int i=0; i<3; i++)
            for (int j=0; j<3; j++)
            {
                num[k] = sudoku[data->row-2+i][data->column-2+j];
                k++;
            }
    }

    int good;
    for (int i=0; i<9; i++)
    {
        good = 0;
        for (int j=0; j<9; j++)
            if (num[j]==i+1) 
                good = 1;
        if (good==0)
            break;
    }
    result[data->id] = good;
    free(data);
    return NULL;
}

void *mainthread(void *vargp)
{
    pthread_attr_t *tattr = malloc(sizeof(pthread_attr_t));
    int ret;
    /* initialize an attribute to the default value */
    ret = pthread_attr_init(tattr);

    result = malloc(sizeof(int)*27);
    for (int i=0; i<27; i++)
        result[i] = 0;

// setup parameters for 9 columns
    for (int i=0; i<9; i++)
    {
        parameters *data = (parameters *) malloc(sizeof(parameters));
        data->row = 0;
        data->column = i+1;
        data->id = i;
        pthread_create(&workers[i], tattr, validate, data);
    }

// setup parameters for 9 rows
    for (int i=0; i<9; i++)
    {
        parameters *data = (parameters *) malloc(sizeof(parameters));
        data->row = i+1;
        data->column = 0;
        data->id = i+9;
        pthread_create(&workers[i+9], tattr, validate, data); 
    }

// setup parameters for 3x3 blocks
    for (int i=0; i<9; i++)
    {
        parameters *data = (parameters *) malloc(sizeof(parameters));
        data->row = (i%3) * 3 + 2;
        data->column = (i/3) * 3 + 2;
        data->id = i + 18;
        pthread_create(&workers[i+18], tattr, validate, data); 
    }

    for (int i=0; i<27; i++)
         pthread_join(workers[i], NULL);

    return NULL;
}


int main(int argv, char** argc)
{
    /*
    char c[128];
    // char file[128] = argc[2];
    // read 9x9 input file
    FILE *fd = fopen("input.csv", "r");
    for (int i = 0 ; i<9; i++)
    {
        fgets(c,128,fd);
        char *token = strtok(c, ",");
        int j=0;
        while (token != NULL)
        {
            sudoku[i][j] = atoi(token);            
            token = strtok(NULL, ",");
            j++;
        }    
    }
    fclose(fd);
    */
   
// display the sudoku array
    for (int i=0; i<9; i++)
    {
        for (int j=0; j<9; j++)
            printf("%d ", sudoku[i][j]);
        printf("\n");
    }

    pthread_attr_t *tattr = malloc(sizeof(pthread_attr_t));
    int ret;
    /* initialize an attribute to the default value */
    ret = pthread_attr_init(tattr);

    pthread_create(&boss, tattr, mainthread, NULL);

    pthread_join(boss, NULL);
    
    int valid = 1;
    for (int i=0; i<27; i++)
        if (result[i]==0)
        {
            printf("Sector %d fails validation.\n", i);
            valid = 0;
        }
        
    if (valid) 
        printf("All sectors pass validation\n");

    return 0;
}
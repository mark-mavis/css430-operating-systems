#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMSIZE 80
typedef struct {
    char name;
    int position;
    int length;
} MEMBLOCK;

static char* memory;
static MEMBLOCK* holes;
static MEMBLOCK* mblock;
int num_of_pos;

void FindHoles()
{
    int mypos = 0;
    holes[0].position = 0;
    holes[0].length = MEMSIZE;

    if (num_of_pos == 0)
        return;
    if (num_of_pos > 0)
    {
        holes[0].length = mblock[0].position;
        for (int i = 0; i < num_of_pos; i++)
        {
            holes[i + 1].position = mblock[i].position + mblock[i].length;
            holes[i + 1].length = mblock[i + 1].position - mblock[i].position - mblock[i].length;
        }
    }
    holes[num_of_pos].position = mblock[num_of_pos-1].position + mblock[num_of_pos-1].length;
    holes[num_of_pos].length = MEMSIZE - mblock[num_of_pos-1].position - mblock[num_of_pos-1].length;
}

void Allocate(char name, int size, char alog)
{
    int i = 0;
    FindHoles();
    if (alog == 'F')
    {
        while (i < num_of_pos + 1)
        {
            if (holes[i].length >= size)
                break;
            i++;
        }
    }
    if (alog == 'B')
    {
        int imin = 0;
        int hsize = 80;
        while (i < num_of_pos + 1)
        {
            if (holes[i].length >= size && holes[i].length < hsize)
            {
                hsize = holes[i].length;
                imin = i;
            }
            i++;
        }
        i = imin;
    }
    if (alog == 'W')
    {
        int imax = 0;
        int hsize = 0;
        while (i < num_of_pos + 1)
        {
            if (holes[i].length > hsize)
            {
                hsize = holes[i].length;
                imax = i;
            }
            i++;
        }
        i = imax;
    }

    if (holes[i].length<size) return;

    if (num_of_pos != 0)
    {
        for (int j = num_of_pos; j >= i; j--)
        {
            mblock[j].position = mblock[j - 1].position;
            mblock[j].length = mblock[j - 1].length;
            mblock[j].name = mblock[j - 1].name;
        }
    }
    mblock[i].position = holes[i].position;
    mblock[i].length = size;
    mblock[i].name = name;
    for (int k = 0; k < size; k++)
    {
        memory[k + mblock[i].position] = name;
    }
    num_of_pos++;
    return;
}

void Free(char name)
{
    int bFound = 0;
    for (int i = 0; i < num_of_pos; i++)
    {
        if (mblock[i].name == name)
        {
            bFound++;
            for (int j = 0; j < mblock[i].length; j++)
                memory[mblock[i].position + j] = '.';
            if (i + 1 == num_of_pos)
                break;
        }
        if (bFound > 0)
        {
            if (mblock[i + 1].name != name)
            {
                mblock[i - bFound + 1].name = mblock[i + 1].name;
                mblock[i - bFound + 1].position = mblock[i + 1].position;
                mblock[i - bFound + 1].length = mblock[i + 1].length;
            }
        }
    }
    num_of_pos -= bFound;
}

void Show()
{
    for (int i = 0; i < MEMSIZE; i++)
        printf("%c", memory[i]);
    printf("\n");
}

void Compact()
{
    for (int i = 0; i < MEMSIZE; i++)
        memory[i] = '.';
    int mypos = 0;
    for (int i = 0; i < num_of_pos; i++)
    {
        mblock[i].position = mypos;
        for (int j = 0; j < mblock[i].length; j++)
            memory[mypos + j] = mblock[i].name;
        mypos += mblock[i].length;
    }
}

void Run(char * cmd)
{
    char name;

    //remove extra spaces
    for (int i = strlen(cmd) - 1; i >= 0; i--)
    {
        if (cmd[i - 1] == ' ' && cmd[i] == ' ')
            memmove(&cmd[i], &cmd[i + 1], strlen(cmd) - i);
    }
    if (cmd[0] == 'A')
    {
        name = cmd[2];
        int p = 4;
        char tmp[8];
        while (cmd[p] != ' ')
        {
            tmp[p - 4] = cmd[p];
            p++;
        }
        tmp[p - 4] = 0;
        int sz = atoi(tmp);
        p++;
        char alog = cmd[p];
        Allocate(name, sz, alog);
    }
    if (cmd[0] == 'F')
    {
        name = cmd[2];
        Free(name);
    }
    if (cmd[0] == 'C')
        Compact();

    if (cmd[0] == 'S')
        Show();

}
void Read(char* file)
{
    FILE* t_fd = fopen(file, "r");
    char name;

    if (t_fd == NULL)
    {
        printf("Error in opening file.\n");
        exit(1);
    }
    char buffer[80];
    while (fgets(buffer, 80, (FILE*)t_fd))
    {
        Run(buffer);
    }

    fclose(t_fd);
}


int main(int argc, char **argv)
{
    memory = (char *)malloc(MEMSIZE * sizeof(char));
    mblock = (MEMBLOCK *)malloc(MEMSIZE * sizeof(MEMBLOCK));
    holes = (MEMBLOCK *)malloc(MEMSIZE * sizeof(MEMBLOCK));
    num_of_pos = 0;

    for (int i = 0; i < MEMSIZE; i++)
    {
        memory[i] = '.';
        mblock[i].position = -1;
        mblock[i].length = 0;
        mblock[i].name = ' ';
        holes[i].position = -1;
        holes[i].length = 0;
        holes[i].name = '.';
    }

    if (argc == 2)
        Read(argv[1]);
    else
    {
        char cmd[100];
        fgets(cmd, 100, stdin);
        while (cmd[0] != 'q')
        {
            Run(cmd);
            fgets(cmd,100,stdin);
        }
    }

    free(memory);
    return 0;
}
#include <stdio.h>
#include <stdbool.h>

int main()
{

    int x = 5;
    int y;
    while (true)
    {
        int count = 0;
        while (count == 0)
            ; /* do nothing */
        //next consumed = buffer[out];
        //out = (out + 1) % BUFFER SIZE;
        count--;
        /* consume the item in next consumed */
    }

    if (x == 5) // The "Check"
    {
        y = x * 2; // The "Act"

        // If another thread changed x in between "if (x == 5)" and "y = x * 2" above,
        // y will not be equal to 10.
    }
}
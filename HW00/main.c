#include <stdio.h>
#include <stdlib.h>

#define MIN_INTPUT 0
#define MAX_INPUT 1000

int main(int argc, char **argv)
{

    char *duplicates = calloc(MAX_INPUT - MIN_INTPUT + 1, 1);

    if (duplicates == NULL)
    {
        fprintf(stderr, "Error allocating memory.\n");
        return -1;
    }

    int last_read = -1;
    while (scanf(" %d", &last_read) == 1)
    {
        if (duplicates[last_read - MIN_INTPUT] < 2)
        {
            duplicates[last_read - MIN_INTPUT]++;
        }
    }

    for (int i = 0; i < MAX_INPUT - MIN_INTPUT + 1; i++)
    {
        if (duplicates[i] >= 2)
        {
            printf(" %d", i + MIN_INTPUT);
        }
    }

    printf("\n");

    free(duplicates);
}
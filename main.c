#include "monty.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    /* Check for correct number of arguments */
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        exit(EXIT_FAILURE);
    }

    /* Open the Monty byte code file */
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    /* Read the file line by line and execute the instructions */

    fclose(file);
    return 0;
}

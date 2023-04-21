#include "pre_headers.h"

void echo(char *command)
{
    command = strtok(NULL, "\"\r\n\t ");
    // printf("hi\n");
    while (command)
    {
        printf("%s ", command);
        command = strtok(NULL, "\"\r\n\t ");
    }
    printf("\n");
}
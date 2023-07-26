#include "functions.h"

void printErrorAndExit(char errMsg[])
{
    printf("\n%s\n", errMsg);
    exit(EXIT_FAILURE);
}
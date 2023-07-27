#include "functions.h"

void printErrorAndExit(char errMsg[])
{
    printf("\n%s\n", errMsg);
    exit(EXIT_FAILURE);
}

const char* printFlag(int flag)
{
    char *sFlag;
    switch (flag)
    {
    case SYN:
        sFlag = "SYN";
        break;
    case ACKSYN:
        sFlag = "ACKSYN";
        break;
    case ACK:
        sFlag = "ACK";
        break;
    case FIN:
        sFlag = "FIN";
        break;
    case DATA:
        sFlag = "DATA";
        break;
    }

    return sFlag;
}

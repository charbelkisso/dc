#ifndef FUNCTIONS_H
#define FUNCTIONS_H
/**
 * includes
 */
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

/**
 * Define constats values
 */
#define PORT 5555
#define MAXMSG 50

/*Main state machine*/
#define THREE_WAY_HAND_SHAKING 1
#define SLIDING_WINDOW 2
#define TEAR_DOWN 3

/*Three way handshaking state machine*/
#define SEND_SYN 1
#define WAIT_SYN 2
#define SEND_ACKSYN 3
#define WAIT_ACKSYN 4
#define SEND_ACK 5
#define WAIT_ACK 6

/*FLAGS*/
#define SYN 0
#define ACKSYN 1
#define ACK 2
#define FIN 3
#define DATA 4

typedef struct
{
    int flags;
    int id;
    int seq;
    int windowsize;
    uint16_t crc;
    char data[MAXMSG];
} rtp;

/*Functions*/
void printErrorAndExit(char errMsg[]);
const char* printFlag(int flag);

#endif
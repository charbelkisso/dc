#include "functions.h"

#define WINDOW_SIZE 3

int createSocket(struct sockaddr_in *serverName, char *hostName)
{
    char *hostAdress = "127.0.0.1"; // localhost
    int fd;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("UDP Socket creation failed\n");
    }

    memset(serverName, 0, sizeof(*serverName));

    serverName->sin_addr.s_addr = inet_addr(hostAdress);
    serverName->sin_family = AF_INET;
    serverName->sin_port = PORT;

    return fd;
}

int threeWayHandShaking(int fd, struct sockaddr_in *serverName, rtp *buffer)
{
    return 0;
}

int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in serverName;
    rtp buffer;

    sock = createSocket(&serverName, "localhost");

    int mainState = THREE_WAY_HAND_SHAKING;

    while (1)
    {
        switch (mainState)
        {
        case THREE_WAY_HAND_SHAKING:
            mainState = (threeWayHandShaking(sock, &serverName, &buffer) > 0) ? SLIDING_WINDOW : -1;
            break;

        case SLIDING_WINDOW:
            mainState = (0) ? TEAR_DOWN : -1;
            break;
        case TEAR_DOWN:
            /* code */
            break;
        default:
            printErrorAndExit("h");
            break;
        }
    }
}

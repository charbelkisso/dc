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

int sendMessage(int fd, struct sockaddr_in *serverName, rtp *buffer)
{
    buffer->windowsize = WINDOW_SIZE;
    // TODO: implement checksum
    buffer->crc = 0;
    socklen_t size = sizeof(struct sockaddr_in);
    return sendto(fd, buffer, sizeof(rtp), 0, (struct sockaddr *)serverName, size);
}

int readMessage(int fd, struct sockaddr_in *serverName, rtp *buffer)
{
    socklen_t size = sizeof(struct sockaddr_in);
    return recvfrom(fd, buffer, sizeof(rtp), 0, (struct sockaddr *)serverName, &size);
}



int threeWayHandShaking(int fd, struct sockaddr_in *serverName, rtp *buffer)
{
    int state = SEND_SYN;
    while (1)
    {
        switch (state)
        {
        case SEND_SYN:
            buffer->flags = SYN;
            if (sendMessage(fd, serverName, buffer) > 0)
            {
                state = WAIT_ACKSYN;
            }
            break;
        case WAIT_ACKSYN:
            /**
             * Client will wait in this state until a SYNACK
             * is received.
             * a time out is set to monitor time spend waiting
             * whene time exceeding timeout state will set back to SEND_SYN
             */
            if (readMessage(fd, serverName, buffer) > 0)
            {
                // if ACKSYN is received -> send ACK
                state = SEND_ACK;
            }
            break;
        case SEND_ACK:
            int ret;
            if ((ret = sendMessage(fd, serverName, buffer)) > 0)
                return ret;
            break;
        }
    }

    return 1;
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

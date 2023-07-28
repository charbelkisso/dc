#include "functions.h"

fd_set set;
int windowSize;
/**
 * function for reading incomming messages
 * if return is based on number of bytes received from client
 * return < 0 -> error
 * return = 0 -> nothing fetched
 * return > 0 -> we have recieved somthing
 */
int readMessage(int fileDescriptor, struct sockaddr_in *clientName, rtp *buffer)
{
	socklen_t size = sizeof(struct sockaddr_in);
	return recvfrom(fileDescriptor, buffer, sizeof(rtp), 0, (struct sockaddr *)clientName, &size);
}

int sendMessage(int fileDescriptor, struct sockaddr_in *clientName, rtp *buffer)
{
	buffer->windowsize = windowSize;
	socklen_t size = sizeof(struct sockaddr_in);
	return sendto(fileDescriptor, buffer, sizeof(rtp), 0, (struct sockaddr *)clientName, size);
}
int threeWayHandShaking(int fileDescriptor, struct sockaddr_in *clientName, rtp *buffer)
{
	int state = WAIT_SYN;

	while (1)
	{
		switch (state)
		{
		case WAIT_SYN:
			printf("Waiting for SYN...\n");
			if (readMessage(fileDescriptor, clientName, buffer) > 0)
			{
				if (buffer->flags != SYN)
				{
					/**
					 * we have received wrong flag
					 * drop package and wait again for SYN
					 */
					printf("received %s, drop package and wait for SYN\n", printFlag(buffer->flags));
					free(buffer);
					continue;
				}

				/**
				 * SYN received, set window size to global variable
				 * send ACKSYN to client
				 */
				printf("SYN received...\n");
				windowSize = buffer->windowsize;
				state = SEND_ACKSYN;
			}
			break;

		case SEND_ACKSYN:
			rtp *temp = (rtp *)malloc(sizeof(rtp));
			temp->flags = ACKSYN;
			if (sendMessage(fileDescriptor, clientName, temp) > 0)
			{
				printf("Sending ACK + SYN to client...\n");
				state = WAIT_ACK;
			}
			free(temp);
			break;

		case WAIT_ACK:
			printf("Waiting for ACK...\n");
			int ret;
			/**
			 * ret < 0 ---> error resend ACK + SYN
			 * ret = 0 ---> empty resend ACK + SYN
			 */
			if ((ret = readMessage(fileDescriptor, clientName, buffer)) <= 0)
			{
				free(buffer);
				state = SEND_ACKSYN;
				continue;
			}

			if (buffer->flags != ACK)
			{
				free(buffer);
				state = SEND_ACKSYN;
				continue;
			}
			return ret; // any number > 0
			break;
		}
	}
}

int makeSocket(unsigned short int port, struct sockaddr_in *clientName)
{
	int sock;

	/* Create a socket. */
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // MAke UDP socket.
	if (sock < 0)
	{
		perror("Could not create a socket\n");
		exit(EXIT_FAILURE);
	}
	memset((char *)clientName, 0, sizeof(*clientName));
	clientName->sin_family = AF_INET;
	clientName->sin_port = htons(port);
	clientName->sin_addr.s_addr = htonl(INADDR_ANY);
	/* Assign an address to the socket by calling bind. */
	if (bind(sock, (struct sockaddr *)clientName, sizeof(*clientName)) < 0)
	{
		perror("Could not bind a name to the socket\n");
		exit(EXIT_FAILURE);
	}
	return (sock);
}

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in clientName;
	rtp buffer;

	// Gives the set, zero bits for all file desctiptors
	FD_ZERO(&set);
	// Sets all bits of sock in set
	FD_SET(sock, &set);
	/* Create a UDP socket */
	sock = makeSocket(PORT, &clientName);

	printf("\n Waiting for client...\n");

	int mainState = THREE_WAY_HAND_SHAKING;
	while (1)
	{
		switch (mainState)
		{
		case THREE_WAY_HAND_SHAKING:
			mainState = (threeWayHandShaking(sock, &clientName, &buffer) > 0) ? SLIDING_WINDOW : -1;
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
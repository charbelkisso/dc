#include "functions.h"

fd_set set;
int mainState;

/**
 * function for reading incomming messages 
 * if return is > 0 means that the package is received correctly
*/
int readMessage(int fileDescriptor, socklen_t size)
{
	// Gives the set, zero bits for all file desctiptors
	FD_ZERO(&set);
	// Sets all bits of sock in set
	FD_SET(fileDescriptor, &set);

	int nOfBytes;
	struct timeval timeout;

	size = sizeof(struct sockaddr_in);
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	rtp *RHeader = (rtp *)malloc(sizeof(rtp));
	if (!RHeader)
		printErrorAndExit("Memory allocation error");
	
		
}

int threeWayHandShaking(int fileDescriptor, socklen_t size)
{
	rtp header;
	int state = WAIT_SYN;
	header.id = 0;
	header.seq = -1;
	header.crc = 0;

	while (1)
	{
		switch (state)
		{
		case WAIT_SYN:
			prinf("Waiting for SYN...\n");
			if (readMessage(fileDescriptor, size) > 0)
			{
				// send ACK+SYN to client
				state = WAIT_ACK;
			}
			break;

		case WAIT_ACK:
			break;
		}
	}

	return 0;
}

int makeSocket(unsigned short int port)
{
	int sock;
	struct sockaddr_in name;

	/* Create a socket. */
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // MAke UDP socket.
	if (sock < 0)
	{
		perror("Could not create a socket\n");
		exit(EXIT_FAILURE);
	}
	memset((char *)&name, 0, sizeof(name));
	/* Give the socket a name. */
	/* Socket address format set to AF_INET for Internet use. */
	name.sin_family = AF_INET;
	/* Set port number. The function htons converts from host byte order to network byte order.*/
	name.sin_port = htons(port);

	/* Set the Internet address of the host the function is called from. */
	/* The function htonl converts INADDR_ANY from host byte order to network byte order. */
	/* (htonl does the same thing as htons but the former converts a long integer whereas
	 * htons converts a short.)
	 */
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	/* Assign an address to the socket by calling bind. */
	if (bind(sock, (struct sockaddr *)&name, sizeof(name)) < 0)
	{
		perror("Could not bind a name to the socket\n");
		exit(EXIT_FAILURE);
	}
	return (sock);
}

int main(int argc, char *argv[])
{
	socklen_t size;
	int sock;
	// Gives the set, zero bits for all file desctiptors
	FD_ZERO(&set);
	// Sets all bits of sock in set
	FD_SET(sock, &set);

	size = sizeof(struct sockaddr_in);
	/* Create a UDP socket */
	sock = makeSocket(PORT);

	printf("\n Waiting for client...\n");

	mainState = THREE_WAY_HAND_SHAKING;
	while (1)
	{
		switch (mainState)
		{
		case THREE_WAY_HAND_SHAKING:
			mainState = (threeWayHandShaking(sock, size) == 0) ? SLIDING_WINDOW : -1;
			break;

		case SLIDING_WINDOW:
			mainState = (threeWayHandShaking(sock, size) == 0) ? TEAR_DOWN : -1;
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
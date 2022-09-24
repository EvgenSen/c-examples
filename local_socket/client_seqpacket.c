/**
 * server_dgram.c
 *
 * Starts the client using "SOCK_SEQPACKET", send a messages to the server and wait answer
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "conn_info.h"

int main(int argc, char **argv)
{
	struct sockaddr_un clnt_addr;
	int                sockfd;
	char               buf[MSG_LEN];

	memset(buf, 0, MSG_LEN);

	if ((sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0)) < 0)
	{
		perror("socket");
		return -1;
	}

	memset(&clnt_addr, 0, sizeof(struct sockaddr_un));
	clnt_addr.sun_family = AF_UNIX;
	strncpy(clnt_addr.sun_path, SOCK_FILE, sizeof(clnt_addr.sun_path) - 1);
	if (connect(sockfd, (struct sockaddr *)&clnt_addr, sizeof(struct sockaddr_un)) < 0)
	{
		perror("connect");
		return -2;
	}

	strcpy(buf, "Test first msg from client");
	if (write(sockfd, buf, strlen(buf) + 1) < 0)
	{
		perror("write");
		return -3;
	}

	// strcpy (buf, "DOWN");
	strcpy(buf, "Test second msg from client");
	if (write(sockfd, buf, strlen(buf) + 1) < 0)
	{
		perror("write");
		return -3;
	}

	strcpy(buf, "END");
	if (write(sockfd, buf, strlen(buf) + 1) < 0)
	{
		perror("write");
		return -3;
	}

	if ((read(sockfd, buf, MSG_LEN)) < 0)
	{
		perror("read");
		return -4;
	}
	printf("Recv reply from server: %s\n", buf);

	close(sockfd);
	return 0;
}

/**
 * server_dgram.c
 *
 * Starts the server using "SOCK_SEQPACKET", receives a messages from the client and sends message back
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
	struct sockaddr_un serv_addr;
	int                serverfd;
	int                clientfd;
	int                down_flag = 0;
	char               buf[MSG_LEN];

	if ((serverfd = socket(AF_UNIX, SOCK_SEQPACKET, 0)) < 0)
	{
		perror("socket\n");
		return -1;
	}

	unlink(SOCK_FILE);
	memset(&serv_addr, 0, sizeof(struct sockaddr_un));
	serv_addr.sun_family = AF_UNIX;
	strncpy(serv_addr.sun_path, SOCK_FILE, sizeof(serv_addr.sun_path) - 1);
	if (bind(serverfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_un)) < 0)
	{
		perror("bind\n");
		return -2;
	}

	if (listen(serverfd, BACKLOG_SIZE) < 0)
	{
		perror("listen");
		return -3;
	}

	while (1)
	{
		printf("Waiting connection\n");
		if ((clientfd = accept(serverfd, NULL, NULL)) < 0)
		{
			perror("accept");
			return -4;
		}

		while (1)
		{
			memset(buf, 0, MSG_LEN);
			if ((read(clientfd, buf, MSG_LEN)) < 0)
			{
				perror("read");
				return -5;
			}
			buf[MSG_LEN - 1] = 0;

			printf("Received from client: %s\n", buf);

			if (!strncmp(buf, "DOWN", MSG_LEN))
			{
				down_flag = 1;
				break;
			}
			if (!strncmp(buf, "END", MSG_LEN))
			{
				printf("Session completed\n");
				break;
			}
		}

		strcpy(buf, "Test session completed");
		if ((write(clientfd, buf, strlen(buf) + 1)) < 0)
		{
			perror("write");
			return -6;
		}

		close(clientfd);

		if (down_flag)
			break;
	}
	close(serverfd);
	unlink(SOCK_FILE);
	return 0;
}

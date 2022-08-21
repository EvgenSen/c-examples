/*
 * server_dgram.c
 *
 * Starts the client using "SOCK_DGRAM", send a message to the server and wait answer
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
	struct sockaddr_un serv_addr, clnt_addr;
	int                sockfd;
	int                msglen, recv_bytes;
	char               buf[MSG_LEN];

	memset(buf, 0, MSG_LEN);

	if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}
	/* Необходимо связать сокет с некоторым локальным адресом,
	   чтобы сервер имел возможность возвратить посланное сообщение.
	   Этот адрес должен быть уникальным в пределах данной операционной
	   системы. Если ответ не планируется это можно пропустить */
	memset(&clnt_addr, 0, sizeof(struct sockaddr_un));
	clnt_addr.sun_family = AF_UNIX;
	sprintf(clnt_addr.sun_path, "/tmp/client%d.sock", getpid());
	if (bind(sockfd, (struct sockaddr *)&clnt_addr, sizeof(struct sockaddr_un)) < 0)
	{
		perror("bind");
		return -2;
	}

	memset(&serv_addr, 0, sizeof(struct sockaddr_un));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, SOCK_FILE);
	strcpy(buf, "Test second msg from client");
	msglen = strlen(buf);
	if (sendto(sockfd, buf, msglen, 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_un)) != msglen)
	{
		perror("sendto");
		return -3;
	}

	if ((recv_bytes = recvfrom(sockfd, buf, MSG_LEN, 0, NULL, 0)) < 0)
	{
		perror("recvfrom");
		return -4;
	}
	printf("Recv reply from server: %s\n", buf);

	close(sockfd);
	unlink(clnt_addr.sun_path);
	return 0;
}

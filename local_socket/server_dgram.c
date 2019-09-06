/*
 * server_dgram.c
 *
 * Starts the server using "SOCK_DGRAM", receives a message from the client and sends it back
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

int main(int argc, char ** argv)
{
    struct sockaddr_un serv_addr, clnt_addr;
    int sockfd;
    int recv_bytes;
    int down_flag = 0;
    socklen_t clnt_addr_len;
    char buf[MSG_LEN];

    if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        return -1;
    }

    unlink(SOCK_FILE);
    memset(&serv_addr, 0, sizeof(struct sockaddr_un));
    serv_addr.sun_family = AF_UNIX;
    strncpy(serv_addr.sun_path, SOCK_FILE, sizeof(serv_addr.sun_path) - 1);
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_un)) < 0)
    {
        perror("bind");
        return -2;
    }

    while(1)
    {
        memset(buf, 0, MSG_LEN);
        clnt_addr_len = sizeof(struct sockaddr_un);
        recv_bytes = recvfrom(sockfd, buf, MSG_LEN, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
        if (recv_bytes < 0)
        {
            perror("recvfrom");
            return -3;
        }
        buf[MSG_LEN-1] = 0;

        printf("Received from client: %s\n", buf);

        if (!strncmp(buf, "DOWN", MSG_LEN)) {
            down_flag = 1;
            break;
        }

        if (sendto(sockfd, buf, recv_bytes, 0, (struct sockaddr*)&clnt_addr, clnt_addr_len) != recv_bytes)
        {
            perror("sendto");
            return -4;
        }

        if (down_flag)
            break;
    }
    close(sockfd);
    unlink(SOCK_FILE);
    return 0;
}

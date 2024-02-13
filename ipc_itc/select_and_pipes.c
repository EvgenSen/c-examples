/**
 * select_and_pipes.c - демонстрация работы select() и pipe()
 *
 * Основной поток ждет в селекте данные из stdin или пайпа.
 * Второй поток периодически отправляет данные в пайп.
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <pthread.h>

int interrupt_stdin_select[2];

void thread_func(void)
{
	printf("[%s()] child started\n", __func__);
	char buffer[32];
	int  i = 0;
	while (1)
	{
		sleep(5);
		sprintf(buffer, "thread_msg_%d", i);
		write(interrupt_stdin_select[1], buffer, strlen(buffer) + 1);
		i++;
	}
}

int main(void)
{
	char           buffer[128];
	int            result, nread;
	fd_set         inputs, testfds;
	struct timeval timeout;
	pthread_t      thread_id;

	printf("[%s()] started\n", __func__);

	if (pipe(interrupt_stdin_select))
	{
		printf("[%s()] pipe create failed: %s\n", __func__, strerror(errno));
	}

	if (pthread_create(&thread_id, NULL, (void *)&thread_func, NULL) != 0)
	{
		perror("Could not create pthread");
		return -1;
	}

	FD_ZERO(&inputs);
	FD_SET(STDIN_FILENO, &inputs);
	FD_SET(interrupt_stdin_select[0], &inputs);
	while (1)
	{
		testfds         = inputs;
		timeout.tv_sec  = 1;
		timeout.tv_usec = 500000;
		result          = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set *)NULL, &timeout);
		switch (result)
		{
		case 0:
			printf("[%s()] timeout\n", __func__);
			break;
		case -1:
			perror("select");
			exit(1);
		default:
			if (FD_ISSET(STDIN_FILENO, &testfds))
			{
				ioctl(0, FIONREAD, &nread);
				if (nread == 0)
				{
					printf("[%s()] keyboard done\n", __func__);
					exit(0);
				}
				nread         = read(0, buffer, nread);
				buffer[nread] = 0;
				printf("[%s()] read %d bytes from keyboard: %s", __func__, nread, buffer);
			}
			else if (FD_ISSET(interrupt_stdin_select[0], &testfds))
			{
				nread = read(interrupt_stdin_select[0], buffer, sizeof(buffer));

				printf("[%s()] read %d bytes from pipe: %s\n", __func__, nread, buffer);
			}
			break;
		}
	}
	return 0;
}

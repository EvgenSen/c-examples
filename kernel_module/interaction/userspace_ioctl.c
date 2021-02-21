/*
 * userspace_ioctl.c - Userspace utility for interacting with the kernel
 * via character device (calls ioctl)
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "interaction_ioctl.h"

#define PATH_LEN 32

int main(int argc, const char *argv[])
{
	char dev_path[PATH_LEN];
	int fd = -1;
	ioctl_data_t data;

	if (argc != 3)
	{
		printf("%s [flag] [data]\n", argv[0]);
		return -1;
	}

	data.flag = atoi(argv[1]);
	strncpy(data.buffer, argv[2], BUFFER_SIZE);

	snprintf(dev_path, PATH_LEN, "/dev/%s", INTER_MOD_DEV_NAME);

	fd = open(dev_path, O_RDWR);
	if(fd < 0)
	{
		printf("Can't open file '%s'\n", dev_path);
		return -2;
	}

	printf("%s: 1 set flag=%d, buff=\"%s\"\n", __FILE__, data.flag, data.buffer);

	ioctl(fd, SET_INTER_MOD_DATA, &data);

	data.flag = 0;
	memset(data.buffer, 0, BUFFER_SIZE);

	printf("%s: 2 nil flag=%d, buff=\"%s\"\n", __FILE__, data.flag, data.buffer);

	ioctl(fd, GET_INTER_MOD_DATA, &data);

	printf("%s: 3 get flag=%d, buff=\"%s\"\n", __FILE__, data.flag, data.buffer);

	close(fd);

	return 0;
}

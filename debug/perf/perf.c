/**
 * perf.c - test program to demonstrate work of perf utility
 *
 * program write data to tmp.txt file with timeout from
 * timeout.txt file
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <unistd.h>

#define DEF_TIMEOUT 100000

int loop_func(void)
{
	int   timeout_us = DEF_TIMEOUT;
	int   i;
	FILE *f = NULL;

	while (1)
	{
		f = fopen("timeout.txt", "r");
		if (!f)
		{
			printf("Error: can't open file timeout.txt!\n");
			exit(-1);
		}

		fscanf(f, "%d", &timeout_us);
		fclose(f);

		if (timeout_us <= 0)
			timeout_us = DEF_TIMEOUT;

		f = fopen("tmp.txt", "w");
		if (!f)
		{
			printf("Error: can't open file tmp.txt!\n");
			exit(-1);
		}

		for (i = 0; i < 10000; i++)
		{
			fprintf(f, "%d\n", i);
		}

		fclose(f);

		usleep(timeout_us);
	}
}

int main()
{
	loop_func();
	return 0;
}

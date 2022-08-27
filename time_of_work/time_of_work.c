/*
 * time_of_work.c - Simple example of calculating the spent time
 *
 * To check the operation time of program and compare the methods
 * used to calculate spent time, you can run this program:
 *
 *     time ./time_of_work
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h> // for struct timeval
#include <time.h>     // for clock_t, time_t, struct timespec

// May be: CLOCK_REALTIME, CLOCK_PROCESS_CPUTIME_ID, CLOCK_MONOTONIC or get id by clock_getcpuclockid()
#define TEST_CLOCK_ID CLOCK_MONOTONIC

void print_time_diff_timeval(struct timeval tv_start, struct timeval tv_end)
{
	int ms = (int)(tv_end.tv_usec - tv_start.tv_usec) / 1000;
	int s  = (int)(tv_end.tv_sec - tv_start.tv_sec);
	if (ms < 0)
		ms = 1 - ms;

	printf("Spent time 1 (timeval):  %d ms\n", s * 1000 + ms);
}

void print_time_diff_timespec(struct timespec st, struct timespec end)
{
	// Time diff in ns:
	unsigned long long delta_timespec = 1000000000ULL * (end.tv_sec - st.tv_sec) + (end.tv_nsec - st.tv_nsec);
	unsigned long      ms             = (delta_timespec) / 1000000;
	unsigned long      ap             = (delta_timespec) % 1000000;

	// printf("Spent time 2 (timespec): %llu ns\n", delta_timespec);
	printf("Spent time 2 (timespec): %lu.%06lu ms\n", ms, ap);
}

int main()
{
	struct timeval  st_timeval, end_timeval;
	clock_t         st_clock, end_clock;
	time_t          st_time, end_time;
	struct timespec st_timespec, end_timespec;

	// Check start time
	gettimeofday(&st_timeval, 0);
	st_clock = clock();
	st_time  = time(NULL);
	clock_gettime(TEST_CLOCK_ID, &st_timespec);

#if 0
	// sleep don't work with clock
	sleep(1);      // 1 c
	usleep(63000); // 63 мс
#else
	int i = 0, j = 0;
	for (i = 0; i < 1000000; i++)
		for (j = 0; j < 2900; j++)
			j = j; // do nothing
#endif

	// Check end time
	gettimeofday(&end_timeval, 0);
	end_clock = clock();
	end_time  = time(NULL);
	clock_gettime(TEST_CLOCK_ID, &end_timespec);

	// Print result
	print_time_diff_timeval(st_timeval, end_timeval);
	print_time_diff_timespec(st_timespec, end_timespec);

	double delta_clock = (double)(end_clock - st_clock) / CLOCKS_PER_SEC;
	double delta_time  = difftime(end_time, st_time);
	printf("Spent time 3 (clock):    %f ms\n", delta_clock * 1000);
	printf("Spent time 4 (time):     %f ms\n", delta_time * 1000);
	return 0;
}

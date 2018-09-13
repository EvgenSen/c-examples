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
#include <sys/time.h>   // for struct timeval
#include <time.h>       // for clock_t

void print_time_diff (struct timeval tv_start, struct timeval tv_end)
{
    int ms = (int)(tv_end.tv_usec - tv_start.tv_usec) / 1000;
    int s  = (int)(tv_end.tv_sec - tv_start.tv_sec);
    if (ms < 0)
        ms = 1 - ms;

    printf("Spent time 1: %d ms\n", s * 1000 + ms);
}

int main(int argc, char *argv[])
{
    struct timeval start_time, end_time;

    gettimeofday(&start_time, 0);
    clock_t start_time_clock = clock();

#if 0
    // sleep don't work with clock
    sleep(1);      // 1 c
    usleep(63000); // 63 мс
#else
    int i = 0, j = 0;
    for (i = 0; i < 1000000; i++)
        for (j = 0; j < 1000; j++)
            j = j; // do nothing
#endif

    gettimeofday(&end_time, 0);
    clock_t end_time_clock = clock();

    print_time_diff(start_time, end_time);

    double time_spent = (double)(end_time_clock - start_time_clock) / CLOCKS_PER_SEC;
    printf("Spent time 2: %f s\n", time_spent);
    return 0;
}

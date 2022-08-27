/*
 * threads.c - демонстрация работы потоков на примере добычи золота из шахты
 *
 * В данной программе создается несколько (THREAD_COUNT) потоков, каждый
 * из которорых уменьшает значение глобальной перменной, пока оно не
 * станет равным 0. Для синхронизации доступа к переменной используются
 * мьютексы.
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define THREAD_COUNT 10  // Общее количество потоков (рабочих добывающих золото)
#define GOLD_ONE     20  // Количество золота, добываемое за раз
#define GOLD_ALL     600 // Общее количество золота

typedef int cb_func(int local_id, int gold_one);

struct thread_data
{
	int      local_id;
	cb_func *get_gold_cb;
};

static int             gold;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/** Get `gold_one` from `gold`
 *
 * @param    local_id      - thread ID
 *           gold_one      - amount of gold getting at a time
 *
 * @return   0  - gold getted successfully
 *          -1  - gold is over
 */
int get_gold(int local_id, int gold_one)
{
	int sleep_us = 0;
	int ret      = -1;

	pthread_mutex_lock(&mutex);

	if (gold > 0)
	{
		gold -= gold_one;
		if (gold < 0)
			gold = 0;
		// На добычу золота юнит тратит от 20 до 40 мс
		sleep_us = rand() % 20000 + 20000;
		usleep(sleep_us);
		printf("[pid = %d, tid = %ld] %2d Юнит забрал золото, потратив %d мкс. В шахте осталось: %d\n", getpid(),
		       syscall(__NR_gettid), local_id + 1, sleep_us, gold);
		ret = 0;
	}

	pthread_mutex_unlock(&mutex);

	return ret;
}

void thread_main(void *arg)
{
	// преобразуем входные аргументы
	struct thread_data *data     = (struct thread_data *)arg;
	int                 sleep_us = 0;

	srand(time(NULL) + data->local_id * 2);

	while (1)
	{
		if (data->get_gold_cb(data->local_id, GOLD_ONE) < 0)
			break;

		sleep_us = rand() % 90000 + 10000;
		// printf("%2d sleep_us = %d\n", local_id + 1, sleep);
		usleep(sleep_us);
	}
}

int main()
{
	int                i;
	pthread_t          thread_id[THREAD_COUNT];
	struct thread_data data[THREAD_COUNT];

	gold = GOLD_ALL;
	printf("Началась добыча золота. Золота в шахте: %d\n", gold);

	for (i = 0; i < THREAD_COUNT; i++)
	{
		data[i].get_gold_cb = get_gold;
		data[i].local_id    = i;
		// создаем потоки и передаем структуру с ID и функцией поулчения золота в качестве аргумента
		if (pthread_create(&thread_id[i], NULL, (void *)&thread_main, &data[i]) != 0)
		{
			perror("Could not create pthread");
			continue;
		}
	}

	for (i = 0; i < THREAD_COUNT; i++)
	{
		// ждем завершения потоков
		pthread_join(thread_id[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	printf("Золота в шахте: %d\n", gold);

	return 0;
}

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

#define THREAD_COUNT 10  // Общее количество потоков (рабочих добывающих золото)
#define ZOLOTO_ONE   20  // Количество золота, добываемое за раз
#define ZOLOTO_ALL   600 // Общее количество золота

static int zoloto;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void thread_main(void * arg)
{
	// преобразуем входные аргументы
	int local_id = * (int *) arg;
	int sleep_us = 0;
	char thread_name[32] = {0};

	sprintf(thread_name, "Unit %d", local_id);
	// Задаем имя текущего потока
	prctl(PR_SET_NAME, thread_name, 0, 0, 0);

	srand(time(NULL) + local_id * 2 );

	while ( 1 )
	{
		pthread_mutex_lock(&mutex);

		if(zoloto > 0 )
		{
			zoloto -= ZOLOTO_ONE;
			// На добычу золота юнит тратит от 20 до 40 мс
			sleep_us = rand()%20000+20000;
			usleep(sleep_us);
			printf("%2d Юнит забрал золото, потратив %d мкс. В шахте осталось: %d\n", local_id + 1, sleep_us, zoloto);
		}
		else
		{
			pthread_mutex_unlock(&mutex);
			break;
		}

		pthread_mutex_unlock(&mutex);

		sleep_us = rand()%90000+10000;
		// printf("%2d sleep_us = %d\n", local_id + 1, sleep);
		usleep(sleep_us);
	}

	sleep(100); // чтобы поток дольше висел и можно было посмотреть в списке процессов
}

int main(int argc, char ** argv)
{
	int i;
	int id[THREAD_COUNT] = {0};
	pthread_t thread_id[THREAD_COUNT];

	zoloto = ZOLOTO_ALL;
	printf("Началась добыча золота. Золота в шахте: %d\n", zoloto);

	for(i=0; i<THREAD_COUNT; i++)
	{
		id[i] = i;
		// создаем потоки и передаем его ID в качестве аргумента
		if( pthread_create(&thread_id[i], NULL, (void *)&thread_main, &id[i]) != 0 )
		{
			perror("Could not create pthread");
			continue;
		}
#if 0
		// задаем имя дочернего потока из родительского
		if ( pthread_setname_np(thread_id[i], "Unit X") != 0 )
		{
			perror("Could not set pthread name");
		}
#endif
	}

	for(i=0; i<THREAD_COUNT; i++)
	{
		// ждем завершения потоков
		pthread_join(thread_id[i], NULL);
	}

	pthread_mutex_destroy(&mutex);
	printf("Золота в шахте: %d\n", zoloto);

	return 0;
}

/*
 * shared_mem_and_semaphore.c - демонстрация взаимодействия процессов при помощи shared memory
 *
 * В данной программе создается несколько (CHILD_COUNT) дочерних процессов, каждый
 * из которорых уменьшает значение перменной из shared memory, пока оно не
 * станет равным 0. Для синхронизации доступа к памяти используется семафор
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>

#define CHILD_COUNT  10  // Общее количество дочерних процессов (рабочих добывающих золото)
#define ZOLOTO_ONE   20  // Количество золота, добываемое за раз
#define ZOLOTO_ALL   600 // Общее количество золота

#define IPC_KEY_FILENAME "shared_ipc_key"  // Имя файла, используемое для генерации ключа
#define SEMAPHORE_NAME   "my_named_semaphore"

int main(int argc, char ** argv)
{
	pid_t pid;
	sem_t *sem;
	int i;
	int *zoloto;   // Указатель на разделяемую память
	int shmid;     // IPC дескриптор для области разделяемой памяти
	char ipc_key_name[] = IPC_KEY_FILENAME;
	key_t key;     // IPC ключ

	srand(time(NULL));

	// if ( argc == 2 )
	// {
	// 	printf("Dropping semaphore...\n");
	// 	if ( (sem = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED )
	// 	{
	// 		perror("sem_open");
	// 		return 1;
	// 	}
	// 	sem_post(sem);
	// 	perror("sem_post");
	// 	printf("Программа запущена\n");
	// 	return 0;
	// }

	printf("start create ipc key...\n");
	if((key = ftok(ipc_key_name,0)) < 0)
	{
		perror("ipc key create failed");
		return 2;
	}
	printf("ipc key successfully created\n");

	printf("start create shared memory...\n");
	if((shmid = shmget(key, sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0)
	{
		if(errno != EEXIST)
		{
			perror("Couldn't create shared memory");
			return 3;
		}
		else
		{
			if((shmid = shmget(key, sizeof(int), 0)) < 0)
			{
				perror("Couldn't get shared memory");
				return 4;
			}
		}
	}
	printf("shared memory successfully created\n");

	printf("start attach shared memory...\n");
	if((zoloto = (int *)shmat(shmid, NULL, 0)) == (int *)(-1))
	{
		perror("Couldn't attach shared memory");
		return 5;
	}
	printf("shared memory successfully attached\n");

	printf("start create semaphore...\n");
	if ( (sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 0)) == SEM_FAILED )
	{
		perror("sem_open");
		return 6;
	}
	printf("semaphore successfully created\n");

	*zoloto = ZOLOTO_ALL;

	for(i = 0; i < CHILD_COUNT; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			/* child process start */
			while (*zoloto > 0 ){
				sem_wait(sem);
				srand(getpid());
				if(*zoloto > 0 ){
					*zoloto -= ZOLOTO_ONE;
					printf("%d Юнит забрал золото, в шахте осталось: %d \n", i+1, *zoloto);
				}
				sem_post(sem);
				sleep(rand()%4+1);
			}
			return 0;
			/* child process end */
		}
	}

	printf("wait child process\n");
	for(i = 0; i < CHILD_COUNT; i++)
	{
		waitpid (-1, NULL, 0);
	}

	printf("Золота в шахте: %d\n", *zoloto);

	printf("start detach shared memory...\n");
	if(shmdt(zoloto) < 0)
	{
		perror("Couldn't detach shared memory");
		return 7;
	}
	printf("shared memory successfully detached\n");

	// Удаляем из системы идентификатор разделяемого сегмента
	shmctl (shmid, IPC_RMID, 0);
	if (sem_close(sem) < 0 )
	{
		perror("sem_close");
	}

	return 0;
}

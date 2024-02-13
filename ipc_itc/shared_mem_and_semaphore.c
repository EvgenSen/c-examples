/**
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
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>

// clang-format off

#define CHILD_COUNT  10  // Общее количество дочерних процессов (рабочих добывающих золото)
#define ZOLOTO_ONE   20  // Количество золота, добываемое за раз
#define ZOLOTO_ALL   600 // Общее количество золота

#define IPC_KEY_FILENAME "shared_ipc_key"  // Имя файла, используемое для генерации ключа
#define SEMAPHORE_NAME   "my_named_semaphore"

// clang-format on

int main(int argc, char *argv[])
{
	pid_t  pid = getpid();
	sem_t *sem;
	int    i;
	int   *zoloto; // Указатель на разделяемую память
	int    shmid;  // IPC дескриптор для области разделяемой памяти
	char   ipc_key_name[] = IPC_KEY_FILENAME;
	key_t  key; // IPC ключ

	srand(time(NULL));

	if (argc == 2 && !strcmp(argv[1], "drop"))
	{
		printf("[%d] Dropping semaphore...\n", pid);
		if ((sem = sem_open(SEMAPHORE_NAME, 0)) == SEM_FAILED)
		{
			perror("sem_open");
			return 1;
		}
		if (sem_post(sem) < 0)
		{
			perror("sem_post");
		}
		printf("[%d] Started!\n", pid);
		return 0;
	}

	printf("[%d] start create ipc key...\n", pid);
	if ((key = ftok(ipc_key_name, 0)) < 0)
	{
		perror("ipc key create failed");
		return 2;
	}
	printf("[%d] ipc key successfully created\n", pid);

	printf("[%d] start create shared memory...\n", pid);
	if ((shmid = shmget(key, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
	{
		if (errno != EEXIST)
		{
			perror("Couldn't create shared memory");
			return 3;
		}
		else
		{
			if ((shmid = shmget(key, sizeof(int), 0)) < 0)
			{
				perror("Couldn't get shared memory");
				return 4;
			}
		}
	}
	printf("[%d] shared memory successfully created\n", pid);

	printf("[%d] start attach shared memory...\n", pid);
	if ((zoloto = (int *)shmat(shmid, NULL, 0)) == (int *)(-1))
	{
		perror("Couldn't attach shared memory");
		return 5;
	}
	printf("[%d] shared memory successfully attached\n", pid);

	printf("[%d] start create semaphore...\n", pid);
	if ((sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0777, 0)) == SEM_FAILED)
	{
		perror("sem_open");
		return 6;
	}
	printf("[%d] semaphore successfully created\n", pid);

	*zoloto = ZOLOTO_ALL;

	for (i = 0; i < CHILD_COUNT; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			pid = getpid();
			printf("[%d] child %d started\n", pid, i + 1);
			/* child process start */
			while (*zoloto > 0)
			{
				sem_wait(sem);
				srand(getpid());
				if (*zoloto > 0)
				{
					*zoloto -= ZOLOTO_ONE;
					printf("[%d] %d Юнит забрал золото, в шахте осталось: %d \n", pid, i + 1, *zoloto);
				}
				sem_post(sem);
				sleep(rand() % 4 + 1);
			}
			return 0;
			/* child process end */
		}
	}

	printf("[%d] wait child process\n", pid);
	for (i = 0; i < CHILD_COUNT; i++)
	{
		waitpid(-1, NULL, 0);
	}

	printf("[%d] Золота в шахте: %d\n", pid, *zoloto);

	printf("[%d] start detach shared memory...\n", pid);
	if (shmdt(zoloto) < 0)
	{
		perror("Couldn't detach shared memory");
		return 7;
	}
	printf("[%d] shared memory successfully detached\n", pid);

	// Удаляем из системы идентификатор разделяемого сегмента
	shmctl(shmid, IPC_RMID, 0);
	if (sem_close(sem) < 0)
	{
		perror("sem_close");
	}

	return 0;
}

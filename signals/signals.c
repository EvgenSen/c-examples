/*
 * signal.c - Simple example of receiving and processing signals
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>

void get_sigusr1(int signum)
{
	printf("%s(): Recv signal %d\n", __FUNCTION__, signum);
}

void get_sigusr2(int signum)
{
	printf("%s(): Recv signal %d\n", __FUNCTION__, signum);
}

void get_sigterm(int signum)
{
	printf("%s(): Recv signal %d\n", __FUNCTION__, signum);
}

int main()
{
	/* Вариант 1 (устаревший) - signal() */
	/* Добавляем обработчик сигнала SIGUSR1.
	 * Вместо обработчика можно передать аргументы:
	 *   SIG_IGN - игнорировать сигнал;
	 *   SIG_DFL - установить обработчик по умолчанию. */
	signal(SIGUSR1, get_sigusr1);
	/* Заменяем обработчик сигнала SIGTERM. */
	signal(SIGTERM, get_sigterm);

	/* Отправляем себе сигнал SIGUSR1 */
	printf("%s(): Send signal SIGUSR1 (%d)\n", __FUNCTION__, SIGUSR1);
	raise(SIGUSR1);

	/* Вариант 2 - sigaction() */
	sigset_t         set;
	struct sigaction act;

	sigemptyset(&set);
	sigaddset(&set, SIGUSR2);

	memset(&act, 0, sizeof(act));
	act.sa_handler = get_sigusr2;
	act.sa_mask    = set;

	sigaction(SIGUSR2, &act, 0);

	/* Отправляем себе сигнала SIGUSR2 */
	printf("%s(): Send signal SIGUSR2 (%d)\n", __FUNCTION__, SIGUSR2);
	raise(SIGUSR2);

	/* Ждем получение сигналов */
	while (1)
	{
		sleep(1);
	}

	return 0;
}

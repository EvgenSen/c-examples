/**
 * backtrace.c - backtrace example
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

int foo(int depth);
int bar(int depth);

/* https://www.gnu.org/software/libc/manual/html_node/Backtraces.html */
void print_backtrace(void)
{
	void  *array[10];
	char **strings;
	int    size, i;

	size    = backtrace(array, 10);
	strings = backtrace_symbols(array, size);
	if (strings != NULL)
	{
		printf("\nObtained %d stack frames.\n", size);
		for (i = 0; i < size; i++)
			printf("%s\n", strings[i]);
	}
	printf("\n");

	free(strings);
}

int foo(int depth)
{
	if (depth > 0)
		return bar(--depth);

	printf("%s: depth = %d\n", __FUNCTION__, depth);
	print_backtrace();
	return 0;
}

int bar(int depth)
{
	if (depth > 0)
		return foo(--depth);

	printf("%s: depth = %d\n", __FUNCTION__, depth);
	print_backtrace();
	return 0;
}

int main()
{
	int depth = 6;

	printf("Started\n");
	print_backtrace();

	foo(depth);

	return 0;
}

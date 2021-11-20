/*
 * array_search.c - Simple example of different
 * searching algorithms and their comparison.
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

/*
 * Results:
 *
 * ARRAY_SIZE = 10000000; MAX_VALUE = 100000
 *   +-----------------+-----------------+-----------------+---------------+--------------------+
 *   | Algorithm       | Element founded | Element correct |      Time, ms | Time with sort, ms |
 *   +-----------------+-----------------+-----------------+---------------+--------------------+
 *   | Linear search   |        +        |        +        |      0.021340 |           0.021340 |
 *   +-----------------+-----------------+-----------------+---------------+--------------------+
 *   | Binary search   |        +        |        +        |      0.002206 |        2039.281307 |
 *   +-----------------+-----------------+-----------------+---------------+--------------------+
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 10000000
#define MAX_VALUE  100000

#define CONST_MILLION 1000000

enum sort_flag_e {
	SORT_NO_NEEDED  = 0,
	SORT_NEEDED     = 1,
};

/** Fill array with random values
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *
 * @return   no returns
 */
void fill_array (int * arr, int arr_size)
{
	int i;

	for (i = 0; i < arr_size; i++)
	{
		arr[i] = rand() % MAX_VALUE;
	}
}

/** Print content of array
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *
 * @return   no returns
 */
void print_array (int * arr, int arr_size)
{
	int i;

	for (i = 0; i < arr_size; i++)
	{
		printf(" %d", arr[i]);
	}

	printf("\n");
}

/** Find element by linear search
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *           val           - target element
 *
 * @return  -1  - element not found
 *           i  - index of target element
 */
int do_linear_search (int * arr, int arr_size, int val)
{
	int i = 0;

	for (i = 0; i < (arr_size - 1); i++)
	{
		if(arr[i] == val)
		{
			return i;
		}
	}

	return -1;
}

/** Find element by binary search
 *
 * @param    arr           - pointer to array (must be sorted)
 *           left          - index of left search border
 *           right         - index of right search border
 *           value         - target element
 *
 * @return  -1  - element not found
 *           i  - index of target element
 */
int binary_search_recursive (int * arr, int left, int right, int value)
{
	int pivot = (left + right) / 2;

	if(arr[pivot] == value)
		return pivot;
	else if((arr[pivot] < value) && ((right - left) > 1))
		return binary_search_recursive(arr, pivot, right, value);
	else if((arr[pivot] > value) && ((right - left) > 1))
		return binary_search_recursive(arr, left, pivot, value);
	else
		return -1;
}

/** Wrapper for use binary_search_recursive with test_search
 *
 * @param    arr           - pointer to array (must be sorted)
 *           arr_size      - array size
 *           val           - target element
 *
 * @return   no returns
 */
int do_binary_search (int * arr, int arr_size, int val)
{
	return binary_search_recursive(arr, 0, arr_size, val);
}

int qsort_cmp (const int *a, const int *b)
{
	return *a - *b;
}

/** Function gets array and function for search. Call search_func()
 *  and print the search time
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *           value         - target element
 *           need_sort     - need sort array before call search_func()
 *           search_func   - pointer to function for searching
 *
 * @return  -1  - Error
 *           i  - index of target element
 */
int test_search (int * arr, int arr_size, int value, int need_sort, int (*search_func)(int *, int, int))
{
	int index = -1;
	struct timespec ts_st, ts_end;
	unsigned long long delta_timespec;
	unsigned long search_ms = 0, sort_ms = 0, summary_ms = 0;
	unsigned long search_fr = 0, sort_fr = 0, summary_fr = 0;

	if(arr == NULL || arr_size == 0 || search_func == NULL)
	{
		fprintf(stderr, "%s(): incorrect input args (%p/%d/%p)\n", __FUNCTION__, arr, arr_size, search_func);
		return -1;
	}

	if(need_sort)
	{
		clock_gettime(CLOCK_MONOTONIC, &ts_st);
		qsort(arr, ARRAY_SIZE, sizeof (int), (int(*) (const void *, const void *)) qsort_cmp);
		clock_gettime(CLOCK_MONOTONIC, &ts_end);

		delta_timespec = 1000000000ULL * (ts_end.tv_sec - ts_st.tv_sec) + (ts_end.tv_nsec - ts_st.tv_nsec);
		sort_ms = (delta_timespec) / CONST_MILLION;
		sort_fr = (delta_timespec) % CONST_MILLION;
	}

	clock_gettime(CLOCK_MONOTONIC, &ts_st);
	index = search_func(arr, arr_size, value);
	clock_gettime(CLOCK_MONOTONIC, &ts_end);

	delta_timespec = 1000000000ULL * (ts_end.tv_sec - ts_st.tv_sec) + (ts_end.tv_nsec - ts_st.tv_nsec);
	search_ms = (delta_timespec) / CONST_MILLION;
	search_fr = (delta_timespec) % CONST_MILLION;

	summary_ms = sort_ms + search_ms;
	summary_fr = sort_fr + search_fr;

	if(summary_fr >= CONST_MILLION )
	{
		summary_fr -= CONST_MILLION;
		summary_ms++;
	}

	printf("       %s        |        %s        | %6lu.%06lu |      %6lu.%06lu |\n",
	       index >= 0 ? "+" : "-",
	       index >= 0 ? (arr[index] == value ? "+" : "-") : "-",
	       search_ms, search_fr, summary_ms, summary_fr);

	return index;
}

int main (int argc, char *argv[])
{
	/* stack size is limited, need to use heap */
	// int arr[ARRAY_SIZE] = {0, };
	int * arr = (int *) calloc(ARRAY_SIZE, sizeof(int));
	int value = -1;
	struct timespec ts_st, ts_end;
	unsigned long long delta_timespec;
	unsigned long ms;
	unsigned long fr;

	srand ( time(NULL) );

	fill_array(arr, ARRAY_SIZE);

	value = rand() % MAX_VALUE;

	printf("Start search value %d in array of %d elements\n", value, ARRAY_SIZE);
	printf("+-----------------+-----------------+-----------------+---------------+--------------------+\n");
	printf("| Algorithm       | Element founded | Element correct |      Time, ms | Time with sort, ms |\n");
	printf("+-----------------+-----------------+-----------------+---------------+--------------------+\n");
	printf("| Linear search   | ");
	test_search(arr, ARRAY_SIZE, value, SORT_NO_NEEDED, do_linear_search);
	printf("+-----------------+-----------------+-----------------+---------------+--------------------+\n");
	printf("| Binary search   | ");
	test_search(arr, ARRAY_SIZE, value, SORT_NEEDED, do_binary_search);
	printf("+-----------------+-----------------+-----------------+---------------+--------------------+\n");

	free(arr);
	return 0;
}

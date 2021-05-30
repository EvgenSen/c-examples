/*
 * array_sort.c - Simple example of different
 * sorting algorithms and their comparison.
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

/*
 * Results:
 *
 * ARRAY_SIZE = 10000; MAX_VALUE = 100
 *   Primitive sort: Array sorted, time: 234414.744529 ms
 *   Bubble sort:    Array sorted, time: 277.357845 ms
 *   Quick sort:     Array sorted, time: 1.570150 ms
 *   Std qsort:      Array sorted, time: 0.821792 ms
 *
 * ARRAY_SIZE = 100000; MAX_VALUE = 100000
 *   Primitive sort: -
 *   Bubble sort:    Array sorted, time: 31368.231846 ms
 *   Quick sort:     Array sorted, time: 13.296424 ms
 *   Std qsort:      Array sorted, time: 11.697078 ms
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define ARRAY_SIZE 100000
#define MAX_VALUE  100000

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

/** Checking, that array sorted from lowest number to greatest
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *
 * @return   1  - True, sorted
 *           0  - False, not sorted
 */
int check_array_sort (int * arr, int arr_size)
{
	int i;

	for (i = 0; i < (arr_size - 1); i++)
	{
		if(arr[i] > arr[i+1])
		{
			return 0;
		}
	}

	return 1;
}

/** Swaps values
 *
 * @param    a   - first value
 *           b   - second value
 *
 * @return   no returns
 */
void swap (int * a, int * b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

/** Sort array by self-written quicksort
 *
 * @param    arr           - pointer to array
 *           start         - first index of the sorted block
 *           last          - last index of the sorted block
 *
 * @return   no returns
 */
void quick_sort_recursive (int *arr, int first, int last)
{
	int pivot = first;    // Первый элемент сортируемого блока выбираем как опорный
	int left = first + 1; // Левая граница - второй элемент
	int right = last;     // Правая граница - последний элемент

	while(1)
	{
		while((arr[right] >= arr[pivot]) && (first < right))
			right--;

		while((arr[left] <= arr[pivot]) && (left < last))
			left++;

		if(left>=right)
			break;

		swap(&arr[left], &arr[right]);
	}

	if(arr[right] < arr[pivot])
		swap(&arr[pivot], &arr[right]);

	pivot = right;

	/* если левый блок имеет больше 1 элемента, сортируем его */
	if(first < pivot - 1)
		quick_sort_recursive(arr, first, pivot - 1);
	/* если правый блок имеет больше 1 элемента, сортируем его */
	if(pivot + 1 < last)
		quick_sort_recursive(arr, pivot + 1, last);

}

int qsort_cmp (const int *a, const int *b)
{
	return *a - *b;
}

/** Sort array by primitive method
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *
 * @return   no returns
 */
void do_primitive_sort (int * arr, int arr_size)
{
	int i = 0;

	while (i < (arr_size - 1))
	{
		if(arr[i] > arr[i+1])
		{
			swap(&arr[i], &arr[i+1]);
			i = 0;
		}
		else
		{
			i++;
		}
	}
}

/** Sort array by bubble sort
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *
 * @return   no returns
 */
void do_bubble_sort (int * arr, int arr_size)
{
	int i, j;

	for (j = 1; j < arr_size; j++)
	{
		for (i = 0; i < (arr_size - j); i++)
		{
			if(arr[i] > arr[i+1])
			{
				swap(&arr[i], &arr[i+1]);
			}
		}
	}
}

/** Wrapper for use quick_sort_recursive with test_sort
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *
 * @return   no returns
 */
void do_quick_sort (int * arr, int arr_size)
{
	quick_sort_recursive(arr, 0, arr_size - 1);
}

/** Wrapper for use qsort() with test_sort()
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *
 * @return   no returns
 */
void do_std_qsort (int * arr, int arr_size)
{
	qsort(arr, arr_size, sizeof (int), (int(*) (const void *, const void *)) qsort_cmp);
}

/** Function gets array and function for sorting. Makes a local copy
 *  of the array, sorts it, validates it, and print the sort time
 *
 * @param    arr           - pointer to array
 *           arr_size      - array size
 *           sort_func     - pointer to function for sorting
 *
 * @return  -1  - Error
 *           0  - Array not sorted
 *           1  - Array sorted
 */
int test_sort (int * arr, int arr_size, void (*sort_func)(int *, int))
{
	int * tmp_arr = NULL;
	int sorted = 0;
	struct timespec ts_st, ts_end;
	unsigned long long delta_timespec;
	unsigned long ms;
	unsigned long fr;

	if(arr == NULL || arr_size == 0 || sort_func == NULL)
	{
		fprintf(stderr, "%s(): incorrect input args (%p/%d/%p)\n", __FUNCTION__, arr, arr_size, sort_func);
		return -1;
	}

	tmp_arr = (int *) malloc( arr_size * sizeof(int) );
	if(tmp_arr == NULL)
	{
		fprintf(stderr, "%s(): malloc failed\n", __FUNCTION__);
		return -1;
	}
	memcpy(tmp_arr, arr, arr_size * sizeof(int));

	clock_gettime(CLOCK_MONOTONIC, &ts_st);
	sort_func(tmp_arr, arr_size);
	clock_gettime(CLOCK_MONOTONIC, &ts_end);

	sorted = check_array_sort(tmp_arr, arr_size);

	// print_array(tmp_arr, ARRAY_SIZE);

	free(tmp_arr);

	delta_timespec = 1000000000ULL * (ts_end.tv_sec - ts_st.tv_sec) + (ts_end.tv_nsec - ts_st.tv_nsec);
	ms = (delta_timespec) / 1000000;
	fr = (delta_timespec) % 1000000;

	printf("Array %s, time: %lu.%lu ms\n", sorted ? "sorted" : "failed", ms, fr);

	return sorted;
}

int main (int argc, char *argv[])
{
	int arr[ARRAY_SIZE] = {0, };

	srand ( time(NULL) );

	fill_array(arr, ARRAY_SIZE);

#if 0 /* too slow */
	printf("Primitive sort: ");
	test_sort(arr, ARRAY_SIZE, do_primitive_sort);
#endif

	printf("Bubble sort:    ");
	test_sort(arr, ARRAY_SIZE, do_bubble_sort);

	printf("Quick sort:     ");
	test_sort(arr, ARRAY_SIZE, do_quick_sort);

	printf("Std qsort:      ");
	test_sort(arr, ARRAY_SIZE, do_std_qsort);

	return 0;
}

/*
 * fop_base.c - base file operations
 *
 * This example shows how to work with plain text files
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#define _GNU_SOURCE // for strcasestr()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <time.h>

#define FILE_NAME "tmp_file.txt"

// #define PRINT_FILE_CONTENT

int readFileLineByLineStaticBuffer(char * filename)
{
	FILE *f = NULL;

	if ((f = fopen(filename, "r")) == NULL)
	{
		perror("Could not open file");
		return -1;
	}

	while(!feof(f))
	{
		char buffer[512] = {0};

		if(fgets(buffer, sizeof(buffer), f))
		{
#ifdef PRINT_FILE_CONTENT
			printf("Readed: %s", buffer);
#endif
		}
	}
	fclose(f);

	return 0;
}

static inline int getStrSize(FILE *f, int one_line)
{
	int n = 0, c = 0;

	if(feof(f))
	{
		return n;
	}

	if(one_line)
	{
		while(!feof(f) && (c = fgetc(f)) != '\n')
		{
			n++;
		}

		n++;

		if(!feof(f))
		{
			fseek(f, -(long)(n), SEEK_CUR);
		}

		return ++n;
	}
	else
	{
		while(!feof(f) && (c = fgetc(f)))
		{
			n++;
		}
	}
	fseek(f, -(long)(n), SEEK_CUR);
	return ++n;
}

int readFileLineByLineDynamicBuffer(char * filename)
{
	FILE * f = NULL;

	if ((f = fopen(filename, "r")) == NULL)
	{
		perror("Could not open file");
		return -1;
	}

	while(!feof(f))
	{
		int str_len = getStrSize(f, 1);
		if (str_len <= 0)
			break;

		char buffer[str_len];
		memset (buffer, 0, sizeof(buffer));

		if (fgets(buffer, sizeof (buffer), f))
		{
#ifdef PRINT_FILE_CONTENT
			printf("Readed: %s", buffer);
#endif
		}
	}
	fclose(f);

	return 0;
}

static int getParamValue(char * filename, char * param_name, char * param_value)
{
	FILE * f = NULL;

	if ((f = fopen(filename, "r")) == NULL)
	{
		perror("Could not open file");
		return -1;
	}

	while(!feof(f))
	{
		int str_len = getStrSize(f, 1);
		if (str_len <= 0)
			break;

		char buffer[str_len];
		memset (buffer, 0, sizeof(buffer));

		if (fgets(buffer, sizeof (buffer), f))
		{
			char *ptr = NULL;
			if ((ptr = strcasestr(buffer, param_name)))
			{
				ptr += strlen(param_name);

				while(*ptr == '\t' || *ptr == ' ' || *ptr == '=')
				{
					ptr++;
				}

				if(ptr[strlen(ptr) - 1] == '\n')
					ptr[strlen(ptr) - 1] = '\0';

				// printf("param_value = '%s' \n", ptr);

				strcpy(param_value, ptr);

				fclose(f);
				return 0;
			}

		}
	}
	fclose(f);

	return -1;
}

int writeDataToFile(char * filename)
{
	FILE * f = NULL;
	char buffer[512] = {0};

	strcpy(buffer, "Buffered string");

	if ((f = fopen(filename, "w")) == NULL)
	{
		perror("Could not open file");
		return -1;
	}

	/* Write to file "Buffered string" (without new line) */
	fputs(buffer, f);
	/* Write to file "\n" */
	fputc ('\n', f);

#if 0
	/* Write to file all contents of buffer "Buffered string\0\0\0..." (without new line) */
	fwrite(buffer, sizeof(buffer), 1, f);
	/* Write to file "\n" */
	fputc ('\n', f);
#endif

	/* Write to file "Buffered string" (without new line)*/
	fwrite(buffer, strlen(buffer), 1, f);
	/* Write to file "\n" */
	fputc ('\n', f);

	/* Write to file "\tmp_param = 123\n" */
	fprintf(f, "\t%s = %d\n", "tmp_param", 123);

	fclose(f);

	return 0;
}

void print_time_diff_timespec (struct timespec st, struct timespec end)
{
	// Time diff in ns:
	unsigned long long delta_timespec =
	    1000000000ULL * (end.tv_sec - st.tv_sec) + (end.tv_nsec - st.tv_nsec);
	unsigned long ms = (delta_timespec) / 1000000;
	unsigned long ap = (delta_timespec) % 1000000;

	//printf("Spent time: %llu ns\n", delta_timespec);
	printf("Spent time: %lu.%06lu ms\n", ms, ap);
}

int main(int argc, const char *argv[])
{
	char param_value[256] = {0};
	struct timespec st_timespec, end_timespec;

	writeDataToFile(FILE_NAME);

	/* Читаем строки из файла в фиксированный буфер.
	 * Если строка больше размера буфера,
	 * то она будет считана за несколько итераций */
	clock_gettime (CLOCK_MONOTONIC, &st_timespec);
	readFileLineByLineStaticBuffer(FILE_NAME);
	clock_gettime (CLOCK_MONOTONIC, &end_timespec);
	print_time_diff_timespec(st_timespec, end_timespec);
	printf("\n");

	/* Читаем строки из файла в динамический буфер.
	 * Всегда считывается одна строка за раз,
	 * но процесс занимает больше времени */
	clock_gettime (CLOCK_MONOTONIC, &st_timespec);
	readFileLineByLineDynamicBuffer(FILE_NAME);
	clock_gettime (CLOCK_MONOTONIC, &end_timespec);
	print_time_diff_timespec(st_timespec, end_timespec);
	printf("\n");

	if (getParamValue(FILE_NAME, "tmp_param", param_value) < 0)
	{
		printf("Can't get param_value\n");
	}
	else
	{
		printf("param_value = '%s' \n", param_value);
	}

	/* Remove file */
	unlink(FILE_NAME);

	return 0;
}

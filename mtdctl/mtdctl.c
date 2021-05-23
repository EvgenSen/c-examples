/*
 * mtdctl.c - MTD Control utility
 *
 * This example shows how to write and read from MTD devices
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>

#include "mtdctl.h"

FILE *log_file;

#define print_dbg(...) fprintf (log_file, __VA_ARGS__)

/** Fill buffer with random values
 *
 * @param      buffer      - pointer to buffer
 *             buffer_len  - buffer size
 *
 * @return     no returns
 */
void fill_buffer (unsigned char * buffer, size_t buffer_len)
{
	int i;

	for (i = 0; i < buffer_len; i++)
	{
		buffer[i] = rand() % 255;
	}
}

/** Read data from MTD block device to buffer
 *
 * @param      offset      - set read offset
 *             mtd_name    - MTD device name
 *             buffer      - pointer to buffer
 *             buffer_len  - buffer size
 *
 * @return     on success, the number of bytes read
 *             on error, negative error code
 */
int mtd_block_read (int offset, const char * mtd_name, unsigned char * buffer, size_t buffer_len)
{
	size_t sz = 0;
	int mtd = open(mtd_name, O_RDWR);

	if(mtd <= 0)
	{
		perror("Can't open file");
		return -1;
	}

	lseek(mtd, offset, SEEK_SET);
	if((sz = read(mtd, buffer, buffer_len) ) < 0)
	{
		perror("Can't read from file");
		close(mtd);
		return -2;
	}
	else
	{
		;//print_dbg("Successful read %zu bytes from addr %#010x on %s\n", sz, offset, mtd_name);
	}

	close(mtd);
	return sz;
}

/** Write data to MTD block device from buffer
 *
 * @param      offset      - set write offset
 *             mtd_name    - MTD device name
 *             buffer      - pointer to buffer
 *             buffer_len  - buffer size
 *
 * @return     on success, the number of bytes written
 *             on error, negative error code
 */
int mtd_block_write (int offset, const char * mtd_name, const unsigned char * buffer, size_t buffer_len)
{
	size_t sz = 0;
	int mtd = open(mtd_name, O_RDWR);

	// if(!strcmp(mtd_name, "/dev/mtdblock0"))
	// {
	// 	fprintf(stderr, "Error: Don't write to %s !!!\n", mtd_name);
	// 	return -3;
	// }

	if(mtd <= 0)
	{
		perror("Can't open file");
		return -1;
	}

	lseek(mtd, offset, SEEK_SET);
	if((sz = write(mtd, buffer, buffer_len) ) < 0)
	{
		perror("Can't write to file");
		close(mtd);
		return -2;
	}
	else
	{
		;//print_dbg("Successful write %zu bytes to addr %#010x on %s\n", sz, offset, mtd_name);
	}

	fsync(mtd);
	close(mtd);
	return sz;
}

/** Get information about MTD char device
 *
 * @param      mtd_name    - MTD device name
 *
 * @return     on success, 0
 *             on error, negative error code
 */
int mtd_char_get_info (const char * mtd_name)
{
	mtd_info_t mtd_info;
	int mtd = open(mtd_name, O_RDWR);

	if(mtd <= 0)
	{
		perror("Can't open file");
		return -1;
	}

	ioctl(mtd, MEMGETINFO, &mtd_info);

	print_dbg("MTD type: %u\n", mtd_info.type);
	print_dbg("MTD total size : %u bytes\n", mtd_info.size);
	print_dbg("MTD erase size : %u bytes\n", mtd_info.erasesize);
	print_dbg("MTD OOB size   : %u bytes\n", mtd_info.oobsize);
	// print_dbg("MTD ECC type   : %u\n", mtd_info.ecctype);
	// print_dbg("MTD ECC size   : %u bytes\n", mtd_info.eccsize);

	close(mtd);
	return 0;
}

/** Write random data to MTD device, read and compare
 *
 * @param      mtd_name    - MTD device name
 *
 * @return     on success, 0
 *             on error, error count
 */
int do_action_test (const char * mtd_name)
{
	int i = 0;
	int offset = 0;
	int err = 0;
	unsigned char read_buffer[BUFF_SIZE] = {0};
	unsigned char write_buffer[BUFF_SIZE] = {0};

	for (i=0; i<16*64; i++)
	{
		int j;

		memset(read_buffer, 0, BUFF_SIZE);
		fill_buffer(write_buffer, BUFF_SIZE);

		mtd_block_write(offset, mtd_name, write_buffer, BUFF_SIZE);
		mtd_block_read(offset, mtd_name, read_buffer, BUFF_SIZE);

		if(memcmp(write_buffer, read_buffer, BUFF_SIZE) == 0)
		{
			;//print_dbg("Offset 0x%x OK\n", offset);
		}
		else
		{
			print_dbg("Offset 0x%x FAIL\n", offset);
			err++;
		}

		for (j=0; j<BUFF_SIZE; j++)
		{
			if(write_buffer[j] != read_buffer[j])
			{
				print_dbg("%4d: 0x%x <> 0x%x\n", j, write_buffer[j], read_buffer[j]);
				break;
			}
		}

		offset += BUFF_SIZE;
	}
	print_dbg("Test end, error count = %d\n", err);
	return err;
}

/** Print usage message
 *
 * @param      app_name    - argv[0] or other app name
 *
 * @return     no returns
 */
static void usage(const char *app_name)
{
	fprintf(stderr, "\n");
	fprintf(stderr, "MTD Control utility, build at %s %s\n", __TIME__, __DATE__);
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "    %s [ACTION] [OPTS]\n", app_name);
	fprintf(stderr, "ACTION:\n");
	fprintf(stderr, "    -r   read form MTD device\n");
	fprintf(stderr, "    -w   write to MTD device\n");
	fprintf(stderr, "    -i,  get information of MTD device\n");
	fprintf(stderr, "    -t   test MTD device\n");
	fprintf(stderr, "OPTS:\n");
	fprintf(stderr, "    -o   offset in HEX (default:0x0)\n");
	fprintf(stderr, "    -m   MTD device name\n");
	fprintf(stderr, "    -v   file for write log msg\n");
	// fprintf(stderr, "    -f   file for read/write data to/from mtd\n"); // TODO
	fprintf(stderr, "    -s   string for write\n");
	fprintf(stderr, "    -c   count of iterations (for test MTD)\n");
	fprintf(stderr, "\n");
}

int main(int argc, char *argv[])
{
	int opt;
	int act = ACT_NONE;
	int offset = 0;
	int count = 1;
	int i = 0;
	const char *file_name = NULL;
	const char *mtd_name = NULL;
	const char *user_str = NULL;
	unsigned char buffer[BUFF_SIZE] = {0};

	log_file = stderr;

	/* Check args count */
	if(argc == 1)
	{
		fprintf(stderr, "Error: no args\n");
		usage(argv[0]);
		exit(1);
	}

	/* Parsing args */
	while ((opt = getopt(argc, argv, "rwito:m:v:d:s:c:")) != -1)
	{
		switch (opt)
		{
		case 'r':
			if (act != ACT_NONE)
			{
				fprintf(stderr, "Error: double action\n");
				usage(argv[0]);
				exit(1);
			}
			act = ACT_READ;
			break;
		case 'w':
			if (act != ACT_NONE)
			{
				fprintf(stderr, "Error: double action\n");
				usage(argv[0]);
				exit(1);
			}
			act = ACT_WRITE;
			break;
		case 'i':
			if (act != ACT_NONE)
			{
				fprintf(stderr, "Error: double action\n");
				usage(argv[0]);
				exit(1);
			}
			act = ACT_INFO;
			break;
		case 't':
			if (act != ACT_NONE)
			{
				fprintf(stderr, "Error: double action\n");
				usage(argv[0]);
				exit(1);
			}
			act = ACT_TEST;
			break;
		case 'o':
			offset = atoi(optarg);
			if (offset < 0)
			{
				fprintf(stderr, "Error: incorrect offset\n");
				usage(argv[0]);
				exit(1);
			}
			break;
		case 'm':
			mtd_name = optarg;
			break;
		case 'v':
			if ((log_file = fopen(optarg, "w")) == NULL)
			{
				perror("Can't create output file");
				exit(1);
			}
			break;
		case 'd':
			file_name = optarg;
			break;
		case 's':
			user_str = optarg;
			break;
		case 'c':
			count = atoi(optarg);
			if (count < 0)
			{
				fprintf(stderr, "Error: incorrect count\n");
				usage(argv[0]);
				exit(1);
			}
			break;
		case 'h':
			usage(argv[0]);
			exit(0);
		default:
			usage(argv[0]);
			exit(1);
		}
	}

	if(mtd_name == NULL)
	{
		fprintf(stderr, "Error: empty MTD device name\n");
		usage(argv[0]);
		exit(1);
	}

	srand(time(NULL));

	/* Start action */
	switch (act)
	{
	case ACT_READ:
		if(mtd_block_read(offset, mtd_name, buffer, BUFF_SIZE) > 0)
		{
			print_dbg("Read: \"%s\"\n", buffer);
		}
		break;
	case ACT_WRITE:
		if(user_str == NULL)
		{
			fprintf(stderr, "Error: empty string for write\n");
			usage(argv[0]);
			exit(1);
		}
		mtd_block_write(offset, mtd_name, (unsigned char *) user_str, strlen(user_str));
		break;
	case ACT_INFO:
		mtd_char_get_info(mtd_name);
		break;
	case ACT_TEST:
		for (i=0; i<count; i++)
		{
			print_dbg("%d/%d: Start test iteration\n", i, count);
			do_action_test(mtd_name);
		}
		break;
	default:
		fprintf(stderr, "Error: incorrect action\n");
		usage(argv[0]);
		exit(1);
	}

	if(log_file && log_file != stderr)
	{
		fclose((FILE *)log_file);
		log_file = NULL;
	}

	return 0;
}

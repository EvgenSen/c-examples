/**
 * mtdctl.c - MTD Control utility
 *
 * This example shows how to write and read from MTD devices,
 * get info about char MTD and test block MTD.
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#define _XOPEN_SOURCE 500 // need for pwrite() and pread()

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

#define print_dbg(...) fprintf(log_file, __VA_ARGS__)

/** Fill buffer with random values
 *
 * @param      buffer      - pointer to buffer
 *             buffer_len  - buffer size
 *
 * @return     no returns
 */
void fill_buffer(unsigned char *buffer, ssize_t buffer_len)
{
	int i;

	for (i = 0; i < buffer_len; i++)
	{
		buffer[i] = rand() % 255;
	}
}

/** Print data from buffer
 *
 * @param      buffer      - pointer to buffer
 *             buffer_len  - buffer size
 *             offset      - start offset (may be 0)
 *             print_nice  - print as hexdump
 *
 * @return     no returns
 */
void print_data(int offset, int print_nice, unsigned char *buffer, ssize_t buffer_len)
{
	int i = 0;

	if (print_nice)
	{
		for (i = 0; i < buffer_len; i++)
		{
			if (i % 16 == 0)
				print_dbg("\n0x%08x:", (unsigned int)(offset + i));
			print_dbg(" %02x", buffer[i]);
		}
		print_dbg("\n");
	}
	else
	{
		print_dbg("\n0x%08x: '%s'\n", offset, buffer);
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
int mtd_block_read(int offset, const char *mtd_name, unsigned char *buffer, size_t buffer_len)
{
	ssize_t sz  = 0;
	int     mtd = open(mtd_name, O_RDWR);

	if (mtd <= 0)
	{
		perror("Can't open MTD block device");
		return -1;
	}

	lseek(mtd, offset, SEEK_SET);
	if ((sz = read(mtd, buffer, buffer_len)) < 0)
	{
		perror("Can't read from MTD block device");
		close(mtd);
		return -2;
	}
	else
	{
		; // print_dbg("Successful read %zu bytes from addr %#010x on %s\n", sz, offset, mtd_name);
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
int mtd_block_write(int offset, const char *mtd_name, const unsigned char *buffer, size_t buffer_len)
{
	ssize_t sz  = 0;
	int     mtd = open(mtd_name, O_RDWR);

	// if(!strcmp(mtd_name, "/dev/mtdblock0"))
	// {
	// 	fprintf(stderr, "Error: Don't write to %s !!!\n", mtd_name);
	// 	return -3;
	// }

	if (mtd <= 0)
	{
		perror("Can't open MTD block device");
		return -1;
	}

	lseek(mtd, offset, SEEK_SET);
	if ((sz = write(mtd, buffer, buffer_len)) < 0)
	{
		perror("Can't write to MTD block device");
		close(mtd);
		return -2;
	}
	else
	{
		; // print_dbg("Successful write %zu bytes to addr %#010x on %s\n", sz, offset, mtd_name);
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
int mtd_char_get_info(const char *mtd_name)
{
	mtd_info_t mtd_info;
	int        mtd = open(mtd_name, O_RDWR);

	if (mtd <= 0)
	{
		perror("Can't open MTD char device");
		return -1;
	}

	if (ioctl(mtd, MEMGETINFO, &mtd_info) != 0)
	{
		perror("MEMGETINFO fail");
		close(mtd);
		return -2;
	}

	print_dbg("MTD type: %u\n", mtd_info.type);
	print_dbg("MTD total size : %u bytes\n", mtd_info.size);
	print_dbg("MTD erase size : %u bytes\n", mtd_info.erasesize);
	print_dbg("MTD OOB size   : %u bytes\n", mtd_info.oobsize);
	// print_dbg("MTD ECC type   : %u\n", mtd_info.ecctype);
	// print_dbg("MTD ECC size   : %u bytes\n", mtd_info.eccsize);

	close(mtd);
	return 0;
}

/** Read data from MTD char device to buffer
 *
 * @param      offset      - set read offset
 *             mtd_name    - MTD device name
 *             buffer      - pointer to buffer
 *             buffer_len  - buffer size
 *
 * @return     on success, the number of bytes read
 *             on error, negative error code
 */
int mtd_char_read(int offset, const char *mtd_name, unsigned char *buffer, ssize_t buffer_len)
{
	mtd_info_t mtd_info;
	int        mtd = open(mtd_name, O_RDWR);

	if (mtd <= 0)
	{
		perror("Can't open MTD char device");
		return -1;
	}

	if (ioctl(mtd, MEMGETINFO, &mtd_info) != 0)
	{
		perror("MEMGETINFO fail");
		close(mtd);
		return -2;
	}

	/* Make sure device page sizes are valid */
	if (mtd_info.oobsize != 64 && mtd_info.oobsize != 16 && mtd_info.oobsize != 8)
	{
		fprintf(stderr, "Unknown flash (not normal NAND)\n");
		close(mtd);
		return -3;
	}

	/* Read page data and exit on failure */
	if (pread(mtd, buffer, buffer_len, offset) != buffer_len)
	{
		perror("Can't read from MTD char device");
		close(mtd);
		return -4;
	}
	else
	{
		; // print_dbg("Successful read %zu bytes from addr %#010x on %s\n", sz, offset, mtd_name);
	}

	close(mtd);
	return buffer_len;
}

/** Write data to MTD char device from buffer
 *
 * @param      offset      - set write offset
 *             mtd_name    - MTD device name
 *             buffer      - pointer to buffer
 *             buffer_len  - buffer size
 *
 * @return     on success, the number of bytes write
 *             on error, negative error code
 */
#if 0 // Function not tested
int mtd_char_write (int offset, const char * mtd_name, const unsigned char * buffer, size_t buffer_len)
{
	mtd_info_t mtd_info;
	int mtd = open(mtd_name, O_RDWR);
	int ret = 0;
	unsigned char local_buffer[PAGE_SIZE];

	if(mtd <= 0)
	{
		perror("Can't open MTD char device");
		return -1;
	}

	if (ioctl(mtd, MEMGETINFO, &mtd_info) != 0)
	{
		perror("MEMGETINFO fail");
		close(mtd);
		return -2;
	}

	/* Make sure device page sizes are valid */
	if (mtd_info.oobsize != 64 &&
	    mtd_info.oobsize != 16 &&
	    mtd_info.oobsize != 8  )
	{
		fprintf(stderr, "Unknown flash (not normal NAND)\n");
		close(mtd);
		return -3;
	}

	// erase_info_t ei;                  // the erase block structure
	// ei.length = mtd_info.erasesize;   // set the erase block size
	// for(ei.start = 0; ei.start < mtd_info.size; ei.start += ei.length)
	// {
	// 	ioctl(mtd, MEMUNLOCK, &ei);
	// 	ioctl(mtd, MEMERASE, &ei);
	// }

	memset(local_buffer, 0xff, PAGE_SIZE);
	memcpy(local_buffer, buffer, buffer_len);

	/* Read page data and exit on failure */
	if ( (ret = pwrite(mtd, local_buffer, PAGE_SIZE, offset)) != PAGE_SIZE)
	{
		perror("Can't write to MTD char device");
		fprintf(stderr, "pwrite ret = %d, %d\n", ret, errno);
		close(mtd);
		return -4;
	}
	else
	{
		;//print_dbg("Successful read %zu bytes from addr %#010x on %s\n", sz, offset, mtd_name);
	}

	close(mtd);
	return buffer_len;
}
#endif

/** Write random data to MTD device, read and compare
 *
 * @param      offset      - set start offset
 *             mtd_name    - MTD device name
 *             page_cnt    - page count for test
 *
 * @return     on success, 0
 *             on error, error count
 */
int do_action_test(int offset, const char *mtd_name, int page_cnt)
{
	int           i                       = 0;
	int           err                     = 0;
	unsigned char read_buffer[BUFF_SIZE]  = {0};
	unsigned char write_buffer[BUFF_SIZE] = {0};

	for (i = 0; i < page_cnt; i++)
	{
		// int j;

		memset(read_buffer, 0x0, BUFF_SIZE);
		fill_buffer(write_buffer, BUFF_SIZE);

		mtd_block_write(offset, mtd_name, write_buffer, BUFF_SIZE);
		mtd_block_read(offset, mtd_name, read_buffer, BUFF_SIZE);

		if (memcmp(write_buffer, read_buffer, BUFF_SIZE) == 0)
		{
			; // print_dbg("Offset 0x%x OK\n", offset);
		}
		else
		{
			print_dbg("Offset 0x%x FAIL (WR 0x%02x <> 0x%02x)\n", offset, write_buffer[0], read_buffer[0]);
			err++;
		}

		/* print all mismatched values */
		// for (j=0; j<BUFF_SIZE; j++)
		// {
		// 	if(write_buffer[j] != read_buffer[j])
		// 	{
		// 		print_dbg("%4d: 0x%x <> 0x%x\n", j, write_buffer[j], read_buffer[j]);
		// 		break;
		// 	}
		// }

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
	fprintf(stderr, "    -p   print nice (hexdump)\n");
	fprintf(stderr, "\n");
}

int main(int argc, char *argv[])
{
	int opt;
	int act    = ACT_NONE;
	int offset = 0;
	int count  = 1;
	int i      = 0;
	int ret;
	int print_nice = 0;
	// const char *  file_name         = NULL;
	const char   *mtd_name          = NULL;
	const char   *user_str          = NULL;
	unsigned char buffer[BUFF_SIZE] = {0};

	log_file = stderr;

	/* Check args count */
	if (argc == 1)
	{
		fprintf(stderr, "Error: no args\n");
		usage(argv[0]);
		exit(1);
	}

	/* Parsing args */
	while ((opt = getopt(argc, argv, "rwito:m:v:s:c:p")) != -1)
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
			offset = (int)strtol(optarg, NULL, 16);
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
		// case 'd':
		// 	file_name = optarg;
		// 	break;
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
		case 'p':
			print_nice = 1;
			break;
		case 'h':
			usage(argv[0]);
			exit(0);
		default:
			usage(argv[0]);
			exit(1);
		}
	}

	if (mtd_name == NULL)
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
		if (strstr(mtd_name, "block") != NULL)
		{
			ret = mtd_block_read(offset, mtd_name, buffer, BUFF_SIZE);
		}
		else
		{
			ret = mtd_char_read(offset, mtd_name, buffer, BUFF_SIZE);
		}

		if (ret > 0)
		{
			print_data(offset, print_nice, buffer, BUFF_SIZE);
		}
		break;
	case ACT_WRITE:
		if (user_str == NULL)
		{
			fprintf(stderr, "Error: empty string for write\n");
			usage(argv[0]);
			exit(1);
		}
		if (strstr(mtd_name, "block") != NULL)
		{
			ret = mtd_block_write(offset, mtd_name, (unsigned char *)user_str, strlen(user_str));
		}
		// else
		// {
		// 	ret = mtd_char_write(offset, mtd_name, (unsigned char *) user_str, strlen(user_str));
		// }
		break;
	case ACT_INFO:
		mtd_char_get_info(mtd_name);
		break;
	case ACT_TEST:
		for (i = 0; i < count; i++)
		{
			print_dbg("%d/%d: Start test iteration\n", i, count);
			do_action_test(offset, mtd_name, 16 * 64 /* 16 blocks of 64 pages */);
		}
		break;
	default:
		fprintf(stderr, "Error: incorrect action\n");
		usage(argv[0]);
		exit(1);
	}

	if (log_file && log_file != stderr)
	{
		fclose((FILE *)log_file);
		log_file = NULL;
	}

	return 0;
}

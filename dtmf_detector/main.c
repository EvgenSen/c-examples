/**
 * main.c - wrapper over dsp.c and wave.c
 *
 * This example open WAV file, read data, find DTMF signals,
 * remove it and save data to new file.
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#include "wave.h"
#include "dsp.h"

#define OUTPUT_FILENAME "out.wav"

/* Read file with 1 channel 16 bit data
   Input arg:  filename  - WAV file name
   Output arg: hdr       - header of WAV file
               wave_data - data array
   Return:     0 - Success, otherwise - error */
int read_wave_file(char *filename, struct wave_header *hdr, int16_t **wave_data)
{
	FILE *fd;

	if ((fd = fopen(filename, "rb")) == NULL)
	{
		printf("Error: Can't open file %s (%s)\n", filename, strerror(errno));
		return -1;
	}

	fread(hdr, sizeof(struct wave_header), 1, fd);

	if(wave_check_header(*hdr) < 0)
	{
		printf("Error: Invalid file header\n");
		fclose(fd);
		return -3;
	}

	wave_print_header_info(*hdr);

	*wave_data = wave_read_data(*hdr, fd);
	if (*wave_data == NULL)
	{
		printf("Error: Can't read file %s\n", filename);
		fclose(fd);
		return -3;
	}

	fclose(fd);

	return 0;
}

/* Write to WAV file 1 channel 16 bit data
   Input arg:  filename  - WAV file name
               hdr       - header of WAV file
               wave_data - data array
   Return:     0 - Success, otherwise - error */
int write_wave_file(char *filename, struct wave_header *hdr, int16_t *wave_data)
{
	FILE *fd;
	int i;
	int16_t * wave_data_ptr = NULL;
	long num_samples = (8 * hdr->data_hdr.len) / (hdr->fmt_hdr.nchan * hdr->fmt_hdr.bits_per_sample);

	if ((fd = fopen(filename, "wb")) == NULL)
	{
		printf("Error: Can't create file %s (%s)\n", filename, strerror(errno));
		return -1;
	}

	fwrite((void*)hdr, sizeof(struct wave_header), 1, fd);

	wave_data_ptr = wave_data;
	for (i = 1; i <= num_samples; i++)
	{
		char data_buffer[2];
		data_buffer[0] = wave_data_ptr[i] & 0x00ff;
		data_buffer[1] = wave_data_ptr[i] >> 8;

		fwrite((void*)&data_buffer, sizeof(data_buffer), 1, fd);
	}

	fclose(fd);

	return 0;
}

int main(int argc, char **argv)
{
	struct wave_header hdr;
	int16_t * wave_data = NULL;
	digit_detect_state_t dtmf_detect_state;
	int last_num_digit = 0;
	int i, j, ret;

	memset(&dtmf_detect_state, 0, sizeof(digit_detect_state_t));

	if (argc < 2 || strlen(argv[1]) == 0)
	{
		printf("Usage: %s filename.wav\n", argv[0]);
		return -1;
	}

	if ((ret = read_wave_file(argv[1], &hdr, &wave_data)) < 0)
		return ret;

#if 0
	{
		long num_samples = (8 * hdr.data_hdr.len) / (hdr.fmt_hdr.nchan * hdr.fmt_hdr.bits_per_sample);
		int16_t * wave_data_ptr = wave_data;
		for (i =1; i <= num_samples; i++)
		{
			printf("%d / %ld: %2d %2d %2d %2d ... %2d %2d %2d %2d\n", i*160, num_samples,
			       wave_data_ptr[0], wave_data_ptr[1], wave_data_ptr[2], wave_data_ptr[3],
			       wave_data_ptr[156], wave_data_ptr[157], wave_data_ptr[158], wave_data_ptr[159]);
			wave_data_ptr += 160;
			if(i*160 > num_samples)
				break;
		}
	}
#endif

	ast_digit_detect_init(&dtmf_detect_state, hdr.fmt_hdr.sample_rate);

	long num_samples = (8 * hdr.data_hdr.len) / (hdr.fmt_hdr.nchan * hdr.fmt_hdr.bits_per_sample);
	int16_t * wave_data_ptr = wave_data;
	for (i = 1; i <= num_samples; i++)
	{
		// printf("%s: --> dtmf_detect()\n", __FUNCTION__);
		int ret = dtmf_detect(&dtmf_detect_state,
		                      wave_data_ptr,
		                      160,
		                      0,
		                      1);
		// printf("%s: %d / %ld ret:%d\n", __FUNCTION__, i*160, num_samples, ret);
		if (ret)
		{
			if (last_num_digit < dtmf_detect_state.detected_digits)
			{
				last_num_digit = dtmf_detect_state.detected_digits;
				printf ("%s: detect DTMF digit %c\n",
				         __FUNCTION__,
				         dtmf_detect_state.digits[last_num_digit - 1]);

			}
			/* reset the detected signal */
			for (j = 0; j < 160; j++)
			{
				wave_data_ptr[j] = 0;
			}
		}

		if((i+1)*160 > num_samples)
			break;
		wave_data_ptr += 160;
	}

	if ((ret = write_wave_file(OUTPUT_FILENAME, &hdr, wave_data)) < 0)
		return ret;

	free(wave_data);

	return 0;
}

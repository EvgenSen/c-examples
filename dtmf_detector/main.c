/**
 * main.c - wrapper over dsp.c and wave.c
 *
 * This example open WAV file, read data and find DTMF signals
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

int main(int argc, char **argv)
{
	FILE *fd;
	struct wave_header hdr;
	digit_detect_state_t dtmf_detect_state;
	int last_num_digit = 0;
	int i;

	memset(&dtmf_detect_state, 0, sizeof(digit_detect_state_t));

	if (argc < 2 || strlen(argv[1]) == 0)
	{
		printf("Usage: %s filename.wav\n", argv[0]);
		return -1;
	}

	if ((fd = fopen(argv[1], "rb")) == NULL)
	{
		printf("Error: Can't open file %s (%s)\n", argv[1], strerror(errno));
		return -2;
	}

	fread((void*)&hdr, sizeof(hdr), 1, fd);

	if(wave_check_header(hdr) < 0)
	{
		printf("Error: Invalid file header\n");
		fclose(fd);
		return -3;
	}

	wave_print_header_info(hdr);

	int16_t * wave_data = wave_read_data(hdr, fd);
	if (wave_data == NULL)
	{
		printf("Error: Can't read file %s\n", argv[1]);
		fclose(fd);
		return -3;
	}

	fclose(fd);

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
			// printf("%s: ret:%d\n", __FUNCTION__, ret);
			// return PJ_EINVALIDOP;
		}

		if((i+1)*160 > num_samples)
			break;
		wave_data_ptr += 160;
	}

	free(wave_data);

	return 0;
}

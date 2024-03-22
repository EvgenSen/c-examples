/*
 * wav.c - functions for working with WAV files
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

#include "wave.h"

int wave_check_header(struct wave_header hdr)
{
	if(hdr.riff_hdr.riff != HEADER_RIFF_TAG ||
	   hdr.riff_hdr.wave != HEADER_WAVE_TAG)
	{
		return -1;
	}

	return 0;
}

void wave_print_header_info(struct wave_header hdr)
{
	printf("Header info:\n");
	printf("\tFile len:            %d\n", hdr.riff_hdr.file_len + 8);
	printf("\tFormat type:         %d (%s)\n", hdr.fmt_hdr.fmt_tag,
	                                      hdr.fmt_hdr.fmt_tag == 1 ? "PCM" :
	                                      hdr.fmt_hdr.fmt_tag == 3 ? "float" :
	                                      hdr.fmt_hdr.fmt_tag == 6 ? "A-law" :
	                                      hdr.fmt_hdr.fmt_tag == 7 ? "U-law" : "Unknown" );
	printf("\tChannels:            %d\n", hdr.fmt_hdr.nchan);
	printf("\tSample rate:         %d\n", hdr.fmt_hdr.sample_rate);
	printf("\tByte Rate:           %d\n", hdr.fmt_hdr.bytes_per_sec);
	printf("\tBit Rate:            %d\n", hdr.fmt_hdr.bytes_per_sec * 8);
	printf("\tBlock Alignment:     %d\n", hdr.fmt_hdr.block_align);
	printf("\tBits per sample:     %d\n", hdr.fmt_hdr.bits_per_sample);
	printf("\tData len:            %d\n", hdr.data_hdr.len);

	printf("Calculated info:\n");

	long num_samples = (8 * hdr.data_hdr.len) / (hdr.fmt_hdr.nchan * hdr.fmt_hdr.bits_per_sample);
	printf("\tNumber of samples:   %lu \n", num_samples);

	long size_of_each_sample = (hdr.fmt_hdr.nchan * hdr.fmt_hdr.bits_per_sample) / 8;
	printf("\tSize of sample:      %ld bytes\n", size_of_each_sample);

	float duration_in_seconds = (float) hdr.riff_hdr.file_len / hdr.fmt_hdr.bytes_per_sec;
	printf("\tDuration:            %f seconds\n", duration_in_seconds);
	// int   h = (duration_in_seconds / 3600); 
	// int   m = (duration_in_seconds -(3600*h))/60;
	// float s = (duration_in_seconds -(3600*h)-(m*60));
	// printf("\tDuration:            %d:%d:%f seconds\n", h,m,s);
}

static int get_limit_values(uint16_t bits_per_sample, long * low_limit, long * high_limit)
{
	switch (bits_per_sample)
	{
		case 8:
		{
			*low_limit = -128;
			*high_limit = 127;
			break;
		}
		case 16:
		{
			*low_limit = -32768;
			*high_limit = 32767;
			break;
		}
		case 32:
		{
			*low_limit = -2147483648;
			*high_limit = 2147483647;
			break;
		}
		default:
		{
			printf("Error: Incorrect bits_per_sample val (%d)\n", bits_per_sample);
			return -1;
		}
	}

	return 0;
}

/* TODO: work only for PCM 16 bit format */
int16_t * wave_read_data(struct wave_header hdr, FILE * fd)
{
	if(hdr.fmt_hdr.fmt_tag != 1)
	{
		printf("Error: WAV file not PCM format, not supported\n");
		return NULL;
	}

	long i = 0;
	int16_t * wave_data = NULL;
	long low_limit = 0l;
	long high_limit = 0l;
	long num_samples = (8 * hdr.data_hdr.len) / (hdr.fmt_hdr.nchan * hdr.fmt_hdr.bits_per_sample);
	long size_of_each_sample = (hdr.fmt_hdr.nchan * hdr.fmt_hdr.bits_per_sample) / 8;
	long bytes_in_each_channel = (size_of_each_sample / hdr.fmt_hdr.nchan);
	char data_buffer[size_of_each_sample];

	printf("size_of_each_sample=%ld\n", size_of_each_sample);

	if ((bytes_in_each_channel  * hdr.fmt_hdr.nchan) != size_of_each_sample)
	{
		printf("Error: %ld x %u != %ld\n", bytes_in_each_channel, hdr.fmt_hdr.nchan, size_of_each_sample);
		return NULL;
	}

	if(get_limit_values(hdr.fmt_hdr.bits_per_sample, &low_limit, &high_limit) < 0)
	{
		printf("Error: Can't get correct limits\n");
		return NULL;
	}

	printf("Valid range for data values: %ld to %ld \n", low_limit, high_limit);

	wave_data = (int16_t*) malloc(sizeof(int16_t) * num_samples + 2);

	for (i = 1; i <= num_samples; i++)
	{
		if(fread(data_buffer, sizeof(data_buffer), 1, fd) != 1)
		{
			printf("Error: Can't read file (%s)\n", strerror(errno));
			continue;
		}

		unsigned int xchannels = 0;
		int data_in_channel = 0;
		int offset = 0; // move the offset for every iteration in the loop below
		for (xchannels = 0; xchannels < hdr.fmt_hdr.nchan; xchannels ++ )
		{
			// printf("Channel#%d : ", (xchannels+1));
			// convert data from little endian to big endian based on bytes in each channel sample
			if (bytes_in_each_channel == 4)
			{
				data_in_channel = (data_buffer[offset] & 0x00ff) |
				                  ((data_buffer[offset + 1] & 0x00ff) <<8) |
				                  ((data_buffer[offset + 2] & 0x00ff) <<16) |
				                  (data_buffer[offset + 3]<<24);
			}
			else if (bytes_in_each_channel == 2)
			{
				data_in_channel = (data_buffer[offset] & 0x00ff) |
				                  (data_buffer[offset + 1] << 8);
			}
			else if (bytes_in_each_channel == 1)
			{
				data_in_channel = data_buffer[offset] & 0x00ff;
				data_in_channel -= 128; //in wave, 8-bit are unsigned, so shifting to signed
			}

			offset += bytes_in_each_channel;
			// printf("%d ", data_in_channel);

			wave_data[i] = data_in_channel;

			// check if value was in range
			if (data_in_channel < low_limit || data_in_channel > high_limit)
				printf("**value out of range\n");
		}

	}

	return wave_data;
}

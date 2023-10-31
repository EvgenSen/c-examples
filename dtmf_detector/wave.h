/**
 * wave.h - functions for working with WAV files
 *
 * Author: Evgeniy Sennikov <sennikov.work@ya.ru>
 *
 * Licence: GPLv3
 */

#ifndef WAVE_H
#define WAVE_H

#include <stdint.h>

#define HEADER_RIFF_TAG       ('F'<<24|'F'<<16|'I'<<8|'R')
#define HEADER_WAVE_TAG       ('E'<<24|'V'<<16|'A'<<8|'W')
#define HEADER_FMT_TAG        (' '<<24|'t'<<16|'m'<<8|'f')
#define HEADER_DATA_TAG       ('a'<<24|'t'<<16|'a'<<8|'d')
#define HEADER_FACT_TAG       ('t'<<24|'c'<<16|'a'<<8|'f')

/* WAV file header format */
struct wave_header
{
	/* This structure describes RIFF WAVE file header */
	struct {
		uint32_t riff;            // "RIFF" ASCII tag
		uint32_t file_len;        // File length minus 8 bytes
		uint32_t wave;            // "WAVE" ASCII tag
	} riff_hdr;

	/* This structure describes format chunks/header */
	struct {
		uint32_t fmt;             // "fmt " ASCII tag
		uint32_t len;             // 16 for PCM
		uint16_t fmt_tag;         // 1: PCM, 3: float, 6: A-law, 7: U-law
		uint16_t nchan;           // Number of channels
		uint32_t sample_rate;     // Sampling rate.
		uint32_t bytes_per_sec;   // Average bytes per second
		uint16_t block_align;     // nchannels * bits / 8
		uint16_t bits_per_sample; // Bits per sample
	} fmt_hdr;

	/* The data header preceeds the actual data in the file */
	struct {
		uint32_t data;            // "data" ASCII tag.
		uint32_t len;             // Data length.
	} data_hdr;
};

int wave_check_header(struct wave_header hdr);
void wave_print_header_info(struct wave_header hdr);
int16_t * wave_read_data(struct wave_header hdr, FILE * fd);

#endif // WAVE_H

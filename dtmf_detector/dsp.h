/*
 * Asterisk -- An open source telephony toolkit.
 *
 * Copyright (C) 1999 - 2005, Digium, Inc.
 *
 * Mark Spencer <markster@digium.com>
 *
 * Goertzel routines are borrowed from Steve Underwood's tremendous work on the
 * DTMF detector.
 *
 * See http://www.asterisk.org for more information about
 * the Asterisk project. Please do not directly contact
 * any of the maintainers of this project for assistance;
 * the project provides a web site, mailing lists and IRC
 * channels for your use.
 *
 * This program is free software, distributed under the terms of
 * the GNU General Public License Version 2. See the LICENSE file
 * at the top of the source tree.
 */

/*! \file
 *
 * \brief Convenience Signal Processing routines
 *
 * \author Mark Spencer <markster@digium.com>
 * \author Steve Underwood <steveu@coppice.org>
 */

#ifndef _ASTERISK_DSP_H
#define _ASTERISK_DSP_H

#define MAX_DTMF_DIGITS 128

typedef void (*digit_detect_ind_cb)(void *user_data, int on_off, int digit);

typedef struct {
	int v2;
	int v3;
	int chunky;
	int fac;
} dtmf_goertzel_state_t;

typedef struct {
	int value;
	int power;
} goertzel_result_t;

typedef struct {
	dtmf_goertzel_state_t row_out[4];
	dtmf_goertzel_state_t col_out[4];
	int hits;   /* How many successive hits we have seen already */
	int misses; /* How many successive misses we have seen already */
	int lasthit;
	int current_hit;
	float energy;
	int current_sample;
	int mute_samples;
} dtmf_detect_state_t;

typedef struct {
	dtmf_goertzel_state_t tone_out[2];
	int tone_count;
	int current_hit;
	int hits[5];
	float energy;
	int current_sample;
	int mute_samples;
} mf_detect_state_t;

typedef struct {
	char digits[MAX_DTMF_DIGITS + 1];
	int digitlen[MAX_DTMF_DIGITS + 1];
	int current_digits;
	int detected_digits;
	int lost_digits;

	union {
		dtmf_detect_state_t dtmf;
		mf_detect_state_t mf;
	} td;

	void *usr;
	digit_detect_ind_cb cb;
} digit_detect_state_t;

int dtmf_detect(digit_detect_state_t *s, int16_t amp[], int samples, int squelch, int relax);
int mf_detect(digit_detect_state_t *s, int16_t amp[], int samples, int squelch, int relax);

void ast_digit_detect_init(digit_detect_state_t *s, unsigned int sample_rate);
void ast_mf_digit_detect_init(digit_detect_state_t *s,
                              float *freq,
                              unsigned int freq_count,
                              unsigned int sample_rate);

void ast_digit_detect_set_ind_cb(digit_detect_state_t *s, void *user_data, digit_detect_ind_cb cb);

#endif /* _ASTERISK_DSP_H */

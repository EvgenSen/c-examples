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

/*! \li \ref dsp.c uses the configuration file \ref dsp.conf
 * \addtogroup configuration_file Configuration Files
 */

/*!
 * \page dsp.conf dsp.conf
 * \verbinclude dsp.conf.sample
 */

/* Some routines from tone_detect.c by Steven Underwood as published under the zapata library */
/*
    tone_detect.c - General telephony tone detection, and specific
                    detection of DTMF.

    Copyright (C) 2001  Steve Underwood <steveu@coppice.org>

    Despite my general liking of the GPL, I place this code in the
    public domain for the benefit of all mankind - even the slimy
    ones who might try to proprietize my work and use it to my
    detriment.
*/

/*** MODULEINFO
    <support_level>core</support_level>
 ***/

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "dsp.h"

/* Basic DTMF (AT&T) specs:
 *
 * Minimum tone on = 40ms
 * Minimum tone off = 50ms
 * Maximum digit rate = 10 per second
 * Normal twist <= 8dB accepted
 * Reverse twist <= 4dB accepted
 * S/N >= 15dB will detect OK
 * Attenuation <= 26dB will detect OK
 * Frequency tolerance +- 1.5% will detect, +-3.5% will reject
 */

#define DTMF_THRESHOLD 8.0e7

#define DEF_DTMF_NORMAL_TWIST       6.31 /* 8.0dB */
#define DEF_RELAX_DTMF_NORMAL_TWIST 6.31 /* 8.0dB */

#if 0 //def RADIO_RELAX
#define DEF_DTMF_REVERSE_TWIST       2.51 /* 4.01dB */
#define DEF_RELAX_DTMF_REVERSE_TWIST 6.61 /* 8.2dB */
#else
#define DEF_DTMF_REVERSE_TWIST       2.51 /* 4.01dB */
#define DEF_RELAX_DTMF_REVERSE_TWIST 3.98 /* 6.0dB */
#endif

//#define DTMF_RELATIVE_PEAK_ROW	6.3     /* 8dB */
//#define DTMF_RELATIVE_PEAK_COL	6.3     /* 8dB */
/* more relaxed value */
#define DTMF_RELATIVE_PEAK_ROW 5.0                 /* 7dB */
#define DTMF_RELATIVE_PEAK_COL 5.0                 /* 7dB */
#define DTMF_2ND_HARMONIC_ROW  (relax ? 1.7 : 2.5) /* 4dB normal */
#define DTMF_2ND_HARMONIC_COL  63.1                /* 18dB */
//#define DTMF_TO_TOTAL_ENERGY    42.0
/* more relaxed value */
#define DTMF_TO_TOTAL_ENERGY 32.0

//#define BELL_MF_THRESHOLD       1.6e9
#define BELL_MF_THRESHOLD       5.4e6 /* -29 dB */
#define BELL_MF_TWIST           4.0   /* 6dB */
#define BELL_MF_RELATIVE_PEAK   12.6  /* 11dB */
#define BELL_MF_TO_TOTAL_ENERGY 22.0

/* MF goertzel size */
#define MF_GSIZE 120

/* DTMF goertzel size */
#define DTMF_GSIZE 102

/* How many successive hits needed to consider begin of a digit
 * IE. Override with dtmf_hits_to_begin=4 in dsp.conf
 */
// #define DEF_DTMF_HITS_TO_BEGIN	2
#define DEF_DTMF_HITS_TO_BEGIN 1

/* How many successive misses needed to consider end of a digit
 * IE. Override with dtmf_misses_to_end=4 in dsp.conf
 */
// #define DEF_DTMF_MISSES_TO_END	3
#define DEF_DTMF_MISSES_TO_END 10

static const float dtmf_row[] = {697.0, 770.0, 852.0, 941.0};
static const float dtmf_col[] = {1209.0, 1336.0, 1477.0, 1633.0};

static const char dtmf_positions[] = "123A"
                                     "456B"
                                     "789C"
                                     "*0#D";
static float dtmf_normal_twist = DEF_DTMF_NORMAL_TWIST;               /* AT&T = 8dB */
static float dtmf_reverse_twist = DEF_DTMF_REVERSE_TWIST;             /* AT&T = 4dB */
static float relax_dtmf_normal_twist = DEF_RELAX_DTMF_NORMAL_TWIST;   /* AT&T = 8dB */
static float relax_dtmf_reverse_twist = DEF_RELAX_DTMF_REVERSE_TWIST; /* AT&T = 6dB */
static int dtmf_hits_to_begin = DEF_DTMF_HITS_TO_BEGIN; /* How many successive hits needed to
                                                           consider begin of a digit */
static int dtmf_misses_to_end = DEF_DTMF_MISSES_TO_END; /* How many successive misses needed to
                                                           consider end of a digit */

static inline void goertzel_sample(dtmf_goertzel_state_t *s, short sample)
{
	int v1;

	/*
	 * Shift previous values so
	 * v1 is previous previous value
	 * v2 is previous value
	 * until the new v3 is calculated.
	 */
	v1 = s->v2;
	s->v2 = s->v3;

	/* Discard the binary fraction introduced by s->fac */
	s->v3 = (s->fac * s->v2) >> 15;
	/* Scale sample to match previous values */
	s->v3 = s->v3 - v1 + (sample >> s->chunky);

	if (abs(s->v3) > 32768) {
		/* The result is now too large so increase the chunky power. */
		s->chunky++;
		s->v3 = s->v3 >> 1;
		s->v2 = s->v2 >> 1;
	}
}

static inline float goertzel_result(dtmf_goertzel_state_t *s)
{
	goertzel_result_t r;

	r.value = (s->v3 * s->v3) + (s->v2 * s->v2);
	r.value -= ((s->v2 * s->v3) >> 15) * s->fac;
	/*
	 * We have to double the exponent because we multiplied the
	 * previous sample calculation values together.
	 */
	r.power = s->chunky * 2;
	return (float)r.value * (float)(1 << r.power);
}

static inline void goertzel_init(dtmf_goertzel_state_t *s, float freq, unsigned int sample_rate)
{
	s->v2 = s->v3 = s->chunky = 0;
	s->fac = (int)(32768.0 * 2.0 * cos(2.0 * M_PI * freq / sample_rate));
}

static inline void goertzel_reset(dtmf_goertzel_state_t *s) { s->v2 = s->v3 = s->chunky = 0; }

typedef struct {
	int start;
	int end;
} fragment_t;

static void ast_dtmf_detect_init(dtmf_detect_state_t *s, unsigned int sample_rate)
{
	int i;

	for (i = 0; i < 4; i++) {
		goertzel_init(&s->row_out[i], dtmf_row[i], sample_rate);
		goertzel_init(&s->col_out[i], dtmf_col[i], sample_rate);
	}
	s->lasthit = 0;
	s->current_hit = 0;
	s->energy = 0.0;
	s->current_sample = 0;
	s->hits = 0;
	s->misses = 0;
}

static void ast_mf_detect_init(mf_detect_state_t *s,
                               float mf_tones[2],
                               unsigned int t_count,
                               unsigned int sample_rate)
{
	int i;

	s->tone_count = t_count;
	if (s->tone_count > 2)
		s->tone_count = 2;
	for (i = 0; i < s->tone_count; i++) {
		goertzel_init(&s->tone_out[i], mf_tones[i], sample_rate);
	}
	s->hits[0] = s->hits[1] = s->hits[2] = s->hits[3] = s->hits[4] = 0;
	s->energy = 0.0;
	s->current_sample = 0;
	s->current_hit = 0;
}

void ast_digit_detect_init(digit_detect_state_t *s, unsigned int sample_rate)
{
	s->current_digits = 0;
	s->detected_digits = 0;
	s->lost_digits = 0;
	s->digits[0] = '\0';
	ast_dtmf_detect_init(&s->td.dtmf, sample_rate);
}

void ast_mf_digit_detect_init(digit_detect_state_t *s,
                              float *freq,
                              unsigned int freq_count,
                              unsigned int sample_rate)
{
	s->current_digits = 0;
	s->detected_digits = 0;
	s->lost_digits = 0;
	s->digits[0] = '\0';
	ast_mf_detect_init(&s->td.mf, freq, freq_count, sample_rate);
}

void ast_digit_detect_set_ind_cb(digit_detect_state_t *s, void *user_data, digit_detect_ind_cb cb)
{
	s->usr = user_data;
	s->cb = cb;
}

static void indicate_digit(digit_detect_state_t *s, int on_off, char digit)
{
	if (s->cb)
		s->cb(s->usr, on_off, digit);
}

static void store_digit(digit_detect_state_t *s, char digit)
{
	s->detected_digits++;
	if (s->current_digits < MAX_DTMF_DIGITS) {
		s->digitlen[s->current_digits] = 0;
		s->digits[s->current_digits++] = digit;
		s->digits[s->current_digits] = '\0';
	} else {
		/* ast_log(LOG_WARNING, "Digit lost due to full buffer\n"); */
		s->lost_digits++;
	}
}

int dtmf_detect(digit_detect_state_t *s, int16_t amp[], int samples, int squelch, int relax)
{
	float row_energy[4];
	float col_energy[4];
	int i;
	int j;
	int sample;
	short samp;
	int best_row;
	int best_col;
	int hit;
	int limit;
	fragment_t mute = {0, 0};

	if (squelch && s->td.dtmf.mute_samples > 0) {
		mute.end = (s->td.dtmf.mute_samples < samples) ? s->td.dtmf.mute_samples : samples;
		s->td.dtmf.mute_samples -= mute.end;
	}

	hit = 0;
	for (sample = 0; sample < samples; sample = limit) {
		/* DTMF_GSIZE is optimised to meet the DTMF specs. */
		if ((samples - sample) >= (DTMF_GSIZE - s->td.dtmf.current_sample)) {
			limit = sample + (DTMF_GSIZE - s->td.dtmf.current_sample);
		} else {
			limit = samples;
		}
		/* The following unrolled loop takes only 35% (rough estimate) of the
		   time of a rolled loop on the machine on which it was developed */
		for (j = sample; j < limit; j++) {
			samp = amp[j];
			s->td.dtmf.energy += (int32_t)samp * (int32_t)samp;
			/* With GCC 2.95, the following unrolled code seems to take about 35%
			   (rough estimate) as long as a neat little 0-3 loop */
			goertzel_sample(s->td.dtmf.row_out, samp);
			goertzel_sample(s->td.dtmf.col_out, samp);
			goertzel_sample(s->td.dtmf.row_out + 1, samp);
			goertzel_sample(s->td.dtmf.col_out + 1, samp);
			goertzel_sample(s->td.dtmf.row_out + 2, samp);
			goertzel_sample(s->td.dtmf.col_out + 2, samp);
			goertzel_sample(s->td.dtmf.row_out + 3, samp);
			goertzel_sample(s->td.dtmf.col_out + 3, samp);
		}
		s->td.dtmf.current_sample += (limit - sample);
		if (s->td.dtmf.current_sample < DTMF_GSIZE) {
			continue;
		}
		/* We are at the end of a DTMF detection block */
		/* Find the peak row and the peak column */
		row_energy[0] = goertzel_result(&s->td.dtmf.row_out[0]);
		col_energy[0] = goertzel_result(&s->td.dtmf.col_out[0]);

		for (best_row = best_col = 0, i = 1; i < 4; i++) {
			row_energy[i] = goertzel_result(&s->td.dtmf.row_out[i]);
			if (row_energy[i] > row_energy[best_row]) {
				best_row = i;
			}
			col_energy[i] = goertzel_result(&s->td.dtmf.col_out[i]);
			if (col_energy[i] > col_energy[best_col]) {
				best_col = i;
			}
		}
		hit = 0;
		/* Basic signal level test and the twist test */
		if (row_energy[best_row] >= DTMF_THRESHOLD && col_energy[best_col] >= DTMF_THRESHOLD
		    && col_energy[best_col] < row_energy[best_row] * (relax ? relax_dtmf_reverse_twist : dtmf_reverse_twist)
		    && row_energy[best_row] < col_energy[best_col] * (relax ? relax_dtmf_normal_twist : dtmf_normal_twist)) {
			/* Relative peak test */
			for (i = 0; i < 4; i++) {
				if ((i != best_col && col_energy[i] * DTMF_RELATIVE_PEAK_COL > col_energy[best_col])
				    || (i != best_row
				        && row_energy[i] * DTMF_RELATIVE_PEAK_ROW > row_energy[best_row])) {
					break;
				}
			}
			/* ... and fraction of total energy test */
			if (i >= 4
			    && (row_energy[best_row]
			        + col_energy[best_col]) > DTMF_TO_TOTAL_ENERGY * s->td.dtmf.energy) {
				/* Got a hit */
				hit = dtmf_positions[(best_row << 2) + best_col];
			}
		}

		/*
		 * Adapted from ETSI ES 201 235-3 V1.3.1 (2006-03)
		 * (40ms reference is tunable with hits_to_begin and misses_to_end)
		 * each hit/miss is 12.75ms with DTMF_GSIZE at 102
		 *
		 * Character recognition: When not DRC *(1) and then
		 *      Shall exist VSC > 40 ms (hits_to_begin)
		 *      May exist 20 ms <= VSC <= 40 ms
		 *      Shall not exist VSC < 20 ms
		 *
		 * Character recognition: When DRC and then
		 *      Shall cease Not VSC > 40 ms (misses_to_end)
		 *      May cease 20 ms >= Not VSC >= 40 ms
		 *      Shall not cease Not VSC < 20 ms
		 *
		 * *(1) or optionally a different digit recognition condition
		 *
		 * Legend: VSC The continuous existence of a valid signal condition.
		 *      Not VSC The continuous non-existence of valid signal condition.
		 *      DRC The existence of digit recognition condition.
		 *      Not DRC The non-existence of digit recognition condition.
		 */

		/*
		 * Example: hits_to_begin=2 misses_to_end=3
		 * -------A last_hit=A hits=0&1
		 * ------AA hits=2 current_hit=A misses=0       BEGIN A
		 * -----AA- misses=1 last_hit=' ' hits=0
		 * ----AA-- misses=2
		 * ---AA--- misses=3 current_hit=' '            END A
		 * --AA---B last_hit=B hits=0&1
		 * -AA---BC last_hit=C hits=0&1
		 * AA---BCC hits=2 current_hit=C misses=0       BEGIN C
		 * A---BCC- misses=1 last_hit=' ' hits=0
		 * ---BCC-C misses=0 last_hit=C hits=0&1
		 * --BCC-CC misses=0
		 *
		 * Example: hits_to_begin=3 misses_to_end=2
		 * -------A last_hit=A hits=0&1
		 * ------AA hits=2
		 * -----AAA hits=3 current_hit=A misses=0       BEGIN A
		 * ----AAAB misses=1 last_hit=B hits=0&1
		 * ---AAABB misses=2 current_hit=' ' hits=2     END A
		 * --AAABBB hits=3 current_hit=B misses=0       BEGIN B
		 * -AAABBBB misses=0
		 *
		 * Example: hits_to_begin=2 misses_to_end=2
		 * -------A last_hit=A hits=0&1
		 * ------AA hits=2 current_hit=A misses=0       BEGIN A
		 * -----AAB misses=1 hits=0&1
		 * ----AABB misses=2 current_hit=' ' hits=2 current_hit=B misses=0 BEGIN B
		 * ---AABBB misses=0
		 */

		if (s->td.dtmf.current_hit) {
			/* We are in the middle of a digit already */
			if (hit != s->td.dtmf.current_hit) {
				s->td.dtmf.misses++;
				if (s->td.dtmf.misses == dtmf_misses_to_end) {
					/* There were enough misses to consider digit ended */
					indicate_digit(s, 0, s->td.dtmf.current_hit);
					s->td.dtmf.current_hit = 0;
				}
			} else {
				s->td.dtmf.misses = 0;
				/* Current hit was same as last, so increment digit duration (of last digit) */
				s->digitlen[s->current_digits - 1] += DTMF_GSIZE;
			}
		}

		/* Look for a start of a new digit no matter if we are already in the middle of some
		   digit or not. This is because hits_to_begin may be smaller than misses_to_end
		   and we may find begin of new digit before we consider last one ended. */

		if (hit != s->td.dtmf.lasthit) {
			s->td.dtmf.lasthit = hit;
			s->td.dtmf.hits = 0;
		}
		if (hit && hit != s->td.dtmf.current_hit) {
			s->td.dtmf.hits++;
			// if (s->td.dtmf.hits == dtmf_hits_to_begin) {
			if (s->td.dtmf.hits > 0) { // instantly set that we have hit
				if (s->td.dtmf.current_hit != 0)
					indicate_digit(s, 0, s->td.dtmf.current_hit);
				indicate_digit(s, 1, hit);
				store_digit(s, hit);
				s->digitlen[s->current_digits - 1] = dtmf_hits_to_begin * DTMF_GSIZE;
				s->td.dtmf.current_hit = hit;
				s->td.dtmf.misses = 0;
			}
		}

		/* If we had a hit in this block, include it into mute fragment */
		if (squelch && hit) {
			if (mute.end < sample - DTMF_GSIZE) {
				/* There is a gap between fragments */
				/*mute_fragment(dsp, &mute);*/
				mute.start = (sample > DTMF_GSIZE) ? (sample - DTMF_GSIZE) : 0;
			}
			mute.end = limit + DTMF_GSIZE;
		}

		/* Reinitialise the detector for the next block */
		for (i = 0; i < 4; i++) {
			goertzel_reset(&s->td.dtmf.row_out[i]);
			goertzel_reset(&s->td.dtmf.col_out[i]);
		}
		s->td.dtmf.energy = 0.0;
		s->td.dtmf.current_sample = 0;
	}

	if (squelch && mute.end) {
		if (mute.end > samples) {
			s->td.dtmf.mute_samples = mute.end - samples;
			mute.end = samples;
		}
		/*mute_fragment(dsp, &mute);*/
	}

	return (s->td.dtmf.current_hit); /* return the debounced hit */
}

int mf_detect(digit_detect_state_t *s, int16_t amp[], int samples, int squelch, int relax)
{
	float energy[6];
	float energy_tot = 0.0;
	/*
	int best;
	int second_best;
	*/
	int i;
	int j;
	int sample;
	short samp;
	int hit;
	int limit;
	(void)relax;
	fragment_t mute = {0, 0};

	if (squelch && s->td.mf.mute_samples > 0) {
		mute.end = (s->td.mf.mute_samples < samples) ? s->td.mf.mute_samples : samples;
		s->td.mf.mute_samples -= mute.end;
	}

	hit = 0;
	for (sample = 0; sample < samples; sample = limit) {
		/* 80 is optimised to meet the MF specs. */
		/* XXX So then why is MF_GSIZE defined as 120? */
		if ((samples - sample) >= (MF_GSIZE - s->td.mf.current_sample)) {
			limit = sample + (MF_GSIZE - s->td.mf.current_sample);
		} else {
			limit = samples;
		}
		/* The following unrolled loop takes only 35% (rough estimate) of the
		   time of a rolled loop on the machine on which it was developed */
		for (j = sample; j < limit; j++) {
			/* With GCC 2.95, the following unrolled code seems to take about 35%
			   (rough estimate) as long as a neat little 0-3 loop */
			samp = amp[j];
			s->td.mf.energy += pow(samp, 4);
			for (i = 0; i < s->td.mf.tone_count; i++) {
				goertzel_sample(&s->td.mf.tone_out[i], samp);
			}
			/*
			goertzel_sample(s->td.mf.tone_out, samp);
			goertzel_sample(s->td.mf.tone_out + 1, samp);
			goertzel_sample(s->td.mf.tone_out + 2, samp);
			goertzel_sample(s->td.mf.tone_out + 3, samp);
			goertzel_sample(s->td.mf.tone_out + 4, samp);
			goertzel_sample(s->td.mf.tone_out + 5, samp);
			*/
		}
		s->td.mf.current_sample += (limit - sample);
		if (s->td.mf.current_sample < MF_GSIZE) {
			continue;
		}
		/* We're at the end of an MF detection block.  */
		/* Find the two highest energies. The spec says to look for
		   two tones and two tones only. Taking this literally -ie
		   only two tones pass the minimum threshold - doesn't work
		   well. The sinc function mess, due to rectangular windowing
		   ensure that! Find the two highest energies and ensure they
		   are considerably stronger than any of the others. */
		for (i = 0; i < s->td.mf.tone_count; i++) {
			energy[i] = goertzel_result(&s->td.mf.tone_out[i]);
		}
		/*
		energy[0] = goertzel_result(&s->td.mf.tone_out[0]);
		energy[1] = goertzel_result(&s->td.mf.tone_out[1]);
		if (energy[0] > energy[1]) {
		    best = 0;
		    second_best = 1;
		} else {
		    best = 1;
		    second_best = 0;
		}
		*/
		/*endif*/ /*
	   for (i = 2; i < 6; i++) {
		   energy[i] = goertzel_result(&s->td.mf.tone_out[i]);
		   if (energy[i] >= energy[best]) {
		       second_best = best;
		       best = i;
		   } else if (energy[i] >= energy[second_best]) {
		       second_best = i;
		   }
	   }
	   */
		/* Basic signal level and twist tests */

		hit = 0;
		for (i = 0; i < s->td.mf.tone_count; i++) {
			if (energy[i] >= BELL_MF_THRESHOLD) {
				for (j = i + 1; j < s->td.mf.tone_count; j++) {
					if (energy[i] >= energy[j] * BELL_MF_TWIST
					    || energy[i] * BELL_MF_TWIST <= energy[j])
						break;
				}
				if (j >= s->td.mf.tone_count)
					hit++;
				energy_tot += energy[i];
			}
		}
		s->td.mf.energy = sqrtf(s->td.mf.energy * MF_GSIZE);

		if (hit != s->td.mf.tone_count || energy_tot < (BELL_MF_TO_TOTAL_ENERGY * s->td.mf.energy))
			hit = 0;
		else
			hit = -1;
		/*
		if (energy[best] >= BELL_MF_THRESHOLD && energy[second_best] >= BELL_MF_THRESHOLD
		    && energy[best] < energy[second_best]*BELL_MF_TWIST
		    && energy[best] * BELL_MF_TWIST > energy[second_best]) {
		    */
		/* Relative peak test */                    /*
		                  hit = -1;
		                  for (i = 0; i < 6; i++) {
		                      if (i != best && i != second_best) {
		                          if (energy[i]*BELL_MF_RELATIVE_PEAK >= energy[second_best]) {
		                              */
		/* The best two are not clearly the best */ /*
	   hit = 0;
	   break;
   }
}
}
}
*/
		if (hit && hit != s->td.mf.current_hit) {
			/* Get the values into ascending order */
			/*
			if (second_best < best) {
			    i = best;
			    best = second_best;
			    second_best = i;
			}
			best = best * 5 + second_best - 1;
			hit = bell_mf_positions[best];
			*/
			/* Look for two successive similar results */
			/* The logic in the next test is:
			   For KP we need 4 successive identical clean detects, with
			   two blocks of something different preceeding it. For anything
			   else we need two successive identical clean detects, with
			   two blocks of something different preceeding it. */
			if (hit == s->td.mf.hits[4] && hit == s->td.mf.hits[3]
			    && ((hit != '*' && hit != s->td.mf.hits[2] && hit != s->td.mf.hits[1])
			        || (hit == '*' && hit == s->td.mf.hits[2] && hit != s->td.mf.hits[1]
			            && hit != s->td.mf.hits[0]))) {
				indicate_digit(s, 1, hit);
				store_digit(s, hit);
				s->td.mf.current_hit = hit;
			}
		}

		if (s->td.mf.current_hit && hit != s->td.mf.hits[4] && hit != s->td.mf.hits[3]) {
			/* Two successive block without a hit terminate current digit */
			indicate_digit(s, 0, s->td.dtmf.current_hit);
			s->td.mf.current_hit = 0;
		}

		s->td.mf.hits[0] = s->td.mf.hits[1];
		s->td.mf.hits[1] = s->td.mf.hits[2];
		s->td.mf.hits[2] = s->td.mf.hits[3];
		s->td.mf.hits[3] = s->td.mf.hits[4];
		s->td.mf.hits[4] = hit;

		/* If we had a hit in this block, include it into mute fragment */
		if (squelch && hit) {
			if (mute.end < sample - MF_GSIZE) {
				/* There is a gap between fragments */
				/*mute_fragment(dsp, &mute);*/
				mute.start = (sample > MF_GSIZE) ? (sample - MF_GSIZE) : 0;
			}
			mute.end = limit + MF_GSIZE;
		}

		/* Reinitialise the detector for the next block */
		for (i = 0; i < s->td.mf.tone_count; i++) {
			goertzel_reset(&s->td.mf.tone_out[i]);
		}
		s->td.mf.energy = 0.0;
		s->td.mf.current_sample = 0;
	}

	if (squelch && mute.end) {
		if (mute.end > samples) {
			s->td.mf.mute_samples = mute.end - samples;
			mute.end = samples;
		}
		/*mute_fragment(dsp, &mute);*/
	}

	return (s->td.mf.current_hit); /* return the debounced hit */
}

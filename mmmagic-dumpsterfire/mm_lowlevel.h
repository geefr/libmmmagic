#ifndef MM_LOWLEVEL_H
#define MM_LOWLEVEL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "platform.h"
#include "minimaidhid.h" 
#include "asyncthreads.h"
#include "update.h"

#define GUARD(success_test) \
	if (!(success_test)) { \
		printf("Failure at line %d, error code = %d\n", __LINE__, GetLastError()); \
		goto error; \
	}

#ifdef __cplusplus
extern "C" {
#endif

struct mm_InputReport {
	unsigned char	id;				/* report ID */

	unsigned char	dipsw;			/* DIP switches (8) */
	unsigned char	inputs[5];		/* JAMMA Inputs + EXT-IN */
};

struct mm_OutputReport {
	unsigned char	id;				/* report ID */

	unsigned char	extout;			/* 573 EXT OUT */
	unsigned char	isoout[4];		/* Expansion board isolated outputs + EXT-IO Transistor Outputs */

	unsigned char	blueled;		/* Blue LED intensity (0 = off, 255 = full) */

	unsigned char	enablekbd;		/* Enable keyboard input (1 = on, 0 = off - default on) */

	unsigned char	hax;			/* Enables for various hacks */
};

#define MMHID_MAX_BUFFERS	100

struct mm_OutputReport *mm_out_report;
struct mm_InputReport *mm_in_report;
struct mm_InputReport *mm_in_reports;
unsigned int reports_read;

typedef void (*mm_minimaid_input_callback_t)(void *ctx);
typedef void (*mm_minimaid_output_callback_t)(void *ctx);


int mm_minimaid_UpdateInputs(mm_minimaid_input_callback_t cb, void *ctx);
int mm_minimaid_SetOutputs(mm_minimaid_output_callback_t cb, void *ctx);




#ifdef __cplusplus
}
#endif

#endif
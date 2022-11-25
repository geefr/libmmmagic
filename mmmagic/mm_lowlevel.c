#include "mm_lowlevel.h"

struct mm_InputReport *in_reports = NULL;
struct mm_InputReport *in_report = NULL;
unsigned int reports_read;
struct mm_OutputReport *out_report = NULL;

unsigned long buffers;
//BOOL asyncupdates;

//#define ENABLE_DEBUG_MESSAGES

#define DEBUG_VERSION_LL 	0
#if DEBUG_VERSION_LL
#define Debug(x)		printf x
#else
#define Debug(x)
#endif


int mm_minimaid_UpdateInputs(mm_minimaid_input_callback_t cb, void *ctx) { 
  Debug("mm: input\n");
	int ret;
/*	if(asyncupdates)
	{
		// Signal the InputThread to run
		minimaid_input_callback = cb;
		minimaid_input_ctx = ctx;
		ReleaseSemaphore(minimaid_input_sem, 1, NULL);
	} else */{
		ret = minimaid_UpdateInputsWork();
		if(cb)
			cb(ctx);
	}

	return ret;
}

int mm_minimaid_SetOutputs(mm_minimaid_output_callback_t cb, void *ctx) {
  Debug("mm: output\n");
	int ret;
/*	if(asyncupdates)
	{
		// Signal the OutputThread to run
		minimaid_output_callback = cb;
		minimaid_output_ctx = ctx;
		ReleaseSemaphore(minimaid_output_sem, 1, NULL);
	} else */{
		ret = minimaid_SetOutputsWork();
		if(cb)
			cb(ctx);
	}

	return ret;
}

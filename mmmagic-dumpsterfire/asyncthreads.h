#ifndef ASYNCTHREADS_H_
#define ASYNCTHREADS_H_

#include "mm_lowlevel.h"
#include "platform.h"

typedef void (*mm_minimaid_input_callback_t)(void *ctx);
typedef void (*mm_minimaid_output_callback_t)(void *ctx);

#ifdef WINBLOWS
//extern mm_minimaid_input_callback_t minimaid_input_callback;
mm_minimaid_input_callback_t minimaid_input_callback;
void *minimaid_input_ctx;
HANDLE minimaid_input_sem;


//extern mm_minimaid_output_callback_t minimaid_output_callback;
mm_minimaid_output_callback_t minimaid_output_callback;
void *minimaid_output_ctx;
HANDLE minimaid_output_sem;


extern DWORD WINAPI InputThread(LPVOID param);
extern DWORD WINAPI OutputThread(LPVOID param);
#else

#endif

#endif

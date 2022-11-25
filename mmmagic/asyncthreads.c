#define true (1==1)
#define false (1==0)

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "asyncthreads.h"

#ifdef WINBLOWS
/* Input work thread */
mm_minimaid_input_callback_t minimaid_input_callback;
void *minimaid_input_ctx;
HANDLE minimaid_input_sem;

DWORD WINAPI InputThread(LPVOID param)
{
	while(true)
	{
		WaitForSingleObject(minimaid_input_sem, INFINITE);
		minimaid_UpdateInputsWork();
		if(minimaid_input_callback)
			minimaid_input_callback(minimaid_input_ctx);
	}
	return 0;
}



/* Output work thread */
mm_minimaid_output_callback_t minimaid_output_callback;
void *minimaid_output_ctx;
HANDLE minimaid_output_sem;

DWORD WINAPI OutputThread(LPVOID param)
{
	while(true)
	{
		WaitForSingleObject(minimaid_output_sem, INFINITE);
		Sleep(1);
		minimaid_SetOutputsWork();
		if(minimaid_output_callback)
			minimaid_output_callback(minimaid_output_ctx);
	}
	return 0;
}

#else

void* InputThread(void *param)
{
	while(1)
	{
		sleep(1);
	}
}

void* OutputThread(void* param)
{
	while(1)
	{	
		sleep(1);
	}
	return 0;
}


#endif

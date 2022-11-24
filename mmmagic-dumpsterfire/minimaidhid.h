#ifndef MINIMAIDHID_H_
#define MINIMAIDHID_H_

#include "platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>



#include "minimaidhid.h" 
#include "asyncthreads.h"
#include "update.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef WINBLOWS
HANDLE HIDHandle;
#else
int fd;
#endif
unsigned long buffers;
BOOL asyncupdates;


int minimaid_open_device(BOOL async);

#ifdef __cplusplus
}
#endif


#endif

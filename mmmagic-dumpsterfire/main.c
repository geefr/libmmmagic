#include "mmmagic.h"

#define WINBLOWS

#ifndef WINBLOWS
extern int fd;
#endif

int main (void)
{
	mm_connect_minimaid();
	printf("Beginning test\n");
	int count=0;
	while (count<100)
	{
		mm_setBlueLED(0xFF);
		mm_sendDDRMiniMaidUpdate();
#ifdef WINBLOWS
    Sleep(3000);
#else
		sleep(3);
#endif
		mm_setBlueLED(0x00);
		mm_sendDDRMiniMaidUpdate();
#ifdef WINBLOWS
    Sleep(5000);
#else
		sleep(5);
#endif
		count++;
	}
#ifndef WINBLOWS
	close(fd);
#endif
	printf("Test complete\n");
	return 0;
}

#include "mmmagic.h"

#include <chrono>
#include <thread>

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
		mm_setDDRAllOn();
		mm_sendDDRMiniMaidUpdate();
		std::this_thread::sleep_for(std::chrono::seconds(3));
		mm_setBlueLED(0x00);
		mm_setDDRAllOff();
		mm_sendDDRMiniMaidUpdate();
		std::this_thread::sleep_for(std::chrono::seconds(5));
		count++;
	}
#ifndef WINBLOWS
	close(fd);
#endif
	printf("Test complete\n");
	return 0;
}

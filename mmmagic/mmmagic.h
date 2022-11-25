/*

IMPORTANT NOTE FOR *NIX USERS:
You must add permissions to access the minimaid as a user! Either do this through udev or manually or something. The below should work but fuck if it works for me on lubuntu 64bit :/

sudo -i
echo SUBSYSTEM==\"usb\", ATTR{idVendor}==\"beef\", ATTR{idProduct}==\"5730\", MODE=\"0666\" > /etc/udev/rules.d/50-minimaid

OR if you can't get it to work like me, hackjob it...

sudo chmod 666 /dev/hidraw*

*/

#ifndef MMMAGIC_H
#define MMMAGIC_H

#include <stdbool.h>
#include <stdio.h>
//#include "platform.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifndef MMMAGIC_API
# ifdef MMMAGIC_API_EXPORT
#  define MMMAGIC_API __declspec(dllexport)
# else
#  define MMMAGIC_API __declspec(dllimport)
# endif
#endif

/*Minimaid bit identifiers*/
//CANINET_LIGHTS
#define DDR_DOUBLE_BASS_LIGHTS   0 //unknown but guessed
#define DDR_DOUBLE_PLAYER1_PANEL   2
#define DDR_DOUBLE_PLAYER2_PANEL   3
#define DDR_DOUBLE_MARQUEE_LOWER_RIGHT   4
#define DDR_DOUBLE_MARQUEE_UPPER_RIGHT   5
#define DDR_DOUBLE_MARQUEE_LOWER_LEFT   6
#define DDR_DOUBLE_MARQUEE_UPPER_LEFT   7

//PADX_LIGHTS
#define DDR_DOUBLE_PAD_UP    0
#define DDR_DOUBLE_PAD_DOWN    1
#define DDR_DOUBLE_PAD_LEFT    2
#define DDR_DOUBLE_PAD_RIGHT    3
#define DDR_DOUBLE_PAD_RESET  4 

#define BIT(i) (1<<(i))
#define BIT_IS_SET(v,i) ((v&BIT(i))!=0)
static bool __bitValid(int bit);


//minimaid prototypes
MMMAGIC_API extern void mm_setDDRPad1Light(int, int);
MMMAGIC_API extern void mm_setDDRPad2Light(int, int);
MMMAGIC_API extern void mm_setDDRCabinetLight(int, int);
MMMAGIC_API extern void mm_setDDRBassLight(int, int);


MMMAGIC_API extern bool mm_connect_minimaid();
MMMAGIC_API extern bool mm_setKB(bool val);

MMMAGIC_API extern void mm_setDDRAllOn();
MMMAGIC_API extern void mm_setDDRAllOff();

MMMAGIC_API extern void mm_setBlueLED(unsigned char value);
MMMAGIC_API extern void mm_setMMOutputReports(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
MMMAGIC_API extern bool mm_sendDDRMiniMaidUpdate();
MMMAGIC_API extern void mm_init();
MMMAGIC_API extern unsigned char mm_turnON(unsigned char set, int bit);
MMMAGIC_API extern unsigned char mm_turnOFF(unsigned char set, int bit);

//void mm_input_callback(void *ctx);

#ifdef __cplusplus
}
#endif

#endif

#include "mmmagic.h"
#include "mm_lowlevel.h"

static bool					__mm=false;
static bool					__kb=true;
static int					__no_mm_counter=0;

static unsigned char	   __BLUE_LED=0;
static unsigned char       __DDR_CABINET_LIGHTS=0;
static unsigned char       __DDR_PAD1_LIGHTS=0;
static unsigned char       __DDR_PAD2_LIGHTS=0;
static unsigned char       __DDR_BASS_LIGHTS=0;

void mm_input_callback(void *ctx)
{
  /*
	printf("\rDIPSW: %02x INPUTS: %02x %02x : %02x %02x  OUTPUTS: %02x%02x%02x%02x  EXTOUT: %02x  BLUELED: %02x", 
		in_report->dipsw,
		in_report->inputs[0], in_report->inputs[1], in_report->inputs[2], in_report->inputs[3],
		out_report->isoout[0], out_report->isoout[1], out_report->isoout[2], out_report->isoout[3],
		out_report->extout,
		out_report->blueled
	);
	*/
}

static bool __bitValid(int bit)
{
  if (bit<0 || bit >7) return false;
  return true;
}

static unsigned char mm_turnON(unsigned char set, int bit)
{
  return set |= (1<<bit);
}

static unsigned char mm_turnOFF(unsigned char set, int bit)
{
  return set &= ~(1 << bit);
}

void mm_setBlueLED(unsigned char value)
{
  //printf("Setting value %02x\n",value);
  __BLUE_LED = value;
}

void mm_setDDRPad1Light(int bit, int value)
{
  if (!__bitValid(bit)) return;
  if (value==1) __DDR_PAD1_LIGHTS=mm_turnON(__DDR_PAD1_LIGHTS,bit);
  if (value==0) __DDR_PAD1_LIGHTS=mm_turnOFF(__DDR_PAD1_LIGHTS,bit);
}

void mm_setDDRPad2Light(int bit, int value)
{
  if (!__bitValid(bit)) return;
  if (value==1) __DDR_PAD2_LIGHTS=mm_turnON(__DDR_PAD2_LIGHTS,bit);
  if (value==0) __DDR_PAD2_LIGHTS=mm_turnOFF(__DDR_PAD2_LIGHTS,bit);
}

void mm_setDDRCabinetLight(int bit, int value)
{
  if (!__bitValid(bit)) return;
  if (value==1)
  {
    __DDR_CABINET_LIGHTS=mm_turnON(__DDR_CABINET_LIGHTS,bit);
  }
  if (value==0)
  {
    __DDR_CABINET_LIGHTS=mm_turnOFF(__DDR_CABINET_LIGHTS,bit);
  }
}

void mm_setDDRBassLight(int bit, int value)
{
  if (!__bitValid(bit)) return;
  if (value==1)
  {
    __DDR_BASS_LIGHTS=mm_turnON(__DDR_BASS_LIGHTS,bit);
    if (bit==DDR_DOUBLE_BASS_LIGHTS)
    {
      mm_setBlueLED(0xFF);
    }
  }
  if (value==0)
  {
    __DDR_BASS_LIGHTS=mm_turnOFF(__DDR_BASS_LIGHTS,bit);
    if (bit==DDR_DOUBLE_BASS_LIGHTS)
    {
      mm_setBlueLED(0x00);
    }
  }
}

void mm_setDDRAllOff()
{
  __DDR_CABINET_LIGHTS=0;
  __DDR_BASS_LIGHTS=0;
  __DDR_PAD1_LIGHTS=0;
  __DDR_PAD2_LIGHTS=0;
  mm_setBlueLED(0x00);
}

void mm_setDDRAllOn()
{
  __DDR_CABINET_LIGHTS=0xFF;
  __DDR_BASS_LIGHTS=0xFF;
  __DDR_PAD1_LIGHTS=0xFF;
  __DDR_PAD2_LIGHTS=0xFF;
  mm_setBlueLED(0xFF);
}

bool mm_setKB(bool val)
{
	if (__mm)
	{
		if(val)
		{
			mm_out_report->enablekbd = 1;
			__kb=true;
		}
		else
		{
			mm_out_report->enablekbd = 0;
			__kb=false;
		}
		if (mm_minimaid_SetOutputs(NULL, NULL)==-1)
		{
			__mm=false;
			return false;
		}
		
	}
	return val;
}

void mm_init()
{
	if(__mm)
	{
		mm_out_report->isoout[0]=0xFF;
		mm_out_report->isoout[1]=0xFF;
		mm_out_report->isoout[2]=0xFF;
		mm_out_report->isoout[3]=0xFF;
		mm_out_report->blueled=0xFF;
		mm_out_report->enablekbd = 1;
		if (mm_minimaid_SetOutputs(NULL, NULL)==-1)
		{
			__mm=false;
			return;
		}
		mm_out_report->isoout[0]=0x00;
		mm_out_report->isoout[1]=0x00;
		mm_out_report->isoout[2]=0x00;
		mm_out_report->isoout[3]=0x00;
		mm_out_report->blueled=0x00;
		if (mm_minimaid_SetOutputs(NULL, NULL)==-1)
		{
			__mm=false;
			return;
		}
		mm_setKB(__kb);
	}
}

bool mm_connect_minimaid()
{
	__mm = minimaid_open_device(false);
	if(__mm==1)
	{
		printf("Failed to connect to the minimaid.\n");
		__mm=false;
	}
	else
	{
		printf("Minimaid connected!\n");
		__mm=true;
	}
	mm_init();
	return __mm;
}

void mm_setMMOutputReports(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
	if(__mm)
	{
		mm_out_report->isoout[0]=a;
		mm_out_report->isoout[1]=b;
		mm_out_report->isoout[2]=c;
		mm_out_report->isoout[3]=d;
		mm_out_report->blueled=__BLUE_LED;
		if(__kb)
		{
			mm_out_report->enablekbd=1;
		}
		else
		{
			mm_out_report->enablekbd=0;
		}
	}
}

bool mm_sendDDRMiniMaidUpdate()
{
 // printf("sending data...");
  //place pads into reset mode, always override just in case
  mm_setDDRPad1Light(DDR_DOUBLE_PAD_RESET,1);
  mm_setDDRPad2Light(DDR_DOUBLE_PAD_RESET,1);
  //if mm is not present, check for it again
  if (!__mm)
  {
       // printf("exception!...");
      mm_connect_minimaid();
  }
	        
  if (!__mm)
  {
	//printf("giving up!\n");
	return false;
  }

  if (__mm)
  {
    //printf("sending...");
    mm_setMMOutputReports(__DDR_CABINET_LIGHTS,__DDR_PAD1_LIGHTS,__DDR_PAD2_LIGHTS,__DDR_BASS_LIGHTS);
    if (mm_minimaid_SetOutputs(NULL, NULL)==-1)
    {
      //printf("bug!\n");
      __mm=false;
      printf("Oh shit! The minimaid was found to be disconnected mid-transmit!\n");
      return false;
    }
  }
  //printf("good!\n");
  return true;
}

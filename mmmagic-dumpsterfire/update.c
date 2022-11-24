#define true (1==1)
#define false (1==0)

#include "update.h"

#define DEBUG_VERSION_UP 	0
#if DEBUG_VERSION_UP
#define Debug		printf
#else
#define Debug
#endif

#ifdef WINBLOWS
extern HANDLE HIDHandle;
#else
extern int fd;
#endif
extern unsigned long buffers;

int minimaid_UpdateInputsWork()
{
	#ifdef WINBLOWS
	DWORD bytes_read;
	#else
	int bytes_read;
	#endif
	unsigned int i;
	struct mm_InputReport *iptr;

	if(mm_in_reports == NULL) {
		Debug("UpdateInputs called with NULL in_reports.  Need to initialize?\n");
		return -1;
	}
	
	Debug("Attempting to ReadFile\n");
	

	#ifdef WINBLOWS

	if (ReadFile(HIDHandle, (LPVOID)mm_in_reports, buffers * sizeof(struct mm_InputReport), &bytes_read, NULL) != 0) {
		reports_read = bytes_read / sizeof(struct mm_InputReport);
		Debug("Read %d bytes (%d reports)\n", bytes_read, reports_read);
	} else {
		Debug("\rError reading: %d - read %d bytes\n", GetLastError(), bytes_read);
		return -1;
	}

	#else
	
	bytes_read = read(fd, mm_in_reports, sizeof(struct mm_InputReport));
	if (bytes_read < 0) {
		perror("read");
		return -1;
	} else {
		printf("read() read %d bytes:\n\t", bytes_read);
	}


	#endif

	/* OR all the returned reports together since that's "everything that happened since last requested" */
	iptr = mm_in_reports;
	memset(mm_in_report, 0, sizeof(struct mm_InputReport));
	for(i = 0; i < reports_read; i++)
	{
		mm_in_report->dipsw |= iptr->dipsw;
		mm_in_report->inputs[0] |= iptr->inputs[0];
		mm_in_report->inputs[1] |= iptr->inputs[1];
		mm_in_report->inputs[2] |= iptr->inputs[2];
		mm_in_report->inputs[3] |= iptr->inputs[3];
		mm_in_report->inputs[4] |= iptr->inputs[4];
		iptr++;
	}

	return reports_read;
}

int minimaid_SetOutputsWork()
{
	#ifdef WINBLOWS
	DWORD bytes_written;
	#else
	int bytes_written;
	#endif

	if(mm_out_report == NULL) {
		Debug("SetOutputs called with NULL out_report.  Need to initialize?\n");
		return -1;
	}

	Debug("Attempting to WriteFile\n");
	Debug("Setting USB Output Report - EXTOUT=%02x ISOOUT=%02x%02x%02x%02x BLUELED=%02x ENABLEKBD=%02x",
		mm_out_report->extout,
		mm_out_report->isoout[0],
		mm_out_report->isoout[1],
		mm_out_report->isoout[2],
		mm_out_report->isoout[3],
		mm_out_report->blueled,
		mm_out_report->enablekbd);

	#ifdef WINBLOWS
	if (WriteFile(HIDHandle, (LPCVOID)mm_out_report, sizeof(struct mm_OutputReport), (LPDWORD)&bytes_written, NULL) == 0) {
		Debug("\rError writing: %d - wrote %d bytes\n", GetLastError(), bytes_written);
		return -1;
	}

	#else

	bytes_written = write(fd, mm_out_report, sizeof(struct mm_OutputReport));
	if (bytes_written < 0) {
		Debug("Error: %d\n", errno);
		return -1;
	} else {
		Debug("write() wrote %d bytes\n", bytes_written);
	}
	
	#endif

	return 0;
}

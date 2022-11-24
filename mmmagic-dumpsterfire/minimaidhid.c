
/**
 * This code sucks.
 *
 * I'd like to tip my hat to the Windows API designers for their ingenuity 
 * in somehow foisting this madness on the world, and their sadistic nature,
 * to which I can only aspire to as some twisted ideal.
 *                                                 -- David Yip
 *
 */

#include "minimaidhid.h"



//#define ENABLE_DEBUG_MESSAGES

#define DEBUG_VERSION_HID 	0
#if DEBUG_VERSION_HID
#define Debug		printf
#else
#define Debug
#endif


int initDataStructures()
{
	if(mm_in_reports != NULL)
		free(mm_in_reports);
	if(mm_in_report != NULL)
		free(mm_in_report);
	if(mm_out_report != NULL)
		free(mm_out_report);

	mm_in_reports = (struct mm_InputReport*)malloc(buffers * sizeof(struct mm_InputReport));
	mm_in_report = (struct mm_InputReport*)malloc(sizeof(struct mm_InputReport));
	mm_out_report = (struct mm_OutputReport*)malloc(sizeof(struct mm_OutputReport));

	if(!(mm_in_report != NULL)) return 1;
	if(!(mm_out_report != NULL)) return 1;

	memset(mm_out_report,0, sizeof(struct mm_OutputReport));
	mm_out_report->enablekbd = 1;
	memset(mm_in_report,0, sizeof(struct mm_InputReport));
	return 0;
}



#ifdef WINBLOWS


static HANDLE open_device(const char *path, BOOL enumerate)
{
	HANDLE handle;
	DWORD desired_access = (enumerate)? 0: (GENERIC_WRITE | GENERIC_READ);
	DWORD share_mode = (enumerate)?
	                      FILE_SHARE_READ|FILE_SHARE_WRITE:
	                      FILE_SHARE_READ;

	handle = CreateFileA(path,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
		
		printf("handle attached at %d\n", handle);
		
	return handle;
}


int minimaid_open_device(BOOL async) {
  Debug("mm: init\n");
	int device=0;
	GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, {0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30} };
	SP_DEVICE_INTERFACE_DETAIL_DATA_A *device_interface_detail_data = NULL;
	// USB HID device interface class GUID
	struct _GUID GUID; 

	// device interface class data
	SP_INTERFACE_DEVICE_DATA DeviceInterfaceData; 
	struct {DWORD cbSize; char DevicePath[1024];} FunctionClassDeviceData; 
	HIDD_ATTRIBUTES HIDAttributes; 
	// handles to class device collection and HID, respectively
	HANDLE PnPHandle;

	// number of bytes returned from a read operation
	unsigned long BytesReturned; 

	// manufacturer and product names
	BOOL HasManufacturerName, HasProductName; 
	WCHAR ManufacturerBuffer[256], ProductBuffer[256]; 
	const WCHAR NotSupplied[] = L"NULL"; 

	// Did things works?
	BOOL success;

	// get HID interface device class GUID
	HidD_GetHidGuid(&GUID);

	// for SetupDiEnumDeviceInterfaces
	DeviceInterfaceData.cbSize = sizeof(DeviceInterfaceData);

	PnPHandle = SetupDiGetClassDevs(&GUID, NULL, NULL, DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
	GUARD(PnPHandle != INVALID_HANDLE_VALUE);
 
	for(device = 0; ; device++) 
	{
		HANDLE write_handle = INVALID_HANDLE_VALUE;
		DWORD required_size = 0;
		HIDD_ATTRIBUTES attrib;

		success = SetupDiEnumDeviceInterfaces(PnPHandle,
			NULL,
			&InterfaceClassGuid,
			device,
			&DeviceInterfaceData);
		
		if (!success) {
			// A return of FALSE from this function means that
			// there are no more devices.
			Debug("No more devices found.\n");
			break;
		}

		Debug("Attempting to retrieve details of device %d\n", device);
		// Call with 0-sized detail size, and let the function
		// tell us how long the detail struct needs to be. The
		// size is put in &required_size.
		success = SetupDiGetDeviceInterfaceDetailA(PnPHandle,
			&DeviceInterfaceData,
			NULL,
			0,
			&required_size,
			NULL);
			
			Debug("size: %d\n",required_size);
			device_interface_detail_data = (SP_DEVICE_INTERFACE_DETAIL_DATA_A*) malloc(required_size);
			device_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

		// Allocate a long enough structure for device_interface_detail_data.
		//device_interface_detail_data = (SP_DEVICE_INTERFACE_DETAIL_DATA_A*) malloc(required_size);
		//device_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
		//FunctionClassDeviceData.cbSize = 6;
		FunctionClassDeviceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);

		// Get the detailed data for this device. The detail data gives us
		// the device path for this device, which is then passed into
		// CreateFile() to get a handle to the device.
		success = SetupDiGetDeviceInterfaceDetailA(PnPHandle,
			&DeviceInterfaceData, 
			device_interface_detail_data,
			required_size,
			NULL,
			NULL);
	
		GUARD(success);

		
		
		Debug("Attempting to retrieve details of device %d\n", device);
		
		Debug("Creating Filew on device %d\n", device);
		HIDHandle = open_device(device_interface_detail_data->DevicePath, TRUE);									// hTemplateFile
		
		if (HIDHandle == INVALID_HANDLE_VALUE)
		{
			Debug("Cannot open device %d, continuing with next device\n", device);
			continue;
		}

		success = HidD_GetAttributes(HIDHandle, &HIDAttributes);
		GUARD(success);

		HasManufacturerName = HidD_GetManufacturerString(HIDHandle, ManufacturerBuffer, 256); 
		HasProductName = HidD_GetProductString(HIDHandle, ProductBuffer, 256);  		
		Debug("Vendor ID = %4.4x ", HIDAttributes.VendorID); 
		Debug("Name = %ws, ", HasManufacturerName ? ManufacturerBuffer : NotSupplied); 
		Debug("Product ID = %4.4x ", HIDAttributes.ProductID); 
		Debug("Name = %ls\n", ProductBuffer);   

		if (HIDAttributes.VendorID == 0xBEEF && HIDAttributes.ProductID == 0x5730)
		{
			Debug("Using device %d with VID=%4.4x PID=%4.4x ", device, HIDAttributes.VendorID, HIDAttributes.ProductID);
			HidD_SetNumInputBuffers(HIDHandle, MMHID_MAX_BUFFERS);
			HidD_GetNumInputBuffers(HIDHandle, &buffers);

			Debug("%d buffers available\n", buffers);

			if (initDataStructures()==1) goto error;

			
			break;
		}
	}
	
	if(mm_in_report == NULL || mm_out_report == NULL) {
		Debug("MINIMAID: - Failed to set up HID buffers.\n");
		goto error;
	}

	SetupDiDestroyDeviceInfoList(PnPHandle);

	asyncupdates = async;
	if(async)
	{
		minimaid_input_sem = CreateSemaphore(NULL, 0, 1, NULL);
		minimaid_input_ctx = NULL;
		minimaid_input_callback = NULL;
		CreateThread(NULL, 0, InputThread, NULL, 0, 0);

		minimaid_output_sem = CreateSemaphore(NULL, 0, 1, NULL);
		minimaid_output_ctx = NULL;
		minimaid_output_callback = NULL;
		CreateThread(NULL, 0, OutputThread, NULL, 0 ,0);
	}
	if (HIDAttributes.VendorID != 0xBEEF && HIDAttributes.ProductID != 0x5730) goto error;
	return 0; 

error:
	SetupDiDestroyDeviceInfoList(PnPHandle); 
	Debug("MINIMAID: - Failed to set up HID.\n");
	return 1;
}




#else
sem_private    tokenI;
sem_private    tokenO;




int minimaid_open_device(BOOL async) {
	char devPath[255];
	int res=findMinimaidUdev(&devPath);
	if (res==1) return 1;
	fd = open(devPath, O_RDWR|O_NONBLOCK);
	if (fd < 0) {
		perror("Unable to open minimaid");
		return 1;
	}

	if (initDataStructures()==1) goto error;
	if(mm_in_report == NULL || mm_out_report == NULL) {
		Debug("MINIMAID: - Failed to set up HID buffers.\n");
		goto error;
	}


	return 0;
	
	error:
		close(fd);
		fd=-1;
		Debug("MINIMAID: - Failed to set up HID.\n");
		return 1;
	
}



int findMinimaidUdev(char* devPath)
{
	struct udev *udev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;
	struct udev_device *dev;
	int count=0;
	
	
	/* Create the udev object */
	udev = udev_new();
	if (!udev) {
		printf("Can't create udev\n");
		return 1;
	}
	
	/* Create a list of the devices in the 'hidraw' subsystem. */
	enumerate = udev_enumerate_new(udev);
	udev_enumerate_add_match_subsystem(enumerate, "hidraw");
	udev_enumerate_scan_devices(enumerate);
	devices = udev_enumerate_get_list_entry(enumerate);
	/* For each item enumerated, print out its information.
	   udev_list_entry_foreach is a macro which expands to
	   a loop. The loop will be executed for each member in
	   devices, setting dev_list_entry to a list entry
	   which contains the device's path in /sys. */
	udev_list_entry_foreach(dev_list_entry, devices) {
		const char *path;
		
		/* Get the filename of the /sys entry for the device
		   and create a udev_device object (dev) representing it */
		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(udev, path);

		/* usb_device_get_devnode() returns the path to the device node
		   itself in /dev. */
		memset(devPath,0,sizeof(devPath));
		strcpy(devPath,udev_device_get_devnode(dev));
		printf("Device Node Path: %s\n", devPath);

		/* The device pointed to by dev contains information about
		   the hidraw device. In order to get information about the
		   USB device, get the parent device with the
		   subsystem/devtype pair of "usb"/"usb_device". This will
		   be several levels up the tree, but the function will find
		   it.*/
		dev = udev_device_get_parent_with_subsystem_devtype(
		       dev,
		       "usb",
		       "usb_device");
		if (!dev) {
			printf("Unable to find parent usb device.");
			exit(1);
		}
	
		/* From here, we can call get_sysattr_value() for each file
		   in the device's /sys entry. The strings passed into these
		   functions (idProduct, idVendor, serial, etc.) correspond
		   directly to the files in the directory which represents
		   the USB device. Note that USB strings are Unicode, UCS2
		   encoded, but the strings returned from
		   udev_device_get_sysattr_value() are UTF-8 encoded. */
		if (strcmp ("beef",udev_device_get_sysattr_value(dev,"idVendor"))==0 && strcmp ("5730",udev_device_get_sysattr_value(dev,"idProduct"))==0 )
		{
			count++;
			if (count>1)
			{
				printf("Found minimaid device!\n");
				udev_device_unref(dev);
				udev_enumerate_unref(enumerate);
				udev_unref(udev);
				return 0;
			}
		}

		udev_device_unref(dev);
	}
	/* Free the enumerator object */
	udev_enumerate_unref(enumerate);
	udev_unref(udev);
	memset(devPath,0,sizeof(devPath));
	return 1;       
}





#endif


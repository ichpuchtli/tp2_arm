#include "usb.h"



#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/systick.h"
#include "grlib/grlib.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdmsc.h"
#include "usbStructs.h"
//#include "usb_msc_structs.h"
#include "third_party/fatfs/src/diskio.h"
//#include "drivers/cfal96x64x16.h"



#define SDCARD_PRESENT          0x00000001
#define SDCARD_IN_USE           0x00000002


void USB_Create(void)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    SysCtlDelay(10);
    uDMAControlBaseSet();//&sDMAControlTable[0]);
    ROM_uDMAEnable();

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);


    // Set the USB stack mode to Device mode with VBUS monitoring.

    USBStackModeSet(0, USB_MODE_DEVICE, 0);
//
//    // Pass our device information to the USB library and place the device
//    // on the bus.
//
    USBDMSCInit(0, (tUSBDMSCDevice *)&g_sMSCDevice);
}


struct
{
    unsigned long ulFlags;
}
g_sDriveInformation;


void *
USBDMSCStorageOpen(unsigned long ulDrive)
{

}


void
USBDMSCStorageClose(void * pvDrive)
{

}


unsigned long USBDMSCStorageRead(void * pvDrive,
                                 unsigned char *pucData,
                                 unsigned long ulSector,
                                 unsigned long ulNumBlocks)
{

}


unsigned long USBDMSCStorageWrite(void * pvDrive,
                                  unsigned char *pucData,
                                  unsigned long ulSector,
                                  unsigned long ulNumBlocks)
{

}


unsigned long
USBDMSCStorageNumBlocks(void * pvDrive)
{

}

unsigned long massStorageEventCallback(void* callback, unsigned long event, unsigned long messageParameters, void* messageData)
{
	switch(event)
	{
	case USBD_MSC_EVENT_WRITING:
		{
			break;
		}
	case USBD_MSC_EVENT_READING:
		{
			break;
		}
	case USBD_MSC_EVENT_IDLE:
		{
			break;
		}
	default:
		{
			break;
		}
	}
	return(0);
}

#define USBDMSC_IDLE            0x00000000
#define USBDMSC_NOT_PRESENT     0x00000001
unsigned long USBDMSCStorageStatus(void * pvDrive);

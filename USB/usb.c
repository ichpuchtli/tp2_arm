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
#include "driverlib/udma.h"
#include "grlib/grlib.h"
//#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdmsc.h"
#include "usbStructs.h"
//#include "usb_msc_structs.h"
#include "third_party/fatfs/src/diskio.h"
//#include "drivers/cfal96x64x16.h"

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

#define SDCARD_PRESENT          0x00000001
#define SDCARD_IN_USE           0x00000002

tDMAControlTable sDMAControlTable[64];



#define DISPLAY_BANNER_HEIGHT   11
#define DISPLAY_BANNER_BG       ClrDarkBlue
#define DISPLAY_BANNER_FG       ClrWhite


#define USBMSC_ACTIVITY_TIMEOUT 300

#define FLAG_UPDATE_STATUS      1
static unsigned long g_ulFlags;
static unsigned long g_ulIdleTimeout;

volatile enum
{
    //
    // Unconfigured.
    //
    MSC_DEV_DISCONNECTED,

    //
    // Connected but not yet fully enumerated.
    //
    MSC_DEV_CONNECTED,

    //
    // Connected and fully enumerated but not currently handling a command.
    //
    MSC_DEV_IDLE,

    //
    // Currently reading the SD card.
    //
    MSC_DEV_READ,

    //
    // Currently writing the SD card.
    //
    MSC_DEV_WRITE,
}
g_eMSCState;

void USB_Create(void)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    SysCtlDelay(10);
    ROM_uDMAControlBaseSet(&sDMAControlTable[0]);
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

    unsigned long ulRetcode;
    ulRetcode = disk_initialize(0);

       if(ulRetcode != RES_OK)
       {
    	   GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, (RED_LED | GREEN_LED));
       }
       else
       {
    	   GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, (BLUE_LED | GREEN_LED));
       }


    g_ulIdleTimeout = 0;
    g_ulFlags = 0;
}


struct
{
    unsigned long ulFlags;
}
g_sDriveInformation;


void
SysTickHandler(void)
{
    //
    // Call the FatFs tick timer.
    //
    disk_timerproc();

}

void *
USBDMSCStorageOpen(unsigned long ulDrive)
{
//    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, GREEN_LED);
    unsigned long ulTemp;

    ASSERT(ulDrive == 0);

    //
    // Return if already in use.
    //
    if(g_sDriveInformation.ulFlags & SDCARD_IN_USE)
    {
        return(0);
    }

    //
    // Initialize the drive if it is present.
    //
    ulTemp = disk_initialize(0);

    if(ulTemp == RES_OK)
    {
        //
        // Card is present and in use.
        //
        g_sDriveInformation.ulFlags = SDCARD_PRESENT | SDCARD_IN_USE;
    }
    else if(ulTemp == STA_NODISK)
    {
        //
        // Allocate the card but it is not present.
        //
        g_sDriveInformation.ulFlags = SDCARD_IN_USE;
    }
    else
    {
        return(0);
    }

    return((void *)&g_sDriveInformation);
}

void
USBDMSCStorageClose(void * pvDrive)
{
	unsigned char ucPower;

	ASSERT(pvDrive != 0);

	//
	// Clear all flags.
	//
	g_sDriveInformation.ulFlags = 0;

	//
	// Power up the card.
	//
	ucPower = 0;

	//
	// Turn off the power to the card.
	//
	disk_ioctl(0, CTRL_POWER, &ucPower);
}


unsigned long USBDMSCStorageRead(void * pvDrive,
								 unsigned char *pucData,
								 unsigned long ulSector,
								 unsigned long ulNumBlocks)
{
	ASSERT(pvDrive != 0);

	if(disk_read (0, pucData, ulSector, ulNumBlocks) == RES_OK)
	{
		// TODO remove fixed 512
		return(ulNumBlocks * 512);
	}
	return(0);
}
//
//
//
unsigned long USBDMSCStorageWrite(void * pvDrive,
                                  unsigned char *pucData,
                                  unsigned long ulSector,
                                  unsigned long ulNumBlocks)
{
    ASSERT(pvDrive != 0);

    if(disk_write(0, pucData, ulSector, ulNumBlocks) == RES_OK)
    {
        return(ulNumBlocks * 512);
    }
    return(0);
}

unsigned long
USBDMSCStorageNumBlocks(void * pvDrive)
{
    unsigned long ulSectorCount;

    //
    // Read the number of sectors.
    //
    disk_ioctl(0, GET_SECTOR_COUNT, &ulSectorCount);

    return(ulSectorCount);
}

unsigned long
USBDMSCEventCallback(void *pvCBData, unsigned long ulEvent,
                     unsigned long ulMsgParam, void *pvMsgData)
{
    //
    // Reset the time out every time an event occurs.
    //
    g_ulIdleTimeout = USBMSC_ACTIVITY_TIMEOUT;

    switch(ulEvent)
    {
        //
        // Writing to the device.
        //
        case USBD_MSC_EVENT_WRITING:
        {
            //
            // Only update if this is a change.
            //
            if(g_eMSCState != MSC_DEV_WRITE)
            {
                //
                // Go to the write state.
                //
                g_eMSCState = MSC_DEV_WRITE;

                //
                // Cause the main loop to update the screen.
                //
                g_ulFlags |= FLAG_UPDATE_STATUS;
            }

            break;
        }

        //
        // Reading from the device.
        //
        case USBD_MSC_EVENT_READING:
        {
            //
            // Only update if this is a change.
            //
            if(g_eMSCState != MSC_DEV_READ)
            {
                //
                // Go to the read state.
                //
                g_eMSCState = MSC_DEV_READ;

                //
                // Cause the main loop to update the screen.
                //
                g_ulFlags |= FLAG_UPDATE_STATUS;
            }

            break;
        }
        //
        // The USB host has disconnected from the device.
        //
        case USB_EVENT_DISCONNECTED:
        {
            //
            // Go to the disconnected state.
            //
            g_eMSCState = MSC_DEV_DISCONNECTED;

            //
            // Cause the main loop to update the screen.
            //
            g_ulFlags |= FLAG_UPDATE_STATUS;

            break;
        }
        //
        // The USB host has connected to the device.
        //
        case USB_EVENT_CONNECTED:
        {
            //
            // Go to the idle state to wait for read/writes.
            //
            g_eMSCState = MSC_DEV_IDLE;

            break;
        }
        case USBD_MSC_EVENT_IDLE:
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

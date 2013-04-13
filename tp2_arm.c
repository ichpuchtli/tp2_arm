//*****************************************************************************
//
// project0.c - Example to demonstrate minimal StellarisWare setup
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
//
//*****************************************************************************
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "diskio.h"
#include "ff.h"
#include "driverlib/rom.h"
#include "SDcard/sd.h"
#include "usblib/usblib.h"

#include "USB/usb.h"
#include "USB/usbStructs.h"
#include "wav/processWav.h"
//*****************************************************************************
//
// Define pin to LED color mapping.
//
//*****************************************************************************

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Project Zero (project0)</h1>
//!
//! This example demonstrates the use of StellarisWare to setup the clocks
//! and toggle GPIO pins to make the LED's blink. This is a good place to
//! start understanding your launchpad and the tools that can be used to
//! program it. See http://www.ti.com/stellaris-launchpad/project0 for more
//! information and tutorial videos.
//!
//
//*****************************************************************************


//*****************************************************************************
int
main(void)
{
//    tRectangle sRect;
    //
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / 100);
    ROM_SysTickEnable();
    ROM_SysTickIntEnable();

    //
    // Enable and configure the GPIO port for the LED operation.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED);


    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, RED_LED);
    sd_create();
//    sd_write("ok.txt", "bounce", 6);

//    char *tempBuff;
//    tempBuff = sd_ReadWav("jake.txt", 17, 67);

//    sd_write("new.txt", tempBuff, 17);


    USB_Create();

    processWav_GetWavInfo("test.wav");
    //
    // Loop Forever
    //
    while(1)
    {
//        	GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, (BLUE_LED | GREEN_LED));
//        }
        //
        // Delay for a bit
        //
        SysCtlDelay(2000000);

        //
        // Delay for a bit
        //
        SysCtlDelay(2000000);
    }
}

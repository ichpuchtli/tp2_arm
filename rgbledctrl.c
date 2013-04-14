#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "rgbledctrl.h"

//*****************************************************************************
//
//! Initializes the Timer and GPIO functionality associated with the RGB LED
//!
//! \param ulEnable enables RGB immediately if set.
//!
//! This function must be called during application initialization to
//! configure the GPIO pins to which the LEDs are attached.  It enables
//! the port used by the LEDs and configures each color's Timer. It optionally
//! enables the RGB LED by configuring the GPIO pins and starting the timers.
//!
//! \return None.
//
//*****************************************************************************
void 
RGBInit(unsigned long ulEnable)
{
    //
    // Enable the GPIO Port and Timer for each LED
    //
    SysCtlPeripheralEnable(RED_GPIO_PERIPH);
    SysCtlPeripheralEnable(RED_TIMER_PERIPH);

    SysCtlPeripheralEnable(GREEN_GPIO_PERIPH);
    SysCtlPeripheralEnable(GREEN_TIMER_PERIPH);

    SysCtlPeripheralEnable(BLUE_GPIO_PERIPH);
    SysCtlPeripheralEnable(BLUE_TIMER_PERIPH);

    //
    // Configure each timer for output mode
    //
    HWREG(GREEN_TIMER_BASE + TIMER_O_CFG)   = 0x04;
    HWREG(GREEN_TIMER_BASE + TIMER_O_TAMR)  = 0x0A;
    HWREG(GREEN_TIMER_BASE + TIMER_O_TAILR) = 0xFFFF;

    HWREG(BLUE_TIMER_BASE + TIMER_O_CFG)   = 0x04;
    HWREG(BLUE_TIMER_BASE + TIMER_O_TBMR)  = 0x0A;
    HWREG(BLUE_TIMER_BASE + TIMER_O_TBILR) = 0xFFFF;

    HWREG(RED_TIMER_BASE + TIMER_O_CFG)   = 0x04;
    HWREG(RED_TIMER_BASE + TIMER_O_TBMR)  = 0x0A;
    HWREG(RED_TIMER_BASE + TIMER_O_TBILR) = 0xFFFF;
    
    //
    // Invert the output signals.
    //
    HWREG(RED_TIMER_BASE + TIMER_O_CTL)   |= 0x4000;
    HWREG(GREEN_TIMER_BASE + TIMER_O_CTL)   |= 0x40;
    HWREG(BLUE_TIMER_BASE + TIMER_O_CTL)   |= 0x4000;
    
    if(ulEnable)
    {
        RGBEnable();
    }
}

//*****************************************************************************
//
//! Enable the RGB LED with already configured timer settings
//!
//! This function or RGBDisable should be called during application 
//! initialization to configure the GPIO pins to which the LEDs are attached.  
//! This function enables the timers and configures the GPIO pins as timer
//! outputs.
//!
//! \return None.
//
//*****************************************************************************
void 
RGBEnable(void)
{
    //
    // Enable timers to begin counting
    //
    TimerEnable(RED_TIMER_BASE, TIMER_BOTH);
    TimerEnable(GREEN_TIMER_BASE, TIMER_BOTH);
    TimerEnable(BLUE_TIMER_BASE, TIMER_BOTH);
    
    //
    // Reconfigure each LED's GPIO pad for timer control
    //
    GPIOPinConfigure(GREEN_GPIO_PIN_CFG);
    GPIOPinTypeTimer(GREEN_GPIO_BASE, GREEN_GPIO_PIN);
    GPIOPadConfigSet(GREEN_GPIO_BASE, GREEN_GPIO_PIN, GPIO_STRENGTH_8MA_SC,
                     GPIO_PIN_TYPE_STD);

    GPIOPinConfigure(BLUE_GPIO_PIN_CFG);
    GPIOPinTypeTimer(BLUE_GPIO_BASE, BLUE_GPIO_PIN);
    GPIOPadConfigSet(BLUE_GPIO_BASE, BLUE_GPIO_PIN, GPIO_STRENGTH_8MA_SC,
                     GPIO_PIN_TYPE_STD);

    GPIOPinConfigure(RED_GPIO_PIN_CFG);
    GPIOPinTypeTimer(RED_GPIO_BASE, RED_GPIO_PIN);
    GPIOPadConfigSet(RED_GPIO_BASE, RED_GPIO_PIN, GPIO_STRENGTH_8MA_SC,
                     GPIO_PIN_TYPE_STD);
}

//*****************************************************************************
//
//! Disable the RGB LED by configuring the GPIO's as inputs.
//!
//! This function or RGBEnable should be called during application 
//! initialization to configure the GPIO pins to which the LEDs are attached.  
//! This function disables the timers and configures the GPIO pins as inputs 
//! for minimum current draw.
//!
//! \return None.
//
//*****************************************************************************
void
RGBDisable(void)
{
    //
    // Configure the GPIO pads as general purpose inputs.
    //
    GPIOPinTypeGPIOInput(RED_GPIO_BASE, RED_GPIO_PIN);
    GPIOPinTypeGPIOInput(GREEN_GPIO_BASE, GREEN_GPIO_PIN);
    GPIOPinTypeGPIOInput(BLUE_GPIO_BASE, BLUE_GPIO_PIN);

    //
    // Stop the timer counting.
    //
    TimerDisable(RED_TIMER_BASE, TIMER_BOTH);
    TimerDisable(GREEN_TIMER_BASE, TIMER_BOTH);
    TimerDisable(BLUE_TIMER_BASE, TIMER_BOTH);
}

void
RGBColorSet(volatile uint8_t* pucRGBColor)
{
    TimerMatchSet(RED_TIMER_BASE, RED_TIMER, (pucRGBColor[RED]<<8) & 0xFFFF);
    TimerMatchSet(GREEN_TIMER_BASE, GREEN_TIMER, (pucRGBColor[GREEN]<<8) & 0xFFFF);
    TimerMatchSet(BLUE_TIMER_BASE, BLUE_TIMER, (pucRGBColor[BLUE]<<8) & 0xFFFF);
}

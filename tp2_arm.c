#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "timers.h"
#include "talloc.h"

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

void vTimer0_Event(void){
    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, GREEN_LED);
}

void vTimer1_Event(void){

    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, BLUE_LED);
}

void vTimer3_Event(void){

    vTimersDisable(TIMER2_BASE, TIMER_BOTH);

    vTallocFreeTimer(TIMER2_BASE, TIMER_BOTH);

    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, 0x0);
}
int main(void)
{
    //
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    //
    SysCtlClockSet(SYSCTL_SYSDIV_8|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    //
    // Enable and configure the GPIO port for the LED operation.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED);
    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, RED_LED);


    /*
    if(cTallocReserveTimer(TIMER2_BASE, TIMER_A)){

        vTimersSplitPeriodic(TIMER2_BASE, TIMER_A, 311);

        vTimersAddEventHook(TIMER2_BASE, TIMER_A, &vTimer0_Event);

        vTimersEnable(TIMER2_BASE, TIMER_A);
    }

    */
    if(cTallocReserveTimer(TIMER2_BASE, TIMER_B)){

        vTimersSplitPeriodic(TIMER2_BASE, TIMER_B, 247);

        vTimersAddEventHook(TIMER2_BASE, TIMER_B, &vTimer1_Event);

        vTimersEnable(TIMER2_BASE, TIMER_B);
    }

    if(cTallocReserveTimer(TIMER3_BASE, TIMER_BOTH)){

        vTimersFullWidthOneShot(TIMER4_BASE, 15000000);

        vTimersAddEventHook(TIMER4_BASE, TIMER_A, &vTimer3_Event);

        vTimersEnable(TIMER4_BASE, TIMER_A);
    }

    // Loop Forever
    while(1)
    {
    }
}

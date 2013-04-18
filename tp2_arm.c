#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "taskq.h"
#include "timers.h"

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

void vThreadExample_Task(void* vpData){

    /* NOTE/TODO: recurring tasks need to enqueue themselves immediately */
    /* Append this function to the end of the Task Queue */
    /* This will create a recuring thread of exection. use static variables as globals*/
    vTaskQAppend(vThreadExample_Task, vpData);

    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, GREEN_LED);
}

void vLEDBlinkRED_Task(void* vpData){

    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, RED_LED);

    SysCtlDelay(100000);

    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, RED_LED);
}

void vTimer0_Event(void){}

void vTimer1_Event(void){

    vTaskQAppend(vLEDBlinkRED_Task, (void*) 0x0);
}

int main(void)
{
    //
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    //
    // Enable and configure the GPIO port for the LED operation.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED);
    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED, RED_LED);


    /* Set up timer0 to occur every millisecond */
    //vTimer0IntInit(1000);

    /* Set up timer1 to occur every 500 milliseconds */
    vTimer1IntInit(500000);

    /* Add the ThreadExample task to the queue to begin the chain of function calls */
    vTaskQAppend((func_t) vThreadExample_Task, (void*) 0x0 );

    // Loop Forever
    while(1)
    {
        vTaskQRun();
    }
}

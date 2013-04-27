#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"

#include "uartcomm.h"

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

int
main(void)
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


    vUARTCommInit();
    vUARTCommMapStdio();

    /* Asynchronous Method */
    vUARTCommSendByte('\r');
    vUARTCommSendByte('\n');
    vUARTCommSendString("Hello World!\r\n");


    while(1)
    {
      UARTprintf("\r\nHello World!\r\n");

      SysCtlDelay(100000);
    }

}

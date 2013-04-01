#include "inc/hw_types.h" // tBoolean type
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"

#include "uartcomm.h"

size_t ulStringLen(const uint8_t* pucString){

  size_t size = 0;

  while(*pucString++) size++;

  return size;

}

void vUARTCommSendByte(const uint8_t ucByte){

  UARTCharPutNonBlocking(UART0_BASE, ucByte);
}

void vUARTCommSendStream(const uint8_t* pucBuffer, size_t ulCount){

  while(ulCount--) 
    vUARTCommSendByte(*pucBuffer++);
}

void vUARTCommSendString(const uint8_t* pucString){

  vUARTCommSendStream(pucString, ulStringLen(pucString));
}


void vUARTCommInit(void){

  /* Enable Peripherals use for UART0 */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  /* Configure UART0 Operation */
  UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), UARTCOMM_BAUDRATE, UARTCOMM_OPTIONS);

  /* Enable UART Interrupts */
  UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
  IntEnable(INT_UART0);
  UARTIntEnable(UART0_BASE, UART_INT_RX);

}

void vUARTCommCLIIntHandler(void){}
void vUARTCommRouterIntHandler(void){}

void vUARTCommEchoIntHandler(void){

    uint32_t ulStatus;

    /* Get the interrrupt status. */
    ulStatus = UARTIntStatus(UART0_BASE, true);

    /* Clear the asserted interrupts. */
    UARTIntClear(UART0_BASE, ulStatus);

    /* Loop while there are characters in the receive FIFO. */
    while(UARTCharsAvail(UART0_BASE))
    {
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE));
        UARTCharPutNonBlocking(UART0_BASE, '\n');
        UARTCharPutNonBlocking(UART0_BASE, '\r');
        
    }
}

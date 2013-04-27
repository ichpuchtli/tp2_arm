#ifndef _UART_COMM_H_
#define _UART_COMM_H_

#include "stddef.h"
#include "stdint.h"

#define UARTCOMM_BAUDRATE 115200
#define UARTCOMM_OPTIONS  (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)

/* Interrupt Handlers */
void vUARTCommEchoIntHandler(void);
void vUARTCommCLIIntHanlder(void);
void vUARTCommRouterIntHandler(void);

void vUARTCommSendByte(int8_t cByte);
void vUARTCommSendStream(int8_t* pcBuffer, size_t ulCount);
void vUARTCommSendString(int8_t* pcString);

void vUARTCommInit(void);
void vUARTCommEnableInterrupts(void);
void vUARTCommMapStdio(void);

size_t ulCommStringLen(int8_t* pcString);

#endif

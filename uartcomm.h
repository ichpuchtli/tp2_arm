#ifndef _UART_COMM_H_
#define _UART_COMM_H_

#include "stddef.h"
#include "stdint.h"

#define UARTCOMM_BAUDRATE 115200
#define UARTCOMM_OPTIONS  (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)

/* Interrupt Handlers */
extern void vUARTCommInt_Event(void);

void vUARTCommSendByte(char cByte);
void vUARTCommSendStream(char* pcBuffer, size_t ulCount);
void vUARTCommSendString(char* pcString);

void vUARTCommInit(void);
void vUARTCommEnableInterrupts(void);
void vUARTCommMapStdio(void);

size_t ulCommStringLen(char* pcString);

#endif

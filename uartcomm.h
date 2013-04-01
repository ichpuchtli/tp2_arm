#ifndef _UART_COMM_H_
#define _UART_COMM_H_

#include "stdtypes.h"

#define UARTCOMM_BAUDRATE 115200
#define UARTCOMM_OPTIONS  (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)

/* Interrupt Handlers */
void vUARTCommEchoIntHandler(void);
void vUARTCommCLIIntHanlder(void);
void vUARTCommRouterIntHandler(void);

void vUARTCommSendByte(const uint8_t ucByte);
void vUARTCommSendStream(const uint8_t* pucBuffer, size_t ulCount);
void vUARTCommSendString(const uint8_t* pucString);

size_t ulCommStringLen(const uint8_t* pucString);

#endif

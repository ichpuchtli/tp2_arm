#ifndef _ADC_MUX_H_
#define _ADC_MUX_H_

#include "stdtypes.h"

void vADCMuxInit(void);

void vADCMuxSelect(uint8_t ucChannel);

void vADCMuxTrigger(void);

uint16_t usADCMuxGetValue(void);

#endif

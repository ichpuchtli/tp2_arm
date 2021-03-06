#ifndef _SPI_DAC_H_
#define _SPI_DAC_H_

#include "stddef.h"
#include "stdint.h"

#define SPIDAC_FREQ 44100 // 44.1 kHz

void vSPIDACInit(void);

void vSPIDACWrite(uint16_t usData);

#endif

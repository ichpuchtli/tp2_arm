#ifndef _SPI_DAC_H_
#define _SPI_DAC_H_

<<<<<<< HEAD
#include "stdtypes.h"

#define DACSPI_FREQ 44100 // 44.1 kHz
=======
#include "stddef.h"
#include "stdint.h"

#define SPIDAC_FREQ 44100 // 44.1 kHz
>>>>>>> bucket

void vSPIDACInit(void);

void vSPIDACWrite(uint16_t usData);

<<<<<<< HEAD
void vSPIDACUpdateRoutine(void);

=======
>>>>>>> bucket
#endif

#ifndef _ADC_MUX_H_
#define _ADC_MUX_H_

#include "stdint.h"

enum {

    EFFECT_POT0,
    EFFECT_POT1,
    EFFECT_POT2,
    EFFECT_POT3,
    LFO_POT
};

void vADCMuxInit(void);

void vADCMuxSelect(uint8_t ucChannel);

void vADCMuxTrigger(void);

uint16_t usADCMuxGetValue(void);

#endif

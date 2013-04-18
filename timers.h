#ifndef _TIMERS_H_
#define _TIMERS_H_

#include "stdtypes.h"

void vTimer0IntInit(uint32_t ulPeriodus);
void vTimer1IntInit(uint32_t ulPeriodus);

void vTimer0SetPeriod(uint32_t ulPeriodus);
void vTimer1SetPeriod(uint32_t ulPeriodus);

void vTimer0Disable(void);
void vTimer1Disable(void);

void vTimer0Enable(void);
void vTimer1Enable(void);

/* These must be implemented by the user */
extern void vTimer0_Event(void);
extern void vTimer1_Event(void);

#endif

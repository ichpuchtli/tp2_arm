#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "stdint.h"

void vKeyPadInit(void);

/* User must call this function frequently to demultiplex the keys */
void vKeyPadRotateRowSelection_Hook(void);

/* User defined event handler */
extern void vKeyPadPress_Event(uint32_t ulPin);
#endif

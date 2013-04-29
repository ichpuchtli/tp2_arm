#ifndef _TALLOC_H_
#define _TALLOC_H_

#include "stdint.h"
#include "stdbool.h"

#define TALLOC_INVALID_TIMER (false)
#define TALLOC_ALREADY_RESERVED (false)

// TODO Wide Timer Support

// Allocate & Deallocate Timers
bool cTallocReserveTimer(uint32_t ulBase, uint32_t ulTimer);
void vTallocFreeTimer(uint32_t ulBase, uint32_t ulTimer);

#endif

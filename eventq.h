#ifndef _EVENTQ_H_
#define _EVENTQ_H_

#include "stdtypes.h"

#define EVENTQSIZE 32

typedef void (*func_t)(void*);

void vEventQRegisterEvent(func_t fFunc, void* vpData, uint64_t ulTime);

size_t ulEventQGetSize(void);

#endif

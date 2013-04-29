#ifndef _EVENTQ_H_
#define _EVENTQ_H_

#include "stdint.h"

#define EVENTQSIZE 32

typedef void (*func_t)(void*);

void vEventQRegisterEvent(func_t fFunc, void* vpData, uint64_t ulTickCount);

void vEventQStartOneShot(func_t fFunc, void* vpData, uint64_t ulPeriodus);

void vEventQStartPeriodic(func_t fFunc, void* vpData, uint64_t ulPeriodus);

size_t ulEventQGetSize(void);

#endif

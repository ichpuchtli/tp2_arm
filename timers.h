#ifndef _TIMERS_H_
#define _TIMERS_H_

#include "stdint.h"

// TODO Wide Timer Support
// NOTE Split Timers with 80MHz Main Clock have maximum period of 819 uSeconds
// NOTE Full Width Timers with 80MHz Main Clock have maximum period of 53 000 000 uSeconds
// NOTE higher prescalers can produce longer periods but with less resolution

void vTimersSetPeriod(uint32_t ulBase, uint32_t ulTimer, uint32_t ulPeriodus);
void vTimersDisable(uint32_t ulBase, uint32_t ulTimer);
void vTimersEnable(uint32_t ulBase, uint32_t ulTimer);

void vTimersFullWidthOneShot(uint32_t ulBase, uint32_t ulPeriodus);

void vTimersSplitOneShot(uint32_t ulBase, uint32_t ulTimer, uint32_t ulPeriodus);

void vTimersFullWidthPeriodic(uint32_t ulBase, uint32_t ulPeriodus);

void vTimersSplitPeriodic(uint32_t ulBase, uint32_t ulTimer, uint32_t ulPeriodus);

// Register Timer Interrupt Handlers
void vTimersAddEventHook(uint32_t ulBase, uint32_t ulTimer, void (*func)(void));
void vTimersRemoveEventHooks(uint32_t ulBase, uint32_t ulTimer);

#endif

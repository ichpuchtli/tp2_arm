#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#include "talloc.h"

// [Timer0A,Timer0B,Timer1A,...]
static uint32_t g_ulTallocBitVector;

bool cTallocReserveTimer(uint32_t ulBase, uint32_t ulTimer){

    uint8_t ucBaseTimerNum = 0x0;
    uint32_t ulTimersDemanded = 0x0;

    switch(ulBase){

        case TIMER0_BASE: ucBaseTimerNum = 0; break;
        case TIMER1_BASE: ucBaseTimerNum = 1; break;
        case TIMER2_BASE: ucBaseTimerNum = 2; break;
        case TIMER3_BASE: ucBaseTimerNum = 3; break;
        case TIMER4_BASE: ucBaseTimerNum = 4; break;
        case TIMER5_BASE: ucBaseTimerNum = 5; break;
        default: return TALLOC_INVALID_TIMER; 
    }

    if( ulTimer & TIMER_A )
        ulTimersDemanded |=  1 << (2*ucBaseTimerNum);

    if( ulTimer & TIMER_B )
        ulTimersDemanded |=  1 << (2*ucBaseTimerNum + 1);

    if( g_ulTallocBitVector & ulTimersDemanded )
        return TALLOC_ALREADY_RESERVED;

    return true;

}

void vTallocFreeTimer(uint32_t ulBase, uint32_t ulTimer){

    uint8_t ucBaseTimerNum = 0x0;
    uint32_t ulTimersToFree = 0x0;

    switch(ulBase){

        case TIMER0_BASE: ucBaseTimerNum = 0; break;
        case TIMER1_BASE: ucBaseTimerNum = 1; break;
        case TIMER2_BASE: ucBaseTimerNum = 2; break;
        case TIMER3_BASE: ucBaseTimerNum = 3; break;
        case TIMER4_BASE: ucBaseTimerNum = 4; break;
        case TIMER5_BASE: ucBaseTimerNum = 5; break;
    }

    if( ulTimer & TIMER_A )
        ulTimersToFree |=  1 << (2*ucBaseTimerNum);

    if( ulTimer & TIMER_B )
        ulTimersToFree |=  1 << (2*ucBaseTimerNum + 1);

    g_ulTallocBitVector &= ~(ulTimersToFree);

}

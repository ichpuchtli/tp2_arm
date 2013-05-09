#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "timers.h"
#include "talloc.h"

#define SEQ_TIMERS_SIZE 12 

enum SEQ_TIMER_ENUM {

    TIMERS_SUBTIMER0A,
    TIMERS_SUBTIMER0B,
    TIMERS_SUBTIMER1A,
    TIMERS_SUBTIMER1B,
    TIMERS_SUBTIMER2A,
    TIMERS_SUBTIMER2B,
    TIMERS_SUBTIMER3A,
    TIMERS_SUBTIMER3B,
    TIMERS_SUBTIMER4A,
    TIMERS_SUBTIMER4B,
    TIMERS_SUBTIMER5A,
    TIMERS_SUBTIMER5B,

};

// Timer0A,Timer0B,Timer1A...etc
// For possible event hooks for any single timer interrupt
void (*xEventHookMatrix[SEQ_TIMERS_SIZE])(void) = {0};


void vTimersSetPeriod(uint32_t ulBase, uint32_t ulTimer, uint32_t ulPeriodus){

    ROM_TimerLoadSet(ulBase, ulTimer, ROM_SysCtlClockGet() / 1000000 * ulPeriodus);
}

void vTimersDisable(uint32_t ulBase, uint32_t ulTimer){

    ROM_TimerDisable(ulBase, ulTimer);
}

void vTimersEnable(uint32_t ulBase, uint32_t ulTimer){

    ROM_TimerEnable(ulBase, ulTimer);
}

void vTimersFullWidthOneShot(uint32_t ulBase, uint32_t ulPeriodus){

    switch(ulBase){
        case TIMER0_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        case TIMER1_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        case TIMER2_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
        case TIMER3_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
        case TIMER4_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
        case TIMER5_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
    }

    ROM_TimerConfigure(ulBase, TIMER_CFG_ONE_SHOT);

    vTimersSetPeriod(ulBase, TIMER_BOTH, ulPeriodus);

    switch(ulBase){
        case TIMER0_BASE: ROM_IntEnable(INT_TIMER0A);
        case TIMER1_BASE: ROM_IntEnable(INT_TIMER1A);
        case TIMER2_BASE: ROM_IntEnable(INT_TIMER2A);
        case TIMER3_BASE: ROM_IntEnable(INT_TIMER3A);
        case TIMER4_BASE: ROM_IntEnable(INT_TIMER4A);
        case TIMER5_BASE: ROM_IntEnable(INT_TIMER5A);
    }

    ROM_TimerIntEnable(ulBase, TIMER_TIMA_TIMEOUT);
}

void vTimersSplitOneShot(uint32_t ulBase, uint32_t ulTimer, uint32_t ulPeriodus){

    switch(ulBase){
        case TIMER0_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); break;
        case TIMER1_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); break;
        case TIMER2_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2); break;
        case TIMER3_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3); break;
        case TIMER4_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4); break;
        case TIMER5_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5); break;
    }

    if(ulTimer & TIMER_A)
        ROM_TimerConfigure(ulBase, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_ONE_SHOT);

    if(ulTimer & TIMER_B)
        ROM_TimerConfigure(ulBase, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_ONE_SHOT);

    vTimersSetPeriod(ulBase, ulTimer, ulPeriodus);

    if(ulTimer & TIMER_A){
        switch(ulBase){
            case TIMER0_BASE: ROM_IntEnable(INT_TIMER0A); break;
            case TIMER1_BASE: ROM_IntEnable(INT_TIMER1A); break;
            case TIMER2_BASE: ROM_IntEnable(INT_TIMER2A); break;
            case TIMER3_BASE: ROM_IntEnable(INT_TIMER3A); break;
            case TIMER4_BASE: ROM_IntEnable(INT_TIMER4A); break;
            case TIMER5_BASE: ROM_IntEnable(INT_TIMER5A); break;
        }
    }


    if(ulTimer & TIMER_B){
        switch(ulBase){
            case TIMER0_BASE: ROM_IntEnable(INT_TIMER0B); break;
            case TIMER1_BASE: ROM_IntEnable(INT_TIMER1B); break;
            case TIMER2_BASE: ROM_IntEnable(INT_TIMER2B); break;
            case TIMER3_BASE: ROM_IntEnable(INT_TIMER3B); break;
            case TIMER4_BASE: ROM_IntEnable(INT_TIMER4B); break;
            case TIMER5_BASE: ROM_IntEnable(INT_TIMER5B); break;
        }
    }

    if(ulTimer & TIMER_A)
        ROM_TimerIntEnable(ulBase, TIMER_TIMA_TIMEOUT);

    if(ulTimer & TIMER_B)
        ROM_TimerIntEnable(ulBase, TIMER_TIMB_TIMEOUT);

}

void vTimersFullWidthPeriodic(uint32_t ulBase, uint32_t ulPeriodus){

    switch(ulBase){
        case TIMER0_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        case TIMER1_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        case TIMER2_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
        case TIMER3_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
        case TIMER4_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
        case TIMER5_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
    }

    ROM_TimerConfigure(ulBase, TIMER_CFG_PERIODIC);

    vTimersSetPeriod(ulBase, TIMER_BOTH, ulPeriodus);

    switch(ulBase){
        case TIMER0_BASE: ROM_IntEnable(INT_TIMER0A);
        case TIMER1_BASE: ROM_IntEnable(INT_TIMER1A);
        case TIMER2_BASE: ROM_IntEnable(INT_TIMER2A);
        case TIMER3_BASE: ROM_IntEnable(INT_TIMER3A);
        case TIMER4_BASE: ROM_IntEnable(INT_TIMER4A);
        case TIMER5_BASE: ROM_IntEnable(INT_TIMER5A);
    }

    ROM_TimerIntEnable(ulBase, TIMER_TIMA_TIMEOUT);

}

void vTimersSplitPeriodic(uint32_t ulBase, uint32_t ulTimer, uint32_t ulPeriodus){

    switch(ulBase){
        case TIMER0_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); break;
        case TIMER1_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); break;
        case TIMER2_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2); break;
        case TIMER3_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3); break;
        case TIMER4_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4); break;
        case TIMER5_BASE: ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5); break;
    }

    // TODO: ERROR these statements overwrites existing configuration
    if(ulTimer & TIMER_A)
        ROM_TimerConfigure(ulBase, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);

    if(ulTimer & TIMER_B)
        ROM_TimerConfigure(ulBase, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC);

    vTimersSetPeriod(ulBase, ulTimer, ulPeriodus);

    if(ulTimer & TIMER_A){
        switch(ulBase){
            case TIMER0_BASE: ROM_IntEnable(INT_TIMER0A); break;
            case TIMER1_BASE: ROM_IntEnable(INT_TIMER1A); break;
            case TIMER2_BASE: ROM_IntEnable(INT_TIMER2A); break;
            case TIMER3_BASE: ROM_IntEnable(INT_TIMER3A); break;
            case TIMER4_BASE: ROM_IntEnable(INT_TIMER4A); break;
            case TIMER5_BASE: ROM_IntEnable(INT_TIMER5A); break;
        }
    }

    if(ulTimer & TIMER_B){
        switch(ulBase){
            case TIMER0_BASE: ROM_IntEnable(INT_TIMER0B); break;
            case TIMER1_BASE: ROM_IntEnable(INT_TIMER1B); break;
            case TIMER2_BASE: ROM_IntEnable(INT_TIMER2B); break;
            case TIMER3_BASE: ROM_IntEnable(INT_TIMER3B); break;
            case TIMER4_BASE: ROM_IntEnable(INT_TIMER4B); break;
            case TIMER5_BASE: ROM_IntEnable(INT_TIMER5B); break;
        }
    }

    if(ulTimer & TIMER_A)
        ROM_TimerIntEnable(ulBase, TIMER_TIMA_TIMEOUT);

    if(ulTimer & TIMER_B)
        ROM_TimerIntEnable(ulBase, TIMER_TIMB_TIMEOUT);

}


// Register Timer Interrupt Handlers
void vTimersAddEventHook(uint32_t ulBase, uint32_t ulTimer, void (*func)(void)){

    uint8_t ucBaseTimerNum = 0x0;

    switch(ulBase){

        case TIMER0_BASE: ucBaseTimerNum = 0; break;
        case TIMER1_BASE: ucBaseTimerNum = 1; break;
        case TIMER2_BASE: ucBaseTimerNum = 2; break;
        case TIMER3_BASE: ucBaseTimerNum = 3; break;
        case TIMER4_BASE: ucBaseTimerNum = 4; break;
        case TIMER5_BASE: ucBaseTimerNum = 5; break;
    }

    if( ulTimer & TIMER_A )
        xEventHookMatrix[2*ucBaseTimerNum] = func;

    if( ulTimer & TIMER_B )
        xEventHookMatrix[2*ucBaseTimerNum+1] = func;
}

void vTimersRemoveEventHooks(uint32_t ulBase, uint32_t ulTimer){

    uint8_t ucBaseTimerNum = 0x0;

    switch(ulBase){

        case TIMER0_BASE: ucBaseTimerNum = 0; break;
        case TIMER1_BASE: ucBaseTimerNum = 1; break;
        case TIMER2_BASE: ucBaseTimerNum = 2; break;
        case TIMER3_BASE: ucBaseTimerNum = 3; break;
        case TIMER4_BASE: ucBaseTimerNum = 4; break;
        case TIMER5_BASE: ucBaseTimerNum = 5; break;
    }

    if( ulTimer & TIMER_A )
        xEventHookMatrix[2*ucBaseTimerNum] = (void (*)(void)) 0x0;

    if( ulTimer & TIMER_B )
        xEventHookMatrix[2*ucBaseTimerNum+1] = (void (*)(void)) 0x0;

}

void vTimersInt_Event(uint8_t timer){

    void (*func)(void) = xEventHookMatrix[timer];

    if(func) (*func)();
}

//////////////////////////////////////////////////////////////////////////////
// Interrupt Handlers
//////////////////////////////////////////////////////////////////////////////

void vTimers0AIntHandler(void){

    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER0A);
}

void vTimers0BIntHandler(void){

    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER0B);
}


void vTimers1AIntHandler(void){

    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER1A);
}

void vTimers1BIntHandler(void){

    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER1B);
}

void vTimers2AIntHandler(void){

    ROM_TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER2A);
}

void vTimers2BIntHandler(void){

    ROM_TimerIntClear(TIMER2_BASE, TIMER_TIMB_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER2B);
}

void vTimers3AIntHandler(void){

    ROM_TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER3A);
}

void vTimers3BIntHandler(void){

    ROM_TimerIntClear(TIMER3_BASE, TIMER_TIMB_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER3B);
}

void vTimers4AIntHandler(void){

    ROM_TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER4A);
}

void vTimers4BIntHandler(void){

    ROM_TimerIntClear(TIMER4_BASE, TIMER_TIMB_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER4B);
}

void vTimers5AIntHandler(void){

    ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER5A);
}

void vTimers5BIntHandler(void){

    ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMB_TIMEOUT);

    vTimersInt_Event(TIMERS_SUBTIMER5B);
}

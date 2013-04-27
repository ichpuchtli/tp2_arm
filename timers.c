#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "timers.h"

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif


void vTimer0IntInit(uint32_t ulPeriodus){

    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //
    // Configure the two 32-bit periodic timers.
    //
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet() / 1000000 * ulPeriodus);

    //
    // Setup the interrupts for the timer timeouts.
    //
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);


    //
    // Enable the timers.
    //
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
}

void vTimer1IntInit(uint32_t ulPeriodus){

    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);


    //
    // Configure the two 32-bit periodic timers.
    //
    ROM_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER1_BASE, TIMER_A, ROM_SysCtlClockGet() / 1000000 * ulPeriodus);

    //
    // Setup the interrupts for the timer timeouts.
    //
    ROM_IntEnable(INT_TIMER1A);
    ROM_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timers.
    //
    ROM_TimerEnable(TIMER1_BASE, TIMER_A);

}


//////////////////////////////////////////////////////////////////////////////
// One Shot Setups
//////////////////////////////////////////////////////////////////////////////
void vTimer2AStartOneShot(uint32_t ulPeriodus){

    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

    //
    // Configure the two 32-bit periodic timers.
    //
    ROM_TimerConfigure(TIMER2_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_ONE_SHOT);

    //
    // Setup the interrupts for the timer timeouts.
    //
    ROM_TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    vTimer2ASetPeriod(ulPeriodus);

    vTimer2AEnable();
}

void vTimer2BOneShot(uint32_t ulPeriodus){

    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

    //
    // Configure the two 32-bit periodic timers.
    //
    ROM_TimerConfigure(TIMER2_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_ONE_SHOT);

    ROM_IntPriority(INT_TIMER2B, 0x20); // Lower Priority
    ROM_IntEnable(INT_TEMER2B);

    //
    // Setup the interrupts for the timer timeouts.
    //
    ROM_TimerIntEnable(TIMER2_BASE, TIMER_TIMB_TIMOUT);

    vTimer2BSetPeriod(ulPeriodus);

    vTimer2BEnable();
}

//////////////////////////////////////////////////////////////////////////////
// Period Setters
//////////////////////////////////////////////////////////////////////////////
void vTimer2ASetPeriod(uint32_t ulPeriodus){

    ROM_TimerLoadSet(TIMER2_BASE, TIMER_A, ROM_SysCtlClockGet() / 1000000 * ulPeriodus);
}

void vTimer2BSetPeriod(uint32_t ulPeriodus){

    ROM_TimerLoadSet(TIMER2_BASE, TIMER_B, ROM_SysCtlClockGet() / 1000000 * ulPeriodus);
}

//////////////////////////////////////////////////////////////////////////////
// Interrupt Handlers
//////////////////////////////////////////////////////////////////////////////

void vTimer2ADisable(void){
    
    ROM_TimerDisable(TIMER2_BASE, TIMER_A);
}

void vTimer2BDisable(void){

    ROM_TimerDisable(TIMER2_BASE, TIMER_B);
}

//////////////////////////////////////////////////////////////////////////////
// Interrupt Handlers
//////////////////////////////////////////////////////////////////////////////

void vTimer2AEnable(void){

    ROM_TimerEnable(TIMER2_BASE, TIMER_A);
}

void vTimer2BEnable(void){

    ROM_TimerEnable(TIMER2_BASE, TIMER_B);
}

//////////////////////////////////////////////////////////////////////////////
// Interrupt Handlers
//////////////////////////////////////////////////////////////////////////////

void Timer0AIntHandler(void) {

    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    (void) vTimer0A_Event();
}

void Timer0BIntHandler(void) {

    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMB_TIMEOUT);

    (void) vTimer0B_Event();
}

//////////////////////////////////////////////////////////////////////////////

void Timer1AIntHandler(void) {

    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    (void) vTimer1A_Event();
}

void Timer1BIntHandler(void) {

    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);

    (void) vTimer1B_Event();
}
//////////////////////////////////////////////////////////////////////////////

void Timer2AIntHandler(void) {

    ROM_TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

    (void) vTimer2A_Event();
}
void Timer2BIntHandler(void) {

    ROM_TimerIntClear(TIMER2_BASE, TIMER_TIMB_TIMEOUT);

    (void) vTimer2B_Event();
}

//////////////////////////////////////////////////////////////////////////////

void Timer3AIntHandler(void) {

    ROM_TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);

    (void) vTimer3A_Event();
}

void Timer3BIntHandler(void) {

    ROM_TimerIntClear(TIMER3_BASE, TIMER_TIMB_TIMEOUT);

    (void) vTimer3B_Event();
}
//////////////////////////////////////////////////////////////////////////////

void Timer4AIntHandler(void) {

    ROM_TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);

    (void) vTimer4A_Event();
}
void Timer4BIntHandler(void) {

    ROM_TimerIntClear(TIMER4_BASE, TIMER_TIMB_TIMEOUT);

    (void) vTimer4B_Event();
}
//////////////////////////////////////////////////////////////////////////////

void Timer5AIntHandler(void) {

    ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);

    (void) vTimer5A_Event();
}
void Timer5BIntHandler(void) {

    ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMB_TIMEOUT);

    (void) vTimer5B_Event();
}

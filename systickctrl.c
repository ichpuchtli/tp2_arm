#include "systickctrl.h"

#include "inc/hw_types.h" // tBoolean type
#include "driverlib/sysctl.h" // SYSCTL_*
#include "driverlib/systick.h" // SYSCTL_*

void vSysTickCtrlInit(uint32_t ulTickPeriod){

    SysTickPeriodSet(SysCtlClockGet() / ulTickPeriod);

    SysTickEnable();
    SysTickIntEnable();

    /* Note: SysTick Int Handler must be set in startup_gcc.c */
}

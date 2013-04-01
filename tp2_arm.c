#include "inc/hw_types.h" // tBoolean type
#include "inc/hw_memmap.h" // GPIO_PORTA_BASE,...
#include "driverlib/sysctl.h" // SYSCTL_*
#include "driverlib/gpio.h" // GPIOPinConfigure,GPIO_PIN_3,....

//Timer Interrupts
#include "inc/hw_ints.h" // NVIC interrupt assignment macros
#include "driverlib/interrupt.h" // IntEnable, IntPrioritySet,...
#include "driverlib/timer.h" // TimerConfigure, TimerLoadSet,...

// Bottome Half Proccesing
#include "taskq.h"

// uint8_t, size_t, bool, etc...
#include "stdtypes.h"

// vSPIDACInit, vSPIDACWrite, vSPIDACUpdateRoutine
#include "spidac.h"

#include "uartcomm.h"

#define DELAY_MS_(MS) SysCtlDelay(SysCtlClockGet()/(1000 * MS * 3))
#define DELAY_US_(US) SysCtlDelay(SysCtlClockGet()/(US * 3))

const uint16_t sinetable[256] = {2048, 1998, 1948, 1897, 1847, 1797,
  1747, 1698, 1648, 1599, 1550, 1502, 1453, 1406, 1358, 1311, 1264, 1218,
  1172, 1127, 1083, 1039, 995, 952, 910, 869, 828, 788, 749, 710, 673, 636,
  600, 565, 531, 497, 465, 433, 403, 374, 345, 318, 291, 266, 242, 219, 197,
  176, 156, 137, 120, 103, 88, 74, 61, 50, 39, 30, 22, 15, 10, 6, 2, 1, 0,
  1, 2, 6, 10, 15, 22, 30, 39, 50, 61, 74, 88, 103, 120, 137, 156, 176, 197,
  219, 242, 266, 291, 318, 345, 374, 403, 433, 465, 497, 531, 565, 600, 636,
  673, 710, 749, 788, 828, 869, 910, 952, 995, 1039, 1083, 1127, 1172, 1218,
  1264, 1311, 1358, 1406, 1453, 1502, 1550, 1599, 1648, 1698, 1747, 1797, 1847,
  1897, 1948, 1998, 2048, 2098, 2148, 2199, 2249, 2299, 2349, 2398, 2448, 2497,
  2546, 2594, 2643, 2690, 2738, 2785, 2832, 2878, 2924, 2969, 3013, 3057, 3101,
  3144, 3186, 3227, 3268, 3308, 3347, 3386, 3423, 3460, 3496, 3531, 3565,
  3599, 3631, 3663, 3693, 3722, 3751, 3778, 3805, 3830, 3854, 3877, 3899,
  3920, 3940, 3959, 3976, 3993, 4008, 4022, 4035, 4046, 4057, 4066, 4074,
  4081, 4086, 4090, 4094, 4095, 4095, 4095, 4094, 4090, 4086, 4081, 4074,
  4066, 4057, 4046, 4035, 4022, 4008, 3993, 3976, 3959, 3940, 3920, 3899,
  3877, 3854, 3830, 3805, 3778, 3751, 3722, 3693, 3663, 3631, 3599, 3565,
  3531, 3496, 3460, 3423, 3386, 3347, 3308, 3268, 3227, 3186, 3144, 3101,
  3057, 3013, 2969, 2924, 2878, 2832, 2785, 2738, 2690, 2643, 2594, 2546,
  2497, 2448, 2398, 2349, 2299, 2249, 2199, 2148, 2098};


int main(void) {

    //
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    vSPIDACInit();
    vUARTCommInit();

    IntMasterEnable(); // avr sei() equivalent


    vUARTCommSendByte('\n');
    vUARTCommSendStream("\r\n", 2);
    vUARTCommSendString("Hello World!\r\n");

    while(1)
    {
        vTaskQRun();
        SysCtlDelay(1000);
    }

}

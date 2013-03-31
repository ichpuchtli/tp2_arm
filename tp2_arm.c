#include "inc/hw_types.h" // tBoolean type
#include "inc/hw_memmap.h" // GPIO_PORTA_BASE,...
#include "driverlib/sysctl.h" // SYSCTL_*
#include "driverlib/gpio.h" // GPIOPinConfigure,GPIO_PIN_3,....

// DAC SPI
#include "driverlib/ssi.h" // SSIDataPut,SSIBusy,...

//Timer Interrupts
#include "inc/hw_ints.h" // NVIC interrupt assignment macros
#include "driverlib/interrupt.h" // IntEnable, IntPrioritySet,...
#include "driverlib/timer.h" // TimerConfigure, TimerLoadSet,...

// Bottome Half Proccesing
#include "taskq.h"

#define DACSPI_FREQ 44100 // 44.1 kHz

void vSPIDACInit(void);
void vSPIDACWrite(unsigned long usData);
void vSPIDACUpdateRoutine(void);

const unsigned long sinetable[256] = {2048, 1998, 1948, 1897, 1847, 1797,
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

void task_shell(void* data){

    int arg = (int) data;

    // do something time consuming
}

int main(void) {

    //
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    vSPIDACInit();

    IntMasterEnable(); // avr sei() equivalent

    while(1)
    {
        vTaskQRun();
        SysCtlDelay(100);
    }

}


void vSPIDACInit(void){

    /******************************************************************************/
    // SPI Interface Configuration

    // Enable Peripheral Bus
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Connect Pin Functionality GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);

    //Configure Pins for use by SSI Peripheral Bus
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 );
  
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
            SSI_MODE_MASTER, SysCtlClockGet()/2, 16);

    SSIEnable(SSI0_BASE);

    /******************************************************************************/
    //Timer/Interrupt Configuration

    // Calculate timer period from DACSPI_FREQ
    unsigned long ulPeriod = SysCtlClockGet() / DACSPI_FREQ - 1;

    // Configure Timer
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ulPeriod);

    // Configure Interrupt
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Start Timer!
    TimerEnable(TIMER0_BASE, TIMER_A);

}

void vSPIDACWrite(unsigned long usData){

 /*
  * bit 15 A/B: DACa or DACb Selection Bit
  *  1 = Write to DACb
  *  0 = Write to DACa
  * 
  *  bit 14 BUF: Vref Input Buffer Control Bit
  *  1 = Buffered
  *  0 = Unbuffered
  * 
  *  bit 13 GA: Output Gain Selection Bit
  *  1 = 1x(Vout = Vref * D/4096)
  *  1 = 2x(Vout = 2 * Vref * D/4096)
  * 
  *  bit 12 ~SHDN: Output Shutdown Control Bit
  *  1 = Active Moder Operation Vout is available
  *  0 = Shutdown the selected DAC channel
  *
  */
  unsigned long usCommand = (0 << 15) | (0 << 14) | (1 << 13) | (1 << 12);

  SSIDataPut(SSI0_BASE, usCommand | (usData & 4095) );

  while(SSIBusy(SSI0_BASE)) continue;

}

// DAC Update Interrupt Handler
// Occurs every 1/DACSPI_FREQ seconds
void vSPIDACUpdateRoutine(void){

    static unsigned long i = 0;

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    
    vSPIDACWrite(sinetable[i++ & 255]);

}

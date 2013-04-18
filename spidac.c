#include "inc/hw_types.h" // tBoolean type
#include "inc/hw_memmap.h" // GPIO_PORTA_BASE,...
#include "driverlib/sysctl.h" // SYSCTL_*
#include "driverlib/gpio.h" // GPIOPinConfigure,GPIO_PIN_3,....

//Timer Interrupts
#include "inc/hw_ints.h" // NVIC interrupt assignment macros
#include "driverlib/interrupt.h" // IntEnable, IntPrioritySet,...
#include "driverlib/timer.h" // TimerConfigure, TimerLoadSet,...

#include "driverlib/ssi.h" // SSIDataPut,SSIBusy,...

#include "spidac.h"


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
    uint32_t ulPeriod = SysCtlClockGet() / DACSPI_FREQ - 1;

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

void vSPIDACWrite(uint16_t usData){

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
  uint16_t usCommand = (0 << 15) | (0 << 14) | (1 << 13) | (1 << 12);

  SSIDataPut(SSI0_BASE, usCommand | (usData & 4095) );

  while(SSIBusy(SSI0_BASE)) continue;

}

// DAC Update Interrupt Handler
// Occurs every 1/DACSPI_FREQ seconds
void vSPIDACIntHandler(void){

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    
    (void) vSPIDACUpdateEvent();
}

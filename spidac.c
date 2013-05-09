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

#include "talloc.h"

void vSPIDACInit(void){

    /******************************************************************************/
    // SPI Interface Configuration

    // Enable Peripheral Bus
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    // Connect Pin Functionality
    GPIOPinConfigure(GPIO_PD1_SSI3FSS);
    GPIOPinConfigure(GPIO_PD0_SSI3CLK);
    GPIOPinConfigure(GPIO_PD3_SSI3TX);

    //Configure Pins for use by SSI Peripheral Bus
    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_3 );
  
    SSIConfigSetExpClk(SSI3_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
            SSI_MODE_MASTER, SysCtlClockGet()/2, 16);

    SSIEnable(SSI3_BASE);

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

  SSIDataPut(SSI3_BASE, usCommand | (usData & 4095) );

  while(SSIBusy(SSI3_BASE)) continue;

}

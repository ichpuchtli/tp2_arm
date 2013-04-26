
//----------------------------------------------------------------------------
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/ssi.h"

#include "sdcard.h"

static FATFS xFatFileSystem;
// Initialises the SD card using SSI0, with a clock of 1MHz. Note: must use

// SSI0, otherwise fatfs drivers must be changed.
//----------------------------------------------------------------------------
void vSDCardInit(void){

    // Set-up the SPI
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable CS
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);

    // Enable the SPI pins
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3);

    // Set up the clk for the SPI
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), 0x00000000,  0x00000000, 1000000, 16);

    // Enable the SPI
    SSIEnable(SSI0_BASE);

    f_mount(0, &xFatFileSystem);
}

void vOpenWavFile(uint8_t num){}
size_t vReadWavFile(uint8_t num, uint16_t* buffer, size_t count){}
void vCloseWavFile(uint8_t num){}

//----------------------------------------------------------------------------
// Includes
//----------------------------------------------------------------------------
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "diskio.h"
#include "ff.h"
#include "driverlib/rom.h"
#include "sd.h"
#include "driverlib/ssi.h"


//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
static FATFS fatSystem;
static FIL fileSystem;
char readBuff[4097];


//------------------------------------------------------------------------------
// sd_read
//
// Reads a given number of bytes from a given file and places it into
// readBuff. Note: can only read a max
// of 4096 bytes.
//
// fileName: The name of the file to be read.
// size: THe number of bytes to be read from the file.
//
// return: The bytes read from the file.
//------------------------------------------------------------------------------
char *sd_read(char *fileName, WORD size)
{
	WORD br;

	f_open(&fileSystem, fileName, FA_READ);
	f_read(&fileSystem, readBuff, size, &br);
	f_close(&fileSystem);
	readBuff[size] = '\0';

	return readBuff;
}


//------------------------------------------------------------------------------
// sd_ReadWav
//
// Opens a wav file a reads from a specific (given location).
//
// fileName: The name of the file to be read.
// size: The amount of data to be read.
// location: The location to begin reading from.
//------------------------------------------------------------------------------
char *sd_ReadWav(char* fileName, WORD size, DWORD location)
{
	WORD br;

	f_open(&fileSystem, fileName, FA_READ);
	f_lseek(&fileSystem, location);
	f_read(&fileSystem, readBuff, size, &br);
	f_close(&fileSystem);
	readBuff[size] = '\0';

	return readBuff;
}

//----------------------------------------------------------------------------
// sd_write
//
// Creates a new file on the sd card and writes the given data to that file.
//
// fileName: The name of the file to be created and written to.
// data: An array containing the data to be written to the file.
// size: The size of the data to be written to the file.
//----------------------------------------------------------------------------
void sd_write(char *fileName, char *data, WORD size){

    WORD bw;

    f_open(&fileSystem, fileName, FA_CREATE_ALWAYS | FA_WRITE);
    f_write(&fileSystem, data, size, &bw);

    // Close the file so the data is flushed onto the SD card
    f_close(&fileSystem);
}


//----------------------------------------------------------------------------
// sd_create
//
// Initialises the SD card using SSI0, with a clock of 1MHz. Note: must use
// SSI0, otherwise fatfs drivers must be changed.
//----------------------------------------------------------------------------
void sd_create(void)
{
	// Set-up the SPI
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	// enable cs
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

	f_mount(0, &fatSystem);
}

#include "ledmatrix.h"
#include "rgbledctrl.h"

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define GND_PERIPH    SYSCTL_PERIPH_GPIOB
#define GND_PORT      GPIO_PORTB_BASE
#define GND_PIN0      GPIO_PIN_0
#define GND_PIN1      GPIO_PIN_1
#define GND_PIN2      GPIO_PIN_2
#define GND_PIN3      GPIO_PIN_3

/* MUX 4:1 00->Row0, 01->Row1, etc.. */
#define SRC_PERIPH    SYSCTL_PERIPH_GPIOF
#define SRC_PORT      GPIO_PORTF_BASE
#define SRC_PIN0      GPIO_PIN_0
#define SRC_PIN1      GPIO_PIN_4

struct rgb24_t g_pxLEDMatrix[LEDMATRIX_AREA];

//////////////////////////////////////////////////////////////////////////////
// Private Functions
//////////////////////////////////////////////////////////////////////////////

static void inline vLEDMatrixPointPWM(uint8_t ucX, uint8_t ucY){

    GPIOPinTypeGPIOInput(GND_PORT, GND_PIN0 | GND_PIN1 | GND_PIN2 | GND_PIN3);

    GPIOPinWrite(SRC_PORT, SRC_PIN0 | SRC_PIN1, ucY );

    /* Open Drain Outputs */
    GPIOPinTypeGPIOOutputOD(GND_PORT, 1 << ucX);

    GPIOPinWrite(GND_PORT, 1 << ucX, 0 << ucX);

}

static void inline vLEDMatrixSetColor(uint8_t* pucRGB){

    RGBColorSet(pucRGB);
}

////////////////////////////////////////////////////////////////////////////////
// Public Functions
//////////////////////////////////////////////////////////////////////////////

void vLEDMatrixInit(void){

    SysCtlPeripheralEnable(GND_PERIPH);

    GPIOPinTypeGPIOInput(GND_PORT, GND_PIN0 | GND_PIN1 | GND_PIN2 | GND_PIN3);

    SysCtlPeripheralEnable(SRC_PERIPH);

    GPIOPinTypeGPIOOutput(SRC_PORT, SRC_PIN0 | SRC_PIN1 );

    /* Init & Enable */
    RGBInit(1UL);
}

void inline vLEDMatrixSetPixelN(uint8_t ucNum, uint8_t* pucRGB){
    
    // assert(num < LEDMATRIX_AREA);
    // assert(rgb != NULL);
    
    g_pxLEDMatrix[ucNum].red = pucRGB[0];
    g_pxLEDMatrix[ucNum].green = pucRGB[1];
    g_pxLEDMatrix[ucNum].blue = pucRGB[2];

}

void inline vLEDMatrixClearPixelN(uint8_t ucNum){

    // assert(num < LEDMATRIX_AREA);
    g_pxLEDMatrix[ucNum].red = 0;
    g_pxLEDMatrix[ucNum].green = 0;
    g_pxLEDMatrix[ucNum].blue = 0;
}

void vLEDMatrixMultiplexer_Event(void){

    static volatile uint8_t ucPosition = 0x0;

    vLEDMatrixSetColor( (uint8_t*) (g_pxLEDMatrix + ucPosition) );

    vLEDMatrixPointPWM( (ucPosition & 3) - 1,  ucPosition / 4 );

    ucPosition = (ucPosition + 1) & (LEDMATRIX_AREA - 1);
}

void inline vLEDMatrixSetPixel(uint8_t ucX, uint8_t ucY, uint8_t* pucRGB){

    vLEDMatrixSetPixelN(ucX + LEDMATRIX_SIZEY*ucY, pucRGB);
}

void inline vLEDMatrixClearPixel(uint8_t ucX, uint8_t ucY){

    vLEDMatrixClearPixelN(ucX + LEDMATRIX_SIZEY*ucY );
}

void inline vLEDMatrixClearAll(void){

    for(uint8_t i = 0; i < LEDMATRIX_AREA; i++){

        vLEDMatrixClearPixelN(i);
    }

}


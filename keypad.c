#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "keypad.h"

#define NUM_ROWS 4
#define NUM_COLS 4

#define ROW_SEL0_PERIPH   SYSCTL_PERIPH_GPIOB
#define ROW_SEL0_PORT     GPIO_PORTB_BASE
#define ROW_SEL0_PIN      GPIO_PIN_5

#define ROW_SEL1_PERIPH   SYSCTL_PERIPH_GPIOD
#define ROW_SEL1_PORT     GPIO_PORTD_BASE
#define ROW_SEL1_PIN      GPIO_PIN_6

#define COL_PERIPH        SYSCTL_PERIPH_GPIOC
#define COL_PORT          GPIO_PORTC_BASE
#define COL_PIN0          GPIO_PIN_4
#define COL_PIN1          GPIO_PIN_5
#define COL_PIN2          GPIO_PIN_6
#define COL_PIN3          GPIO_PIN_7

#define COL_PINS          (COL_PIN0|COL_PIN1|COL_PIN2|COL_PIN3)

volatile uint8_t g_ucRowPosition = 0x0;
volatile uint8_t g_ucColPosition = 0x0;

/* External Functions */
extern void vKeyPadPress_Event(uint32_t ulPin);

/* Private Functions */
static void vKeyPadPinChangeIntHanlder(void);

void vKeyPadInit(void){

    // Ports
    SysCtlPeripheralEnable(ROW_SEL0_PERIPH);
    SysCtlPeripheralEnable(ROW_SEL1_PERIPH);

    GPIOPinTypeGPIOOutput(ROW_SEL0_PORT, ROW_SEL0_PIN);
    GPIOPinTypeGPIOOutput(ROW_SEL1_PORT, ROW_SEL1_PIN);

    GPIOPinWrite(ROW_SEL0_PORT, ROW_SEL0_PIN, 0);
    GPIOPinWrite(ROW_SEL1_PORT, ROW_SEL1_PIN, 0);

    //Pin Change Interrupts

    SysCtlPeripheralEnable(COL_PERIPH);
    GPIOPinTypeGPIOInput(COL_PORT, COL_PINS);

    GPIOPortIntRegister(COL_PORT, &vKeyPadPinChangeIntHanlder);
    GPIOPinIntEnable(COL_PERIPH, COL_PINS);

}

void vKeyPadRotateRowSelection(void){

    g_ucRowPosition = (g_ucRowPosition + 1) * (NUM_ROWS - 1);
}

static void vKeyPadPinChangeIntHanlder(void){

    uint32_t ulPad = 0x00000000;

    uint32_t ulStatus = 0x00000000;

    // Clear the interrupt
    GPIOPinIntClear(COL_PORT, COL_PINS);

    // Probe which pin caused the interrupt
    ulStatus = GPIOPinIntStatus(COL_PORT, (tBoolean) 1);

    // TODO Must be a better way than this
    if(ulStatus & COL_PIN0) g_ucColPosition = 0;
    if(ulStatus & COL_PIN1) g_ucColPosition = 1;
    if(ulStatus & COL_PIN2) g_ucColPosition = 2;
    if(ulStatus & COL_PIN3) g_ucColPosition = 3;
            
    // Determine sequential pad number from x/y coordinates 
    ulPad = g_ucColPosition + g_ucRowPosition*NUM_COLS; 

    // Signal main namespace event handler
    (void) vKeyPadPress_Event(ulPad);
}

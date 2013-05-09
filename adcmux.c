#include "adcmux.h"

#include "driverlib/adc.h"

#define ADCMUX_NUM_SOURCES 8

enum {

    EFFECT_POT,
    LFO_POT,

};

// Stores the latest value of each possible analog source
static uint16_t g_psADCValueCache[ADCMUX_NUM_SOURCES];


void vADCMuxInit(void){


    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    SysCtlADCSpeedSet(SYSCTL_ADCSPEED_250KPS);

    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    //
    // Select the analog ADC function for these pins.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_7);

    //
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 3.
    //
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // information on the ADC sequences and steps, reference the datasheet.
    //
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE |
                             ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    //
    ADCSequenceEnable(ADC0_BASE, 3);

    //
    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    //
    ADCIntClear(ADC0_BASE, 3);

}

static void vADCMuxSelect(uint8_t ucChannel){
    g_ucADCMuxChannel = ucChannel;
}

static void vADCMuxTrigger(void){

    ADCProcessorTrigger(ADC0_BASE, 3);
}

// Synchronous ADC Read
uint32_t ulADCMuxGetValue(void){
    
    //
    // This array is used for storing the data read from the ADC FIFO. It
    // must be as large as the FIFO for the sequencer in use.  This example
    // uses sequence 3 which has a FIFO depth of 1.  If another sequence
    // was used with a deeper FIFO, then the array size must be changed.
    //
    unsigned long ulADC0_Value[1];

    // Wait for conversion to be completed.
    while(!ADCIntStatus(ADC0_BASE, 3, false))
    {
    }

    // Clear the ADC interrupt flag.
    ADCIntClear(ADC0_BASE, 3);

    // Read ADC Value.
    ADCSequenceDataGet(ADC0_BASE, 3, ulADC0_Value);

    return ulADC0_Value[0];
}

// Samler Hook, this function needs to tied a timer interrupt
// the frequency of the timer interrupt is the update frequency
// of a single value in the cache
//
// ---------------------------------------
// | EFFECT_POT1 | LFO_POT | EFFECT_POT2 |
// ---------------------------------------
// |     128     |    38   |       0     |
// ---------------------------------------
//
void vADCMuxSampler_Hook(void){

    static uint8_t index = 0;

    // Wait for ADC conversion then update the cache 
    g_psADCValueCache[index] = (uint16_t) ulADCMuxGetValue();

    // Advance array index
    index = (index + 1) & (ADCMUX_NUM_SOURCES - 1);

    // TODO: Switch Mux Selectors

    // Select the next ADC source and trigger a conversion
    vADCMuxSelect(index);
    vADCMuxTrigger();
}

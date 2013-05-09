#include "adcmux.h"

#include "driverlib/adc.h"

#define ADCMUX_NUM_SOURCES 5

// Stores the latest value of each possible analog source
static uint16_t g_psADCValueCache[ADCMUX_NUM_SOURCES];

static void vADCMuxSelect(uint8_t channel){

  // Clear All Select Pins
  GPIOPinWrite(ADCMUX_SEL0_PORT, ADCMUX_SEL0_PIN, 0);
  GPIOPinWrite(ADCMUX_SEL1_PORT, ADCMUX_SEL1_PIN, 0);
  GPIOPinWrite(ADCMUX_SEL2_PORT, ADCMUX_SEL2_PIN, 0);

  switch(channel){

    // 000
    case EFFECT_POT0:
      break;

    // 001
    case EFFECT_POT1:
      GPIOPinWrite(ADCMUX_SEL0_PORT, ADCMUX_SEL0_PIN, 0xFF);
      break;

    // 010
    case EFFECT_POT2:
      GPIOPinWrite(ADCMUX_SEL1_PORT, ADCMUX_SEL1_PIN, 0xFF);
      break;

    // 011
    case EFFECT_POT3:
      GPIOPinWrite(ADCMUX_SEL0_PORT, ADCMUX_SEL0_PIN, 0xFF);
      GPIOPinWrite(ADCMUX_SEL1_PORT, ADCMUX_SEL1_PIN, 0xFF);
      break;

    // 100
    case LFO_POT:
      GPIOPinWrite(ADCMUX_SEL3_PORT, ADCMUX_SEL1_PIN, 0);
      break;

  }

}

void vADCMuxInit(void){

    // Initialize Select Ports
    SysCtlPeripheralEnable(ADCMUX_SEL0_PERIPH);
    GPIOPinTypeGPIOOutput(ADCMUX_SEL0_PORT, ADCMUX_SEL0_PIN);

    SysCtlPeripheralEnable(ADCMUX_SEL1_PERIPH);
    GPIOPinTypeGPIOOutput(ADCMUX_SEL1_PORT, ADCMUX_SEL1_PIN);

    SysCtlPeripheralEnable(ADCMUX_SEL2_PERIPH);
    GPIOPinTypeGPIOOutput(ADCMUX_SEL2_PORT, ADCMUX_SEL2_PIN);

    // Initialize Analog Input Port
    SysCtlPeripheralEnable(ADCMUX_ADC_MOD);

    SysCtlADCSpeedSet(SYSCTL_ADCSPEED_250KPS);

    GPIOPinTypeADC(ADCMUX_PORT, ADCMUX_PIN);

    //
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 3.
    //

    // Software Triggered, Low Priority
    ADCSequenceConfigure(ADCMUX_ADC_BASE, ADCMUX_SEQ_NUM, ADC_TRIGGER_PROCESSOR, 3);

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
    ADCSequenceStepConfigure(ADCMUX_ADC_BASE, ADCMUX_SEQ_NUM, 0, ADC_CTL_CH8 | ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    // ----------------------------------------
    // | Sequencer | No. Samples | FIFO Depth |
    // ----------------------------------------
    // | SS3*      | 1           | 1          |
    // ----------------------------------------
    // | SS2       | 4           | 4          |
    // ----------------------------------------
    // | SS1       | 4           | 4          |
    // ----------------------------------------
    // | SS0       | 8           | 8          |
    // ----------------------------------------

    ADCSequenceEnable(ADCMUX_ADC_BASE, ADCMUX_SEQ_NUM);

    //
    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    //
    //ADCIntClear(ADCMUX_ADC_BASE, ADCMUX_SEQ_NUM);
    //
    ADCProcessorTrigger(ADCMUX_ADC_BASE, 3);
}

// Synchronous ADC Read
uint32_t ulADCMuxGetValue(void){
    
    // This array is used for storing the data read from the ADC FIFO. It
    // must be as large as the FIFO for the sequencer in use.  This example
    // uses sequence 3 which has a FIFO depth of 1.  If another sequence
    // was used with a deeper FIFO, then the array size must be changed.
    //
    unsigned long ulADC0_Value[1];

    // Wait for conversion to be completed
    // 4us @ 250Ksps
    while(!ADCIntStatus(ADCMUX_ADC_BASE, 3, false))
    {
    }

    // Clear the ADC interrupt flag.
    ADCIntClear(ADCMUX_ADC_BASE, 3);

    // Read ADC Value.
    ADCSequenceDataGet(ADCMUX_ADC_BASE, 3, ulADC0_Value);

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
    index = (index + 1) & (ADCMUX_NUM_SOURCES - 1);/

    vADCMuxSelect(index);

    // Select the next ADC source and trigger a conversion
    ADCProcessorTrigger(ADCMUX_ADC_BASE, 3);
}

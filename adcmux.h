#ifndef _ADC_MUX_H_
#define _ADC_MUX_H_

#include "stdint.h"

#define ADCMUX_NUM_SOURCES         8

#define ADCMUX_ADC_MODULE          SYSCTL_PERIPH_ADC0
#define ADCMUX_ADC_BASE            ADCMUX_ADC_BASE
#define ADCMUX_SEQ_NUM             3

#define ADCMUX_ANALOG_PERIPH       SYSCTL_PERIPH_GPIOE
#define ADCMUX_ANALOG_PORT         GPIO_PORTE_BASE
#define ADCMUX_ANALOG_PIN          GPIO_PIN_5

#define ADCMUX_SEL0_PERIPH         SYSCTL_PERIPH_GPIOD
#define ADCMUX_SEL0_PORT           GPIO_PORTD_BASE
#define ADCMUX_SEL0_PIN            GPIO_PIN_2

#define ADCMUX_SEL1_PERIPH         SYSCTL_PERIPH_GPIOD
#define ADCMUX_SEL1_PORT           GPIO_PORTD_BASE
#define ADCMUX_SEL1_PIN            GPIO_PIN_3

#define ADCMUX_SEL3_PERIPH         SYSCTL_PERIPH_GPIOA
#define ADCMUX_SEL3_PORT           GPIO_PORTA_BASE
#define ADCMUX_SEL3_PIN            GPIO_PIN_6

// 1.06 PE5 Pin60 AIN8 U5Tx I2C2SDA T1CCP0 CAN0Rx

enum {

    EFFECT_POT0,
    EFFECT_POT1,
    EFFECT_POT2,
    EFFECT_POT3,
    LFO_POT
};

void vADCMuxInit(void);

void vADCMuxSampler_Hook(void);

void uint16_t usADCMuxGetCachedSample(uint8_t num);

#endif
